options gen2
options persistent_heap

// Benchmark: index access — count trues via a[i] in a 1M-element array of alternating
// true/false. Compares BoolArray (bit-packed) vs array<bool> random access performance.
// Ported from examples/profile/bool_array_test/test02.das.

require dastest/testing_boost

require daslib/bool_array

let TOTAL_ELEMENTS = 1000000

[sideeffects]
def count_trues(var a : auto(ArrayType)) {
    var count = 0
    for (i in 0 .. TOTAL_ELEMENTS) {
        if (a[i]) {
            count ++
        }
    }
    return count
}

[benchmark]
def bool_array_access(b : B?) {
    var inscope a : BoolArray
    a.reserve(TOTAL_ELEMENTS)
    for (i in 0 .. TOTAL_ELEMENTS) {
        a.push((i & 1) == 0)
    }
    b |> run("access/{TOTAL_ELEMENTS}", TOTAL_ELEMENTS) <| $() {
        let t = count_trues(a)
        if (t != TOTAL_ELEMENTS / 2) {
            b->failNow()
        }
    }
}

[benchmark]
def array_bool_access(b : B?) {
    var inscope a : array<bool>
    a.reserve(TOTAL_ELEMENTS)
    for (i in 0 .. TOTAL_ELEMENTS) {
        a.push((i & 1) == 0)
    }
    b |> run("access/{TOTAL_ELEMENTS}", TOTAL_ELEMENTS) <| $() {
        let t = count_trues(a)
        if (t != TOTAL_ELEMENTS / 2) {
            b->failNow()
        }
    }
}
