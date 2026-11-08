options gen2

require dastest/testing_boost public
require metal/msl_shader
require _metal_common  // nolint:STYLE030,LINT019 - used in the das_metal static_if half
require math
require daslib/math_bits
require ?das_metal metal/das_metal_boost

// Real-GPU behavioral gate vs the CPU-reference oracle for the
// Phase-2 arithmetic kernels (uint/int/float scalar classes +
// float4/int2 vectors). The kernel bodies are ordinary daslang: a
// driver loop feeds gl_GlobalInvocationID and runs the same method on
// the CPU — that run IS the expected output. Ints compare bit-exact
// (wrap/truncation semantics match C on both sides); floats with
// fastmath tolerance. The CPU half runs on every platform; the GPU
// half is static_if-gated on das_metal and skips cleanly where there
// is no Metal device.
let N = 256
let TPG = 64u

class UArith {
    @ssbo @binding = 0 a : array<uint>
    @ssbo @binding = 1 b : array<uint>
    @ssbo @binding = 2 c : array<uint>
    @ssbo @binding = 3 d : array<uint>

    [metal_kernel(name="uarith_gpu_msl"), marker(no_coverage)]
    def uarith {
        let i = gl_GlobalInvocationID.x
        let x = a[i]
        let y = b[i] | 1u
        c[i] = ((x + y) - (x * y)) + ((x / y) + (x % y))
        d[i] = ((x & y) | (x ^ y)) + ((~x) + ((x << 2u) >> 1u))
    }
}

class IArith {
    @ssbo @binding = 0 a : array<int>
    @ssbo @binding = 1 b : array<int>
    @ssbo @binding = 2 c : array<int>
    @ssbo @binding = 3 d : array<int>

    [metal_kernel(name="iarith_gpu_msl"), marker(no_coverage)]
    def iarith {
        let i = gl_GlobalInvocationID.x
        let x = a[i]
        let y = b[i] | 1
        c[i] = (((x + y) - (x * y)) + ((x / y) + (x % y))) + (-x)
        d[i] = ((x & y) | (x ^ y)) + ((~x) + ((x << 2) >> 1))
    }
}

class FArith {
    @ssbo @binding = 0 a : array<float>
    @ssbo @binding = 1 b : array<float>
    @ssbo @binding = 2 c : array<float>

    [metal_kernel(name="farith_gpu_msl"), marker(no_coverage)]
    def farith {
        let i = gl_GlobalInvocationID.x
        let x = a[i]
        let y = b[i] + 1.5
        c[i] = ((((x + y) - (x * y)) + (x / y)) + (x % y)) + (-x)
    }
}

class VecArith {
    @ssbo @binding = 0 fa : array<float4>
    @ssbo @binding = 1 fb : array<float4>
    @ssbo @binding = 2 fc : array<float4>
    @ssbo @binding = 3 ia : array<int2>
    @ssbo @binding = 4 ib : array<int2>
    @ssbo @binding = 5 flags : array<int>

    [metal_kernel(name="vecarith_gpu_msl"), marker(no_coverage)]
    def vecarith {
        let i = gl_GlobalInvocationID.x
        let u = fa[i]
        let v = fb[i] * 0.5
        fc[i] = (((u + v) - (u * v)) + (u / v)) + ((u * 2.0) + (-u))
        let p = ia[i]
        let q = ib[i]
        flags[i] = (p == q) ? 1 : ((((p + q) - (p * q)) != q) ? 2 : 3)
    }
}

