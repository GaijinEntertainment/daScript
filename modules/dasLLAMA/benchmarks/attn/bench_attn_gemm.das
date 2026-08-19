options gen2
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost
require attn_gemm_variants
require dasllama/dasllama_math   // engine gemm_f32 + softmax for the head emulation
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

def time_8x8(var c : array<float>; a, b : array<float>; M, K, N : int64; reps : int) : float {
    for (i in range64(M * N)) { c[i] = 0.0 }
    unsafe {
        gemm_tiled_8x8(addr(c[0]), addr(a[0]), addr(b[0]), M, K, N)
        let t0 = ref_time_ticks()
        for (_r in range(reps)) {
            gemm_tiled_8x8(addr(c[0]), addr(a[0]), addr(b[0]), M, K, N)
        }
        return float(double(M * K * N) * double(reps) / double(ref_time_ticks() - t0))
    }
}

def time_12x8(var c : array<float>; a, b : array<float>; M, K, N : int64; reps : int) : float {
    for (i in range64(M * N)) { c[i] = 0.0 }
    unsafe {
        gemm_tiled_12x8(addr(c[0]), addr(a[0]), addr(b[0]), M, K, N)
        let t0 = ref_time_ticks()
        for (_r in range(reps)) {
            gemm_tiled_12x8(addr(c[0]), addr(a[0]), addr(b[0]), M, K, N)
        }
        return float(double(M * K * N) * double(reps) / double(ref_time_ticks() - t0))
    }
}

def time_6x16(var c : array<float>; a, b : array<float>; M, K, N : int64; reps : int) : float {
    for (i in range64(M * N)) { c[i] = 0.0 }
    unsafe {
        gemm_tiled_6x16(addr(c[0]), addr(a[0]), addr(b[0]), M, K, N)
        let t0 = ref_time_ticks()
        for (_r in range(reps)) {
            gemm_tiled_6x16(addr(c[0]), addr(a[0]), addr(b[0]), M, K, N)
        }
        return float(double(M * K * N) * double(reps) / double(ref_time_ticks() - t0))
    }
}

def gate_variant(cref : array<float>; var ctry : array<float>; a, b : array<float>; M, K, N : int64; which : int) {
    for (i in range64(M * N)) {
        ctry[i] = 0.0
    }
    unsafe {
        if (which == 0) {
            gemm_tiled(addr(ctry[0]), addr(a[0]), addr(b[0]), M, K, N)
        } elif (which == 1) {
            gemm_tiled_8x8(addr(ctry[0]), addr(a[0]), addr(b[0]), M, K, N)
        } elif (which == 2) {
            gemm_tiled_12x8(addr(ctry[0]), addr(a[0]), addr(b[0]), M, K, N)
        } else {
            gemm_tiled_6x16(addr(ctry[0]), addr(a[0]), addr(b[0]), M, K, N)
        }
    }
    verify(max_abs_diff(cref, ctry, M * N) < 1.0e-3)
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
    // correctness gate: every variant vs the scalar reference before any timing
    for (i in range64(M * N)) { cref[i] = 0.0 }
    unsafe {
        gemm_ref(addr(cref[0]), addr(a[0]), addr(b[0]), M, K, N)
    }
    for (which in range(4)) {
        gate_variant(cref, ctil, a, b, M, K, N, which)
    }
    let gref = time_ref(cref, a, b, M, K, N, reps)
    let gnof = time_nofma(ctil, a, b, M, K, N, reps)
    let gtil = time_tiled(ctil, a, b, M, K, N, reps)
    let gu2 = time_u2(ctil, a, b, M, K, N, reps)
    let g88 = time_8x8(ctil, a, b, M, K, N, reps)
    let g128 = time_12x8(ctil, a, b, M, K, N, reps)
    let g616 = time_6x16(ctil, a, b, M, K, N, reps)
    to_log(LOG_INFO, "{name} M={M} K={K} N={N}  ref {gref}  nofma {gnof}  4x16 {gtil}  u2 {gu2}  8x8 {g88}  12x8 {g128}  6x16 {g616} GMAC/s  | best-tall/4x16 {max(g88, max(g128, g616)) / gtil}x\n")
    delete a; delete b; delete cref; delete ctil
}

