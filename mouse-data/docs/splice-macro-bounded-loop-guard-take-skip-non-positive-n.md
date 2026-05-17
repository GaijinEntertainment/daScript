---
slug: splice-macro-bounded-loop-guard-take-skip-non-positive-n
title: When emitting bounded-loop counter guards (take/skip) in a splice macro, what comparison operator matches iterator reference semantics for non-positive N — `==` or `>=`?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---


**Use `>=` for take, `> 0` for skip.** Mirrors `daslib/linq.das` iterator semantics where `take_impl` and `take_to_array` both `if (total <= 0) break` (non-positive N → take nothing) and `skip_impl` `if (total > 0) { total--; continue }` (non-positive K → naturally inert, skip nothing).

```das
// Take guard — break when limit reached.
// `>=` (not `==`): for N <= 0, takenCount=0 already satisfies `0 >= -N`, breaks
// on first iteration → matches reference "non-positive → take 0".
// For N > 0, `>=` agrees with `==` since takenCount increments by 1 from 0.
prefixed |> push <| qmacro_expr() {
    if ($i(takeCountName) >= $e(takeLimit)) {
        break
    }
}

// Skip guard — decrement and continue while there's skip remaining.
// `> 0` strict: K <= 0 → condition false → no decrement, element passes through.
// Structurally identical to skip_impl in linq.das, so it inherits its semantics
// for free including the non-positive case.
prefixed |> push <| qmacro_expr() {
    if ($i(skipName) > 0) {
        $i(skipName)--
        continue
    }
}
```

**Why `==` is wrong for take**: when N is negative, `takenCount` (which starts at 0 and only ever increments) **never** satisfies the equality, so the spliced loop happily takes every element of the source. That's a silent divergence from `take_impl` / `take_to_array` / `take(arr, total)`-style overloads which all break on `total <= 0`. It looks correct in tests with positive N and only blows up when a user threads a computed `take(n - m)` that goes negative.

**Concrete bug + fix**: caught on PR #2697 by Copilot's pull-request reviewer. Fix commit `969343ae0`: one-character `==` → `>=` in `daslib/linq_fold.das::wrap_with_skip_take` + 4 pinning tests covering `take(-1)`, `take(0)`, `skip(-1)`, `skip(0)` in `tests/linq/test_linq_fold.das`.

**Why skip didn't need fixing**: pure luck of having reached for the natural `> 0` guard at emission time instead of e.g. counting down to `== 0`. The two paths are not symmetrical — take counts UP from 0 to N (so any non-positive limit needs `>=`), while skip counts DOWN from K to 0 (so the natural `> 0` covers both positive and non-positive K).

**Lesson for any new bounded-loop emission**: pick the guard that short-circuits on the first iteration when the bound is non-positive. `==` against a counter that starts at 0 and increments fails this. Don't trust positive-N tests to prove the emission is sound.

See [[chained-select-splice-bind-via-clone-assign-universal]] for the broader splice emission patterns and [[macro-planner-named-marker-arms-leave-room-for-future-modes]] for the chain-recognition structure that hosts these guards.

## Questions
- When emitting bounded-loop counter guards (take/skip) in a splice macro, what comparison operator matches iterator reference semantics for non-positive N — `==` or `>=`?