[test(description = "uint arithmetic run on the GPU gives the same results as the CPU reference.")]
def test_uarith_gpu_vs_cpu(t : T?) {
    t |> run("uint arithmetic: GPU == CPU-reference") <| @(t : T?) {
        var oracle = new UArith()
        oracle.a |> resize(N)
        oracle.b |> resize(N)
        oracle.c |> resize(N)
        oracle.d |> resize(N)
        for (k in range(N)) {
            oracle.a[k] = uint(k * 7 + 3)
            oracle.b[k] = uint(k % 13)
        }
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            oracle->uarith()
        }
        // x=3, y=1: (3+1)-(3*1) + (3/1)+(3%1) = 4 — the oracle itself
        // computed
        t |> equal(oracle.c[0], 4u)
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, uarith_gpu_msl, uarith_gpu_msl_entry,
                                               uarith_gpu_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var ba = buf_upload(dev, oracle.a)
                var bb = buf_upload(dev, oracle.b)
                var bc = buf_fill(dev, N, 0u)
                var bd = buf_fill(dev, N, 0u)
                var bufs <- [ba, bb, bc, bd]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = buf_mismatch_exact(bc, oracle.c) + buf_mismatch_exact(bd, oracle.d)
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(ba)
                metal_release(bb)
                metal_release(bc)
                metal_release(bd)
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

[test(description = "int arithmetic run on the GPU gives the same results as the CPU reference.")]
def test_iarith_gpu_vs_cpu(t : T?) {
    t |> run("int arithmetic: GPU == CPU-reference") <| @(t : T?) {
        var oracle = new IArith()
        oracle.a |> resize(N)
        oracle.b |> resize(N)
        oracle.c |> resize(N)
        oracle.d |> resize(N)
        for (k in range(N)) {
            oracle.a[k] = k % 97 - 40
            oracle.b[k] = k % 13 - 6
        }
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            oracle->iarith()
        }
        t |> equal(oracle.c[0], -197)   // x=-40, y=-5: (-45 - 200) + (8 + 0) + 40 = -197
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, iarith_gpu_msl, iarith_gpu_msl_entry,
                                               iarith_gpu_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var ba = buf_upload(dev, oracle.a)
                var bb = buf_upload(dev, oracle.b)
                var bc = buf_fill(dev, N, 0)
                var bd = buf_fill(dev, N, 0)
                var bufs <- [ba, bb, bc, bd]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = buf_mismatch_exact(bc, oracle.c) + buf_mismatch_exact(bd, oracle.d)
                }
                bufs |> clear()
                unsafe {
                    delete bufs
                }
                metal_release(ba)
                metal_release(bb)
                metal_release(bc)
                metal_release(bd)
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

[test(description = "float arithmetic including fmod run on the GPU matches the CPU reference within tolerance.")]
def test_farith_gpu_vs_cpu(t : T?) {
    t |> run("float arithmetic (incl. fmod): GPU ~= CPU-reference") <| @(t : T?) {
        var oracle = new FArith()
        oracle.a |> resize(N)
        oracle.b |> resize(N)
        oracle.c |> resize(N)
        for (k in range(N)) {
            oracle.a[k] = float(k % 23)
            oracle.b[k] = float(k % 7)
        }
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            oracle->farith()
        }
        t |> success(fapprox(oracle.c[0], 1.5),
                     "x=0, y=1.5: result is 1.5 — the oracle itself computed")
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, farith_gpu_msl, farith_gpu_msl_entry,
                                               farith_gpu_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var ba = buf_upload(dev, oracle.a)
                var bb = buf_upload(dev, oracle.b)
                var bc = buf_fill(dev, N, 0.0)
                var bufs <- [ba, bb, bc]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = buf_mismatch_approx(bc, oracle.c)
                }
                bufs |> clear()
                unsafe {
                    delete bufs
                }
                metal_release(ba)
                metal_release(bb)
                metal_release(bc)
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

[test(description = "Vector arithmetic and all/any compares run on the GPU match the CPU reference within tolerance.")]
def test_vecarith_gpu_vs_cpu(t : T?) {
    t |> run("vector arithmetic + all/any compare: GPU ~= CPU-reference") <| @(t : T?) {
        var oracle = new VecArith()
        oracle.fa |> resize(N)
        oracle.fb |> resize(N)
        oracle.fc |> resize(N)
        oracle.ia |> resize(N)
        oracle.ib |> resize(N)
        oracle.flags |> resize(N)
        for (k in range(N)) {
            oracle.fa[k] = float4(float(k % 9) - 4.0, float(k % 5), float(k % 3) + 0.5,
                                  -float(k % 7))
            oracle.fb[k] = float4(float(k % 4) + 1.0, float(k % 6) + 1.5, 2.0, float(k % 3) + 1.0)
            oracle.ia[k] = int2(k % 5, k % 7 - 3)
            oracle.ib[k] = (k % 5 == 0) ? oracle.ia[k] : int2(k % 4 - 1, k % 6)
        }
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            oracle->vecarith()
        }
        t |> equal(oracle.flags[0], 1)      // ib[0] == ia[0] by construction -> the all() branch
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, vecarith_gpu_msl, vecarith_gpu_msl_entry,
                                               vecarith_gpu_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var bfa = buf_upload(dev, oracle.fa)
                var bfb = buf_upload(dev, oracle.fb)
                var bfc = buf_fill(dev, N, float4(0.0))
                var bia = buf_upload(dev, oracle.ia)
                var bib = buf_upload(dev, oracle.ib)
                var bfl = buf_fill(dev, N, 0)
                var bufs <- [bfa, bfb, bfc, bia, bib, bfl]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = buf_mismatch_approx4(bfc, oracle.fc) + buf_mismatch_exact(bfl, oracle.flags)
                }
                bufs |> clear()
                unsafe {
                    delete bufs
                }
                metal_release(bfa)
                metal_release(bfb)
                metal_release(bfc)
                metal_release(bia)
                metal_release(bib)
                metal_release(bfl)
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

// Real-GPU behavioral gate for the math builtins
// (exp/log/sqrt/rsqrt/sin/cos/tanh/abs/saturate/
// min/max/floor/ceil/atan/atan2/pow/fmod/clamp/mad — float scalar +
// float4 + int/uint classes). fastmath=false pins the precise MSL
// math path: fast::sin/cos carry an ABSOLUTE ~2^-11 error bound that
// would swamp the oracle tolerance, while the precise set stays
// within a few ULP of the CPU libm reference. abs/min/max/ saturate
// and the integer forms compare exact; transcendentals compare with
// fapprox.

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
        // as_type bitcast, valid-float bits
        c[i] += uint_bits_to_float((ua[i] & 0x007FFFFFu) | 0x3F800000u)
        // ! off the origin: Metal's atan2(0, 0) is NaN where libm answers 0
        c[i] += (((floor(x) + ceil(y)) + (atan(x) + atan2(y, x + 0.25)))
            + (pow(abs(x) + 1.0, 0.5 * y) + fmod(x, 3.0)))
        c[i] += clamp(x, -1.0, 1.0) + mad(x, y, 0.5)
        let u = va[i]
        let v = vb[i]
        vc[i] = (((exp(min(u, float4(4.0))) + sqrt(abs(u))) + (rsqrt(abs(v) + float4(1.0)) + tanh(u))) +
            ((sin(u) + cos(v)) + (saturate(u) + max(u, v))))
        vc[i] = vc[i] + round(v)
        vc[i] = (vc[i] + (floor(u) + ceil(v))) + (clamp(u, float4(-1.0), float4(1.0)) + mad(u, v, float4(0.5)))
        // 4-lane round-trip, bit-exact
        vc[i] = uint_bits_to_float(float_bits_to_uint(vc[i]))
        ic[i] = min(abs(ia[i]), ib[i]) + max(ia[i], ib[i])
        // negative floats bit-exact too
        ic[i] += float_bits_to_int(y)
        ic[i] += clamp(ia[i], -8, 8)
        uc[i] = min(ua[i], ub[i]) + max(ua[i], ub[i])
        uc[i] += float_bits_to_uint(x) & 0x7FFFFFFFu
        uc[i] += clamp(ua[i], 2u, 20u)
        qc[i] = int8(round(b[i] * 12.5))
    }
}

