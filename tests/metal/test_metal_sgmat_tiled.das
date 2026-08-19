options gen2
options indenting = 4

// Real-GPU behavioral gate for Phase-5 simdgroup_matrix, tiled-GEMM layer — the offload kernel's
// shape: each threadgroup (32 threads = one simdgroup) owns one 8x8 C tile, walks K in 8-steps
// staging f16 A/B tiles into threadgroup memory, barriers, and runs the MIXED-precision mac (f32
// accumulators, f16 operands). A barrier kernel cannot be sequentially replayed (Phase-3 rule),
// so the oracle is the directly computed matmul over the widened halves; all values are small
// integers (every product exact in half, every K=32 sum exact in float), so the compare is
// BIT-EXACT.

require dastest/testing_boost public
require metal/msl_shader
require _metal_common  // nolint:STYLE030,LINT019 — used only inside the Apple static_if half; off-Apple lint sees the branch compiled out

let M = 16
let N = 16
let K = 32
let TPG = 32u           // one simdgroup per threadgroup, one C tile each

class SgMatTiledGpu {
    @ssbo @binding = 0 xa : array<float16>  // A [M x K] row-major
    @ssbo @binding = 1 wb : array<float16>  // B [K x N] row-major
    @ssbo @binding = 2 yo : array<float>    // C [M x N]
    @uniform @binding = 3 kdim : uint
    @uniform @binding = 4 ndim : uint
    @workgroup ta : float16[64]
    @workgroup tb : float16[64]

    [metal_kernel(name="sgmat_tiled_gpu_msl"), marker(no_coverage)]
    def sgmat_tiled_gpu {
        let lid = gl_LocalInvocationID.x
        let ntiles = ndim / 8u
        let tr = gl_WorkGroupID.x / ntiles
        let tc = gl_WorkGroupID.x % ntiles
        var acc : simdgroup_float8x8
        var kk = 0u
        while (kk < kdim) {
            let i0 = lid
            let i1 = lid + 32u
            ta[i0] = xa[(tr * 8u + (i0 / 8u)) * kdim + (kk + (i0 % 8u))]
            ta[i1] = xa[(tr * 8u + (i1 / 8u)) * kdim + (kk + (i1 % 8u))]
            tb[i0] = wb[(kk + (i0 / 8u)) * ndim + (tc * 8u + (i0 % 8u))]
            tb[i1] = wb[(kk + (i1 / 8u)) * ndim + (tc * 8u + (i1 % 8u))]
            barrier()
            var ma : simdgroup_half8x8
            var mb : simdgroup_half8x8
            simdgroup_load(ma, ta, 0u, 8u)
            simdgroup_load(mb, tb, 0u, 8u)
            simdgroup_multiply_accumulate(acc, ma, mb, acc)
            barrier()
            kk += 8u
        }
        simdgroup_store(acc, yo, (tr * 8u) * ndim + (tc * 8u), ndim)
    }
}

[test]
def test_sgmat_tiled_gpu_vs_cpu(t : T?) {
    t |> run("simdgroup_matrix tiled GEMM: GPU == direct CPU matmul, bit-exact") <| @(t : T?) {
        var a : array<float16>
        var b : array<float16>
        a |> resize(M * K)
        b |> resize(K * N)
        for (r in range(M)) {
            for (k in range(K)) {
                a[r * K + k] = float16(float((r + k * 3) % 7 - 3))
            }
        }
        for (k in range(K)) {
            for (c in range(N)) {
                b[k * N + c] = float16(float((k * 5 + c) % 9 - 4))
            }
        }
        var want : array<float>
        want |> resize(M * N)
        for (r in range(M)) {
            for (c in range(N)) {
                var s = 0.0
                for (k in range(K)) {
                    s += float(a[r * K + k]) * float(b[k * N + c])
                }
                want[r * N + c] = s
            }
        }
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, sgmat_tiled_gpu_msl, sgmat_tiled_gpu_msl_entry, sgmat_tiled_gpu_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var bxa = buf_upload(dev, a)
                var bwb = buf_upload(dev, b)
                var byo = buf_fill(dev, M * N, -1000.0)
                var bk = buf_fill(dev, 1, uint(K))
                var bn = buf_fill(dev, 1, uint(N))
                var bufs <- [bxa, bwb, byo, bk, bn]
                var rerr : string
                // (M/8)*(N/8) = 4 threadgroups of one simdgroup each — 128 threads
                let ran = run_compute_1d(queue, pso, bufs, uint((M / 8) * (N / 8)) * TPG, TPG, rerr, sgmat_tiled_gpu_msl_tgmem)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = buf_mismatch_exact(byo, want)
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(bxa)
                metal_release(bwb)
                metal_release(byo)
                metal_release(bk)
                metal_release(bn)
                metal_release(pso)
                metal_release(queue)
            }
            if (gpu_bad >= 0) {
                t |> equal(gpu_bad, 0)
            }
            t |> equal(metal_live_object_count(), 0l)
        } else {
            feint("das_metal is not built on this platform; nothing to compare\n")
        }
        delete a
        delete b
        delete want
    }
}
