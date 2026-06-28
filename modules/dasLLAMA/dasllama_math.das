options gen2

module dasllama_math shared public

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
