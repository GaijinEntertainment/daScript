options gen2
options stack = 2048
require dastest/testing_boost

[test]
def test_opt_b(t : T?) {
    t |> success(true)
}
