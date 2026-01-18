options gen2
options persistent_heap

require daslib/bool_array

let TOTAL_ELEMENTS = 1000000

[sideeffects]
def test(var a : auto(ArrayType)) {
    var count = 0
    for (e in a) {
        if (e) count ++
    }
    return count
}

[export]
def main {
    print("test03 - access by iteration; count total number of trues\n")
    if (true) {
        var a : BoolArray
        a.reserve(TOTAL_ELEMENTS)
        for (i in 0 .. TOTAL_ELEMENTS) {
            a.push((i & 1) == 0)
        }
        profile(20, "BoolArray - access") {
            let t = test(a)
            assert(t == (TOTAL_ELEMENTS / 2))
        }
    }
    if (true) {
        var a : array<bool>
        a.reserve(TOTAL_ELEMENTS)
        for (i in 0 .. TOTAL_ELEMENTS) {
            a.push((i & 1) == 0)
        }
        profile(20, "Array<bool> - access") {
            let t = test(a)
            assert(t == (TOTAL_ELEMENTS / 2))
        }
    }
}
