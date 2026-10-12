options gen2
options optimize = false
require dastest/testing_boost

// A local typedef nobody uses is never inferred, so the expressions in its dimension
// reach the JIT with no type.
[test]
def test_unused_typedef_with_an_untyped_dim(t : T?) {
    typedef Alias = Elem[Elem[Index]]
    t |> success(true)
}

[test]
def test_unused_typedef_with_an_untyped_for_source(t : T?) {
    typedef Alias = int[[for(x in Elem); x]]
    t |> success(true)
}
