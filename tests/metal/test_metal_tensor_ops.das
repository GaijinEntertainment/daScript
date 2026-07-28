options gen2
options indenting = 4

// Metal-4 tensor ops (mpp::tensor_ops::matmul2d) through the das kernel rail: each tmm2d_*
// call lowers to a generated preamble helper building tensor_inline views over the plain
// buffer binds (no host MTLTensor). Data is integer-valued (and scales are powers of two),
// so every float accumulation order is exact — GPU must equal the CPU-replayed stub EXACTLY.
// The emitted-source asserts run on every platform; the dispatch half is Apple-only.

require dastest/testing_boost public
require strings
require metal/msl_shader
require daslib/math_bits   // float_bits_to_uint — bf16 halfword construction
require llvm/daslib/f16_cvt   // f32_to_f16 — the f16s scale plane
require _metal_common  // nolint:STYLE030 — used only inside the Apple static_if half; off-Apple lint sees the branch compiled out

let M = 8
let N = 16
let KK = 64
let NB = KK / 32
let SGS = 2u

class TmmKernels {
    @ssbo @binding = 0 xa : array<float>     // A: M x KK f32 activations (bf16 case)
    @ssbo @binding = 1 wb : array<uint16>    // W: N x KK bf16 halfwords
    @ssbo @binding = 2 oc : array<float>     // C: M x N
    @ssbo @binding = 3 aq : array<int8>      // A: M x KK int8 quants (q8 case)
    @ssbo @binding = 4 asf : array<float>    // as: M x NB activation block scales
    @ssbo @binding = 5 wq : array<int8>      // W: N x KK int8 quants
    @ssbo @binding = 6 wsf : array<float>    // ws: N x NB weight block scales
    @ssbo @binding = 7 wsh : array<uint16>   // ws as binary16 halfwords (the f16s twin)
    @ssbo @binding = 8 bsh : array<float16>  // q8b: 34B-interleaved W blob, half-scale view
    @ssbo @binding = 9 bqb : array<int8>     // q8b: the SAME blob layout, byte view

    [metal_kernel(name="tmm_bf16_msl"), marker(no_coverage)]
    def tmm_bf16_k {
        var ap = unsafe(addr(xa[0]))
        var wp = unsafe(addr(wb[0]))
        var cp = unsafe(addr(oc[0]))
        tmm2d_f32_bf16_f32(8u, 16u, 2u, ap, 64u, wp, 64u, cp, 16u, 64u)
    }

    [metal_kernel(name="tmm_q8_msl"), marker(no_coverage)]
    def tmm_q8_k {
        var ap = unsafe(addr(aq[0]))
        var sp = unsafe(addr(asf[0]))
        var wp = unsafe(addr(wq[0]))
        var vp = unsafe(addr(wsf[0]))
        var cp = unsafe(addr(oc[0]))
        tmm2d_q8_f32(8u, 16u, 2u, ap, 64u, sp, 2u, wp, 64u, vp, 2u, cp, 16u, 64u)
    }

    [metal_kernel(name="tmm_q8h_msl"), marker(no_coverage)]
    def tmm_q8h_k {
        var ap = unsafe(addr(aq[0]))
        var sp = unsafe(addr(asf[0]))
        var wp = unsafe(addr(wq[0]))
        var vp = unsafe(addr(wsh[0]))
        var cp = unsafe(addr(oc[0]))
        tmm2d_q8_f16s(8u, 16u, 2u, ap, 64u, sp, 2u, wp, 64u, vp, 2u, cp, 16u, 64u)
    }

    @workgroup tga : float16[256]    // tg protocol: 8 x 32 flat A chunk
    @workgroup tgb : float16[512]    // 16 x 32 flat B chunk

    [metal_kernel(name="tmm_tg_msl"), marker(no_coverage)]
    def tmm_tg_k {
        var acc : float[128]
        var ap = unsafe(addr(xa[0]))
        var wp = unsafe(addr(wb[0]))
        var cp = unsafe(addr(oc[0]))
        tmm2d_tg_begin(acc, 8u, 16u, 2u, 32u)
        var kb = 0u
        while (kb < 2u) {
            var i = gl_LocalInvocationID.x
            while (i < 256u) {
                tga[i] = float16(unsafe(ap[(i / 32u) * 64u + kb * 32u + i % 32u]))
                i += gl_WorkGroupSize.x
            }
            i = gl_LocalInvocationID.x
            while (i < 512u) {
                tgb[i] = float16(uint_bits_to_float(uint(unsafe(wp[(i / 32u) * 64u + kb * 32u + i % 32u])) << 16u))
                i += gl_WorkGroupSize.x
            }
            barrier()
            tmm2d_tg_step(acc, tga, tgb, 8u, 16u, 32u)
            barrier()
            kb++
        }
        tmm2d_tg_store(acc, cp, 8u, 16u, 16u)
    }

