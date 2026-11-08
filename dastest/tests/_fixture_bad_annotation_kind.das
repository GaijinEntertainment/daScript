options gen2

require dastest/testing_boost public

//! a fixture for test_timeouts.das: a description that is not
//! a string does not compile

[test(description = 5)]
def test_bad_description(t : T?) {
    t |> equal(1, 1)
}
