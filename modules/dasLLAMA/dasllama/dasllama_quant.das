options gen2
options _comment_hygiene = true

module dasllama_quant shared public

require math
require dasllama/dasllama_tune  // [tuned]: reconstitute the activation-requant kernel with per-box loop hints
require dasllama/dasllama_par   // maybe_parallel_for: the load-time bulk requant threads
require dasllama/dasllama_math  // count_inline_run / count_parallel_dispatch_thru (the par macro's counters)

// dasLLAMA quantization — curriculum step 6. Batch-1 inference is bandwidth-bound
// (time ~= bytes_read / bandwidth), so the lever is reading FEWER bytes per weight.
// Q8_0 stores 32 weights as int8 + one fp32 scale: 36 bytes vs 128 (~3.55x smaller).
// This file is the FORMAT + a quant-quality detector; the fast dequant-in-dot kernel
// comes after we can measure the loss.

let QK8 = 32l  // Q8_0 block size: 32 weights share one scale
let QK4 = 32l  // Q4_0 block size: 32 weights share one scale

//! A Q8_0-quantized tensor: int8 quants (length n) plus one fp32 scale per 32-element
//! block. Dequant is w[i] ~= q[i] * scales[i / 32]. Symmetric (zero-centered, no offset).
struct Q8Tensor {
    q : array<int8>
    scales : array<float>
    n : int64
}

//! Quantize a flat fp32 tensor to Q8_0. Per block: d = max(|w|)/127, q = round(w/d)
//! in [-127,127]. The scale factors out of the block so a later dot is d * sum(q[i]*x[i]).
//! A short final block (n not a multiple of 32) is handled.
def quantize_q8_0(src : array<float> | #; n : int64) : Q8Tensor {
    var qt : Q8Tensor
    qt.n = n
    qt.q |> resize(n)
    let nblocks = (n + QK8 - 1l) / QK8
    qt.scales |> resize(nblocks)
    for (b in range64(nblocks)) {
        let base = b * QK8
        let blen = min(QK8, n - base)
        var amax = 0.0
        for (i in range64(blen)) {
            amax = max(amax, abs(src[base + i]))
        }
        let d = amax / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0  // all-zero block: scale 0, quants 0
        qt.scales[b] = d
        for (i in range64(blen)) {
            qt.q[base + i] = int8(round(src[base + i] * id))
        }
    }
    return <- qt
}

//! Quantize a flat fp32 tensor to Q8_0 directly into pre-allocated slices: quants land at
//! qdst[qoff..], scales at sdst[soff..]. No allocation — the memory-honest loader uses this to
//! quantize one tensor at a time into the model's shared qblob/qscales. n must be a multiple of 32.
def quantize_q8_0_into(src : array<float> | #; n : int64; var qdst : array<int8>; var sdst : array<float>; qoff, soff : int64) {
    assert(n % QK8 == 0l, "quantize_q8_0_into requires n to be a multiple of 32")
    // O(1) range guards — the pointerized path below bypasses per-element bounds checks
    if (n > long_length(src) || qoff < 0l || qoff + n > long_length(qdst) || soff < 0l || soff + n / QK8 > long_length(sdst)) {
        panic("quantize_q8_0_into: [{qoff}, +{n}) / scales [{soff}, +{n / QK8}) exceeds dst ({long_length(qdst)}, {long_length(sdst)}) or src {long_length(src)}")
    }
    // threads only past ~2M elements (the load-time bulk requant); decode-sized calls stay inline
    let njobs = is_job_que_available() ? int(clamp(n / 2097152l, 1l, 64l)) : 1
    unsafe {
        let sp = addr<float const?>(src[0])
        var qp = addr(qdst[0])
        var scp = addr(sdst[0])
        maybe_parallel_for(0, int(n / QK8), njobs) $(rb, re) {
            unsafe {
                quantize_q8_0_into_ptr(sp + int64(rb) * QK8, (int64(re) - int64(rb)) * QK8,
                    qp, scp, qoff + int64(rb) * QK8, soff + int64(rb))
            }
        }
    }
}