    [metal_kernel(name="tmm_q8b_msl"), marker(no_coverage)]
    def tmm_q8b_k {
        var sp = unsafe(addr(bsh[0]))
        var wp = unsafe(addr(bqb[2]))
        var ap = unsafe(addr(xa[0]))
        var cp = unsafe(addr(oc[0]))
        tmm2d_q8b_f32(8u, 16u, 2u, sp, wp, ap, 64u, cp, 16u, 64u, 2u)
    }
}

def private fill_oracle(var o : TmmKernels?) {
    o.xa |> resize(M * KK)
    o.wb |> resize(N * KK)
    o.oc |> resize(M * N)
    o.aq |> resize(M * KK)
    o.asf |> resize(M * NB)
    o.wq |> resize(N * KK)
    o.wsf |> resize(N * NB)
    o.wsh |> resize(N * NB)
    for (i in range(M * KK)) {
        o.xa[i] = float(i % 7 - 3)             // integer-valued -> exact in any sum order
        o.aq[i] = int8(i % 17 - 8)
    }
    for (i in range(N * KK)) {
        // bf16 halfword of an integer value: float bits >> 16 (values here are bf16-exact)
        o.wb[i] = uint16(float_bits_to_uint(float(i % 5 - 2)) >> 16u)
        o.wq[i] = int8((i * 3) % 15 - 7)
    }
    for (i in range(M * NB)) {
        o.asf[i] = float(1 << (i % 3))         // powers of two: scale folds exact
    }
    for (i in range(N * NB)) {
        let v = 0.5 * float(1 << (i % 4))
        o.wsf[i] = v
        o.wsh[i] = uint16(f32_to_f16(v))       // powers of two are f16-exact
    }
    // q8b blob: block (j*NB+b) = 2B f16 scale + 32 int8 quants, integer-valued and pow2-scaled
    o.bsh |> resize(N * NB * 17)
    o.bqb |> resize(N * NB * 34)
    for (blk in range(N * NB)) {
        o.bsh[blk * 17] = float16(0.5 * float(1 << (blk % 3)))
        for (k in range(32)) {
            o.bqb[blk * 34 + 2 + k] = int8((blk * 7 + k * 3) % 19 - 9)
        }
    }
}

