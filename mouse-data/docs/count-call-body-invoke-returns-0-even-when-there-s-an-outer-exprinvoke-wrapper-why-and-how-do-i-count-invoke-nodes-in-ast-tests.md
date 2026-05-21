---
slug: count-call-body-invoke-returns-0-even-when-there-s-an-outer-exprinvoke-wrapper-why-and-how-do-i-count-invoke-nodes-in-ast-tests
title: count_call(body, "invoke") returns 0 even when there's an outer ExprInvoke wrapper — why, and how do I count invoke nodes in AST tests?
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

`count_call(expr, funcName)` in `tests/linq/test_linq_fold_ast.das:640` matches **`ExprCall` nodes** with a matching `c.func.name` — it does NOT count `ExprInvoke` nodes. `invoke(...)` in daslang AST is `ExprInvoke` (its own node class), not `ExprCall("invoke")`. So `count_call(body_expr, "invoke")` always returns 0 regardless of how many `ExprInvoke` wrappers exist in the subtree.

When you need to count invoke wrappers — e.g. proving a peeled aggregate body emits ZERO per-element block invokes while only the outer `_fold` splice wrapper remains — write a sibling helper:

```
def count_invoke_nodes(expr : Expression?) : int {
    if (expr == null) return 0
    var n = 0
    if (expr is ExprInvoke) {
        n ++
    }
    // ... recurse into ExprBlock/ExprFor/ExprIfThenElse/ExprOp2/ExprCall/
    // ExprMakeBlock/ExprInvoke/ExprReturn/ExprOp1/ExprLet (same shape as count_call)
    return n
}
```

Then assert `equal(1, count_invoke_nodes(body_expr))` for peeled (only the outer wrapper) vs `>= 2` for runtime-invoke fallback (outer wrapper + per-element block invoke).

Canonical example: PR-F's `test_aggregate_splices_peeled` uses `count_invoke_nodes` to prove the aggregate splice peeled the block body inline. See `tests/linq/test_linq_fold_ast.das` around the `// ── Phase 3+ terminal-walk lane` section.

**Don't confuse with the tier-1/tier-2 distinguisher** (see [[how-do-i-write-an-ast-shape-test-that-distinguishes-tier-1-splice-from-tier-2-cascade-in-fold-exprinvoke-matches-both]]) — that's about `body_expr is ExprInvoke` matching both. This is about the helper function's traversal: `count_call` walks INTO ExprInvoke nodes' arguments but never counts the wrapper itself.

## Questions
- count_call(body, "invoke") returns 0 even when there's an outer ExprInvoke wrapper — why, and how do I count invoke nodes in AST tests?
