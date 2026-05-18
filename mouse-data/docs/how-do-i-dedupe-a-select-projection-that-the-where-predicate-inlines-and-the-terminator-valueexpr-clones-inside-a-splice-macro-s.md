---
slug: how-do-i-dedupe-a-select-projection-that-the-where-predicate-inlines-and-the-terminator-valueexpr-clones-inside-a-splice-macro-s
title: how do I dedupe a select projection that the where predicate inlines AND the terminator valueExpr clones inside a splice macro so the projection evaluates once per element instead of twice
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

**The pattern:** `_select(proj) |> _where(p) |> terminator` chains splice via the where-after-select arm in `plan_loop_or_count`. Phase 3d (PR #2712) substituted `proj` INTO the predicate via peel-aware `fold_linq_cond_peel`, and the lane emitters ALSO cloned `proj` into the terminator's `valueExpr` (via `clone_expression(projection)`). Net: projection evaluated twice per element on ARRAY / ACCUMULATOR / EARLY_EXIT lanes.

**Why intermediateBinds doesn't fix it:** `intermediateBinds` (used for chained-select bind chains) gets PREPENDED inside `perMatchStmts` BEFORE `wrap_with_condition`, so it ends up INSIDE the `if (whereCond) { ... }` wrap. A bind there can't dedupe with the predicate — the predicate is the wrap's `cond`, evaluated first; if it referenced the bind var, the var wouldn't exist yet at that point.

**Fix (PR #2714):** new `preConditionStmts : array<Expression?>` planner state, spliced into the loop body OUTSIDE the if-wrap via a `prepend_precond` helper. In the `_where` arm when `seenSelect=true`:
```das
if (has_sideeffects(projection)) return null   // semantic gate — see below
if (lane != LinqLane.COUNTER) {                // COUNTER opt-out — see below
    let wbName = "`vw`{at.line}`{at.column}`{length(preCondStmts)}"
    var projType = clone_type(elementType)
    preCondStmts |> push <| qmacro_expr() {
        var $i(wbName) : $t(projType) := $e(projection)
    }
    var pvar = new ExprVar(at = at, name := wbName)
    pvar._type = clone_type(elementType)
    pvar._type.flags.ref = true
    projection = pvar
}
predicate = fold_linq_cond_peel(cll._0.arguments[1], projection)
```

Both the predicate (peel-substituted to reference `wbName`) AND `valueExpr` (cloned from the now-rewritten `projection`) reference the same single bind. Projection evaluates exactly once per element.

**Two non-obvious constraints:**

1. **The ExprVar replacement must be typed manually.** Setting `pvar._type` from `clone_type(elementType)` + `flags.ref = true` matters: an untyped `ExprVar` would propagate `auto` into `push_clone` / accumulator call sites and surface as `30165: cannot infer push_clone return type` at the typer (downstream of the splice, hard to diagnose). Don't rely on the typer to re-walk the loop's local decls to resolve the ExprVar's type from the `var wbName : $t(projType) := ...` binding — that re-resolution happens AFTER the type errors fire.

2. **`has_sideeffects` is a semantic gate, not a perf gate.** Moving an impure projection outside the if-wrap would visibly fire side effects on filter-rejected elements — bail to tier 2 cascade in that case. (NOT "double the work" — both the old and new forms run the projection twice per element on a pure projection; the dedup just turns that into one bind read + one bind read of the same var.)

3. **COUNTER lane opt-out:** count terminators never reference `valueExpr`, so the dedup brings zero benefit, and the extra per-element bind decl regresses the single-stmt fast path. Gate the bind on `lane != LinqLane.COUNTER`; benchmark proof: `select_where_count` m3f stayed at 5 ns/op pre- and post-PR with the opt-out, regressed to 7 ns/op without it.

**Files:** `daslib/linq_fold.das` `plan_loop_or_count` (preCondStmts state + where-arm bind logic), `prepend_precond` helper, signatures of `emit_accumulator_lane` / `emit_early_exit_lane` extended with `preCondStmts` param.

**Bench impact** (100K rows INTERP): `select_where_sum` m3 59 → m3f 7 ns/op (8.4×; also beats SQLite at 37 ns/op by 5.3×).

Linked: [[parser-bug-30701-tagged-block-arg-dup-check]] for the qmacro fix that surfaced during the PR, [[pr-2714-linq-splice-single-eval]] for the landed PR.

## Questions
- how do I dedupe a select projection that the where predicate inlines AND the terminator valueExpr clones inside a splice macro so the projection evaluates once per element instead of twice