[hint(noalias = src, noalias = qdst, noalias = sdst)]
def template quantize_q8_0_into_ptr_template(src : float const ?; n : int64; var qdst : int8 ?; var sdst : float ?; qoff, soff : int64) : void {
    let nblocks = n / QK8
    unsafe {
        let p4 = reinterpret<float4 const?>(src)
        for (b in range64(nblocks)) {
            let base = b * QK8
            // hand-vectorized |x| max: LLVM won't auto-vectorize fmax reduction without nnan/nsz
            // fast-math we don't emit, so reassociate by hand — two float4 lanes hide fmax latency.
            // Bit-exact on finite input (max is order-free); weights/activations are always finite.
            let f4 = base / 4l
            var m0 = abs(p4[f4])
            var m1 = abs(p4[f4 + 1l])
            for (k in range64(1l, 4l)) {
                m0 = max(m0, abs(p4[f4 + 2l * k]))
                m1 = max(m1, abs(p4[f4 + 2l * k + 1l]))
            }
            let m = max(m0, m1)
            let amax = max(max(m.x, m.y), max(m.z, m.w))
            let d = amax / 127.0
            let id = d != 0.0 ? 1.0 / d : 0.0
            sdst[soff + b] = d
            for [tune = 1] (i in range64(QK8)) {
                qdst[qoff + base + i] = int8(round(src[base + i] * id))
            }
        }
    }
}

//! Pointer core of quantize_q8_0_into — the batched prefill requant threads over positions in fork
//! contexts, which can only touch raw addresses; destinations are `var int8?`/`var float?`, written
//! directly. Bit-identical to the array overload. fallback "plain" ships unhinted; a box profile can vectorize.
[hint(noalias = src, noalias = qdst, noalias = sdst), tuned(fallback = "plain")]
def quantize_q8_0_into_ptr(src : float const?; n : int64; var qdst : int8?; var sdst : float?; qoff, soff : int64) {}

//! quantize_q8_0_into + per-16 block sums: bdst[boff + i/16] = sum of the 16 int8 quants — the
//! K-quant dot's offset term is o*(sum of activation quants) per weight sub-block, emitted in one pass.
//! Quants/scales are bit-identical to quantize_q8_0_into. n must be a multiple of 32.
def quantize_q8_0_bs_into(src : array<float> | #; n : int64; var qdst : array<int8>; var sdst : array<float>; var bdst : array<int>; qoff, soff, boff : int64) {
    assert(n % QK8 == 0l, "quantize_q8_0_bs_into requires n to be a multiple of 32")
    quantize_q8_0_bs_into_ptr(unsafe(addr(src[0])), n, unsafe(addr(qdst[0])), unsafe(addr(sdst[0])), unsafe(addr(bdst[0])), qoff, soff, boff)
}

[hint(noalias = src, noalias = qdst, noalias = sdst, noalias = bdst)]
def template quantize_q8_0_bs_into_ptr_template(src : float const ?; n : int64; var qdst : int8 ?; var sdst : float ?; var bdst : int ?; qoff, soff, boff : int64) : void {
    let nblocks = n / QK8
    unsafe {
        let p4 = reinterpret<float4 const?>(src)
        for (b in range64(nblocks)) {
            let base = b * QK8
            // amax reduction: same hand-vectorized two-lane shape as quantize_q8_0_into_ptr_template
            let f4 = base / 4l
            var m0 = abs(p4[f4])
            var m1 = abs(p4[f4 + 1l])
            for (k in range64(1l, 4l)) {
                m0 = max(m0, abs(p4[f4 + 2l * k]))
                m1 = max(m1, abs(p4[f4 + 2l * k + 1l]))
            }
            let m = max(m0, m1)
            let amax = max(max(m.x, m.y), max(m.z, m.w))
            let d = amax / 127.0
            let id = d != 0.0 ? 1.0 / d : 0.0
            sdst[soff + b] = d
            for [tune = 1] (i in range64(QK8)) {
                qdst[qoff + base + i] = int8(round(src[base + i] * id))
            }
            // per-16 sums of the just-stored quants (L1-hot); plain loop — 16 int8 adds is not
            // the kernel's cost, the fp32 read above is
            for (h in range64(2l)) {
                var bs = 0
                let hb = qoff + base + h * 16l
                for (i in range64(16l)) {
                    bs += int(qdst[hb + i])
                }
                bdst[boff + b * 2l + h] = bs
            }
        }
    }
}

//! Pointer core of quantize_q8_0_bs_into (see there); fork-context callers thread raw addresses.
[hint(noalias = src, noalias = qdst, noalias = sdst, noalias = bdst), tuned(fallback = "plain")]
def quantize_q8_0_bs_into_ptr(src : float const?; n : int64; var qdst : int8?; var sdst : float?; var bdst : int?; qoff, soff, boff : int64) {}