[test(description = "Math builtins run on the GPU with precise math match the CPU reference within tolerance.")]
def test_math_gpu_vs_cpu(t : T?) { // nolint:STYLE038 - linear GPU-vs-CPU comparison body
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
            oracle.va[k] = float4(float(k % 9) - 4.0, float(k % 5) * 0.75, float(k % 13) - 6.5,
                                  -float(k % 7))
            oracle.vb[k] = float4(float(k % 4) - 1.5, float(k % 6) + 0.25, -float(k % 3),
                                  float(k % 11) * 0.5)
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
        // k=0: x=-11, y=-4:
        // exp(-11)+sqrt(11)+rsqrt(5)+tanh(-11)+sin(-11)+cos(-4)
        // +saturate(-11)+max(-11,-4),
        // then += float(qa[0]) = -125, then += log(|x|+1) = log(12), then +=
        // exp2(-4) + log2(5.5), then += bits((0&0x7FFFFF)|0x3F800000) = 1.0,
        // then += floor(-11) + ceil(-4) + atan(-11) + atan2(-4, -10.75) +
        // 12^-2 + fmod(-11, 3), then += clamp(-11) + mad(-11, -4, 0.5)
        t |> success(fapprox(oracle.c[0], exp(-11.0) + sqrt(11.0) + rsqrt(5.0) + tanh(-11.0) +
            sin(-11.0) + cos(-4.0) + 0.0 + (-4.0) - 125.0 + log(12.0) + exp2(-4.0) + log2(5.5) + 1.0 +
            (-11.0 - 4.0 + atan(-11.0) + atan2(-4.0, -10.75) + 1.0 / 144.0 - 2.0) + (-1.0 + 44.5)),
            "the oracle itself computed")
        t |> equal(oracle.ic[0],
                   min(9, -3) + max(-9, -3) + float_bits_to_int(-4.0) + clamp(-9, -8, 8))
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, mathcalls_gpu_msl, mathcalls_gpu_msl_entry,
                                               mathcalls_gpu_msl_fastmath, perr)
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

