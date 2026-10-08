options gen2

require dastest/testing_boost public
require daslib/match

[sideeffects]
def legacy_arms(n : int) {
    match (n) {   // nolint:STYLE044 - covers the deprecated arm form while it still compiles
        if (0) {
            return "zero"
        }
        if ($v(k) && k > 0) {
            return "positive {k}"
        }
        if (_) {
            return "negative"
        }
    }
    return "unreachable"
}

[test]
def test_legacy_if_arms_still_compile(t : T?) {
    t |> equal(legacy_arms(0), "zero")
    t |> equal(legacy_arms(3), "positive 3")
    t |> equal(legacy_arms(-3), "negative")
}
