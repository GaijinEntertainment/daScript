options gen2
options indenting = 4
options rtti

require daslib/ast
require daslib/rtti
require daslib/ast_match
require daslib/ast_boost
require daslib/templates_boost
require dastest/testing_boost public

// ── macro-synthesized op nodes ──────────────────────────────────────────
// ExprOp* inherit ExprCallFunc; the parser mirrors `op` into `name`, but a
// macro-built node (`new ExprOp2(op := "-")`) leaves `name` empty. Matching
// keys on `op` + children only — the call-machinery fields are skipped.

[test(description = "an operator pattern matches a synthesized ExprOp2 whose name is empty and captures its operands")]
def test_synthesized_op2_matches(t : T?) {
    ast_gc_guard() {
        var e : ExpressionPtr = new ExprOp2(at = LineInfo(), op := "-", left = qmacro(p), right = qmacro(q))
        var x, y : ExpressionPtr
        let r = qmatch(e, $e(x) - $e(y))
        t |> success(r.matched, "pattern must match a synthesized ExprOp2 (empty `name`)")
        if (r.matched) {
            t |> equal(describe(x), "p")
            t |> equal(describe(y), "q")
        }
    }
}

[test(description = "a minus pattern does not match a synthesized plus ExprOp2")]
def test_synthesized_op2_wrong_op_fails(t : T?) {
    ast_gc_guard() {
        var e : ExpressionPtr = new ExprOp2(at = LineInfo(), op := "+", left = qmacro(p), right = qmacro(q))
        var x, y : ExpressionPtr
        let r = qmatch(e, $e(x) - $e(y))
        t |> success(!r.matched, "`+` node must not match a `-` pattern")
    }
}

[test(description = "a unary pattern matches a synthesized ExprOp1 whose name is empty")]
def test_synthesized_op1_matches(t : T?) {
    ast_gc_guard() {
        var e : ExpressionPtr = new ExprOp1(at = LineInfo(), op := "-", subexpr = qmacro(p))
        var x : ExpressionPtr
        let r = qmatch(e, -$e(x))
        t |> success(r.matched, "pattern must match a synthesized ExprOp1 (empty `name`)")
        if (r.matched) {
            t |> equal(describe(x), "p")
        }
    }
}

[test(description = "a nested pattern matches a synthesized tree of operator nodes")]
def test_synthesized_nested_matches(t : T?) {
    ast_gc_guard() {
        // the flatten re-fuse shape: synthesized mul under synthesized add
        var mul : ExpressionPtr = new ExprOp2(at = LineInfo(), op := "*", left = qmacro(a), right = qmacro(b))
        var e : ExpressionPtr = new ExprOp2(at = LineInfo(), op := "+", left = mul, right = qmacro(c))
        var fa, fb, fc : ExpressionPtr
        let r = qmatch(e, $e(fa) * $e(fb) + $e(fc))
        t |> success(r.matched, "nested synthesized op2 shape must match")
        if (r.matched) {
            t |> equal(describe(fa), "a")
            t |> equal(describe(fc), "c")
        }
    }
}
