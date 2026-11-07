options gen2
require dastest/testing_boost

struct U {
    x : int
}

def operator .s(u : U) : int => u.x

def operator .s :=(var u : U; v : int) {
    u.x = v
}

struct S {
    u : U
}

[test]
def test_compound_assignment_through_a_property_under_deref_of_a_pointer_sum(t : T?) {
    var arr : S[4]
    unsafe {
        var p : S? = addr(arr[0])
        deref(p + 2).u.s += 1
        let k = 3
        deref(p + k).u.s += 5
    }
    t |> equal(arr[2].u.x, 1)
    t |> equal(arr[3].u.x, 5)
}

[test]
def test_compound_assignment_through_a_property_of_a_with_over_deref_of_a_pointer_sum(t : T?) {
    var arr : S[4]
    unsafe {
        var p : S? = addr(arr[0])
        with (deref(p + 1).u) {
            s += 4
        }
    }
    t |> equal(arr[1].u.x, 4)
}

[test]
def test_compound_assignment_through_a_property_of_a_with_over_a_pointer(t : T?) {
    var arr : S[4]
    var p : U? = unsafe(addr(arr[2].u))
    with (p) {
        s += 6
    }
    t |> equal(arr[2].u.x, 6)
}
