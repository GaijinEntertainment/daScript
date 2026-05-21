---
slug: in-a-linq-fold-chain-recognizer-how-do-i-distinguish-bare-count-from-the-predicate-overload-count-pred-before-popping-it-as-the
title: In a linq_fold chain recognizer, how do I distinguish bare count() from the predicate overload count(pred) before popping it as the splice terminator?
created: 2026-05-21
last_verified: 2026-05-21
links: []
---

Both forms parse as the same `count` `ExprCall` with `name == "count"`. The dispatch is by **argument count**: `count(src)` has 1 argument (the source iterator), `count(src, pred)` has 2.

When you're peeling a trailing `count` off the chain to recognize it as the splice terminator, **check `length(arguments) == 1`** before consuming. Otherwise you silently miscompile `count(pred)` chains by dropping the predicate and emitting a bare count.

```das
// inside plan_X — after recognizing the group_by / source shape:
let lastName = calls.back()._0.name
if (lastName == "count" && length(calls.back()._0.arguments) == 1) {
    terminatorName = lastName
    calls |> pop
}
// chain still contains count(pred) if length was 2 — cascade handles it
```

Same trap shape applies to other 2-arg-overloaded terminators that splice planners pop: `first(src, pred)`, `last(src, pred)`, `any(src, pred)`, `all(src, pred)`, `single(src, pred)`. `count` is the most common offender because the predicate form is the natural way to write filtered counts (instead of `_where(pred).count()`).

Reference fix: PR #2775 commit `2a16ab408` (Slice 5e, Copilot review #2). Both `plan_group_by` (array source) and `plan_decs_group_by` (decs bridge source) had the same bug — the recognizer popped `count` unconditionally and the bench/test suite missed it because every group_by test used bare `.count()`. Copilot caught it. Fix is two lines in each planner; regression test added in `tests/linq/test_linq_fold.das::test_group_by_count_predicate_cascades` (array) and `tests/linq/test_linq_from_decs.das` parity test (decs).

## Questions

- In a linq_fold chain recognizer, how do I distinguish bare count() from the predicate overload count(pred) before popping it as the splice terminator?
- Why does my splice planner silently break `count(pred)` chains — the count vanishes from the emitted code?
- Which terminator names overload on argument count and need an `arguments.length == 1` guard in a splice recognizer?

## See also

- [[when-adding-a-decs-specific-splice-planner-e-g-plan-decs-group-by-to-the-linq-fold-cascade-where-does-it-go-relative-to-the-arra]] — cascade ordering trap (sibling Copilot lesson from the same PR)
- [[macro-planner-named-marker-arms-leave-room-for-future-modes]] — broader chain-recognition discipline

## Questions
- In a linq_fold chain recognizer, how do I distinguish bare count() from the predicate overload count(pred) before popping it as the splice terminator?
