options gen2
require daslib/templates
require dastest/testing_boost public

[template(a), sideeffects]
def boo(x : int; a : auto(TT)) => "{x}_{typeinfo typename(type<TT>)}"

[test(description = "a [template] argument binds a type or decltype so the function sees it as a const type name")]
def test_template(t : T?) {
    let a = boo(123, type<float>)
    t |> equal(a, "123_float const")
    let b = boo(456, decltype(2 + 2))
    t |> equal(b, "456_int const")
}
