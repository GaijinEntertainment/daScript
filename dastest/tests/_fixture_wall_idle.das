options gen2

require dastest/testing_boost public
require daslib/fio

//! a fixture for test_timeouts.das: one test that ends at once, and
//! a finalize that outlasts the --test-wall the parent passes - the
//! wall guards a test, not what its file does after it

[test]
def test_ends_at_once(t : T?) {
    t |> success(true, "the test is over before the wall")
}

[finalize]
def idle_past_the_wall() {
    fio::sleep(2500u)
}
