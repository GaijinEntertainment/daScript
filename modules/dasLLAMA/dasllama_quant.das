options gen2

module dasllama_quant shared public

require math

// dasLLAMA quantization — curriculum step 6. Batch-1 inference is bandwidth-bound
// (time ~= bytes_read / bandwidth), so the lever is reading FEWER bytes per weight.
// Q8_0 stores 32 weights as int8 + one fp32 scale: 36 bytes vs 128 (~3.55x smaller).
// This file is the FORMAT + a quant-quality detector; the fast dequant-in-dot kernel
// comes after we can measure the loss.

let QK8 = 32l  // Q8_0 block size: 32 weights share one scale

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
