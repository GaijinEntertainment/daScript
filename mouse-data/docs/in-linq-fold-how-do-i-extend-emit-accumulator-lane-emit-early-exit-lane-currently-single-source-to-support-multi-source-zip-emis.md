---
slug: in-linq-fold-how-do-i-extend-emit-accumulator-lane-emit-early-exit-lane-currently-single-source-to-support-multi-source-zip-emis
title: In linq_fold, how do I extend `emit_accumulator_lane` / `emit_early_exit_lane` (currently single-source) to support multi-source zip emission without duplicating ~150 LOC of per-op state-building?
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

**Generalize via parallel arrays.** Change `srcName : string` → `srcNames : array<string>` and `top : Expression?` → `topExprs : array<Expression?>`. Single-source callers (plan_loop_or_count) pass 1-element arrays. Multi-source callers (plan_zip) pass N-element arrays.

**What stays the same** (source-count-agnostic):
- All per-op prelude / per-match / tail stmt building (the bulk of the helper)
- `prepend_binds` / `wrap_with_ranges` / `wrap_with_condition` / `prepend_precond`
- The `valueName` / `itName` indirection (helpers reference `$i(itName)`; for zip, caller threads `let it = (itA, itB)` via `preCondStmts` so the bind happens inside the loop body where `itName` resolves to the tuple)

**What needs to branch on `length(srcNames)`**:

1. **For-loop emission** — qmacro can't dynamically fan out iter vars (see [[in-a-qmacro-template-can-i-use-i-name-in-the-iter-var-position-of-a-multi-iterator-for-loop-e-g-for-i-a-i-b-in]]). Hardcode 1-source branch with `$i(itName)`; for 2-source branch, use literal `itA, itB` matching the caller convention:
   ```das
   if (length(srcNames) == 1) {
       bodyStmts |> push <| qmacro_expr() {
           for ($i(itName) in $i(srcNames[0])) {
               $e(loopBody)
           }
       }
   } else {
       bodyStmts |> push <| qmacro_expr() {
           for (itA, itB in $i(srcNames[0]), $i(srcNames[1])) {
               $e(loopBody)
           }
       }
   }
   ```

2. **Invoke wrap** — clone tops + derive param types + emit 1-arg or 2-arg invoke. Factor into a helper to avoid duplication across `emit_accumulator_lane` and `emit_early_exit_lane`. Pattern:
   ```das
   def private finalize_lane_emission(var topExprs : array<Expression?>; srcNames : array<string>;
                                      var bodyStmts : array<Expression?>; at : LineInfo) : Expression? {
       if (length(srcNames) == 1) {
           var topExpr = clone_expression(topExprs[0])
           topExpr.genFlags.alwaysSafe = true
           var srcParamType = invoke_src_param_type(topExprs[0])
           var res = qmacro(invoke($($i(srcNames[0]) : $t(srcParamType)) {
               $b(bodyStmts)
           }, $e(topExpr)))
           return finalize_invoke(res, at)
       }
       // 2-source: clone both tops, derive both types, emit 2-arg invoke
       ...
   }
   ```

3. **`finalize_invoke`** — was hardcoded to set `can_shadow` on `arguments[0]`. Make it loop over all block arguments:
   ```das
   var blkBlock = blk._block as ExprBlock  // var, not let — propagates non-const through field chain
   for (i in 0 .. length(blkBlock.arguments)) {
       blkBlock.arguments[i].flags.can_shadow = true
   }
   ```
   Single-source path unaffected (1-element loop ≡ original behavior). The `var` (not `let`) on `blkBlock` matters — `let` propagates const through the field-chain and the `flags` field-set fails with "can't get field 'genFlags' of auto& -const" or similar.

**What the caller (plan_zip) must do**:
- Build `preCondStmts` with `let $i(itName) = (itA, itB)` so the body bind is in scope before `whereCond` evaluation (which happens inside `wrap_with_condition`'s `if (...)` guard).
- Pre-fold the projection / predicates via `fold_linq_cond(expr, itName)` rename so they reference `itName`, not the original lambda arg name.
- For accumulator without projection on zip: bail to tier-2 (`if (projection == null) return null`) — `default<tuple<...>>` doesn't have `+=` so sum/min/max/average wouldn't typecheck anyway, but the explicit bail keeps the error in linq's normal cascade rather than the splice.

**Verified on:** `daslib/linq_fold.das` Phase 2B+ branch — 220/220 ast tests + 369/369 fold tests green after refactor + adding zip+sum/min/max/average/first/first_or_default/any/all/contains dispatch. Single-source plan_loop_or_count path unaffected (just wraps params in 1-element arrays at its call sites).

**Future:** N-arity beyond 2 needs either more arity-specific qmacro branches OR direct ExprFor AST construction. For-loop emission is the bottleneck; everything else (body construction, invoke wrap) scales cleanly via the parallel-arrays approach.

## Questions
- In linq_fold, how do I extend `emit_accumulator_lane` / `emit_early_exit_lane` (currently single-source) to support multi-source zip emission without duplicating ~150 LOC of per-op state-building?
