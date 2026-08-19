options gen2

module attn_gemm_variants shared public

require dasllama/dasllama_lint public
require daslib/jobque_boost
require math   // mad (fused multiply-add) in the tiled kernel

// fp32 micro-GEMM for flash attention: C[M x N] += A[M x K] * B[K x N], all row-major, with B's
// N as the contiguous (SIMD) dimension. This is the form that makes QKᵀ and P·V fast — a
// broadcast-A FMA register tile with NO horizontal reduce (mirrors llama.cpp ggml simd-gemm.h).
// Every variant lives here so bench_attn_gemm.das can compare them head-to-head; the winner is
// promoted into the engine. Pointers (not arrays) so the kernels are fork/thread-callable.

//! Reference: scalar triple loop, C += A·B (sum over kk in increasing order). Correctness oracle
//! and the scalar fallback for tile remainders. Not fast — never benched as a contender.
def gemm_ref(var cp : float?; ap, bp : float const?; M, K, N : int64) {
    unsafe {
        for (i in range64(M)) {
            let crow = i * N
            for (kk in range64(K)) {
                let a = ap[i * K + kk]
                let brow = kk * N
                for (j in range64(N)) {
                    cp[crow + j] += a * bp[brow + j]
                }
            }
        }
    }
}

// Scalar C += A·B over a sub-block rows [i0,i1) × cols [j0,j1) — covers the L-shaped remainder
// (right strip + bottom strip) left by the 4×16 tiling.
def private gemm_ref_region(var cp : float?; ap, bp : float const?; i0, i1, j0, j1, K, N : int64) {
    unsafe {
        for (i in range64(i0, i1)) {
            let crow = i * N
            for (kk in range64(K)) {
                let a = ap[i * K + kk]
                let brow = kk * N
                for (j in range64(j0, j1)) {
                    cp[crow + j] += a * bp[brow + j]
                }
            }
        }
    }
}

// 4-row × 16-col register tile: 16 float4 accumulators (4 rows × 4 lanes-of-4), broadcast-A FMA,
// zero horizontal reduce. cols [j0, j0+16) of rows [i0, i0+4). C is accumulated (+=).
def private uk_4x16(var cp : float?; ap, bp : float const?; i0, j0, K, N : int64) {
    unsafe {
        let o0 = (i0 + 0l) * N + j0; let o1 = (i0 + 1l) * N + j0
        let o2 = (i0 + 2l) * N + j0; let o3 = (i0 + 3l) * N + j0
        var r0a = *reinterpret<float4?>(cp + o0);       var r0b = *reinterpret<float4?>(cp + o0 + 4l)
        var r0c = *reinterpret<float4?>(cp + o0 + 8l);  var r0d = *reinterpret<float4?>(cp + o0 + 12l)
        var r1a = *reinterpret<float4?>(cp + o1);       var r1b = *reinterpret<float4?>(cp + o1 + 4l)
        var r1c = *reinterpret<float4?>(cp + o1 + 8l);  var r1d = *reinterpret<float4?>(cp + o1 + 12l)
        var r2a = *reinterpret<float4?>(cp + o2);       var r2b = *reinterpret<float4?>(cp + o2 + 4l)
        var r2c = *reinterpret<float4?>(cp + o2 + 8l);  var r2d = *reinterpret<float4?>(cp + o2 + 12l)
        var r3a = *reinterpret<float4?>(cp + o3);       var r3b = *reinterpret<float4?>(cp + o3 + 4l)
        var r3c = *reinterpret<float4?>(cp + o3 + 8l);  var r3d = *reinterpret<float4?>(cp + o3 + 12l)
        for (kk in range64(K)) {
            let bo = kk * N + j0
            let b0 = *reinterpret<float4 const?>(bp + bo);      let b1 = *reinterpret<float4 const?>(bp + bo + 4l)
            let b2 = *reinterpret<float4 const?>(bp + bo + 8l); let b3 = *reinterpret<float4 const?>(bp + bo + 12l)
            let a0 = ap[(i0 + 0l) * K + kk]; let v0 = float4(a0, a0, a0, a0)
            r0a = mad(b0, v0, r0a); r0b = mad(b1, v0, r0b); r0c = mad(b2, v0, r0c); r0d = mad(b3, v0, r0d)
            let a1 = ap[(i0 + 1l) * K + kk]; let v1 = float4(a1, a1, a1, a1)
            r1a = mad(b0, v1, r1a); r1b = mad(b1, v1, r1b); r1c = mad(b2, v1, r1c); r1d = mad(b3, v1, r1d)
            let a2 = ap[(i0 + 2l) * K + kk]; let v2 = float4(a2, a2, a2, a2)
            r2a = mad(b0, v2, r2a); r2b = mad(b1, v2, r2b); r2c = mad(b2, v2, r2c); r2d = mad(b3, v2, r2d)
            let a3 = ap[(i0 + 3l) * K + kk]; let v3 = float4(a3, a3, a3, a3)
            r3a = mad(b0, v3, r3a); r3b = mad(b1, v3, r3b); r3c = mad(b2, v3, r3c); r3d = mad(b3, v3, r3d)
        }
        *reinterpret<float4?>(cp + o0) = r0a;       *reinterpret<float4?>(cp + o0 + 4l) = r0b
        *reinterpret<float4?>(cp + o0 + 8l) = r0c;  *reinterpret<float4?>(cp + o0 + 12l) = r0d
        *reinterpret<float4?>(cp + o1) = r1a;       *reinterpret<float4?>(cp + o1 + 4l) = r1b
        *reinterpret<float4?>(cp + o1 + 8l) = r1c;  *reinterpret<float4?>(cp + o1 + 12l) = r1d
        *reinterpret<float4?>(cp + o2) = r2a;       *reinterpret<float4?>(cp + o2 + 4l) = r2b
        *reinterpret<float4?>(cp + o2 + 8l) = r2c;  *reinterpret<float4?>(cp + o2 + 12l) = r2d
        *reinterpret<float4?>(cp + o3) = r3a;       *reinterpret<float4?>(cp + o3 + 4l) = r3b
        *reinterpret<float4?>(cp + o3 + 8l) = r3c;  *reinterpret<float4?>(cp + o3 + 12l) = r3d
    }
}

