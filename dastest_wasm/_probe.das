options gen2
require testing_boost

[test]
def test_pass_one(t : testing::T?) {
    t |> run("lambda invoke") @(t : testing::T?) {
        var lam <- @(x : int) : int { return x * 2 }
        t |> equal(invoke(lam, 5), 10)
        unsafe { delete lam; }
    }
}

[test]
def test_pass_two(t : testing::T?) {
    t |> run("arithmetic") @(t : testing::T?) {
        t |> equal(1 + 1, 2)
    }
}
