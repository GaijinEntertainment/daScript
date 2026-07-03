options gen2

module dasllama_math_x64_avx shared public

require dasllama/dasllama_math
require dasllama/dasllama_math_default  // portable mx4/groupn fallback kernels (no native x64 tier yet — the MXFP4-x64 arc)
require llvm/daslib/x64_avx  // dot32: JIT-emitted 256-bit x86 sign-trick dot (VPMADDUBSW+VPMADDWD ymm), scalar fallback elsewhere
require daslib/jobque_boost public  // kernels expand parallel_for; consumers need its symbols
require dasllama/dasllama_par  // maybe_parallel_for
require math  // min: the batch kernel's token-block clamp

// x86-64 dot32 backends for the Q8·Q8 matmuls — the x64 mirror of dasllama_math_aarch64_neon.das
// (read that header for the registry / threading / dispatch contract). Four backends, all
// registered at [init] gated on x86_64 + jit_enabled() + cpu_supports("avx2"): "x64-avx2" (row-major dot4x4, priority 10,
// the auto default), "x64-avx2-repack" (grp4 4-row block-interleave + token-blocked batch,
// priority 20, picked by select_matmul_backend_for_load), and two pinnable experiments —
// "x64-avx2-acc8" (llama.cpp-shaped float[8] memory accumulator, priority 9; fastest at LOW thread
// counts, wash at 32T) and "x64-avx2-ps" (fused float epilogue, priority 8; -13% on Zen2, kept for
// A/B). Off-JIT the dot intrinsics run their scalar fallbacks — slower than the vectorized portable
// kernels — so the portable backend stays selected there, as on every non-x64 host.
//
// EXACTNESS: dot32's sign-trick lowering requires activation bytes in [-127, 127]; daslang's Q8
// quantizer guarantees that by construction (d = amax/127 — see x64_avx.das). Weights may be any
// int8 including -128, so foreign-written GGUF Q8 weights are safe as-is.

//! x86 dot32 dot: 4 blocks/iter, 4 independent int4 accumulators (each block = ONE dot32 of a full
//! 32-byte Q8 block, ymm), hsum + dual-scale flushed per block into 4 separate float accumulators —
//! the same structure as the NEON sdot4x4 kernel. dot32's high/low-half fold regroups lanes within
//! a block, but the per-block hsum makes the flushed float identical to the two-dot4 form, so
//! results stay bit-for-bit close to dot_q8q8. Not `private`: called from inside the lifted
//! parallel_for worker lambdas, which can't see module-private functions.
[hint(unsafe_range_check, noalias = wq, noalias = xq, noalias = ws, noalias = xs)]
def dot_q8q8_x64dot4x4(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {
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
            a0 = dot32(a0, wq + bb, xq + bb)
            a1 = dot32(a1, wq + bb + 32l, xq + bb + 32l)
            a2 = dot32(a2, wq + bb + 64l, xq + bb + 64l)
            a3 = dot32(a3, wq + bb + 96l, xq + bb + 96l)
            f0 += float(a0.x + a0.y + a0.z + a0.w) * ws[bi + 0l] * xs[bi + 0l]
            f1 += float(a1.x + a1.y + a1.z + a1.w) * ws[bi + 1l] * xs[bi + 1l]
            f2 += float(a2.x + a2.y + a2.z + a2.w) * ws[bi + 2l] * xs[bi + 2l]
            f3 += float(a3.x + a3.y + a3.z + a3.w) * ws[bi + 3l] * xs[bi + 3l]
            bi += 4l
        }
        while (bi < nb) {
            let base = bi * 32l
            var acc = int4(0, 0, 0, 0)
            acc = dot32(acc, wq + base, xq + base)
            f0 += float(acc.x + acc.y + acc.z + acc.w) * ws[bi] * xs[bi]
            bi++
        }
    }
    return f0 + f1 + f2 + f3
}

def private q8q8_kernel_x64_dot4x4(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
    let nb = n / 32l
    var myp = yp
    maybe_parallel_for(n * d >= g_matmul_par_threshold, 0, int(d), get_total_hw_jobs()) $(rb, re) {
        unsafe {
            for (i in range(rb, re)) {
                myp[i] = dot_q8q8_x64dot4x4(wp + int64(i) * n, sp + int64(i) * nb, xqp, xsp, n)
            }
        }
    }
}

