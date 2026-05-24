# Benchmarks — SQL / Array / Decs comparison

Generated 2026-05-23 from master `31e4339a8`. Fixture size: n = 100 000
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
| `aggregate_match` | 34.4 | 6.0 | 5.9 | 0.98× |
| `all_match` | 27.6 | 3.7 | 3.5 | 0.95× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 29.5 | 5.9 | 8.8 | 1.49× |
| `bare_order_where` | 275.1 | 118.3 | 126.0 | 1.07× |
| `chained_where` | 36.0 | 6.7 | 6.6 | 0.99× |
| `contains_match` | 0.00 | 2.2 | 1.4 | 0.64× |
| `count_aggregate` | 29.7 | 4.1 | 4.1 | 1.00× |
| `distinct_by_count` | 41.0 | 15.6 | 15.9 | 1.02× |
| `distinct_count` | 41.2 | 15.9 | 15.8 | 0.99× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 170.6 | 29.9 | 30.0 | 1.00× |
| `groupby_count` | 140.3 | 19.2 | 19.3 | 1.01× |
| `groupby_first` | — | 18.4 | 19.1 | 1.04× |
| `groupby_having_count` | 139.9 | 19.1 | 19.2 | 1.01× |
| `groupby_having_hidden_sum` | 174.7 | 24.3 | 24.0 | 0.99× |
| `groupby_max` | 173.4 | 24.9 | 25.2 | 1.01× |
| `groupby_min` | 172.4 | 24.9 | 25.2 | 1.01× |
| `groupby_multi_reducer` | 188.8 | 32.2 | 32.3 | 1.00× |
| `groupby_select_sum` | 200.4 | 36.5 | 36.3 | 0.99× |
| `groupby_sum` | 169.3 | 18.6 | 18.6 | 1.00× |
| `groupby_where_count` | 75.6 | 14.6 | 14.8 | 1.01× |
| `groupby_where_sum` | 85.9 | 14.1 | 14.6 | 1.04× |
| `indexed_lookup` | 1441.6 | 203340.0 | 474.5 | 0.00× |
| `join_count` | 37.9 | 121.3 | — | — |
| `last_match` | 0.00 | 5.8 | 13.8 | 2.38× |
| `long_count_aggregate` | 29.4 | 4.2 | 4.0 | 0.95× |
| `max_aggregate` | 30.9 | 6.0 | 6.9 | 1.15× |
| `min_aggregate` | 31.0 | 6.1 | 6.9 | 1.13× |
| `order_take_desc` | 38.2 | 15.8 | 19.9 | 1.26× |
| `reverse_take` | 0.10 | 0.00 | 9.2 | — |
| `select_count` | 0.10 | 0.00 | 2.2 | — |
| `select_where` | 196.8 | 11.1 | 19.6 | 1.77× |
| `select_where_count` | 32.6 | 5.2 | 7.4 | 1.42× |
| `select_where_order_take` | 36.5 | 12.2 | 14.9 | 1.22× |
| `select_where_sum` | 36.8 | 7.5 | 7.5 | 1.00× |
| `single_match` | 0.00 | 2.9 | 5.5 | 1.90× |
| `skip_take` | 0.50 | 0.10 | 0.20 | 2.00× |
| `skip_while_match` | 3.4 | 5.3 | 5.3 | 1.00× |
| `sort_first` | 37.8 | 11.0 | 13.3 | 1.21× |
| `sort_take` | 38.1 | 16.3 | 20.2 | 1.24× |
| `sum_aggregate` | 30.0 | 2.2 | 2.1 | 0.95× |
| `sum_where` | 33.0 | 4.2 | 4.3 | 1.02× |
| `take_count` | 3.6 | 0.20 | 0.40 | 2.00× |
| `take_count_filtered` | — | 0.20 | 0.20 | 1.00× |
| `take_sum_aggregate` | — | 0.10 | 0.10 | 1.00× |
| `take_while_match` | 8.0 | 2.7 | 2.5 | 0.93× |
| `to_array_filter` | 70.0 | 11.6 | 11.7 | 1.01× |
| `zip_dot_product` | — | 8.0 | 4.8 | 0.60× |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | Decs vs Array |
|---|---:|---:|---:|---:|
| `aggregate_match` | 34.4 | 0.40 | 0.70 | 1.75× |
| `all_match` | 27.4 | 0.30 | 0.20 | 0.67× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 30.0 | 1.0 | 3.5 | 3.50× |
| `bare_order_where` | 184.2 | 33.5 | 34.8 | 1.04× |
| `chained_where` | 36.2 | 0.60 | 0.80 | 1.33× |
| `contains_match` | 0.00 | 0.20 | 0.10 | 0.50× |
| `count_aggregate` | 29.4 | 0.30 | 0.60 | 2.00× |
| `distinct_by_count` | 41.0 | 2.1 | 2.1 | 1.00× |
| `distinct_count` | 41.1 | 2.1 | 2.1 | 1.00× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 170.0 | 2.6 | 2.9 | 1.12× |
| `groupby_count` | 140.7 | 2.3 | 2.5 | 1.09× |
| `groupby_first` | — | 2.2 | 3.1 | 1.41× |
| `groupby_having_count` | 140.5 | 2.3 | 2.5 | 1.09× |
| `groupby_having_hidden_sum` | 174.0 | 2.5 | 2.8 | 1.12× |
| `groupby_max` | 175.1 | 2.4 | 2.7 | 1.13× |
| `groupby_min` | 172.6 | 2.4 | 2.7 | 1.13× |
| `groupby_multi_reducer` | 187.8 | 2.7 | 2.9 | 1.07× |
| `groupby_select_sum` | 198.1 | 3.2 | 3.7 | 1.16× |
| `groupby_sum` | 169.5 | 2.4 | 2.7 | 1.13× |
| `groupby_where_count` | 75.0 | 1.7 | 1.8 | 1.06× |
| `groupby_where_sum` | 87.9 | 1.7 | 1.8 | 1.06× |
| `indexed_lookup` | 1238.5 | 35025.4 | 101.7 | 0.00× |
| `join_count` | 37.6 | 35.7 | — | — |
| `last_match` | 0.00 | 0.50 | 1.4 | 2.80× |
| `long_count_aggregate` | 28.8 | 0.30 | 0.60 | 2.00× |
| `max_aggregate` | 30.5 | 0.60 | 0.50 | 0.83× |
| `min_aggregate` | 30.5 | 0.60 | 0.50 | 0.83× |
| `order_take_desc` | 37.6 | 0.70 | 1.3 | 1.86× |
| `reverse_take` | 0.00 | 0.00 | 1.1 | — |
| `select_count` | 0.10 | 0.00 | 0.00 | — |
| `select_where` | 105.3 | 4.1 | 5.5 | 1.34× |
| `select_where_count` | 32.4 | 0.40 | 0.60 | 1.50× |
| `select_where_order_take` | 36.2 | 0.70 | 1.3 | 1.86× |
| `select_where_sum` | 36.6 | 0.50 | 0.60 | 1.20× |
| `single_match` | 0.00 | 0.40 | 1.1 | 2.75× |
| `skip_take` | 0.30 | 0.00 | 0.00 | — |
| `skip_while_match` | 3.4 | 0.40 | 0.40 | 1.00× |
| `sort_first` | 37.5 | 0.40 | 1.3 | 3.25× |
| `sort_take` | 37.6 | 0.70 | 1.3 | 1.86× |
| `sum_aggregate` | 29.8 | 0.40 | 0.30 | 0.75× |
| `sum_where` | 32.3 | 0.40 | 0.60 | 1.50× |
| `take_count` | 1.8 | 0.10 | 0.20 | 2.00× |
| `take_count_filtered` | — | 0.00 | 0.00 | — |
| `take_sum_aggregate` | — | 0.00 | 0.00 | — |
| `take_while_match` | 7.9 | 0.20 | 0.30 | 1.50× |
| `to_array_filter` | 47.6 | 3.2 | 3.4 | 1.06× |
| `zip_dot_product` | — | 0.50 | 0.50 | 1.00× |

## Notes on missing lanes (the `—` cells)

The reasons each cell is empty are also recorded as a comment in the
corresponding `.das` bench file; the bullets below quote that comment.

- **`groupby_first` SQL** — no direct SQL aggregator for "first
  source-order row per group". Window functions (`ROW_NUMBER() OVER
  (PARTITION BY brand ORDER BY id)` + outer `WHERE rn=1`) would be the
  SQL equivalent; sqlite_linq does not currently lower window
  functions. Follow-up TODO 2026-05-23.
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
