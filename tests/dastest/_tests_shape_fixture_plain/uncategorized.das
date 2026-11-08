options gen2
options persistent_heap

require dastest/testing_boost public

//! the folder's plain uncategorized file carrying a fence option: a
//! plain test could not join it

[test(description = "the plain uncategorized file must carry no fence option")]
def test_fenced_plain(t : T?) {
    t |> equal(1, 1)
}
