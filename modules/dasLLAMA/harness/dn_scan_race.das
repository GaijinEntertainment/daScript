options gen2
options _dasllama_internal = true

// THE GATED DELTA SCAN - the prefill's recurrent walk at the Qwen3.6-35B-A3B geometry (16 key heads,
// 32 value heads of 128, a 2048-token window), thirty dispatches a command buffer the way a window
// carries them: the f32-row stamp with the gates computed in the walk against the half-row stamp
// over prepared gates, each stamp at four and at two simdgroups a threadgroup.
//
//   bin/daslang -jit modules/dasLLAMA/harness/dn_scan_race.das

require metal/msl_shader
require metal/das_metal_boost
require ?das_metal das_metal
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_kernels
require _race_fixture

let NPOS = 2048
let NKH = 16
let NVH = 32
let DS = 128
let KD = NKH * DS
let DI = NVH * DS
let CD = 2 * KD + DI
let LAYERS = 30

//! the scan over every layer of one stamp, the state pool bound again as the capture arena (a group table carrying no capture reads nothing from it)
def enc_layers(enc : MetalComputeEncoder?; pso : MetalComputePipeline?; cv, st, gates, aa, o, stm, grp : MetalBuffer?; var ka : DnArgs; grid, tg : uint3) {
    for (_l in range(LAYERS)) {
        kn_pipeline(enc, pso)
        kn_buffer(enc, cv, 0ul, 0)
        kn_buffer(enc, st, 0ul, 1)
        kn_buffer(enc, gates, 0ul, 2)
        kn_buffer(enc, gates, 0ul, 3)
        kn_buffer(enc, aa, 0ul, 4)
        kn_buffer(enc, aa, 0ul, 5)
        kn_buffer(enc, o, 0ul, 6)
        kn_buffer(enc, stm, 0ul, 7)
        kn_buffer(enc, grp, 0ul, 8)
        kn_kargs(enc, ka, 9)
        kn_buffer(enc, st, 0ul, 10)
        kn_dispatch(enc, grid, tg)
    }
}

[export]
def main {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var dev = metal_create_system_default_device()
        var queue = metal_new_command_queue(dev)
        var err = ""
        var p_f = pipeline_from_source(dev, MetalDnScan_metal_dn_scan_msl, MetalDnScan_metal_dn_scan_msl_entry, MetalDnScan_metal_dn_scan_msl_fastmath, err)
        var p_h = pipeline_from_source(dev, MetalDnScanH_metal_dn_scan_msl, MetalDnScanH_metal_dn_scan_msl_entry, MetalDnScanH_metal_dn_scan_msl_fastmath, err)
        if (p_f == null || p_h == null) {
            print("pso: {err}\n")
            return
        }
        var cvf <- race_fill(NPOS * CD, 1, 0.18, 0.0)
        var cvh <- [for (v in cvf); float16(v)]
        var raw <- race_fill(NPOS * NVH, 2, 2.0, 0.0)
        var prep <- race_fill(NPOS * NVH, 3, 0.1, 0.9)
        var bcv = metal_buffer_from(dev, uint64(NPOS * CD * 4), unsafe(addr<void?>(cvf[0])))
        var bcvh = metal_buffer_from(dev, uint64(NPOS * CD * 2), unsafe(addr<void?>(cvh[0])))
        var bst = metal_buffer_from(dev, uint64(NVH * DS * DS * 4), null)
        var bstm = metal_buffer_from(dev, uint64(NVH * DS * DS * 4), null)
        var braw = metal_buffer_from(dev, uint64(NPOS * NVH * 4), unsafe(addr<void?>(raw[0])))
        var bprep = metal_buffer_from(dev, uint64(NPOS * NVH * 4), unsafe(addr<void?>(prep[0])))
        var baa = metal_buffer_from(dev, uint64(NVH * 4), unsafe(addr<void?>(raw[0])))
        var bo = metal_buffer_from(dev, uint64(NPOS * DI * 4), null)
        var ka = DnArgs(cd = uint(CD), dconv = 4u, npos = uint(NPOS), kd = uint(KD), ds = uint(DS), nvh = uint(NVH),
            nkh = uint(NKH), di = uint(DI), eps = 1e-6, qscale = 1.0, mid = 0u, split = 0u, row = 0u, bstr = uint(NVH))
        var grp <- [0u, uint(NPOS), 0u, 0u, 0u, 0u, 0u, 0u]
        var bgrp = metal_buffer_from(dev, 32ul, unsafe(addr<void?>(grp[0])))
        for (sgs in [4, 2, 1]) {
            var t_f = -1.0lf
            var t_h = -1.0lf
            let grid = uint3(uint(DS / 8 / sgs), uint(NVH), 1u)
            let tg = uint3(uint(32 * sgs), 1u, 1u)
            race_pair_ms(queue, 3, t_f, t_h, $(enc) {
                enc_layers(enc, p_f, bcv, bst, braw, baa, bo, bstm, bgrp, ka, grid, tg)
            }, $(enc) {
                enc_layers(enc, p_h, bcvh, bst, bprep, baa, bo, bstm, bgrp, ka, grid, tg)
            })
            print("scan {NPOS} tokens, {sgs} simdgroups a threadgroup: f32 rows + gates in the walk {t_f / double(LAYERS)} ms a layer, half rows + prepared gates {t_h / double(LAYERS)} ms\n")
        }
        delete cvf
        delete cvh
        delete raw
        delete prep
        delete grp
        for (b in [bcv, bcvh, bst, bstm, braw, bprep, baa, bo, bgrp]) {
            metal_release(b)
        }
        metal_release(p_f)
        metal_release(p_h)
    } else {
        print("dn_scan_race: no das_metal in this build\n")
    }
}
