options gen2
options indenting = 4

// Real-GPU behavioral gate vs the CPU-reference oracle for the Phase-2 arithmetic kernels
// (uint/int/float scalar classes + float4/int2 vectors). The kernel bodies are ordinary daslang:
// a driver loop feeds gl_GlobalInvocationID and runs the same method on the CPU — that run IS the
// expected output. Ints compare bit-exact (wrap/truncation semantics match C on both sides);
// floats with fastmath tolerance. The CPU half runs on every platform; the GPU half is
// static_if-gated on das_metal and skips cleanly where there is no Metal device.

require dastest/testing_boost public
require metal/msl_shader
require _metal_common

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

[test]
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
        t |> equal(oracle.c[0], 4u)     // x=3, y=1: (3+1)-(3*1) + (3/1)+(3%1) = 4 — the oracle itself computed
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, uarith_gpu_msl, uarith_gpu_msl_entry, uarith_gpu_msl_fastmath, perr)
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

[test]
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
                var pso = pipeline_from_source(dev, iarith_gpu_msl, iarith_gpu_msl_entry, iarith_gpu_msl_fastmath, perr)
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

[test]
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
        t |> success(fapprox(oracle.c[0], 1.5), "x=0, y=1.5: result is 1.5 — the oracle itself computed")
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, farith_gpu_msl, farith_gpu_msl_entry, farith_gpu_msl_fastmath, perr)
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

[test]
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
            oracle.fa[k] = float4(float(k % 9) - 4.0, float(k % 5), float(k % 3) + 0.5, -float(k % 7))
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
                var pso = pipeline_from_source(dev, vecarith_gpu_msl, vecarith_gpu_msl_entry, vecarith_gpu_msl_fastmath, perr)
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