// Same tile, but separate fmul+fadd (the pre-mad form: no fused multiply-add). A/B baseline that
// isolates the mad→fmuladd effect on one binary.
def private uk_4x16_nofma(var cp : float?; ap, bp : float const?; i0, j0, K, N : int64) {
    unsafe {
        let o0 = (i0 + 0l) * N + j0; let o1 = (i0 + 1l) * N + j0
        let o2 = (i0 + 2l) * N + j0; let o3 = (i0 + 3l) * N + j0
        var r0a = *reinterpret<float4?>(cp + o0);       var r0b = *reinterpret<float4?>(cp + o0 + 4l)
        var r0c = *reinterpret<float4?>(cp + o0 + 8l);  var r0d = *reinterpret<float4?>(cp + o0 + 12l)
        var r1a = *reinterpret<float4?>(cp + o1);       var r1b = *reinterpret<float4?>(cp + o1 + 4l)
        var r1c = *reinterpret<float4?>(cp + o1 + 8l);  var r1d = *reinterpret<float4?>(cp + o1 + 12l)
        var r2a = *reinterpret<float4?>(cp + o2);       var r2b = *reinterpret<float4?>(cp + o2 + 4l)
        var r2c = *reinterpret<float4?>(cp + o2 + 8l);  var r2d = *reinterpret<float4?>(cp + o2 + 12l)
        var r3a = *reinterpret<float4?>(cp + o3);       var r3b = *reinterpret<float4?>(cp + o3 + 4l)
        var r3c = *reinterpret<float4?>(cp + o3 + 8l);  var r3d = *reinterpret<float4?>(cp + o3 + 12l)
        for (kk in range64(K)) {
            let bo = kk * N + j0
            let b0 = *reinterpret<float4 const?>(bp + bo);      let b1 = *reinterpret<float4 const?>(bp + bo + 4l)
            let b2 = *reinterpret<float4 const?>(bp + bo + 8l); let b3 = *reinterpret<float4 const?>(bp + bo + 12l)
            let a0 = ap[(i0 + 0l) * K + kk]; let v0 = float4(a0, a0, a0, a0)
            r0a += b0 * v0; r0b += b1 * v0; r0c += b2 * v0; r0d += b3 * v0
            let a1 = ap[(i0 + 1l) * K + kk]; let v1 = float4(a1, a1, a1, a1)
            r1a += b0 * v1; r1b += b1 * v1; r1c += b2 * v1; r1d += b3 * v1
            let a2 = ap[(i0 + 2l) * K + kk]; let v2 = float4(a2, a2, a2, a2)
            r2a += b0 * v2; r2b += b1 * v2; r2c += b2 * v2; r2d += b3 * v2
            let a3 = ap[(i0 + 3l) * K + kk]; let v3 = float4(a3, a3, a3, a3)
            r3a += b0 * v3; r3b += b1 * v3; r3c += b2 * v3; r3d += b3 * v3
        }
        *reinterpret<float4?>(cp + o0) = r0a;       *reinterpret<float4?>(cp + o0 + 4l) = r0b
        *reinterpret<float4?>(cp + o0 + 8l) = r0c;  *reinterpret<float4?>(cp + o0 + 12l) = r0d
        *reinterpret<float4?>(cp + o1) = r1a;       *reinterpret<float4?>(cp + o1 + 4l) = r1b
        *reinterpret<float4?>(cp + o1 + 8l) = r1c;  *reinterpret<float4?>(cp + o1 + 12l) = r1d
        *reinterpret<float4?>(cp + o2) = r2a;       *reinterpret<float4?>(cp + o2 + 4l) = r2b
        *reinterpret<float4?>(cp + o2 + 8l) = r2c;  *reinterpret<float4?>(cp + o2 + 12l) = r2d
        *reinterpret<float4?>(cp + o3) = r3a;       *reinterpret<float4?>(cp + o3 + 4l) = r3b
        *reinterpret<float4?>(cp + o3 + 8l) = r3c;  *reinterpret<float4?>(cp + o3 + 12l) = r3d
    }
}

