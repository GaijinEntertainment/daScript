options gen2
options indenting = 4

// Real-GPU behavioral gate for Phase-5 simdgroup_matrix, single-tile layer: 8x8 loads at a
// runtime offset/stride (uniform-fed), multiply, multiply-accumulate, and stores — f32 and f16 —
// vs the same-body CPU replay. Every sgmat call is the FULL cooperative tile op in
// simdgroup-uniform flow and the reference body performs the whole op per call, so replaying the
// lanes sequentially on the CPU is idempotent; all values are small integers, every product and
// 8-term sum is exactly representable in half and float, so accumulation order cannot matter and
// the compare is BIT-EXACT.

require dastest/testing_boost public
require metal/msl_shader
require _metal_common  // nolint:STYLE030,LINT019 — used only inside the Apple static_if half; off-Apple lint sees the branch compiled out

let TPG = 32u           // one simdgroup on Apple silicon

class SgMatGpu {
    @ssbo @binding = 0 fa : array<float>    // 8 rows x 16 cols — the kernel loads the RIGHT half tile
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
        var ma : simdgroup_float8x8
        var mb : simdgroup_float8x8
        simdgroup_load(ma, fa, off_a, stride_a)
        simdgroup_load(mb, fb, 0u, 8u)
        var mm : simdgroup_float8x8
        simdgroup_multiply(mm, ma, mb)
        simdgroup_store(mm, fc, 0u, 8u)
        var acc = make_filled_simdgroup_float8x8(1.0)
        simdgroup_multiply_accumulate(acc, ma, mb, acc)
        simdgroup_store(acc, fd, 0u, 8u)
        var hx : simdgroup_half8x8
        var hy : simdgroup_half8x8
        simdgroup_load(hx, ha, 0u, 8u)
        simdgroup_load(hy, hb, 0u, 8u)
        var hacc = make_filled_simdgroup_half8x8(0.5h)
        simdgroup_multiply_accumulate(hacc, hx, hy, hacc)
        simdgroup_store(hacc, hc, 0u, 8u)
    }
}

[test]
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
        // no builtin ID is read — each replayed lane redoes the full tile ops (idempotent)
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
                var pso = pipeline_from_source(dev, sgmat_gpu_msl, sgmat_gpu_msl_entry, sgmat_gpu_msl_fastmath, perr)
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
