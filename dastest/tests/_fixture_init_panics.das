options gen2

require dastest/testing_boost public

//! a fixture for test_together.das: a file whose [init] panics, so
//! the together root's one simulate fails for every file of the batch

[init]
def init_panics() {
    panic("the fixture's init panics")
}

[test]
def test_never_runs(t : T?) {
    t |> failure("a program whose simulate failed runs no test")
}
