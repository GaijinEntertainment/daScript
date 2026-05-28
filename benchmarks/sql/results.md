# Benchmarks — SQL / Array / Decs comparison

Generated 2026-05-27 from PR `bbatkin/sqlite-linq-distinct-by-composition` (sqlite_linq `_distinct_by` composition + `reverse() |> _distinct_by` MAX(pk) + `_group_by |> first()` row projection). Four SQL `—` cells now populated, draining the entire "window-function lowerings" group from [`sqlite_linq_gaps.md`](sqlite_linq_gaps.md) — none of them actually needed window functions, all lower via SQLite's bare-aggregate optimization with outer ORDER BY / LIMIT or MAX(pk) variant:
- `distinct_by_order_take` m1 = 240.5 INTERP / 253.6 JIT
- `distinct_by_order_to_array` m1 = 240.6 INTERP / 239.3 JIT
- `reverse_distinct_by` m1 = 312.5 INTERP / 297.1 JIT (MAX(pk) variant)
- `groupby_first` m1 = 255.1 INTERP / 252.4 JIT (tuple-with-row projection)

Drift is minor and mostly converges back toward pre-#2906 baselines: `distinct_by_order_take` m4 INTERP 30.9 → 23.8 (-23%) and `groupby_select_order` m4 INTERP 24.2 → 18.8 (-22%) reverse the post-#2906 drift entries. `indexed_lookup` m4 now populated (482.9 INTERP / 107.5 JIT) — earlier `—` was stale bench-output parsing. Remaining JIT cells drift ±5% at measurement floor.
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
| `aggregate_match` | 34.8 | 6.0 | 5.9 | 0.98× |
| `all_match` | 27.8 | 3.6 | 3.5 | 0.97× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 30.2 | 5.9 | 8.8 | 1.49× |
| `bare_order_where` | 276.7 | 117.6 | 123.5 | 1.05× |
| `chained_select_collapse` | — | 17.9 | 17.6 | 0.98× |
| `chained_where` | 36.9 | 6.7 | 7.1 | 1.06× |
| `contains_match` | 0.00 | 2.2 | 1.4 | 0.64× |
| `count_aggregate` | 29.8 | 4.2 | 4.1 | 0.98× |
| `decs_count_bare_pred` | — | — | 4.2 | — |
| `distinct_by_count` | 41.4 | 15.7 | 15.7 | 1.00× |
| `distinct_by_order_take` | 240.5 | 21.9 | 23.8 | 1.09× |
| `distinct_by_order_to_array` | 240.6 | 22.2 | 23.3 | 1.05× |
| `distinct_count` | 42.0 | 16.1 | 15.9 | 0.99× |
| `distinct_count_pred` | 253.2 | 15.8 | 16.1 | 1.02× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 172.6 | 30.1 | 30.3 | 1.01× |
| `groupby_count` | 142.5 | 19.4 | 19.5 | 1.01× |
| `groupby_first` | 255.1 | 18.5 | 19.2 | 1.04× |
| `groupby_having_count` | 143.0 | 19.3 | 19.2 | 0.99× |
| `groupby_having_hidden_sum` | 176.0 | 23.8 | 24.4 | 1.03× |
| `groupby_having_post_where` | 172.7 | 18.6 | 18.8 | 1.01× |
| `groupby_max` | 175.5 | 25.2 | 25.2 | 1.00× |
| `groupby_min` | 173.2 | 25.5 | 25.8 | 1.01× |
| `groupby_multi_reducer` | 191.2 | 32.1 | 32.5 | 1.01× |
| `groupby_select_order` | 175.4 | 18.6 | 18.8 | 1.01× |
| `groupby_select_sum` | 202.2 | 36.5 | 35.8 | 0.98× |
| `groupby_sum` | 175.9 | 18.7 | 18.6 | 0.99× |
| `groupby_where_count` | 75.5 | 14.5 | 14.9 | 1.03× |
| `groupby_where_sum` | 86.4 | 14.2 | 14.5 | 1.02× |
| `indexed_lookup` | 1442.6 | 202815.2 | 482.9 | 0.00× |
| `join_count` | 38.5 | 120.6 | 64.0 | 0.53× |
| `join_groupby_count` | — | 186.6 | 91.7 | 0.49× |
| `join_groupby_to_array` | — | 217.1 | 92.6 | 0.43× |
| `join_select` | — | 148.7 | 85.8 | 0.58× |
| `join_where_count` | 39.4 | 147.2 | 74.2 | 0.50× |
| `last_match` | 0.00 | 5.8 | 14.0 | 2.41× |
| `long_count_aggregate` | 29.7 | 4.2 | 4.1 | 0.98× |
| `max_aggregate` | 31.3 | 6.0 | 6.9 | 1.15× |
| `min_aggregate` | 31.2 | 6.1 | 6.9 | 1.13× |
| `order_distinct_take` | 140.2 | 15.9 | 94.8 | 5.96× |
| `order_reverse_normalized` | 38.4 | 16.2 | 20.1 | 1.24× |
| `order_take_desc` | 38.4 | 16.0 | 20.0 | 1.25× |
| `reverse_distinct_by` | 312.5 | 21.8 | — | — |
| `reverse_take` | 0.1 | 0.00 | 10.1 | — |
| `reverse_take_select` | 0.00 | 34.6 | 48.4 | 1.40× |
| `select_count` | 0.1 | 0.00 | 2.2 | — |
| `select_where` | 193.8 | 11.3 | 19.8 | 1.75× |
| `select_where_count` | 33.1 | 5.2 | 7.4 | 1.42× |
| `select_where_order_take` | 37.0 | 12.3 | 15.0 | 1.22× |
| `select_where_sum` | 37.3 | 7.5 | 7.5 | 1.00× |
| `single_match` | 0.00 | 2.9 | 5.6 | 1.93× |
| `skip_take` | 0.5 | 0.1 | 0.2 | 2.00× |
| `skip_while_match` | 3.5 | 5.2 | 5.3 | 1.02× |
| `sort_first` | 38.1 | 11.0 | 13.5 | 1.23× |
| `sort_take` | 38.7 | 16.5 | 20.7 | 1.25× |
| `sort_take_select` | 38.4 | 16.1 | 20.0 | 1.24× |
| `sum_aggregate` | 30.2 | 2.2 | 2.1 | 0.95× |
| `sum_where` | 32.9 | 4.3 | 4.3 | 1.00× |
| `take_count` | 3.6 | 0.2 | 0.5 | 2.50× |
| `take_count_filtered` | — | 0.2 | 0.2 | 1.00× |
| `take_sum_aggregate` | — | 0.1 | 0.1 | 1.00× |
| `take_where_count` | — | 0.1 | 0.1 | 1.00× |
| `take_while_match` | 7.8 | 2.4 | 2.4 | 1.00× |
| `to_array_filter` | 70.2 | 11.7 | 11.8 | 1.01× |
| `zip_count_pred` | — | 15.2 | — | — |
| `zip_dot_product` | — | 12.6 | 10.6 | 0.84× |
| `zip_dot_product_3arg` | — | 12.7 | — | — |
| `zip_reverse_to_array` | — | 31.2 | — | — |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | Decs vs Array |
|---|---:|---:|---:|---:|
| `aggregate_match` | 34.8 | 0.4 | 0.7 | 1.75× |
| `all_match` | 27.7 | 0.3 | 0.2 | 0.67× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 30.2 | 1.0 | 3.6 | 3.60× |
| `bare_order_where` | 185.6 | 34.1 | 34.1 | 1.00× |
| `chained_select_collapse` | — | 2.1 | 2.1 | 1.00× |
| `chained_where` | 36.9 | 0.6 | 0.9 | 1.50× |
| `contains_match` | 0.00 | 0.2 | 0.1 | 0.50× |
| `count_aggregate` | 29.7 | 0.4 | 0.6 | 1.50× |
| `decs_count_bare_pred` | — | — | 0.6 | — |
| `distinct_by_count` | 42.0 | 2.1 | 2.1 | 1.00× |
| `distinct_by_order_take` | 253.6 | 2.6 | 3.2 | 1.23× |
| `distinct_by_order_to_array` | 239.3 | 2.7 | 3.2 | 1.19× |
| `distinct_count` | 42.2 | 2.1 | 2.1 | 1.00× |
| `distinct_count_pred` | 253.0 | 2.1 | 2.3 | 1.10× |
| `distinct_take` | 0.00 | 0.1 | 0.00 | 0.00× |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 171.9 | 2.6 | 2.9 | 1.12× |
| `groupby_count` | 142.9 | 2.4 | 2.5 | 1.04× |
| `groupby_first` | 252.4 | 2.2 | 3.1 | 1.41× |
| `groupby_having_count` | 142.3 | 2.4 | 2.5 | 1.04× |
| `groupby_having_hidden_sum` | 176.1 | 2.5 | 2.8 | 1.12× |
| `groupby_having_post_where` | 172.4 | 2.4 | 2.7 | 1.13× |
| `groupby_max` | 173.8 | 2.4 | 2.7 | 1.13× |
| `groupby_min` | 181.6 | 2.4 | 2.7 | 1.13× |
| `groupby_multi_reducer` | 190.9 | 2.7 | 3.0 | 1.11× |
| `groupby_select_order` | 171.5 | 2.4 | 2.7 | 1.13× |
| `groupby_select_sum` | 199.7 | 3.2 | 3.7 | 1.16× |
| `groupby_sum` | 172.5 | 2.4 | 2.7 | 1.13× |
| `groupby_where_count` | 76.2 | 1.5 | 1.8 | 1.20× |
| `groupby_where_sum` | 86.9 | 1.5 | 1.8 | 1.20× |
| `indexed_lookup` | 1265.6 | 35161.7 | 107.5 | 0.00× |
| `join_count` | 38.4 | 34.5 | 12.8 | 0.37× |
| `join_groupby_count` | — | 47.1 | 21.9 | 0.46× |
| `join_groupby_to_array` | — | 54.7 | 21.9 | 0.40× |
| `join_select` | — | 41.8 | 22.6 | 0.54× |
| `join_where_count` | 39.5 | 41.5 | 22.9 | 0.55× |
| `last_match` | 0.00 | 0.5 | 1.4 | 2.80× |
| `long_count_aggregate` | 29.5 | 0.4 | 0.6 | 1.50× |
| `max_aggregate` | 31.1 | 0.6 | 0.5 | 0.83× |
| `min_aggregate` | 31.2 | 0.6 | 0.5 | 0.83× |
| `order_distinct_take` | 138.7 | 2.1 | 75.1 | 35.76× |
| `order_reverse_normalized` | 38.3 | 0.7 | 1.3 | 1.86× |
| `order_take_desc` | 38.2 | 0.7 | 1.3 | 1.86× |
| `reverse_distinct_by` | 297.1 | 2.7 | — | — |
| `reverse_take` | 0.00 | 0.00 | 1.1 | — |
| `reverse_take_select` | 0.00 | 8.7 | 11.2 | 1.29× |
| `select_count` | 0.1 | 0.00 | 0.00 | — |
| `select_where` | 106.1 | 4.3 | 5.7 | 1.33× |
| `select_where_count` | 32.9 | 0.4 | 0.6 | 1.50× |
| `select_where_order_take` | 36.8 | 0.7 | 1.4 | 2.00× |
| `select_where_sum` | 37.6 | 0.5 | 0.6 | 1.20× |
| `single_match` | 0.00 | 0.4 | 1.1 | 2.75× |
| `skip_take` | 0.3 | 0.00 | 0.00 | — |
| `skip_while_match` | 3.5 | 0.4 | 0.4 | 1.00× |
| `sort_first` | 38.2 | 0.4 | 1.3 | 3.25× |
| `sort_take` | 38.3 | 0.7 | 1.4 | 2.00× |
| `sort_take_select` | 38.4 | 0.7 | 1.4 | 2.00× |
| `sum_aggregate` | 30.4 | 0.4 | 0.3 | 0.75× |
| `sum_where` | 32.9 | 0.4 | 0.6 | 1.50× |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.00× |
| `take_count_filtered` | — | 0.00 | 0.00 | — |
| `take_sum_aggregate` | — | 0.00 | 0.00 | — |
| `take_where_count` | — | 0.00 | 0.00 | — |
| `take_while_match` | 7.8 | 0.2 | 0.3 | 1.50× |
| `to_array_filter` | 48.3 | 3.3 | 3.4 | 1.03× |
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
- **`join_groupby_count` SQL** — sqlite_linq's `_group_by` after `_join`
  doesn't currently lower (group key column is from the joined
  projection, not a base-table column). Follow-up TODO 2026-05-25.
- **`join_groupby_to_array` SQL** — same reason as `join_groupby_count`.
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