//! Q8_K-form activation quantization (kq v2 — llama.cpp K-quant dot contract): ONE scale per
//! 256-element superblock (sdst) + per-16 quant sums (bdst, 16/superblock) — lets K-quant kernels
//! fold sub-scale/bsums-x-mins per superblock instead of per 32-block. Separate plane from Q8_0.
def quantize_q8_k_into(src : array<float> | #; n : int64; var qdst : array<int8>; var sdst : array<float>; var bdst : array<int>; qoff, soff, boff : int64) {
    assert(n % 256l == 0l, "quantize_q8_k_into requires n to be a multiple of 256")
    quantize_q8_k_into_ptr(unsafe(addr(src[0])), n, unsafe(addr(qdst[0])), unsafe(addr(sdst[0])), unsafe(addr(bdst[0])), qoff, soff, boff)
}

//! Pointer core of quantize_q8_k_into (see there); fork-context callers thread raw addresses.
[hint(noalias = src, noalias = qdst, noalias = sdst, noalias = bdst)]
def quantize_q8_k_into_ptr(src : float const?; n : int64; var qdst : int8?; var sdst : float?; var bdst : int?; qoff, soff, boff : int64) {
    let nsb = n / 256l
    unsafe {
        let p4 = reinterpret<float4 const?>(src)
        for (s in range64(nsb)) {
            let base = s * 256l
            // amax reduction: the same hand-vectorized two-lane shape as the Q8_0 quantizers
            let f4 = base / 4l
            var m0 = abs(p4[f4])
            var m1 = abs(p4[f4 + 1l])
            for (k in range64(1l, 32l)) {
                m0 = max(m0, abs(p4[f4 + 2l * k]))
                m1 = max(m1, abs(p4[f4 + 2l * k + 1l]))
            }
            let m = max(m0, m1)
            let amax = max(max(m.x, m.y), max(m.z, m.w))
            let d = amax / 127.0
            let id = d != 0.0 ? 1.0 / d : 0.0
            sdst[soff + s] = d
            for (i in range64(256l)) {
                qdst[qoff + base + i] = int8(round(src[base + i] * id))
            }
            for (h in range64(16l)) {
                var bs = 0
                let hb = qoff + base + h * 16l
                for (i in range64(16l)) {
                    bs += int(qdst[hb + i])
                }
                bdst[boff + s * 16l + h] = bs
            }
        }
    }
}

//! Dequantize Q8_0 back to fp32 in place: dst[i] = q[i] * scales[i / 32]. The exact
//! reference the dequant-in-dot kernel must reproduce, and the input to the fp32 forward
//! when measuring end-to-end quantization loss. dst must be pre-sized to qt.n.
def dequantize_q8_0(qt : Q8Tensor; var dst : array<float> | #) {
    let nblocks = (qt.n + QK8 - 1l) / QK8
    for (b in range64(nblocks)) {
        let base = b * QK8
        let blen = min(QK8, qt.n - base)
        let d = qt.scales[b]
        for (i in range64(blen)) {
            dst[base + i] = float(qt.q[base + i]) * d
        }
    }
}

//! A Q4_0-quantized tensor in GGML's exact block layout: 32 weights/block as 16 bytes split-half
//! packed nibbles (byte j: low nibble = element j, high nibble = element j+16) + one scale.
//! w[i] ~= (nibble - 8) * scales[block]; scale is fp32 (GGUF loader transcodes fp16 disk once).
struct Q4Tensor {
    q : array<uint8>
    scales : array<float>
    n : int64
}

//! Quantize a flat fp32 tensor to Q4_0, GGML convention. Per block: pick the signed element of
//! largest magnitude `smax`, set d = smax / -8 (so that element maps to nibble 0), then
//! nibble = clamp(round(w/d) + 8, 0, 15). The scale factors out so a later dot is d * sum(q[i]*x[i]).
def quantize_q4_0(src : array<float> | #; n : int64) : Q4Tensor {
    assert(n % QK4 == 0l, "Q4_0 requires n to be a multiple of 32")
    let half = QK4 / 2l  // 16 packed bytes per 32-weight block
    var qt : Q4Tensor
    qt.n = n
    qt.q |> resize(n / 2l)
    let nblocks = n / QK4
    qt.scales |> resize(nblocks)
    for (b in range64(nblocks)) {
        let base = b * QK4
        var amax = 0.0
        var smax = 0.0  // signed value achieving amax (GGML keeps the sign)
        for (i in range64(QK4)) {
            let v = src[base + i]
            if (abs(v) > amax) {
                amax = abs(v)
                smax = v
            }
        }
        let d = smax / -8.0
        let id = d != 0.0 ? 1.0 / d : 0.0  // all-zero block: scale 0, nibbles 8 (-> 0)
        qt.scales[b] = d
        let bbyte = b * half
        for (j in range64(half)) {
            let lo = clamp(int(src[base + j] * id + 8.5), 0, 15)
            let hi = clamp(int(src[base + half + j] * id + 8.5), 0, 15)
            qt.q[bbyte + j] = uint8(lo | (hi << 4))
        }
    }
    return <- qt
}

