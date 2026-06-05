options gen2
options indenting = 4
options no_unused_function_arguments = false

require dastest/testing_boost public
require flat_api_macro
require math

//! Cross-module integration test for the PUBLIC `flatten_function` entry.
//!
//! `flat_api_macro` (a separate module, the real backend topology) calls
//! `flatten_function` directly with its own whitelist {max, min}. This proves
//! `flatten_function` is callable from another module's macro and honors a
//! caller-supplied primitive set. The flattened twin must match an unflattened
//! reference over the input grid.

// Unflattened reference (identical body) for differential comparison.
def ref_max_min(x : int) : int {
    return max(x, 0) + min(x, 10)
}

[flat_api]
def api_demo(x : int) : int {
    return max(x, 0) + min(x, 10)
}

// A recursive user function the backend whitelists as a leaf primitive. flatten
// must KEEP the call (whitelist precedence), not inline it — inlining a recursive
// function would trip the recursion boundary and fail the compile. That this
// compiles + matches the reference proves whitelisted calls survive un-inlined
// even though they have a (here recursive) body — the [hint]/[stub] shader case.
def myrec(n : int) : int {
    return n <= 0 ? 0 : n + myrec(n - 1)
}

def ref_rec(x : int) : int {
    return myrec(x) + 1
}

[flat_api]
def api_rec(x : int) : int {
    return myrec(x) + 1
}

var GRID : array<int>

[init]
def fill_grid {
    GRID <- [-5, -1, 0, 1, 5, 10, 20]
}

[test]
def test_flatten_api(t : T?) {
    t |> run("flatten_function callable cross-module with custom whitelist") @(t : T?) {
        for (v in GRID) {
            t |> equal(api_demo_apiflat(v), ref_max_min(v))
        }
    }
    t |> run("whitelisted recursive user fn is kept, not inlined") @(t : T?) {
        for (v in GRID) {
            t |> equal(api_rec_apiflat(v), ref_rec(v))
        }
    }
}
