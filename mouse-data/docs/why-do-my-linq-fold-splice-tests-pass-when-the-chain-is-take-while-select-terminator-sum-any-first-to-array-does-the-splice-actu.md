---
slug: why-do-my-linq-fold-splice-tests-pass-when-the-chain-is-take-while-select-terminator-sum-any-first-to-array-does-the-splice-actu
title: Why do my linq_fold splice tests pass when the chain is take_while + _select + terminator (sum / any / first / to_array) — does the splice actually fire?
created: 2026-05-21
last_verified: 2026-05-21
links: []
---

**No — those tests fall through to the tier-2 eager bridge. The splice doesn't fire.**

The canonical chain order is `where → skip → skip_while → take_while → take`. `select` may appear ANYWHERE in the prefix (before the first range op), but is **forbidden after any range op** because the predicate-driven ranges (`skip_while` / `take_while`) require seeing the source element / source tuple. Array side bails on `seenSelect` at [daslib/linq_fold.das:1615](daslib/linq_fold.das#L1615) and [:1623](daslib/linq_fold.das#L1623). Decs side bails inside `extract_decs_ranges` when the suffix walk hits a non-range op (returns `null` → `plan_decs_unroll` returns `null` → eager bridge takes over).

**Why this trap bites:** A chain like `_take_while(_.val < 3)._select(_.val).sum()` looks like a natural splice candidate, and the test passes because the tier-2 eager bridge (`take_while_to_array → select → sum`) computes the same answer. But the splice planner returned `null`, so the new code path you added (e.g., explicit-state routing for `take_while + any/all/contains` in `emit_decs_early_exit`) gets ZERO coverage.

**Always verify splice firing via `ast_dump --mode source`** — the splice emits `for_each_archetype(_find)` directly with predicates inline; tier-2 emits the user chain as eager `take_while_to_array` + `select` + terminator wrapped in `to_sequence`. Caught on PR #2772 by Copilot.

**Rewrite recipes for take_while/skip_while tests that actually splice:**

```das
// any/all/contains: predicate over the SOURCE tuple (skip _select entirely)
_take_while(_.val < 3)._any(_.val == 2)           // splices
_take_while(_.val < 3)._all(_.val < 5)            // splices
// contains: source-tuple equality is undefined; use any/all
// (contains shares the same useExplicitState branch in emit_decs_early_exit)

// first / to_array: return source tuples; test field access on the returned value
_take_while(_.val < 3).first().val                // splice + extract scalar at call site
_take_while(_.val < 3).to_array()                 // array<tuple>; test got[i].val

// min_by / max_by: key extractor over source tuple (no _select needed)
_take_while(_.val < 3)._min_by(_.val)             // splices (emit_decs_min_max_by)

// sum / min / max / average: NO valid splice shape on decs.
// _select-after-take_while bails (suffix), _select-before-take_while bails
// (seenSelect). For now, drop these or document they hit the eager bridge.
```

**AST shape gate to lock the splice in:**
```das
t |> equal(describe_count(body_expr, "to_sequence"), 0, "splice fires, NOT tier-2")
t |> equal(describe_count(body_expr, "for_each_archetype_find"), 1, "take_while-stop routes to _find")
t |> equal(describe_count(body_expr, "decs_found"), 3, "explicit-state var + set + return")
```

See PR #2772 commit `e2228a5f4` for the full rewrite, and [[in-a-fold-splice-planner-how-do-i-extract-a-select-call-s-element-type-when-the-chain-is-wrapped-in-fold-and-cll-0-type-firsttyp]] for related splice-planner conventions.

## Questions
- Why do my linq_fold splice tests pass when the chain is take_while + _select + terminator (sum / any / first / to_array) — does the splice actually fire?
