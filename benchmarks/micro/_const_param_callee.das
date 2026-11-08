options gen2
module _const_param_callee shared public

//! the const-parameter callees of const_param_readonly.das, in a
//! module of their own so a split build calls them across a
//! partition boundary

struct Stats {
    xs : array<float>
    scale : float
}

[never_inline, hint(noinline)]
def dot_cross(a, b : array<float>) : float {
    var acc = 0.0f
    for (i in range(length(a))) {   // nolint:PERF029 - a zip would lock both arrays
        acc += a[i] * b[i]
    }
    return acc
}

[never_inline, hint(noinline)]
def sum_for_cross(a : array<int>) : int {
    var acc = 0
    for (x in a) {
        acc += x
    }
    return acc
}

[never_inline, hint(noinline)]
def total_cross(s : Stats) : float {
    var acc = 0.0f
    for (x in s.xs) {
        acc += x * s.scale
    }
    return acc
}
