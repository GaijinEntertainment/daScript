options gen2

require dastest/testing_boost public
require daslib/fio

//! a fixture for test_timeouts.das: one described failure, one
//! described pass, one test past its budget

[test(description = "the described failure prints this claim under its FAIL line")]
def test_described_failure(t : T?) {
    t |> run("fails on purpose") <| @(t : T?) {
        t |> failure("deliberate")
    }
}

[test(description = "a passing test's claim is never printed")]
def test_described_pass(t : T?) {
    t |> run("passes") <| @(t : T?) {
        t |> equal(1, 1)
    }
}

[test(timeout = 0.001)]
def test_over_budget(t : T?) {
    t |> run("sleeps past a millisecond budget") <| @(t : T?) {
        fio::sleep(50u)
        t |> equal(1, 1)
    }
}

[test(timeout = 1)]
def test_over_integer_budget(t : T?) {
    t |> run("sleeps past a one-second budget spelled as an int") <| @(t : T?) {
        fio::sleep(1200u)
        t |> equal(1, 1)
    }
}
