options gen2
options persistent_heap

require dastest/testing_boost public

//! a fenced uncategorized file: its signature is the option in its
//! name, so one described test is fine

[test(description = "a test that needs the persistent heap lives here")]
def test_heap(t : T?) {
    t |> equal(1, 1)
}
