options gen2

require dastest/testing_boost public

//! named as fenced but carrying no fence option: its test belongs in
//! uncategorized.das

[test(description = "a plain test hiding in a fenced name")]
def test_rtti(t : T?) {
    t |> equal(1, 1)
}
