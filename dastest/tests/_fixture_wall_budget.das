options gen2

require dastest/testing_boost public
require daslib/fio

//! a fixture for test_timeouts.das: one test that outlasts the
//! --test-wall the parent passes, inside the budget it declares

[test(description = "a test that declares its budget runs past the run's wall", timeout = 30)]
def test_within_its_budget(t : T?) {
    fio::sleep(2500u)
    t |> success(true, "the test outlasted the run's wall")
}
