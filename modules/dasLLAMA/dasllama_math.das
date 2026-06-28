options gen2

module dasllama_math shared public

require math
require daslib/jobque_boost public  // matmul's threaded path expands parallel_for; consumers need its symbols

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

// n*d above this -> split the output rows across worker threads. Below it, the per-call
// job-dispatch overhead (~90us) dwarfs the gain, so stay single-thread. (Measured on M1 Max:
// classifier 32000x288 = 9.2M threads to ~2x; ffn 768x288 = 0.22M would be ~5x slower.)
let MATMUL_PAR_THRESHOLD = 2_000_000l

//! y = W·x — matrix-vector product, the dominant LLM compute kernel.
//! W is row-major [d rows x n cols], x is [n], y is [d]; each y[i] is one dot
//! product of W's i-th row with x. Shapes match llama2.c (n = input dim, d = output dim).
//! Large matmuls thread the output rows across workers (requires an enclosing with_job_que);
//! small ones stay single-thread. Workers read y/w/x via raw pointers (unsafe cross-context).
def matmul(var y : array<float>; w : array<float> | #; x : array<float>; n, d : int64) {
    if (n * d >= MATMUL_PAR_THRESHOLD) {
        unsafe {
            let yp = addr(y[0])
            let wp = reinterpret<float const?>(addr(w[0]))  // strip the `#` if w is a view
            let xp = addr(x[0])
            parallel_for(0, int(d), get_total_hw_jobs()) $(rb, re, wg) {
                unsafe {
                    var myp = reinterpret<float?>(yp)  // var: lambda capture would const-bind the write pointer
                    for (i in range(rb, re)) {
                        myp[i] = dot(wp + int64(i) * n, xp, n)
                    }
                }
                wg |> notify_and_release
            }
        }
    } else {
        unsafe {
            let xp = addr(x[0])
            for (i in range64(d)) {
                y[i] = dot(reinterpret<float const?>(addr(w[i * n])), xp, n)
            }
        }
    }
}

//! Q8_0 dequant-in-dot: weights are int8 quants `wq` + one fp32 scale `ws` per 32-block,
//! activations `x` stay fp32. A float4 accumulator is carried across the blocks (scale
//! applied per block as a broadcast fma), with a single horizontal reduce at the end —
//! this layout won the kernel bench (see matmul_variants). n must be a multiple of 32.
def dot_q8(wq : int8 const?; ws : float const?; x : float const?; n : int64) : float {
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

//! Q8_0 matmul: y = W·x with W stored as int8 quants `wq` [d x n] + per-block scales
//! `ws` [d x n/32]. Same auto-dispatch as the fp32 matmul; large matmuls thread the output
//! rows (2x the worker count — Q8 is compute-bound and threads better than fp32). n % 32 == 0.
def matmul_q8(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; x : array<float>; n, d : int64) {
    let nb = n / 32l
    if (n * d >= MATMUL_PAR_THRESHOLD) {
        unsafe {
            let yp = addr(y[0])
            let wp = reinterpret<int8 const?>(addr(wq[0]))
            let sp = reinterpret<float const?>(addr(ws[0]))
            let xp = addr(x[0])
            parallel_for(0, int(d), 2 * get_total_hw_jobs()) $(rb, re, wg) {
                unsafe {
                    var myp = reinterpret<float?>(yp)
                    for (i in range(rb, re)) {
                        myp[i] = dot_q8(wp + int64(i) * n, sp + int64(i) * nb, xp, n)
                    }
                }
                wg |> notify_and_release
            }
        }
    } else {
        unsafe {
            let xp = addr(x[0])
            for (i in range64(d)) {
                y[i] = dot_q8(reinterpret<int8 const?>(addr(wq[i * n])), reinterpret<float const?>(addr(ws[i * nb])), xp, n)
            }
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
