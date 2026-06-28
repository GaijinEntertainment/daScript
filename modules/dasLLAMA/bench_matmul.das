options gen2
options persistent_heap

require dastest/testing_boost
require matmul_variants
require dasllama_quant
require math

// Deterministic fill — values don't matter, only that they're non-trivial.
def fill(var a : array<float>; count : int) {
    a |> resize(count)
    for (i in range(count)) {
        a[i] = float(i % 7) * 0.1
    }
}

// Correctness gate: a variant must match the production kernel before its timing means anything.
// (float4 reorders the summation, so results differ in the low bits — compare with a tolerance.)
def check_same(b : B?; w, x : array<float>; n, d : int) {
    var yn : array<float>
    var yf : array<float>
    var yv : array<float>
    yn |> resize(d)
    yf |> resize(d)
    yv |> resize(d)
    matmul_naive(yn, w, x, int64(n), int64(d))
    matmul_f4(yf, w, x, int64(n), int64(d))
    matmul_llvm_vec(yv, w, x, int64(n), int64(d))
    for (i in range(d)) {
        if (abs(yn[i] - yf[i]) > 1.0e-2 || abs(yn[i] - yv[i]) > 1.0e-2) {
            b->failNow()
        }
    }
}

[benchmark]
def bench_matmul(b : B?) {
    // stories15M attention/FFN projection — W is 288 x 288.
    {
        var w : array<float>
        var x : array<float>
        var y : array<float>
        fill(w, 288 * 288)
        fill(x, 288)
        y |> resize(288)
        check_same(b, w, x, 288, 288)
        b |> run("naive proj 288x288") {
            matmul_naive(y, w, x, 288l, 288l)
        }
        b |> run("f4    proj 288x288") {
            matmul_f4(y, w, x, 288l, 288l)
        }
        b |> run("llvm  proj 288x288") {
            matmul_llvm_vec(y, w, x, 288l, 288l)
        }
    }
    // stories15M classifier head — W is 32000 x 288 (the heaviest single matmul).
    {
        var w : array<float>
        var x : array<float>
        var y : array<float>
        fill(w, 32000 * 288)
        fill(x, 288)
        y |> resize(32000)
        check_same(b, w, x, 288, 32000)
        b |> run("naive classifier 32000x288") {
            matmul_naive(y, w, x, 288l, 32000l)
        }
        b |> run("f4    classifier 32000x288") {
            matmul_f4(y, w, x, 288l, 32000l)
        }
        b |> run("llvm  classifier 32000x288") {
            matmul_llvm_vec(y, w, x, 288l, 32000l)
        }
        b |> run("w8u2  classifier 32000x288") {
            matmul_w8u2(y, w, x, 288l, 32000l)
        }
        b |> run("w8u4  classifier 32000x288") {
            matmul_w8u4(y, w, x, 288l, 32000l)
        }
        b |> run("w8u8  classifier 32000x288") {
            matmul_w8u8(y, w, x, 288l, 32000l)
        }
        b |> run("w16   classifier 32000x288") {
            matmul_w16(y, w, x, 288l, 32000l)
        }
        b |> run("w16u4 classifier 32000x288") {
            matmul_w16u4(y, w, x, 288l, 32000l)
        }
        // Q8_0: quantize the weights once, then bench the dequant-in-dot kernel.
        // Gate: matmul_q8 must match fp32 matmul on the DEQUANTIZED weights (quant is lossy).
        let qt <- quantize_q8_0(w, 288l * 32000l)
        var wdeq : array<float>
        wdeq |> resize(288 * 32000)
        dequantize_q8_0(qt, wdeq)
        check_q8(b, qt, wdeq, x, 288, 32000)
        b |> run("q8    classifier 32000x288") {
            matmul_q8(y, qt.q, qt.scales, x, 288l, 32000l)
        }
        b |> run("q8b   classifier 32000x288") {
            matmul_q8b(y, qt.q, qt.scales, x, 288l, 32000l)
        }
        b |> run("q8f4  classifier 32000x288") {
            matmul_q8_f4(y, qt.q, qt.scales, x, 288l, 32000l)
        }
        b |> run("q8flat classifier 32000x288") {
            matmul_q8_flat(y, qt.q, qt.scales, x, 288l, 32000l)
        }
        // Q8·Q8: quantize the activation x to Q8 too, then the int8·int8 SDOT kernel. The benched
        // closure re-quantizes x each call, so the timing includes the realistic one-time x-quant cost.
        var xq : array<int8>
        var xs : array<float>
        xq |> resize(288)
        xs |> resize(288 / 32)
        let xqt <- quantize_q8_0(x, 288l)
        var xdeq : array<float>
        xdeq |> resize(288)
        dequantize_q8_0(xqt, xdeq)
        quantize_q8_0_into(x, 288l, xq, xs, 0l, 0l)
        check_q8q8(b, qt, wdeq, xq, xs, xdeq, 288, 32000)
        b |> run("q8q8  classifier 32000x288") {
            quantize_q8_0_into(x, 288l, xq, xs, 0l, 0l)
            matmul_q8q8(y, qt.q, qt.scales, xq, xs, 288l, 32000l)
        }
        // Q4_0: 8x fewer weight bytes than fp32, but a costlier unpack (shift+mask per element).
        let qt4 <- quantize_q4_0(w, 288l * 32000l)
        var wdeq4 : array<float>
        wdeq4 |> resize(288 * 32000)
        dequantize_q4_0(qt4, wdeq4)
        check_q4(b, qt4, wdeq4, x, 288, 32000)
        b |> run("q4    classifier 32000x288") {
            matmul_q4(y, qt4.q, qt4.scales, x, 288l, 32000l)
        }
        b |> run("q4fv  classifier 32000x288") {
            matmul_q4_fv(y, qt4.q, qt4.scales, x, 288l, 32000l)
        }
        b |> run("q4fv-w4u4 classifier      ") {
            matmul_q4_fv_w4u4(y, qt4.q, qt4.scales, x, 288l, 32000l)
        }
        b |> run("q4fv-w16  classifier      ") {
            matmul_q4_fv_w16(y, qt4.q, qt4.scales, x, 288l, 32000l)
        }
        b |> run("q4flat classifier 32000x288") {
            matmul_q4_flat(y, qt4.q, qt4.scales, x, 288l, 32000l)
        }
    }
}

