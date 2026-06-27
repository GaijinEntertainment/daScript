options gen2
require testing_boost

[test]
def test_will_pass(t : testing::T?) {
    t |> run("ok") @(t : testing::T?) { t |> equal(2 + 2, 4) }
}

[test]
def test_will_fail(t : testing::T?) {
    t |> run("bad") @(t : testing::T?) { t |> equal(2 + 2, 5) }
}
