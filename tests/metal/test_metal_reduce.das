options gen2
options indenting = 4

// Real-GPU behavioral gate for Phase-3 threadgroup memory + barriers: the classic tree reduction —
// each threadgroup loads 64 elements into a @workgroup tile, barriers, halves, and thread 0 writes
// the partial. A barrier kernel cannot be replayed by the sequential same-body driver (thread 0
// would reduce before the other lanes ever load), so the CPU reference here is the directly
// computed per-group sum: uint wrap-exact, float under the fastmath tolerance. A missing or
// misplaced barrier shows up as partials built from stale tile lanes.

require dastest/testing_boost public
require metal/msl_shader
require _metal_common  // nolint:STYLE030 — used only inside the Apple static_if half; off-Apple lint sees the branch compiled out

let N = 4096            // exact grid: 64 threadgroups of 64
let TPG = 64u
let GROUPS = 64

class Reduce {
    @ssbo @binding = 0 usrc : array<uint>
    @ssbo @binding = 1 udst : array<uint>
    @ssbo @binding = 2 fsrc : array<float>
    @ssbo @binding = 3 fdst : array<float>
    @workgroup utile : uint[64]
    @workgroup ftile : float[64]

    [metal_kernel(name="reduce_u_gpu_msl"), marker(no_coverage)]
    def reduce_u {
        let gid = gl_GlobalInvocationID.x
        let lid = gl_LocalInvocationID.x
        utile[lid] = usrc[gid]
        barrier()
        var s = gl_WorkGroupSize.x / 2u
        while (s > 0u) {
            if (lid < s) {
                utile[lid] += utile[lid + s]
            }
            barrier()
            s /= 2u
        }
        if (lid == 0u) {
            udst[gl_WorkGroupID.x] = utile[0]
        }
    }

    [metal_kernel(name="reduce_f_gpu_msl"), marker(no_coverage)]
    def reduce_f {
        let gid = gl_GlobalInvocationID.x
        let lid = gl_LocalInvocationID.x
        ftile[lid] = fsrc[gid]
        barrier()
        var s = gl_WorkGroupSize.x / 2u
        while (s > 0u) {
            if (lid < s) {
                ftile[lid] += ftile[lid + s]
            }
            barrier()
            s /= 2u
        }
        if (lid == 0u) {
            fdst[gl_WorkGroupID.x] = ftile[0]
        }
    }
}

[test]
def test_reduce_gpu_vs_cpu(t : T?) { // nolint:STYLE038 - linear GPU-vs-CPU comparison body
    t |> run("threadgroup tree reduction: GPU partials == directly computed sums") <| @(t : T?) {
        var usrc : array<uint>
        var fsrc : array<float>
        usrc |> resize(N)
        fsrc |> resize(N)
        for (k in range(N)) {
            usrc[k] = uint(k) * 2654435761u + 12345u    // wraps — bit-exact on both sides
            fsrc[k] = float(k % 31) * 0.125 + 0.5
        }
        var wantU : array<uint>
        var wantF : array<float>
        wantU |> resize(GROUPS)
        wantF |> resize(GROUPS)
        for (g in range(GROUPS)) {
            var su = 0u
            var sf = 0.0
            for (k in range(int(TPG))) {
                su += usrc[g * int(TPG) + k]
                sf += fsrc[g * int(TPG) + k]
            }
            wantU[g] = su
            wantF[g] = sf
        }
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var psoU = pipeline_from_source(dev, reduce_u_gpu_msl, reduce_u_gpu_msl_entry, reduce_u_gpu_msl_fastmath, perr)
                t |> success(psoU != null, "pipeline_from_source(u): {perr}")
                var psoF = pipeline_from_source(dev, reduce_f_gpu_msl, reduce_f_gpu_msl_entry, reduce_f_gpu_msl_fastmath, perr)
                t |> success(psoF != null, "pipeline_from_source(f): {perr}")
                if (psoU == null || psoF == null) {
                    if (psoU != null) {
                        metal_release(psoU)
                    }
                    if (psoF != null) {
                        metal_release(psoF)
                    }
                    return
                }
                var queue = metal_new_command_queue(dev)
                var busrc = buf_upload(dev, usrc)
                var budst = buf_fill(dev, GROUPS, 0xABAD1DEA)
                var bfsrc = buf_upload(dev, fsrc)
                var bfdst = buf_fill(dev, GROUPS, -1.0)
                var bufs <- [busrc, budst, bfsrc, bfdst]
                var rerr : string
                let ranU = run_compute_1d(queue, psoU, bufs, uint(N), TPG, rerr, reduce_u_gpu_msl_tgmem)
                t |> success(ranU, "run_compute_1d(u): {rerr}")
                let ranF = run_compute_1d(queue, psoF, bufs, uint(N), TPG, rerr, reduce_f_gpu_msl_tgmem)
                t |> success(ranF, "run_compute_1d(f): {rerr}")
                if (ranU && ranF) {
                    gpu_bad = buf_mismatch_exact(budst, wantU) + buf_mismatch_approx(bfdst, wantF)
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(busrc)
                metal_release(budst)
                metal_release(bfsrc)
                metal_release(bfdst)
                metal_release(psoU)
                metal_release(psoF)
                metal_release(queue)
            }
            if (gpu_bad >= 0) {
                t |> equal(gpu_bad, 0)
            }
            t |> equal(metal_live_object_count(), 0l)
        } else {
            feint("das_metal is not built on this platform; nothing to compare\n")
        }
        delete usrc
        delete fsrc
        delete wantU
        delete wantF
    }
}
