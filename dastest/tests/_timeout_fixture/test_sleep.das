options gen2

require dastest/testing_boost public
require daslib/fio

//! a fixture for test_timeout.das: one test that outlives the one-second --timeout the parent passes

[test]
def test_sleep(t : T?) {
    t |> run("sleeps past the parent's --timeout") <| @(t : T?) {
        fio::sleep(8000u)
        t |> failure("the parent's --timeout should have ended the process before this line")
    }
}
