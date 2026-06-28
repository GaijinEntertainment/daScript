options gen2

module dasllama_math shared public

require math

// dasLLAMA numeric primitives — naive fp32 baseline (curriculum step 1).
// Correctness first; optimization (blocking, float4, threading) comes later.

//! y = W·x — matrix-vector product, the dominant LLM compute kernel.
//! W is row-major [d rows x n cols], x is [n], y is [d]; each y[i] is one dot
//! product of W's i-th row with x. Shapes match llama2.c (n = input dim, d = output dim).
def matmul(var y : array<float>; w : array<float>; x : array<float>; n, d : int) {
    for (i in range(d)) {
        var acc = 0.0
        let row = i * n
        for (j in range(n)) {
            acc += w[row + j] * x[j]
        }
        y[i] = acc
    }
}

//! RMSNorm — root-mean-square normalization (no mean subtraction, no bias).
//! o[i] = weight[i] * x[i] / sqrt(mean(x^2) + eps). Matches llama2.c (eps = 1e-5).
def rmsnorm(var o : array<float>; x : array<float>; weight : array<float>; size : int) {
    var ss = 0.0
    for (j in range(size)) {
        ss += x[j] * x[j]
    }
    ss /= float(size)
    ss += 1e-5
    ss = 1.0 / sqrt(ss)
    for (j in range(size)) {
        o[j] = weight[j] * (x[j] * ss)
    }
}
