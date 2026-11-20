options gen2
options persistent_heap
options bound_check_elision

require dastest/testing_boost
require math

let M = 50000

var g_sink : int64

def private make_ints(n : int) {
    var res <- [for (i in range(n)); i]
    return <- res
}

def private str_len_loop(a : array<int>; s : string) : int {
    var t = 0
    for (x in a) {
        t += x + length(s)
    }
    return t
}

def private indexed_sum(a : array<int>) : int {
    var s = 0
    for (i in range(length(a))) {   // nolint:PERF018
        s += a[i]
    }
    return s
}

def private indexed_axpy(var y : array<float>; x : array<float>; k : float) {
    let n = min(length(x), length(y))
    for (i in range(n)) {
        y[i] = y[i] + k * x[i]
    }
}

def private fixed_dot(a, b : float[16]) : float {
    var s = 0.
    for (i in range(16)) {
        s += a[i] * b[i]
    }
    return s
}

[benchmark]
def attributes(b : B?) {
    let ia <- make_ints(M)
    let s = "a string of some length"
    b |> run("pure_call_hoist/{M}", 1) {
        g_sink += int64(str_len_loop(ia, s))
    }
    b |> run("indexed_sum/{M}", 1) {
        g_sink += int64(indexed_sum(ia))
    }
    let fx <- [for (i in range(M)); float(i)]
    var fy <- [for (i in range(M)); float(i) * 0.5]
    b |> run("indexed_axpy/{M}", 1) {
        indexed_axpy(fy, fx, 0.25)
        g_sink += int64(fy[M - 1])
    }
    var fa, fb : float[16]
    for (i in range(16)) {
        fa[i] = float(i)
        fb[i] = float(16 - i)
    }
    b |> run("fixed_dot/{M}", M) {
        var t = 0.
        for (_ in range(M)) {
            t += fixed_dot(fa, fb)
        }
        g_sink += int64(t)
    }
}
