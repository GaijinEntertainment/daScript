# Benchmarks — SQL / Array / Decs comparison

Generated 2026-05-26 from `0f88a64b6` (PR D3 — full GroupBySourceAdapter melt; closes the linq_fold pattern-table refactor masterplan). Full INTERP + JIT rerun across all 69 lanes; PRs D1/D2/D3 were pure refactors, so the matrix is expected within run-to-run noise of the prior `2c18845d5` baseline.
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
| `aggregate_match` | 35.1 | 5.9 | 5.9 | 1.00× |
| `all_match` | 28.0 | 3.5 | 3.5 | 1.00× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 29.7 | 6.0 | 8.7 | 1.45× |
| `bare_order_where` | 271.1 | 116.7 | 125.2 | 1.07× |
| `chained_select_collapse` | — | 17.7 | 17.6 | 0.99× |
| `chained_where` | 36.1 | 6.7 | 7.2 | 1.07× |
| `contains_match` | 0.00 | 2.2 | 2.2 | 1.00× |
| `count_aggregate` | 30.1 | 4.1 | 4.1 | 1.00× |
| `decs_count_bare_pred` | — | — | 4.2 | — |
| `distinct_by_count` | 41.7 | 15.8 | 16.0 | 1.01× |
| `distinct_by_order_take` | — | 21.8 | 23.5 | 1.08× |
| `distinct_count` | 41.8 | 16.2 | 15.9 | 0.98× |
| `distinct_count_pred` | — | 16.0 | 16.0 | 1.00× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 171.1 | 30.5 | 30.1 | 0.99× |
| `groupby_count` | 144.2 | 20.5 | 20.6 | 1.00× |
| `groupby_first` | — | 19.9 | 20.6 | 1.04× |
| `groupby_having_count` | 141.8 | 20.5 | 20.6 | 1.00× |
| `groupby_having_hidden_sum` | 176.2 | 24.5 | 24.3 | 0.99× |
| `groupby_having_post_where` | 171.9 | 20.8 | 19.9 | 0.96× |
| `groupby_max` | 175.4 | 25.1 | 25.2 | 1.00× |
| `groupby_min` | 176.3 | 32.7 | 25.3 | 0.77× |
| `groupby_multi_reducer` | 190.4 | 32.7 | 32.6 | 1.00× |
| `groupby_select_order` | 171.1 | 20.2 | 19.9 | 0.99× |
| `groupby_select_sum` | 206.6 | 37.1 | 36.5 | 0.98× |
| `groupby_sum` | 172.5 | 20.1 | 19.9 | 0.99× |
| `groupby_where_count` | 76.6 | 15.8 | 15.0 | 0.95× |
| `groupby_where_sum` | 86.7 | 14.3 | 14.8 | 1.03× |
| `indexed_lookup` | 1443.0 | 205391.4 | 484.6 | 0.00× |
| `join_count` | 37.7 | 122.0 | 64.0 | 0.52× |
| `join_groupby_count` | — | 186.8 | 91.6 | 0.49× |
| `join_groupby_to_array` | — | 217.2 | 90.9 | 0.42× |
| `join_select` | — | 149.4 | 86.5 | 0.58× |
| `join_where_count` | 39.2 | 150.6 | 80.9 | 0.54× |
| `last_match` | 0.00 | 5.9 | 15.6 | 2.64× |
| `long_count_aggregate` | 34.6 | 4.9 | 4.8 | 0.98× |
| `max_aggregate` | 34.8 | 7.2 | 8.3 | 1.15× |
| `min_aggregate` | 35.4 | 7.2 | 8.5 | 1.18× |
| `order_distinct_take` | — | 20.6 | — | — |
| `order_reverse_normalized` | 47.5 | 18.6 | 23.2 | 1.25× |
| `order_take_desc` | 48.1 | 19.6 | 23.1 | 1.18× |
| `reverse_take` | 0.10 | 0.00 | 11.8 | — |
| `reverse_take_select` | 0.10 | 42.3 | 63.0 | 1.49× |
| `select_count` | 0.20 | 0.00 | 2.6 | — |
| `select_where` | 249.9 | 15.0 | 24.8 | 1.65× |
| `select_where_count` | 39.5 | 7.0 | 10.8 | 1.54× |
| `select_where_order_take` | 47.4 | 15.5 | 18.3 | 1.18× |
| `select_where_sum` | 49.1 | 11.2 | 10.6 | 0.95× |
| `single_match` | 0.00 | 4.1 | 7.1 | 1.73× |
| `skip_take` | 0.60 | 0.10 | 0.20 | 2.00× |
| `skip_while_match` | 4.3 | 6.6 | 6.6 | 1.00× |
| `sort_first` | 44.2 | 14.0 | 20.7 | 1.48× |
| `sort_take` | 60.7 | 18.2 | 23.0 | 1.26× |
| `sort_take_select` | 45.6 | 18.6 | 22.7 | 1.22× |
| `sum_aggregate` | 34.4 | 2.6 | 2.4 | 0.92× |
| `sum_where` | 35.8 | 4.9 | 4.9 | 1.00× |
| `take_count` | 4.1 | 0.30 | 0.50 | 1.67× |
| `take_count_filtered` | — | 0.20 | 0.20 | 1.00× |
| `take_sum_aggregate` | — | 0.10 | 0.10 | 1.00× |
| `take_where_count` | — | 0.10 | 37.5 | 375.00× |
| `take_while_match` | 8.5 | 2.7 | 2.6 | 0.96× |
| `to_array_filter` | 76.3 | 12.8 | 13.1 | 1.02× |
| `zip_count_pred` | — | 15.6 | — | — |
| `zip_dot_product` | — | 14.4 | 10.7 | 0.74× |
| `zip_dot_product_3arg` | — | 14.7 | — | — |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | Decs vs Array |
|---|---:|---:|---:|---:|
| `aggregate_match` | 34.1 | 0.40 | 0.70 | 1.75× |
| `all_match` | 27.3 | 0.30 | 0.20 | 0.67× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 29.6 | 1.0 | 3.6 | 3.60× |
| `bare_order_where` | 185.9 | 34.8 | 45.2 | 1.30× |
| `chained_select_collapse` | — | 2.1 | 2.1 | 1.00× |
| `chained_where` | 36.3 | 0.60 | 0.90 | 1.50× |
| `contains_match` | 0.00 | 0.20 | 0.10 | 0.50× |
| `count_aggregate` | 29.1 | 0.40 | 0.60 | 1.50× |
| `decs_count_bare_pred` | — | — | 0.60 | — |
| `distinct_by_count` | 40.8 | 2.1 | 2.1 | 1.00× |
| `distinct_by_order_take` | — | 2.7 | 3.3 | 1.22× |
| `distinct_count` | 41.1 | 2.1 | 2.1 | 1.00× |
| `distinct_count_pred` | — | 2.1 | 2.3 | 1.10× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 213.8 | 3.2 | 3.6 | 1.12× |
| `groupby_count` | 187.1 | 3.0 | 3.1 | 1.03× |
| `groupby_first` | — | 2.8 | 4.0 | 1.43× |
| `groupby_having_count` | 193.4 | 3.0 | 3.1 | 1.03× |
| `groupby_having_hidden_sum` | 238.7 | 3.2 | 3.7 | 1.16× |
| `groupby_having_post_where` | 225.4 | 3.1 | 3.5 | 1.13× |
| `groupby_max` | 228.1 | 3.3 | 3.4 | 1.03× |
| `groupby_min` | 219.0 | 3.0 | 3.3 | 1.10× |
| `groupby_multi_reducer` | 237.0 | 3.3 | 3.6 | 1.09× |
| `groupby_select_order` | 291.3 | 2.8 | 3.7 | 1.32× |
| `groupby_select_sum` | 234.6 | 3.8 | 4.3 | 1.13× |
| `groupby_sum` | 200.7 | 2.8 | 3.1 | 1.11× |
| `groupby_where_count` | 87.1 | 1.7 | 2.0 | 1.18× |
| `groupby_where_sum` | 97.8 | 1.7 | 1.9 | 1.12× |
| `indexed_lookup` | 1251.9 | 38941.4 | 115.5 | 0.00× |
| `join_count` | 39.3 | 38.7 | 14.6 | 0.38× |
| `join_groupby_count` | — | 53.5 | 26.1 | 0.49× |
| `join_groupby_to_array` | — | 56.9 | 26.9 | 0.47× |
| `join_select` | — | 43.2 | 24.2 | 0.56× |
| `join_where_count` | 39.5 | 42.7 | 24.7 | 0.58× |
| `last_match` | 0.00 | 0.50 | 1.4 | 2.80× |
| `long_count_aggregate` | 29.8 | 0.40 | 0.60 | 1.50× |
| `max_aggregate` | 31.2 | 0.60 | 0.50 | 0.83× |
| `min_aggregate` | 36.7 | 0.60 | 0.50 | 0.83× |
| `order_distinct_take` | — | 2.1 | — | — |
| `order_reverse_normalized` | 38.1 | 0.70 | 1.4 | 2.00× |
| `order_take_desc` | 40.4 | 0.70 | 1.3 | 1.86× |
| `reverse_take` | 0.00 | 0.00 | 1.1 | — |
| `reverse_take_select` | 0.00 | 8.8 | 10.8 | 1.23× |
| `select_count` | 0.10 | 0.00 | 0.00 | — |
| `select_where` | 107.0 | 4.2 | 5.6 | 1.33× |
| `select_where_count` | 32.6 | 0.40 | 0.60 | 1.50× |
| `select_where_order_take` | 36.6 | 0.70 | 1.4 | 2.00× |
| `select_where_sum` | 38.6 | 0.50 | 0.60 | 1.20× |
| `single_match` | 0.00 | 0.40 | 1.1 | 2.75× |
| `skip_take` | 0.30 | 0.00 | 0.00 | — |
| `skip_while_match` | 3.5 | 0.40 | 0.40 | 1.00× |
| `sort_first` | 38.2 | 0.40 | 1.3 | 3.25× |
| `sort_take` | 38.3 | 0.70 | 1.4 | 2.00× |
| `sort_take_select` | 38.5 | 0.70 | 1.4 | 2.00× |
| `sum_aggregate` | 30.1 | 0.40 | 0.30 | 0.75× |
| `sum_where` | 32.4 | 0.40 | 0.60 | 1.50× |
| `take_count` | 1.8 | 0.10 | 0.10 | 1.00× |
| `take_count_filtered` | — | 0.00 | 0.00 | — |
| `take_sum_aggregate` | — | 0.00 | 0.00 | — |
| `take_where_count` | — | 0.00 | 10.3 | — |
| `take_while_match` | 8.7 | 0.20 | 0.30 | 1.50× |
| `to_array_filter` | 48.7 | 3.3 | 3.4 | 1.03× |
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
