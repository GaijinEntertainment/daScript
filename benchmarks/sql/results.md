# Benchmarks — SQL / Array / Decs comparison

Generated 2026-05-27 from `ee2f1fe77` (PR F4.3 — `order_distinct_take` decs lane via DecsBrand fixture). Matrix refreshed for the new m4 cell: `_order_by(K) |> distinct() |> take(N) |> to_array` on a decs source now reports honest cost via a duplicate-prone single-int template. The m4 cell lands at 95.0 ns/op INTERP / 74.8 JIT vs m3f at 15.9 / 2.1 — the ratio is NOT apples-to-apples (see `order_distinct_take` note below). Other drift is bench-suite ordering noise: `sort_first` m4 INTERP drifted +28% on the F4.2 baseline, now drifts back -22% on this run (long-running pattern on this lane); `select_where_order_take` ±6-8% INTERP noise; sub-nanosecond JIT drifts at measurement floor.
Fixture size: n = 100 000 (cars), 100 dealers, 5 brands. Each row is
one bench family in `benchmarks/sql/`; columns are nanoseconds per
logical operation. `—` marks an intentionally absent lane — see
"Notes on missing lanes" below.

The benchmarked chain shapes are summarised in
`benchmarks/README.md` and the splice arms each chain fires are
catalogued in `doc/source/reference/linq_fold_patterns.rst`.

- **SQL** — `_sql` macro over an in-memory SQLite database
  (`with_sqlite(":memory:")`).
- **Array** — `_fold` over `each(arr)` chain, in-memory `array<Car>`.
- **Decs** — `_fold` over `from_decs_template(type<DecsCar>)`,
  per-archetype walk.
- **Decs vs Array** — ratio `decs_ns / array_ns`. Values below 1× mean
  decs wins; values above mean array wins.

Sub-nanosecond cells (`0.00`) reflect early-exit terminators that exit
before the timer resolution can measure them — they should be read as
"effectively free."


