---
slug: in-a-decs-splice-planner-plan-decs-unroll-when-wrapping-per-element-range-guards-take-skip-and-a-where-chain-around-the-same-bod
title: In a decs splice planner (plan_decs_unroll), when wrapping per-element range guards (take/skip) and a where_ chain around the same body, which wrap comes first — ranges or chain?
created: 2026-05-20
last_verified: 2026-05-20
links: []
---

**Ranges INSIDE chain. Chain wrap goes around the range-wrapped perElement so the `where_` predicate is the outermost gate.**

Concretely: in each `emit_decs_*` function that supports both ranges and chain ops:

```das
// WRONG — ranges fire BEFORE where_ filters, so take(N) counts UNFILTERED elements
var body = wrap_decs_chain(perElement, chainInfo, calls, intermediateEnd, at)
body = wrap_inner_for_with_decs_ranges(body, rangeInfo, skipName, takeCountName)

// RIGHT — range guards wrap perElement first, then where_ wraps the whole thing
perElement = wrap_inner_for_with_decs_ranges(perElement, rangeInfo, skipName, takeCountName)
var body = wrap_decs_chain(perElement, chainInfo, calls, intermediateEnd, at)
```

The final inner-loop shape becomes:
```
for (e in arch) {
    var tup = (...)
    if (where_pred) {        // ← outermost: where filter first
        if (takeC >= N) return true
        if (skipN > 0) { skipN--; continue }
        takeC++
        perElement (e.g. acc++)
    }
}
```

**Why this matters:** User code `_where(p).take(2)` is "take 2 AFTER filter" — standard linq/SQL semantics. If you wrap ranges OUTSIDE chain, the take counter ticks on every element regardless of where_, and the cap is hit before 2 matching elements are seen.

**Symptom:** `test_unroll_where_take_count_parity` (or any where+take test) fails with a too-small count. Caught when `_where(_.flag == 1).take(2).count()` on a 7-row fixture (3 flag==1 rows: 1,3,5) returned 1 instead of 2.

**Mirror:** array-side `plan_loop_or_count` uses the same order:
```das
wrap_with_ranges(perMatchStmts, ...)            // prefix range guards onto stmts
var loopBody = wrap_with_condition(stmts_to_expr(perMatchStmts), whereCond)  // wrap the whole thing in if(where)
```

So `wrap_with_condition(wrap_with_ranges(...))` array-side is the analog of "ranges wrap perElement, then chain wraps the result" decs-side.

**Cross-archetype behavior:** the counters MUST be hoisted ABOVE `for_each_archetype` (in the invoke prelude) so they survive across archetypes — take(5) means "5 total entities across all archetypes", not "5 per archetype". The `for_each_archetype_find` `: bool` block returns `true` from the inner `return true` to stop the archetype walk when the cap is hit. Skip-only chains don't need _find — plain `for_each_archetype` is fine since skip never short-circuits.

See PR #2770 for the canonical implementation in linq_fold.das (Slice 5a).

## Questions
- In a decs splice planner (plan_decs_unroll), when wrapping per-element range guards (take/skip) and a where_ chain around the same body, which wrap comes first — ranges or chain?