// mad tile with the kk loop unrolled ×2 (the hints/unroll experiment).
def private uk_4x16_u2(var cp : float?; ap, bp : float const?; i0, j0, K, N : int64) {
    unsafe {
        let o0 = (i0 + 0l) * N + j0; let o1 = (i0 + 1l) * N + j0
        let o2 = (i0 + 2l) * N + j0; let o3 = (i0 + 3l) * N + j0
        var r0a = *reinterpret<float4?>(cp + o0);       var r0b = *reinterpret<float4?>(cp + o0 + 4l)
        var r0c = *reinterpret<float4?>(cp + o0 + 8l);  var r0d = *reinterpret<float4?>(cp + o0 + 12l)
        var r1a = *reinterpret<float4?>(cp + o1);       var r1b = *reinterpret<float4?>(cp + o1 + 4l)
        var r1c = *reinterpret<float4?>(cp + o1 + 8l);  var r1d = *reinterpret<float4?>(cp + o1 + 12l)
        var r2a = *reinterpret<float4?>(cp + o2);       var r2b = *reinterpret<float4?>(cp + o2 + 4l)
        var r2c = *reinterpret<float4?>(cp + o2 + 8l);  var r2d = *reinterpret<float4?>(cp + o2 + 12l)
        var r3a = *reinterpret<float4?>(cp + o3);       var r3b = *reinterpret<float4?>(cp + o3 + 4l)
        var r3c = *reinterpret<float4?>(cp + o3 + 8l);  var r3d = *reinterpret<float4?>(cp + o3 + 12l)
        for [unroll_count = 2] (kk in range64(K)) {
            let bo = kk * N + j0
            let b0 = *reinterpret<float4 const?>(bp + bo);      let b1 = *reinterpret<float4 const?>(bp + bo + 4l)
            let b2 = *reinterpret<float4 const?>(bp + bo + 8l); let b3 = *reinterpret<float4 const?>(bp + bo + 12l)
            let a0 = ap[(i0 + 0l) * K + kk]; let v0 = float4(a0, a0, a0, a0)
            r0a = mad(b0, v0, r0a); r0b = mad(b1, v0, r0b); r0c = mad(b2, v0, r0c); r0d = mad(b3, v0, r0d)
            let a1 = ap[(i0 + 1l) * K + kk]; let v1 = float4(a1, a1, a1, a1)
            r1a = mad(b0, v1, r1a); r1b = mad(b1, v1, r1b); r1c = mad(b2, v1, r1c); r1d = mad(b3, v1, r1d)
            let a2 = ap[(i0 + 2l) * K + kk]; let v2 = float4(a2, a2, a2, a2)
            r2a = mad(b0, v2, r2a); r2b = mad(b1, v2, r2b); r2c = mad(b2, v2, r2c); r2d = mad(b3, v2, r2d)
            let a3 = ap[(i0 + 3l) * K + kk]; let v3 = float4(a3, a3, a3, a3)
            r3a = mad(b0, v3, r3a); r3b = mad(b1, v3, r3b); r3c = mad(b2, v3, r3c); r3d = mad(b3, v3, r3d)
        }
        *reinterpret<float4?>(cp + o0) = r0a;       *reinterpret<float4?>(cp + o0 + 4l) = r0b
        *reinterpret<float4?>(cp + o0 + 8l) = r0c;  *reinterpret<float4?>(cp + o0 + 12l) = r0d
        *reinterpret<float4?>(cp + o1) = r1a;       *reinterpret<float4?>(cp + o1 + 4l) = r1b
        *reinterpret<float4?>(cp + o1 + 8l) = r1c;  *reinterpret<float4?>(cp + o1 + 12l) = r1d
        *reinterpret<float4?>(cp + o2) = r2a;       *reinterpret<float4?>(cp + o2 + 4l) = r2b
        *reinterpret<float4?>(cp + o2 + 8l) = r2c;  *reinterpret<float4?>(cp + o2 + 12l) = r2d
        *reinterpret<float4?>(cp + o3) = r3a;       *reinterpret<float4?>(cp + o3 + 4l) = r3b
        *reinterpret<float4?>(cp + o3 + 8l) = r3c;  *reinterpret<float4?>(cp + o3 + 12l) = r3d
    }
}

