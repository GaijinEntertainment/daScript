# Benchmarks — SQL / Array / Decs comparison

Generated 2026-05-26 from `e968eec74` (PR F3 R1 — Copilot follow-up). Matrix refreshed for **Phase F PRs F1+F2+F3** (Terminator lane unification — `TerminatorSpec` for array side + `DecsTerminatorSpec` + `DecsDispatch` for decs side; 9 emit fns migrated to spec-builders). All three PRs verified AST byte-identical at emit time, so the cross-the-board 15-30% drop vs the prior `0f88a64b6` (PR D3) baseline reflects environment delta + cumulative wins across PR E + F1-F3, not refactor-introduced perf change. No regressions in any cell.
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
| `aggregate_match` | 34.5 | 6.0 | 5.9 | 0.98× |
| `all_match` | 27.7 | 3.5 | 3.6 | 1.03× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 29.9 | 6.0 | 8.8 | 1.47× |
| `bare_order_where` | 273.9 | 117.7 | 124.1 | 1.05× |
| `chained_select_collapse` | — | 17.9 | 17.6 | 0.98× |
| `chained_where` | 36.2 | 6.7 | 7.2 | 1.07× |
| `contains_match` | 0.00 | 2.2 | 1.4 | 0.64× |
| `count_aggregate` | 29.3 | 4.1 | 4.1 | 1.00× |
| `decs_count_bare_pred` | — | — | 4.2 | — |
| `distinct_by_count` | 41.1 | 15.8 | 16.0 | 1.01× |
| `distinct_by_order_take` | — | 21.8 | 23.3 | 1.07× |
| `distinct_count` | 42.1 | 16.2 | 16.1 | 0.99× |
| `distinct_count_pred` | — | 15.8 | 16.1 | 1.02× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 172.7 | 30.8 | 30.3 | 0.98× |
| `groupby_count` | 141.6 | 19.5 | 19.3 | 0.99× |
| `groupby_first` | — | 18.6 | 19.2 | 1.03× |
| `groupby_having_count` | 142.9 | 19.4 | 19.3 | 0.99× |
| `groupby_having_hidden_sum` | 175.1 | 24.0 | 24.3 | 1.01× |
| `groupby_having_post_where` | 173.0 | 18.6 | 18.8 | 1.01× |
| `groupby_max` | 173.4 | 25.2 | 25.2 | 1.00× |
| `groupby_min` | 174.3 | 25.1 | 25.3 | 1.01× |
| `groupby_multi_reducer` | 192.6 | 32.7 | 32.6 | 1.00× |
| `groupby_select_order` | 171.3 | 18.7 | 18.9 | 1.01× |
| `groupby_select_sum` | 205.0 | 36.8 | 36.0 | 0.98× |
| `groupby_sum` | 173.6 | 18.8 | 18.6 | 0.99× |
| `groupby_where_count` | 75.5 | 14.6 | 15.1 | 1.03× |
| `groupby_where_sum` | 87.1 | 14.3 | 14.7 | 1.03× |
| `indexed_lookup` | 1462.1 | 205000.2 | 475.5 | 0.00× |
| `join_count` | 38.1 | 121.7 | 64.3 | 0.53× |
| `join_groupby_count` | — | 187.4 | 91.5 | 0.49× |
| `join_groupby_to_array` | — | 216.5 | 90.7 | 0.42× |
| `join_select` | — | 150.8 | 85.8 | 0.57× |
| `join_where_count` | 39.9 | 148.6 | 75.0 | 0.50× |
| `last_match` | 0.00 | 5.9 | 14.0 | 2.37× |
| `long_count_aggregate` | 29.4 | 4.1 | 4.1 | 1.00× |
| `max_aggregate` | 30.5 | 6.0 | 6.9 | 1.15× |
| `min_aggregate` | 30.5 | 6.1 | 6.8 | 1.11× |
| `order_distinct_take` | — | 15.9 | — | — |
| `order_reverse_normalized` | 38.1 | 16.2 | 20.1 | 1.24× |
| `order_take_desc` | 38.3 | 16.2 | 19.9 | 1.23× |
| `reverse_take` | 0.10 | 0.00 | 10.2 | — |
| `reverse_take_select` | 0.00 | 34.0 | 48.6 | 1.43× |
| `select_count` | 0.10 | 0.00 | 2.2 | — |
| `select_where` | 192.5 | 11.4 | 19.5 | 1.71× |
| `select_where_count` | 32.4 | 5.2 | 7.4 | 1.42× |
| `select_where_order_take` | 36.4 | 12.4 | 14.9 | 1.20× |
| `select_where_sum` | 37.0 | 7.5 | 7.5 | 1.00× |
| `single_match` | 0.00 | 3.0 | 5.5 | 1.83× |
| `skip_take` | 0.50 | 0.10 | 0.20 | 2.00× |
| `skip_while_match` | 3.5 | 5.3 | 5.3 | 1.00× |
| `sort_first` | 38.5 | 11.1 | 13.3 | 1.20× |
| `sort_take` | 38.0 | 16.4 | 20.9 | 1.27× |
| `sort_take_select` | 38.1 | 16.2 | 20.4 | 1.26× |
| `sum_aggregate` | 29.9 | 2.2 | 2.1 | 0.95× |
| `sum_where` | 32.8 | 4.3 | 4.3 | 1.00× |
| `take_count` | 3.6 | 0.20 | 0.40 | 2.00× |
| `take_count_filtered` | — | 0.20 | 0.20 | 1.00× |
| `take_sum_aggregate` | — | 0.10 | 0.10 | 1.00× |
| `take_where_count` | — | 0.10 | 35.4 | 354.00× |
| `take_while_match` | 7.8 | 2.4 | 2.5 | 1.04× |
| `to_array_filter` | 71.0 | 11.8 | 11.9 | 1.01× |
| `zip_count_pred` | — | 15.1 | — | — |
| `zip_dot_product` | — | 12.8 | 10.7 | 0.84× |
| `zip_dot_product_3arg` | — | 12.8 | — | — |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | Decs vs Array |
|---|---:|---:|---:|---:|
| `aggregate_match` | 34.8 | 0.40 | 0.70 | 1.75× |
| `all_match` | 27.8 | 0.30 | 0.20 | 0.67× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 29.9 | 1.0 | 3.6 | 3.60× |
| `bare_order_where` | 183.8 | 34.0 | 35.0 | 1.03× |
| `chained_select_collapse` | — | 2.1 | 2.1 | 1.00× |
| `chained_where` | 36.4 | 0.60 | 0.90 | 1.50× |
| `contains_match` | 0.00 | 0.20 | 0.10 | 0.50× |
| `count_aggregate` | 29.4 | 0.40 | 0.60 | 1.50× |
| `decs_count_bare_pred` | — | — | 0.60 | — |
| `distinct_by_count` | 41.3 | 2.1 | 2.1 | 1.00× |
| `distinct_by_order_take` | — | 2.7 | 3.2 | 1.19× |
| `distinct_count` | 41.2 | 2.1 | 2.1 | 1.00× |
| `distinct_count_pred` | — | 2.1 | 2.3 | 1.10× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 173.1 | 2.6 | 2.9 | 1.12× |
| `groupby_count` | 141.3 | 2.4 | 2.5 | 1.04× |
| `groupby_first` | — | 2.2 | 3.1 | 1.41× |
| `groupby_having_count` | 141.8 | 2.4 | 2.5 | 1.04× |
| `groupby_having_hidden_sum` | 179.5 | 2.5 | 2.8 | 1.12× |
| `groupby_having_post_where` | 172.8 | 2.4 | 2.7 | 1.13× |
| `groupby_max` | 173.7 | 2.4 | 2.7 | 1.13× |
| `groupby_min` | 173.8 | 2.4 | 2.7 | 1.13× |
| `groupby_multi_reducer` | 190.7 | 2.7 | 3.0 | 1.11× |
| `groupby_select_order` | 171.5 | 2.4 | 2.7 | 1.13× |
| `groupby_select_sum` | 204.0 | 3.2 | 3.7 | 1.16× |
| `groupby_sum` | 172.2 | 2.4 | 2.7 | 1.13× |
| `groupby_where_count` | 75.7 | 1.5 | 1.8 | 1.20× |
| `groupby_where_sum` | 87.0 | 1.5 | 1.8 | 1.20× |
| `indexed_lookup` | 1242.9 | 35154.5 | 106.4 | 0.00× |
| `join_count` | 38.1 | 34.6 | 12.9 | 0.37× |
| `join_groupby_count` | — | 47.4 | 22.0 | 0.46× |
| `join_groupby_to_array` | — | 54.6 | 21.9 | 0.40× |
| `join_select` | — | 41.6 | 22.6 | 0.54× |
| `join_where_count` | 39.4 | 41.3 | 23.2 | 0.56× |
| `last_match` | 0.00 | 0.50 | 1.4 | 2.80× |
| `long_count_aggregate` | 29.3 | 0.40 | 0.60 | 1.50× |
| `max_aggregate` | 30.6 | 0.60 | 0.50 | 0.83× |
| `min_aggregate` | 30.5 | 0.60 | 0.50 | 0.83× |
| `order_distinct_take` | — | 2.1 | — | — |
| `order_reverse_normalized` | 38.0 | 0.70 | 1.3 | 1.86× |
| `order_take_desc` | 37.8 | 0.70 | 1.3 | 1.86× |
| `reverse_take` | 0.00 | 0.00 | 1.1 | — |
| `reverse_take_select` | 0.00 | 8.4 | 10.4 | 1.24× |
| `select_count` | 0.10 | 0.00 | 0.00 | — |
| `select_where` | 104.4 | 4.2 | 5.6 | 1.33× |
| `select_where_count` | 32.6 | 0.40 | 0.60 | 1.50× |
| `select_where_order_take` | 36.3 | 0.70 | 1.4 | 2.00× |
| `select_where_sum` | 37.1 | 0.50 | 0.60 | 1.20× |
| `single_match` | 0.00 | 0.40 | 1.1 | 2.75× |
| `skip_take` | 0.30 | 0.00 | 0.00 | — |
| `skip_while_match` | 3.4 | 0.40 | 0.40 | 1.00× |
| `sort_first` | 37.6 | 0.40 | 1.3 | 3.25× |
| `sort_take` | 38.0 | 0.70 | 1.3 | 1.86× |
| `sort_take_select` | 37.9 | 0.70 | 1.3 | 1.86× |
| `sum_aggregate` | 29.9 | 0.40 | 0.30 | 0.75× |
| `sum_where` | 32.9 | 0.40 | 0.60 | 1.50× |
| `take_count` | 1.8 | 0.10 | 0.10 | 1.00× |
| `take_count_filtered` | — | 0.00 | 0.00 | — |
| `take_sum_aggregate` | — | 0.00 | 0.00 | — |
| `take_where_count` | — | 0.00 | 10.3 | — |
| `take_while_match` | 7.8 | 0.20 | 0.30 | 1.50× |
| `to_array_filter` | 47.8 | 3.3 | 3.4 | 1.03× |
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
