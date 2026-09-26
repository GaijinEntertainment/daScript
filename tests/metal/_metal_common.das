options gen2
options indenting = 4

// Shared GPU-side helpers for the tests/metal behavioral suite. The buffer helpers are generic
// (untyped `dev`/`buf`) on purpose: their bodies reference das_metal externs, and generics only
// instantiate at call sites — which all sit inside static_if(builtin_module_exists(das_metal))
// blocks — so this module compiles on every platform.

module _metal_common shared public

require math
require ?das_metal metal/das_metal_boost public

//! count exact mismatches between buffer contents and the oracle array (int/uint scalars + vectors)
def public buf_mismatch_exact(buf; want : array<auto(TT)>) : int {
    var p = unsafe(reinterpret<TT -const?>(metal_buffer_contents(buf)))
    var bad = 0
    for (k in range(length(want))) {
        if (unsafe(p[k]) != want[k]) {
            bad++
        }
    }
    return bad
}

//! count approx mismatches for a float buffer (fastmath tolerance)
def public buf_mismatch_approx(buf; want : array<float>) : int {
    var p = unsafe(reinterpret<float?>(metal_buffer_contents(buf)))
    var bad = 0
    for (k in range(length(want))) {
        if (!fapprox(unsafe(p[k]), want[k])) {
            bad++
        }
    }
    return bad
}

//! count approx mismatches for a float4 buffer (per component)
def public buf_mismatch_approx4(buf; want : array<float4>) : int {
    var p = unsafe(reinterpret<float4?>(metal_buffer_contents(buf)))
    var bad = 0
    for (k in range(length(want))) {
        let v = unsafe(p[k])
        let e = want[k]
        if (!(fapprox(v.x, e.x) && fapprox(v.y, e.y) && fapprox(v.z, e.z) && fapprox(v.w, e.w))) {
            bad++
        }
    }
    return bad
}
