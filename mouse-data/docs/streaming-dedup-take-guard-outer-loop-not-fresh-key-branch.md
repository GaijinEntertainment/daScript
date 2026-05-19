---
slug: streaming-dedup-take-guard-outer-loop-not-fresh-key-branch
title: In a streaming dedup splice (distinct + take), where should the take(N) break-guard go — top of for-loop or inside the fresh-key branch?
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

**Top of the for-loop**, not inside the fresh-key branch. Inner-branch placement still scans duplicates after the Nth fresh key and gives worst-case O(source-length) on adversarial inputs; outer-branch placement gives true O(N-source-positions-until-Nth-fresh-key) early exit.

### The bug

Initial naïve emission for `each(arr) |> distinct() |> take(N) |> to_array()`:

```das
for (it in src) {
    if (<where-cond>) {
        let k = _::unique_key(<projection>)
        if (!key_exists(seen, k)) {
            // INNER take-guard — wrong for streaming exit
            if (taken >= N) { break }
            seen |> insert(k)
            buf |> push_clone(<projection>)
            taken++
        }
    }
}
```

Looks fine on `[0,1,2,3,4]` with `N=3` — breaks at position 3 on the 4th fresh key. Looks fine on cycling brand inputs where new keys arrive evenly. BUT on `[0,1,2,0,0,0,0,...,0,3]` it scans every duplicate `0` after position 3 because the break is gated on the next fresh-key arrival.

### The fix

```das
for (it in src) {
    if (taken >= N) { break }           // OUTER guard — fires on the iteration AFTER hitting N
    if (<where-cond>) {
        let k = _::unique_key(<projection>)
        if (!key_exists(seen, k)) {
            seen |> insert(k)
            buf |> push_clone(<projection>)
            taken++
        }
    }
}
```

Now on `[0,1,2,0,0,...]` with `N=3`: position 0,1,2 push; position 3 enters the loop, the outer guard fires (`taken == 3 >= 3`), break. Done.

### Subtleties

- **`>=` not `==`** matches the reference `take_impl` semantics for non-positive N (see [[splice-macro-bounded-loop-guard-take-skip-non-positive-n]]).
- **Inner guard is still semantically correct** — it just leaks performance. Both pass functional parity tests like `[1,2,3,4,5].distinct().take(3) == [1,2,3]`.
- **Per-iteration cost of the outer guard is one int compare + branch** — cheap, runs once per source element, dominated by the body anyway.
- **Don't place AFTER `taken++`** — putting `break if (taken >= N)` after the increment causes the break check to land on the NEXT iteration anyway (you've already done the work to push the Nth element, you want to exit at the top of the next iteration before processing the (N+1)th candidate). So either inside the fresh-key branch (wrong, see above) or top of the loop (right).

### Why the bench numbers didn't catch this

Headline bench (`benchmarks/sql/distinct_take.das`) uses `BRAND_COUNT=5`, `TAKE_N=3`, cycling brands. With 5 distinct brands cycling through 100K rows, positions 0,1,2 are distinct, position 3 is the first duplicate, position 4 is distinct (4th), position 5 is the next dup, etc. By position ~3 we already have 3 distinct. Outer guard fires at position 3; inner guard would fire at position 4 when the 4th fresh key arrives. Both produce m3f = 0 ns/op on this distribution. The bench can't distinguish them — but adversarial inputs can. Add a worst-case bench (`[0,1,2] + [0] * 99997 + [3]`) if you need to verify.

### Where to assert in tests

In AST-shape tests, assert `count_break_continue(body, true) >= 1` AND `count_inner_for_loops(body) == 1` (single fused loop) AND the `if (... >= N) break` statement is the FIRST statement of the loop body — not nested in the where-cond, not nested in the fresh-key branch. Walk the for-block's first child:

```das
let first = forStmt.body[0]
assert(first is ExprIfThenElse)
assert(first.cond is ExprOp2 && first.cond.op == ">=")
```

See [[group-by-first-key-wins-single-hash-tab-dummy-addr-compare]] for the sibling pattern in group_by, and [[chained-select-splice-bind-via-clone-assign-universal]] for the broader splice emission template.

## Questions
- In a streaming dedup splice (distinct + take), where should the take(N) break-guard go — top of for-loop or inside the fresh-key branch?
