options gen2
options persistent_heap

// Benchmark: erase at index 0 — resize to 10K elements then erase all from front.
// Tests BoolArray's bit-shift erase vs array<bool>'s memmove erase.
// Ported from examples/profile/bool_array_test/test06.das.

require dastest/testing_boost

require daslib/bool_array

let TOTAL_ELEMENTS = 10000

[sideeffects]
def erase_from_front(dummy : auto(ArrayType)) {
    var inscope a : ArrayType
    a.resize(TOTAL_ELEMENTS)
    for (i in 0 .. TOTAL_ELEMENTS) {
        a.erase(0)
    }
    return a.length()
}

def run_bench(b : B?; dummy : auto(ArrayType); name : string) {
    b |> run("{name}/{TOTAL_ELEMENTS}", TOTAL_ELEMENTS) <| $() {
        erase_from_front(dummy)
    }
}

[benchmark]
def bool_array_erase(b : B?) {
    run_bench(b, default<BoolArray>, "erase_at_0")
}

[benchmark]
def array_bool_erase(b : B?) {
    run_bench(b, default<array<bool>>, "erase_at_0")
}
