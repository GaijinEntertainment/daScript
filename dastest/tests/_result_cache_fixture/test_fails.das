options gen2

require dastest/testing_boost public

[test]
def test_always_fails(t : T?) {
    t |> failure("this fixture fails on purpose")
}
