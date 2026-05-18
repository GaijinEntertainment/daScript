---
slug: why-does-order-by-v-v-compile-fail-with-function-function-while-order-by-works
title: Why does `_order_by(@@(v) => -v)` compile-fail with "_::<(function, function)" while `_order_by(-_)` works?
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

**`_order_by(EXPR)` is a daslib comprehension macro — `EXPR` is a key-extraction expression using the `_` placeholder for the element, NOT a lambda or function pointer.**

The macro expands `_order_by(EXPR)` to roughly `order_by(iter, $($) => EXPR)` — it wraps EXPR into a block that produces the key per element. So:

- ✅ `_order_by(-_)`         → emits `order_by(iter, $($) => -$)` → key = `-element`
- ✅ `_order_by(_.price)`    → emits `order_by(iter, $($) => $.price)` → key = field access
- ✅ `_order_by(_ * 2)`      → emits `order_by(iter, $($) => $ * 2)`
- ❌ `_order_by(@@(v) => -v)` → emits `order_by(iter, $($) => @@(v) => -v)` — the inner block returns a FUNCTION POINTER, not a value. Then `_::less(key(v1), key(v2))` tries to compare two function pointers → error 30341.

Same rule for `_order_by_descending`, `_select`, `_where`, and every other comprehension form in `daslib/linq.das` that takes a `_`-placeholder expression.

The `top_n_by(arr, K, key)` direct call is different — `key` IS a function/lambda parameter. There you pass `@@(v : int) => -v` or `$(v : int) => -v`. The two forms only collide when you write `_order_by(@@...)` (comprehension form taking a key expression but getting a function literal).

**How to spot the diagnostic:** error `30341: no matching functions or generics: _::<(function<...>, function<...>)` with the call stack pointing into `linq.das` `order_by` instantiation. The "comparing two functions" is the tell.

## Questions
- Why does `_order_by(@@(v) => -v)` compile-fail with "_::<(function, function)" while `_order_by(-_)` works?
- What's the difference between `_order_by(EXPR)` (comprehension form) and `order_by(arr, key)` (direct call) for the key argument?
- What does the `_` placeholder mean in daslib comprehension forms like `_select` / `_where` / `_order_by`?
