# Benchmarks — SQL / Array / Decs comparison

Generated 2026-05-28 from PR `bbatkin/linq-fold-array-join-splice` (chunk N+3 — linq_fold array-side `_join` splice + cross-arm `_join |> _group_by` via new `ArrayJoin` SourceAdapter) + follow-up `bbatkin/linq-fold-join-emit-dedup` (refactor: shared standalone + adapter helpers). Closes the m3f-vs-m4 parity gap across the entire `join_*` family — all 5 cells m3f beats m4 in both INTERP and JIT:

| Cell | m3f INTERP before / after | m3f JIT before / after |
|---|---:|---:|
| `join_count` | 128.2 → **51.0** | 34.6 → **11.6** |
| `join_select` | 148.1 → **72.0** | 41.6 → **19.7** |
| `join_where_count` | 148.5 → **60.1** | 41.2 → **20.4** |
| `join_groupby_count` | 186.2 → **78.3** | 47.2 → **19.4** |
| `join_groupby_to_array` | 217.8 → **78.1** | 55.5 → **19.5** |

Drift across the rest of the matrix is at measurement floor (±5% INTERP, ±10% JIT). The splice is gated on `array_source` + primitive equi-key + (for cross-arm) `upstream_join` capture, so non-join chains aren't touched.
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
| `aggregate_match` | 32.3 | 5.8 | 5.8 | 1.00× |
| `all_match` | 26.3 | 3.5 | 3.5 | 1.00× |
| `any_match` | 0.0 | 0.0 | 0.0 | — |
| `average_aggregate` | 29.0 | 5.9 | 8.8 | 1.49× |
| `bare_order_where` | 270.0 | 115.2 | 124.7 | 1.08× |
| `chained_select_collapse` | — | 17.7 | 17.4 | 0.98× |
| `chained_where` | 35.5 | 6.6 | 7.1 | 1.08× |
| `contains_match` | 0.0 | 2.2 | 1.4 | 0.64× |
| `count_aggregate` | 28.3 | 4.1 | 4.0 | 0.98× |
| `decs_count_bare_pred` | — | — | 4.1 | — |
| `distinct_by_count` | 39.8 | 15.5 | 15.7 | 1.01× |
| `distinct_by_order_take` | 247.9 | 21.6 | 23.9 | 1.11× |
| `distinct_by_order_to_array` | 236.5 | 21.8 | 23.2 | 1.06× |
| `distinct_count` | 40.2 | 15.8 | 15.9 | 1.01× |
| `distinct_count_pred` | 246.2 | 15.7 | 15.8 | 1.01× |
| `distinct_take` | 0.0 | 0.0 | 0.0 | — |
| `element_at_match` | 0.0 | 0.0 | 0.0 | — |
| `first_match` | 0.0 | 0.0 | 0.0 | — |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | — |
| `groupby_average` | 170.1 | 29.7 | 29.7 | 1.00× |
| `groupby_count` | 139.6 | 19.2 | 19.1 | 0.99× |
| `groupby_first` | 246.7 | 18.3 | 19.0 | 1.04× |
| `groupby_having_count` | 139.3 | 19.0 | 19.0 | 1.00× |
| `groupby_having_hidden_sum` | 173.2 | 23.6 | 24.0 | 1.02× |
| `groupby_having_post_where` | 168.0 | 18.9 | 18.6 | 0.98× |
| `groupby_max` | 171.2 | 24.9 | 25.0 | 1.00× |
| `groupby_min` | 170.2 | 25.4 | 25.6 | 1.01× |
| `groupby_multi_reducer` | 195.4 | 31.8 | 32.2 | 1.01× |
| `groupby_select_order` | 168.0 | 18.5 | 18.5 | 1.00× |
| `groupby_select_sum` | 197.7 | 35.6 | 35.7 | 1.00× |
| `groupby_sum` | 168.5 | 18.4 | 18.4 | 1.00× |
| `groupby_where_count` | 74.2 | 14.4 | 14.8 | 1.03× |
| `groupby_where_sum` | 84.7 | 14.1 | 14.4 | 1.02× |
| `indexed_lookup` | 1453.2 | 202198.9 | 470.2 | 0.00× |
| `join_count` | 36.8 | 51.0 | 63.6 | 1.25× |
| `join_groupby_count` | 153.9 | 78.3 | 90.1 | 1.15× |
| `join_groupby_to_array` | 186.0 | 78.1 | 91.4 | 1.17× |
| `join_select` | — | 72.0 | 83.8 | 1.16× |
| `join_where_count` | 38.3 | 60.1 | 74.3 | 1.24× |
| `last_match` | 0.0 | 5.7 | 13.9 | 2.44× |
| `long_count_aggregate` | 28.1 | 4.0 | 4.0 | 1.00× |
| `max_aggregate` | 29.3 | 6.0 | 6.8 | 1.13× |
| `min_aggregate` | 29.2 | 6.0 | 6.7 | 1.12× |
| `order_distinct_take` | 135.7 | 15.6 | 93.6 | 6.00× |
| `order_reverse_normalized` | 36.4 | 16.0 | 19.8 | 1.24× |
| `order_take_desc` | 36.3 | 15.9 | 19.8 | 1.25× |
| `reverse_distinct_by` | 284.8 | 21.3 | — | — |
| `reverse_take` | 0.1 | 0.0 | 10.0 | — |
| `reverse_take_select` | 0.0 | 33.4 | 47.4 | 1.42× |
| `select_count` | 0.1 | 0.0 | 2.2 | — |
| `select_where` | 190.8 | 10.9 | 19.1 | 1.75× |
| `select_where_count` | 31.4 | 5.1 | 7.3 | 1.43× |
| `select_where_order_take` | 35.3 | 12.1 | 14.7 | 1.21× |
| `select_where_sum` | 35.8 | 7.4 | 7.4 | 1.00× |
| `single_match` | 0.0 | 2.9 | 5.4 | 1.86× |
| `skip_take` | 0.5 | 0.1 | 0.2 | 2.00× |
| `skip_while_match` | 3.4 | 5.2 | 5.3 | 1.02× |
| `sort_first` | 36.6 | 10.9 | 13.2 | 1.21× |
| `sort_take` | 38.8 | 16.2 | 20.5 | 1.27× |
| `sort_take_select` | 36.9 | 16.0 | 19.8 | 1.24× |
| `sum_aggregate` | 29.4 | 2.2 | 2.1 | 0.95× |
| `sum_where` | 31.4 | 4.2 | 4.2 | 1.00× |
| `take_count` | 3.6 | 0.2 | 0.4 | 2.00× |
| `take_count_filtered` | — | 0.2 | 0.2 | 1.00× |
| `take_sum_aggregate` | — | 0.1 | 0.1 | 1.00× |
| `take_where_count` | — | 0.1 | 0.1 | 1.00× |
| `take_while_match` | 7.5 | 2.4 | 2.4 | 1.00× |
| `to_array_filter` | 68.0 | 11.5 | 11.6 | 1.01× |
| `zip_count_pred` | — | 15.1 | — | — |
| `zip_dot_product` | — | 12.5 | 10.9 | 0.87× |
| `zip_dot_product_3arg` | — | 12.8 | — | — |
| `zip_reverse_to_array` | — | 31.1 | — | — |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | Decs vs Array |
|---|---:|---:|---:|---:|
| `aggregate_match` | 33.8 | 0.4 | 0.7 | 1.75× |
| `all_match` | 27.8 | 0.3 | 0.2 | 0.67× |
| `any_match` | 0.0 | 0.0 | 0.0 | — |
| `average_aggregate` | 29.0 | 1.0 | 3.5 | 3.50× |
| `bare_order_where` | 177.2 | 33.9 | 35.2 | 1.04× |
| `chained_select_collapse` | — | 2.1 | 2.1 | 1.00× |
| `chained_where` | 35.3 | 0.6 | 0.9 | 1.50× |
| `contains_match` | 0.0 | 0.2 | 0.1 | 0.50× |
| `count_aggregate` | 28.9 | 0.4 | 0.6 | 1.50× |
| `decs_count_bare_pred` | — | — | 0.6 | — |
| `distinct_by_count` | 40.1 | 2.1 | 2.1 | 1.00× |
| `distinct_by_order_take` | 235.3 | 2.6 | 3.2 | 1.23× |
| `distinct_by_order_to_array` | 238.8 | 2.7 | 3.3 | 1.22× |
| `distinct_count` | 40.5 | 2.1 | 2.1 | 1.00× |
| `distinct_count_pred` | 246.8 | 2.1 | 2.2 | 1.05× |
| `distinct_take` | 0.0 | 0.1 | 0.0 | 0.00× |
| `element_at_match` | 0.0 | 0.0 | 0.0 | — |
| `first_match` | 0.0 | 0.0 | 0.0 | — |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | — |
| `groupby_average` | 168.6 | 2.6 | 2.9 | 1.12× |
| `groupby_count` | 141.0 | 2.3 | 2.5 | 1.09× |
| `groupby_first` | 250.4 | 2.2 | 3.1 | 1.41× |
| `groupby_having_count` | 146.9 | 2.3 | 2.5 | 1.09× |
| `groupby_having_hidden_sum` | 171.6 | 2.5 | 2.8 | 1.12× |
| `groupby_having_post_where` | 167.2 | 2.4 | 2.6 | 1.08× |
| `groupby_max` | 168.0 | 2.4 | 2.6 | 1.08× |
| `groupby_min` | 167.5 | 2.4 | 2.6 | 1.08× |
| `groupby_multi_reducer` | 189.6 | 2.7 | 2.9 | 1.07× |
| `groupby_select_order` | 168.3 | 2.4 | 2.6 | 1.08× |
| `groupby_select_sum` | 190.1 | 3.2 | 3.6 | 1.12× |
| `groupby_sum` | 165.7 | 2.7 | 2.7 | 1.00× |
| `groupby_where_count` | 73.8 | 1.4 | 1.7 | 1.21× |
| `groupby_where_sum` | 84.4 | 1.5 | 1.8 | 1.20× |
| `indexed_lookup` | 1224.3 | 34689.8 | 106.0 | 0.00× |
| `join_count` | 36.7 | 11.6 | 12.7 | 1.09× |
| `join_groupby_count` | 153.3 | 19.4 | 21.6 | 1.11× |
| `join_groupby_to_array` | 187.2 | 19.5 | 21.6 | 1.11× |
| `join_select` | — | 19.7 | 22.2 | 1.13× |
| `join_where_count` | 38.2 | 20.4 | 22.5 | 1.10× |
| `last_match` | 0.0 | 0.5 | 1.4 | 2.80× |
| `long_count_aggregate` | 28.0 | 0.4 | 0.6 | 1.50× |
| `max_aggregate` | 29.5 | 0.6 | 0.5 | 0.83× |
| `min_aggregate` | 29.4 | 0.6 | 0.5 | 0.83× |
| `order_distinct_take` | 136.1 | 2.1 | 73.9 | 35.19× |
| `order_reverse_normalized` | 36.5 | 0.7 | 1.3 | 1.86× |
| `order_take_desc` | 36.5 | 0.7 | 1.3 | 1.86× |
| `reverse_distinct_by` | 289.8 | 2.6 | — | — |
| `reverse_take` | 0.0 | 0.0 | 1.1 | — |
| `reverse_take_select` | 0.0 | 8.1 | 9.9 | 1.22× |
| `select_count` | 0.1 | 0.0 | 0.0 | — |
| `select_where` | 105.0 | 4.1 | 5.3 | 1.29× |
| `select_where_count` | 31.3 | 0.4 | 0.6 | 1.50× |
| `select_where_order_take` | 35.2 | 0.7 | 1.3 | 1.86× |
| `select_where_sum` | 35.8 | 0.5 | 0.6 | 1.20× |
| `single_match` | 0.0 | 0.4 | 1.1 | 2.75× |
| `skip_take` | 0.3 | 0.0 | 0.0 | — |
| `skip_while_match` | 3.3 | 0.4 | 0.4 | 1.00× |
| `sort_first` | 36.4 | 0.4 | 1.3 | 3.25× |
| `sort_take` | 36.4 | 0.7 | 1.3 | 1.86× |
| `sort_take_select` | 37.1 | 0.7 | 1.4 | 2.00× |
| `sum_aggregate` | 28.4 | 0.4 | 0.3 | 0.75× |
| `sum_where` | 31.1 | 0.4 | 0.6 | 1.50× |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.00× |
| `take_count_filtered` | — | 0.0 | 0.0 | — |
| `take_sum_aggregate` | — | 0.0 | 0.0 | — |
| `take_where_count` | — | 0.0 | 0.0 | — |
| `take_while_match` | 7.4 | 0.2 | 0.3 | 1.50× |
| `to_array_filter` | 45.7 | 3.1 | 3.2 | 1.03× |
| `zip_count_pred` | — | 0.6 | — | — |
| `zip_dot_product` | — | 0.5 | 0.5 | 1.00× |
| `zip_dot_product_3arg` | — | 0.5 | — | — |
| `zip_reverse_to_array` | — | 4.3 | — | — |

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
