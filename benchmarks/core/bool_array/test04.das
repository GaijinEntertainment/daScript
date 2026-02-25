options gen2
options persistent_heap

// Benchmark: compound XOR (^^=) bit flip on 1M elements. Tests BoolArray's ^^= operator,
// BoolArray's manual read+write ([]+[]=), and array<bool>'s ^^= for comparison.
// Ported from examples/profile/bool_array_test/test04.das.

require dastest/testing_boost

require daslib/bool_array

let TOTAL_ELEMENTS = 1000000

[sideeffects]
def flip_compound(var a : auto(ArrayType)) {
    for (i in 0 .. TOTAL_ELEMENTS) {
        a[i] ^^= (i & 1) == 0
    }
}

[sideeffects]
def flip_simple(var a : auto(ArrayType)) {
    for (i in 0 .. TOTAL_ELEMENTS) {
        let v = a[i]
        a[i] = v ^^ ((i & 1) == 0)
    }
}

def make_bool_array() : BoolArray {
    var inscope a : BoolArray
    a.reserve(TOTAL_ELEMENTS)
    for (i in 0 .. TOTAL_ELEMENTS) {
        a.push(true)
    }
    return <- a
}

def make_array_bool() : array<bool> {
    var inscope a : array<bool>
    a.reserve(TOTAL_ELEMENTS)
    for (i in 0 .. TOTAL_ELEMENTS) {
        a.push(true)
    }
    return <- a
}

[benchmark]
def bool_array_xor_compound(b : B?) {
    var a <- make_bool_array()
    b |> run("flip_compound/{TOTAL_ELEMENTS}", TOTAL_ELEMENTS) <| $() {
        flip_compound(a)
    }
    delete a
}

[benchmark]
def bool_array_xor_simple(b : B?) {
    var a <- make_bool_array()
    b |> run("flip_simple/{TOTAL_ELEMENTS}", TOTAL_ELEMENTS) <| $() {
        flip_simple(a)
    }
    delete a
}

[benchmark]
def array_bool_xor(b : B?) {
    var a <- make_array_bool()
    b |> run("flip_compound/{TOTAL_ELEMENTS}", TOTAL_ELEMENTS) <| $() {
        flip_compound(a)
    }
    delete a
}