//! Register-tiled fp32 GEMM, C += A·B. 4×16 tile over the bulk; the L-shaped remainder
//! (cols N%16, rows M%4) falls back to scalar. General over M/K/N. Tolerance-equal to gemm_ref
//! (FMA contraction reorders low bits), not bit-exact.
def gemm_tiled(var cp : float?; ap, bp : float const?; M, K, N : int64) {
    let m4 = M - M % 4l
    let n16 = N - N % 16l
    var ii = 0l
    while (ii < m4) {
        var jj = 0l
        while (jj < n16) {
            uk_4x16(cp, ap, bp, ii, jj, K, N)
            jj += 16l
        }
        ii += 4l
    }
    if (n16 < N) {                                   // right strip: rows [0,m4) × cols [n16,N)
        gemm_ref_region(cp, ap, bp, 0l, m4, n16, N, K, N)
    }
    if (m4 < M) {                                    // bottom strip: rows [m4,M) × all cols
        gemm_ref_region(cp, ap, bp, m4, M, 0l, N, K, N)
    }
}

//! Pre-mad baseline (separate fmul+fadd) — A/B against gemm_tiled to isolate the FMA effect.
def gemm_tiled_nofma(var cp : float?; ap, bp : float const?; M, K, N : int64) {
    let m4 = M - M % 4l
    let n16 = N - N % 16l
    var ii = 0l
    while (ii < m4) {
        var jj = 0l
        while (jj < n16) {
            uk_4x16_nofma(cp, ap, bp, ii, jj, K, N)
            jj += 16l
        }
        ii += 4l
    }
    if (n16 < N) {
        gemm_ref_region(cp, ap, bp, 0l, m4, n16, N, K, N)
    }
    if (m4 < M) {
        gemm_ref_region(cp, ap, bp, m4, M, 0l, N, K, N)
    }
}

//! mad tile with the kk loop unrolled ×2 — the hints/unroll experiment.
def gemm_tiled_u2(var cp : float?; ap, bp : float const?; M, K, N : int64) {
    let m4 = M - M % 4l
    let n16 = N - N % 16l
    var ii = 0l
    while (ii < m4) {
        var jj = 0l
        while (jj < n16) {
            uk_4x16_u2(cp, ap, bp, ii, jj, K, N)
            jj += 16l
        }
        ii += 4l
    }
    if (n16 < N) {
        gemm_ref_region(cp, ap, bp, 0l, m4, n16, N, K, N)
    }
    if (m4 < M) {
        gemm_ref_region(cp, ap, bp, m4, M, 0l, N, K, N)
    }
}

