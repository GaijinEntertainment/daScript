options gen2
options persistent_heap

// Benchmark: push 1M alternating true/false values into BoolArray vs array<bool>.
// Tests bulk append performance of the bit-packed BoolArray.
// Ported from examples/profile/bool_array_test/test01.das.

require dastest/testing_boost

require daslib/bool_array

let TOTAL_ELEMENTS = 1000000

[sideeffects]
def fill_array(dummy : auto(ArrayType)) {
    var inscope a : ArrayType
    a.reserve(TOTAL_ELEMENTS)
    for (i in 0 .. TOTAL_ELEMENTS) {
        a.push((i & 1) == 0)
    }
    return a.length()
}

def run_bench(b : B?; dummy : auto(ArrayType); name : string) {
    b |> run("{name}/{TOTAL_ELEMENTS}", TOTAL_ELEMENTS) <| $() {
        fill_array(dummy)
    }
}

[benchmark]
def bool_array_push(b : B?) {
    run_bench(b, default<BoolArray>, "push")
}

[benchmark]
def array_bool_push(b : B?) {
    run_bench(b, default<array<bool>>, "push")
}
