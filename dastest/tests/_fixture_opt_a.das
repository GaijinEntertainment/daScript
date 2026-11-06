options gen2
options stack = 1024
require dastest/testing_boost

[test]
def test_opt_a(t : T?) {
    t |> success(true)
}
