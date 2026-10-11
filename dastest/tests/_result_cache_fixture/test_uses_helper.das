options gen2

require dastest/testing_boost public
require _helper

[test]
def test_helper_value(t : T?) {
    t |> equal(helper_value(), 42)
}
