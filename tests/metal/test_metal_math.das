options gen2
options indenting = 4

// Real-GPU behavioral gate for the Phase-6 math builtins (exp/log/sqrt/rsqrt/sin/cos/tanh/abs/
// saturate/min/max — float scalar + float4 + int/uint classes). fastmath=false pins the precise
// MSL math path: fast::sin/cos carry an ABSOLUTE ~2^-11 error bound that would swamp the oracle
// tolerance, while the precise set stays within a few ULP of the CPU libm reference. abs/min/max/
// saturate and the integer forms compare exact; transcendentals compare with fapprox.

require dastest/testing_boost public
require math
require daslib/math_bits
require metal/msl_shader
require _metal_common

let N = 256
let TPG = 64u

class MathCalls {
    @ssbo @binding = 0 a : array<float>
    @ssbo @binding = 1 b : array<float>
    @ssbo @binding = 2 c : array<float>
    @ssbo @binding = 3 va : array<float4>
    @ssbo @binding = 4 vb : array<float4>
    @ssbo @binding = 5 vc : array<float4>
    @ssbo @binding = 6 ia : array<int>
    @ssbo @binding = 7 ib : array<int>
    @ssbo @binding = 8 ic : array<int>
    @ssbo @binding = 9 ua : array<uint>
    @ssbo @binding = 10 ub : array<uint>
    @ssbo @binding = 11 uc : array<uint>
    @ssbo @binding = 12 qa : array<int8>
    @ssbo @binding = 13 qc : array<int8>

    [metal_kernel(name="mathcalls_gpu_msl", fastmath=false), marker(no_coverage)]
    def mathcalls {
        let i = gl_GlobalInvocationID.x
        let x = a[i]
        let y = b[i]
        c[i] = (((exp(min(x, 4.0)) + sqrt(abs(x))) + (rsqrt(abs(y) + 1.0) + tanh(x))) +
            ((sin(x) + cos(y)) + (saturate(x) + max(x, y))))
        c[i] += float(qa[i])
        c[i] += log(abs(x) + 1.0)
        c[i] += exp2(min(y, 8.0)) + log2(abs(y) + 1.5)
        c[i] += uint_bits_to_float((ua[i] & 0x007FFFFFu) | 0x3F800000u)   // as_type bitcast, valid-float bits
        let u = va[i]
        let v = vb[i]
        vc[i] = (((exp(min(u, float4(4.0))) + sqrt(abs(u))) + (rsqrt(abs(v) + float4(1.0)) + tanh(u))) +
            ((sin(u) + cos(v)) + (saturate(u) + max(u, v))))
        vc[i] = vc[i] + round(v)
        vc[i] = uint_bits_to_float(float_bits_to_uint(vc[i]))             // 4-lane round-trip, bit-exact
        ic[i] = min(abs(ia[i]), ib[i]) + max(ia[i], ib[i])
        ic[i] += float_bits_to_int(y)                                     // negative floats bit-exact too
        uc[i] = min(ua[i], ub[i]) + max(ua[i], ub[i])
        uc[i] += float_bits_to_uint(x) & 0x7FFFFFFFu
        qc[i] = int8(round(b[i] * 12.5))
    }
}

[test]
def test_math_gpu_vs_cpu(t : T?) {
    t |> run("math builtins: GPU ~= CPU-reference (precise math)") <| @(t : T?) {
        var oracle = new MathCalls()
        oracle.a |> resize(N)
        oracle.b |> resize(N)
        oracle.c |> resize(N)
        oracle.va |> resize(N)
        oracle.vb |> resize(N)
        oracle.vc |> resize(N)
        oracle.ia |> resize(N)
        oracle.ib |> resize(N)
        oracle.ic |> resize(N)
        oracle.ua |> resize(N)
        oracle.ub |> resize(N)
        oracle.uc |> resize(N)
        oracle.qa |> resize(N)
        oracle.qc |> resize(N)
        for (k in range(N)) {
            oracle.a[k] = float(k % 23) - 11.0
            oracle.b[k] = float(k % 17) * 0.5 - 4.0
            oracle.va[k] = float4(float(k % 9) - 4.0, float(k % 5) * 0.75, float(k % 13) - 6.5, -float(k % 7))
            oracle.vb[k] = float4(float(k % 4) - 1.5, float(k % 6) + 0.25, -float(k % 3), float(k % 11) * 0.5)
            oracle.ia[k] = k % 19 - 9
            oracle.ib[k] = k % 7 - 3
            oracle.ua[k] = uint((k * 3) % 31)
            oracle.ub[k] = uint(k % 11)
            oracle.qa[k] = int8(k % 251 - 125)
        }
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            oracle->mathcalls()
        }
        // k=0: x=-11, y=-4: exp(-11)+sqrt(11)+rsqrt(5)+tanh(-11)+sin(-11)+cos(-4)+saturate(-11)+max(-11,-4),
        // then += float(qa[0]) = -125, then += log(|x|+1) = log(12),
        // then += exp2(-4) + log2(5.5), then += bits((0&0x7FFFFF)|0x3F800000) = 1.0
        t |> success(fapprox(oracle.c[0], exp(-11.0) + sqrt(11.0) + rsqrt(5.0) + tanh(-11.0) +
            sin(-11.0) + cos(-4.0) + 0.0 + (-4.0) - 125.0 + log(12.0) + exp2(-4.0) + log2(5.5) + 1.0), "the oracle itself computed")
        t |> equal(oracle.ic[0], min(9, -3) + max(-9, -3) + float_bits_to_int(-4.0))
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, mathcalls_gpu_msl, mathcalls_gpu_msl_entry, mathcalls_gpu_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var ba = buf_upload(dev, oracle.a)
                var bb = buf_upload(dev, oracle.b)
                var bc = buf_fill(dev, N, 0.0)
                var bva = buf_upload(dev, oracle.va)
                var bvb = buf_upload(dev, oracle.vb)
                var bvc = buf_fill(dev, N, float4(0.0))
                var bia = buf_upload(dev, oracle.ia)
                var bib = buf_upload(dev, oracle.ib)
                var bic = buf_fill(dev, N, 0)
                var bua = buf_upload(dev, oracle.ua)
                var bub = buf_upload(dev, oracle.ub)
                var buc = buf_fill(dev, N, 0u)
                var bqa = buf_upload(dev, oracle.qa)
                var bqc = buf_fill(dev, N, int8(0))
                var bufs <- [ba, bb, bc, bva, bvb, bvc, bia, bib, bic, bua, bub, buc, bqa, bqc]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = (((buf_mismatch_approx(bc, oracle.c) + buf_mismatch_approx4(bvc, oracle.vc)) +
                        (buf_mismatch_exact(bic, oracle.ic) + buf_mismatch_exact(buc, oracle.uc))) +
                        buf_mismatch_exact(bqc, oracle.qc))
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(ba)
                metal_release(bb)
                metal_release(bc)
                metal_release(bva)
                metal_release(bvb)
                metal_release(bvc)
                metal_release(bia)
                metal_release(bib)
                metal_release(bic)
                metal_release(bua)
                metal_release(bub)
                metal_release(buc)
                metal_release(bqa)
                metal_release(bqc)
                metal_release(pso)
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
