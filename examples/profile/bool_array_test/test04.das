options gen2
options persistent_heap

require daslib/bool_array

let TOTAL_ELEMENTS = 1000000

[sideeffects]
def test(var a : auto(ArrayType)) {
    for (i in 0 .. TOTAL_ELEMENTS) {
        a[i] ^^= (i & 1) == 0
    }
}


[sideeffects]
def test_simple(var a : auto(ArrayType)) {
    for (i in 0 .. TOTAL_ELEMENTS) {
        let A = a[i]
        a[i] = A ^^ ((i & 1) == 0)
    }
}

[export]
def main {
    print("test04 - compound operation, ^^= with flipping bit \n")
    if (true) {
        var inscope a : BoolArray
        a.reserve(TOTAL_ELEMENTS)
        for (i in 0 .. TOTAL_ELEMENTS) {
            a.push(true)
        }
        profile(20, "BoolArray - flip bits, use []^^=") {
            test(a)
        }
        var count = 0
        for (i in 0 .. TOTAL_ELEMENTS) {
            if (a[i]) count ++
        }
        assert(count == TOTAL_ELEMENTS)
    }
    if (true) {
        var inscope a : BoolArray
        a.reserve(TOTAL_ELEMENTS)
        for (i in 0 .. TOTAL_ELEMENTS) {
            a.push(true)
        }
        profile(20, "BoolArray - flip bits, use [] and then []=") {
            test_simple(a)
        }
        var count = 0
        for (i in 0 .. TOTAL_ELEMENTS) {
            if (a[i]) count ++
        }
        assert(count == TOTAL_ELEMENTS)
    }
    if (true) {
        var inscope a : array<bool>
        a.reserve(TOTAL_ELEMENTS)
        for (i in 0 .. TOTAL_ELEMENTS) {
            a.push(true)
        }
        profile(20, "Array<bool> - flip bits") {
            test(a)
        }
        var count = 0
        for (i in 0 .. TOTAL_ELEMENTS) {
            if (a[i]) count ++
        }
        assert(count == TOTAL_ELEMENTS)
    }
}
