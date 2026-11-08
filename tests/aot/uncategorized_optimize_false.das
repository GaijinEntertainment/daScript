options gen2
options optimize = false
require dastest/testing_boost public

[export]
def assumed_interpolation : string {
    assume v = "n={1 + 1}"
    return "{v}"
}

[test(description = "An assume bound to an interpolated string evaluates to its text with optimizations off")]
def test_assume_string_builder(t : T?) {
    t |> equal(assumed_interpolation(), "n=2")
}