// 8-row × 8-col register tile: 16 float4 accumulators, broadcast-A FMA — taller than
// 4×16 so every B load feeds 8 rows (2x the B reuse; B streaming is the 4×16 wall at M=4).
def private uk_8x8(var cp : float?; ap, bp : float const?; i0, j0, K, N : int64) {
    unsafe {
        let o0 = (i0 + 0l) * N + j0; let o1 = (i0 + 1l) * N + j0
        let o2 = (i0 + 2l) * N + j0; let o3 = (i0 + 3l) * N + j0
        let o4 = (i0 + 4l) * N + j0; let o5 = (i0 + 5l) * N + j0
        let o6 = (i0 + 6l) * N + j0; let o7 = (i0 + 7l) * N + j0
        var r0a = *reinterpret<float4?>(cp + o0);  var r0b = *reinterpret<float4?>(cp + o0 + 4l)
        var r1a = *reinterpret<float4?>(cp + o1);  var r1b = *reinterpret<float4?>(cp + o1 + 4l)
        var r2a = *reinterpret<float4?>(cp + o2);  var r2b = *reinterpret<float4?>(cp + o2 + 4l)
        var r3a = *reinterpret<float4?>(cp + o3);  var r3b = *reinterpret<float4?>(cp + o3 + 4l)
        var r4a = *reinterpret<float4?>(cp + o4);  var r4b = *reinterpret<float4?>(cp + o4 + 4l)
        var r5a = *reinterpret<float4?>(cp + o5);  var r5b = *reinterpret<float4?>(cp + o5 + 4l)
        var r6a = *reinterpret<float4?>(cp + o6);  var r6b = *reinterpret<float4?>(cp + o6 + 4l)
        var r7a = *reinterpret<float4?>(cp + o7);  var r7b = *reinterpret<float4?>(cp + o7 + 4l)
        for (kk in range64(K)) {
            let bo = kk * N + j0
            let b0 = *reinterpret<float4 const?>(bp + bo);      let b1 = *reinterpret<float4 const?>(bp + bo + 4l)
            let a0 = ap[(i0 + 0l) * K + kk]; let v0 = float4(a0, a0, a0, a0)
            r0a = mad(b0, v0, r0a); r0b = mad(b1, v0, r0b)
            let a1 = ap[(i0 + 1l) * K + kk]; let v1 = float4(a1, a1, a1, a1)
            r1a = mad(b0, v1, r1a); r1b = mad(b1, v1, r1b)
            let a2 = ap[(i0 + 2l) * K + kk]; let v2 = float4(a2, a2, a2, a2)
            r2a = mad(b0, v2, r2a); r2b = mad(b1, v2, r2b)
            let a3 = ap[(i0 + 3l) * K + kk]; let v3 = float4(a3, a3, a3, a3)
            r3a = mad(b0, v3, r3a); r3b = mad(b1, v3, r3b)
            let a4 = ap[(i0 + 4l) * K + kk]; let v4 = float4(a4, a4, a4, a4)
            r4a = mad(b0, v4, r4a); r4b = mad(b1, v4, r4b)
            let a5 = ap[(i0 + 5l) * K + kk]; let v5 = float4(a5, a5, a5, a5)
            r5a = mad(b0, v5, r5a); r5b = mad(b1, v5, r5b)
            let a6 = ap[(i0 + 6l) * K + kk]; let v6 = float4(a6, a6, a6, a6)
            r6a = mad(b0, v6, r6a); r6b = mad(b1, v6, r6b)
            let a7 = ap[(i0 + 7l) * K + kk]; let v7 = float4(a7, a7, a7, a7)
            r7a = mad(b0, v7, r7a); r7b = mad(b1, v7, r7b)
        }
        *reinterpret<float4?>(cp + o0) = r0a;  *reinterpret<float4?>(cp + o0 + 4l) = r0b
        *reinterpret<float4?>(cp + o1) = r1a;  *reinterpret<float4?>(cp + o1 + 4l) = r1b
        *reinterpret<float4?>(cp + o2) = r2a;  *reinterpret<float4?>(cp + o2 + 4l) = r2b
        *reinterpret<float4?>(cp + o3) = r3a;  *reinterpret<float4?>(cp + o3 + 4l) = r3b
        *reinterpret<float4?>(cp + o4) = r4a;  *reinterpret<float4?>(cp + o4 + 4l) = r4b
        *reinterpret<float4?>(cp + o5) = r5a;  *reinterpret<float4?>(cp + o5 + 4l) = r5b
        *reinterpret<float4?>(cp + o6) = r6a;  *reinterpret<float4?>(cp + o6 + 4l) = r6b
        *reinterpret<float4?>(cp + o7) = r7a;  *reinterpret<float4?>(cp + o7 + 4l) = r7b
    }
}

//! 8×8 tile over the bulk; L-shaped remainder falls back to scalar. Same per-element
//! kk-order fma chains as the 4×16 form — tile geometry does not change per-element math.
def gemm_tiled_8x8(var cp : float?; ap, bp : float const?; M, K, N : int64) {
    let mT = M - M % 8l
    let nT = N - N % 8l
    var ii = 0l
    while (ii < mT) {
        var jj = 0l
        while (jj < nT) {
            uk_8x8(cp, ap, bp, ii, jj, K, N)
            jj += 8l
        }
        ii += 8l
    }
    if (nT < N) {                                   // right strip: rows [0,mT) × cols [nT,N)
        gemm_ref_region(cp, ap, bp, 0l, mT, nT, N, K, N)
    }
    if (mT < M) {                                   // bottom strip: rows [mT,M) × all cols
        gemm_ref_region(cp, ap, bp, mT, M, 0l, N, K, N)
    }
}

