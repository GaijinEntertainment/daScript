options gen2

require dastest/testing_boost public
require metal/msl_shader
require _metal_common  // nolint:STYLE030,LINT019 — used only inside the Apple static_if half; off-Apple lint sees the branch compiled out

// Real-GPU behavioral gate for Phase-5 simdgroup_matrix, single-tile
// layer: 8x8 loads at a runtime offset/stride (uniform-fed),
// multiply, multiply-accumulate, and stores — f32 and f16 — vs the
// same-body CPU replay. Every sgmat call is the FULL cooperative tile
// op in simdgroup-uniform flow and the reference body performs the
// whole op per call, so replaying the lanes sequentially on the CPU
// is idempotent; all values are small integers, every product and
// 8-term sum is exactly representable in half and float, so
// accumulation order cannot matter and the compare is BIT-EXACT.
let TPG = 32u           // one simdgroup on Apple silicon

class SgMatGpu {
    // 8 rows x 16 cols — the kernel loads the RIGHT half tile
    @ssbo @binding = 0 fa : array<float>
    @ssbo @binding = 1 fb : array<float>    // 8x8
    @ssbo @binding = 2 fc : array<float>    // ma * mb
    @ssbo @binding = 3 fd : array<float>    // ma * mb + 1
    @ssbo @binding = 4 ha : array<float16>  // 8x8
    @ssbo @binding = 5 hb : array<float16>  // 8x8
    @ssbo @binding = 6 hc : array<float16>  // hx * hy + 0.5
    @uniform @binding = 7 off_a : uint      // 8 — the right-half column offset
    @uniform @binding = 8 stride_a : uint   // 16 — the wide source's row stride

    [metal_kernel(name="sgmat_gpu_msl"), marker(no_coverage)]
    def sgmat_gpu {
        var ma, mb : simdgroup_float8x8
        simdgroup_load(ma, fa, off_a, stride_a)
        simdgroup_load(mb, fb, 0u, 8u)
        var mm : simdgroup_float8x8
        simdgroup_multiply(mm, ma, mb)
        simdgroup_store(mm, fc, 0u, 8u)
        var acc = make_filled_simdgroup_float8x8(1.0)
        simdgroup_multiply_accumulate(acc, ma, mb, acc)
        simdgroup_store(acc, fd, 0u, 8u)
        var hx, hy : simdgroup_half8x8
        simdgroup_load(hx, ha, 0u, 8u)
        simdgroup_load(hy, hb, 0u, 8u)
        var hacc = make_filled_simdgroup_half8x8(0.5h)
        simdgroup_multiply_accumulate(hacc, hx, hy, hacc)
        simdgroup_store(hacc, hc, 0u, 8u)
    }
}

[test(description = "A single simdgroup_matrix tile on the GPU is bit-exact with the CPU replay.")]
def test_sgmat_gpu_vs_cpu(t : T?) { // nolint:STYLE038 - linear GPU-vs-CPU comparison body
    t |> run("simdgroup_matrix single tile: GPU == CPU replay, bit-exact") <| @(t : T?) {
        var oracle = new SgMatGpu()
        oracle.fa |> resize(8 * 16)
        oracle.fb |> resize(64)
        oracle.fc |> resize(64)
        oracle.fd |> resize(64)
        oracle.ha |> resize(64)
        oracle.hb |> resize(64)
        oracle.hc |> resize(64)
        for (r in range(8)) {
            for (c in range(16)) {
                oracle.fa[r * 16 + c] = float((r * 3 + c * 5) % 13 - 6)
            }
        }
        for (k in range(64)) {
            oracle.fb[k] = float((k * 5) % 11 - 5)
            oracle.ha[k] = float16(float((k * 3) % 9 - 4))
            oracle.hb[k] = float16(float((k * 7) % 7 - 3))
        }
        oracle.off_a = 8u
        oracle.stride_a = 16u
        // no builtin ID is read — each replayed lane redoes the full tile ops
        // (idempotent)
        for (_lane in range(int(TPG))) {
            oracle->sgmat_gpu()
        }
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, sgmat_gpu_msl, sgmat_gpu_msl_entry,
                                               sgmat_gpu_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var bfa = buf_upload(dev, oracle.fa)
                var bfb = buf_upload(dev, oracle.fb)
                var bfc = buf_fill(dev, 64, -1000.0)
                var bfd = buf_fill(dev, 64, -1000.0)
                var bha = buf_upload(dev, oracle.ha)
                var bhb = buf_upload(dev, oracle.hb)
                var bhc = buf_fill(dev, 64, float16(-1000.0))
                var boff = buf_fill(dev, 1, 8u)
                var bstride = buf_fill(dev, 1, 16u)
                var bufs <- [bfa, bfb, bfc, bfd, bha, bhb, bhc, boff, bstride]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, TPG, TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = ((buf_mismatch_exact(bfc, oracle.fc) + buf_mismatch_exact(bfd, oracle.fd)) +
                        buf_mismatch_exact(bhc, oracle.hc))
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(bfa)
                metal_release(bfb)
                metal_release(bfc)
                metal_release(bfd)
                metal_release(bha)
                metal_release(bhb)
                metal_release(bhc)
                metal_release(boff)
                metal_release(bstride)
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
        unsafe {
            delete oracle
        }
    }
}

