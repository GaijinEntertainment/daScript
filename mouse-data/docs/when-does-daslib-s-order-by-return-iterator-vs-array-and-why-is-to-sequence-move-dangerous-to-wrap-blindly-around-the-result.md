---
slug: when-does-daslib-s-order-by-return-iterator-vs-array-and-why-is-to-sequence-move-dangerous-to-wrap-blindly-around-the-result
title: When does daslib's `order_by` return iterator vs array? And why is `to_sequence_move()` dangerous to wrap blindly around the result?
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

**`order_by` has 2 overloads in daslib/linq.das (and same for `order` / `order_descending` / `order_by_descending`):**
- `def order_by(var a : iterator<auto(TT)>; key) : iterator<TT -const -&>` ([daslib/linq.das:405](daslib/linq.das#L405)) — iterator in → **iterator out**
- `def order_by(a : array<auto(TT)>; key) : array<TT -const -&>` ([daslib/linq.das:412](daslib/linq.das#L412)) — array in → **array out**

Return-type-mirrors-source-type. Caller picks the overload by what they pass in.

Contrast with **`top_n_by` / `top_n*` (PR #2707): both overloads return `array<T>` regardless of input shape** — iterator source still returns array. No `top_n_by_iterator` form.

**Practical implication for splice planners:** `to_sequence_move()` is array-only — it crashes on iterator. So if you have a planner that emits a chain call and then wraps with `to_sequence_move()` when the outer expression was iterator-typed, gate the wrap on the *emission's* output type, not the outer-chain type:

```
let emissionIsArray = takeExpr != null /* top_n* always array */ || top._type.isGoodArrayType
if (needIterWrap && emissionIsArray) {
    emission = qmacro($e(emission).to_sequence_move())
}
```

Without the gate, a bare `order_by(iter, key)` emission gets `.to_sequence_move()` glued on its iterator result → compile error.

This was a latent bug in `plan_order_family` (PR #2712 round 1) caught by Copilot review; my tests all used `each(arr)` sources where `peel_each` succeeded and yielded array, masking the iterator-source path.

## Questions
- When does daslib's `order_by` return iterator vs array? And why is `to_sequence_move()` dangerous to wrap blindly around the result?
- What's the difference between `order_by` and `top_n_by` return types for iterator vs array source?
- Why does my splice planner's `.to_sequence_move()` wrap compile-fail on iterator sources?
- What's the array-only constraint on `to_sequence_move` in linq?

## See also
- `daslib-order-family-and-top-n-overload-shapes` — companion table

## Questions
- When does daslib's `order_by` return iterator vs array? And why is `to_sequence_move()` dangerous to wrap blindly around the result?
