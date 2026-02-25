options gen2
options persistent_heap

// Benchmark: iteration access — count trues via `for (e in a)` in a 1M-element array
// of alternating true/false. Compares BoolArray iterator vs array<bool> iterator.
// Ported from examples/profile/bool_array_test/test03.das.

require dastest/testing_boost

require daslib/bool_array

let TOTAL_ELEMENTS = 1000000

[sideeffects]
def count_trues_iter_bool(var a : array<bool>) {
    var count = 0
    for (e in a) {
        if (e) {
            count ++
        }
    }
    return count
}

[sideeffects]
def count_trues_iter_ba(var a : BoolArray) {
    var count = 0
    for (e in a) {
        if (e) {
            count ++
        }
    }
    return count
}

[benchmark]
def bool_array_iterate(b : B?) {
    var inscope a : BoolArray
    a.reserve(TOTAL_ELEMENTS)
    for (i in 0 .. TOTAL_ELEMENTS) {
        a.push((i & 1) == 0)
    }
    b |> run("iterate/{TOTAL_ELEMENTS}", TOTAL_ELEMENTS) <| $() {
        let t = count_trues_iter_ba(a)
        if (t != TOTAL_ELEMENTS / 2) {
            b->failNow()
        }
    }
}

[benchmark]
def array_bool_iterate(b : B?) {
    var inscope a : array<bool>
    a.reserve(TOTAL_ELEMENTS)
    for (i in 0 .. TOTAL_ELEMENTS) {
        a.push((i & 1) == 0)
    }
    b |> run("iterate/{TOTAL_ELEMENTS}", TOTAL_ELEMENTS) <| $() {
        let t = count_trues_iter_bool(a)
        if (t != TOTAL_ELEMENTS / 2) {
            b->failNow()
        }
    }
}
