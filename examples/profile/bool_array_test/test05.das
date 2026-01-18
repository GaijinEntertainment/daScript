options gen2
options persistent_heap

require daslib/bool_array

let TOTAL_ELEMENTS = 10000

[sideeffects]
def test(dummy : auto(ArrayType)) {
    var inscope a : ArrayType
    a.reserve(TOTAL_ELEMENTS)
    for (i in 0 .. TOTAL_ELEMENTS) {
        a.push((i & 1) == 0, 0)
    }
    return a.length()
}

[export]
def main {
    print("test05 - insert at 0; fill array with alternating true/false values\n")
    profile(20, "BoolArray - insert") {
        test(default<BoolArray>)
    }
    profile(20, "Array<bool> - push") {
        test(default<array<bool>>)
    }
}

