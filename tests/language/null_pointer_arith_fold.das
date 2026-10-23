options gen2
require dastest/testing_boost public

def private offset_or_null(p : int?; n : int64) : int const? {
    unsafe {
        return p != null ? p + n : null
    }
}

[test]
def test_folded_null_pointer_arith(t : T?) {
    var p : int? = null
    unsafe {
        let q = p != null ? p + 3l : null
        t |> equal(q == null, true)
    }
    t |> equal(offset_or_null(null, 5l) == null, true)
}
