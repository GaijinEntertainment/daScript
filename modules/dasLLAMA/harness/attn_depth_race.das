options gen2
options _dasllama_internal = true

// ATTENTION AT DEPTH - the prefill attention trio (QK scores, row softmax, AV) raced against the
// device pair at one deep window: 2048 queries landing on 8192 rows of context, the
// Qwen3.6-35B-A3B geometry (16 heads of 256 over 2 KV heads). The staged trio stages K, P and
// V through threadgroup memory a tile at a time; the device pair hands matmul2d device tensors of
// half on both sides - a scaled K twin, the slab holding exp(s - max) in place, a V twin - and
// scales each output row by its reciprocal sum. Both arms dispatch the shipped stamps through
// their generated builders.
//
//   bin/daslang -jit modules/dasLLAMA/harness/attn_depth_race.das

require math
require strings
require metal/msl_shader
require metal/das_metal_boost
require ?das_metal das_metal
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_prefill
require _race_fixture

let HEADS = 16
let HS = 256
let KV_MUL = 8
let QD = HEADS * HS
let KVD = (HEADS / KV_MUL) * HS
let MP = 2048
let QOFF = 8192
let NK = QOFF + MP

//! the attention output at a handful of (row, head, column) points from the f32 operands in
//! double, and each buffer's largest distance from it
def private ref_report(qf, kf, vf : array<float>; scale : float; bufs : array<tuple<name : string; buf : MetalBuffer?>>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var worst <- [for (_b in bufs); 0.0lf]
        var mag = 0.0lf
        var sc : array<double>
        sc |> resize(NK)
        for (r in [0, 777, MP - 1]) {
            for (h in [0, 9, HEADS - 1]) {
                let cnt = QOFF + r + 1
                let kvo = (h / KV_MUL) * HS
                var mx = -1.0e300lf
                for (j in range(cnt)) {
                    var dot = 0.0lf
                    for (d in range(HS)) {
                        dot += double(qf[r * QD + h * HS + d]) * double(kf[j * KVD + kvo + d])
                    }
                    sc[j] = dot * double(scale)
                    mx = max(mx, sc[j])
                }
                var tot = 0.0lf
                for (j in range(cnt)) {
                    sc[j] = exp(sc[j] - mx)
                    tot += sc[j]
                }
                for (d in [0, 100, HS - 1]) {
                    var want = 0.0lf
                    for (j in range(cnt)) {
                        want += sc[j] * double(vf[j * KVD + kvo + d])
                    }
                    want /= tot
                    mag = max(mag, abs(want))
                    for (bi, b in count(), bufs) {
                        var got = 0.0
                        unsafe {
                            got = reinterpret<float?>(metal_buffer_contents(b.buf))[r * QD + h * HS + d]
                        }
                        worst[bi] = max(worst[bi], abs(double(got) - want))
                    }
                }
            }
        }
        for (b, w in bufs, worst) {
            print("reference (f32 operands, double): {b.name} off by at most {w} of max {mag}\n")
        }
        delete worst
        delete sc
    }
}

def private half_of(src : array<float>; scale : float) : array<float16> {
    var xa : array<float16>
    xa |> resize(length(src))
    for (d, s in xa, src) {
        d = float16(s * scale)
    }
    return <- xa
}

