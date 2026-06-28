options gen2
options persistent_heap

require dastest/testing_boost
require dasllama_math

// Deterministic fill — values don't matter, only that they're non-trivial.
def fill(var a : array<float>; count : int) {
    a |> resize(count)
    for (i in range(count)) {
        a[i] = float(i % 7) * 0.1
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
        b |> run("proj 288x288") {
            matmul(y, w, x, 288, 288)
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
        b |> run("classifier 32000x288") {
            matmul(y, w, x, 288, 32000)
        }
    }
}
