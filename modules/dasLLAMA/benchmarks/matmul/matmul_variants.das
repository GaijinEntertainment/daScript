options gen2

module matmul_variants shared public

require daslib/jobque_boost
require llvm/daslib/aarch64_neon  // sdot4: JIT-emitted ARM SDOT (scalar fallback elsewhere)

// Experiment ledger for the matmul kernel. Every optimization attempt lives here as
// its own function so bench_matmul.das can compare them head-to-head (and against the
// production dasllama_math::matmul). The winner gets promoted into dasllama_math.
// Same signature as the production kernel: y = W·x, W row-major [d x n].

//! Baseline: scalar sequential dot product (the original naive kernel, kept for comparison).
def matmul_naive(var y : array<float> | #; w : array<float> | #; x : array<float> | #; n, d : int64) {
    for (i in range64(d)) {
        var acc = 0.0
        let row = i * n
        for (j in range64(n)) {
            acc += w[row + j] * x[j]
        }
        y[i] = acc
    }
}

// One row's dot product, auto-vectorized by the LLVM JIT (width 8, no bounds checks, non-aliasing).
[hint(unsafe_range_check, noalias=x, noalias=w)]
def dot_vec(w, x : float const?; n : int64) {
    var acc = 0.0
    for [vectorize, vectorize_width=8] (j in range64(n)) {
        acc += unsafe(w[j]) * unsafe(x[j])
    }
    return acc
}

//! LLVM auto-vectorize variant: per-row dot_vec with vectorize/noalias/unsafe-range hints.
def matmul_llvm_vec(var y : array<float> | #; w : array<float> | #; x : array<float> | #; n, d : int64) {
    for (i in range64(d)) {
        let pw = unsafe(addr(w[i * n]))
        let px = unsafe(addr(x[0]))
        y[i] = dot_vec(pw, px, n)
    }
}

// --- LLVM hint sweep: vectorize_width x unroll_count. dot_vec_WxxUyy + matmul_WxxUyy pairs.
// Hints are codegen-only (same scalar semantics as the gated matmul_llvm_vec), so they need
// no separate correctness gate. n=288 for every real matmul, so the classifier shape suffices.

[hint(unsafe_range_check, noalias=x, noalias=w)]
def dv_w8u2(w, x : float const?; n : int64) {
    var acc = 0.0
    for [vectorize, vectorize_width=8, unroll_count=2] (j in range64(n)) {
        acc += unsafe(w[j]) * unsafe(x[j])
    }
    return acc
}
def matmul_w8u2(var y : array<float> | #; w : array<float> | #; x : array<float> | #; n, d : int64) {
    for (i in range64(d)) {
        unsafe {
            y[i] = dv_w8u2(addr(w[i * n]), addr(x[0]), n)
        }
    }
}

[hint(unsafe_range_check, noalias=x, noalias=w)]
def dv_w8u4(w, x : float const?; n : int64) {
    var acc = 0.0
    for [vectorize, vectorize_width=8, unroll_count=4] (j in range64(n)) {
        acc += unsafe(w[j]) * unsafe(x[j])
    }
    return acc
}
def matmul_w8u4(var y : array<float> | #; w : array<float> | #; x : array<float> | #; n, d : int64) {
    for (i in range64(d)) {
        unsafe {
            y[i] = dv_w8u4(addr(w[i * n]), addr(x[0]), n)
        }
    }
}

[hint(unsafe_range_check, noalias=x, noalias=w)]
def dv_w8u8(w, x : float const?; n : int64) {
    var acc = 0.0
    for [vectorize, vectorize_width=8, unroll_count=8] (j in range64(n)) {
        acc += unsafe(w[j]) * unsafe(x[j])
    }
    return acc
}
def matmul_w8u8(var y : array<float> | #; w : array<float> | #; x : array<float> | #; n, d : int64) {
    for (i in range64(d)) {
        unsafe {
            y[i] = dv_w8u8(addr(w[i * n]), addr(x[0]), n)
        }
    }
}

[hint(unsafe_range_check, noalias=x, noalias=w)]
def dv_w16(w, x : float const?; n : int64) {
    var acc = 0.0
    for [vectorize, vectorize_width=16] (j in range64(n)) {
        acc += unsafe(w[j]) * unsafe(x[j])
    }
    return acc
}
def matmul_w16(var y : array<float> | #; w : array<float> | #; x : array<float> | #; n, d : int64) {
    for (i in range64(d)) {
        unsafe {
            y[i] = dv_w16(addr(w[i * n]), addr(x[0]), n)
        }
    }
}

[hint(unsafe_range_check, noalias=x, noalias=w)]
def dv_w16u4(w, x : float const?; n : int64) {
    var acc = 0.0
    for [vectorize, vectorize_width=16, unroll_count=4] (j in range64(n)) {
        acc += unsafe(w[j]) * unsafe(x[j])
    }
    return acc
}
def matmul_w16u4(var y : array<float> | #; w : array<float> | #; x : array<float> | #; n, d : int64) {
    for (i in range64(d)) {
        unsafe {
            y[i] = dv_w16u4(addr(w[i * n]), addr(x[0]), n)
        }
    }
}

//! float4 SIMD: accumulate the dot product four lanes at a time, horizontal-sum at the end.
def matmul_f4(var y : array<float> | #; w : array<float> | #; x : array<float> | #; n, d : int64) {
    for (i in range64(d)) {
        let row = i * n
        var acc = float4(0.0, 0.0, 0.0, 0.0)
        var j = 0l
        while (j + 4l <= n) {
            unsafe {
                let w4 = *reinterpret<float4?>(addr(w[row + j]))
                let x4 = *reinterpret<float4?>(addr(x[j]))
                acc += w4 * x4
            }
            j += 4l
        }
        var sum = acc.x + acc.y + acc.z + acc.w
        while (j < n) {  // tail (n not a multiple of 4)
            sum += w[row + j] * x[j]
            j++
        }
        y[i] = sum
    }
}

// --- Q8_0 dequant-in-dot. Weights are int8 (4x fewer bytes) + one fp32 scale per 32-block.
// Batch-1 matmul is bandwidth-bound, so reading int8 instead of fp32 is the real lever.
// One row's dot = sum over blocks of scale_b * sum_k(q[k]*x[k]); the int8->float widen and
// multiply is the vectorizable inner loop, the scale applies once per 32-wide block.

[hint(unsafe_range_check, noalias = x, noalias = wq, noalias = ws)]
def dot_q8(wq : int8 const?; ws : float const?; x : float const?; n : int64) : float {
    var acc = 0.0
    let nb = n / 32l
    for (bi in range64(nb)) {
        let base = bi * 32l
        var bs = 0.0
        for [vectorize, vectorize_width = 8, unroll_count = 2] (k in range64(32l)) {
            bs += float(unsafe(wq[base + k])) * unsafe(x[base + k])
        }
        acc += unsafe(ws[bi]) * bs
    }
    return acc
}

