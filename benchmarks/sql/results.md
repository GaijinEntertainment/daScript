# Benchmarks — SQL / Array / Decs comparison

Generated 2026-05-27 from PR `bbatkin/sqlite-linq-join-groupby-alias` (chunk N+2 — sqlite_linq projection-alias resolver: post-join `_.<alias>` resolves through the join's `into` projection registry, transitively enabling `_group_by` / `_having` / `_order_by` / aggregates / computed-key in chains following a `_join`). Two SQL `—` cells now populated, draining the "`_group_by` after `_join`" entry from [`sqlite_linq_gaps.md`](sqlite_linq_gaps.md):
- `join_groupby_count` m1 = 160.7 INTERP / 157.8 JIT (beats `m3f` 186.2 / 47.2 in INTERP but JIT m3f still wins)
- `join_groupby_to_array` m1 = 189.9 INTERP / 190.3 JIT (similar shape)

Drift across the rest of the matrix is at measurement floor (±5% INTERP, ±10% JIT). The `_join`-side surface didn't touch any non-join planner, so the row-level changes are noise.
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
| `aggregate_match` | 34.2 | 6.0 | 6.0 | 1.00× |
| `all_match` | 27.7 | 3.6 | 3.6 | 1.00× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 30.0 | 6.0 | 8.9 | 1.48× |
| `bare_order_where` | 286.5 | 118.0 | 128.2 | 1.09× |
| `chained_select_collapse` | — | 18.3 | 18.3 | 1.00× |
| `chained_where` | 36.3 | 6.8 | 7.7 | 1.13× |
| `contains_match` | 0.00 | 2.3 | 1.5 | 0.65× |
| `count_aggregate` | 29.5 | 4.2 | 4.4 | 1.05× |
| `decs_count_bare_pred` | — | — | 4.3 | — |
| `distinct_by_count` | 41.1 | 15.9 | 16.1 | 1.01× |
| `distinct_by_order_take` | 242.4 | 22.0 | 24.0 | 1.09× |
| `distinct_by_order_to_array` | 239.8 | 22.1 | 23.6 | 1.07× |
| `distinct_count` | 41.3 | 16.0 | 16.1 | 1.01× |
| `distinct_count_pred` | 253.8 | 16.1 | 16.0 | 0.99× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 173.2 | 30.9 | 30.3 | 0.98× |
| `groupby_count` | 146.6 | 19.4 | 19.4 | 1.00× |
| `groupby_first` | 253.3 | 18.6 | 19.2 | 1.03× |
| `groupby_having_count` | 142.9 | 19.3 | 19.4 | 1.01× |
| `groupby_having_hidden_sum` | 178.1 | 24.0 | 24.5 | 1.02× |
| `groupby_having_post_where` | 173.8 | 19.1 | 18.8 | 0.98× |
| `groupby_max` | 175.0 | 25.4 | 25.5 | 1.00× |
| `groupby_min` | 177.7 | 26.0 | 26.0 | 1.00× |
| `groupby_multi_reducer` | 191.3 | 32.3 | 32.7 | 1.01× |
| `groupby_select_order` | 173.4 | 18.9 | 18.7 | 0.99× |
| `groupby_select_sum` | 208.8 | 36.8 | 36.5 | 0.99× |
| `groupby_sum` | 173.4 | 18.6 | 18.8 | 1.01× |
| `groupby_where_count` | 75.9 | 14.7 | 15.0 | 1.02× |
| `groupby_where_sum` | 87.5 | 14.3 | 14.7 | 1.03× |
| `indexed_lookup` | 1457.1 | 205119.6 | 482.2 | 0.00× |
| `join_count` | 37.9 | 128.2 | 65.4 | 0.51× |
| `join_groupby_count` | 160.7 | 186.2 | 90.4 | 0.49× |
| `join_groupby_to_array` | 189.9 | 217.8 | 93.1 | 0.43× |
| `join_select` | — | 148.1 | 85.3 | 0.58× |
| `join_where_count` | 39.3 | 148.5 | 74.4 | 0.50× |
| `last_match` | 0.00 | 5.8 | 14.3 | 2.47× |
| `long_count_aggregate` | 29.5 | 4.1 | 4.1 | 1.00× |
| `max_aggregate` | 30.5 | 6.0 | 7.0 | 1.17× |
| `min_aggregate` | 30.5 | 6.1 | 6.8 | 1.11× |
| `order_distinct_take` | 139.1 | 15.8 | 97.2 | 6.15× |
| `order_reverse_normalized` | 38.0 | 16.2 | 20.1 | 1.24× |
| `order_take_desc` | 37.9 | 16.2 | 20.0 | 1.23× |
| `reverse_distinct_by` | 292.5 | 21.4 | — | — |
| `reverse_take` | 0.1 | 0.00 | 10.1 | — |
| `reverse_take_select` | 0.00 | 33.8 | 48.3 | 1.43× |
| `select_count` | 0.1 | 0.00 | 2.2 | — |
| `select_where` | 193.2 | 11.1 | 19.6 | 1.77× |
| `select_where_count` | 32.4 | 5.2 | 7.5 | 1.44× |
| `select_where_order_take` | 36.3 | 12.3 | 15.0 | 1.22× |
| `select_where_sum` | 37.0 | 7.5 | 7.6 | 1.01× |
| `single_match` | 0.00 | 2.9 | 5.5 | 1.90× |
| `skip_take` | 0.5 | 0.1 | 0.2 | 2.00× |
| `skip_while_match` | 3.5 | 5.3 | 5.3 | 1.00× |
| `sort_first` | 37.7 | 11.1 | 13.3 | 1.20× |
| `sort_take` | 37.9 | 16.4 | 20.8 | 1.27× |
| `sort_take_select` | 37.9 | 16.2 | 20.1 | 1.24× |
| `sum_aggregate` | 30.2 | 2.2 | 2.1 | 0.95× |
| `sum_where` | 32.3 | 4.3 | 4.2 | 0.98× |
| `take_count` | 3.6 | 0.2 | 0.4 | 2.00× |
| `take_count_filtered` | — | 0.2 | 0.2 | 1.00× |
| `take_sum_aggregate` | — | 0.1 | 0.1 | 1.00× |
| `take_where_count` | — | 0.1 | 0.1 | 1.00× |
| `take_while_match` | 7.8 | 2.4 | 2.5 | 1.04× |
| `to_array_filter` | 70.1 | 11.7 | 11.8 | 1.01× |
| `zip_count_pred` | — | 15.3 | — | — |
| `zip_dot_product` | — | 12.8 | 10.9 | 0.85× |
| `zip_dot_product_3arg` | — | 12.9 | — | — |
| `zip_reverse_to_array` | — | 31.0 | — | — |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | Decs vs Array |
|---|---:|---:|---:|---:|
| `aggregate_match` | 34.6 | 0.4 | 0.7 | 1.75× |
| `all_match` | 27.6 | 0.3 | 0.2 | 0.67× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 29.8 | 1.0 | 3.6 | 3.60× |
| `bare_order_where` | 196.3 | 35.4 | 36.8 | 1.04× |
| `chained_select_collapse` | — | 2.2 | 2.2 | 1.00× |
| `chained_where` | 36.4 | 0.6 | 0.9 | 1.50× |
| `contains_match` | 0.00 | 0.2 | 0.1 | 0.50× |
| `count_aggregate` | 29.0 | 0.4 | 0.6 | 1.50× |
| `decs_count_bare_pred` | — | — | 0.6 | — |
| `distinct_by_count` | 40.8 | 2.2 | 2.1 | 0.95× |
| `distinct_by_order_take` | 247.0 | 2.7 | 3.3 | 1.22× |
| `distinct_by_order_to_array` | 240.3 | 2.8 | 3.3 | 1.18× |
| `distinct_count` | 41.2 | 2.1 | 2.1 | 1.00× |
| `distinct_count_pred` | 250.2 | 2.1 | 2.3 | 1.10× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 174.9 | 2.6 | 2.9 | 1.12× |
| `groupby_count` | 142.7 | 2.4 | 2.5 | 1.04× |
| `groupby_first` | 251.9 | 2.6 | 3.1 | 1.19× |
| `groupby_having_count` | 148.8 | 2.4 | 2.5 | 1.04× |
| `groupby_having_hidden_sum` | 175.4 | 2.5 | 2.8 | 1.12× |
| `groupby_having_post_where` | 175.7 | 2.4 | 2.7 | 1.13× |
| `groupby_max` | 183.8 | 2.4 | 2.7 | 1.13× |
| `groupby_min` | 172.8 | 2.4 | 2.7 | 1.13× |
| `groupby_multi_reducer` | 190.1 | 2.7 | 3.0 | 1.11× |
| `groupby_select_order` | 170.5 | 2.4 | 2.7 | 1.13× |
| `groupby_select_sum` | 201.4 | 3.2 | 3.7 | 1.16× |
| `groupby_sum` | 170.1 | 2.4 | 2.7 | 1.13× |
| `groupby_where_count` | 75.7 | 1.5 | 1.8 | 1.20× |
| `groupby_where_sum` | 86.9 | 1.5 | 1.8 | 1.20× |
| `indexed_lookup` | 1256.0 | 35187.8 | 108.3 | 0.00× |
| `join_count` | 37.9 | 34.6 | 14.2 | 0.41× |
| `join_groupby_count` | 157.8 | 47.2 | 22.1 | 0.47× |
| `join_groupby_to_array` | 190.3 | 55.5 | 21.9 | 0.39× |
| `join_select` | — | 41.6 | 22.6 | 0.54× |
| `join_where_count` | 39.0 | 41.2 | 22.8 | 0.55× |
| `last_match` | 0.00 | 0.5 | 1.4 | 2.80× |
| `long_count_aggregate` | 29.0 | 0.4 | 0.6 | 1.50× |
| `max_aggregate` | 30.8 | 0.6 | 0.5 | 0.83× |
| `min_aggregate` | 30.8 | 0.6 | 0.5 | 0.83× |
| `order_distinct_take` | 138.1 | 2.1 | 74.7 | 35.57× |
| `order_reverse_normalized` | 37.8 | 0.7 | 1.3 | 1.86× |
| `order_take_desc` | 37.9 | 0.7 | 1.3 | 1.86× |
| `reverse_distinct_by` | 295.8 | 2.6 | — | — |
| `reverse_take` | 0.00 | 0.00 | 1.1 | — |
| `reverse_take_select` | 0.00 | 8.3 | 10.1 | 1.22× |
| `select_count` | 0.1 | 0.00 | 0.00 | — |
| `select_where` | 105.0 | 4.1 | 5.6 | 1.37× |
| `select_where_count` | 32.5 | 0.4 | 0.6 | 1.50× |
| `select_where_order_take` | 36.9 | 0.7 | 1.3 | 1.86× |
| `select_where_sum` | 37.3 | 0.5 | 0.6 | 1.20× |
| `single_match` | 0.00 | 0.4 | 1.1 | 2.75× |
| `skip_take` | 0.3 | 0.00 | 0.00 | — |
| `skip_while_match` | 3.4 | 0.4 | 0.4 | 1.00× |
| `sort_first` | 37.6 | 0.4 | 1.3 | 3.25× |
| `sort_take` | 38.1 | 0.7 | 1.3 | 1.86× |
| `sort_take_select` | 38.2 | 0.7 | 1.3 | 1.86× |
| `sum_aggregate` | 29.9 | 0.4 | 0.3 | 0.75× |
| `sum_where` | 32.4 | 0.4 | 0.6 | 1.50× |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.00× |
| `take_count_filtered` | — | 0.00 | 0.00 | — |
| `take_sum_aggregate` | — | 0.00 | 0.00 | — |
| `take_where_count` | — | 0.00 | 0.00 | — |
| `take_while_match` | 7.8 | 0.2 | 0.3 | 1.50× |
| `to_array_filter` | 48.3 | 3.2 | 3.4 | 1.06× |
| `zip_count_pred` | — | 0.7 | — | — |
| `zip_dot_product` | — | 0.5 | 0.5 | 1.00× |
| `zip_dot_product_3arg` | — | 0.5 | — | — |
| `zip_reverse_to_array` | — | 4.5 | — | — |

## Notes on missing lanes (the `—` cells)

The reasons each cell is empty are also recorded as a comment in the
corresponding `.das` bench file; the bullets below quote that comment.
For deeper detail on SQL cells — what query each would lower to,
whether the gap is window-function / surface-limitation / by-design,
and which gaps could land in a single PR — see
[`sqlite_linq_gaps.md`](sqlite_linq_gaps.md).

- **`chained_select_collapse` SQL** — `_sql` rejects `distinct() |> count()`
  as non-translatable. The equivalent SQL `COUNT(DISTINCT computed-expr)`
  isn't currently emitted by sqlite_linq's surface. By design — no follow-up.
- **`decs_count_bare_pred` SQL / Array** — covers a Theme 4 root-cause
  fix specific to the decs lane (bare `from_decs_template(...).count(P)`
  with no upstream where/select previously bailed because
  `forExpr.iteratorVariables` was unpopulated). Array-side bare `count(P)`
  was always reachable; SQL `count(P)` is covered by `count_aggregate.das`
  with a where shape. By design.
- **`indexed_lookup` Decs vs Array ratio** — the ratio is reported as
  `0.00×` because array's lane measures the unspliced linear scan
  (~204k ns/op), while decs uses `query(eid)` for O(1) lookup. The
  ratio is mathematically tiny but the comparison is between two
  fundamentally different algorithms — not a like-for-like benchmark.
- **`join_select` SQL** — sqlite_linq's `_select` after `_join` rejects
  bare `_` (only the `into` lambda's parameter names are valid
  receivers; computed projections would need to be inlined into the
  `into` lambda). By design — sqlite_linq surface limitation.
- **`reverse_distinct_by` Decs** — splice arm is array-only
  (`array_source` predicate in `daslib/linq_fold.das:3221`) because
  backward-index walk has no archetype analog. SQL lane closed by
  Arm B of this PR via MAX(pk) bare-aggregate.
- **`order_distinct_take` Decs vs Array ratio** — m4 (95.5 INTERP /
  74.9 JIT) vs m3f (15.8 INTERP / 2.1 JIT) is NOT apples-to-apples.
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
- **`zip_reverse_to_array` SQL / Decs** — same `zip-not-relational`
  reason as `zip_count_pred`. This row exercises the Theme 8 audit C4
  splice — trailing `reverse()` on the zip array lane emits
  `_::reverse_inplace` after the zip lockstep buffer fill. By design,
  no follow-up.

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
