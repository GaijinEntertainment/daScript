options gen2

require dastest/testing_boost public

//! a fixture for test_timeouts.das: an argument [test] does
//! not take does not compile

[test(slow = true)]
def test_unknown_argument(t : T?) {
    t |> equal(1, 1)
}
