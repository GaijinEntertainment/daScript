# Benchmarks — SQL / Array / Decs comparison

Generated 2026-05-23 from master `30021f183`. Fixture size: n = 100 000
(cars), 100 dealers, 5 brands. Each row is one bench family in
`benchmarks/sql/`; columns are nanoseconds per logical operation.
`—` marks an intentionally absent lane — see "Notes on missing lanes"
below.

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
| `aggregate_match` | 34.7 | 5.9 | 5.8 | 0.98× |
| `all_match` | 27.6 | 3.5 | 3.4 | 0.97× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 29.9 | 5.8 | 8.7 | 1.50× |
| `bare_order_where` | 274.4 | 117.6 | 125.5 | 1.07× |
| `chained_where` | 36.2 | 6.7 | 6.6 | 0.99× |
| `contains_match` | 0.00 | 2.2 | 1.4 | 0.64× |
| `count_aggregate` | 29.4 | 4.1 | 4.1 | 1.00× |
| `distinct_by_count` | 40.5 | 15.6 | 16.0 | 1.03× |
| `distinct_count` | 41.1 | 15.9 | 15.8 | 0.99× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 170.0 | 30.1 | 30.1 | 1.00× |
| `groupby_count` | 140.2 | 19.2 | 19.2 | 1.00× |
| `groupby_first` | — | 18.4 | 19.1 | 1.04× |
| `groupby_having_count` | 140.4 | 19.5 | 19.2 | 0.98× |
| `groupby_having_hidden_sum` | 173.9 | 24.3 | 24.0 | 0.99× |
| `groupby_max` | 176.4 | 24.8 | 25.9 | 1.04× |
| `groupby_min` | 172.1 | 24.8 | 25.2 | 1.02× |
| `groupby_multi_reducer` | 188.7 | 32.3 | 32.3 | 1.00× |
| `groupby_select_sum` | — | 36.5 | 36.0 | 0.99× |
| `groupby_sum` | 173.7 | 18.6 | 18.6 | 1.00× |
| `groupby_where_count` | 74.8 | 14.6 | 14.8 | 1.01× |
| `groupby_where_sum` | 85.9 | 14.1 | 14.6 | 1.04× |
| `indexed_lookup` | 1454.2 | 202887.8 | 479.4 | 0.00× |
| `join_count` | 38.1 | 127.5 | — | — |
| `last_match` | 0.00 | 6.2 | 14.0 | 2.26× |
| `long_count_aggregate` | 29.4 | 4.2 | 4.0 | 0.95× |
| `max_aggregate` | 30.8 | 6.0 | 6.9 | 1.15× |
| `min_aggregate` | 30.8 | 6.1 | 6.9 | 1.13× |
| `order_take_desc` | 37.9 | 15.8 | 19.9 | 1.26× |
| `reverse_take` | 0.10 | 0.00 | 9.2 | — |
| `select_count` | 0.10 | 0.00 | 2.2 | — |
| `select_where` | 194.9 | 11.1 | 19.4 | 1.75× |
| `select_where_count` | 32.4 | 5.2 | 7.4 | 1.42× |
| `select_where_order_take` | 36.1 | 12.1 | 15.1 | 1.25× |
| `select_where_sum` | 36.8 | 7.4 | 7.5 | 1.01× |
| `single_match` | 0.00 | 2.9 | 5.4 | 1.86× |
| `skip_take` | 0.50 | 0.10 | 0.20 | 2.00× |
| `skip_while_match` | 3.4 | 5.3 | 5.3 | 1.00× |
| `sort_first` | 37.4 | 11.0 | 13.3 | 1.21× |
| `sort_take` | 38.0 | 16.2 | 20.1 | 1.24× |
| `sum_aggregate` | 29.7 | 2.1 | 2.1 | 1.00× |
| `sum_where` | 32.8 | 4.2 | 4.3 | 1.02× |
| `take_count` | 3.6 | 0.20 | 0.40 | 2.00× |
| `take_count_filtered` | — | 0.20 | 0.20 | 1.00× |
| `take_sum_aggregate` | — | 0.10 | 0.10 | 1.00× |
| `take_while_match` | 7.9 | 2.4 | 2.5 | 1.04× |
| `to_array_filter` | 69.9 | 11.7 | 11.7 | 1.00× |
| `zip_dot_product` | — | 8.0 | 4.8 | 0.60× |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | Decs vs Array |
|---|---:|---:|---:|---:|
| `aggregate_match` | 34.4 | 0.40 | 0.70 | 1.75× |
| `all_match` | 27.4 | 0.30 | 0.20 | 0.67× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 30.0 | 1.0 | 3.5 | 3.50× |
| `bare_order_where` | 183.7 | 33.5 | 34.7 | 1.04× |
| `chained_where` | 35.8 | 0.60 | 0.80 | 1.33× |
| `contains_match` | 0.00 | 0.20 | 0.10 | 0.50× |
| `count_aggregate` | 29.5 | 0.30 | 0.60 | 2.00× |
| `distinct_by_count` | 40.9 | 2.1 | 2.1 | 1.00× |
| `distinct_count` | 41.1 | 2.1 | 2.1 | 1.00× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 170.0 | 2.6 | 2.9 | 1.12× |
| `groupby_count` | 143.0 | 2.3 | 2.5 | 1.09× |
| `groupby_first` | — | 2.2 | 3.1 | 1.41× |
| `groupby_having_count` | 139.6 | 2.3 | 2.5 | 1.09× |
| `groupby_having_hidden_sum` | 173.4 | 2.5 | 2.8 | 1.12× |
| `groupby_max` | 171.5 | 2.4 | 2.7 | 1.13× |
| `groupby_min` | 172.8 | 2.4 | 2.7 | 1.13× |
| `groupby_multi_reducer` | 187.7 | 2.7 | 2.9 | 1.07× |
| `groupby_select_sum` | — | 3.2 | 3.7 | 1.16× |
| `groupby_sum` | 169.2 | 2.4 | 2.7 | 1.13× |
| `groupby_where_count` | 75.0 | 1.7 | 1.8 | 1.06× |
| `groupby_where_sum` | 85.9 | 1.7 | 1.8 | 1.06× |
| `indexed_lookup` | 1228.9 | 34818.1 | 102.7 | 0.00× |
| `join_count` | 38.0 | 35.8 | — | — |
| `last_match` | 0.00 | 0.50 | 1.4 | 2.80× |
| `long_count_aggregate` | 29.5 | 0.40 | 0.60 | 1.50× |
| `max_aggregate` | 30.6 | 0.70 | 0.50 | 0.71× |
| `min_aggregate` | 31.4 | 0.60 | 0.50 | 0.83× |
| `order_take_desc` | 37.6 | 0.70 | 1.3 | 1.86× |
| `reverse_take` | 0.00 | 0.00 | 1.1 | — |
| `select_count` | 0.10 | 0.00 | 0.00 | — |
| `select_where` | 105.9 | 4.1 | 5.4 | 1.32× |
| `select_where_count` | 32.4 | 0.40 | 0.60 | 1.50× |
| `select_where_order_take` | 36.3 | 0.70 | 1.4 | 2.00× |
| `select_where_sum` | 36.7 | 0.50 | 0.60 | 1.20× |
| `single_match` | 0.00 | 0.40 | 1.1 | 2.75× |
| `skip_take` | 0.30 | 0.00 | 0.00 | — |
| `skip_while_match` | 3.4 | 0.40 | 0.40 | 1.00× |
| `sort_first` | 37.4 | 0.40 | 1.3 | 3.25× |
| `sort_take` | 37.7 | 0.70 | 1.3 | 1.86× |
| `sum_aggregate` | 29.7 | 0.40 | 0.30 | 0.75× |
| `sum_where` | 32.4 | 0.40 | 0.60 | 1.50× |
| `take_count` | 1.8 | 0.10 | 0.10 | 1.00× |
| `take_count_filtered` | — | 0.00 | 0.00 | — |
| `take_sum_aggregate` | — | 0.00 | 0.00 | — |
| `take_while_match` | 7.9 | 0.20 | 0.30 | 1.50× |
| `to_array_filter` | 48.8 | 3.2 | 3.3 | 1.03× |
| `zip_dot_product` | — | 0.50 | 0.50 | 1.00× |

