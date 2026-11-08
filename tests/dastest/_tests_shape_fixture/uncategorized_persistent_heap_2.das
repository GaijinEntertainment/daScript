options gen2
options persistent_heap

require dastest/testing_boost public

//! a continuation of a fenced file that is nowhere near the cap

[test(description = "a continuation of a file under the cap belongs in that file")]
def test_early_continuation(t : T?) {
    t |> equal(1, 1)
}
