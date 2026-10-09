options gen2
options _nolint = "PERF013" // the '-= 1' compound write is a shape under test, beside the postfix '--'
require dastest/testing_boost

struct S {
    a : int
    b : int[4]
}

def dec_offset(var s : S?; k : int) {
    var q : int? = unsafe(reinterpret<int?>(unsafe(reinterpret<uint64>(s)) + 4ul))
    unsafe(q[k]) -= 1
}

def dec_offset_postfix(var s : S?; k : int) {
    var q : int? = unsafe(reinterpret<int?>(unsafe(reinterpret<uint64>(s)) + 4ul))
    unsafe(q[k])--
}

def dec_field(var s : S?; k : int) {
    s.b[k] -= 1
}

def dec_branch(var s : S?; k : int) {
    if (k < 2) {
        var q : int? = unsafe(reinterpret<int?>(unsafe(reinterpret<uint64>(s)) + 4ul))
        unsafe(q[k]) -= 1
    } else {
        var r : int? = unsafe(reinterpret<int?>(unsafe(reinterpret<uint64>(s)) + 12ul))
        unsafe(r[k - 2]) -= 1
    }
}

[unsafe_deref]
def store_byte(dst : uint64; value : uint8) : uint64 {
    unsafe(unsafe(reinterpret<uint8?>(dst))[0]) = value
    return dst
}

[unsafe_deref]
def store_via(p : uint8?; value : uint8) {
    store_byte(unsafe(reinterpret<uint64>(p)), value)
}

[test]
def test_a_store_through_a_pointer_computed_from_an_address_is_a_side_effect(t : T?) {
    var w = S()
    var x = S()
    var y = S()
    var z = S()
    for (k in [0, 1, 1, 3]) {
        dec_offset(unsafe(addr(w)), k)
        dec_offset_postfix(unsafe(addr(x)), k)
        dec_field(unsafe(addr(y)), k)
        dec_branch(unsafe(addr(z)), k)
    }
    t |> equal([w.b[0], w.b[1], w.b[3]], [-1, -2, -1], "pointer held in a local, -= 1")
    t |> equal([x.b[0], x.b[1], x.b[3]], [-1, -2, -1], "pointer held in a local, postfix --")
    t |> equal([y.b[0], y.b[1], y.b[3]], [-1, -2, -1], "field access")
    t |> equal([z.b[0], z.b[1], z.b[3]], [-1, -2, -1], "pointer held in a local inside a branch")
}

[test]
def test_a_call_whose_only_effect_is_a_store_through_a_reinterpreted_pointer_is_kept(t : T?) {
    var a : uint8[16]
    store_byte(unsafe(reinterpret<uint64>(unsafe(addr(a[0])))), uint8(7))
    t |> equal(int(a[0]), 7, "store_byte called as a statement")
    store_via(unsafe(addr(a[1])), uint8(9))
    t |> equal(int(a[1]), 9, "store_via called as a statement under [unsafe_deref]")
}