// Q8 variant FLAT: one vectorized loop over the whole row, scale gathered per element via
// ws[k/32]. The LLVM JIT proves the scale is constant across each unrolled 32-element chunk
// (= one block) and emits a single scalar-load + broadcast, so we get the vector sitofp AND
// loop-carried vector accumulators with ONE horizontal reduce per row (like the fp32 dot).
[hint(unsafe_range_check, noalias = x, noalias = wq, noalias = ws)]
def dot_q8_flat(wq : int8 const?; ws : float const?; x : float const?; n : int64) : float {
    var acc = 0.0
    for [vectorize, vectorize_width = 8, unroll_count = 2] (k in range64(n)) {
        acc += unsafe(ws[k / 32l]) * float(unsafe(wq[k])) * unsafe(x[k])
    }
    return acc
}
def matmul_q8_flat(var y : array<float> | #; wq : array<int8> | #; ws : array<float> | #; x : array<float> | #; n, d : int64) {
    let nb = n / 32l
    unsafe {
        let xp = addr(x[0])
        for (i in range64(d)) {
            y[i] = dot_q8_flat(reinterpret<int8 const?>(addr(wq[i * n])), reinterpret<float const?>(addr(ws[i * nb])), xp, n)
        }
    }
}

//! Q8_0 matmul: y = W·x with W stored as int8 quants `wq` [d x n] plus per-block scales
//! `ws` [d x n/32]. n must be a multiple of 32 (true for every stories15M matmul: 288, 768).
def matmul_q8(var y : array<float> | #; wq : array<int8> | #; ws : array<float> | #; x : array<float> | #; n, d : int64) {
    let nb = n / 32l
    unsafe {
        let xp = addr(x[0])
        for (i in range64(d)) {
            let wp = reinterpret<int8 const?>(addr(wq[i * n]))
            let sp = reinterpret<float const?>(addr(ws[i * nb]))
            y[i] = dot_q8(wp, sp, xp, n)
        }
    }
}

// Q8 variant B: dequantize each row into an fp32 scratch buffer (a pure map loop that
// vectorizes the int8->float widen cleanly), then reuse the proven fp32 dot_vec. One
// horizontal reduce per row instead of per block.
[hint(unsafe_range_check, noalias = out, noalias = wq, noalias = ws)]
def dequant_row(wq : int8 const?; ws : float const?; var out : float?; n : int64) {
    let nb = n / 32l
    for (bi in range64(nb)) {
        let base = bi * 32l
        let s = unsafe(ws[bi])
        for [vectorize, vectorize_width = 8, unroll_count = 2] (k in range64(32l)) {
            unsafe {
                out[base + k] = s * float(wq[base + k])
            }
        }
    }
}
def matmul_q8b(var y : array<float> | #; wq : array<int8> | #; ws : array<float> | #; x : array<float> | #; n, d : int64) {
    let nb = n / 32l
    var wrow : array<float>
    wrow |> resize(n)
    unsafe {
        let xp = addr(x[0])
        var wop = addr(wrow[0])
        for (i in range64(d)) {
            dequant_row(reinterpret<int8 const?>(addr(wq[i * n])), reinterpret<float const?>(addr(ws[i * nb])), wop, n)
            y[i] = dot_vec(wop, xp, n)
        }
    }
}

// Q8 variant C: float4 accumulator carried across the blocks, scale applied per block as a
// broadcast fma, single horizontal reduce at the end of the row. int8->float4 is built from
// four scalar conversions (no SIMD widen intrinsic in daslang).
def dot_q8_f4(wq : int8 const?; ws : float const?; x : float const?; n : int64) : float {
    let nb = n / 32l
    var acc = float4(0.0, 0.0, 0.0, 0.0)
    for (bi in range64(nb)) {
        let base = bi * 32l
        let s = unsafe(ws[bi])
        var bacc = float4(0.0, 0.0, 0.0, 0.0)
        var c = 0l
        while (c < 32l) {
            let off = base + c
            unsafe {
                let q4 = float4(float(wq[off]), float(wq[off + 1l]), float(wq[off + 2l]), float(wq[off + 3l]))
                let x4 = *reinterpret<float4?>(addr(x[off]))
                bacc += q4 * x4
            }
            c += 4l
        }
        acc += float4(s, s, s, s) * bacc
    }
    return acc.x + acc.y + acc.z + acc.w
}
def matmul_q8_f4(var y : array<float> | #; wq : array<int8> | #; ws : array<float> | #; x : array<float> | #; n, d : int64) {
    let nb = n / 32l
    unsafe {
        let xp = addr(x[0])
        for (i in range64(d)) {
            y[i] = dot_q8_f4(reinterpret<int8 const?>(addr(wq[i * n])), reinterpret<float const?>(addr(ws[i * nb])), xp, n)
        }
    }
}

//! Threaded Q8_0 matmul: split the d output rows into `num_jobs` chunks via parallel_for.
//! Workers read main-context y/wq/ws/x through raw pointers. Requires an enclosing with_job_que.
def matmul_q8_par(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; x : array<float>; n, d : int64; num_jobs : int) {
    let nb = n / 32l
    unsafe {
        let yp = addr(y[0])
        let wp = reinterpret<int8 const?>(addr(wq[0]))
        let sp = reinterpret<float const?>(addr(ws[0]))
        let xp = addr(x[0])
        parallel_for(0, int(d), num_jobs) $(rb, re, wg) {
            unsafe {
                var myp = reinterpret<float?>(yp)
                for (i in range(rb, re)) {
                    myp[i] = dot_q8(wp + int64(i) * n, sp + int64(i) * nb, xp, n)
                }
            }
            wg |> notify_and_release
        }
    }
}

//! Threaded Q8_0 matmul using the float4 dot (one horizontal reduce per row). Register-only
//! (no per-row scratch), so it threads cleanly — each worker just needs the shared pointers.
def matmul_q8f4_par(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; x : array<float>; n, d : int64; num_jobs : int) {
    let nb = n / 32l
    unsafe {
        let yp = addr(y[0])
        let wp = reinterpret<int8 const?>(addr(wq[0]))
        let sp = reinterpret<float const?>(addr(ws[0]))
        let xp = addr(x[0])
        parallel_for(0, int(d), num_jobs) $(rb, re, wg) {
            unsafe {
                var myp = reinterpret<float?>(yp)
                for (i in range(rb, re)) {
                    myp[i] = dot_q8_f4(wp + int64(i) * n, sp + int64(i) * nb, xp, n)
                }
            }
            wg |> notify_and_release
        }
    }
}

//! Threaded Q8_0 matmul using the flat dot (vector sitofp + loop-carried accumulators,
//! one reduce per row). Register-only, threads cleanly.
def matmul_q8flat_par(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; x : array<float>; n, d : int64; num_jobs : int) {
    let nb = n / 32l
    unsafe {
        let yp = addr(y[0])
        let wp = reinterpret<int8 const?>(addr(wq[0]))
        let sp = reinterpret<float const?>(addr(ws[0]))
        let xp = addr(x[0])
        parallel_for(0, int(d), num_jobs) $(rb, re, wg) {
            unsafe {
                var myp = reinterpret<float?>(yp)
                for (i in range(rb, re)) {
                    myp[i] = dot_q8_flat(wp + int64(i) * n, sp + int64(i) * nb, xp, n)
                }
            }
            wg |> notify_and_release
        }
    }
}

// --- Q8·Q8 (activation-quantized). The activation x is ALSO quantized to Q8 per block, so the
// inner block loop is int8·int8 -> the LLVM JIT emits ARM SDOT (vdotq_s32: 16 int8 MACs/instr,
// exact int32 accumulate). The two per-block fp32 scales (weight ws[b], activation xs[b]) multiply
// in once per 32-wide block. This is llama.cpp's Q8_0 kernel shape. x is pre-quantized by the
// caller (xq int8 [n] + xs fp32 [n/32]); reused across all d output rows.

