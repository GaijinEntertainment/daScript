options gen2

require dastest/testing_boost public

//! a continuation whose predecessor uncategorized_2.das does not
//! exist

[test(description = "a continuation file with no predecessor is misnamed")]
def test_orphan_continuation(t : T?) {
    t |> equal(1, 1)
}
