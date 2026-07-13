options gen2
options indenting = 4

// Real-GPU behavioral gate for the Phase-7 rolled-GEMM constructs: simdgroup-matrix ARRAYS,
// unroll_range loops (rolled-with-pragma MSL), and fixed-array staging locals. One threadgroup
// (32 threads = one simdgroup) computes C(16x8) = A(16xK) x B(Kx8) with two f32 accumulator
// tiles over f16 operands. Barrier kernel — the oracle is the directly computed matmul over
// the widened halves; all values small integers, so the compare is BIT-EXACT.

require dastest/testing_boost public
require metal/msl_shader
require _metal_common  // nolint:STYLE030 — used only inside the Apple static_if half; off-Apple lint sees the branch compiled out

let M = 16
let N = 8
let K = 32
let TPG = 32u           // one simdgroup, two C tiles

class SgMatRolledGpu {
    @ssbo @binding = 0 xa : array<float16>  // A [M x K] row-major
    @ssbo @binding = 1 wb : array<float16>  // B [K x N] row-major
    @ssbo @binding = 2 yo : array<float>    // C [M x N]
    @uniform @binding = 3 kdim : uint
    @workgroup ta : float16[128]            // A slab: 2 row-tiles of 8x8
    @workgroup tb : float16[64]             // B slab: one 8x8 tile

    [metal_kernel(name="sgmat_rolled_gpu_msl"), marker(no_coverage)]
    def sgmat_rolled_gpu {
        let lid = gl_LocalInvocationID.x
        var mc : simdgroup_float8x8[2]
        var ma : simdgroup_half8x8[2]
        var va : float16[4]
        var kk = 0u
        while (kk < kdim) {
            // this thread's 4 A elements are contiguous in xa — read through a device cursor
            let flat0 = lid * 4u
            let xcur = dev_cursor(xa, ((flat0 / 64u) * 8u + (flat0 % 64u) / 8u) * kdim + kk + (flat0 % 8u))
            for (i in unroll_range(4)) {
                va[i] = xa[xcur + uint(i)]
            }
            for (i in unroll_range(4)) {
                ta[lid * 4u + uint(i)] = va[i]
            }
            tb[lid] = wb[(kk + lid / 8u) * 8u + (lid % 8u)]
            tb[lid + 32u] = wb[(kk + (lid + 32u) / 8u) * 8u + ((lid + 32u) % 8u)]
            barrier()
            var mbb : simdgroup_half8x8
            simdgroup_load(mbb, tb, 0u, 8u)
            var acur = tg_cursor(ta, 0u)
            for (i in unroll_range(2)) {
                simdgroup_load(ma[i], ta, acur, 8u)
                simdgroup_multiply_accumulate(mc[i], ma[i], mbb, mc[i])
                acur += 64u
            }
            barrier()
            kk += 8u
        }
        for (i in unroll_range(2)) {
            simdgroup_store(mc[i], yo, uint(i) * 64u, 8u)
        }
    }
}

[test]
def test_sgmat_rolled_gpu_vs_cpu(t : T?) {
    t |> run("rolled GEMM (matrix arrays + unroll_range): GPU == direct CPU matmul, bit-exact") <| @(t : T?) {
        var a : array<float16>
        var b : array<float16>
        a |> resize(M * K)
        b |> resize(K * N)
        for (r in range(M)) {
            for (k in range(K)) {
                a[r * K + k] = float16(float((r * 2 + k) % 7 - 3))
            }
        }
        for (k in range(K)) {
            for (c in range(N)) {
                b[k * N + c] = float16(float((k + c * 5) % 9 - 4))
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
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, sgmat_rolled_gpu_msl, sgmat_rolled_gpu_msl_entry, sgmat_rolled_gpu_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var bxa = buf_upload(dev, a)
                var bwb = buf_upload(dev, b)
                var byo = buf_fill(dev, M * N, -1000.0)
                var bk = buf_fill(dev, 1, uint(K))
                var bufs <- [bxa, bwb, byo, bk]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, TPG, TPG, rerr)
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
