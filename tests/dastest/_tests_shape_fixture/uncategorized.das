options gen2

require dastest/testing_boost public

//! the folder's plain uncategorized file: no fence option, three
//! described tests

[test(description = "one plus one is two")]
def test_a(t : T?) {
    t |> equal(1 + 1, 2)
}

[test(description = "two plus two is four")]
def test_b(t : T?) {
    t |> equal(2 + 2, 4)
}

[test(description = "three plus three is six")]
def test_c(t : T?) {
    t |> equal(3 + 3, 6)
}
