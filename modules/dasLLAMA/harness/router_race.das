options gen2
options _dasllama_internal = true

// THE PREFILL ROUTER - a window's router logits (2048 positions x 256 experts over a 2048-wide row,
// the Qwen3.6-35B-A3B geometry) on the batched slab GEMV the driver dispatches against a
// device-direct tensor GEMM, f32 operands and half operands. Hand MSL, nothing here ships.
//
//   bin/daslang -jit modules/dasLLAMA/harness/router_race.das

require math
require strings
require metal/msl_shader
require metal/das_metal_boost
require ?das_metal das_metal
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_kernels

let NPOS = 2048
let DIM = 2048
let NE = 256

def private router_t_msl(elem : string; mt : int) : string {
    return build_string() $(var w) {
        w |> write("#include <metal_stdlib>\n#include <MetalPerformancePrimitives/MetalPerformancePrimitives.h>\n")
        w |> write("using namespace metal;\nusing namespace mpp::tensor_ops;\n")
        w |> write("kernel void lab_router_{elem}{mt}(device const {elem}* w [[buffer(0)]], device const {elem}* x [[buffer(1)]],\n")
        w |> write("        device float* y [[buffer(2)]], constant uint& kdim [[buffer(3)]], constant uint& ndim [[buffer(4)]],\n")
        w |> write("        uint3 wg [[threadgroup_position_in_grid]]) \{\n")
        w |> write("    typedef tensor<device {elem}, dextents<int32_t, 2>, tensor_inline> OT;\n")
        w |> write("    auto A = OT((device {elem} *)(x + wg.x * {mt}u * kdim), dextents<int32_t, 2>(int(kdim), {mt}), metal::array<int32_t, 2>\{1, int(kdim)\});\n")
        w |> write("    auto W = OT((device {elem} *)(w + wg.y * 64u * kdim), dextents<int32_t, 2>(int(kdim), 64), metal::array<int32_t, 2>\{1, int(kdim)\});\n")
        w |> write("    auto C = tensor<device float, dextents<int32_t, 2>, tensor_inline>(y + wg.x * {mt}u * ndim + wg.y * 64u, dextents<int32_t, 2>(64, {mt}), metal::array<int32_t, 2>\{1, int(ndim)\});\n")
        w |> write("    constexpr auto d = matmul2d_descriptor({mt}, 64, static_cast<int>(dynamic_extent), false, true, true);\n")
        w |> write("    matmul2d<d, execution_simdgroups<4>> op;\n")
        w |> write("    auto cT = op.get_destination_cooperative_tensor<OT, OT, float>();\n")
        w |> write("    op.run(A, W, cT);\n")
        w |> write("    cT.store(C);\n\}\n")
    }
}

def private fill(count, salt : int; amp : float) : array<float> {
    var xa : array<float>
    xa |> resize(count)
    var st = uint(salt) * 2654435761u + 12345u
    for (i in range(count)) {
        st = st * 1664525u + 1013904223u
        xa[i] = (float(int((st >> 9u) & 0xffffu)) / 65536.0 - 0.5) * amp
    }
    return <- xa
}

