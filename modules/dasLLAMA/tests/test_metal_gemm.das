options gen2

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_math_default        // registers the "portable" backend at [init] (the oracle + fallback)
require ?das_metal dasllama/dasllama_metal_gemm  // registers the "metal" batch donor on Apple builds
require ?das_metal metal/das_metal_boost  // metal_live_object_count for the leak gate
require math   // nolint:STYLE030,LINT019 — used only inside the Apple static_if half

// The Metal GPU prefill-GEMM donor (Phase-5 dasMetal offload) vs the portable CPU kernel on
// synthetic Q8 data. Two comparisons: power-of-two scales make the GPU's dequant-to-half EXACT
// (int8 quants and int8 x pow2 products are exactly representable in half, block sums stay under
// 2^24 in the f32 accumulators) so the compare is bit-exact; arbitrary scales carry the half
// rounding of the folded scale product and compare under relative tolerance — the documented
// envelope of the f16-tile path (the same one ggml's Metal mul_mm accepts). Also exercised: the
// 32-row padding rail (ntok deliberately NOT a multiple of 32) and the small-ntok CPU delegation.
// Off-Apple (or with no Metal device) every check feints cleanly.

let N = 128     // K: weight row length, %32
let D = 64      // output rows, %32
let NTOK = 40   // deliberately unpadded — exercises the 32-row pad rail (M' = 64)

def private fill_case(var wq : array<int8>; var ws : array<float>; var xq : array<int8>; var xs : array<float>; pow2_scales : bool; n, d, ntok : int) {
    wq |> resize(d * n)
    ws |> resize(d * (n / 32))
    xq |> resize(ntok * n)
    xs |> resize(ntok * (n / 32))
    for (i in range(d * n)) {
        wq[i] = int8((i * 7) % 255 - 127)
    }
    for (i in range(ntok * n)) {
        xq[i] = int8((i * 11) % 255 - 127)
    }
    for (i in range(d * (n / 32))) {
        ws[i] = pow2_scales ? (((i % 3) == 0) ? 0.5 : (((i % 3) == 1) ? 0.25 : 2.0)) : (0.013 + 0.001 * float(i % 17))
    }
    for (i in range(ntok * (n / 32))) {
        xs[i] = pow2_scales ? (((i % 2) == 0) ? 1.0 : 0.125) : (0.021 + 0.003 * float(i % 13))
    }
}

def private run_batch(var y : array<float>; wq : array<int8>; ws : array<float>; xq : array<int8>; xs : array<float>; n, d, ntok : int) {
    y |> resize(ntok * d)
    matmul_q8q8_batch(y, wq, ws, 0l, xq, xs, int64(n), int64(d), int64(ntok))
}

