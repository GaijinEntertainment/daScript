options gen2
options indenting = 4

// Real-GPU behavioral gate for the Phase-3 workgroup-grid builtins (thread_position_in_threadgroup,
// threadgroup_position_in_grid, threadgroups_per_grid, threads_per_threadgroup,
// thread_index_in_threadgroup). The kernel has no barrier and no simd ops, so the same-body
// CPU-reference replay stays valid: the driver walks the exact grid setting all six builtin
// globals per thread, and the GPU result must match bit-exactly.

require dastest/testing_boost public
require metal/msl_shader
require _metal_common  // nolint:STYLE030,LINT019 — used only inside the Apple static_if half; off-Apple lint sees the branch compiled out

let N = 512             // exact grid: 8 threadgroups of 64
let TPG = 64u
let GROUPS = 8u

class GridIds {
    @ssbo @binding = 0 code : array<uint>
    @ssbo @binding = 1 dims : array<uint>

    [metal_kernel(name="grid_ids_msl"), marker(no_coverage)]
    def grid_ids {
        let gid = gl_GlobalInvocationID.x
        code[gid] = ((gl_WorkGroupID.x * 1000u) + gl_LocalInvocationID.x) * 10000u + gl_LocalInvocationIndex
        dims[gid] = (gl_WorkGroupSize.x * 100000u) + gl_NumWorkGroups.x
    }
}

[test]
def test_grid_ids_gpu_vs_cpu(t : T?) {
    t |> run("workgroup-grid builtins: GPU == CPU-reference replay") <| @(t : T?) {
        var oracle = new GridIds()
        oracle.code |> resize(N)
        oracle.dims |> resize(N)
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            gl_LocalInvocationID = uint3(uint(k) % TPG, 0u, 0u)
            gl_LocalInvocationIndex = uint(k) % TPG
            gl_WorkGroupID = uint3(uint(k) / TPG, 0u, 0u)
            gl_WorkGroupSize = uint3(TPG, 1u, 1u)
            gl_NumWorkGroups = uint3(GROUPS, 1u, 1u)
            oracle->grid_ids()
        }
        // spot-check the replay itself: k=65 -> workgroup 1, local id/index 1
        t |> equal(oracle.code[0], 0u)
        t |> equal(oracle.code[65], 10010001u)
        t |> equal(oracle.dims[0], 6400008u)
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, grid_ids_msl, grid_ids_msl_entry, grid_ids_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var bcode = buf_fill(dev, N, 0u)
                var bdims = buf_fill(dev, N, 0u)
                var bufs <- [bcode, bdims]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = buf_mismatch_exact(bcode, oracle.code) + buf_mismatch_exact(bdims, oracle.dims)
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(bcode)
                metal_release(bdims)
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
