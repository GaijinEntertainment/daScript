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

//! Batched fp32 GEMM for prefill: Y [ntok x d] = X [ntok x n] · Wᵀ, W row-major [d x n].
//! X is token-major (token t at X[t*n..]); Y is token-major (token t at Y[t*d..]). Weight-
//! stationary nest (rows outer) — each weight row streams from DRAM once and is reused across
//! all ntok tokens, vs ntok× matmul re-reading W every call. Bit-for-bit ntok× matmul.
def matmul_batch(var y : array<float>; w : array<float> | #; x : array<float>; n, d, ntok : int64) {
    if (n * d * ntok >= MATMUL_PAR_THRESHOLD) {
        unsafe {
            let yp = addr(y[0])
            let wp = reinterpret<float const?>(addr(w[0]))
            let xp = reinterpret<float const?>(addr(x[0]))
            parallel_for(0, int(d), get_total_hw_jobs()) $(rb, re, wg) {
                unsafe {
                    var myp = reinterpret<float?>(yp)
                    for (i in range(rb, re)) {
                        let wrow = wp + int64(i) * n
                        for (tk in range64(ntok)) {
                            myp[tk * d + int64(i)] = dot(wrow, xp + tk * n, n)
                        }
                    }
                }
                wg |> notify_and_release
            }
        }
    } else {
        unsafe {
            let wp = reinterpret<float const?>(addr(w[0]))
            let xp = reinterpret<float const?>(addr(x[0]))
            for (i in range64(d)) {
                let wrow = wp + i * n
                for (tk in range64(ntok)) {
                    y[tk * d + i] = dot(wrow, xp + tk * n, n)
                }
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

//! Q8·Q8 dot: BOTH the weight (wq/ws) and the activation (xq/xs) are Q8 — int8 quants + one fp32
//! scale per 32-block. The inner block loop is a pure int8·int8 multiply-accumulate, which the LLVM
//! JIT lowers to ARM SDOT (vdotq_s32: 16 int8 MACs/instr, exact int32 accumulate). The two block
//! scales multiply in once per 32-wide block. ~3.8x the dequant-in-dot dot_q8 on the kernel bench.
//! n must be a multiple of 32.
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

//! Q8·Q8 matmul: y = W·x with W as int8 quants `wq` [d x n] + scales `ws` [d x n/32] and the
//! activation PRE-quantized to (xq [n], xs [n/32]) by the caller (it's shared across all d rows, so
//! quantizing once amortizes). Large matmuls thread the rows at 1 chunk per hw job (1N — measured
//! fastest for this compute-light kernel); small ones stay single-thread. n % 32 == 0.
def matmul_q8q8(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; xq : array<int8>; xs : array<float>; n, d : int64) {
    let nb = n / 32l
    if (n * d >= MATMUL_PAR_THRESHOLD) {
        unsafe {
            let yp = addr(y[0])
            let wp = reinterpret<int8 const?>(addr(wq[0]))
            let sp = reinterpret<float const?>(addr(ws[0]))
            let xqp = reinterpret<int8 const?>(addr(xq[0]))
            let xsp = reinterpret<float const?>(addr(xs[0]))
            // 1N (one chunk per hw job) measured best on the real shapes with the cond-var + fork-pool
            // job manager — both in the kernel microbench and end-to-end (~52 vs ~51 tok/s at 2N).
            // The compute-light SDOT kernel gains nothing from more chunks; they only add dispatch cost.
            parallel_for(0, int(d), get_total_hw_jobs()) $(rb, re, wg) {
                unsafe {
                    var myp = reinterpret<float?>(yp)
                    for (i in range(rb, re)) {
                        myp[i] = dot_q8q8(wp + int64(i) * n, sp + int64(i) * nb, xqp, xsp, n)
                    }
                }
                wg |> notify_and_release
            }
        }
    } else {
        unsafe {
            let xqp = reinterpret<int8 const?>(addr(xq[0]))
            let xsp = reinterpret<float const?>(addr(xs[0]))
            for (i in range64(d)) {
                y[i] = dot_q8q8(reinterpret<int8 const?>(addr(wq[i * n])), reinterpret<float const?>(addr(ws[i * nb])), xqp, xsp, n)
            }
        }
    }
}

//! Batched Q8·Q8 GEMM for prefill: same weight-stationary nest as matmul_batch over the int8
//! SDOT kernel. Activations PRE-quantized per token into (xq [ntok x n], xs [ntok x n/32]).
//! Y token-major [ntok x d]. Bit-for-bit ntok× matmul_q8q8. n % 32 == 0.
def matmul_q8q8_batch(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; xq : array<int8>; xs : array<float>; n, d, ntok : int64) {
    let nb = n / 32l
    if (n * d * ntok >= MATMUL_PAR_THRESHOLD) {
        unsafe {
            let yp = addr(y[0])
            let wp = reinterpret<int8 const?>(addr(wq[0]))
            let sp = reinterpret<float const?>(addr(ws[0]))
            let xqp = reinterpret<int8 const?>(addr(xq[0]))
            let xsp = reinterpret<float const?>(addr(xs[0]))
            parallel_for(0, int(d), get_total_hw_jobs()) $(rb, re, wg) {
                unsafe {
                    var myp = reinterpret<float?>(yp)
                    for (i in range(rb, re)) {
                        let wrow = wp + int64(i) * n
                        let srow = sp + int64(i) * nb
                        for (tk in range64(ntok)) {
                            myp[tk * d + int64(i)] = dot_q8q8(wrow, srow, xqp + tk * n, xsp + tk * nb, n)
                        }
                    }
                }
                wg |> notify_and_release
            }
        }
    } else {
        unsafe {
            let xqp = reinterpret<int8 const?>(addr(xq[0]))
            let xsp = reinterpret<float const?>(addr(xs[0]))
            for (i in range64(d)) {
                let wrow = reinterpret<int8 const?>(addr(wq[i * n]))
                let srow = reinterpret<float const?>(addr(ws[i * nb]))
                for (tk in range64(ntok)) {
                    y[tk * d + i] = dot_q8q8(wrow, srow, xqp + tk * n, xsp + tk * nb, n)
                }
            }
        }
    }
}

//! Fused Q8·Q8 for three independent row-groups that share the activation (attention Q/K/V): one
//! parallel_for over the combined d0+d1+d2 output rows, so idle workers fill in on the small K/V rows
//! that would otherwise run single-thread while the pool sits idle (~23% faster than the sequential
//! Q-parallel + K,V-single on the real shapes). wq/ws is the whole quantized blob; woff{0,1,2} are the
//! per-group weight-element offsets; the activation is pre-quantized once into (xq, xs).
// wq/ws are the whole owning quantized blob (not a view), so plain array types — keeping the kernel
// non-generic also keeps its inner dot_q8q8 resolving here, not in a caller that sees another dot_q8q8.
def matmul_q8q8_group3(var y0 : array<float>; var y1 : array<float>; var y2 : array<float>;
                       wq : array<int8>; ws : array<float>; woff0, woff1, woff2 : int64;
                       xq : array<int8>; xs : array<float>; n, d0, d1, d2 : int64) {
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
        parallel_for(0, int(dtot), get_total_hw_jobs()) $(rb, re, wg) {
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

//! Q4_0 dequant-in-dot: weights are 4-bit quants `wq` in GGML split-half packing (per 32-block,
//! byte j holds element j low / j+16 high) + one fp32 scale `ws` per block; activations `x` fp32.
//! Per block: the inner for[vectorize] widens nibbles to float at width 4 (M1 native NEON lane —
//! the heavy Q4 unpack loses ~7% at width 8 from cross-lane shuffles), then one scaled reduce per
//! block. This won the kernel bench (see matmul_variants). n must be a multiple of 32.
[hint(unsafe_range_check, noalias = x, noalias = wq, noalias = ws)]
def dot_q4(wq : uint8 const?; ws : float const?; x : float const?; n : int64) : float {
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

//! Q4_0 matmul: y = W·x with W as 4-bit quants `wq` [d x n/2 bytes] + per-block scales `ws`
//! [d x n/32]. Large matmuls thread the output rows at 4 chunks per hw job (4N — the heavy nibble
//! unpack is compute-bound, so finer splitting balances better); small ones stay single. n % 32 == 0.
def matmul_q4(var y : array<float>; wq : array<uint8> | #; ws : array<float> | #; x : array<float>; n, d : int64) {
    let nb = n / 32l
    let nh = n / 2l
    if (n * d >= MATMUL_PAR_THRESHOLD) {
        unsafe {
            let yp = addr(y[0])
            let wp = reinterpret<uint8 const?>(addr(wq[0]))
            let sp = reinterpret<float const?>(addr(ws[0]))
            let xp = addr(x[0])
            // 4N measured fastest for Q4 on the real shapes: the heavy nibble unpack makes each row
            // compute-bound, so finer splitting balances better (esp. the classifier) — more than the
            // compute-light Q8 path, which prefers 1N.
            parallel_for(0, int(d), 4 * get_total_hw_jobs()) $(rb, re, wg) {
                unsafe {
                    var myp = reinterpret<float?>(yp)
                    for (i in range(rb, re)) {
                        myp[i] = dot_q4(wp + int64(i) * nh, sp + int64(i) * nb, xp, n)
                    }
                }
                wg |> notify_and_release
            }
        }
    } else {
        unsafe {
            let xp = addr(x[0])
            for (i in range64(d)) {
                y[i] = dot_q4(reinterpret<uint8 const?>(addr(wq[i * nh])), reinterpret<float const?>(addr(ws[i * nb])), xp, n)
            }
        }
    }
}

//! RMSNorm — root-mean-square normalization (no mean subtraction, no bias).
//! o[i] = weight[i] * x[i] / sqrt(mean(x^2) + eps). eps defaults to 1e-5 (Llama-2/3);
//! Qwen2 / Gemma2 pass 1e-6 from the model's metadata.
def rmsnorm(var o : array<float> | #; x : array<float> | #; weight : array<float> | #; size : int64; eps : float = 1.0e-5) {
    var ss = 0.0
    for (j in range64(size)) {
        ss += x[j] * x[j]
    }
    ss /= float(size)
    ss += eps
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

//! GELU, tanh approximation (HF's gelu_pytorch_tanh): the GeGLU gate nonlinearity (Gemma2).
//! gelu(x) = 0.5x(1 + tanh(sqrt(2/pi)(x + 0.044715 x^3))). In place over the first `size`
//! elements. ggml's LLM_FFN_GELU approximates this same form via an fp16 lookup table; we
//! compute it directly (more accurate — near-ties against the LUT are expected, not bugs).
def gelu(var x : array<float> | #; size : int64) {
    let k = 0.7978845608028654  // sqrt(2/pi)
    for (i in range64(size)) {
        let v = x[i]
        x[i] = 0.5 * v * (1.0 + tanh(k * (v + 0.044715 * v * v * v)))
    }
}

//! Logit soft-capping (Gemma2): x = cap * tanh(x / cap), elementwise over the first `size`.
//! Smoothly bounds values to (-cap, cap). Applied to attention scores (cap 50) after the
//! 1/sqrt(head_size) scale and before softmax, and to the final classifier logits (cap 30).
def softcap(var x : array<float> | #; size : int64; cap : float) {
    let inv = 1.0 / cap
    for (i in range64(size)) {
        x[i] = cap * tanh(x[i] * inv)
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

//! RoPE with an explicit base frequency θ and optional per-pair frequency factors
//! (the GGUF `rope_freqs` tensor — Llama-3.1's NTK-by-parts scaling). Empty factors =>
//! plain RoPE at base θ, so (θ=10000, empty) reproduces rope() above bit-for-bit.
//! freq_factors[j] divides the j-th pair's frequency, matching ggml (theta/ff).
def rope_scaled(var vec : array<float> | #; pos, head_size, n : int64; theta : float; freq_factors : array<float> | #) {
    let has_ff = !empty(freq_factors)
    for (p in range64(n / 2l)) {
        let i = p * 2l
        let head_dim = i % head_size
        var freq = 1.0 / pow(theta, float(head_dim) / float(head_size))
        if (has_ff) {
            freq /= freq_factors[head_dim / 2l]
        }
        let angle = float(pos) * freq
        let fcr = cos(angle)
        let fci = sin(angle)
        let v0 = vec[i]
        let v1 = vec[i + 1]
        vec[i] = v0 * fcr - v1 * fci
        vec[i + 1] = v0 * fci + v1 * fcr
    }
}

//! NEOX-style RoPE (Qwen2 / Phi3 / Gemma2): the rotated pair is offset by head_size/2 — vec[base+j]
//! with vec[base+j+half] — instead of the adjacent (2j, 2j+1) pair the Llama-family NORM rope uses.
//! Same per-pair frequencies; only the pairing differs.
def rope_scaled_neox(var vec : array<float> | #; pos, head_size, n : int64; theta : float; freq_factors : array<float> | #; mscale : float = 1.0) {
    let has_ff = !empty(freq_factors)
    let half = head_size / 2l
    for (h in range64(n / head_size)) {
        let base = h * head_size
        for (j in range64(half)) {
            var freq = 1.0 / pow(theta, float(2l * j) / float(head_size))
            if (has_ff) {
                freq /= freq_factors[j]
            }
            let angle = float(pos) * freq
            // mscale (LongRoPE attn_factor) scales cos/sin — i.e. the rotated magnitude (Phi3 = 1.19)
            let fcr = cos(angle) * mscale
            let fci = sin(angle) * mscale
            let v0 = vec[base + j]
            let v1 = vec[base + j + half]
            vec[base + j] = v0 * fcr - v1 * fci
            vec[base + j + half] = v0 * fci + v1 * fcr
        }
    }
}

//! Dispatch RoPE by the model's convention: NEOX (Qwen2/Phi3/Gemma2) or the default NORM (Llama).
//! mscale is the LongRoPE magnitude factor (1.0 for everything but Phi3); only the NEOX path uses it.
def rope_apply(var vec : array<float> | #; pos, head_size, n : int64; theta : float; freq_factors : array<float> | #; neox : bool; mscale : float = 1.0) {
    if (neox) {
        rope_scaled_neox(vec, pos, head_size, n, theta, freq_factors, mscale)
    } else {
        rope_scaled(vec, pos, head_size, n, theta, freq_factors)
    }
}
