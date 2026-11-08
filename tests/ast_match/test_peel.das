options gen2
options rtti

require daslib/ast
require daslib/ast_match
require daslib/ast_boost
require daslib/templates_boost
require dastest/testing_boost public

// ``peel_lambda_rename_2vars`` is the 2-arg variant (shape: aggregate's
// ``block<(acc, x) : AGG>``). Unlike the 1-arg form, this returns ``null``
// for non-peelable inputs — caller decides the fallback.

[test(description = "peel_lambda_rename_2vars turns a two-argument single-return lambda into its body with both bound variables renamed")]
def test_simple_rename(t : T?) {
    ast_gc_guard() {
        var lam = qmacro(@(acc, x) { return acc + x; })
        let body = peel_lambda_rename_2vars(lam, "a", "b")
        t |> success(body != null, "2-arg lambda should peel")
        t |> success(body is ExprOp2, "result should be ExprOp2")
        let op = body as ExprOp2
        t |> success(op.left is ExprVar, "left should be renamed ExprVar")
        t |> equal(string((op.left as ExprVar).name), "a", "acc → a")
        t |> success(op.right is ExprVar, "right should be renamed ExprVar")
        t |> equal(string((op.right as ExprVar).name), "b", "x → b")
    }
}

[test(description = "peel_lambda_rename_2vars returns null for a one-argument lambda")]
def test_one_arg_returns_null(t : T?) {
    ast_gc_guard() {
        var lam = qmacro(@(x) { return x + 1; })
        let body = peel_lambda_rename_2vars(lam, "a", "b")
        t |> success(body == null, "1-arg lambda must return null (no fallback)")
    }
}

[test(description = "peel_lambda_rename_2vars returns null for a three-argument lambda")]
def test_three_arg_returns_null(t : T?) {
    ast_gc_guard() {
        var lam = qmacro(@(x, y, z) { return x + y + z; })
        let body = peel_lambda_rename_2vars(lam, "a", "b")
        t |> success(body == null, "3-arg lambda must return null")
    }
}

[test(description = "peel_lambda_rename_2vars returns null for a lambda with more than one statement")]
def test_multi_statement_returns_null(t : T?) {
    ast_gc_guard() {
        var lam = qmacro(@(acc, x) { let t = acc; return t + x; })
        let body = peel_lambda_rename_2vars(lam, "a", "b")
        t |> success(body == null, "multi-statement body must return null")
    }
}

[test(description = "peel_lambda_rename_2vars returns null for an expression that is not a lambda")]
def test_non_lambda_returns_null(t : T?) {
    ast_gc_guard() {
        var notLam = qmacro(42)
        let body = peel_lambda_rename_2vars(notLam, "a", "b")
        t |> success(body == null, "non-lambda must return null")
    }
}

// ``peel_lambda_rename_var`` peels a single-arg, single-return lambda by
// renaming its bound variable to ``argName`` in the cloned body. Non-peelable
// inputs fall through to an ``invoke($e(expr), $i(argName))`` call so callers
// can splice the result unconditionally.

// ── Helpers ─────────────────────────────────────────────────────────────

def private leftmost_var_name(var e : Expression?) : string {
    // Walks ExprOp2.left chain to the first ExprVar; returns its name or "".
    var cur = e
    while (cur != null && cur is ExprOp2) {
        cur = (cur as ExprOp2).left
    }
    if (cur is ExprVar) return string((cur as ExprVar).name)
    return ""
}

// ── Peel succeeds: renames the bound var in the body ───────────────────

[test(description = "peel_lambda_rename_var turns a one-argument single-return lambda into its body with the bound variable renamed")]
def test_simple_rename_rename_var(t : T?) {
    ast_gc_guard() {
        var lam = qmacro(@(x) { return x + 1; })
        var body = peel_lambda_rename_var(lam, "y")
        t |> success(body != null, "single-return lambda should peel")
        t |> success(body is ExprOp2, "peeled body should be ExprOp2")
        t |> equal(leftmost_var_name(body), "y", "bound var should be renamed to y")
    }
}

