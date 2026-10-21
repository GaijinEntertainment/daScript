options gen2

require dastest/testing_boost public
require daslib/ast_boost
require daslib/ast_verify

[test]
def test_parsed_ranges(t : T?) {
    ast_gc_guard() {
        var clean = quote(a + b * c)
        t |> equal(0, verify_parsed_expression(clean), "a parsed expression has no misplaced range")

        var escaped = quote(a + b) as ExprOp2
        escaped.right.at.last_line += 5u
        t |> success(verify_parsed_expression(escaped) > 0,
            "a child past its parent's end is not reported")

        var swapped = quote(a * b) as ExprOp2
        let left = swapped.left.at
        swapped.left.at = swapped.right.at
        swapped.right.at = left
        t |> success(verify_parsed_expression(swapped) > 0,
            "siblings out of source order are not reported")
    }
}
