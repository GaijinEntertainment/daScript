options gen2
require dastest/testing_boost public
require daslib/ast_boost
require daslib/ast_verify

// Trees the verifier must accept. A quoted tree is never inferred, so an increment of a
// literal inside one is data, not a malformed l-value.

[test]
def test_quoted_increment_of_a_constant(t : T?) {
    ast_gc_guard() {
        let q = quote(0 ++)
        t |> success(q != null)
    }
}