[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def dot_q8q8(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {
    var acc = 0.0
    let nb = n / 32l
    for (bi in range64(nb)) {
        let base = bi * 32l
        var iacc = 0
        for [vectorize, vectorize_width = 16] (k in range64(32l)) {
            iacc += int(unsafe(wq[base + k])) * int(unsafe(xq[base + k]))
        }
        acc += float(iacc) * unsafe(ws[bi]) * unsafe(xs[bi])
    }
    return acc
}

//! Q8·Q8 matmul: x already quantized to (xq, xs). Each row is an SDOT dot.
def matmul_q8q8(var y : array<float> | #; wq : array<int8> | #; ws : array<float> | #; xq : array<int8> | #; xs : array<float> | #; n, d : int64) {
    let nb = n / 32l
    unsafe {
        let xqp = reinterpret<int8 const?>(addr(xq[0]))
        let xsp = reinterpret<float const?>(addr(xs[0]))
        for (i in range64(d)) {
            y[i] = dot_q8q8(reinterpret<int8 const?>(addr(wq[i * n])), reinterpret<float const?>(addr(ws[i * nb])), xqp, xsp, n)
        }
    }
}

// ===== TRUE SDOT via JIT intrinsic (sdot4 lives in llvm/aarch64_neon now) =====

// sdot4: per block, 2 sdot (16+16 int8) into one int4 accumulator, hsum once per block.
[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def dot_q8q8_sdot4(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {
    var facc = 0.0
    let nb = n / 32l
    unsafe {
        for (bi in range64(nb)) {
            let base = bi * 32l
            var acc = int4(0, 0, 0, 0)
            acc = sdot4(acc, wq + base, xq + base)
            acc = sdot4(acc, wq + base + 16l, xq + base + 16l)
            facc += float(acc.x + acc.y + acc.z + acc.w) * ws[bi] * xs[bi]
        }
    }
    return facc
}

// sdot4 + 4 blocks/iter with 4 independent int4 accumulators (the unrolled form, prior PoC's 1.99×).
[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def dot_q8q8_sdot4x4(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {
    let nb = n / 32l
    var f0 = 0.0
    var f1 = 0.0
    var f2 = 0.0
    var f3 = 0.0
    var bi = 0l
    unsafe {
        while (bi + 4l <= nb) {
            let bb = bi * 32l
            var a0 = int4(0, 0, 0, 0)
            var a1 = int4(0, 0, 0, 0)
            var a2 = int4(0, 0, 0, 0)
            var a3 = int4(0, 0, 0, 0)
            a0 = sdot4(a0, wq + bb, xq + bb)
            a0 = sdot4(a0, wq + bb + 16l, xq + bb + 16l)
            a1 = sdot4(a1, wq + bb + 32l, xq + bb + 32l)
            a1 = sdot4(a1, wq + bb + 48l, xq + bb + 48l)
            a2 = sdot4(a2, wq + bb + 64l, xq + bb + 64l)
            a2 = sdot4(a2, wq + bb + 80l, xq + bb + 80l)
            a3 = sdot4(a3, wq + bb + 96l, xq + bb + 96l)
            a3 = sdot4(a3, wq + bb + 112l, xq + bb + 112l)
            f0 += float(a0.x + a0.y + a0.z + a0.w) * ws[bi + 0l] * xs[bi + 0l]
            f1 += float(a1.x + a1.y + a1.z + a1.w) * ws[bi + 1l] * xs[bi + 1l]
            f2 += float(a2.x + a2.y + a2.z + a2.w) * ws[bi + 2l] * xs[bi + 2l]
            f3 += float(a3.x + a3.y + a3.z + a3.w) * ws[bi + 3l] * xs[bi + 3l]
            bi += 4l
        }
        while (bi < nb) {
            let base = bi * 32l
            var acc = int4(0, 0, 0, 0)
            acc = sdot4(acc, wq + base, xq + base)
            acc = sdot4(acc, wq + base + 16l, xq + base + 16l)
            f0 += float(acc.x + acc.y + acc.z + acc.w) * ws[bi] * xs[bi]
            bi++
        }
    }
    return f0 + f1 + f2 + f3
}

def matmul_q8q8_sdot4(var y : array<float> | #; wq : array<int8> | #; ws : array<float> | #; xq : array<int8> | #; xs : array<float> | #; n, d : int64) {
    let nb = n / 32l
    unsafe {
        let xqp = reinterpret<int8 const?>(addr(xq[0]))
        let xsp = reinterpret<float const?>(addr(xs[0]))
        for (i in range64(d)) {
            y[i] = dot_q8q8_sdot4(reinterpret<int8 const?>(addr(wq[i * n])), reinterpret<float const?>(addr(ws[i * nb])), xqp, xsp, n)
        }
    }
}

def matmul_q8q8_sdot4x4(var y : array<float> | #; wq : array<int8> | #; ws : array<float> | #; xq : array<int8> | #; xs : array<float> | #; n, d : int64) {
    let nb = n / 32l
    unsafe {
        let xqp = reinterpret<int8 const?>(addr(xq[0]))
        let xsp = reinterpret<float const?>(addr(xs[0]))
        for (i in range64(d)) {
            y[i] = dot_q8q8_sdot4x4(reinterpret<int8 const?>(addr(wq[i * n])), reinterpret<float const?>(addr(ws[i * nb])), xqp, xsp, n)
        }
    }
}

// ===== laneq + interleaved repack (4-row tile, 1 token) — the lane-indexed SDOT endgame =====
// Repack [d x n] row-major int8 weights into 4-row-interleaved groups so the laneq kernel keeps each
// accumulator lane on a distinct output row (no per-block addv, ever). Done ONCE at model load — the
// weights are static — so the kernel below pays no repack cost. Layout per 4-row group g (rows 4g..4g+3):
// for each 4-col group kg (n/4 of them) write 16 bytes = [row0[kg], row1[kg], row2[kg], row3[kg]], each
// [..] = 4 contiguous int8. Block scales are interleaved as [s0,s1,s2,s3] per 32-block. Tail rows (d%4)
// stay row-major after the groups and run the sdot4x4 fallback.
def repack_q8q8_4(wq : array<int8>; ws : array<float>; n, d : int64; var wout : array<int8>; var sout : array<float>) {
    let nb = n / 32l
    let ng = d / 4l
    let ncg = n / 4l
    wout |> resize(d * n)
    sout |> resize(d * nb)
    unsafe {
        let wp = reinterpret<int8 const?>(addr(wq[0]))
        let sp = reinterpret<float const?>(addr(ws[0]))
        var wo = addr(wout[0])
        var so = addr(sout[0])
        for (g in range64(ng)) {
            let gbase = g * 4l * n
            for (kg in range64(ncg)) {
                let dst = gbase + kg * 16l
                for (r in range64(4l)) {
                    let src = (g * 4l + r) * n + kg * 4l
                    wo[dst + r * 4l + 0l] = wp[src + 0l]
                    wo[dst + r * 4l + 1l] = wp[src + 1l]
                    wo[dst + r * 4l + 2l] = wp[src + 2l]
                    wo[dst + r * 4l + 3l] = wp[src + 3l]
                }
            }
            let sgbase = g * 4l * nb
            for (bi in range64(nb)) {
                for (r in range64(4l)) {
                    so[sgbase + bi * 4l + r] = sp[(g * 4l + r) * nb + bi]
                }
            }
        }
        for (i in range64(ng * 4l * n, d * n)) {
            wo[i] = wp[i]
        }
        for (i in range64(ng * 4l * nb, d * nb)) {
            so[i] = sp[i]
        }
    }
}

//! Lane-indexed SDOT over one 4-row interleaved group against one token. 8 sdot4_laneq per 32-block
//! (two 16-k chunks × lanes 0..3); the int4 accumulator's lane L is output row L, flushed to a float4
//! with the 4 rows' block scales × the token block scale — no horizontal reduce anywhere.
[hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xq, noalias = xs)]
def dot_q8q8_laneq4(wg : int8 const?; sg : float const?; xq : int8 const?; xs : float const?; n : int64) : float4 {
    let nb = n / 32l
    var f = float4(0.0, 0.0, 0.0, 0.0)
    unsafe {
        for (bi in range64(nb)) {
            let wb = bi * 128l
            let xb = bi * 32l
            var acc = int4(0, 0, 0, 0)
            acc = sdot4_laneq(acc, wg + wb, xq + xb, 0)
            acc = sdot4_laneq(acc, wg + wb + 16l, xq + xb, 1)
            acc = sdot4_laneq(acc, wg + wb + 32l, xq + xb, 2)
            acc = sdot4_laneq(acc, wg + wb + 48l, xq + xb, 3)
            acc = sdot4_laneq(acc, wg + wb + 64l, xq + xb + 16l, 0)
            acc = sdot4_laneq(acc, wg + wb + 80l, xq + xb + 16l, 1)
            acc = sdot4_laneq(acc, wg + wb + 96l, xq + xb + 16l, 2)
            acc = sdot4_laneq(acc, wg + wb + 112l, xq + xb + 16l, 3)
            let s = float4(sg[bi * 4l], sg[bi * 4l + 1l], sg[bi * 4l + 2l], sg[bi * 4l + 3l])
            let xsb = xs[bi]
            f += float4(acc) * s * float4(xsb, xsb, xsb, xsb)
        }
    }
    return f
}

// Driver over repacked weights (wgq/wgs from repack_q8q8_4). Tail rows are row-major → sdot4x4.
def matmul_q8q8_laneq(var y : array<float> | #; wgq : array<int8> | #; wgs : array<float> | #; xq : array<int8> | #; xs : array<float> | #; n, d : int64) {
    let nb = n / 32l
    let ng = d / 4l
    unsafe {
        var yp = addr(y[0])
        let wp = reinterpret<int8 const?>(addr(wgq[0]))
        let sp = reinterpret<float const?>(addr(wgs[0]))
        let xqp = reinterpret<int8 const?>(addr(xq[0]))
        let xsp = reinterpret<float const?>(addr(xs[0]))
        for (g in range64(ng)) {
            let f = dot_q8q8_laneq4(wp + g * 4l * n, sp + g * 4l * nb, xqp, xsp, n)
            yp[g * 4l + 0l] = f.x
            yp[g * 4l + 1l] = f.y
            yp[g * 4l + 2l] = f.z
            yp[g * 4l + 3l] = f.w
        }
        for (i in range64(ng * 4l, d)) {
            yp[i] = dot_q8q8_sdot4x4(wp + i * n, sp + i * nb, xqp, xsp, n)
        }
    }
}

// uf: original nested, inner 32-element reduction marked unroll_full (llvm.loop.unroll.full).
// Only the inner loop has a compile-time trip count (32) — the runtime block loop can't full-unroll.
[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def dot_q8q8_uf(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {
    var acc = 0.0
    let nb = n / 32l
    for (bi in range64(nb)) {
        let base = bi * 32l
        var iacc = 0
        for [vectorize, vectorize_width = 16, unroll_full] (k in range64(32l)) {
            iacc += int(unsafe(wq[base + k])) * int(unsafe(xq[base + k]))
        }
        acc += float(iacc) * unsafe(ws[bi]) * unsafe(xs[bi])
    }
    return acc
}

def matmul_q8q8_uf(var y : array<float> | #; wq : array<int8> | #; ws : array<float> | #; xq : array<int8> | #; xs : array<float> | #; n, d : int64) {
    let nb = n / 32l
    unsafe {
        let xqp = reinterpret<int8 const?>(addr(xq[0]))
        let xsp = reinterpret<float const?>(addr(xs[0]))
        for (i in range64(d)) {
            y[i] = dot_q8q8_uf(reinterpret<int8 const?>(addr(wq[i * n])), reinterpret<float const?>(addr(ws[i * nb])), xqp, xsp, n)
        }
    }
}

// ===== q8q8 inner-loop vectorize sweep (the part skipped earlier) =====
// Each keeps the original outer block-loop + scalar float accumulate; only the inner int8·int8
// reduction's [vectorize] hint changes. The matmul wrapper is identical bar the dot fn it calls.

[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def dot_q8q8_w8(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {
    var acc = 0.0
    let nb = n / 32l
    for (bi in range64(nb)) {
        let base = bi * 32l
        var iacc = 0
        for [vectorize, vectorize_width = 8] (k in range64(32l)) {
            iacc += int(unsafe(wq[base + k])) * int(unsafe(xq[base + k]))
        }
        acc += float(iacc) * unsafe(ws[bi]) * unsafe(xs[bi])
    }
    return acc
}

[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def dot_q8q8_w32(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {
    var acc = 0.0
    let nb = n / 32l
    for (bi in range64(nb)) {
        let base = bi * 32l
        var iacc = 0
        for [vectorize, vectorize_width = 32] (k in range64(32l)) {
            iacc += int(unsafe(wq[base + k])) * int(unsafe(xq[base + k]))
        }
        acc += float(iacc) * unsafe(ws[bi]) * unsafe(xs[bi])
    }
    return acc
}

[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def dot_q8q8_w16u2(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {
    var acc = 0.0
    let nb = n / 32l
    for (bi in range64(nb)) {
        let base = bi * 32l
        var iacc = 0
        for [vectorize, vectorize_width = 16, unroll_count = 2] (k in range64(32l)) {
            iacc += int(unsafe(wq[base + k])) * int(unsafe(xq[base + k]))
        }
        acc += float(iacc) * unsafe(ws[bi]) * unsafe(xs[bi])
    }
    return acc
}

// 4acc: 4 blocks per outer iter, each its OWN inline vectorized inner loop (keeps SDOT) + its own
// int + float accumulator, so 4 independent dependency chains run in parallel (more loads in flight).
[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def dot_q8q8_4acc(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {
    let nb = n / 32l
    var a0 = 0.0
    var a1 = 0.0
    var a2 = 0.0
    var a3 = 0.0
    var bi = 0l
    while (bi + 4l <= nb) {
        let b0 = (bi + 0l) * 32l
        let b1 = (bi + 1l) * 32l
        let b2 = (bi + 2l) * 32l
        let b3 = (bi + 3l) * 32l
        var i0 = 0
        var i1 = 0
        var i2 = 0
        var i3 = 0
        for [vectorize, vectorize_width = 16] (k in range64(32l)) {
            i0 += int(unsafe(wq[b0 + k])) * int(unsafe(xq[b0 + k]))
        }
        for [vectorize, vectorize_width = 16] (k in range64(32l)) {
            i1 += int(unsafe(wq[b1 + k])) * int(unsafe(xq[b1 + k]))
        }
        for [vectorize, vectorize_width = 16] (k in range64(32l)) {
            i2 += int(unsafe(wq[b2 + k])) * int(unsafe(xq[b2 + k]))
        }
        for [vectorize, vectorize_width = 16] (k in range64(32l)) {
            i3 += int(unsafe(wq[b3 + k])) * int(unsafe(xq[b3 + k]))
        }
        a0 += float(i0) * unsafe(ws[bi + 0l]) * unsafe(xs[bi + 0l])
        a1 += float(i1) * unsafe(ws[bi + 1l]) * unsafe(xs[bi + 1l])
        a2 += float(i2) * unsafe(ws[bi + 2l]) * unsafe(xs[bi + 2l])
        a3 += float(i3) * unsafe(ws[bi + 3l]) * unsafe(xs[bi + 3l])
        bi += 4l
    }
    var acc = a0 + a1 + a2 + a3
    while (bi < nb) {
        let base = bi * 32l
        var iacc = 0
        for [vectorize, vectorize_width = 16] (k in range64(32l)) {
            iacc += int(unsafe(wq[base + k])) * int(unsafe(xq[base + k]))
        }
        acc += float(iacc) * unsafe(ws[bi]) * unsafe(xs[bi])
        bi++
    }
    return acc
}

def matmul_q8q8_w8(var y : array<float> | #; wq : array<int8> | #; ws : array<float> | #; xq : array<int8> | #; xs : array<float> | #; n, d : int64) {
    let nb = n / 32l
    unsafe {
        let xqp = reinterpret<int8 const?>(addr(xq[0]))
        let xsp = reinterpret<float const?>(addr(xs[0]))
        for (i in range64(d)) {
            y[i] = dot_q8q8_w8(reinterpret<int8 const?>(addr(wq[i * n])), reinterpret<float const?>(addr(ws[i * nb])), xqp, xsp, n)
        }
    }
}

def matmul_q8q8_w32(var y : array<float> | #; wq : array<int8> | #; ws : array<float> | #; xq : array<int8> | #; xs : array<float> | #; n, d : int64) {
    let nb = n / 32l
    unsafe {
        let xqp = reinterpret<int8 const?>(addr(xq[0]))
        let xsp = reinterpret<float const?>(addr(xs[0]))
        for (i in range64(d)) {
            y[i] = dot_q8q8_w32(reinterpret<int8 const?>(addr(wq[i * n])), reinterpret<float const?>(addr(ws[i * nb])), xqp, xsp, n)
        }
    }
}

def matmul_q8q8_w16u2(var y : array<float> | #; wq : array<int8> | #; ws : array<float> | #; xq : array<int8> | #; xs : array<float> | #; n, d : int64) {
    let nb = n / 32l
    unsafe {
        let xqp = reinterpret<int8 const?>(addr(xq[0]))
        let xsp = reinterpret<float const?>(addr(xs[0]))
        for (i in range64(d)) {
            y[i] = dot_q8q8_w16u2(reinterpret<int8 const?>(addr(wq[i * n])), reinterpret<float const?>(addr(ws[i * nb])), xqp, xsp, n)
        }
    }
}

def matmul_q8q8_4acc(var y : array<float> | #; wq : array<int8> | #; ws : array<float> | #; xq : array<int8> | #; xs : array<float> | #; n, d : int64) {
    let nb = n / 32l
    unsafe {
        let xqp = reinterpret<int8 const?>(addr(xq[0]))
        let xsp = reinterpret<float const?>(addr(xs[0]))
        for (i in range64(d)) {
            y[i] = dot_q8q8_4acc(reinterpret<int8 const?>(addr(wq[i * n])), reinterpret<float const?>(addr(ws[i * nb])), xqp, xsp, n)
        }
    }
}

// 8acc: same idea, 8 independent chains.
[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def dot_q8q8_8acc(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {
    let nb = n / 32l
    var a : float[8]
    var bi = 0l
    while (bi + 8l <= nb) {
        for (lane in range64(8l)) {
            let base = (bi + lane) * 32l
            var iacc = 0
            for [vectorize, vectorize_width = 16] (k in range64(32l)) {
                iacc += int(unsafe(wq[base + k])) * int(unsafe(xq[base + k]))
            }
            a[int(lane)] += float(iacc) * unsafe(ws[bi + lane]) * unsafe(xs[bi + lane])
        }
        bi += 8l
    }
    var acc = 0.0
    for (lane in range(8)) {
        acc += a[lane]
    }
    while (bi < nb) {
        let base = bi * 32l
        var iacc = 0
        for [vectorize, vectorize_width = 16] (k in range64(32l)) {
            iacc += int(unsafe(wq[base + k])) * int(unsafe(xq[base + k]))
        }
        acc += float(iacc) * unsafe(ws[bi]) * unsafe(xs[bi])
        bi++
    }
    return acc
}

def matmul_q8q8_8acc(var y : array<float> | #; wq : array<int8> | #; ws : array<float> | #; xq : array<int8> | #; xs : array<float> | #; n, d : int64) {
    let nb = n / 32l
    unsafe {
        let xqp = reinterpret<int8 const?>(addr(xq[0]))
        let xsp = reinterpret<float const?>(addr(xs[0]))
        for (i in range64(d)) {
            y[i] = dot_q8q8_8acc(reinterpret<int8 const?>(addr(wq[i * n])), reinterpret<float const?>(addr(ws[i * nb])), xqp, xsp, n)
        }
    }
}

// 8acc_int: stage 8 block sums as int[8] (the SDOT phase), THEN a separate float scale/flush phase.
// Splitting int-MAC from float-convert lets each pipeline (8 SDOTs back-to-back, then a flat float
// flush over the 8 lanes) instead of interleaving an sitofp + 2 float muls into every block.
[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def dot_q8q8_8acc_int(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {
    let nb = n / 32l
    var facc = 0.0
    var bi = 0l
    while (bi + 8l <= nb) {
        var s : int[8]
        for (lane in range(8)) {
            let base = (bi + int64(lane)) * 32l
            var iacc = 0
            for [vectorize, vectorize_width = 16] (k in range64(32l)) {
                iacc += int(unsafe(wq[base + k])) * int(unsafe(xq[base + k]))
            }
            s[lane] = iacc
        }
        for (lane in range(8)) {
            facc += float(s[lane]) * unsafe(ws[bi + int64(lane)]) * unsafe(xs[bi + int64(lane)])
        }
        bi += 8l
    }
    while (bi < nb) {
        let base = bi * 32l
        var iacc = 0
        for [vectorize, vectorize_width = 16] (k in range64(32l)) {
            iacc += int(unsafe(wq[base + k])) * int(unsafe(xq[base + k]))
        }
        facc += float(iacc) * unsafe(ws[bi]) * unsafe(xs[bi])
        bi++
    }
    return facc
}

def matmul_q8q8_8acc_int(var y : array<float> | #; wq : array<int8> | #; ws : array<float> | #; xq : array<int8> | #; xs : array<float> | #; n, d : int64) {
    let nb = n / 32l
    unsafe {
        let xqp = reinterpret<int8 const?>(addr(xq[0]))
        let xsp = reinterpret<float const?>(addr(xs[0]))
        for (i in range64(d)) {
            y[i] = dot_q8q8_8acc_int(reinterpret<int8 const?>(addr(wq[i * n])), reinterpret<float const?>(addr(ws[i * nb])), xqp, xsp, n)
        }
    }
}

// ===== FLAT q8q8: ONE loop over the whole row, NO 32-element inner block loop =====
// Scale gathered per element (k>>5 = block). int8·int8 -> float per element (abandons int32 SDOT),
// the q8q8 analog of dot_q8_flat. Different vectorize settings; _wXuY = width X, unroll Y.

[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def dot_q8q8_flat(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {
    var acc = 0.0
    for [vectorize, vectorize_width = 16] (k in range64(n)) {
        acc += float(int(unsafe(wq[k])) * int(unsafe(xq[k]))) * unsafe(ws[k >> 5l]) * unsafe(xs[k >> 5l])
    }
    return acc
}

[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def dot_q8q8_flat_w8u2(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {
    var acc = 0.0
    for [vectorize, vectorize_width = 8, unroll_count = 2] (k in range64(n)) {
        acc += float(int(unsafe(wq[k])) * int(unsafe(xq[k]))) * unsafe(ws[k >> 5l]) * unsafe(xs[k >> 5l])
    }
    return acc
}

[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def dot_q8q8_flat_w16u4(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {
    var acc = 0.0
    for [vectorize, vectorize_width = 16, unroll_count = 4] (k in range64(n)) {
        acc += float(int(unsafe(wq[k])) * int(unsafe(xq[k]))) * unsafe(ws[k >> 5l]) * unsafe(xs[k >> 5l])
    }
    return acc
}

// FLAT with INTEGER accumulator: ONE loop over the whole row, an int32 accumulator that flushes to
// float ONLY at each 32-block boundary (where the scale changes). Keeps the cheap int MAC; converts
// once per block like the nested form, but with no explicit inner loop. The "flat" the float variants
// above weren't.
[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def dot_q8q8_flat_int(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {
    var facc = 0.0
    var iacc = 0
    for (k in range64(n)) {
        iacc += int(unsafe(wq[k])) * int(unsafe(xq[k]))
        if ((k & 31l) == 31l) {
            let b = k >> 5l
            facc += float(iacc) * unsafe(ws[b]) * unsafe(xs[b])
            iacc = 0
        }
    }
    return facc
}

def matmul_q8q8_flat_int(var y : array<float> | #; wq : array<int8> | #; ws : array<float> | #; xq : array<int8> | #; xs : array<float> | #; n, d : int64) {
    let nb = n / 32l
    unsafe {
        let xqp = reinterpret<int8 const?>(addr(xq[0]))
        let xsp = reinterpret<float const?>(addr(xs[0]))
        for (i in range64(d)) {
            y[i] = dot_q8q8_flat_int(reinterpret<int8 const?>(addr(wq[i * n])), reinterpret<float const?>(addr(ws[i * nb])), xqp, xsp, n)
        }
    }
}

def matmul_q8q8_flat(var y : array<float> | #; wq : array<int8> | #; ws : array<float> | #; xq : array<int8> | #; xs : array<float> | #; n, d : int64) {
    let nb = n / 32l
    unsafe {
        let xqp = reinterpret<int8 const?>(addr(xq[0]))
        let xsp = reinterpret<float const?>(addr(xs[0]))
        for (i in range64(d)) {
            y[i] = dot_q8q8_flat(reinterpret<int8 const?>(addr(wq[i * n])), reinterpret<float const?>(addr(ws[i * nb])), xqp, xsp, n)
        }
    }
}

def matmul_q8q8_flat_w8u2(var y : array<float> | #; wq : array<int8> | #; ws : array<float> | #; xq : array<int8> | #; xs : array<float> | #; n, d : int64) {
    let nb = n / 32l
    unsafe {
        let xqp = reinterpret<int8 const?>(addr(xq[0]))
        let xsp = reinterpret<float const?>(addr(xs[0]))
        for (i in range64(d)) {
            y[i] = dot_q8q8_flat_w8u2(reinterpret<int8 const?>(addr(wq[i * n])), reinterpret<float const?>(addr(ws[i * nb])), xqp, xsp, n)
        }
    }
}

def matmul_q8q8_flat_w16u4(var y : array<float> | #; wq : array<int8> | #; ws : array<float> | #; xq : array<int8> | #; xs : array<float> | #; n, d : int64) {
    let nb = n / 32l
    unsafe {
        let xqp = reinterpret<int8 const?>(addr(xq[0]))
        let xsp = reinterpret<float const?>(addr(xs[0]))
        for (i in range64(d)) {
            y[i] = dot_q8q8_flat_w16u4(reinterpret<int8 const?>(addr(wq[i * n])), reinterpret<float const?>(addr(ws[i * nb])), xqp, xsp, n)
        }
    }
}

//! Threaded Q8·Q8 matmul: split the d rows across workers. x pre-quantized; workers read shared
//! pointers. Requires an enclosing with_job_que.
def matmul_q8q8_par(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; xq : array<int8> | #; xs : array<float> | #; n, d : int64; num_jobs : int) {
    let nb = n / 32l
    unsafe {
        let yp = addr(y[0])
        let wp = reinterpret<int8 const?>(addr(wq[0]))
        let sp = reinterpret<float const?>(addr(ws[0]))
        let xqp = reinterpret<int8 const?>(addr(xq[0]))
        let xsp = reinterpret<float const?>(addr(xs[0]))
        parallel_for(0, int(d), num_jobs) $(rb, re, wg) {
            unsafe {
                var myp = reinterpret<float?>(yp)
                for (i in range(rb, re)) {
                    myp[i] = dot_q8q8(wp + int64(i) * n, sp + int64(i) * nb, xqp, xsp, n)
                }
            }
            wg |> notify_and_release
        }
    }
}

//! Fused Q8·Q8 for THREE independent row-groups that share the activation x (e.g. attention Q/K/V):
//! one parallel_for over the combined d0+d1+d2 output rows, so workers fill in on the small K/V rows
//! that would otherwise run single-thread while the pool sits idle. wq/ws is the whole quantized blob;
//! woff{0,1,2} are the weight-element offsets of each group; the activation is pre-quantized once.
def matmul_q8q8_group3_par(var y0 : array<float>; var y1 : array<float>; var y2 : array<float>;
                           wq : array<int8> | #; ws : array<float> | #;
                           woff0, woff1, woff2 : int64;
                           xq : array<int8> | #; xs : array<float> | #;
                           n, d0, d1, d2 : int64; num_jobs : int) {
    let nb = n / 32l
    let dtot = d0 + d1 + d2
    unsafe {
        let y0p = addr(y0[0])
        let y1p = addr(y1[0])
        let y2p = addr(y2[0])
        let wp = reinterpret<int8 const?>(addr(wq[0]))
        let sp = reinterpret<float const?>(addr(ws[0]))
        let xqp = reinterpret<int8 const?>(addr(xq[0]))
        let xsp = reinterpret<float const?>(addr(xs[0]))
        parallel_for(0, int(dtot), num_jobs) $(rb, re, wg) {
            unsafe {
                var w0 = reinterpret<float?>(y0p)
                var w1 = reinterpret<float?>(y1p)
                var w2 = reinterpret<float?>(y2p)
                for (gi in range(rb, re)) {
                    let g = int64(gi)
                    if (g < d0) {
                        w0[g] = dot_q8q8(wp + woff0 + g * n, sp + woff0 / 32l + g * nb, xqp, xsp, n)
                    } elif (g < d0 + d1) {
                        let r = g - d0
                        w1[r] = dot_q8q8(wp + woff1 + r * n, sp + woff1 / 32l + r * nb, xqp, xsp, n)
                    } else {
                        let r = g - d0 - d1
                        w2[r] = dot_q8q8(wp + woff2 + r * n, sp + woff2 / 32l + r * nb, xqp, xsp, n)
                    }
                }
            }
            wg |> notify_and_release
        }
    }
}

// --- Q4_0 dequant-in-dot. Weights are 4-bit (8x fewer bytes than fp32) in GGML's split-half
// packing: per 32-weight block, 16 bytes where byte j holds element j in the low nibble and
// element j+16 in the high nibble, plus one fp32 scale. Half the bytes of Q8 to read, but the
// unpack costs an extra shift+mask per element on top of the int->float widen.

// One row's Q4 dot, float4 accumulator carried across blocks (mirrors dot_q8_f4). Each 4-byte
// group yields 4 low-nibble elements (positions base+j..) and 4 high-nibble elements (base+16+j..),
// each dequantized as (nibble - 8); the block scale applies once as a broadcast fma at block end.
def dot_q4(wq : uint8 const?; ws : float const?; x : float const?; n : int64) : float {
    let nb = n / 32l
    var acc = float4(0.0, 0.0, 0.0, 0.0)
    for (bi in range64(nb)) {
        let base = bi * 32l
        let bbyte = bi * 16l
        let s = unsafe(ws[bi])
        var bacc = float4(0.0, 0.0, 0.0, 0.0)
        var j = 0l
        while (j < 16l) {
            unsafe {
                let b0 = int(wq[bbyte + j])
                let b1 = int(wq[bbyte + j + 1l])
                let b2 = int(wq[bbyte + j + 2l])
                let b3 = int(wq[bbyte + j + 3l])
                let lo4 = float4(float((b0 & 0x0F) - 8), float((b1 & 0x0F) - 8), float((b2 & 0x0F) - 8), float((b3 & 0x0F) - 8))
                let hi4 = float4(float((b0 >> 4) - 8), float((b1 >> 4) - 8), float((b2 >> 4) - 8), float((b3 >> 4) - 8))
                let xlo = *reinterpret<float4?>(addr(x[base + j]))
                let xhi = *reinterpret<float4?>(addr(x[base + 16l + j]))
                bacc += lo4 * xlo + hi4 * xhi
            }
            j += 4l
        }
        acc += float4(s, s, s, s) * bacc
    }
    return acc.x + acc.y + acc.z + acc.w
}

def matmul_q4(var y : array<float> | #; wq : array<uint8> | #; ws : array<float> | #; x : array<float> | #; n, d : int64) {
    let nb = n / 32l
    unsafe {
        let xp = addr(x[0])
        for (i in range64(d)) {
            y[i] = dot_q4(reinterpret<uint8 const?>(addr(wq[i * (n / 2l)])), reinterpret<float const?>(addr(ws[i * nb])), xp, n)
        }
    }
}

// Q4 variant FV: per-block, but the inner loop is a scalar for[vectorize] over the 16 bytes
// (no hand-rolled float4) so the JIT widens the mask/shift/sitofp. Low nibbles read a contiguous
// x[base..base+15] run, high nibbles x[base+16..base+31]. Per-block scalar reduce (like dot_q8).
[hint(unsafe_range_check, noalias = x, noalias = wq, noalias = ws)]
def dot_q4_fv(wq : uint8 const?; ws : float const?; x : float const?; n : int64) : float {
    var acc = 0.0
    let nb = n / 32l
    for (bi in range64(nb)) {
        let base = bi * 32l
        let bbyte = bi * 16l
        let s = unsafe(ws[bi])
        var bs = 0.0
        for [vectorize, vectorize_width = 8, unroll_count = 2] (j in range64(16l)) {
            let byte = int(unsafe(wq[bbyte + j]))
            let lo = float((byte & 0x0F) - 8)
            let hi = float((byte >> 4) - 8)
            bs += lo * unsafe(x[base + j]) + hi * unsafe(x[base + 16l + j])
        }
        acc += s * bs
    }
    return acc
}
def matmul_q4_fv(var y : array<float> | #; wq : array<uint8> | #; ws : array<float> | #; x : array<float> | #; n, d : int64) {
    let nb = n / 32l
    let nh = n / 2l
    unsafe {
        let xp = addr(x[0])
        for (i in range64(d)) {
            y[i] = dot_q4_fv(reinterpret<uint8 const?>(addr(wq[i * nh])), reinterpret<float const?>(addr(ws[i * nb])), xp, n)
        }
    }
}

// --- Q4 fv hint sweep: same body as dot_q4_fv, different inner-loop vectorize hints (codegen-only,
// so no separate correctness gate). The inner trip is 16, so width*unroll past 16 spills to a scalar
// remainder; full-cover combos only. Findings on M1 Max (classifier, single-thread):
//   width=4 (native NEON lane) WINS — w4u4 1270us vs w8u2 1364us (heavy Q4 unpack hates 8-wide
//   splitting); unroll past full-cover is flat (w4u2 == w4u4); width=16 is a 2.6x disaster (3559us);
//   vectorize_predicate is a NEON no-op; for vs while inner is byte-identical IR. w4u4 promoted.
[hint(unsafe_range_check, noalias = x, noalias = wq, noalias = ws)]
def dot_q4_fv_w4u4(wq : uint8 const?; ws : float const?; x : float const?; n : int64) : float {
    var acc = 0.0
    let nb = n / 32l
    for (bi in range64(nb)) {
        let base = bi * 32l
        let bbyte = bi * 16l
        let s = unsafe(ws[bi])
        var bs = 0.0
        for [vectorize, vectorize_width = 4, unroll_count = 4] (j in range64(16l)) {
            let byte = int(unsafe(wq[bbyte + j]))
            bs += float((byte & 0x0F) - 8) * unsafe(x[base + j]) + float((byte >> 4) - 8) * unsafe(x[base + 16l + j])
        }
        acc += s * bs
    }
    return acc
}
def matmul_q4_fv_w4u4(var y : array<float> | #; wq : array<uint8> | #; ws : array<float> | #; x : array<float> | #; n, d : int64) {
    let nb = n / 32l
    let nh = n / 2l
    unsafe {
        let xp = addr(x[0])
        for (i in range64(d)) {
            y[i] = dot_q4_fv_w4u4(reinterpret<uint8 const?>(addr(wq[i * nh])), reinterpret<float const?>(addr(ws[i * nb])), xp, n)
        }
    }
}

//! Threaded Q4_0 matmul using the width-4/unroll-4 fv dot (M1 native-lane match).
def matmul_q4fvw4u4_par(var y : array<float>; wq : array<uint8> | #; ws : array<float> | #; x : array<float>; n, d : int64; num_jobs : int) {
    let nb = n / 32l
    let nh = n / 2l
    unsafe {
        let yp = addr(y[0])
        let wp = reinterpret<uint8 const?>(addr(wq[0]))
        let sp = reinterpret<float const?>(addr(ws[0]))
        let xp = addr(x[0])
        parallel_for(0, int(d), num_jobs) $(rb, re, wg) {
            unsafe {
                var myp = reinterpret<float?>(yp)
                for (i in range(rb, re)) {
                    myp[i] = dot_q4_fv_w4u4(wp + int64(i) * nh, sp + int64(i) * nb, xp, n)
                }
            }
            wg |> notify_and_release
        }
    }
}

[hint(unsafe_range_check, noalias = x, noalias = wq, noalias = ws)]
def dot_q4_fv_w16(wq : uint8 const?; ws : float const?; x : float const?; n : int64) : float {
    var acc = 0.0
    let nb = n / 32l
    for (bi in range64(nb)) {
        let base = bi * 32l
        let bbyte = bi * 16l
        let s = unsafe(ws[bi])
        var bs = 0.0
        for [vectorize, vectorize_width = 16] (j in range64(16l)) {
            let byte = int(unsafe(wq[bbyte + j]))
            bs += float((byte & 0x0F) - 8) * unsafe(x[base + j]) + float((byte >> 4) - 8) * unsafe(x[base + 16l + j])
        }
        acc += s * bs
    }
    return acc
}
def matmul_q4_fv_w16(var y : array<float> | #; wq : array<uint8> | #; ws : array<float> | #; x : array<float> | #; n, d : int64) {
    let nb = n / 32l
    let nh = n / 2l
    unsafe {
        let xp = addr(x[0])
        for (i in range64(d)) {
            y[i] = dot_q4_fv_w16(reinterpret<uint8 const?>(addr(wq[i * nh])), reinterpret<float const?>(addr(ws[i * nb])), xp, n)
        }
    }
}

// Q4 variant FLAT: one for[vectorize] over all n/2 bytes with a loop-carried accumulator (one
// horizontal reduce per row, the dot_q8_flat trick). The split-half index needs bk/16 (block) and
// bk%16 (within) per byte, so the x gather is strided — the IR shows whether the vectorizer copes.
[hint(unsafe_range_check, noalias = x, noalias = wq, noalias = ws)]
def dot_q4_flat(wq : uint8 const?; ws : float const?; x : float const?; n : int64) : float {
    var acc = 0.0
    let nbytes = n / 2l
    for [vectorize, vectorize_width = 8, unroll_count = 2] (bk in range64(nbytes)) {
        let blk = bk / 16l
        let within = bk - blk * 16l
        let s = unsafe(ws[blk])
        let byte = int(unsafe(wq[bk]))
        let lo = float((byte & 0x0F) - 8)
        let hi = float((byte >> 4) - 8)
        let xbase = blk * 32l + within
        acc += s * (lo * unsafe(x[xbase]) + hi * unsafe(x[xbase + 16l]))
    }
    return acc
}
def matmul_q4_flat(var y : array<float> | #; wq : array<uint8> | #; ws : array<float> | #; x : array<float> | #; n, d : int64) {
    let nb = n / 32l
    let nh = n / 2l
    unsafe {
        let xp = addr(x[0])
        for (i in range64(d)) {
            y[i] = dot_q4_flat(reinterpret<uint8 const?>(addr(wq[i * nh])), reinterpret<float const?>(addr(ws[i * nb])), xp, n)
        }
    }
}

//! Threaded Q4_0 matmul (float4 dot, register-only so it threads cleanly). Same shape as
//! matmul_q8f4_par: split d output rows into num_jobs chunks, workers read shared pointers.
def matmul_q4_par(var y : array<float>; wq : array<uint8> | #; ws : array<float> | #; x : array<float>; n, d : int64; num_jobs : int) {
    let nb = n / 32l
    let nh = n / 2l
    unsafe {
        let yp = addr(y[0])
        let wp = reinterpret<uint8 const?>(addr(wq[0]))
        let sp = reinterpret<float const?>(addr(ws[0]))
        let xp = addr(x[0])
        parallel_for(0, int(d), num_jobs) $(rb, re, wg) {
            unsafe {
                var myp = reinterpret<float?>(yp)
                for (i in range(rb, re)) {
                    myp[i] = dot_q4(wp + int64(i) * nh, sp + int64(i) * nb, xp, n)
                }
            }
            wg |> notify_and_release
        }
    }
}

//! Threaded Q4_0 matmul using the for[vectorize] per-block dot.
def matmul_q4fv_par(var y : array<float>; wq : array<uint8> | #; ws : array<float> | #; x : array<float>; n, d : int64; num_jobs : int) {
    let nb = n / 32l
    let nh = n / 2l
    unsafe {
        let yp = addr(y[0])
        let wp = reinterpret<uint8 const?>(addr(wq[0]))
        let sp = reinterpret<float const?>(addr(ws[0]))
        let xp = addr(x[0])
        parallel_for(0, int(d), num_jobs) $(rb, re, wg) {
            unsafe {
                var myp = reinterpret<float?>(yp)
                for (i in range(rb, re)) {
                    myp[i] = dot_q4_fv(wp + int64(i) * nh, sp + int64(i) * nb, xp, n)
                }
            }
            wg |> notify_and_release
        }
    }
}

//! Threaded Q4_0 matmul using the flat (loop-carried accumulator) dot.
def matmul_q4flat_par(var y : array<float>; wq : array<uint8> | #; ws : array<float> | #; x : array<float>; n, d : int64; num_jobs : int) {
    let nb = n / 32l
    let nh = n / 2l
    unsafe {
        let yp = addr(y[0])
        let wp = reinterpret<uint8 const?>(addr(wq[0]))
        let sp = reinterpret<float const?>(addr(ws[0]))
        let xp = addr(x[0])
        parallel_for(0, int(d), num_jobs) $(rb, re, wg) {
            unsafe {
                var myp = reinterpret<float?>(yp)
                for (i in range(rb, re)) {
                    myp[i] = dot_q4_flat(wp + int64(i) * nh, sp + int64(i) * nb, xp, n)
                }
            }
            wg |> notify_and_release
        }
    }
}

//! Threaded matmul: split the d output rows into `num_jobs` chunks via parallel_for.
//! Each worker reads main-context y/w/x through raw pointers (unsafe cross-context), so
//! dot_vec runs per row on a worker. Requires an enclosing with_job_que. Sweep num_jobs.
def matmul_par(var y : array<float>; w : array<float> | #; x : array<float>; n, d : int64; num_jobs : int) {
    unsafe {
        let yp = addr(y[0])
        let wp = reinterpret<float const?>(addr(w[0]))  // strip the `#` if w is a view
        let xp = addr(x[0])
        parallel_for(0, int(d), num_jobs) $(rb, re, wg) {
            unsafe {
                var myp = reinterpret<float?>(yp)  // var (not let): let would const-bind the write pointer
                for (i in range(rb, re)) {
                    myp[i] = dot_vec(wp + int64(i) * n, xp, n)
                }
            }
            wg |> notify_and_release
        }
    }
}
