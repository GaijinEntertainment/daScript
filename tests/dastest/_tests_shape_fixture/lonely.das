options gen2

require dastest/testing_boost public

//! a category file with one test: too small to be a category

[test(description = "a lone test outside uncategorized")]
def test_lonely(t : T?) {
    t |> equal(1, 1)
}