## INTERP

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | Decs vs Array |
|---|---:|---:|---:|---:|
| `aggregate_match` | 35.3 | 6.1 | 5.9 | 0.97× |
| `all_match` | 28.1 | 3.5 | 3.5 | 1.00× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 30.5 | 5.9 | 8.8 | 1.49× |
| `bare_order_where` | 279.7 | 116.5 | 124.8 | 1.07× |
| `chained_select_collapse` | — | 17.9 | 17.7 | 0.99× |
| `chained_where` | 37.0 | 6.7 | 7.1 | 1.06× |
| `contains_match` | 0.00 | 2.2 | 1.4 | 0.64× |
| `count_aggregate` | 30.0 | 4.2 | 4.1 | 0.98× |
| `decs_count_bare_pred` | — | — | 4.2 | — |
| `distinct_by_count` | 41.8 | 15.8 | 15.8 | 1.00× |
| `distinct_by_order_take` | — | 21.8 | 23.4 | 1.07× |
| `distinct_count` | 41.7 | 16.0 | 15.9 | 0.99× |
| `distinct_count_pred` | — | 16.2 | 16.2 | 1.00× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 171.9 | 30.3 | 30.2 | 1.00× |
| `groupby_count` | 142.9 | 19.4 | 19.2 | 0.99× |
| `groupby_first` | — | 18.5 | 19.2 | 1.04× |
| `groupby_having_count` | 142.4 | 19.3 | 19.2 | 0.99× |
| `groupby_having_hidden_sum` | 175.2 | 24.1 | 24.4 | 1.01× |
| `groupby_having_post_where` | 172.9 | 18.5 | 18.7 | 1.01× |
| `groupby_max` | 174.2 | 25.0 | 25.6 | 1.02× |
| `groupby_min` | 176.3 | 25.0 | 25.2 | 1.01× |
| `groupby_multi_reducer` | 190.1 | 32.5 | 33.0 | 1.02× |
| `groupby_select_order` | 169.9 | 18.6 | 18.7 | 1.01× |
| `groupby_select_sum` | 200.9 | 36.5 | 35.9 | 0.98× |
| `groupby_sum` | 172.6 | 18.8 | 18.8 | 1.00× |
| `groupby_where_count` | 75.9 | 14.5 | 15.0 | 1.03× |
| `groupby_where_sum` | 86.9 | 14.2 | 14.6 | 1.03× |
| `indexed_lookup` | 1468.7 | 204087.0 | 475.9 | 0.00× |
| `join_count` | 38.5 | 121.4 | 64.1 | 0.53× |
| `join_groupby_count` | — | 186.2 | 91.0 | 0.49× |
| `join_groupby_to_array` | — | 216.7 | 90.7 | 0.42× |
| `join_select` | — | 149.4 | 85.2 | 0.57× |
| `join_where_count` | 39.4 | 148.9 | 74.4 | 0.50× |
| `last_match` | 0.00 | 5.8 | 14.1 | 2.43× |
| `long_count_aggregate` | 30.0 | 4.2 | 4.1 | 0.98× |
| `max_aggregate` | 31.4 | 6.0 | 6.9 | 1.15× |
| `min_aggregate` | 31.5 | 6.2 | 6.9 | 1.11× |
| `order_distinct_take` | — | 15.9 | 95.0 | 5.97× |
| `order_reverse_normalized` | 38.6 | 16.3 | 20.0 | 1.23× |
| `order_take_desc` | 38.5 | 16.2 | 19.9 | 1.23× |
| `reverse_take` | 0.10 | 0.00 | 10.2 | — |
| `reverse_take_select` | 0.00 | 34.3 | 48.4 | 1.41× |
| `select_count` | 0.10 | 0.00 | 2.2 | — |
| `select_where` | 195.7 | 11.3 | 19.9 | 1.76× |
| `select_where_count` | 33.3 | 5.2 | 7.5 | 1.44× |
| `select_where_order_take` | 37.1 | 13.3 | 15.9 | 1.20× |
| `select_where_sum` | 37.3 | 7.6 | 7.6 | 1.00× |
| `single_match` | 0.00 | 2.9 | 5.5 | 1.90× |
| `skip_take` | 0.50 | 0.10 | 0.20 | 2.00× |
| `skip_while_match` | 3.5 | 5.3 | 5.3 | 1.00× |
| `sort_first` | 38.3 | 11.1 | 13.4 | 1.21× |
| `sort_take` | 38.7 | 16.4 | 20.7 | 1.26× |
| `sort_take_select` | 38.6 | 16.2 | 20.1 | 1.24× |
| `sum_aggregate` | 30.1 | 2.2 | 2.1 | 0.95× |
| `sum_where` | 33.2 | 4.3 | 4.2 | 0.98× |
| `take_count` | 3.6 | 0.20 | 0.40 | 2.00× |
| `take_count_filtered` | — | 0.20 | 0.20 | 1.00× |
| `take_sum_aggregate` | — | 0.10 | 0.10 | 1.00× |
| `take_where_count` | — | 0.10 | 0.10 | 1.00× |
| `take_while_match` | 7.9 | 2.4 | 2.4 | 1.00× |
| `to_array_filter` | 71.0 | 11.7 | 11.8 | 1.01× |
| `zip_count_pred` | — | 15.1 | — | — |
| `zip_dot_product` | — | 12.7 | 10.7 | 0.84× |
| `zip_dot_product_3arg` | — | 12.8 | — | — |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | Decs vs Array |
|---|---:|---:|---:|---:|
| `aggregate_match` | 35.2 | 0.40 | 0.70 | 1.75× |
| `all_match` | 27.9 | 0.30 | 0.20 | 0.67× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 30.5 | 1.0 | 3.6 | 3.60× |
| `bare_order_where` | 187.3 | 34.2 | 35.2 | 1.03× |
| `chained_select_collapse` | — | 2.1 | 2.1 | 1.00× |
| `chained_where` | 36.7 | 0.60 | 0.90 | 1.50× |
| `contains_match` | 0.00 | 0.20 | 0.10 | 0.50× |
| `count_aggregate` | 29.6 | 0.40 | 0.60 | 1.50× |
| `decs_count_bare_pred` | — | — | 0.60 | — |
| `distinct_by_count` | 41.4 | 2.1 | 2.1 | 1.00× |
| `distinct_by_order_take` | — | 2.7 | 3.2 | 1.19× |
| `distinct_count` | 41.5 | 2.1 | 2.1 | 1.00× |
| `distinct_count_pred` | — | 2.1 | 2.3 | 1.10× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 172.8 | 2.6 | 2.9 | 1.12× |
| `groupby_count` | 142.0 | 2.4 | 2.5 | 1.04× |
| `groupby_first` | — | 2.2 | 3.1 | 1.41× |
| `groupby_having_count` | 142.4 | 2.3 | 2.5 | 1.09× |
| `groupby_having_hidden_sum` | 176.4 | 2.5 | 2.8 | 1.12× |
| `groupby_having_post_where` | 171.4 | 2.4 | 2.7 | 1.13× |
| `groupby_max` | 175.4 | 2.4 | 2.7 | 1.13× |
| `groupby_min` | 174.0 | 2.4 | 2.7 | 1.13× |
| `groupby_multi_reducer` | 190.5 | 2.7 | 3.0 | 1.11× |
| `groupby_select_order` | 171.9 | 2.4 | 2.7 | 1.13× |
| `groupby_select_sum` | 200.4 | 3.2 | 3.7 | 1.16× |
| `groupby_sum` | 172.1 | 2.4 | 2.7 | 1.13× |
| `groupby_where_count` | 75.8 | 1.5 | 1.8 | 1.20× |
| `groupby_where_sum` | 87.5 | 1.5 | 1.8 | 1.20× |
| `indexed_lookup` | 1249.3 | 35189.7 | 105.7 | 0.00× |
| `join_count` | 38.6 | 34.5 | 13.0 | 0.38× |
| `join_groupby_count` | — | 47.5 | 21.9 | 0.46× |
| `join_groupby_to_array` | — | 56.1 | 21.9 | 0.39× |
| `join_select` | — | 41.7 | 22.7 | 0.54× |
| `join_where_count` | 39.4 | 41.5 | 22.7 | 0.55× |
| `last_match` | 0.00 | 0.50 | 1.4 | 2.80× |
| `long_count_aggregate` | 29.9 | 0.40 | 0.60 | 1.50× |
| `max_aggregate` | 31.3 | 0.60 | 0.50 | 0.83× |
| `min_aggregate` | 31.2 | 0.60 | 0.50 | 0.83× |
| `order_distinct_take` | — | 2.1 | 74.8 | 35.62× |
| `order_reverse_normalized` | 38.3 | 0.70 | 1.3 | 1.86× |
| `order_take_desc` | 38.4 | 0.70 | 1.3 | 1.86× |
| `reverse_take` | 0.00 | 0.00 | 1.1 | — |
| `reverse_take_select` | 0.00 | 8.5 | 10.5 | 1.24× |
| `select_count` | 0.10 | 0.00 | 0.00 | — |
| `select_where` | 108.0 | 4.2 | 5.6 | 1.33× |
| `select_where_count` | 33.1 | 0.40 | 0.60 | 1.50× |
| `select_where_order_take` | 36.7 | 0.70 | 1.3 | 1.86× |
| `select_where_sum` | 37.2 | 0.50 | 0.60 | 1.20× |
| `single_match` | 0.00 | 0.40 | 1.1 | 2.75× |
| `skip_take` | 0.30 | 0.00 | 0.00 | — |
| `skip_while_match` | 3.5 | 0.40 | 0.40 | 1.00× |
| `sort_first` | 38.3 | 0.40 | 1.3 | 3.25× |
| `sort_take` | 38.6 | 0.70 | 1.4 | 2.00× |
| `sort_take_select` | 38.5 | 0.80 | 1.4 | 1.75× |
| `sum_aggregate` | 30.4 | 0.40 | 0.30 | 0.75× |
| `sum_where` | 34.7 | 0.40 | 0.60 | 1.50× |
| `take_count` | 1.8 | 0.10 | 0.10 | 1.00× |
| `take_count_filtered` | — | 0.00 | 0.00 | — |
| `take_sum_aggregate` | — | 0.00 | 0.00 | — |
| `take_where_count` | — | 0.00 | 0.00 | — |
| `take_while_match` | 7.8 | 0.20 | 0.30 | 1.50× |
| `to_array_filter` | 48.4 | 3.3 | 3.4 | 1.03× |
| `zip_count_pred` | — | 0.70 | — | — |
| `zip_dot_product` | — | 0.50 | 0.50 | 1.00× |
| `zip_dot_product_3arg` | — | 0.50 | — | — |


