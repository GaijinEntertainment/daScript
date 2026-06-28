options gen2
options persistent_heap

require dastest/testing_boost
require matmul_variants
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
    }
}
