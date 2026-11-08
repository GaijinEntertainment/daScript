options gen2
options persistent_heap

require dastest/testing_boost public

//! a second fenced file with the signature
//! uncategorized_persistent_heap.das already owns

[test(description = "a duplicate fence signature in one folder is two files for one program")]
def test_second_heap(t : T?) {
    t |> equal(1, 1)
}
