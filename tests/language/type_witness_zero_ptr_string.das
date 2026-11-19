//! a pointer or string type witness has its own zero const node
options gen2

require dastest/testing_boost public

[test(description = "a pointer type witness is a null pointer const")]
def test_type_witness_zero_pointer(t : T?) {
    let p = [type<int?&> ]
    t |> equal(p[0], null)
}

[test(description = "a string type witness is an empty string const")]
def test_type_witness_zero_string(t : T?) {
    let s = [type<string&> ]
    t |> equal(length(s[0]), 0)
}

[test(description = "a workhorse type witness still zero-initializes")]
def test_type_witness_zero_int(t : T?) {
    let i = [type<int&> ]
    t |> equal(i[0], 0)
}
