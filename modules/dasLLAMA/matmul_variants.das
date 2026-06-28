options gen2

module matmul_variants shared public

require daslib/jobque_boost

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