[export]
def main {   // nolint:STYLE038 — one-shot race harness; buffers + PSOs stay live to the release tail
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var dev = metal_create_system_default_device()
        var queue = metal_new_command_queue(dev)
        var err = ""
        var prod = pipeline_from_source(dev, MetalMoeRouterB_metal_moe_router_msl, MetalMoeRouterB_metal_moe_router_msl_entry, MetalMoeRouterB_metal_moe_router_msl_fastmath, err)
        var tf = pipeline_from_source(dev, router_t_msl("float", 128), "lab_router_float128", true, err)
        var th = pipeline_from_source(dev, router_t_msl("half", 128), "lab_router_half128", true, err)
        if (prod == null || tf == null || th == null) {
            print("pso: {err}\n")
            return
        }
        var wf <- fill(NE * DIM, 1, 0.1)
        var xf <- fill(NPOS * DIM, 2, 4.0)
        var wh <- [for (v in wf); float16(v)]
        var xh <- [for (v in xf); float16(v)]
        var bw = race_buf(dev, uint64(NE * DIM * 4), unsafe(addr<void?>(wf[0])))
        var bx = race_buf(dev, uint64(NPOS * DIM * 4), unsafe(addr<void?>(xf[0])))
        var bwh = race_buf(dev, uint64(NE * DIM * 2), unsafe(addr<void?>(wh[0])))
        var bxh = race_buf(dev, uint64(NPOS * DIM * 2), unsafe(addr<void?>(xh[0])))
        var by_p = race_buf(dev, uint64(NPOS * NE * 4), null)
        var by_f = race_buf(dev, uint64(NPOS * NE * 4), null)
        var by_h = race_buf(dev, uint64(NPOS * NE * 4), null)
        var brb = race_buf(dev, uint64(NE * 4), null)
        var u_dim = race_uniform_u32(dev, uint(DIM))
        var u_ne = race_uniform_u32(dev, uint(NE))
        var u_zero = race_uniform_u32(dev, 0u)
        var u_np = race_uniform_u32(dev, uint(NPOS))
        let enc_prod = $(enc : MetalComputeEncoder?) {
            kn_pipeline(enc, prod)
            kn_buffer(enc, bw, 0ul, 0)
            kn_buffer(enc, bx, 0ul, 1)
            kn_buffer(enc, by_p, 0ul, 2)
            kn_buffer(enc, u_dim, 0ul, 3)
            kn_buffer(enc, u_ne, 0ul, 4)
            kn_buffer(enc, u_dim, 0ul, 5)
            kn_buffer(enc, brb, 0ul, 6)
            kn_buffer(enc, u_zero, 0ul, 7)
            kn_buffer(enc, u_np, 0ul, 8)
            kn_dispatch(enc, uint3(uint(NE / 4), uint(NPOS / 8), 1u), uint3(128u, 1u, 1u))
        }
        var t_p = -1.0lf
        var t_f = -1.0lf
        var t_h = -1.0lf
        race_pair_ms(queue, 5, t_p, t_f, enc_prod, $(enc) {
            kn_pipeline(enc, tf)
            kn_buffer(enc, bw, 0ul, 0)
            kn_buffer(enc, bx, 0ul, 1)
            kn_buffer(enc, by_f, 0ul, 2)
            kn_buffer(enc, u_dim, 0ul, 3)
            kn_buffer(enc, u_ne, 0ul, 4)
            kn_dispatch(enc, uint3(uint(NPOS / 128), uint(NE / 64), 1u), uint3(128u, 1u, 1u))
        })
        var t_p2 = -1.0lf
        race_pair_ms(queue, 5, t_p2, t_h, enc_prod, $(enc) {
            kn_pipeline(enc, th)
            kn_buffer(enc, bwh, 0ul, 0)
            kn_buffer(enc, bxh, 0ul, 1)
            kn_buffer(enc, by_h, 0ul, 2)
            kn_buffer(enc, u_dim, 0ul, 3)
            kn_buffer(enc, u_ne, 0ul, 4)
            kn_dispatch(enc, uint3(uint(NPOS / 128), uint(NE / 64), 1u), uint3(128u, 1u, 1u))
        })
        //! the logits against the dot in double from the f32 operands, and the top-8 picks each form makes
        var worst_p = 0.0lf
        var worst_f = 0.0lf
        var worst_h = 0.0lf
        var mag = 0.0lf
        unsafe {
            let pp = reinterpret<float?>(metal_buffer_contents(by_p))
            let pf = reinterpret<float?>(metal_buffer_contents(by_f))
            let ph = reinterpret<float?>(metal_buffer_contents(by_h))
            for (p in [0, 777, NPOS - 1]) {
                for (e in range(NE)) {
                    var want = 0.0lf
                    for (k in range(DIM)) {
                        want += double(wf[e * DIM + k]) * double(xf[p * DIM + k])
                    }
                    mag = max(mag, abs(want))
                    worst_p = max(worst_p, abs(double(pp[p * NE + e]) - want))
                    worst_f = max(worst_f, abs(double(pf[p * NE + e]) - want))
                    worst_h = max(worst_h, abs(double(ph[p * NE + e]) - want))
                }
            }
        }
        let gfma = double(NPOS) * double(NE) * double(DIM) / 1.0e9lf
        print("router: {NPOS} positions x {NE} experts over {DIM} - {gfma} G multiply-adds\n")
        print("slab GEMV   {t_p} ms ({t_p2} in the second pair), off the double dot by at most {worst_p} of max {mag}\n")
        print("tensor f32  {t_f} ms x{t_p / t_f}, off by at most {worst_f}\n")
        print("tensor half {t_h} ms x{t_p2 / t_h}, off by at most {worst_h}\n")
        delete wf
        delete xf
        delete wh
        delete xh
        for (b in [bw, bx, bwh, bxh, by_p, by_f, by_h, brb, u_dim, u_ne, u_zero, u_np]) {
            metal_release(b)
        }
        for (p in [prod, tf, th]) {
            metal_release(p)
        }
    } else {
        print("router_race: no das_metal in this build\n")
    }
}
