options gen2
options indenting = 4
options strict_smart_pointers = true

module match_solver shared private

//! Compile-time pattern-shape helpers for ``daslib/match``.

require daslib/ast_boost

[macro_function]
def public peel_r2v(var e : Expression?) : Expression? {
    //! Strip post-typer ``ExprRef2Value`` wrappers: a typed pattern or value reads as the source spelled it.
    while (e != null && e is ExprRef2Value) {
        e = (e as ExprRef2Value).subexpr
    }
    return e
}

[macro_function]
def public peel_r2v(e : Expression const?) : Expression const? {
    //! Pointee-const flavor — same peel, preserves the caller's const view.
    var c : Expression const? = e
    while (c != null && c is ExprRef2Value) {
        c = (c as ExprRef2Value).subexpr
    }
    return c
}

[macro_function]
def public is_rest(p : Expression const?) : bool {
    let x = peel_r2v(p)
    return x is ExprTag && (x as ExprTag).name == "..."
}
