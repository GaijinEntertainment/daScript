options gen2
options _dasllama_internal = true

// THE PREFILL ROUTER - a window's router logits (2048 positions x 256 experts over a 2048-wide row,
// the Qwen3.6-35B-A3B geometry) on the batched slab GEMV the driver dispatches against the half
// tensor GEMM over half operands, its 128-row tile. Both arms dispatch the shipped stamps through
// their generated builders.
//
//   bin/daslang -jit modules/dasLLAMA/harness/router_race.das

require math
require metal/msl_shader
require metal/das_metal_boost
require ?das_metal das_metal
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_kernels
require dasllama/dasllama_metal_prefill
require _race_fixture

let NPOS = 2048
let DIM = 2048
let NE = 256

[export]
def main {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var dev = metal_create_system_default_device()
        var queue = metal_new_command_queue(dev)
        var err = ""
        var prod = pipeline_from_source(dev, MetalMoeRouterB_metal_moe_router_msl, MetalMoeRouterB_metal_moe_router_msl_entry, MetalMoeRouterB_metal_moe_router_msl_fastmath, err)
        var th = pipeline_from_source(dev, MetalHalfMulMmTH128_metal_half_mulmm_th_msl, MetalHalfMulMmTH128_metal_half_mulmm_th_msl_entry, MetalHalfMulMmTH128_metal_half_mulmm_th_msl_fastmath, err)
        if (prod == null || th == null) {
            print("pso: {err}\n")
            return
        }
        var wf <- race_fill(NE * DIM, 1, 0.1, 0.0)
        var xf <- race_fill(NPOS * DIM, 2, 4.0, 0.0)
        var wh <- [for (v in wf); float16(v)]
        var xh <- [for (v in xf); float16(v)]
        var bw = race_buf(dev, uint64(NE * DIM * 4), unsafe(addr<void?>(wf[0])))
        var bx = race_buf(dev, uint64(NPOS * DIM * 4), unsafe(addr<void?>(xf[0])))
        var bwh = race_buf(dev, uint64(NE * DIM * 2), unsafe(addr<void?>(wh[0])))
        var bxh = race_buf(dev, uint64(NPOS * DIM * 2), unsafe(addr<void?>(xh[0])))
        var by_p = race_buf(dev, uint64(NPOS * NE * 4), null)
        var by_h = race_buf(dev, uint64(NPOS * NE * 4), null)
        var brb = race_buf(dev, uint64(NE * 4), null)
        var u_dim = race_uniform_u32(dev, uint(DIM))
        var u_ne = race_uniform_u32(dev, uint(NE))
        var u_zero = race_uniform_u32(dev, 0u)
        var u_np = race_uniform_u32(dev, uint(NPOS))
        var t_p = -1.0lf
        var t_h = -1.0lf
        race_pair_ms(queue, 5, t_p, t_h, $(enc) {
            enc_moe_router_b_pso(enc, prod, bw, bx, by_p, u_dim, u_ne, u_dim, brb, u_zero, u_np, int64(NE), int64(NPOS))
        }, $(enc) {
            enc_gemm_hmm_th128_pso(enc, th, bwh, 0ul, bxh, 0ul, by_h, 0ul, u_dim, u_ne, int64(NPOS), int64(NE))
        })
        //! the logits against the dot in double from the f32 operands
        var worst_p = 0.0lf
        var worst_h = 0.0lf
        var mag = 0.0lf
        unsafe {
            let pp = reinterpret<float?>(metal_buffer_contents(by_p))
            let ph = reinterpret<float?>(metal_buffer_contents(by_h))
            for (p in [0, 777, NPOS - 1]) {
                for (e in range(NE)) {
                    var want = 0.0lf
                    for (k in range(DIM)) {
                        want += double(wf[e * DIM + k]) * double(xf[p * DIM + k])
                    }
                    mag = max(mag, abs(want))
                    worst_p = max(worst_p, abs(double(pp[p * NE + e]) - want))
                    worst_h = max(worst_h, abs(double(ph[p * NE + e]) - want))
                }
            }
        }
        let gfma = double(NPOS) * double(NE) * double(DIM) / 1.0e9lf
        print("router: {NPOS} positions x {NE} experts over {DIM} - {gfma} G multiply-adds\n")
        print("slab GEMV   {t_p} ms, off the double dot by at most {worst_p} of max {mag}\n")
        print("tensor half {t_h} ms x{t_p / t_h}, off by at most {worst_h}\n")
        delete wf
        delete xf
        delete wh
        delete xh
        for (b in [bw, bx, bwh, bxh, by_p, by_h, brb, u_dim, u_ne, u_zero, u_np]) {
            metal_release(b)
        }
        for (p in [prod, th]) {
            metal_release(p)
        }
    } else {
        print("router_race: no das_metal in this build\n")
    }
}