[test]
def test_metal_gemm_vs_portable(t : T?) {   // nolint:STYLE038 — flat fixture build + one comparison cell per scale flavor
    t |> run("metal batch donor == portable CPU kernel on synthetic Q8 GEMM") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var found = false
            for (n in kernel_backend_names()) {
                if (n == "metal") {
                    found = true
                }
            }
            t |> success(found, "metal backend registered at [init]")
            var wq : array<int8>
            var ws : array<float>
            var xq : array<int8>
            var xs : array<float>
            var want : array<float>
            var got : array<float>
            // exact case: pow2 scales, GPU path forced (min_ntok 1)
            fill_case(wq, ws, xq, xs, true, N, D, NTOK)
            run_batch(want, wq, ws, xq, xs, N, D, NTOK)     // portable serves the batch slot
            select_batch_backend("metal")
            set_metal_gemm_min_ntok(1l)
            run_batch(got, wq, ws, xq, xs, N, D, NTOK)
            let st = metal_gemm_stats()
            if (st.dispatches == 0l) {
                // registered but no usable device (or compile failure) — the fallback served it;
                // results still match, but the GPU claims are not being tested
                select_batch_backend("")
                set_metal_gemm_min_ntok(32l)
                metal_gemm_shutdown()
                feint("metal backend fell back to CPU (no device?); GPU compare skipped\n")
                return
            }
            var bad_exact = 0
            for (k in range(NTOK * D)) {
                if (got[k] != want[k]) {
                    bad_exact++
                }
            }
            t |> equal(bad_exact, 0)                        // pow2 scales: bit-exact
            // 64x64-kernel case: a big output dim pushes the grid past the gemm_use64 threshold,
            // so this exact compare runs through the big-tile kernel (the D=64 case above stays
            // on the 32x32 kernel — both dispatch paths covered). Fresh fill sizes = fresh weight
            // arrays, so drop the pointer-keyed cache first.
            metal_gemm_shutdown()
            let D64 = 8192
            t |> success(gemm_use64(64l, int64(D64)), "test shape reaches the 64x64 kernel path")
            fill_case(wq, ws, xq, xs, true, N, D64, NTOK)
            select_batch_backend("portable")
            run_batch(want, wq, ws, xq, xs, N, D64, NTOK)
            select_batch_backend("metal")
            run_batch(got, wq, ws, xq, xs, N, D64, NTOK)
            var bad64 = 0
            for (k in range(NTOK * D64)) {
                if (got[k] != want[k]) {
                    bad64++
                }
            }
            t |> equal(bad64, 0)                            // pow2 scales: bit-exact on the 64 path too
            // tolerance case: arbitrary scales carry the dequant-to-half rounding. The refill
            // reuses the SAME arrays, so the pointer-keyed lazy weight cache must be dropped —
            // in production weights are load-once-stable (the documented cache contract). The
            // oracle run pins "portable" explicitly: select_batch_backend("") only clears the
            // standing pin — the slot restores at the NEXT whole-backend activation.
            metal_gemm_shutdown()
            fill_case(wq, ws, xq, xs, false, N, D, NTOK)
            select_batch_backend("portable")
            run_batch(want, wq, ws, xq, xs, N, D, NTOK)
            select_batch_backend("metal")
            run_batch(got, wq, ws, xq, xs, N, D, NTOK)
            // each dequantized half rounds at 2^-11 relative, so a product carries ~2^-10 of its
            // own magnitude — the error bound scales with the DOT ENVELOPE sum |w·x| (cancellation
            // makes |y| itself arbitrarily smaller), 2e-3 = 2x margin over the worst case
            var bad_tol = 0
            for (tok in range(NTOK)) {
                for (o in range(D)) {
                    var den = 0.0
                    for (k in range(N)) {
                        den += (abs(float(wq[o * N + k]) * ws[o * (N / 32) + k / 32]) *
                            abs(float(xq[tok * N + k]) * xs[tok * (N / 32) + k / 32]))
                    }
                    let idx = tok * D + o
                    if (abs(got[idx] - want[idx]) > 2e-3 * max(den, 1.0)) {
                        bad_tol++
                    }
                }
            }
            t |> equal(bad_tol, 0)                          // f16-tile envelope
            // small-ntok delegation: below the threshold the CPU fallback serves the call
            set_metal_gemm_min_ntok(1000l)
            let fb0 = metal_gemm_stats().fallbacks
            run_batch(got, wq, ws, xq, xs, N, D, NTOK)
            t |> success(metal_gemm_stats().fallbacks > fb0, "small batches delegate to the CPU kernel")
            var bad_fb = 0
            for (k in range(NTOK * D)) {
                if (got[k] != want[k]) {
                    bad_fb++
                }
            }
            t |> equal(bad_fb, 0)                           // the delegate IS the portable kernel
            // restore process-wide state (clear the pin AND re-activate to restore the slot) +
            // the leak gate
            select_batch_backend("")
            select_kernel_backend(active_kernel_backend())
            set_metal_gemm_min_ntok(32l)
            metal_gemm_shutdown()
            t |> equal(metal_log_leaks_if_any(), 0l)
            delete wq
            delete ws
            delete xq
            delete xs
            delete want
            delete got
        } else {
            feint("das_metal is not built on this platform; nothing to compare\n")
        }
    }
}