// Real-GPU behavioral gate vs the CPU-reference oracle
// (ARCHITECTURE.md sec.4, layer 3). The kernel body is ordinary
// daslang, so the same method runs on the CPU: a driver loop sets
// gl_GlobalInvocationID and calls the method on an instance whose
// members are plain arrays — that run IS the expected output. The GPU
// result must match exactly (integer-valued floats). The CPU half
// runs on every platform; the GPU half is static_if-gated on
// das_metal (Apple), and skips cleanly (not fails) on Apple boxes
// with no Metal device (CI paravirtual fallback).

class MulAB {
    @ssbo @binding = 0 a : array<float>
    @ssbo @binding = 1 b : array<float>
    @ssbo @binding = 2 c : array<float>

    [metal_kernel(name="mul_ab_gpu_msl"), marker(no_coverage)]
    def mul_ab {
        let i = gl_GlobalInvocationID.x
        c[i] = a[i] * b[i]
    }
}

[test(description = "The emitted MSL of a*b run on the GPU equals a CPU run of the same method.")]
def test_mul_ab_gpu_vs_cpu(t : T?) {
    t |> run("a*b: emitted MSL on GPU == CPU-reference run of the same method") <| @(t : T?) {
        // CPU-reference oracle — every platform
        var oracle = new MulAB()
        oracle.a |> resize(N)
        oracle.b |> resize(N)
        oracle.c |> resize(N)
        for (i in range(N)) {
            oracle.a[i] = float(i)
            oracle.b[i] = float(i % 7 + 1)
        }
        for (i in range(N)) {
            gl_GlobalInvocationID = uint3(uint(i), 0u, 0u)
            oracle->mul_ab()
        }
        t |> equal(oracle.c[9], 27.0)   // 9 * (9%7+1 = 3) — the oracle itself computed
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, mul_ab_gpu_msl, mul_ab_gpu_msl_entry,
                                               mul_ab_gpu_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                let bytes = uint64(N * 4)
                var ba = metal_new_buffer(dev, bytes)
                var bb = metal_new_buffer(dev, bytes)
                var bc = metal_new_buffer(dev, bytes)
                var pa = unsafe(reinterpret<float?>(metal_buffer_contents(ba)))
                var pb = unsafe(reinterpret<float?>(metal_buffer_contents(bb)))
                var pc = unsafe(reinterpret<float?>(metal_buffer_contents(bc)))
                for (i in range(N)) {
                    unsafe {
                        pa[i] = oracle.a[i]
                        pb[i] = oracle.b[i]
                        pc[i] = 0.0
                    }
                }
                var bufs <- [ba, bb, bc]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = 0
                    for (i in range(N)) {
                        if (unsafe(pc[i]) != oracle.c[i]) {
                            gpu_bad++
                        }
                    }
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(ba)
                metal_release(bb)
                metal_release(bc)
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
