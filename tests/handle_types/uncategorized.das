options gen2

require dastest/testing_boost
require UnitTest
require daslib/rtti

def operator [] (arr : Point3Array; ind : int) => 2.0 * arr.[ind]

def test_point3_array_index(t : T?; arr : Point3Array) {
    assert(arr |> length == 10)
    for (i in range(10)) {
        let p = arr[i]
        let p_native = arr.[i]
        t |> equal(2.0, p.x)
        t |> equal(4.0, p.y)
        t |> equal(6.0, p.z)
        t |> equal(1.0, p_native.x)
        t |> equal(2.0, p_native.y)
        t |> equal(3.0, p_native.z)
    }
}

def test_point3_array_each(t : T?; arr : Point3Array) {
    assert(arr |> length == 10)
    for (p in arr) {
        t |> equal(1.0, p.x)
        t |> equal(2.0, p.y)
        t |> equal(3.0, p.z)
    }
}

def test_point3_array_block(t : T?) {
    testPoint3Array() $(arr) {
        assert(arr |> length == 10)
        for (p in arr) {
            t |> equal(1.0, p.x)
            t |> equal(2.0, p.y)
            t |> equal(3.0, p.z)
        }
    }
}

[test(description = "a handled vector type indexes with a user operator, iterates with each and passes through an invoked block")]
def test_handle_type(t : T?) {
    t |> run("[]") @(t : T?) {
        testPoint3Array() $(arr) {
            test_point3_array_index(t, arr)
        }
    }
    t |> run("each") @(t : T?) {
        testPoint3Array() $(arr) {
            test_point3_array_each(t, arr)
        }
    }
    t |> run("invoke block") @(t : T?) {
        test_point3_array_block(t)
    }
}

// A handled sub-object of a literal is filled through a fake variable that carries its own
// offset inside the literal.  Where the literal writes through a reference -- assignment or
// move into an existing local, and the inline splice that lowers to one -- that offset used
// to be added twice, so everything but a sub-object at offset zero landed past its field.

struct Outer {
    lead : int
    @safe_when_uninitialized at : LineInfo
    trail : int
}


struct Pair {
    lead : int
    @safe_when_uninitialized ats : LineInfo[2]
}


[test(description = "a handled sub-object inside a literal lands at its own field offset for copy, move, initializer and array-element writes into existing locals")]
def test_handled_sub_object_offset(t : T?) {
    t |> run("copy into an existing local") @(t : T?) {
        var o = Outer()
        o = Outer(lead = 1, at = LineInfo(line = 42u), trail = 2)
        t |> equal(1, o.lead)
        t |> equal(42u, o.at.line)
        t |> equal(2, o.trail)
    }
    t |> run("move into an existing local") @(t : T?) {
        var o = Outer()
        o <- Outer(lead = 1, at = LineInfo(line = 42u), trail = 2)
        t |> equal(42u, o.at.line)
    }
    t |> run("initializer") @(t : T?) {
        let o <- Outer(lead = 1, at = LineInfo(line = 42u), trail = 2)
        t |> equal(42u, o.at.line)
    }
    t |> run("array element into an existing local") @(t : T?) {
        var p = Pair()
        p = Pair(lead = 1, ats = fixed_array(LineInfo(line = 42u), LineInfo(line = 43u)))
        t |> equal(42u, p.ats[0].line)
        t |> equal(43u, p.ats[1].line)
    }
}

[test(description = "a null string reaches a native function as empty, an empty literal stays empty and a non-empty string is untouched")]
def test_string_arg_never_null(t : T?) {
    t |> run("a null string arrives as empty") @(t : T?) {
        let nul_str : string
        t |> equal(test_string_arg_length(nul_str), 0l)
    }
    t |> run("an empty literal arrives as empty") @(t : T?) {
        let empty_literal = ""
        t |> equal(test_string_arg_length(empty_literal), 0l)
    }
    t |> run("a non-empty string is untouched") @(t : T?) {
        t |> equal(test_string_arg_length("abc"), 3l)
    }
}

[benchmark]
def bench_string_arg_wrap(b : B?) {
    let nul_str : string
    let lit = "abcd"
    var acc = 0l
    b |> run("4 string args") {
        acc += test_four_strings(nul_str, lit, nul_str, lit)
    }
    let nul_ptr : void? = null
    let some_ptr = unsafe(reinterpret<void?>(lit))
    b |> run("4 pointer args") {
        acc += test_four_pointers(nul_ptr, some_ptr, nul_ptr, some_ptr)
    }
    feint("{acc}")
}

[test(description = "four string arguments and four pointer arguments reach the native function as the same pointers")]
def test_four_string_args_pass_the_same_pointers_as_four_pointer_args(t : T?) {
    let nul_str : string
    let lit = "abcd"
    t |> equal(test_four_strings(nul_str, lit, nul_str, lit), 0l)
    let nul_ptr : void? = null
    let some_ptr = unsafe(reinterpret<void?>(lit))
    t |> equal(test_four_pointers(nul_ptr, some_ptr, nul_ptr, some_ptr), 0l)
}
