options gen2

require dastest/testing_boost public

//! four described tests: over a cap of three

[test(description = "big one")]
def test_b1(t : T?) {
    t |> equal(1, 1)
}

[test(description = "big two")]
def test_b2(t : T?) {
    t |> equal(1, 1)
}

[test(description = "big three")]
def test_b3(t : T?) {
    t |> equal(1, 1)
}

[test(description = "big four")]
def test_b4(t : T?) {
    t |> equal(1, 1)
}