//! Quantize a flat fp32 tensor to Q4_0 (GGML split-half) directly into pre-allocated slices:
//! packed nibble bytes land at qdst[qoff_bytes..] (qoff_bytes = weight-element-offset / 2), scales
//! at sdst[soff..]. No allocation — the memory-honest loader's one-tensor-at-a-time path. n % 32 == 0.
def quantize_q4_0_into(src : array<float> | #; n : int64; var qdst : array<uint8>; var sdst : array<float>; qoff_bytes, soff : int64) {
    assert(n % QK4 == 0l, "quantize_q4_0_into requires n to be a multiple of 32")
    let half = QK4 / 2l
    let nblocks = n / QK4
    for (b in range64(nblocks)) {
        let base = b * QK4
        var amax = 0.0
        var smax = 0.0
        for (i in range64(QK4)) {
            let v = src[base + i]
            if (abs(v) > amax) {
                amax = abs(v)
                smax = v
            }
        }
        let d = smax / -8.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        sdst[soff + b] = d
        let bbyte = qoff_bytes + b * half
        for (j in range64(half)) {
            let lo = clamp(int(src[base + j] * id + 8.5), 0, 15)
            let hi = clamp(int(src[base + half + j] * id + 8.5), 0, 15)
            qdst[bbyte + j] = uint8(lo | (hi << 4))
        }
    }
}

//! Dequantize Q4_0 back to fp32 in place: low nibbles -> elements 0..15, high nibbles -> 16..31,
//! dst[i] = (nibble - 8) * scales[block]. The exact reference the dequant-in-dot kernel must
//! reproduce, and the input to the fp32 forward when measuring quant loss. dst pre-sized to qt.n.
def dequantize_q4_0(qt : Q4Tensor; var dst : array<float> | #) {
    let half = QK4 / 2l
    let nblocks = qt.n / QK4
    for (b in range64(nblocks)) {
        let base = b * QK4
        let d = qt.scales[b]
        let bbyte = b * half
        for (j in range64(half)) {
            let raw = int(qt.q[bbyte + j])
            let lo = (raw & int(0x0F)) - 8
            let hi = ((raw >> 4) & int(0x0F)) - 8
            dst[base + j] = float(lo) * d
            dst[base + half + j] = float(hi) * d
        }
    }
}

//! Quantization-error report between an original tensor and its dequantized approximation.
//! snr_db (signal-to-noise ratio) is the headline number: higher = less loss. Q8 typically
//! lands ~45-50 dB, Q4 ~25-30 dB.
struct QuantStats {
    n : int64
    mean_abs_err : double
    max_abs_err : double
    rms_signal : double
    rms_error : double
    snr_db : double
}

//! Reconstruction error of `approx` vs `orig` over n elements. Accumulates in double so
//! the sums stay accurate across millions of weights.
def quant_error_stats(orig : array<float> | #; approx : array<float> | #; n : int64) : QuantStats {
    var st : QuantStats
    st.n = n
    var sum_abs = 0.0lf
    var max_abs = 0.0lf
    var sum_sig2 = 0.0lf
    var sum_err2 = 0.0lf
    for (i in range64(n)) {
        let s = double(orig[i])
        let e = abs(double(approx[i]) - s)
        sum_abs += e
        max_abs = max(max_abs, e)
        sum_err2 += e * e
        sum_sig2 += s * s
    }
    st.mean_abs_err = sum_abs / double(n)
    st.max_abs_err = max_abs
    st.rms_signal = sqrt(sum_sig2 / double(n))
    st.rms_error = sqrt(sum_err2 / double(n))
    st.snr_db = st.rms_error > 0.0lf ? 20.0lf * log10(st.rms_signal / st.rms_error) : 0.0lf
    return st
}