[test(description = "peel_lambda_rename_var renames only the lambda's bound variable and leaves other identifiers alone")]
def test_rename_keeps_other_idents(t : T?) {
    ast_gc_guard() {
        // outerCapture shouldn't be touched — only x → y.
        var lam = qmacro(@(x) { return x + outerCapture; })
        let body = peel_lambda_rename_var(lam, "y")
        t |> success(body is ExprOp2, "should peel to ExprOp2")
        let op = body as ExprOp2
        t |> success(op.left is ExprVar, "left is ExprVar")
        t |> equal(string((op.left as ExprVar).name), "y", "left bound var → y")
        t |> success(op.right is ExprVar, "right is ExprVar")
        t |> equal(string((op.right as ExprVar).name), "outerCapture", "right outerCapture is preserved")
    }
}

// ── Peel fails: falls back to invoke ───────────────────────────────────

[test(description = "peel_lambda_rename_var wraps a two-argument lambda in an invoke instead of peeling")]
def test_two_arg_falls_back_to_invoke(t : T?) {
    ast_gc_guard() {
        var lam = qmacro(@(x, y) { return x + y; })
        let res = peel_lambda_rename_var(lam, "z")
        t |> success(res != null, "fallback should not return null")
        // ``invoke(...)`` parses to ExprInvoke, not a generic ExprCall.
        t |> success(res is ExprInvoke, "non-peelable: result should be ExprInvoke")
    }
}

[test(description = "peel_lambda_rename_var wraps a non-lambda expression in an invoke instead of peeling")]
def test_non_lambda_falls_back_to_invoke(t : T?) {
    ast_gc_guard() {
        var notLam = qmacro(somePtr)
        let res = peel_lambda_rename_var(notLam, "z")
        t |> success(res is ExprInvoke, "non-lambda → ExprInvoke fallback")
    }
}

// ── Clone discipline: input must not be mutated ────────────────────────

[test(description = "peel_lambda_rename_var leaves the input lambda's bound variable name unchanged")]
def test_input_lambda_not_mutated(t : T?) {
    ast_gc_guard() {
        var lam = qmacro(@(x) { return x + 1; })
        var _ = peel_lambda_rename_var(lam, "y")
        // Original lambda's bound-arg name must still be `x`.
        let mblk = lam as ExprMakeBlock
        let blk = mblk._block as ExprBlock
        t |> equal(string(blk.arguments[0].name), "x", "original lambda's bound var must remain `x`")
    }
}

// ``peel_lambda_replace_var`` peels a single-arg, single-return lambda by
// substituting its bound variable with an arbitrary expression in the cloned
// body (peeling-aware: strips typer ``ExprRef2Value`` wrappers from already-
// typed source ASTs). Non-peelable inputs fall back to ``invoke($e(expr), $e(replacement))``.

// ── Substitution-success path requires typed AST (ExprRef2Value wrappers) ──
// ``replaceVariablePeeling`` only fires on ``ExprRef2Value(ExprVar(name))``, not
// on a bare ``ExprVar``. The typer inserts these wrappers around reference-typed
// reads at inference time — raw ``qmacro``-built AST never has them. Build the
// wrapped shape manually to exercise the success path.
//
// apply_template mutates AST children in place but cannot replace the cloned
// root pointer (matches production usage in linq_fold: the var-being-replaced
// is always buried inside a larger expression like ``x + 1``, never at root).
// So the test body uses ``x + 1`` rather than bare ``x``.

def private wrap_ref2value_around_lhs_var(var lam : Expression?) : Expression? {
    // Lambda is ``@(x) { return x + 1; }``. Wrap the ExprVar ``x`` (the .left of
    // ret.subexpr's ExprOp2) in an ExprRef2Value, mimicking what the typer does.
    var mblk = lam as ExprMakeBlock
    var blk = mblk._block as ExprBlock
    var ret = blk.list[0] as ExprReturn
    var op = ret.subexpr as ExprOp2
    op.left = new ExprRef2Value(at = op.left.at, subexpr = op.left)
    return lam
}

