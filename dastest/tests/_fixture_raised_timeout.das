options gen2

require dastest/testing_boost public
require daslib/fio

//! a fixture for test_timeouts.das: one test raises its own
//! budget past the per-file deadline the parent passes

[test(description = "a test that declares its budget keeps the file alive past the per-file deadline", timeout = 10)]
def test_raised(t : T?) {
    t |> run("sleeps past the parent's per-file deadline, within its own budget") <| @(t : T?) {
        fio::sleep(300u)
        t |> equal(1, 1)
    }
}
