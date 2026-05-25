# Benchmarks — SQL / Array / Decs comparison

Generated 2026-05-25 from `2c18845d5` (master post-Theme-7 chained `_select` collapse).
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
| `aggregate_match` | 34.3 | 6.0 | 5.9 | 0.98× |
| `all_match` | 27.4 | 3.5 | 3.5 | 1.00× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 29.7 | 5.9 | 8.8 | 1.49× |
| `bare_order_where` | 279.1 | 119.0 | 127.4 | 1.07× |
| `chained_select_collapse` | — | 17.8 | 17.9 | 1.01× |
| `chained_where` | 36.1 | 6.7 | 6.7 | 1.00× |
| `contains_match` | 0.00 | 2.2 | 1.4 | 0.64× |
| `count_aggregate` | 29.0 | 4.1 | 4.1 | 1.00× |
| `decs_count_bare_pred` | — | — | 4.1 | — |
| `distinct_by_count` | 40.9 | 15.8 | 16.3 | 1.03× |
| `distinct_by_order_take` | — | 21.8 | 23.8 | 1.09× |
| `distinct_count` | 41.1 | 16.0 | 16.1 | 1.01× |
| `distinct_count_pred` | — | 15.8 | 15.8 | 1.00× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 176.1 | 30.5 | 30.4 | 1.00× |
| `groupby_count` | 143.1 | 19.4 | 19.4 | 1.00× |
| `groupby_first` | — | 18.6 | 19.7 | 1.06× |
| `groupby_having_count` | 143.4 | 19.3 | 19.3 | 1.00× |
| `groupby_having_hidden_sum` | 177.9 | 24.5 | 24.2 | 0.99× |
| `groupby_having_post_where` | 171.9 | 18.6 | 18.7 | 1.01× |
| `groupby_max` | 175.7 | 25.1 | 25.3 | 1.01× |
| `groupby_min` | 174.7 | 25.1 | 25.4 | 1.01× |
| `groupby_multi_reducer` | 192.6 | 33.1 | 32.7 | 0.99× |
| `groupby_select_order` | 171.1 | 18.8 | 18.7 | 0.99× |
| `groupby_select_sum` | 207.4 | 36.9 | 36.4 | 0.99× |
| `groupby_sum` | 173.3 | 18.8 | 19.0 | 1.01× |
| `groupby_where_count` | 76.0 | 14.7 | 15.0 | 1.02× |
| `groupby_where_sum` | 87.3 | 14.3 | 15.1 | 1.06× |
| `indexed_lookup` | 1453.6 | 204197.0 | 469.0 | 0.00× |
| `join_count` | 38.0 | 122.8 | 64.0 | 0.52× |
| `join_groupby_count` | — | 186.9 | 90.4 | 0.48× |
| `join_groupby_to_array` | — | 217.7 | 91.0 | 0.42× |
| `join_select` | — | 149.0 | 86.3 | 0.58× |
| `join_where_count` | 39.5 | 149.0 | 81.3 | 0.55× |
| `last_match` | 0.00 | 5.9 | 14.0 | 2.37× |
| `long_count_aggregate` | 29.1 | 4.1 | 4.1 | 1.00× |
| `max_aggregate` | 30.4 | 6.0 | 6.8 | 1.13× |
| `min_aggregate` | 30.8 | 6.1 | 6.8 | 1.11× |
| `order_distinct_take` | — | 16.1 | — | — |
| `order_reverse_normalized` | 37.9 | 16.1 | 20.1 | 1.25× |
| `order_take_desc` | 38.0 | 15.9 | 20.4 | 1.28× |
| `reverse_take` | 0.10 | 0.00 | 9.3 | — |
| `reverse_take_select` | 0.00 | 34.6 | 48.8 | 1.41× |
| `select_count` | 0.10 | 0.00 | 2.2 | — |
| `select_where` | 195.9 | 11.3 | 19.7 | 1.74× |
| `select_where_count` | 32.4 | 5.3 | 7.4 | 1.40× |
| `select_where_order_take` | 36.6 | 12.2 | 14.9 | 1.22× |
| `select_where_sum` | 37.0 | 7.5 | 7.5 | 1.00× |
| `single_match` | 0.00 | 2.9 | 5.6 | 1.93× |
| `skip_take` | 0.50 | 0.10 | 0.20 | 2.00× |
| `skip_while_match` | 3.5 | 5.2 | 5.3 | 1.02× |
| `sort_first` | 37.6 | 19.5 | 15.8 | 0.81× |
| `sort_take` | 38.0 | 16.4 | 20.4 | 1.24× |
| `sort_take_select` | 38.2 | 16.3 | 20.3 | 1.25× |
| `sum_aggregate` | 29.7 | 2.2 | 2.1 | 0.95× |
| `sum_where` | 32.3 | 4.3 | 4.3 | 1.00× |
| `take_count` | 3.6 | 0.20 | 0.40 | 2.00× |
| `take_count_filtered` | — | 0.20 | 0.20 | 1.00× |
| `take_sum_aggregate` | — | 0.10 | 0.10 | 1.00× |
| `take_where_count` | — | 0.10 | 34.6 | 346.00× |
| `take_while_match` | 7.9 | 2.5 | 2.5 | 1.00× |
| `to_array_filter` | 70.8 | 11.8 | 11.9 | 1.01× |
| `zip_count_pred` | — | 14.9 | — | — |
| `zip_dot_product` | — | 12.6 | 10.3 | 0.82× |
| `zip_dot_product_3arg` | — | 12.6 | — | — |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | Decs vs Array |
|---|---:|---:|---:|---:|
| `aggregate_match` | 34.2 | 0.40 | 0.70 | 1.75× |
| `all_match` | 27.6 | 0.30 | 0.20 | 0.67× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 29.6 | 1.0 | 3.6 | 3.60× |
| `bare_order_where` | 186.4 | 34.9 | 34.9 | 1.00× |
| `chained_select_collapse` | — | 2.1 | 2.1 | 1.00× |
| `chained_where` | 36.0 | 0.60 | 0.80 | 1.33× |
| `contains_match` | 0.00 | 0.20 | 0.10 | 0.50× |
| `count_aggregate` | 29.1 | 0.40 | 0.60 | 1.50× |
| `decs_count_bare_pred` | — | — | 0.60 | — |
| `distinct_by_count` | 40.8 | 2.1 | 2.1 | 1.00× |
| `distinct_by_order_take` | — | 2.6 | 3.3 | 1.27× |
| `distinct_count` | 40.9 | 2.1 | 2.1 | 1.00× |
| `distinct_count_pred` | — | 2.1 | 2.3 | 1.10× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 171.7 | 2.6 | 2.9 | 1.12× |
| `groupby_count` | 141.0 | 2.4 | 2.5 | 1.04× |
| `groupby_first` | — | 2.2 | 3.1 | 1.41× |
| `groupby_having_count` | 141.1 | 2.4 | 2.5 | 1.04× |
| `groupby_having_hidden_sum` | 175.9 | 2.5 | 2.9 | 1.16× |
| `groupby_having_post_where` | 172.2 | 2.4 | 2.7 | 1.13× |
| `groupby_max` | 174.3 | 2.4 | 2.7 | 1.13× |
| `groupby_min` | 175.3 | 2.4 | 2.7 | 1.13× |
| `groupby_multi_reducer` | 188.8 | 2.7 | 3.0 | 1.11× |
| `groupby_select_order` | 171.5 | 2.4 | 2.7 | 1.13× |
| `groupby_select_sum` | 200.7 | 3.2 | 3.7 | 1.16× |
| `groupby_sum` | 172.2 | 2.4 | 2.7 | 1.13× |
| `groupby_where_count` | 75.9 | 1.7 | 1.8 | 1.06× |
| `groupby_where_sum` | 86.5 | 1.7 | 1.8 | 1.06× |
| `indexed_lookup` | 1238.2 | 35098.4 | 102.8 | 0.00× |
| `join_count` | 37.9 | 36.1 | 13.4 | 0.37× |
| `join_groupby_count` | — | 48.4 | 23.5 | 0.49× |
| `join_groupby_to_array` | — | 57.0 | 23.4 | 0.41× |
| `join_select` | — | 43.3 | 24.6 | 0.57× |
| `join_where_count` | 39.0 | 42.7 | 25.4 | 0.59× |
| `last_match` | 0.00 | 0.50 | 1.4 | 2.80× |
| `long_count_aggregate` | 29.0 | 0.40 | 0.60 | 1.50× |
| `max_aggregate` | 30.7 | 0.60 | 0.50 | 0.83× |
| `min_aggregate` | 30.6 | 0.70 | 0.50 | 0.71× |
| `order_distinct_take` | — | 2.1 | — | — |
| `order_reverse_normalized` | 37.9 | 0.70 | 1.4 | 2.00× |
| `order_take_desc` | 38.0 | 0.70 | 1.4 | 2.00× |
| `reverse_take` | 0.00 | 0.00 | 1.1 | — |
| `reverse_take_select` | 0.00 | 8.7 | 10.6 | 1.22× |
| `select_count` | 0.10 | 0.00 | 0.00 | — |
| `select_where` | 106.2 | 4.2 | 5.8 | 1.38× |
| `select_where_count` | 32.6 | 0.40 | 0.60 | 1.50× |
| `select_where_order_take` | 36.2 | 0.70 | 1.4 | 2.00× |
| `select_where_sum` | 36.9 | 0.50 | 0.60 | 1.20× |
| `single_match` | 0.00 | 0.40 | 1.1 | 2.75× |
| `skip_take` | 0.30 | 0.00 | 0.00 | — |
| `skip_while_match` | 3.4 | 0.40 | 0.40 | 1.00× |
| `sort_first` | 37.5 | 0.40 | 1.3 | 3.25× |
| `sort_take` | 38.0 | 0.70 | 1.4 | 2.00× |
| `sort_take_select` | 38.5 | 0.70 | 1.4 | 2.00× |
| `sum_aggregate` | 33.5 | 0.40 | 0.30 | 0.75× |
| `sum_where` | 32.9 | 0.40 | 0.60 | 1.50× |
| `take_count` | 1.8 | 0.10 | 0.10 | 1.00× |
| `take_count_filtered` | — | 0.00 | 0.00 | — |
| `take_sum_aggregate` | — | 0.00 | 0.00 | — |
| `take_where_count` | — | 0.00 | 10.1 | — |
| `take_while_match` | 8.0 | 0.20 | 0.30 | 1.50× |
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
- **`order_distinct_take` Decs** — no duplicate-prone decs template in
  `_common.das`; plain `distinct()` (whole-element dedup) needs
  intentional duplicates to exercise the set-gate. The splice path IS
  verified in
  `tests/linq/test_linq_fold_theme3_c1_c5_distinct_order_take.das`
  (`test_c5_decs_order_distinct_take_to_array`). Follow-up TODO
  2026-05-25: add a `DecsBrand` (or similar) fixture.
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
