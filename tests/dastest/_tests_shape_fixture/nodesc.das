options gen2

require dastest/testing_boost public

//! a category file of three where one test has no description

[test(description = "the first described test")]
def test_d1(t : T?) {
    t |> equal(1, 1)
}

[test]
def test_undescribed(t : T?) {
    t |> equal(1, 1)
}

[test(description = "the third described test")]
def test_d3(t : T?) {
    t |> equal(1, 1)
}
