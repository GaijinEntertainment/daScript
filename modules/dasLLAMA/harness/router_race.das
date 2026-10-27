options gen2
options _dasllama_internal = true

// THE PREFILL ROUTER - a window's router logits (2048 positions x 256 experts over a 2048-wide row,
// the Qwen3.6-35B-A3B geometry) on the batched slab GEMV against the f32 tile GEMM the driver
// dispatches (`MetalF32ExactMm`), both over the router rows' own f32 bytes. Both arms dispatch the
// shipped stamps through their generated builders.
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
        var prod = pipeline_from_source(dev, metal_moe_router_b_msl, metal_moe_router_b_msl_entry, metal_moe_router_b_msl_fastmath, err)
        var fx = pipeline_from_source(dev, metal_f32_exact_mulmm_msl, metal_f32_exact_mulmm_msl_entry, metal_f32_exact_mulmm_msl_fastmath, err)
        if (prod == null || fx == null) {
            print("pso: {err}\n")
            return
        }
        var wf <- race_fill(NE * DIM, 1, 0.1, 0.0)
        var xf <- race_fill(NPOS * DIM, 2, 4.0, 0.0)
        var bw = metal_buffer_from(dev, uint64(NE * DIM * 4), unsafe(addr<void?>(wf[0])))
        var bx = metal_buffer_from(dev, uint64(NPOS * DIM * 4), unsafe(addr<void?>(xf[0])))
        var by_p = metal_buffer_from(dev, uint64(NPOS * NE * 4), null)
        var by_x = metal_buffer_from(dev, uint64(NPOS * NE * 4), null)
        var brb = metal_buffer_from(dev, uint64(NE * 4), null)
        var u_dim = race_uniform_u32(dev, uint(DIM))
        var u_ne = race_uniform_u32(dev, uint(NE))
        var rka = moe_router_args(int64(NE), int64(DIM), int64(NPOS))
        var t_p = -1.0lf
        var t_x = -1.0lf
        race_pair_ms(queue, 9, t_p, t_x, $(enc) {
            enc_moe_router_b_pso(enc, prod, bw, bx, by_p, brb, rka, int64(NE), int64(NPOS))
        }, $(enc) {
            enc_f32_exact_mm_pso(enc, fx, bw, 0ul, bx, by_x, 0ul, u_dim, u_ne, int64(NPOS), int64(NE))
        })
        //! the logits against the dot in double from the f32 operands
        var worst_p = 0.0lf
        var worst_x = 0.0lf
        var mag = 0.0lf
        unsafe {
            let pp = reinterpret<float?>(metal_buffer_contents(by_p))
            let px = reinterpret<float?>(metal_buffer_contents(by_x))
            for (p in [0, 777, NPOS - 1]) {
                for (e in range(NE)) {
                    var want = 0.0lf
                    for (k in range(DIM)) {
                        want += double(wf[e * DIM + k]) * double(xf[p * DIM + k])
                    }
                    mag = max(mag, abs(want))
                    worst_p = max(worst_p, abs(double(pp[p * NE + e]) - want))
                    worst_x = max(worst_x, abs(double(px[p * NE + e]) - want))
                }
            }
        }
        let gfma = double(NPOS) * double(NE) * double(DIM) / 1.0e9lf
        print("router: {NPOS} positions x {NE} experts over {DIM} - {gfma} G multiply-adds\n")
        print("slab GEMV   {t_p} ms, off the double dot by at most {worst_p} of max {mag}\n")
        print("f32 tile    {t_x} ms x{t_p / t_x}, off by at most {worst_x}\n")
        delete wf
        delete xf
        for (b in [bw, bx, by_p, by_x, brb, u_dim, u_ne]) {
            metal_release(b)
        }
        for (p in [prod, fx]) {
            metal_release(p)
        }
    } else {
        print("router_race: no das_metal in this build\n")
    }
}
