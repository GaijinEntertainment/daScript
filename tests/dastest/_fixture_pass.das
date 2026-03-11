options gen2
require dastest/testing_boost

[test]
def test_passing(t : T?) {
    t |> success(true)
}

[test]
def test_also_passing(t : T?) {
    t |> equal(1, 1)
}
