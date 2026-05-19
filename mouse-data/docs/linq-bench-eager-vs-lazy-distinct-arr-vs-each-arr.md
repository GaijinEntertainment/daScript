---
slug: linq-bench-eager-vs-lazy-distinct-arr-vs-each-arr
title: Why does my linq benchmark show wildly different `m3` numbers between `arr |> distinct() |> ...` and `each(arr) |> distinct() |> ...` — is `distinct` eager on arrays?
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

**Yes — `distinct(arr)` is the eager array-shape overload; `distinct()` on an iterator is the lazy generator.** Different code paths in `daslib/linq.das`, very different per-element cost. Benchmark authors MUST match source shapes across the m1/m3/m3f lanes for apples-to-apples comparison.

### The two overloads

`daslib/linq.das` exposes two `distinct` shapes:

1. **Array-shape (eager)** — `def distinct(arr : array<auto(TT)>) : array<TT>` (`linq.das` ~line 668 area, see also `set` lane). Walks the whole array, builds a `table<TT>` of seen keys, returns a new array. O(N) work UP FRONT regardless of what consumes it.
2. **Iterator-shape (lazy generator)** — `def distinct(it : iterator<auto(TT) const -&>) : iterator<TT -&>` (also `linq.das:668` area). Returns a generator that yields the next unseen value on demand. O(1) per yield, stops on the next `break` from the consumer.

`take(N)` after the lazy form consumes N yields then stops. `take(N)` after the eager form is a slice of an already-materialized full array.

### The benchmark trap

The bug in `benchmarks/sql/distinct_take.das` (caught during PR #2721):

```das
// m3 — eager: distinct(arr) materializes ALL distinct values, THEN take(3)
let rows <- (arr |> _select(_.brand) |> distinct() |> take(TAKE_N) |> to_array())

// m3f — lazy: each(arr) → iterator-distinct, take(3) stops on the 3rd yield
let rows <- _fold(each(arr)._select(_.brand).distinct().take(TAKE_N).to_array())
```

Both compile, both produce the right answer (`take(3)` of N distinct), but m3 walks the entire 100K-row source while m3f exits at the 3rd distinct (position ~3 with cycling brands). `m3` reports something like 30 ns/op; `m3f` reports 0 ns/op. The headline "lazy beats eager 30:0" is real but it's NOT a splice-vs-cascade comparison — it's an eager-vs-lazy comparison with the splice as confounder.

### The fix

Match source shapes:

```das
// m3 — lazy (matches m3f's source)
unsafe {
    let rows <- (each(arr) |> _select(_.brand) |> distinct() |> take(TAKE_N) |> to_array())
}
```

Now both lanes share the iterator-form `distinct()`. The remaining delta is purely splice vs generator-dispatch overhead.

### Why `each(arr)` needs `unsafe`

Calling `each(arr)` outside a for-loop is gated on `unsafe { ... }` — the iterator escapes the scope where the array's lifetime is anchored. Inside `for (x in each(arr))` it's auto-anchored and no `unsafe` is needed. See [[why-does-each-arr-fail-with-unsafe-when-not-source-of-for-loop-outside-a-for-and-what-s-the-alternative-in-a-linq-chain]].

### How to spot this in any new benchmark

- Two lanes both call `op |> distinct()` but one passes `arr` and the other passes `each(arr)` / iterator-comprehension / `to_sequence(arr)`. Mismatch.
- The "slow" lane's numbers don't scale with `TAKE_N` (it's doing full-source work regardless). The "fast" lane scales linearly with `TAKE_N` (early-exit kicks in).
- `dasperf` shows the slow lane's hot frame inside `_distinct_impl_array` (eager) vs `_distinct_impl_iterator` / generator step (lazy).

### Affected operators

Same shape issue exists for `reverse(arr)` (eager copy) vs `reverse(iterator)` (lazy yield in reverse — but needs a buffer internally), and any other op with both array and iterator overloads. When in doubt: check `daslib/linq.das` for the operator name to see if there are two overloads.

See [[when-does-peel-each-in-daslib-linq-fold-das-unwrap-each-x-to-x-and-what-s-the-design-rationale]] for how the splice planner normalizes `each(arr)` → `arr` for the source-binding type lookup.

## Questions
- Why does my linq benchmark show wildly different `m3` numbers between `arr |> distinct() |> ...` and `each(arr) |> distinct() |> ...` — is `distinct` eager on arrays?