## Notes on missing lanes (the `—` cells)

The reasons each cell is empty are also recorded as a comment in the
corresponding `.das` bench file; the bullets below quote that comment.

- **`chained_select_collapse` SQL** — `_sql` rejects `distinct() |> count()`
  as non-translatable. The equivalent SQL `COUNT(DISTINCT computed-expr)`
  isn't currently emitted by sqlite_linq's surface. By design — no follow-up.
- **`decs_count_bare_pred` SQL / Array** — covers a Theme 4 root-cause
  fix specific to the decs lane (bare `from_decs_template(...).count(P)`
  with no upstream where/select previously bailed because
  `forExpr.iteratorVariables` was unpopulated). Array-side bare `count(P)`
  was always reachable; SQL `count(P)` is covered by `count_aggregate.das`
  with a where shape. By design.
- **`distinct_by_order_take` SQL** — no faithful SQL form. "Pick one row
  per dealer, sort by price, take N" needs window functions
  (`ROW_NUMBER() OVER (PARTITION BY dealer_id ORDER BY price)` + outer
  `WHERE rn=1` + `LIMIT N`); sqlite_linq does not currently lower window
  functions. Follow-up TODO 2026-05-25.
- **`distinct_count_pred` SQL** — sqlite_linq's `_distinct_by` + 2-arg
  `count(P)` shape isn't surfaced through `_sql` (would lower as
  `COUNT(*) FILTER WHERE ...`, not currently emitted). By design.