// One parakeet rel-pos attention head, engine block-loop verbatim (minus the transposes/scatter —
// those are the packing slice): per Q-row block, zero + scores GEMM + rel GEMM + combine/softmax
// + P·V GEMM. Returns ms per head. `jo` picks the jj-outer tile walk; Q=4 + ii-outer IS the
// current engine shape. Full scratch matrices like ParakeetState (scores tt×tt, rel tt×ww).
def head_emu_ms(Q, tt, dh : int64; jo : bool; rounds : int) : double {
    let ww = 2l * tt - 1l
    let scale = 1.0 / sqrt(float(dh))
    var qu : array<float>
    var qv : array<float>
    var kt : array<float>
    var pop : array<float>
    var vh : array<float>
    var sc : array<float>
    var rl : array<float>
    var oh : array<float>
    fillf(qu, tt * dh, 1l)
    fillf(qv, tt * dh, 3l)
    fillf(kt, dh * tt, 7l)
    fillf(pop, dh * ww, 11l)
    fillf(vh, tt * dh, 13l)
    sc |> resize(tt * tt)
    rl |> resize(tt * ww)
    oh |> resize(tt * dh)
    var best = 1e30lf
    unsafe {
        var scp = addr(sc[0])
        var rlp = addr(rl[0])
        var ohp = addr(oh[0])
        let qup = addr<float const?>(qu[0])
        let qvp = addr<float const?>(qv[0])
        let ktp = addr<float const?>(kt[0])
        let pop_ = addr<float const?>(pop[0])
        let vhp = addr<float const?>(vh[0])
        for (rnd in range(rounds + 1)) {   // round 0 = warmup
            let t0 = ref_time_ticks()
            var r0 = 0l
            while (r0 < tt) {
                let r1 = min(r0 + Q, tt)
                let nr = r1 - r0
                for (i in range64(r0 * tt, r1 * tt)) {
                    scp[i] = 0.0
                }
                for (i in range64(r0 * ww, r1 * ww)) {
                    rlp[i] = 0.0
                }
                if (jo) {
                    gemm_tiled_jo(scp + r0 * tt, qup + r0 * dh, ktp, nr, dh, tt)
                    gemm_tiled_jo(rlp + r0 * ww, qvp + r0 * dh, pop_, nr, dh, ww)
                } else {
                    gemm_f32(scp + r0 * tt, qup + r0 * dh, ktp, nr, dh, tt)
                    gemm_f32(rlp + r0 * ww, qvp + r0 * dh, pop_, nr, dh, ww)
                }
                for (q in range64(r0, r1)) {
                    let base = tt - 1l - q
                    for (k in range64(tt)) {
                        scp[q * tt + k] = (scp[q * tt + k] + rlp[q * ww + base + k]) * scale
                    }
                    softmax(scp + q * tt, tt)
                }
                for (i in range64(r0 * dh, r1 * dh)) {
                    ohp[i] = 0.0
                }
                if (jo) {
                    gemm_tiled_jo(ohp + r0 * dh, scp + r0 * tt, vhp, nr, tt, dh)
                } else {
                    gemm_f32(ohp + r0 * dh, scp + r0 * tt, vhp, nr, tt, dh)
                }
                r0 = r1
            }
            if (rnd > 0) {
                best = min(best, double(get_time_usec(t0)))
            }
        }
    }
    delete qu; delete qv; delete kt; delete pop; delete vh
    delete sc; delete rl; delete oh
    return best / 1000.0lf
}

[benchmark, unused_argument(b)]
def bench_attn_head_emu(b : B?) {
    if (!jit_enabled()) {
        to_log(LOG_WARNING, "skipped: interpreted (dasLLAMA benchmarks are JIT-only)\n")
        return
    }
    to_log(LOG_INFO, "=== parakeet attn head emulation: row-block quantum x tile walk order (single thread) ===\n")
    for (tt in [138l, 2484l]) {
        let dh = 128l
        let macs = double(dh) * double(tt) * double(tt + (2l * tt - 1l) + tt)
        for (Q in [4l, 8l, 16l, 32l, 64l, 128l]) {
            let mii = head_emu_ms(Q, tt, dh, false, 3)
            let mjo = head_emu_ms(Q, tt, dh, true, 3)
            let gii = macs / mii / 1.0e6lf
            let gjo = macs / mjo / 1.0e6lf
            to_log(LOG_INFO, "tt={tt} Q={Q}  ii-outer {mii} ms ({gii} GMAC/s)  jj-outer {mjo} ms ({gjo} GMAC/s)  jo/ii {mii / mjo}x\n")
        }
    }
}

[benchmark, unused_argument(b)]
def bench_attn_gemm(b : B?) {
    if (!jit_enabled()) {
        to_log(LOG_WARNING, "skipped: interpreted (dasLLAMA benchmarks are JIT-only)\n")
        return
    }
    to_log(LOG_INFO, "=== fp32 micro-GEMM: scalar ref vs register-tile geometries (single thread) ===\n")
    bench_shape("score 64^3 ", 64l, 64l, 64l, 4000)     // QKᵀ: Qtile×head×KVtile
    bench_shape("out   64^3 ", 64l, 64l, 64l, 4000)     // P·V:  Qtile×KVtile×head (same shape @1B)
    bench_shape("score 32   ", 32l, 64l, 32l, 8000)     // smaller tile
    bench_shape("k128       ", 64l, 128l, 64l, 2000)    // head_size 128 (8B)
    bench_shape("wide  N256  ", 64l, 64l, 256l, 1000)   // longer KV tile
    // parakeet rel-pos attention shapes (dh=128; jfk tt=138, gb1 tt=2484; M = a caller row slab —
    // lcm-friendly 120 so every tile geometry divides it)
    bench_shape("pk score gb1", 120l, 128l, 2484l, 50)  // (Q+u)·Kᵀ
    bench_shape("pk out   gb1", 120l, 2484l, 128l, 50)  // P·V
    bench_shape("pk score jfk", 120l, 128l, 138l, 900)
    bench_shape("pk out   jfk", 120l, 138l, 128l, 900)
}
