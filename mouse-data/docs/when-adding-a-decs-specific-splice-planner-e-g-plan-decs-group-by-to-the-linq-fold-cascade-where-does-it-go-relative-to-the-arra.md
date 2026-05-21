---
slug: when-adding-a-decs-specific-splice-planner-e-g-plan-decs-group-by-to-the-linq-fold-cascade-where-does-it-go-relative-to-the-arra
title: When adding a decs-specific splice planner (e.g. plan_decs_group_by) to the linq_fold cascade, where does it go relative to the array-side planner of the same shape (e.g. plan_group_by)?
created: 2026-05-21
last_verified: 2026-05-21
links: []
---

**Pattern:** decs-specific planner MUST run BEFORE the array-side planner of the same shape. The decs planner has the stricter requirement (`extract_decs_bridge(top)` must succeed) so it won't grab non-decs chains; running it second causes the generic planner to match on the eager-bridge invoke and emit correct-but-slow code that wraps the FULL eager bridge — losing the entire perf win silently.

### The trap

```das
res = plan_group_by(call.arguments[0])    // ← matches on eager-bridge iterator
if (res != null) return res
res = plan_decs_group_by(call.arguments[0])  // ← NEVER REACHED for decs chains
```

`plan_group_by` accepts any iterator/array source via `peel_each(top)` then iterates with `for (it in src) { ... }`. When `top` is the from_decs_template eager-bridge invoke (which returns `iterator<tuple<...>>` via `to_sequence(res)`), plan_group_by happily matches: it binds `srcName` to the eager-bridge invoke, emits the state-table walk around it, and `finalize_emission_stmts(top, srcName, ...)` wraps the whole thing. Tests pass (functional correctness preserved), BUT the eager bridge's `array<tuple>` materialization + `to_sequence` + iterator walk all still happen — that's the WHOLE thing Slice 5+ was designed to skip.

### How to spot it

`ast_dump --mode source` on a target function. If you see this shape:

```
return <- invoke($(var source : iterator<tuple<...>>) : ... {
    var inscope tab : ...
    for (it in source) { ... }              // ← array-side group_by loop
}, invoke($() : iterator<tuple<...>> {       // ← eager bridge passed as source
    var res : array<tuple<...>>
    for_each_archetype(...) { for (...) { push(...) } }
    return <- to_sequence(res)               // ← THE smoking gun
}))
```

…then the decs planner never fired. The presence of `to_sequence` + `var res : array<tuple<...>>` + `to_sequence(res)` is the diagnostic — your splice's AST shape gate should assert `describe_count(body, "to_sequence") == 0` to catch this regression.

The correct splice shape (decs planner fired) is a SINGLE invoke wrapping `var inscope tab + var dummy + for_each_archetype(req, erq, $(arch) { for (iters in get_ro) { tup_bind; key + uk + table_update } }) + table walk + return buf` — no nested invoke, no `to_sequence`, no `to_sequence_move`, no intermediate `var res`.

### Why the array-side planner doesn't bail

`extract_decs_bridge` only fires when `top` matches the specific eager-bridge shape (`invoke($() { var res; for_each_archetype(...); return <- to_sequence(res) })`). The array-side planner has no such guard — it just needs an iterator source, which the bridge invoke trivially provides. So unless the decs planner runs first, the array planner WILL win.

### Cascade ordering rule

For any "X on decs" splice in `daslib/linq_fold.das:LinqFold.visit`:

```das
res = plan_decs_<X>(call.arguments[0])    // ← decs first (strict guard)
if (res != null) return res
res = plan_<X>(call.arguments[0])         // ← array-side fallback
if (res != null) return res
```

Same logic applies to future Slice 5c/5d/5f (reverse/distinct/order_by on decs) — each new decs splice MUST be inserted BEFORE its array-side equivalent in the cascade.

### Regression guard pattern

Every decs splice's AST shape gate should include:

```das
t |> equal(describe_count(body_expr, "to_sequence"), 0, "<op> splice must NOT fall to tier-2 to_sequence (cascade ordering)")
t |> equal(describe_count(body_expr, "for_each_archetype"), 1, "exactly one for_each_archetype (no eager-bridge wrapping)")
```

The `to_sequence == 0` assertion catches both tier-2 cascade fall-through AND the cascade-ordering trap above (they leave the same fingerprint).

Discovered 2026-05-20 implementing Slice 5e (plan_decs_group_by on PR #2773-to-be). Initial cascade placement after `plan_group_by` → all parity tests passed, AST shape gate caught the missing splice. Fix: swap the order, 6 line change. See [[macro-planner-named-marker-arms-leave-room-for-future-modes]] for the broader marker-arms strategy this complements.

## Questions
- When adding a decs-specific splice planner (e.g. plan_decs_group_by) to the linq_fold cascade, where does it go relative to the array-side planner of the same shape (e.g. plan_group_by)?
