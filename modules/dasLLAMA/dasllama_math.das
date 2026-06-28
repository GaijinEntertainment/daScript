options gen2

module dasllama_math shared public

require math

// dasLLAMA numeric primitives — naive fp32 baseline (curriculum step 1).
// Correctness first; optimization (blocking, float4, threading) comes later.
//
// Every array param is `array<float> | #` (owning array OR borrowed view), so weights
// can be passed zero-copy as views into one big model blob and outputs can be written
// straight into a view (e.g. the KV cache). Dims/indices are int64 for large models.

//! Dot product of two n-length float buffers, auto-vectorized by the LLVM JIT
//! (width-8, no bounds checks, a/b asserted non-aliasing). Beats hand-rolled float4 ~1.8x.
[hint(unsafe_range_check, noalias = a, noalias = b)]
def dot(a, b : float const?; n : int64) : float {
    var acc = 0.0
    for [vectorize, vectorize_width = 8, unroll_count = 2] (j in range64(n)) {
        acc += unsafe(a[j]) * unsafe(b[j])
    }
    return acc
}

//! AXPY: d[j] += scale * s[j] over n elements, same auto-vectorization.
[hint(unsafe_range_check, noalias = d, noalias = s)]
def axpy(var d : float?; s : float const?; scale : float; n : int64) {
    for [vectorize, vectorize_width = 8, unroll_count = 2] (j in range64(n)) {
        unsafe {
            d[j] += scale * s[j]
        }
    }
}

//! y = W·x — matrix-vector product, the dominant LLM compute kernel.
//! W is row-major [d rows x n cols], x is [n], y is [d]; each y[i] is one dot
//! product of W's i-th row with x. Shapes match llama2.c (n = input dim, d = output dim).
def matmul(var y : array<float> | #; w : array<float> | #; x : array<float> | #; n, d : int64) {
    // reinterpret normalizes the element pointer (a weight view yields a `#` temp pointer)
    // to a plain pointer dot_vec accepts; the view stays valid for the synchronous call.
    for (i in range64(d)) {
        unsafe {
            y[i] = dot(reinterpret<float const?>(addr(w[i * n])), reinterpret<float const?>(addr(x[0])), n)
        }
    }
}

//! RMSNorm — root-mean-square normalization (no mean subtraction, no bias).
//! o[i] = weight[i] * x[i] / sqrt(mean(x^2) + eps). Matches llama2.c (eps = 1e-5).
def rmsnorm(var o : array<float> | #; x : array<float> | #; weight : array<float> | #; size : int64) {
    var ss = 0.0
    for (j in range64(size)) {
        ss += x[j] * x[j]
    }
    ss /= float(size)
    ss += 1e-5
    ss = 1.0 / sqrt(ss)
    for (j in range64(size)) {
        o[j] = weight[j] * (x[j] * ss)
    }
}

//! softmax — turns a score vector into a probability distribution (sums to 1).
//! In place over the first `size` elements. Subtracts the max first so exp() can't
//! overflow (exp(x-max)/sum(exp(x-max)) is identical math, numerically safe).
def softmax(var x : array<float> | #; size : int64) {
    var maxv = x[0]
    for (i in range64(1l, size)) {
        maxv = max(maxv, x[i])
    }
    var sum = 0.0
    for (i in range64(size)) {
        x[i] = exp(x[i] - maxv)
        sum += x[i]
    }
    for (i in range64(size)) {
        x[i] /= sum
    }
}

//! SiLU (a.k.a. swish) activation: silu(x) = x * sigmoid(x) = x / (1 + exp(-x)).
//! In place over the first `size` elements. The nonlinearity in the SwiGLU FFN gate;
//! smooth, ~identity for large x, dips slightly negative just below zero.
def silu(var x : array<float> | #; size : int64) {
    for (i in range64(size)) {
        let v = x[i]
        x[i] = v / (1.0 + exp(-v))
    }
}

//! RoPE — rotary position embedding. Rotates each adjacent coordinate pair (2j, 2j+1)
//! within a head by angle pos*freq, freq = 1/10000^(2j/head_size) (low pairs spin fast,
//! high pairs slow). This encodes position as rotation, so a later Q·K dot product
//! depends only on the relative offset between the two tokens. In place over the first
//! `n` elements (n = dim for Q, kv_dim for K); matches llama2.c's adjacent-pairs scheme.
def rope(var vec : array<float> | #; pos, head_size, n : int64) {
    for (p in range64(n / 2l)) {
        let i = p * 2l
        let head_dim = i % head_size
        let freq = 1.0 / pow(10000.0, float(head_dim) / float(head_size))
        let angle = float(pos) * freq
        let fcr = cos(angle)
        let fci = sin(angle)
        let v0 = vec[i]
        let v1 = vec[i + 1]
        vec[i] = v0 * fcr - v1 * fci
        vec[i + 1] = v0 * fci + v1 * fcr
    }
}
