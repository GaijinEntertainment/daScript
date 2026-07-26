options gen2
options indenting = 4

// Packed 4x8-bit integer dots (shader_lingua_franca sdot4/usdot4) on real GPU: each lowers to a
// __das_sdot4/__das_usdot4 preamble helper (as_type widen-mul — MSL has no dp4a builtin). Pure
// integer math, so GPU must equal the CPU-replayed reference EXACTLY. The emitted-source asserts
// run on every platform (the MSL string is generated at compile time); the dispatch is Apple-only.

require dastest/testing_boost public
require strings
require metal/msl_shader
require _metal_common  // nolint:STYLE030 — used only inside the Apple static_if half; off-Apple lint sees the branch compiled out

let N = 256
let TPG = 64u

class SdotKernel {
    @ssbo @binding = 0 wa : array<uint>
    @ssbo @binding = 1 wb : array<uint>
    @ssbo @binding = 2 os : array<int>
    @ssbo @binding = 3 ou : array<int>

    [metal_kernel(name="sdot_gpu_msl"), marker(no_coverage)]
    def sdot_k {
        let i = gl_GlobalInvocationID.x
        os[i] = sdot4(wa[i], wb[i]) + sdot4(wa[i], 0x01010101u)
        ou[i] = usdot4(wa[i], wb[i])
    }
}

[test]
def test_sdot_gpu_vs_cpu(t : T?) {
    t |> run("sdot4/usdot4: helper lowering + GPU == CPU-reference exact") <| @(t : T?) {
        // the preamble helpers are injected (sorted, before the kernel signature) and the calls
        // route through them — checkable from the generated source on any platform
        t |> success(find(sdot_gpu_msl, "static inline int __das_sdot4(uint a, uint b)") >= 0, "sdot4 helper in preamble")
        t |> success(find(sdot_gpu_msl, "static inline int __das_usdot4(uint a, uint b)") >= 0, "usdot4 helper in preamble")
        t |> success(find(sdot_gpu_msl, "__das_usdot4(") < find(sdot_gpu_msl, "kernel void"), "helpers precede the kernel")
        var oracle = new SdotKernel()
        oracle.wa |> resize(N)
        oracle.wb |> resize(N)
        oracle.os |> resize(N)
        oracle.ou |> resize(N)
        for (k in range(N)) {
            // full byte range in both words (0x80/0xFF lanes exercise sign- vs zero-extension)
            oracle.wa[k] = pack32(ubyte4(uint(k % 256), uint((k * 3 + 1) % 256), uint((k * 7 + 2) % 256), uint((k * 11 + 3) % 256)))
            oracle.wb[k] = uint(pack32(byte4(k % 251 - 125, (k * 5) % 251 - 125, (k * 13 + 7) % 251 - 125, (k * 17 + 11) % 251 - 125)))
        }
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            oracle->sdot_k()
        }
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, sdot_gpu_msl, sdot_gpu_msl_entry, sdot_gpu_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var bwa = buf_upload(dev, oracle.wa)
                var bwb = buf_upload(dev, oracle.wb)
                var bos = buf_fill(dev, N, 0)
                var bou = buf_fill(dev, N, 0)
                var bufs <- [bwa, bwb, bos, bou]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = buf_mismatch_exact(bos, oracle.os) + buf_mismatch_exact(bou, oracle.ou)
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(bwa)
                metal_release(bwb)
                metal_release(bos)
                metal_release(bou)
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