// Real-GPU behavioral gate for the Phase-7 rolled-GEMM constructs:
// simdgroup-matrix ARRAYS, [unroll_full] loops (rolled-with-pragma
// MSL), and fixed-array staging locals. One threadgroup (32 threads =
// one simdgroup) computes C(16x8) = A(16xK) x B(Kx8) with two f32
// accumulator tiles over f16 operands. Barrier kernel — the oracle is
// the directly computed matmul over the widened halves; all values
// small integers, so the compare is BIT-EXACT.
let M = 16
let N = 8
let K = 32

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
            // this thread's 4 A elements are contiguous in xa — read through a
            // das device pointer
            let flat0 = lid * 4u
            let xcur = unsafe(addr(xa[((flat0 / 64u) * 8u + (flat0 % 64u) / 8u) * kdim + kk + (flat0 % 8u)]))
            for [unroll_full] (i in range(4)) {
                va[i] = unsafe(xcur[i])
            }
            for [unroll_full] (i in range(4)) {
                ta[lid * 4u + uint(i)] = va[i]
            }
            tb[lid] = wb[(kk + lid / 8u) * 8u + (lid % 8u)]
            tb[lid + 32u] = wb[(kk + (lid + 32u) / 8u) * 8u + ((lid + 32u) % 8u)]
            barrier()
            var mbb : simdgroup_half8x8
            simdgroup_load(mbb, tb, 0u, 8u)
            var acur = unsafe(addr(ta[0]))
            for [unroll_full] (i in range(2)) {
                simdgroup_load(ma[i], acur, 0u, 8u)
                simdgroup_multiply_accumulate(mc[i], ma[i], mbb, mc[i])
                acur = unsafe(acur + 64)
            }
            barrier()
            kk += 8u
        }
        for [unroll_full] (i in range(2)) {
            simdgroup_store(mc[i], yo, uint(i) * 64u, 8u)
        }
    }
}

[test(description = "A rolled GEMM with matrix arrays under unroll_full is bit-exact with a direct CPU matmul.")]
def test_sgmat_rolled_gpu_vs_cpu(t : T?) {
    t |> run("rolled GEMM (matrix arrays + [unroll_full]): GPU == direct CPU matmul, bit-exact") <| @(t : T?) {
        var a, b : array<float16>
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
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, sgmat_rolled_gpu_msl,
                                               sgmat_rolled_gpu_msl_entry,
                    sgmat_rolled_gpu_msl_fastmath, perr)
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
                let ran = run_compute_1d(queue, pso, bufs, TPG, TPG, rerr,
                                         sgmat_rolled_gpu_msl_tgmem)
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

// Real-GPU behavioral gate for Phase-5 simdgroup_matrix, tiled-GEMM
// layer — the offload kernel's shape: each threadgroup (32 threads =
// one simdgroup) owns one 8x8 C tile, walks K in 8-steps staging f16
// A/B tiles into threadgroup memory, barriers, and runs the
// MIXED-precision mac (f32 accumulators, f16 operands). A barrier
// kernel cannot be sequentially replayed (Phase-3 rule), so the
// oracle is the directly computed matmul over the widened halves; all
// values are small integers (every product exact in half, every K=32
// sum exact in float), so the compare is BIT-EXACT.
let N_sgmat_tiled = 16

class SgMatTiledGpu {
    @ssbo @binding = 0 xa : array<float16>  // A [M x K] row-major
    @ssbo @binding = 1 wb : array<float16>  // B [K x N_sgmat_tiled] row-major
    @ssbo @binding = 2 yo : array<float>    // C [M x N_sgmat_tiled]
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
            var ma, mb : simdgroup_half8x8
            simdgroup_load(ma, ta, 0u, 8u)
            simdgroup_load(mb, tb, 0u, 8u)
            simdgroup_multiply_accumulate(acc, ma, mb, acc)
            barrier()
            kk += 8u
        }
        simdgroup_store(acc, yo, (tr * 8u) * ndim + (tc * 8u), ndim)
    }
}

[test(description = "A tiled simdgroup_matrix GEMM is bit-exact with a direct CPU matmul.")]
def test_sgmat_tiled_gpu_vs_cpu(t : T?) {  // nolint:STYLE038 - one arm per tile shape
    t |> run("simdgroup_matrix tiled GEMM: GPU == direct CPU matmul, bit-exact") <| @(t : T?) {
        var a, b : array<float16>
        a |> resize(M * K)
        b |> resize(K * N_sgmat_tiled)
        for (r in range(M)) {
            for (k in range(K)) {
                a[r * K + k] = float16(float((r + k * 3) % 7 - 3))
            }
        }
        for (k in range(K)) {
            for (c in range(N_sgmat_tiled)) {
                b[k * N_sgmat_tiled + c] = float16(float((k * 5 + c) % 9 - 4))
            }
        }
        var want : array<float>
        want |> resize(M * N_sgmat_tiled)
        for (r in range(M)) {
            for (c in range(N_sgmat_tiled)) {
                var s = 0.0
                for (k in range(K)) {
                    s += float(a[r * K + k]) * float(b[k * N_sgmat_tiled + c])
                }
                want[r * N_sgmat_tiled + c] = s
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
                var pso = pipeline_from_source(dev, sgmat_tiled_gpu_msl, sgmat_tiled_gpu_msl_entry,
                                               sgmat_tiled_gpu_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var bxa = buf_upload(dev, a)
                var bwb = buf_upload(dev, b)
                var byo = buf_fill(dev, M * N_sgmat_tiled, -1000.0)
                var bk = buf_fill(dev, 1, uint(K))
                var bn = buf_fill(dev, 1, uint(N_sgmat_tiled))
                var bufs <- [bxa, bwb, byo, bk, bn]
                var rerr : string
                // (M/8)*(N_sgmat_tiled/8) = 4 threadgroups of one simdgroup each —
                // 128 threads
                let ran = run_compute_1d(queue, pso, bufs,
                                         uint((M / 8) * (N_sgmat_tiled / 8)) * TPG, TPG, rerr,
                    sgmat_tiled_gpu_msl_tgmem)
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