def private q8q8_batch_kernel_x64_dot4x4(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    let nb = n / 32l
    var myp = yp
    maybe_parallel_for(n * d * ntok >= g_matmul_par_threshold, 0, int(d), get_total_hw_jobs()) $(rb, re) {
        unsafe {
            for (i in range(rb, re)) {
                let wrow = wp + int64(i) * n
                let srow = sp + int64(i) * nb
                for (tk in range64(ntok)) {
                    myp[tk * d + int64(i)] = dot_q8q8_x64dot4x4(wrow, srow, xqp + tk * n, xsp + tk * nb, n)
                }
            }
        }
    }
}

def private q8q8_group3_kernel_x64_dot4x4(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?;
                                          woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {
    let nb = n / 32l
    let dtot = d0 + d1 + d2
    var w0 = y0p
    var w1 = y1p
    var w2 = y2p
    parallel_for(0, int(dtot), get_total_hw_jobs()) $(rb, re, wg) {
        unsafe {
            for (gi in range(rb, re)) {
                let g = int64(gi)
                if (g < d0) {
                    w0[g] = dot_q8q8_x64dot4x4(wp + woff0 + g * n, sp + woff0 / 32l + g * nb, xqp, xsp, n)
                } elif (g < d0 + d1) {
                    let r = g - d0
                    w1[r] = dot_q8q8_x64dot4x4(wp + woff1 + r * n, sp + woff1 / 32l + r * nb, xqp, xsp, n)
                } else {
                    let r = g - d0 - d1
                    w2[r] = dot_q8q8_x64dot4x4(wp + woff2 + r * n, sp + woff2 / 32l + r * nb, xqp, xsp, n)
                }
            }
        }
        wg |> notify_and_release
    }
}

// ===== ps backend (dot32_ps: fused float epilogue on row-major weights) =====
// Same row-major layout and block order as the dot4x4 family, but the per-block epilogue (int4
// hsum + scalar scale chain, ~10 µops/block) is fused into the dot32_ps intrinsic (VCVTDQ2PS +
// broadcast mul + vector add, 3 µops) and the horizontal reduce happens ONCE per row — the
// llama.cpp vec_dot shape. Float accumulation differs from dot4x4/portable (per-lane float
// partials, scaled per block), so this tier is tolerance-gated (A/B maxdiff + token parity),
// not bit-identity-gated.

//! One full Q8 row against one token via dot32_ps: float4 partial accumulators, 4-block unroll
//! for ILP, one horizontal reduce at row end. Not `private` (worker lambdas call it).
[hint(unsafe_range_check, noalias = wq, noalias = ws, noalias = xq, noalias = xs)]
def dot_q8q8_x64ps(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {
    let nb = n / 32l
    var f0 = float4(0.0, 0.0, 0.0, 0.0)
    var f1 = float4(0.0, 0.0, 0.0, 0.0)
    var f2 = float4(0.0, 0.0, 0.0, 0.0)
    var f3 = float4(0.0, 0.0, 0.0, 0.0)
    var bi = 0l
    unsafe {
        while (bi + 4l <= nb) {
            let bb = bi * 32l
            f0 = dot32_ps(f0, wq + bb, xq + bb, ws[bi] * xs[bi])
            f1 = dot32_ps(f1, wq + bb + 32l, xq + bb + 32l, ws[bi + 1l] * xs[bi + 1l])
            f2 = dot32_ps(f2, wq + bb + 64l, xq + bb + 64l, ws[bi + 2l] * xs[bi + 2l])
            f3 = dot32_ps(f3, wq + bb + 96l, xq + bb + 96l, ws[bi + 3l] * xs[bi + 3l])
            bi += 4l
        }
        while (bi < nb) {
            f0 = dot32_ps(f0, wq + bi * 32l, xq + bi * 32l, ws[bi] * xs[bi])
            bi++
        }
    }
    let f = f0 + f1 + f2 + f3
    return f.x + f.y + f.z + f.w
}

def private q8q8_kernel_x64_ps(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
    let nb = n / 32l
    var myp = yp
    maybe_parallel_for(n * d >= g_matmul_par_threshold, 0, int(d), get_total_hw_jobs()) $(rb, re) {
        unsafe {
            for (i in range(rb, re)) {
                myp[i] = dot_q8q8_x64ps(wp + int64(i) * n, sp + int64(i) * nb, xqp, xsp, n)
            }
        }
    }
}

def private q8q8_batch_kernel_x64_ps(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    let nb = n / 32l
    var myp = yp
    maybe_parallel_for(n * d * ntok >= g_matmul_par_threshold, 0, int(d), get_total_hw_jobs()) $(rb, re) {
        unsafe {
            for (i in range(rb, re)) {
                let wrow = wp + int64(i) * n
                let srow = sp + int64(i) * nb
                for (tk in range64(ntok)) {
                    myp[tk * d + int64(i)] = dot_q8q8_x64ps(wrow, srow, xqp + tk * n, xsp + tk * nb, n)
                }
            }
        }
    }
}

def private q8q8_group3_kernel_x64_ps(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?;
                                      woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {
    let nb = n / 32l
    let dtot = d0 + d1 + d2
    var w0 = y0p
    var w1 = y1p
    var w2 = y2p
    parallel_for(0, int(dtot), get_total_hw_jobs()) $(rb, re, wg) {
        unsafe {
            for (gi in range(rb, re)) {
                let g = int64(gi)
                if (g < d0) {
                    w0[g] = dot_q8q8_x64ps(wp + woff0 + g * n, sp + woff0 / 32l + g * nb, xqp, xsp, n)
                } elif (g < d0 + d1) {
                    let r = g - d0
                    w1[r] = dot_q8q8_x64ps(wp + woff1 + r * n, sp + woff1 / 32l + r * nb, xqp, xsp, n)
                } else {
                    let r = g - d0 - d1
                    w2[r] = dot_q8q8_x64ps(wp + woff2 + r * n, sp + woff2 / 32l + r * nb, xqp, xsp, n)
                }
            }
        }
        wg |> notify_and_release
    }
}

// ===== acc8 backend (dot32_acc8: 8-wide float accumulators, no per-block fold) =====
// Same row-major layout and block order as the dot4x4 family, but the per-block epilogue vanishes
// entirely: dot32_acc8 converts the 8 raw quads to float (VCVTDQ2PS ymm) and multiply-accumulates
// into a memory-backed float[8] that LICM promotes into a ymm register across the block loop —
// llama.cpp's vec_dot shape (under _jit_fast_math the mul+add contracts to VFMADD). No fold, no
// per-block hsum; ONE 32-lane reduce per row. Float accumulation differs from dot4x4/portable
// (per-lane partials), so this tier is tolerance-gated (A/B maxdiff + token parity), like ps.

//! One full Q8 row against one token via dot32_acc8: 4 memory-backed 8-lane float accumulators
//! (each promoted to a ymm by LICM), 4-block unroll to break the FMA latency chain, one 32-lane
//! horizontal reduce at row end. Not `private` (worker lambdas call it).
[hint(unsafe_range_check, noalias = wq, noalias = ws, noalias = xq, noalias = xs)]
def dot_q8q8_x64acc8(wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; n : int64) : float {
    let nb = n / 32l
    var a0 : float[8]
    var a1 : float[8]
    var a2 : float[8]
    var a3 : float[8]
    var bi = 0l
    unsafe {
        var p0 = addr(a0[0])
        var p1 = addr(a1[0])
        var p2 = addr(a2[0])
        var p3 = addr(a3[0])
        while (bi + 4l <= nb) {
            let bb = bi * 32l
            dot32_acc8(p0, wq + bb, xq + bb, ws[bi] * xs[bi])
            dot32_acc8(p1, wq + bb + 32l, xq + bb + 32l, ws[bi + 1l] * xs[bi + 1l])
            dot32_acc8(p2, wq + bb + 64l, xq + bb + 64l, ws[bi + 2l] * xs[bi + 2l])
            dot32_acc8(p3, wq + bb + 96l, xq + bb + 96l, ws[bi + 3l] * xs[bi + 3l])
            bi += 4l
        }
        while (bi < nb) {
            dot32_acc8(p0, wq + bi * 32l, xq + bi * 32l, ws[bi] * xs[bi])
            bi++
        }
    }
    var t = 0.0
    for (j in range(8)) {
        t += (a0[j] + a1[j]) + (a2[j] + a3[j])
    }
    return t
}

def private q8q8_kernel_x64_acc8(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
    let nb = n / 32l
    var myp = yp
    maybe_parallel_for(n * d >= g_matmul_par_threshold, 0, int(d), get_total_hw_jobs()) $(rb, re) {
        unsafe {
            for (i in range(rb, re)) {
                myp[i] = dot_q8q8_x64acc8(wp + int64(i) * n, sp + int64(i) * nb, xqp, xsp, n)
            }
        }
    }
}

def private q8q8_batch_kernel_x64_acc8(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    let nb = n / 32l
    var myp = yp
    // OVERSPLIT: more chunks than workers — idle workers pop a late-woken worker's remaining
    // chunks from the fifo, so a straggler strands one small chunk instead of a whole 1/K range.
    // Measured on the 3990X @32 workers, qo x 512 tokens: x1 473, x4 655, x8 627 GMAC/s (hardware
    // ceiling 772 same-session) — x4 is the knee; ggml's chunked mul_mat is the same defense.
    maybe_parallel_for(n * d * ntok >= g_matmul_par_threshold, 0, int(d), get_total_hw_jobs() * get_q8_batch_chunks_per_job()) $(rb, re) {
        unsafe {
            for (i in range(rb, re)) {
                let wrow = wp + int64(i) * n
                let srow = sp + int64(i) * nb
                for (tk in range64(ntok)) {
                    myp[tk * d + int64(i)] = dot_q8q8_x64acc8(wrow, srow, xqp + tk * n, xsp + tk * nb, n)
                }
            }
        }
    }
}

def private q8q8_group3_kernel_x64_acc8(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?;
                                        woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {
    let nb = n / 32l
    let dtot = d0 + d1 + d2
    var w0 = y0p
    var w1 = y1p
    var w2 = y2p
    parallel_for(0, int(dtot), get_total_hw_jobs()) $(rb, re, wg) {
        unsafe {
            for (gi in range(rb, re)) {
                let g = int64(gi)
                if (g < d0) {
                    w0[g] = dot_q8q8_x64acc8(wp + woff0 + g * n, sp + woff0 / 32l + g * nb, xqp, xsp, n)
                } elif (g < d0 + d1) {
                    let r = g - d0
                    w1[r] = dot_q8q8_x64acc8(wp + woff1 + r * n, sp + woff1 / 32l + r * nb, xqp, xsp, n)
                } else {
                    let r = g - d0 - d1
                    w2[r] = dot_q8q8_x64acc8(wp + woff2 + r * n, sp + woff2 / 32l + r * nb, xqp, xsp, n)
                }
            }
        }
        wg |> notify_and_release
    }
}

// ===== grp4 backend (dot32 over block-interleaved 4-row groups) — Q8 model load only =====
// The x64 mirror of the NEON laneq tier, at BLOCK granularity: dot32 consumes whole 32-byte Q8
// blocks, so a 4-row group interleaves per block (row0's block | row1's | row2's | row3's = 128
// bytes at wg + bi*128 — the same group/stride arithmetic as NEON's laneq, only the inner byte
// order differs) and the scales go float4-per-block (sg[bi*4 + r]). One activation block feeds all
// 4 rows (the x load CSEs; the sign-trick's psign is per-row since it keys on the weight), and the
// flush is one float4 mul-add instead of 4 scalar chains. Activated only by
// select_matmul_backend_for_load at Q8 model load (direct row-major callers would read the
// interleaved layout as garbage). Tail rows (d % 4) stay row-major and run the dot4x4 fallback.
// Float accumulation order differs from the row-major kernel (per-block flush vs dot4x4's 4-block
// grouping) — A/B against portable is tolerance-gated, and parity gates token-for-token, as with
// the NEON laneq tier. [tuned]-rig migration for the tile is a follow-up once the shape settles.

//! dot32 over one 4-row block-interleaved group against one token: per block, the shared x block
//! against the 4 row blocks into 4 int4 accumulators, hsum'd into one float4 flush with the group's
//! float4 scales × the token block scale. Not `private`: called from inside the lifted parallel_for
//! worker lambdas.
[hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xq, noalias = xs)]
def dot_q8q8_x64grp4(wg : int8 const?; sg : float const?; xq : int8 const?; xs : float const?; n : int64) : float4 {
    let nb = n / 32l
    var f = float4(0.0, 0.0, 0.0, 0.0)
    unsafe {
        for (bi in range64(nb)) {
            let wb = bi * 128l
            let xb = xq + bi * 32l
            var a0 = int4(0, 0, 0, 0)
            var a1 = int4(0, 0, 0, 0)
            var a2 = int4(0, 0, 0, 0)
            var a3 = int4(0, 0, 0, 0)
            a0 = dot32(a0, wg + wb, xb)
            a1 = dot32(a1, wg + wb + 32l, xb)
            a2 = dot32(a2, wg + wb + 64l, xb)
            a3 = dot32(a3, wg + wb + 96l, xb)
            let s = float4(sg[bi * 4l], sg[bi * 4l + 1l], sg[bi * 4l + 2l], sg[bi * 4l + 3l])
            let xsb = xs[bi]
            f += float4(float(a0.x + a0.y + a0.z + a0.w), float(a1.x + a1.y + a1.z + a1.w),
                float(a2.x + a2.y + a2.z + a2.w), float(a3.x + a3.y + a3.z + a3.w)) * s * float4(xsb, xsb, xsb, xsb)
        }
    }
    return f
}

//! 4-row × 2-token tile: one interleaved group against 2 tokens, 8 int4 accumulators. Each weight
//! block's load+abs (the sign-trick's unsigned side) is shared across both tokens by CSE, each
//! token's x block across the 4 rows — the AVX2 analog of the NEON laneq4x4 tile at the register
//! budget dot32's per-pair accumulators allow (4 rows × 4 tokens would need 16 live xmm accs).
//! Writes 8 outputs token-major. ntok - t0 >= 2 assumed. Not `private` (worker lambdas call it).
[hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xqp, noalias = xsp)]
def dot_q8q8_x64grp4x2(var yp : float?; wg : int8 const?; sg : float const?; xqp : int8 const?; xsp : float const?; n, d, g, t0 : int64) {
    let nb = n / 32l
    var f0 = float4(0.0, 0.0, 0.0, 0.0)
    var f1 = float4(0.0, 0.0, 0.0, 0.0)
    unsafe {
        let x0 = xqp + (t0 + 0l) * n
        let x1 = xqp + (t0 + 1l) * n
        let xs0 = xsp + (t0 + 0l) * nb
        let xs1 = xsp + (t0 + 1l) * nb
        for (bi in range64(nb)) {
            let wb = bi * 128l
            let xb = bi * 32l
            var a00 = int4(0, 0, 0, 0)
            var a01 = int4(0, 0, 0, 0)
            var a02 = int4(0, 0, 0, 0)
            var a03 = int4(0, 0, 0, 0)
            var a10 = int4(0, 0, 0, 0)
            var a11 = int4(0, 0, 0, 0)
            var a12 = int4(0, 0, 0, 0)
            var a13 = int4(0, 0, 0, 0)
            a00 = dot32(a00, wg + wb, x0 + xb); a10 = dot32(a10, wg + wb, x1 + xb)
            a01 = dot32(a01, wg + wb + 32l, x0 + xb); a11 = dot32(a11, wg + wb + 32l, x1 + xb)
            a02 = dot32(a02, wg + wb + 64l, x0 + xb); a12 = dot32(a12, wg + wb + 64l, x1 + xb)
            a03 = dot32(a03, wg + wb + 96l, x0 + xb); a13 = dot32(a13, wg + wb + 96l, x1 + xb)
            let s = float4(sg[bi * 4l], sg[bi * 4l + 1l], sg[bi * 4l + 2l], sg[bi * 4l + 3l])
            let q0 = xs0[bi]
            let q1 = xs1[bi]
            f0 += float4(float(a00.x + a00.y + a00.z + a00.w), float(a01.x + a01.y + a01.z + a01.w),
                float(a02.x + a02.y + a02.z + a02.w), float(a03.x + a03.y + a03.z + a03.w)) * s * float4(q0, q0, q0, q0)
            f1 += float4(float(a10.x + a10.y + a10.z + a10.w), float(a11.x + a11.y + a11.z + a11.w),
                float(a12.x + a12.y + a12.z + a12.w), float(a13.x + a13.y + a13.z + a13.w)) * s * float4(q1, q1, q1, q1)
        }
        let r = g * 4l
        yp[(t0 + 0l) * d + r + 0l] = f0.x
        yp[(t0 + 0l) * d + r + 1l] = f0.y
        yp[(t0 + 0l) * d + r + 2l] = f0.z
        yp[(t0 + 0l) * d + r + 3l] = f0.w
        yp[(t0 + 1l) * d + r + 0l] = f1.x
        yp[(t0 + 1l) * d + r + 1l] = f1.y
        yp[(t0 + 1l) * d + r + 2l] = f1.z
        yp[(t0 + 1l) * d + r + 3l] = f1.w
    }
}

// In-place repack of one Q8 weight region [d x n] into 4-row block-interleaved groups (+ float4-
// per-block scales). Copies the region to a scratch, then writes the interleaved layout back; tail
// rows (d % 4) stay row-major. Runs once per weight tensor at load, on the main context.
def private repack_q8q8_x64(var wp : int8?; var sp : float?; n, d : int64) {
    let nb = n / 32l
    let ng = d / 4l
    var tw : array<int8>
    var ts : array<float>
    tw |> resize(d * n)
    ts |> resize(d * nb)
    unsafe {
        var twp = addr(tw[0])
        var tsp = addr(ts[0])
        for (i in range64(d * n)) {
            twp[i] = wp[i]
        }
        for (i in range64(d * nb)) {
            tsp[i] = sp[i]
        }
        for (g in range64(ng)) {
            let gbase = g * 4l * n
            for (bi in range64(nb)) {
                let dst = gbase + bi * 128l
                for (r in range64(4l)) {
                    let src = (g * 4l + r) * n + bi * 32l
                    for (k in range64(32l)) {
                        wp[dst + r * 32l + k] = twp[src + k]
                    }
                }
            }
            let sgbase = g * 4l * nb
            for (bi in range64(nb)) {
                for (r in range64(4l)) {
                    sp[sgbase + bi * 4l + r] = tsp[(g * 4l + r) * nb + bi]
                }
            }
        }
    }
    delete tw
    delete ts
}

def private q8q8_kernel_x64_grp4(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
    let nb = n / 32l
    let ng = d / 4l
    var myp = yp
    maybe_parallel_for(n * d >= g_matmul_par_threshold, 0, int(ng), get_total_hw_jobs()) $(rb, re) {
        unsafe {
            for (gi in range(rb, re)) {
                let g = int64(gi)
                let f = dot_q8q8_x64grp4(wp + g * 4l * n, sp + g * 4l * nb, xqp, xsp, n)
                myp[g * 4l + 0l] = f.x
                myp[g * 4l + 1l] = f.y
                myp[g * 4l + 2l] = f.z
                myp[g * 4l + 3l] = f.w
            }
        }
    }
    unsafe {
        for (i in range64(ng * 4l, d)) {
            myp[i] = dot_q8q8_x64dot4x4(wp + i * n, sp + i * nb, xqp, xsp, n)
        }
    }
}

def private q8q8_batch_kernel_x64_grp4(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    let nb = n / 32l
    let ng = d / 4l
    var myp = yp
    // Token-blocked like the NEON laneq batch kernel: TB tokens outer, groups inner, so the TB×n
    // activation slice stays cache-resident across this thread's groups (see that kernel's note).
    // OVERSPLIT (see the acc8 batch kernel note), FLOORED at 4 groups per chunk: the token-blocked
    // walk re-reads the TB x n activation slice per chunk, so 1-group chunks pay it 4x. Measured
    // @32 workers ntok=512: kv ng=128 x1 456 > x2 447 > x4 425 GMAC/s (floor binds -> x1); qo
    // ng=512 x4 473 > x1 430 (floor doesn't bind -> full oversplit).
    let TB = effective_token_block(g_q8_token_block, n)
    let hw = get_total_hw_jobs()
    let njobs = clamp(int(ng / 4l), hw, hw * get_q8_batch_chunks_per_job())
    maybe_parallel_for(n * d * ntok >= g_matmul_par_threshold, 0, int(ng), njobs) $(rb, re) {
        unsafe {
            var tb = 0l
            while (tb < ntok) {
                let tbe = min(tb + TB, ntok)
                let tb2 = tb + ((tbe - tb) / 2l) * 2l
                for (gi in range(rb, re)) {
                    let g = int64(gi)
                    let wgp = wp + g * 4l * n
                    let sgp = sp + g * 4l * nb
                    var tk = tb
                    while (tk < tb2) {     // 4-row × 2-token tiles (the throughput path)
                        dot_q8q8_x64grp4x2(myp, wgp, sgp, xqp, xsp, n, d, g, tk)
                        tk += 2l
                    }
                    while (tk < tbe) {    // token tail within this block: one token at a time
                        let f = dot_q8q8_x64grp4(wgp, sgp, xqp + tk * n, xsp + tk * nb, n)
                        myp[tk * d + g * 4l + 0l] = f.x
                        myp[tk * d + g * 4l + 1l] = f.y
                        myp[tk * d + g * 4l + 2l] = f.z
                        myp[tk * d + g * 4l + 3l] = f.w
                        tk++
                    }
                }
                tb += TB
            }
        }
    }
    unsafe {
        for (i in range64(ng * 4l, d)) {   // row tail (d % 4): row-major dot4x4
            let wrow = wp + i * n
            let srow = sp + i * nb
            for (tk in range64(ntok)) {
                myp[tk * d + i] = dot_q8q8_x64dot4x4(wrow, srow, xqp + tk * n, xsp + tk * nb, n)
            }
        }
    }
}

def private q8q8_group3_kernel_x64_grp4(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?;
                                        woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {
    let nb = n / 32l
    let ng0 = d0 / 4l
    let ng1 = d1 / 4l
    let ng2 = d2 / 4l
    let ngtot = ng0 + ng1 + ng2
    var w0 = y0p
    var w1 = y1p
    var w2 = y2p
    parallel_for(0, int(ngtot), get_total_hw_jobs()) $(rb, re, wg) {
        unsafe {
            for (gi in range(rb, re)) {
                let g = int64(gi)
                if (g < ng0) {
                    let f = dot_q8q8_x64grp4(wp + woff0 + g * 4l * n, sp + woff0 / 32l + g * 4l * nb, xqp, xsp, n)
                    w0[g * 4l + 0l] = f.x
                    w0[g * 4l + 1l] = f.y
                    w0[g * 4l + 2l] = f.z
                    w0[g * 4l + 3l] = f.w
                } elif (g < ng0 + ng1) {
                    let r = g - ng0
                    let f = dot_q8q8_x64grp4(wp + woff1 + r * 4l * n, sp + woff1 / 32l + r * 4l * nb, xqp, xsp, n)
                    w1[r * 4l + 0l] = f.x
                    w1[r * 4l + 1l] = f.y
                    w1[r * 4l + 2l] = f.z
                    w1[r * 4l + 3l] = f.w
                } else {
                    let r = g - ng0 - ng1
                    let f = dot_q8q8_x64grp4(wp + woff2 + r * 4l * n, sp + woff2 / 32l + r * 4l * nb, xqp, xsp, n)
                    w2[r * 4l + 0l] = f.x
                    w2[r * 4l + 1l] = f.y
                    w2[r * 4l + 2l] = f.z
                    w2[r * 4l + 3l] = f.w
                }
            }
        }
        wg |> notify_and_release
    }
    unsafe {
        for (i in range64(ng0 * 4l, d0)) {
            w0[i] = dot_q8q8_x64dot4x4(wp + woff0 + i * n, sp + woff0 / 32l + i * nb, xqp, xsp, n)
        }
        for (i in range64(ng1 * 4l, d1)) {
            w1[i] = dot_q8q8_x64dot4x4(wp + woff1 + i * n, sp + woff1 / 32l + i * nb, xqp, xsp, n)
        }
        for (i in range64(ng2 * 4l, d2)) {
            w2[i] = dot_q8q8_x64dot4x4(wp + woff2 + i * n, sp + woff2 / 32l + i * nb, xqp, xsp, n)
        }
    }
}

[init]
def dasllama_math_x64_avx_register() {
    // Only register under JIT on x86-64 WITH AVX2. Off-JIT (interpreter or AOT) dot32 runs its SCALAR
    // fallback body — slower than the portable dot_q8q8, which the vectorizer handles — so the
    // portable backend stays selected everywhere the hardware sequence isn't emitted. The
    // cpu_supports("avx2") half mirrors the JIT's own emitter gate (g_target_x64_avx2): on an
    // AVX2-less x64 box the emitter refuses the intrinsics anyway, so these kernels would be the
    // same scalar fallbacks — registering them would replace portable with something slower.
    if (get_architecture_name() == "x86_64" && jit_enabled() && cpu_supports("avx2")) {
        // The mx4/groupn tiers register the PORTABLE forms (dasllama_math_default) on every x64
        // backend — no native x64 MXFP4/groupn kernels yet (the next arc); MoE/MXFP4 models stay
        // correct at portable speed instead of panicking in the unset kernels.
        // x64-avx2: dot4x4 on row-major weights — auto-selected default for direct callers
        // (tests, benchmarks). priority 10 out-ranks portable (0).
        register_kernel_backend(KernelBackend(name = "x64-avx2", mm = @@q8q8_kernel_x64_dot4x4,
            batch = @@q8q8_batch_kernel_x64_dot4x4, group3 = @@q8q8_group3_kernel_x64_dot4x4,
            repack = @@repack_q8q8_identity, needs_repack = false, priority = 10,
            mm_mx4 = @@mx4q8_kernel, groupn = @@q8q8_groupn_kernel, groupn_mx4 = @@mx4q8_groupn_kernel))
        // x64-avx2-ps: dot32_ps fused float epilogue on the same row-major weights. MEASURED SLOWER
        // than the int-flush dot4x4 on Zen2 (27.9 vs 32.0 t/s decode): the scalar epilogue ran on
        // the idle scalar ALU ports in parallel with the ymm pmadd stream, while the vector epilogue
        // (fold+cvt+mul+add) competes for the same FP ports. Kept registered at priority 8 (never
        // auto-selected) as the pinnable A/B baseline for the 8-wide-accumulator follow-up.
        register_kernel_backend(KernelBackend(name = "x64-avx2-ps", mm = @@q8q8_kernel_x64_ps,
            batch = @@q8q8_batch_kernel_x64_ps, group3 = @@q8q8_group3_kernel_x64_ps,
            repack = @@repack_q8q8_identity, needs_repack = false, priority = 8,
            mm_mx4 = @@mx4q8_kernel, groupn = @@q8q8_groupn_kernel, groupn_mx4 = @@mx4q8_groupn_kernel))
        // x64-avx2-acc8: dot32_acc8 — llama.cpp's vec_dot per-block shape (no fold, no per-block
        // hsum; VCVTDQ2PS + mul-add — VFMADD under fast-math — into LICM-promoted ymm accumulators,
        // one reduce per row). Priority 9: pinnable A/B experiment; promote above the dot4x4 tier
        // only if it measures faster in-model.
        register_kernel_backend(KernelBackend(name = "x64-avx2-acc8", mm = @@q8q8_kernel_x64_acc8,
            batch = @@q8q8_batch_kernel_x64_acc8, group3 = @@q8q8_group3_kernel_x64_acc8,
            repack = @@repack_q8q8_identity, needs_repack = false, priority = 9,
            mm_mx4 = @@mx4q8_kernel, groupn = @@q8q8_groupn_kernel, groupn_mx4 = @@mx4q8_groupn_kernel))
        // x64-avx2-repack: grp4 + block-interleaved repack — the highest priority (20), but
        // needs_repack, so only select_matmul_backend_for_load (at Q8 model load, which then
        // repacks) activates it. The mx4 side stays row-major (repack_mx4 identity default) so
        // the portable mx4 kernels read the layout they expect.
        register_kernel_backend(KernelBackend(name = "x64-avx2-repack", mm = @@q8q8_kernel_x64_grp4,
            batch = @@q8q8_batch_kernel_x64_grp4, group3 = @@q8q8_group3_kernel_x64_grp4,
            repack = @@repack_q8q8_x64, needs_repack = true, priority = 20,
            mm_mx4 = @@mx4q8_kernel, groupn = @@q8q8_groupn_kernel, groupn_mx4 = @@mx4q8_groupn_kernel))
    }
}
