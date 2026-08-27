options gen2
options indenting = 4

// Real-GPU behavioral gate vs the CPU-reference oracle (ARCHITECTURE.md sec.4, layer 3).
// The kernel body is ordinary daslang, so the same method runs on the CPU: a driver loop sets
// gl_GlobalInvocationID and calls the method on an instance whose members are plain arrays —
// that run IS the expected output. The GPU result must match exactly (integer-valued floats).
// The CPU half runs on every platform; the GPU half is static_if-gated on das_metal (Apple),
// and skips cleanly (not fails) on Apple boxes with no Metal device (CI paravirtual fallback).

require dastest/testing_boost public
require metal/msl_shader
require ?das_metal metal/das_metal_boost

let N = 256
let TPG = 64u

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

[test]
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
                var pso = pipeline_from_source(dev, mul_ab_gpu_msl, mul_ab_gpu_msl_entry, mul_ab_gpu_msl_fastmath, perr)
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