[export]
def main {   // nolint:STYLE038 — one-shot race harness; buffers + PSOs stay live to the release tail
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var dev = metal_create_system_default_device()
        var queue = metal_new_command_queue(dev)
        var err = ""
        var qk_p = pipeline_from_source(dev, MetalAttnQKMmTH_metal_attn_qk_mm_t_msl, MetalAttnQKMmTH_metal_attn_qk_mm_t_msl_entry, MetalAttnQKMmTH_metal_attn_qk_mm_t_msl_fastmath, err)
        var rs_p = pipeline_from_source(dev, metal_attn_rowstat_msl, metal_attn_rowstat_msl_entry, metal_attn_rowstat_msl_fastmath, err)
        var av_p = pipeline_from_source(dev, MetalAttnAVMmP_metal_attn_av_mm_msl, MetalAttnAVMmP_metal_attn_av_mm_msl_entry, MetalAttnAVMmP_metal_attn_av_mm_msl_fastmath, err)
        var tw_d = pipeline_from_source(dev, metal_kv_twin_msl, metal_kv_twin_msl_entry, metal_kv_twin_msl_fastmath, err)
        var qk_d = pipeline_from_source(dev, MetalAttnQKDev128_metal_attn_qk_dev_msl, MetalAttnQKDev128_metal_attn_qk_dev_msl_entry, MetalAttnQKDev128_metal_attn_qk_dev_msl_fastmath, err)
        var px_d = pipeline_from_source(dev, metal_attn_pexp_msl, metal_attn_pexp_msl_entry, metal_attn_pexp_msl_fastmath, err)
        var av_d = pipeline_from_source(dev, MetalAttnAVDev128_metal_attn_av_dev_msl, MetalAttnAVDev128_metal_attn_av_dev_msl_entry, MetalAttnAVDev128_metal_attn_av_dev_msl_fastmath, err)
        if (qk_p == null || rs_p == null || av_p == null || tw_d == null || qk_d == null || px_d == null || av_d == null) {
            print("pso: {err}\n")
            return
        }
        let scale = 1.0 / sqrt(float(HS))
        //! `-- <amp>` widens Q so a row's scores spread the way a trained head's do (default 1: a flat row)
        let args <- get_command_line_arguments()
        let amp = !empty(args) && to_float(args[length(args) - 1]) > 0.0 ? to_float(args[length(args) - 1]) : 1.0
        var qf <- race_fill(MP * QD, 1, 0.5, 0.0, 3u)
        for (x in qf) {
            x *= amp
        }
        print("Q amplitude x{amp}\n")
        var kf <- race_fill(NK * KVD, 2, 0.5, 0.0, 5u)
        var vf <- race_fill(NK * KVD, 3, 0.5, 0.0, 7u)
        var qh <- half_of(qf, 1.0)
        var kh <- half_of(kf, scale)
        var vh <- half_of(vf, 1.0)
        var bqh = race_buf(dev, uint64(MP * QD * 2), unsafe(addr<void?>(qh[0])))
        var bk = race_buf(dev, uint64(NK * KVD * 4), unsafe(addr<void?>(kf[0])))
        var bkh = race_buf(dev, uint64(NK * KVD * 2), unsafe(addr<void?>(kh[0])))
        var bv = race_buf(dev, uint64(NK * KVD * 4), unsafe(addr<void?>(vf[0])))
        var bvh = race_buf(dev, uint64(NK * KVD * 2), unsafe(addr<void?>(vh[0])))
        let slab = uint64(HEADS) * uint64(MP) * uint64(NK) * 2ul
        var batt_a = race_buf(dev, slab, null)
        var batt_b = race_buf(dev, slab, null)
        var bstat_a = race_buf(dev, uint64(HEADS * MP * 2 * 4), null)
        var bstat_b = race_buf(dev, uint64(HEADS * MP * 2 * 4), null)
        var bxb_a = race_buf(dev, uint64(MP * QD * 4), null)
        var bxb_b = race_buf(dev, uint64(MP * QD * 4), null)
        var bsink = race_buf(dev, uint64(HEADS * 4), null)
        var ka = AttnArgs(qd = uint(QD), kv_dim = uint(KVD), head_size = uint(HS), kv_mul = uint(KV_MUL),
            npos = uint(NK), np32 = uint(NK), scale = scale, qoff = uint(QOFF), qrows = uint(MP),
            window = 0u, softcap = 0.0, hass = 0u)
        var tka = KvTwinArgs(total = uint(NK * KVD), scale = scale)
        let gfma = double(MP) * double(NK) * double(HS) * double(HEADS) / 1.0e9lf
        print("attention at depth: {MP} queries at {QOFF}, {NK} keys, {HEADS} heads of {HS} over {HEADS / KV_MUL} KV heads - {gfma} G multiply-adds a GEMM\n")
        for (stage in range(4)) {
            var t_a = -1.0lf
            var t_b = -1.0lf
            race_pair_ms(queue, 3, t_a, t_b, $(enc) {
                enc_qk_mm_th_c_pso(enc, qk_p, bqh, bk, batt_a, ka, int64(MP), int64(NK), int64(HEADS))
                if (stage >= 1) {
                    enc_rowstat_pso(enc, rs_p, batt_a, ka, bstat_a, bsink, int64(MP), int64(HEADS))
                }
                if (stage >= 2) {
                    enc_av_mm_p_c_pso(enc, av_p, batt_a, bv, bxb_a, ka, bstat_a, int64(MP), int64(HEADS), int64(HS))
                }
            }, $(enc) {
                if (stage == 3) {
                    enc_kv_twin_pso(enc, tw_d, bk, bv, bkh, bvh, tka, int64(NK * KVD))
                }
                enc_qk_dev128_c_pso(enc, qk_d, bqh, bkh, batt_b, ka, int64(MP), int64(NK), int64(HEADS))
                if (stage >= 1) {
                    enc_attn_pexp_pso(enc, px_d, batt_b, ka, bstat_b, bsink, int64(MP), int64(HEADS))
                }
                if (stage >= 2) {
                    enc_av_dev128_c_pso(enc, av_d, batt_b, bvh, bxb_b, ka, bstat_b, int64(MP), int64(HEADS), int64(HS))
                }
            })
            var note = ""
            if (stage == 0 && race_envelope_ok_h(batt_a, batt_b, MP * NK, note)) {   //! head 0's rows
                note = "scores-agree"
            } elif (stage >= 2) {
                race_envelope_ok(bxb_a, bxb_b, MP * QD, note)
            }
            print("stage {stage} (0 qk, 1 +softmax, 2 +av, 3 +twin pass) staged={t_a}ms device={t_b}ms x{t_a / t_b} {note}\n")
        }
        ref_report(qf, kf, vf, scale, [(name = "staged trio", buf = bxb_a), (name = "device pair", buf = bxb_b)])
        delete qf
        delete kf
        delete vf
        delete qh
        delete kh
        delete vh
        for (b in [bqh, bk, bkh, bv, bvh, batt_a, batt_b, bstat_a, bstat_b, bxb_a, bxb_b, bsink]) {
            metal_release(b)
        }
        for (p in [qk_p, rs_p, av_p, tw_d, qk_d, px_d, av_d]) {
            metal_release(p)
        }
    } else {
        print("attn_depth_race: no das_metal in this build\n")
    }
}
