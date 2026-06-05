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
}
