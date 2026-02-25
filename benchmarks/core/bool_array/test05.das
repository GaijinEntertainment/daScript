options gen2
options persistent_heap

// Benchmark: insert at index 0 — push 10K alternating true/false values at the front.
// Tests BoolArray's bit-shift insert vs array<bool>'s memmove insert.
// Ported from examples/profile/bool_array_test/test05.das.

require dastest/testing_boost

require daslib/bool_array

let TOTAL_ELEMENTS = 10000

[sideeffects]
def fill_insert_at_0(dummy : auto(ArrayType)) {
    var inscope a : ArrayType
    a.reserve(TOTAL_ELEMENTS)
    for (i in 0 .. TOTAL_ELEMENTS) {
        a.push((i & 1) == 0, 0)
    }
    return a.length()
}

def run_bench(b : B?; dummy : auto(ArrayType); name : string) {
    b |> run("{name}/{TOTAL_ELEMENTS}", TOTAL_ELEMENTS) <| $() {
        fill_insert_at_0(dummy)
    }
}

[benchmark]
def bool_array_insert(b : B?) {
    run_bench(b, default<BoolArray>, "insert_at_0")
}

[benchmark]
def array_bool_insert(b : B?) {
    run_bench(b, default<array<bool>>, "insert_at_0")
}