// 12-row × 8-col register tile: 24 float4 accumulators, broadcast-A FMA — taller than
// 4×16 so every B load feeds 12 rows (3x the B reuse; B streaming is the 4×16 wall at M=4).
def private uk_12x8(var cp : float?; ap, bp : float const?; i0, j0, K, N : int64) {
    unsafe {
        let o0 = (i0 + 0l) * N + j0; let o1 = (i0 + 1l) * N + j0
        let o2 = (i0 + 2l) * N + j0; let o3 = (i0 + 3l) * N + j0
        let o4 = (i0 + 4l) * N + j0; let o5 = (i0 + 5l) * N + j0
        let o6 = (i0 + 6l) * N + j0; let o7 = (i0 + 7l) * N + j0
        let o8 = (i0 + 8l) * N + j0; let o9 = (i0 + 9l) * N + j0
        let o10 = (i0 + 10l) * N + j0; let o11 = (i0 + 11l) * N + j0
        var r0a = *reinterpret<float4?>(cp + o0);  var r0b = *reinterpret<float4?>(cp + o0 + 4l)
        var r1a = *reinterpret<float4?>(cp + o1);  var r1b = *reinterpret<float4?>(cp + o1 + 4l)
        var r2a = *reinterpret<float4?>(cp + o2);  var r2b = *reinterpret<float4?>(cp + o2 + 4l)
        var r3a = *reinterpret<float4?>(cp + o3);  var r3b = *reinterpret<float4?>(cp + o3 + 4l)
        var r4a = *reinterpret<float4?>(cp + o4);  var r4b = *reinterpret<float4?>(cp + o4 + 4l)
        var r5a = *reinterpret<float4?>(cp + o5);  var r5b = *reinterpret<float4?>(cp + o5 + 4l)
        var r6a = *reinterpret<float4?>(cp + o6);  var r6b = *reinterpret<float4?>(cp + o6 + 4l)
        var r7a = *reinterpret<float4?>(cp + o7);  var r7b = *reinterpret<float4?>(cp + o7 + 4l)
        var r8a = *reinterpret<float4?>(cp + o8);  var r8b = *reinterpret<float4?>(cp + o8 + 4l)
        var r9a = *reinterpret<float4?>(cp + o9);  var r9b = *reinterpret<float4?>(cp + o9 + 4l)
        var r10a = *reinterpret<float4?>(cp + o10);  var r10b = *reinterpret<float4?>(cp + o10 + 4l)
        var r11a = *reinterpret<float4?>(cp + o11);  var r11b = *reinterpret<float4?>(cp + o11 + 4l)
        for (kk in range64(K)) {
            let bo = kk * N + j0
            let b0 = *reinterpret<float4 const?>(bp + bo);      let b1 = *reinterpret<float4 const?>(bp + bo + 4l)
            let a0 = ap[(i0 + 0l) * K + kk]; let v0 = float4(a0, a0, a0, a0)
            r0a = mad(b0, v0, r0a); r0b = mad(b1, v0, r0b)
            let a1 = ap[(i0 + 1l) * K + kk]; let v1 = float4(a1, a1, a1, a1)
            r1a = mad(b0, v1, r1a); r1b = mad(b1, v1, r1b)
            let a2 = ap[(i0 + 2l) * K + kk]; let v2 = float4(a2, a2, a2, a2)
            r2a = mad(b0, v2, r2a); r2b = mad(b1, v2, r2b)
            let a3 = ap[(i0 + 3l) * K + kk]; let v3 = float4(a3, a3, a3, a3)
            r3a = mad(b0, v3, r3a); r3b = mad(b1, v3, r3b)
            let a4 = ap[(i0 + 4l) * K + kk]; let v4 = float4(a4, a4, a4, a4)
            r4a = mad(b0, v4, r4a); r4b = mad(b1, v4, r4b)
            let a5 = ap[(i0 + 5l) * K + kk]; let v5 = float4(a5, a5, a5, a5)
            r5a = mad(b0, v5, r5a); r5b = mad(b1, v5, r5b)
            let a6 = ap[(i0 + 6l) * K + kk]; let v6 = float4(a6, a6, a6, a6)
            r6a = mad(b0, v6, r6a); r6b = mad(b1, v6, r6b)
            let a7 = ap[(i0 + 7l) * K + kk]; let v7 = float4(a7, a7, a7, a7)
            r7a = mad(b0, v7, r7a); r7b = mad(b1, v7, r7b)
            let a8 = ap[(i0 + 8l) * K + kk]; let v8 = float4(a8, a8, a8, a8)
            r8a = mad(b0, v8, r8a); r8b = mad(b1, v8, r8b)
            let a9 = ap[(i0 + 9l) * K + kk]; let v9 = float4(a9, a9, a9, a9)
            r9a = mad(b0, v9, r9a); r9b = mad(b1, v9, r9b)
            let a10 = ap[(i0 + 10l) * K + kk]; let v10 = float4(a10, a10, a10, a10)
            r10a = mad(b0, v10, r10a); r10b = mad(b1, v10, r10b)
            let a11 = ap[(i0 + 11l) * K + kk]; let v11 = float4(a11, a11, a11, a11)
            r11a = mad(b0, v11, r11a); r11b = mad(b1, v11, r11b)
        }
        *reinterpret<float4?>(cp + o0) = r0a;  *reinterpret<float4?>(cp + o0 + 4l) = r0b
        *reinterpret<float4?>(cp + o1) = r1a;  *reinterpret<float4?>(cp + o1 + 4l) = r1b
        *reinterpret<float4?>(cp + o2) = r2a;  *reinterpret<float4?>(cp + o2 + 4l) = r2b
        *reinterpret<float4?>(cp + o3) = r3a;  *reinterpret<float4?>(cp + o3 + 4l) = r3b
        *reinterpret<float4?>(cp + o4) = r4a;  *reinterpret<float4?>(cp + o4 + 4l) = r4b
        *reinterpret<float4?>(cp + o5) = r5a;  *reinterpret<float4?>(cp + o5 + 4l) = r5b
        *reinterpret<float4?>(cp + o6) = r6a;  *reinterpret<float4?>(cp + o6 + 4l) = r6b
        *reinterpret<float4?>(cp + o7) = r7a;  *reinterpret<float4?>(cp + o7 + 4l) = r7b
        *reinterpret<float4?>(cp + o8) = r8a;  *reinterpret<float4?>(cp + o8 + 4l) = r8b
        *reinterpret<float4?>(cp + o9) = r9a;  *reinterpret<float4?>(cp + o9 + 4l) = r9b
        *reinterpret<float4?>(cp + o10) = r10a;  *reinterpret<float4?>(cp + o10 + 4l) = r10b
        *reinterpret<float4?>(cp + o11) = r11a;  *reinterpret<float4?>(cp + o11 + 4l) = r11b
    }
}

