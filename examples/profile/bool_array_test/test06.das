options gen2
options persistent_heap

require daslib/bool_array

let TOTAL_ELEMENTS = 10000

[sideeffects]
def test(dummy : auto(ArrayType)) {
    var a : ArrayType
    a.resize(TOTAL_ELEMENTS)
    for (i in 0 .. TOTAL_ELEMENTS) {
        a.erase(0)
    }
    return a.length()
}

[export]
def main {
    print("test06 - erase at 0\n")
    profile(20, "BoolArray - erase") {
        test(default<BoolArray>)
    }
    profile(20, "Array<bool> - erase") {
        test(default<array<bool>>)
    }
}

