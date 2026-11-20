options gen2

require dastest/testing_boost public
require daslib/fio

//! a fixture for test_timeouts.das: one test that outlasts the
//! budget it declares, under the longer --test-wall the parent passes

[test(description = "a test past its own budget runs to its end under a longer wall", timeout = 1)]
def test_past_its_budget(t : T?) {
    fio::sleep(2500u)
    t |> success(true, "the wall did not end the process at the budget")
}