//! 12×8 tile over the bulk; L-shaped remainder falls back to scalar. Same per-element
//! kk-order fma chains as the 4×16 form — tile geometry does not change per-element math.
def gemm_tiled_12x8(var cp : float?; ap, bp : float const?; M, K, N : int64) {
    let mT = M - M % 12l
    let nT = N - N % 8l
    var ii = 0l
    while (ii < mT) {
        var jj = 0l
        while (jj < nT) {
            uk_12x8(cp, ap, bp, ii, jj, K, N)
            jj += 8l
        }
        ii += 12l
    }
    if (nT < N) {                                   // right strip: rows [0,mT) × cols [nT,N)
        gemm_ref_region(cp, ap, bp, 0l, mT, nT, N, K, N)
    }
    if (mT < M) {                                   // bottom strip: rows [mT,M) × all cols
        gemm_ref_region(cp, ap, bp, mT, M, 0l, N, K, N)
    }
}

// 6-row × 16-col register tile: 24 float4 accumulators, broadcast-A FMA — taller than
// 4×16 so every B load feeds 6 rows (2x the B reuse; B streaming is the 4×16 wall at M=4).
def private uk_6x16(var cp : float?; ap, bp : float const?; i0, j0, K, N : int64) {
    unsafe {
        let o0 = (i0 + 0l) * N + j0; let o1 = (i0 + 1l) * N + j0
        let o2 = (i0 + 2l) * N + j0; let o3 = (i0 + 3l) * N + j0
        let o4 = (i0 + 4l) * N + j0; let o5 = (i0 + 5l) * N + j0
        var r0a = *reinterpret<float4?>(cp + o0);  var r0b = *reinterpret<float4?>(cp + o0 + 4l)
        var r0c = *reinterpret<float4?>(cp + o0 + 8l);  var r0d = *reinterpret<float4?>(cp + o0 + 12l)
        var r1a = *reinterpret<float4?>(cp + o1);  var r1b = *reinterpret<float4?>(cp + o1 + 4l)
        var r1c = *reinterpret<float4?>(cp + o1 + 8l);  var r1d = *reinterpret<float4?>(cp + o1 + 12l)
        var r2a = *reinterpret<float4?>(cp + o2);  var r2b = *reinterpret<float4?>(cp + o2 + 4l)
        var r2c = *reinterpret<float4?>(cp + o2 + 8l);  var r2d = *reinterpret<float4?>(cp + o2 + 12l)
        var r3a = *reinterpret<float4?>(cp + o3);  var r3b = *reinterpret<float4?>(cp + o3 + 4l)
        var r3c = *reinterpret<float4?>(cp + o3 + 8l);  var r3d = *reinterpret<float4?>(cp + o3 + 12l)
        var r4a = *reinterpret<float4?>(cp + o4);  var r4b = *reinterpret<float4?>(cp + o4 + 4l)
        var r4c = *reinterpret<float4?>(cp + o4 + 8l);  var r4d = *reinterpret<float4?>(cp + o4 + 12l)
        var r5a = *reinterpret<float4?>(cp + o5);  var r5b = *reinterpret<float4?>(cp + o5 + 4l)
        var r5c = *reinterpret<float4?>(cp + o5 + 8l);  var r5d = *reinterpret<float4?>(cp + o5 + 12l)
        for (kk in range64(K)) {
            let bo = kk * N + j0
            let b0 = *reinterpret<float4 const?>(bp + bo);      let b1 = *reinterpret<float4 const?>(bp + bo + 4l)
            let b2 = *reinterpret<float4 const?>(bp + bo + 8l);      let b3 = *reinterpret<float4 const?>(bp + bo + 12l)
            let a0 = ap[(i0 + 0l) * K + kk]; let v0 = float4(a0, a0, a0, a0)
            r0a = mad(b0, v0, r0a); r0b = mad(b1, v0, r0b); r0c = mad(b2, v0, r0c); r0d = mad(b3, v0, r0d)
            let a1 = ap[(i0 + 1l) * K + kk]; let v1 = float4(a1, a1, a1, a1)
            r1a = mad(b0, v1, r1a); r1b = mad(b1, v1, r1b); r1c = mad(b2, v1, r1c); r1d = mad(b3, v1, r1d)
            let a2 = ap[(i0 + 2l) * K + kk]; let v2 = float4(a2, a2, a2, a2)
            r2a = mad(b0, v2, r2a); r2b = mad(b1, v2, r2b); r2c = mad(b2, v2, r2c); r2d = mad(b3, v2, r2d)
            let a3 = ap[(i0 + 3l) * K + kk]; let v3 = float4(a3, a3, a3, a3)
            r3a = mad(b0, v3, r3a); r3b = mad(b1, v3, r3b); r3c = mad(b2, v3, r3c); r3d = mad(b3, v3, r3d)
            let a4 = ap[(i0 + 4l) * K + kk]; let v4 = float4(a4, a4, a4, a4)
            r4a = mad(b0, v4, r4a); r4b = mad(b1, v4, r4b); r4c = mad(b2, v4, r4c); r4d = mad(b3, v4, r4d)
            let a5 = ap[(i0 + 5l) * K + kk]; let v5 = float4(a5, a5, a5, a5)
            r5a = mad(b0, v5, r5a); r5b = mad(b1, v5, r5b); r5c = mad(b2, v5, r5c); r5d = mad(b3, v5, r5d)
        }
        *reinterpret<float4?>(cp + o0) = r0a;  *reinterpret<float4?>(cp + o0 + 4l) = r0b
        *reinterpret<float4?>(cp + o0 + 8l) = r0c;  *reinterpret<float4?>(cp + o0 + 12l) = r0d
        *reinterpret<float4?>(cp + o1) = r1a;  *reinterpret<float4?>(cp + o1 + 4l) = r1b
        *reinterpret<float4?>(cp + o1 + 8l) = r1c;  *reinterpret<float4?>(cp + o1 + 12l) = r1d
        *reinterpret<float4?>(cp + o2) = r2a;  *reinterpret<float4?>(cp + o2 + 4l) = r2b
        *reinterpret<float4?>(cp + o2 + 8l) = r2c;  *reinterpret<float4?>(cp + o2 + 12l) = r2d
        *reinterpret<float4?>(cp + o3) = r3a;  *reinterpret<float4?>(cp + o3 + 4l) = r3b
        *reinterpret<float4?>(cp + o3 + 8l) = r3c;  *reinterpret<float4?>(cp + o3 + 12l) = r3d
        *reinterpret<float4?>(cp + o4) = r4a;  *reinterpret<float4?>(cp + o4 + 4l) = r4b
        *reinterpret<float4?>(cp + o4 + 8l) = r4c;  *reinterpret<float4?>(cp + o4 + 12l) = r4d
        *reinterpret<float4?>(cp + o5) = r5a;  *reinterpret<float4?>(cp + o5 + 4l) = r5b
        *reinterpret<float4?>(cp + o5 + 8l) = r5c;  *reinterpret<float4?>(cp + o5 + 12l) = r5d
    }
}

