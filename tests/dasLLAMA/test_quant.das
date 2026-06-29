options gen2

require dastest/testing_boost public
require dasllama/dasllama_quant
require math

// Q8_0 round-trip properties. fp scaling isn't bit-exact, so we assert bounds, not values.

[test]
def test_quant(t : T?) {
    // A constant block recovers (near-)exactly: amax=|c|, every q hits +-127, dq == c.
    t |> run("constant block round-trips") @(t : T?) {
        var src : array<float>
        src |> resize(32)
        for (i in range(32)) {
            src[i] = 3.0
        }
        let qt <- quantize_q8_0(src, 32l)
        var deq : array<float>
        deq |> resize(32)
        dequantize_q8_0(qt, deq)
        for (i in range(32)) {
            t |> success(abs(deq[i] - 3.0) < 1.0e-4, "constant must round-trip")
        }
    }
    // An all-zero block stays zero (the d==0 guard must not divide by zero).
    t |> run("zero block stays zero") @(t : T?) {
        var src : array<float>
        src |> resize(32)
        let qt <- quantize_q8_0(src, 32l)
        t |> equal(qt.scales[0], 0.0)
        var deq : array<float>
        deq |> resize(32)
        dequantize_q8_0(qt, deq)
        for (i in range(32)) {
            t |> equal(deq[i], 0.0)
        }
    }
    // With amax=127 the scale is exactly 1, so per-element error is |round(w)-w| <= 0.5,
    // and the max-magnitude element comes back exactly (it sets the scale).
    t |> run("error bounded by half a step") @(t : T?) {
        let src <- [127.0, 0.4, -0.4, 1.6, -1.6, 2.5, -2.5, 10.3,
                    -10.3, 50.7, -50.7, 3.0, -3.0, 99.9, -99.9, 0.1,
                    -0.1, 7.7, -7.7, 64.0, -64.0, 0.49, -0.49, 31.5,
                    -31.5, 12.0, -12.0, 88.0, -88.0, 5.5, -5.5, 2.0]
        let qt <- quantize_q8_0(src, 32l)
        var deq : array<float>
        deq |> resize(32)
        dequantize_q8_0(qt, deq)
        let d = qt.scales[0]
        t |> success(abs(d - 1.0) < 1.0e-4, "amax=127 => scale 1")
        t |> success(abs(deq[0] - 127.0) < 1.0e-3, "max element recovers exactly")
        for (i in range(32)) {
            t |> success(abs(deq[i] - src[i]) <= 0.5 + 1.0e-4, "error within half a step")
        }
    }
    // quant_error_stats over a multi-block ramp: bounded max error, healthy SNR.
    t |> run("error stats on a ramp") @(t : T?) {
        let n = 96l
        var src : array<float>
        src |> resize(n)
        for (i in range64(n)) {
            src[i] = float(i) * 0.25 - 12.0  // -12 .. +11.75 across 3 blocks
        }
        let qt <- quantize_q8_0(src, n)
        var deq : array<float>
        deq |> resize(n)
        dequantize_q8_0(qt, deq)
        let st = quant_error_stats(src, deq, n)
        t |> success(st.max_abs_err < 0.2lf, "ramp max error stays small")
        t |> success(st.snr_db > 30.0lf, "Q8 SNR should clear 30 dB")
    }
    // A short final block (n not a multiple of 32) is quantized too.
    t |> run("partial final block") @(t : T?) {
        let n = 40l  // one full block + a 8-wide tail
        var src : array<float>
        src |> resize(n)
        for (i in range64(n)) {
            src[i] = float(i) + 1.0
        }
        let qt <- quantize_q8_0(src, n)
        t |> equal(int64(length(qt.scales)), 2l)
        var deq : array<float>
        deq |> resize(n)
        dequantize_q8_0(qt, deq)
        let st = quant_error_stats(src, deq, n)
        t |> success(st.snr_db > 30.0lf, "tail block still quantizes well")
    }
}

// Q4_0 round-trip properties (GGML layout: d = smax/-8, offset-8, split-half packing). 4 bits, so
// the step is coarser than Q8: errors are bounded by half a step, and SNR lands ~20 dB lower.

