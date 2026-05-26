options gen2
options indenting = 4

require dastest/testing_boost public
require daslib/with_boost

struct A {
    f1 : int
    f2 : int
}

[test]
def test_return_int_from_array(t : T?) {
    //! `let f = with_(arr[i]) { return _.field }` — value flows out of
    //! the body through the helper's return.
    var arr <- [A(f1=42, f2=99)]
    let v = with_(arr[0]) {
        return _.f1
    }
    t |> equal(v, 42, "returned value matches the bound element field")
    delete arr
}

[test]
def test_return_after_mutation(t : T?) {
    //! Mutation inside the body THEN return the field — both effects.
    var arr <- [A(f1=1, f2=2)]
    let v = with_(arr[0]) {
        _.f1 = 999
        return _.f1
    }
    t |> equal(v, 999, "returned the post-mutation value")
    t |> equal(arr[0].f1, 999, "mutation also persisted")
    delete arr
}

[test]
def test_return_struct_copy(t : T?) {
    //! Returning the struct itself yields a COPY; subsequent mutation
    //! of the returned value does not affect the array.
    var arr <- [A(f1=10, f2=20)]
    var copyOut = with_(arr[0]) {
        return _
    }
    copyOut.f1 = 100
    t |> equal(arr[0].f1, 10, "array element untouched by mutation of returned copy")
    t |> equal(copyOut.f1, 100, "local copy reflects the post-mutation value")
    delete arr
}

[test]
def test_return_from_table(t : T?) {
    var tab : table<string; A>
    tab |> insert("k", A(f1 = 7, f2 = 8))
    let v = with_(tab["k"]) $(v) {
        return v.f1 * v.f2
    }
    t |> equal(v, 56, "computed value returned from table-with_")
}

[test]
def test_named_binding_return(t : T?) {
    var arr <- [A(f1=3, f2=4)]
    let r = with_(arr[0]) $(elem) {
        return elem.f2
    }
    t |> equal(r, 4, "named binding return value")
    delete arr
}