[test]
def test_tensor_ops(t : T?) {
    t |> run("tmm2d lowering: generated helpers + MPP preamble") @(t : T?) {
        t |> success(find(tmm_bf16_msl, "__das_tmm2d_bf_8_16_2") >= 0, "bf helper generated")
        t |> success(find(tmm_q8_msl, "__das_tmm2d_q8_8_16_2") >= 0, "q8 helper generated")
        t |> success(find(tmm_q8h_msl, "__das_tmm2d_q8h_8_16_2") >= 0, "q8h helper generated")
        t |> success(find(tmm_q8b_msl, "__das_tmm2d_q8b_8_16_2") >= 0, "q8b helper generated")
        t |> success(find(tmm_tg_msl, "__tgop_acc.run(") >= 0, "tg protocol op emitted")
        t |> success(find(tmm_tg_msl, "float acc[") < 0, "acc local suppressed on GPU")
        t |> success(find(tmm_bf16_msl, "MetalPerformancePrimitives") >= 0, "MPP include present")
        t |> success(find(tmm_bf16_msl, "mpp::tensor_ops") >= 0, "namespace present")
        t |> success(find(tmm_bf16_msl, "__das_tmm2d_bf_8_16_2(") < find(tmm_bf16_msl, "kernel void"), "helper precedes the kernel")
    }
    t |> run("tmm2d: GPU == CPU-replay exact") @(t : T?) {
        var oracle = new TmmKernels()
        fill_oracle(oracle)
        gl_LocalInvocationID = uint3(0u, 0u, 0u)
        gl_WorkGroupSize = uint3(1u, 1u, 1u)   // width-1 replay: staging strides cover whole tiles
        var want_bf : array<float>
        var want_q8 : array<float>
        var want_q8h : array<float>
        oracle->tmm_bf16_k()
        want_bf := oracle.oc
        oracle->tmm_q8_k()
        want_q8 := oracle.oc
        oracle->tmm_q8h_k()
        want_q8h := oracle.oc
        var want_q8b : array<float>
        oracle->tmm_q8b_k()
        want_q8b := oracle.oc
        var want_tg : array<float>
        oracle->tmm_tg_k()
        want_tg := oracle.oc
        var nz = 0
        for (v in want_q8) {
            nz += v != 0.0 ? 1 : 0
        }
        t |> success(nz > M * N / 2, "q8 reference is non-trivial")
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var queue = metal_new_command_queue(dev)
                var bxa = buf_upload(dev, oracle.xa)
                var bwb = buf_upload(dev, oracle.wb)
                var baq = buf_upload(dev, oracle.aq)
                var basf = buf_upload(dev, oracle.asf)
                var bwq = buf_upload(dev, oracle.wq)
                var bwsf = buf_upload(dev, oracle.wsf)
                var bwsh = buf_upload(dev, oracle.wsh)
                var bbsh = buf_upload(dev, oracle.bsh)
                var bbqb = buf_upload(dev, oracle.bqb)
                gpu_bad = 0
                for (which in range(5)) {
                    let src = which == 0 ? tmm_bf16_msl : (which == 1 ? tmm_q8_msl : (which == 2 ? tmm_q8h_msl : (which == 3 ? tmm_q8b_msl : tmm_tg_msl)))
                    let entry = which == 0 ? tmm_bf16_msl_entry : (which == 1 ? tmm_q8_msl_entry : (which == 2 ? tmm_q8h_msl_entry : (which == 3 ? tmm_q8b_msl_entry : tmm_tg_msl_entry)))
                    var perr : string
                    var pso = pipeline_from_source(dev, src, entry, false, perr)
                    t |> success(pso != null, "pipeline {entry}: {perr}")
                    if (pso == null) {
                        gpu_bad++
                        continue
                    }
                    var boc = buf_fill(dev, M * N, 0.0)
                    let tew = metal_pipeline_thread_execution_width(pso)
                    var bufs <- [bxa, bwb, boc, baq, basf, bwq, bwsf, bwsh, bbsh, bbqb]
                    var rerr : string
                    let tpg = uint(tew) * SGS
                    let ran = run_compute_1d(queue, pso, bufs, tpg, tpg, rerr, which == 4 ? tmm_tg_msl_tgmem : 0ul)
                    t |> success(ran, "dispatch {entry}: {rerr}")
                    if (ran) {
                        let bad = buf_mismatch_exact(boc, which == 0 ? want_bf : (which == 1 ? want_q8 : (which == 2 ? want_q8h : (which == 3 ? want_q8b : want_tg))))
                        if (bad != 0) {
                            var got <- buf_download(boc, M * N, type<float>)
                            let wref & = unsafe(which == 0 ? want_bf : (which == 1 ? want_q8 : (which == 2 ? want_q8h : (which == 3 ? want_q8b : want_tg))))
                            var shown = 0
                            for (i in range(M * N)) {
                                if (got[i] != wref[i] && shown < 4) {
                                    print("MISMATCH {entry} [{i / N},{i % N}] gpu={got[i]} want={wref[i]}\n")
                                    shown++
                                }
                            }
                            delete got
                        }
                        t |> equal(bad, 0)
                        gpu_bad += bad
                    } else {
                        gpu_bad++
                    }
                    bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                    unsafe {
                        delete bufs
                    }
                    metal_release(boc)
                    metal_release(pso)
                }
                metal_release(bxa)
                metal_release(bwb)
                metal_release(baq)
                metal_release(basf)
                metal_release(bwq)
                metal_release(bwsf)
                metal_release(bwsh)
                metal_release(bbsh)
                metal_release(bbqb)
                metal_release(queue)
            }
            if (gpu_bad >= 0) {
                t |> equal(gpu_bad, 0)
            }
            t |> equal(metal_live_object_count(), 0l)
        } else {
            feint("das_metal is not built on this platform; CPU-reference half only\n")
        }
        unsafe {
            delete oracle
        }
    }
}
