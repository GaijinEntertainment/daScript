options gen2
options indenting = 4

require dastest/testing_boost
require mc_generic_origin_mod

[test]
def test_go(t : T?) {
    t |> equal(go(1, 2.0f), 2, "two words")
}