[test(description = "peel_lambda_replace_var substitutes the replacement for a bound variable wrapped in a ref2value node and strips the wrapper")]
def test_replaces_buried_var(t : T?) {
    ast_gc_guard() {
        var raw = qmacro(@(x) { return x + 1; })
        var lam = wrap_ref2value_around_lhs_var(raw)
        var replacement : Expression? = qmacro(myVal)
        let body = peel_lambda_replace_var(lam, replacement)
        t |> success(body != null, "should peel")
        t |> success(body is ExprOp2, "result should be ExprOp2")
        let op = body as ExprOp2
        t |> success(op.left is ExprVar, "left should be substituted ExprVar (R2V wrapper stripped)")
        t |> equal(string((op.left as ExprVar).name), "myVal", "bound var was substituted with the replacement var")
    }
}

[test(description = "peel_lambda_replace_var wraps a two-argument lambda in an invoke instead of peeling")]
def test_two_arg_falls_back_to_invoke_replace_var(t : T?) {
    ast_gc_guard() {
        var lam = qmacro(@(x, y) { return x + y; })
        var replacement : Expression? = qmacro(z)
        let res = peel_lambda_replace_var(lam, replacement)
        // ``invoke(...)`` parses to ExprInvoke, not a generic ExprCall.
        t |> success(res is ExprInvoke, "non-peelable → ExprInvoke fallback")
    }
}

[test(description = "peel_lambda_replace_var wraps a non-lambda expression in an invoke instead of peeling")]
def test_non_lambda_falls_back_to_invoke_replace_var(t : T?) {
    ast_gc_guard() {
        var notLam = qmacro(somePtr)
        var replacement : Expression? = qmacro(z)
        let res = peel_lambda_replace_var(notLam, replacement)
        t |> success(res is ExprInvoke, "non-lambda → ExprInvoke fallback")
    }
}

// ``peel_lambda_single_return`` extracts the body of a single-arg, single-return
// lambda. The structural check is RTTI-only — no typer pass needed, so we can
// build the lambda AST inline with qmacro and assert directly.

[test(description = "peel_lambda_single_return returns the body expression of a one-argument single-return lambda")]
def test_simple_return(t : T?) {
    ast_gc_guard() {
        var lam = qmacro(@(x) { return x + 1; })
        let body = peel_lambda_single_return(lam)
        t |> success(body != null, "single-return lambda should peel to its body")
        // Body should be `x + 1` — an ExprOp2.
        t |> success(body is ExprOp2, "peeled body should be ExprOp2")
    }
}

[test(description = "peel_lambda_single_return returns null for a two-argument lambda")]
def test_two_args_does_not_peel(t : T?) {
    ast_gc_guard() {
        var lam = qmacro(@(x, y) { return x + y; })
        let body = peel_lambda_single_return(lam)
        t |> success(body == null, "2-arg lambda must return null from single-arg peel")
    }
}

[test(description = "peel_lambda_single_return returns null for a lambda with more than one statement")]
def test_multi_statement_does_not_peel(t : T?) {
    ast_gc_guard() {
        var lam = qmacro(@(x) { let a = x; return a; })
        let body = peel_lambda_single_return(lam)
        t |> success(body == null, "multi-statement lambda must return null")
    }
}

[test(description = "peel_lambda_single_return returns null for an expression that is not a lambda")]
def test_non_lambda_does_not_peel(t : T?) {
    ast_gc_guard() {
        var notLam = qmacro(42)
        let body = peel_lambda_single_return(notLam)
        t |> success(body == null, "non-lambda expression must return null")
    }
}

[test(description = "peel_lambda_single_return returns null for a null expression")]
def test_null_does_not_peel(t : T?) {
    var lam : Expression?
    let body = peel_lambda_single_return(lam)
    t |> success(body == null, "null input must return null")
}

// ``peel_tuple_field_read`` checks for the shape ``<bindName>._<fieldIndex>``
// (a tuple-slot read on a named bind). Single-level ``ExprRef2Value`` peel
// on each side — the typer never nests these wrappers.

// ── Builders ────────────────────────────────────────────────────────────

def private mk_var(name : string) : Expression? {
    var v = new ExprVar(at = LineInfo())
    v.name := name
    return v
}

def private mk_field(var value : Expression?; fieldName : string) : Expression? {
    var f = new ExprField(at = LineInfo(), value = value)
    f.name := fieldName
    return f
}