## Notes on missing lanes (the `—` cells)

The reasons each cell is empty are also recorded as a comment in the
corresponding `.das` bench file; the bullets below quote that comment.

- **`groupby_first` SQL** — no direct SQL aggregator for "first
  source-order row per group". Window functions (`ROW_NUMBER() OVER
  (PARTITION BY brand ORDER BY id)` + outer `WHERE rn=1`) would be the
  SQL equivalent; sqlite_linq does not currently lower window
  functions. Follow-up TODO 2026-05-23.
- **`groupby_select_sum` SQL** — sqlite_linq's `_group_by` requires
  `_.Field` or a tuple of `_.Field`s; the expression key (`_ % 100`)
  is rejected. Follow-up TODO 2026-05-23: expression-key support in
  sqlite_linq lowering. Probe error:

  ```
  error[50503]: _sql: _group_by: key must be `_.Field` or
    a tuple of `_.Field`s; got: (_ % 100)
  ```
- **`take_count_filtered` SQL** — by design. In SQL, LIMIT after an
  aggregate has no effect (the aggregate collapses to one row), so the
  bound-then-count shape has no faithful SQL translation. No follow-up.
- **`take_sum_aggregate` SQL** — same reason as `take_count_filtered`.
  By design, no follow-up.
- **`zip_dot_product` SQL** — `zip` is not a relational operation.
  No SQL form exists; by design, no follow-up.
- **`join_count` Decs** — `_join` does not currently lower onto the
  decs archetype walk. Faithfully porting cross-archetype lookup would
  need a Dealer `[decs_template]` plus a hash-by-id index. Follow-up
  TODO 2026-05-23: decs join / cross-archetype lookup machinery in
  `plan_join` (`daslib/linq_fold.das`).

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
