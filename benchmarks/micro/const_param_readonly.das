options gen2
options persistent_heap

require dastest/testing_boost
require _const_param_callee

//! What the `readonly` parameter attribute buys at a call site: a
//! caller keeps a const array or struct live across repeated calls to
//! a callee that takes it by const reference and is not inlined.
//! `local` callees sit in this module, `cross` callees in
//! `_const_param_callee`, which a split build puts in another
//! partition, where LLVM reads the attributes off the declaration.

let CALLS = 4096
let N = 64

var g_sink_f = 0.0f
var g_sink_i = 0

[never_inline, hint(noinline)]
def dot_local(a, b : array<float>) : float {
    var acc = 0.0f
    for (i in range(length(a))) {   // nolint:PERF029 - a zip would lock both arrays
        acc += a[i] * b[i]
    }
    return acc
}

[never_inline, hint(noinline)]
def sum_for_local(a : array<int>) : int {
    var acc = 0
    for (x in a) {
        acc += x
    }
    return acc
}

[never_inline, hint(noinline)]
def total_local(s : Stats) : float {
    var acc = 0.0f
    for (x in s.xs) {
        acc += x * s.scale
    }
    return acc
}

[benchmark]
def const_param_calls(b : B?) {
    var fa <- [for (i in range(N)); float(i) * 0.5f]
    let fb <- [for (i in range(N)); float(N - i)]
    let ia <- [for (i in range(N)); i]
    var st = Stats(scale = 0.25f)
    st.xs := fa
    b |> run("dot/local", CALLS) {
        var acc = 0.0f
        for (_ in range(CALLS)) {
            acc += dot_local(fa, fb) + fa[0] + float(length(fb))
        }
        g_sink_f = acc
    }
    b |> run("dot/cross", CALLS) {
        var acc = 0.0f
        for (_ in range(CALLS)) {
            acc += dot_cross(fa, fb) + fa[0] + float(length(fb))
        }
        g_sink_f = acc
    }
    b |> run("sum_for/local", CALLS) {
        var acc = 0
        for (_ in range(CALLS)) {
            acc += sum_for_local(ia) + ia[0] + length(ia)
        }
        g_sink_i = acc
    }
    b |> run("sum_for/cross", CALLS) {
        var acc = 0
        for (_ in range(CALLS)) {
            acc += sum_for_cross(ia) + ia[0] + length(ia)
        }
        g_sink_i = acc
    }
    b |> run("total/local", CALLS) {
        var acc = 0.0f
        for (_ in range(CALLS)) {
            acc += total_local(st) + st.scale + float(length(st.xs))
        }
        g_sink_f = acc
    }
    b |> run("total/cross", CALLS) {
        var acc = 0.0f
        for (_ in range(CALLS)) {
            acc += total_cross(st) + st.scale + float(length(st.xs))
        }
        g_sink_f = acc
    }
}