//! 6×16 tile over the bulk; L-shaped remainder falls back to scalar. Same per-element
//! kk-order fma chains as the 4×16 form — tile geometry does not change per-element math.
def gemm_tiled_6x16(var cp : float?; ap, bp : float const?; M, K, N : int64) {
    let mT = M - M % 6l
    let nT = N - N % 16l
    var ii = 0l
    while (ii < mT) {
        var jj = 0l
        while (jj < nT) {
            uk_6x16(cp, ap, bp, ii, jj, K, N)
            jj += 16l
        }
        ii += 6l
    }
    if (nT < N) {                                   // right strip: rows [0,mT) × cols [nT,N)
        gemm_ref_region(cp, ap, bp, 0l, mT, nT, N, K, N)
    }
    if (mT < M) {                                   // bottom strip: rows [mT,M) × all cols
        gemm_ref_region(cp, ap, bp, mT, M, 0l, N, K, N)
    }
}

//! 4×16 tile walked COLUMN-BLOCKS OUTER (jj outer, ii inner): the K×16 B block stays L1-hot
//! across all row blocks of the call, so B streams from L2 once per CALL instead of once per
//! 4-row block. Same tiles, same per-element kk-order fma chains — numerically identical to
//! gemm_tiled; only the tile visit order changes.
def gemm_tiled_jo(var cp : float?; ap, bp : float const?; M, K, N : int64) {
    let m4 = M - M % 4l
    let n16 = N - N % 16l
    var jj = 0l
    while (jj < n16) {
        var ii = 0l
        while (ii < m4) {
            uk_4x16(cp, ap, bp, ii, jj, K, N)
            ii += 4l
        }
        jj += 16l
    }
    if (n16 < N) {                                   // right strip: rows [0,m4) × cols [n16,N)
        gemm_ref_region(cp, ap, bp, 0l, m4, n16, N, K, N)
    }
    if (m4 < M) {                                    // bottom strip: rows [m4,M) × all cols
        gemm_ref_region(cp, ap, bp, m4, M, 0l, N, K, N)
    }
}
