---
slug: in-plan-zip-splice-multi-iterator-for-loop-how-do-i-pass-the-tuple-element-ita-itb-into-a-peeled-lambda-predicate-without-the-ty
title: In plan_zip splice (multi-iterator for-loop), how do I pass the tuple element `(itA, itB)` into a peeled lambda predicate without the typer choking on `can't locate variable 't'`?
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

**Problem:** plan_zip (Phase 2B Z3) needs to splice `zip(a, b) |> where_($(t : tuple<int;int>) => t._0 > 0)` into a `for (itA, itB in srcA, srcB) { if (<peeled-pred>) { ... } }`. Tried `fold_linq_cond_peel(pred, qmacro((itA, itB)))` — substituting the lambda's `t` with the raw tuple-literal expression `(itA, itB)` — analogous to plan_loop_or_count's after-select peel where projection is substituted into the predicate. It typer-failed with `error[30838]: can't locate variable 't'` at the zip-call's location, even though the predicate body referenced `t` (not the substituted name).

**Root cause (suspected):** Inside a `_fold(...)` wrapper, the typer's resolution of `qmacro((itA, itB))` (the substitution-source expression built at macro time) doesn't carry through the post-splice apply_template re-walk cleanly. The `replaceVariablePeeling` rewrites references in the cloned predicate body, but somewhere downstream the typer re-discovers a stale `t` symbol — possibly from a wrap or projection that the peel didn't reach. The exact mechanism wasn't tracked down; the symptom is reliable across all three predicate ops (where_, take_while, skip_while).

**Fix (PR Phase 2B / #2739):** bind the tuple as a body-local `it` and use rename-based `fold_linq_cond(pred, itName)` instead of peel substitution:

```das
// In the chain walker:
var predicate : Expression? = fold_linq_cond(cll._0.arguments[1], itName)
// ...

// In the body assembly, when any chain op references `it`:
let needsItBind = whereCond != null || projection != null || skipWhileCond != null || takeWhileCond != null
if (needsItBind) {
    var preStmts : array<Expression?>
    preStmts |> reserve(2)
    preStmts |> push <| qmacro_expr() {     // nolint:STYLE012
        let $i(itName) = (itA, itB)
    }
    preStmts |> push(perBody)
    perBody = stmts_to_expr(preStmts)
}
```

The `let it = (itA, itB)` binds the tuple-of-iter-vars to a local at the top of the for-body (BEFORE the where-wrap and BEFORE the take/skip-while range guards, since those predicates also reference `it`). All chain ops then refer to `itName` via the standard rename-target path.

**Cost:** one tuple stack-copy per iteration. For workhorse element types (int int) this is 16 bytes/iter — measurable but cheap. For larger element types it scales linearly with sizeof(elem). Acceptable for the chain-fuse perf wins (vs. tier-2's `array<tuple>` materialize + roundtrip).

**Three non-obvious constraints:**

1. **`needsItBind` must be computed BEFORE the for-loop emission**, because it gates whether iter vars are emitted as `itA, itB` (referenced by the bind) or `_itA, _itB` (LINT002 underscore-prefix for unreferenced vars). Got bit by this: had underscore-prefix in the for-header AND `let it = (itA, itB)` in the body → typer said `can't locate variable 'itA'`. Fix: only underscore-prefix when `!needsItBind && isCounter && noChain`.

2. **The bind goes OUTSIDE the where-wrap, not inside.** `wrap_with_condition(stmts_to_expr(perStmts), whereCond)` wraps everything in `if (whereCond) { ... }`. If the bind were inside the if, the where predicate (which is the `if`'s condition, evaluated before the body) wouldn't see `it`. The bind must precede the whole if-wrap — same shape as `prepend_precond` in plan_loop_or_count for the chained-select dedup case.

3. **`var preStmts : array<Expression?> <- [itBind, perBody]` triggers a const-mismatch.** Array-literal init from a mix of `let` (const) and `var` (non-const) Expression refs gets inferred as `array<Expression? const>` which can't be assigned to `array<Expression?>`. Workaround: declare empty + reserve + push pattern (with `nolint:STYLE012` since reserve-then-push-twice is being flagged).

**Related:** [[in-a-fold-splice-planner-how-do-i-extract-a-select-call-s-element-type-when-the-chain-is-wrapped-in-fold-and-cll-0-type-firsttyp]] — companion issue in the same PR: select's element type needs to be pulled from the projection block's return-expr `_type`, not from `cll._0._type.firstType`.

## Questions
- In plan_zip splice (multi-iterator for-loop), how do I pass the tuple element `(itA, itB)` into a peeled lambda predicate without the typer choking on `can't locate variable 't'`?

## Questions
- In plan_zip splice (multi-iterator for-loop), how do I pass the tuple element `(itA, itB)` into a peeled lambda predicate without the typer choking on `can't locate variable 't'`?