def private mk_r2v(var inner : Expression?) : Expression? =>
    new ExprRef2Value(at = LineInfo(), subexpr = inner)

// ── Direct matches ──────────────────────────────────────────────────────

[test(description = "peel_tuple_field_read accepts g._0 for bind name g and index 0")]
def test_match_bind_field0(t : T?) {
    ast_gc_guard() {
        let e = mk_field(mk_var("g"), "_0")
        t |> success(peel_tuple_field_read(e, "g", 0), "g._0 should match (g, 0)")
    }
}

[test(description = "peel_tuple_field_read accepts bind._5 for bind name bind and index 5")]
def test_match_bind_field5(t : T?) {
    ast_gc_guard() {
        let e = mk_field(mk_var("bind"), "_5")
        t |> success(peel_tuple_field_read(e, "bind", 5), "bind._5 should match (bind, 5)")
    }
}

// ── Mismatches ──────────────────────────────────────────────────────────

[test(description = "peel_tuple_field_read rejects a field read whose base variable has a different name")]
def test_wrong_bind_name(t : T?) {
    ast_gc_guard() {
        let e = mk_field(mk_var("g"), "_0")
        t |> success(!peel_tuple_field_read(e, "other", 0), "wrong bindName must not match")
    }
}

[test(description = "peel_tuple_field_read rejects a field read whose tuple index differs")]
def test_wrong_field_index(t : T?) {
    ast_gc_guard() {
        let e = mk_field(mk_var("g"), "_0")
        t |> success(!peel_tuple_field_read(e, "g", 1), "wrong fieldIndex must not match")
    }
}

[test(description = "peel_tuple_field_read rejects a field whose name is not a tuple slot _N")]
def test_field_not_tuple_slot(t : T?) {
    ast_gc_guard() {
        let e = mk_field(mk_var("g"), "name")
        t |> success(!peel_tuple_field_read(e, "g", 0), "non-_N field must not match")
    }
}

[test(description = "peel_tuple_field_read rejects a bare variable that is not a field read")]
def test_non_field_does_not_match(t : T?) {
    ast_gc_guard() {
        let e = mk_var("g")
        t |> success(!peel_tuple_field_read(e, "g", 0), "ExprVar alone must not match")
    }
}

[test(description = "peel_tuple_field_read rejects a field read whose base is not a variable")]
def test_value_not_var(t : T?) {
    ast_gc_guard() {
        // field over a literal — value is not ExprVar.
        var lit = qmacro(42)
        let e = mk_field(lit, "_0")
        t |> success(!peel_tuple_field_read(e, "g", 0), "ExprField over non-ExprVar must not match")
    }
}

// ── ExprRef2Value peeling (single level on each side) ──────────────────

[test(description = "peel_tuple_field_read sees through a ref2value wrapper around the whole field read")]
def test_r2v_wrapped_outer(t : T?) {
    ast_gc_guard() {
        var inner = mk_field(mk_var("g"), "_0")
        let wrapped = mk_r2v(inner)
        t |> success(peel_tuple_field_read(wrapped, "g", 0), "outer R2V wrapper must peel")
    }
}

[test(description = "peel_tuple_field_read sees through a ref2value wrapper around the base variable")]
def test_r2v_wrapped_inner_value(t : T?) {
    ast_gc_guard() {
        let e = mk_field(mk_r2v(mk_var("g")), "_0")
        t |> success(peel_tuple_field_read(e, "g", 0), "R2V wrapping inner ExprVar must peel")
    }
}

[test(description = "peel_tuple_field_read sees through ref2value wrappers on both the base and the whole read")]
def test_r2v_wrapped_both(t : T?) {
    ast_gc_guard() {
        var inner = mk_field(mk_r2v(mk_var("g")), "_0")
        let wrapped = mk_r2v(inner)
        t |> success(peel_tuple_field_read(wrapped, "g", 0), "R2V on both sides must peel")
    }
}

// ── Null guards ────────────────────────────────────────────────────────

[test(description = "peel_tuple_field_read rejects a null expression")]
def test_null_does_not_match(t : T?) {
    var e : Expression?
    t |> success(!peel_tuple_field_read(e, "g", 0), "null input must not match")
}
