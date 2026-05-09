---
slug: why-does-expr-left-give-me-an-expression-const-i-can-t-reassign-or-pass-to-a-non-const-helper
title: Why does `expr.left` give me an `Expression? const` I can't reassign or pass to a non-const helper?
created: 2026-05-08
last_verified: 2026-05-08
links: []
---

When a function parameter is **not** declared `var`, gen2 propagates const through every accessor. So in `def override preVisitExprOp2(expr : ExprOp2?)`, the type of `expr.left` is `Expression? const` — a constant pointer. Two failure modes:

1. **Helper signature mismatch**: passing `expr.left` to `def helper(e : Expression?)` fails with `error[30187]: incompatible argument 2 ... ast_core::Expression const?& vs ast_core::Expression? const`.
2. **Local reassignment**: `var x = expr.left ; x = (x as ExprRef2Value.subexpr)` fails with `error[30915]: can only copy compatible type ; ... -const = ... const&`.

Two fixes, picked by what the call site needs:

```
// (a) If the helper only READS the expr, declare its param as `Expression? const`:
def expr_equal_struct(a : Expression? const ; b : Expression? const) : bool { ... }

// (b) If the *visitor* needs mutable access (writing to fields), take the
// override param as `var`. Then expr.left is non-const all the way down:
def override preVisitExprOp2(var expr : ExprOp2?) : void { ... }
```

For the local-reassignment case, prefer chaining helpers that take/return `Expression? const`:

```
def peel_ref2value(e : Expression? const) : Expression? const {
    if (e != null && e is ExprRef2Value) {
        return (e as ExprRef2Value.subexpr)
    }
    return e
}
let lhs = self->peel_ref2value(expr.left)   // lhs is `Expression? const`, immutable
```

Avoid `unsafe(reinterpret<Expression?>(...))` to strip const — it works but it's the wrong tool. Reach for it only when you genuinely cannot make the parent `var`.

Related: visitor override signatures (`preVisitExprOp2`, `preVisitExprIfThenElse`, etc.) are usually **non-`var`** by convention, even though daslib's own visitors sometimes use `var` selectively when they mutate. Match the existing pattern in the file you're editing.

## Questions
- Why does `expr.left` give me an `Expression? const` I can't reassign or pass to a non-const helper?
- expr.left const propagation
- ast_core::Expression const?& vs Expression? const error 30187
- why can't I assign expr.left to a local var
- visitor parameter const stripping AST
- error 30915 can only copy compatible type Expression