- **`groupby_first` SQL** — no direct SQL aggregator for "first
  source-order row per group". Window functions (`ROW_NUMBER() OVER
  (PARTITION BY brand ORDER BY id)` + outer `WHERE rn=1`) would be the
  SQL equivalent; sqlite_linq does not currently lower window
  functions. Follow-up TODO 2026-05-23.
- **`indexed_lookup` Decs vs Array ratio** — the ratio is reported as
  `0.00×` because array's lane measures the unspliced linear scan
  (~204k ns/op), while decs uses `query(eid)` for O(1) lookup. The
  ratio is mathematically tiny but the comparison is between two
  fundamentally different algorithms — not a like-for-like benchmark.
- **`join_groupby_count` SQL** — sqlite_linq's `_group_by` after `_join`
  doesn't currently lower (group key column is from the joined
  projection, not a base-table column). Follow-up TODO 2026-05-25.
- **`join_groupby_to_array` SQL** — same reason as `join_groupby_count`.
- **`join_select` SQL** — sqlite_linq's `_select` after `_join` rejects
  bare `_` (only the `into` lambda's parameter names are valid
  receivers; computed projections would need to be inlined into the
  `into` lambda). By design — sqlite_linq surface limitation.
- **`order_distinct_take` Decs vs Array ratio** — m4 (95.0 INTERP /
  74.8 JIT) vs m3f (15.9 INTERP / 2.1 JIT) is NOT apples-to-apples.
  `unique_key` (`daslib/linq.das:614`) short-circuits to a direct
  return for workhorse types and falls back to `"{a}"` string
  interpolation for everything else. Array m3f operates on
  `array<int>` (workhorse — direct hash, zero allocs per element);
  decs m4 operates on `DecsBrand` (struct — one string allocation
  per element). The 6× INTERP / 36× JIT m4 overhead is the
  per-entity string-hash cost, not decs-walk vs array-walk. For a
  key-based dedup variant on decs that avoids `unique_key`'s string
  path see `distinct_by_count` (`_distinct_by(_.brand)` over decs,
  m4 = 15.8 INTERP / 2.1 JIT = parity with the array fast path).
- **`order_distinct_take` SQL** — `_sql`'s `_order_by` requires a
  `_.Field` (column-ref) key, not bare `_`. Bench operates on a
  synthesized `array<int>` (no named column to project), so the SQL
  form has no `.Field` to pass. Independent of distinct/take ordering
  — `distinct_take.das` proves `_sql` does lower `distinct() |> take(N)`
  when the source has a named column. By design — adding a SQL lane
  would need a 1-column table fixture and `_order_by(_.col)`.
- **`take_count_filtered` SQL** — by design. In SQL, LIMIT after an
  aggregate has no effect (the aggregate collapses to one row), so the
  bound-then-count shape has no faithful SQL translation. No follow-up.
- **`take_sum_aggregate` SQL** — same reason as `take_count_filtered`.
  By design, no follow-up.
- **`take_where_count` SQL** — `take(N) |> _where(P) |> count()` has no
  faithful SQL translation. In SQL, LIMIT applies after WHERE, not
  before — preserving the "first N elements, then keep matching"
  semantic would require a derived-table form that sqlite_linq does
  not currently emit. By design.
- **`zip_count_pred` SQL / Decs** — `zip` is not a relational operation
  (no SQL form) and is not naturally expressible over a single
  archetype walk (no decs form). By design, no follow-up.
- **`zip_dot_product` SQL** — `zip` is not a relational operation.
  No SQL form exists; by design, no follow-up.
- **`zip_dot_product_3arg` SQL / Decs** — same reason as `zip_count_pred`.
  This row exercises the 3-arg `zip(a, b, sel)` pre-lowering (Theme 1,
  audit 7a); a companion to `zip_dot_product` with the explicit 2-arg
  + `_select` form. By design, no follow-up.

## How to re-run

From the repo root, on macOS / Linux:

```bash
# INTERP
rm -f /tmp/bench_interp.txt
for f in benchmarks/sql/*.das; do
  bn=$(basename "$f")
  [[ "$bn" == _* ]] && continue
  echo "=== $bn ===" >> /tmp/bench_interp.txt
  build/daslang dastest/dastest.das -- --bench --test "$f" \
    >> /tmp/bench_interp.txt 2>&1
done

# JIT (-jit goes BEFORE dastest.das)
rm -f /tmp/bench_jit.txt
for f in benchmarks/sql/*.das; do
  bn=$(basename "$f")
  [[ "$bn" == _* ]] && continue
  echo "=== $bn ===" >> /tmp/bench_jit.txt
  build/daslang -jit dastest/dastest.das -- --bench --test "$f" \
    >> /tmp/bench_jit.txt 2>&1
done
```

If JIT fails to compile (e.g. `error[30341]: no matching functions or
generics: host_jit_triple()`), the `bin/daslang` binary is stale
relative to `src/builtin/module_jit.cpp` — rebuild with
`cmake --build build --config Release -j 64` and retry.
