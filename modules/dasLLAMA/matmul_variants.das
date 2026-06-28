options gen2

module matmul_variants shared public

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