// Q4 correctness gate: dequant the weights, run the fp32 kernel on them, compare to each Q4 kernel.
// Algebraically identical (scale factors out of each block), so the gap is fp-order only.
def check_q4(b : B?; qt : Q4Tensor; wdeq, x : array<float>; n, d : int) {
    var yref : array<float>
    var ya : array<float>
    var yb : array<float>
    var yc : array<float>
    yref |> resize(d)
    ya |> resize(d)
    yb |> resize(d)
    yc |> resize(d)
    matmul_llvm_vec(yref, wdeq, x, int64(n), int64(d))
    matmul_q4(ya, qt.q, qt.scales, x, int64(n), int64(d))
    matmul_q4_fv(yb, qt.q, qt.scales, x, int64(n), int64(d))
    matmul_q4_flat(yc, qt.q, qt.scales, x, int64(n), int64(d))
    for (i in range(d)) {
        if (abs(yref[i] - ya[i]) > 1.0e-2 || abs(yref[i] - yb[i]) > 1.0e-2 || abs(yref[i] - yc[i]) > 1.0e-2) {
            b->failNow()
        }
    }
}

// Q8·Q8 correctness gate: the activation is quantized too, so the reference is the fp32 kernel on
// BOTH dequantized weights and the dequantized activation. matmul_q8q8 computes the same block sums
// (exact int dot * the two scales), so the gap is fp-order only.
def check_q8q8(b : B?; qt : Q8Tensor; wdeq : array<float>; xq : array<int8>; xs : array<float>; xdeq : array<float>; n, d : int) {
    var yref : array<float>
    var ya : array<float>
    yref |> resize(d)
    ya |> resize(d)
    matmul_llvm_vec(yref, wdeq, xdeq, int64(n), int64(d))
    matmul_q8q8(ya, qt.q, qt.scales, xq, xs, int64(n), int64(d))
    for (i in range(d)) {
        if (abs(yref[i] - ya[i]) > 1.0e-2) {
            b->failNow()
        }
    }
}

// Q8 correctness gate: dequant the weights, run the fp32 kernel on them, compare to matmul_q8.
// They are algebraically identical (scale factors out of each block), so the gap is fp-order only.
def check_q8(b : B?; qt : Q8Tensor; wdeq, x : array<float>; n, d : int) {
    var yref : array<float>
    var ya : array<float>
    var yb : array<float>
    var yc : array<float>
    var yd : array<float>
    yref |> resize(d)
    ya |> resize(d)
    yb |> resize(d)
    yc |> resize(d)
    yd |> resize(d)
    matmul_llvm_vec(yref, wdeq, x, int64(n), int64(d))
    matmul_q8(ya, qt.q, qt.scales, x, int64(n), int64(d))
    matmul_q8b(yb, qt.q, qt.scales, x, int64(n), int64(d))
    matmul_q8_f4(yc, qt.q, qt.scales, x, int64(n), int64(d))
    matmul_q8_flat(yd, qt.q, qt.scales, x, int64(n), int64(d))
    for (i in range(d)) {
        if (abs(yref[i] - ya[i]) > 1.0e-2 || abs(yref[i] - yb[i]) > 1.0e-2 || abs(yref[i] - yc[i]) > 1.0e-2 || abs(yref[i] - yd[i]) > 1.0e-2) {
            b->failNow()
        }
    }
}
