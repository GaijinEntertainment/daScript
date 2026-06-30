options gen2
options persistent_heap

require dastest/testing_boost
require attn_gemm_variants
require math

// Isolation A/B for the fp32 micro-GEMM (C[M x N] += A[M x K] * B[K x N]) — the register-tiled
// broadcast-FMA kernel vs the scalar reference, single-thread, on the attention tile shapes
// (score: M=Qtile, K=head_size, N=KVtile; output: M=Qtile, K=KVtile, N=head_size). Correctness
// is gated first (max |tiled - ref| within fp tolerance — FMA contraction reorders low bits), then
// each variant is timed and reported as GMAC/s.
//
//   bin/daslang -jit dastest/dastest.das -- --test modules/dasLLAMA/benchmarks/attn/bench_attn_gemm.das --bench

def fillf(var a : array<float>; cnt : int64; seed : int64) {
    a |> resize(cnt)
    for (i in range64(cnt)) {
        a[i] = float((i + seed) % 17l) * 0.05 - 0.4
    }
}

def max_abs_diff(a, b : array<float>; cnt : int64) : float {
    var m = 0.0
    for (i in range64(cnt)) {
        m = max(m, abs(a[i] - b[i]))
    }
    return m
}

// GMAC/s for one variant. C is zeroed once, then `reps` accumulating calls are timed.
def time_ref(var c : array<float>; a, b : array<float>; M, K, N : int64; reps : int) : float {
    for (i in range64(M * N)) { c[i] = 0.0 }
    unsafe {
        gemm_ref(addr(c[0]), addr(a[0]), addr(b[0]), M, K, N)   // warmup
        let t0 = ref_time_ticks()
        for (_r in range(reps)) {
            gemm_ref(addr(c[0]), addr(a[0]), addr(b[0]), M, K, N)
        }
        let dt_ns = ref_time_ticks() - t0
        return float(double(M * K * N) * double(reps) / double(dt_ns))
    }
}

def time_tiled(var c : array<float>; a, b : array<float>; M, K, N : int64; reps : int) : float {
    for (i in range64(M * N)) { c[i] = 0.0 }
    unsafe {
        gemm_tiled(addr(c[0]), addr(a[0]), addr(b[0]), M, K, N)   // warmup
        let t0 = ref_time_ticks()
        for (_r in range(reps)) {
            gemm_tiled(addr(c[0]), addr(a[0]), addr(b[0]), M, K, N)
        }
        let dt_ns = ref_time_ticks() - t0
        return float(double(M * K * N) * double(reps) / double(dt_ns))
    }
}

def time_nofma(var c : array<float>; a, b : array<float>; M, K, N : int64; reps : int) : float {
    for (i in range64(M * N)) { c[i] = 0.0 }
    unsafe {
        gemm_tiled_nofma(addr(c[0]), addr(a[0]), addr(b[0]), M, K, N)
        let t0 = ref_time_ticks()
        for (_r in range(reps)) {
            gemm_tiled_nofma(addr(c[0]), addr(a[0]), addr(b[0]), M, K, N)
        }
        return float(double(M * K * N) * double(reps) / double(ref_time_ticks() - t0))
    }
}

def time_u2(var c : array<float>; a, b : array<float>; M, K, N : int64; reps : int) : float {
    for (i in range64(M * N)) { c[i] = 0.0 }
    unsafe {
        gemm_tiled_u2(addr(c[0]), addr(a[0]), addr(b[0]), M, K, N)
        let t0 = ref_time_ticks()
        for (_r in range(reps)) {
            gemm_tiled_u2(addr(c[0]), addr(a[0]), addr(b[0]), M, K, N)
        }
        return float(double(M * K * N) * double(reps) / double(ref_time_ticks() - t0))
    }
}

def bench_shape(name : string; M, K, N : int64; reps : int) {
    var a : array<float>
    var b : array<float>
    fillf(a, M * K, 1l)
    fillf(b, K * N, 7l)
    var cref : array<float>
    var ctil : array<float>
    cref |> resize(M * N)
    ctil |> resize(M * N)
    // correctness gate: one zeroed call each
    for (i in range64(M * N)) { cref[i] = 0.0; ctil[i] = 0.0 }
    unsafe {
        gemm_ref(addr(cref[0]), addr(a[0]), addr(b[0]), M, K, N)
        gemm_tiled(addr(ctil[0]), addr(a[0]), addr(b[0]), M, K, N)
    }
    let md = max_abs_diff(cref, ctil, M * N)
    verify(md < 1.0e-3)
    let gref = time_ref(cref, a, b, M, K, N, reps)
    let gnof = time_nofma(ctil, a, b, M, K, N, reps)
    let gtil = time_tiled(ctil, a, b, M, K, N, reps)
    let gu2 = time_u2(ctil, a, b, M, K, N, reps)
    to_log(LOG_INFO, "{name} M={M} K={K} N={N}  ref {gref}  nofma {gnof}  mad {gtil}  mad+u2 {gu2} GMAC/s  | mad/nofma {gtil / gnof}x  u2/mad {gu2 / gtil}x\n")
    delete a; delete b; delete cref; delete ctil
}

[benchmark, unused_argument(b)]
def bench_attn_gemm(b : B?) {
    to_log(LOG_INFO, "=== fp32 micro-GEMM: scalar ref vs 4x16 register tile (single thread) ===\n")
    bench_shape("score 64^3 ", 64l, 64l, 64l, 4000)     // QKᵀ: Qtile×head×KVtile
    bench_shape("out   64^3 ", 64l, 64l, 64l, 4000)     // P·V:  Qtile×KVtile×head (same shape @1B)
    bench_shape("score 32   ", 32l, 64l, 32l, 8000)     // smaller tile
    bench_shape("k128       ", 64l, 128l, 64l, 2000)    // head_size 128 (8B)
    bench_shape("wide  N256  ", 64l, 64l, 256l, 1000)   // longer KV tile
}
