options gen2

module attn_gemm_variants shared public

require daslib/jobque_boost

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