[test]
def test_quant_q4(t : T?) {
    // A constant block recovers exactly: smax=c, d=c/-8, every element maps to nibble 0, dq == c.
    t |> run("constant block round-trips") @(t : T?) {
        var src : array<float>
        src |> resize(32)
        for (i in range(32)) {
            src[i] = 3.0
        }
        let qt <- quantize_q4_0(src, 32l)
        t |> equal(int64(length(qt.q)), 16l)  // 32 nibbles packed two per byte
        var deq : array<float>
        deq |> resize(32)
        dequantize_q4_0(qt, deq)
        for (i in range(32)) {
            t |> success(abs(deq[i] - 3.0) < 1.0e-4, "constant must round-trip")
        }
    }
    // An all-zero block stays zero (the d==0 guard must not divide by zero; nibbles are 8 -> 0).
    t |> run("zero block stays zero") @(t : T?) {
        var src : array<float>
        src |> resize(32)
        let qt <- quantize_q4_0(src, 32l)
        t |> equal(qt.scales[0], 0.0)
        var deq : array<float>
        deq |> resize(32)
        dequantize_q4_0(qt, deq)
        for (i in range(32)) {
            t |> equal(deq[i], 0.0)
        }
    }
    // With the max-magnitude element = 8 the scale magnitude is exactly 1, so per-element error is
    // <= 0.5, and that element comes back exactly (it sets the scale, mapping to nibble 0).
    t |> run("error bounded by half a step") @(t : T?) {
        let src <- [8.0, 0.4, -0.4, 1.6, -1.6, 2.5, -2.5, 6.3,
                    -6.3, 5.7, -5.7, 3.0, -3.0, 6.9, -6.9, 0.1,
                    -0.1, 4.7, -4.7, 4.0, -4.0, 0.49, -0.49, 3.5,
                    -3.5, 2.0, -2.0, 5.0, -5.0, 5.5, -5.5, 1.0]
        let qt <- quantize_q4_0(src, 32l)
        var deq : array<float>
        deq |> resize(32)
        dequantize_q4_0(qt, deq)
        let d = qt.scales[0]
        t |> success(abs(abs(d) - 1.0) < 1.0e-4, "max-magnitude 8 => |scale| 1")
        t |> success(abs(deq[0] - 8.0) < 1.0e-3, "max element recovers exactly")
        for (i in range(32)) {
            t |> success(abs(deq[i] - src[i]) <= 0.5 + 1.0e-4, "error within half a step")
        }
    }
    // quant_error_stats over a multi-block ramp: bounded max error, SNR clears 20 dB (4-bit).
    t |> run("error stats on a ramp") @(t : T?) {
        let n = 96l
        var src : array<float>
        src |> resize(n)
        for (i in range64(n)) {
            src[i] = float(i) * 0.25 - 12.0  // -12 .. +11.75 across 3 blocks
        }
        let qt <- quantize_q4_0(src, n)
        var deq : array<float>
        deq |> resize(n)
        dequantize_q4_0(qt, deq)
        let st = quant_error_stats(src, deq, n)
        t |> success(st.snr_db > 20.0lf, "Q4 SNR should clear 20 dB")
    }
}

// The *_into variants quantize directly into a pre-allocated, offset slice — the path the
// memory-honest loader uses. They must produce byte-identical quants/scales to the alloc versions
// (writing at the given offsets), so a loaded model is the same whether built tensor-at-a-time or not.

[test]
def test_quant_into(t : T?) {
    // Q8: quantize_q8_0_into at a non-zero offset must match quantize_q8_0 element-for-element.
    t |> run("quantize_q8_0_into matches alloc version at offset") @(t : T?) {
        let n = 96l  // 3 blocks
        var src : array<float>
        src |> resize(n)
        for (i in range64(n)) {
            src[i] = float(i) * 0.37 - 15.0  // arbitrary multi-block signal
        }
        let qoff = 64l  // 2-block lead-in, exercises the offset arithmetic
        let soff = qoff / 32l
        let ref <- quantize_q8_0(src, n)
        var q : array<int8>
        q |> resize(qoff + n)
        var s : array<float>
        s |> resize(soff + n / 32l)
        quantize_q8_0_into(src, n, q, s, qoff, soff)
        for (i in range64(n)) {
            t |> equal(int(q[qoff + i]), int(ref.q[i]))
        }
        for (b in range64(n / 32l)) {
            t |> equal(s[soff + b], ref.scales[b])
        }
    }
    // Q4: same, with byte-offset packing (qoff_bytes = element-offset / 2).
    t |> run("quantize_q4_0_into matches alloc version at offset") @(t : T?) {
        let n = 96l
        var src : array<float>
        src |> resize(n)
        for (i in range64(n)) {
            src[i] = float(i) * 0.37 - 15.0
        }
        let qoff = 64l
        let qoff_bytes = qoff / 2l
        let soff = qoff / 32l
        let ref <- quantize_q4_0(src, n)
        var q : array<uint8>
        q |> resize(qoff_bytes + n / 2l)
        var s : array<float>
        s |> resize(soff + n / 32l)
        quantize_q4_0_into(src, n, q, s, qoff_bytes, soff)
        for (i in range64(n / 2l)) {
            t |> equal(int(q[qoff_bytes + i]), int(ref.q[i]))
        }
        for (b in range64(n / 32l)) {
            t |> equal(s[soff + b], ref.scales[b])
        }
    }
}
