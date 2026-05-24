# Benchmarks — SQL / Array / Decs comparison

Generated 2026-05-23 from `62336a4a7` (PR for `plan_decs_join`).
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
| `aggregate_match` | 35.3 | 5.9 | 5.8 | 0.98× |
| `all_match` | 28.1 | 3.6 | 3.5 | 0.97× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 29.9 | 5.9 | 8.8 | 1.49× |
| `bare_order_where` | 278.2 | 118.4 | 126.6 | 1.07× |
| `chained_where` | 38.5 | 6.7 | 6.7 | 1.00× |
| `contains_match` | 0.00 | 2.2 | 1.4 | 0.64× |
| `count_aggregate` | 29.1 | 4.1 | 4.2 | 1.02× |
| `distinct_by_count` | 40.6 | 15.8 | 16.0 | 1.01× |
| `distinct_count` | 41.1 | 16.1 | 16.0 | 0.99× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 174.3 | 30.3 | 30.2 | 1.00× |
| `groupby_count` | 144.3 | 19.4 | 19.3 | 0.99× |
| `groupby_first` | — | 20.0 | 19.3 | 0.97× |
| `groupby_having_count` | 142.6 | 19.3 | 19.4 | 1.01× |
| `groupby_having_hidden_sum` | 175.7 | 24.5 | 24.1 | 0.98× |
| `groupby_max` | 176.7 | 25.0 | 25.4 | 1.02× |
| `groupby_min` | 175.7 | 25.1 | 25.4 | 1.01× |
| `groupby_multi_reducer` | 191.4 | 33.7 | 32.7 | 0.97× |
| `groupby_select_sum` | 207.1 | 36.8 | 36.7 | 1.00× |
| `groupby_sum` | 172.4 | 18.8 | 18.8 | 1.00× |
| `groupby_where_count` | 75.7 | 14.7 | 15.0 | 1.02× |
| `groupby_where_sum` | 86.7 | 14.3 | 14.7 | 1.03× |
| `indexed_lookup` | 1454.5 | 204673.2 | 472.2 | 0.00× |
| `join_count` | 38.0 | 121.2 | 64.0 | 0.53× |
| `last_match` | 0.00 | 5.9 | 14.0 | 2.37× |
| `long_count_aggregate` | 29.3 | 4.2 | 4.2 | 1.00× |
| `max_aggregate` | 30.8 | 6.1 | 6.9 | 1.13× |
| `min_aggregate` | 30.4 | 6.2 | 6.9 | 1.11× |
| `order_take_desc` | 38.1 | 15.9 | 20.1 | 1.26× |
| `reverse_take` | 0.10 | 0.00 | 9.3 | — |
| `select_count` | 0.10 | 0.00 | 2.2 | — |
| `select_where` | 194.2 | 11.1 | 19.5 | 1.76× |
| `select_where_count` | 32.5 | 5.2 | 7.4 | 1.42× |
| `select_where_order_take` | 36.4 | 12.2 | 14.9 | 1.22× |
| `select_where_sum` | 37.0 | 7.5 | 7.5 | 1.00× |
| `single_match` | 0.00 | 2.9 | 5.5 | 1.90× |
| `skip_take` | 0.50 | 0.10 | 0.20 | 2.00× |
| `skip_while_match` | 3.4 | 5.3 | 5.3 | 1.00× |
| `sort_first` | 37.9 | 11.1 | 13.4 | 1.21× |
| `sort_take` | 38.1 | 16.4 | 20.3 | 1.24× |
| `sum_aggregate` | 30.0 | 2.2 | 2.1 | 0.95× |
| `sum_where` | 32.8 | 4.3 | 4.3 | 1.00× |
| `take_count` | 3.6 | 0.20 | 0.40 | 2.00× |
| `take_count_filtered` | — | 0.20 | 0.20 | 1.00× |
| `take_sum_aggregate` | — | 0.10 | 0.10 | 1.00× |
| `take_while_match` | 7.9 | 2.5 | 2.5 | 1.00× |
| `to_array_filter` | 70.1 | 11.7 | 11.9 | 1.02× |
| `zip_dot_product` | — | 8.1 | 4.8 | 0.59× |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | Decs vs Array |
|---|---:|---:|---:|---:|
| `aggregate_match` | 34.4 | 0.40 | 0.70 | 1.75× |
| `all_match` | 27.4 | 0.30 | 0.20 | 0.67× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 29.7 | 1.0 | 3.6 | 3.60× |
| `bare_order_where` | 185.9 | 33.7 | 35.0 | 1.04× |
| `chained_where` | 35.9 | 0.60 | 0.80 | 1.33× |
| `contains_match` | 0.00 | 0.20 | 0.10 | 0.50× |
| `count_aggregate` | 29.0 | 0.40 | 0.60 | 1.50× |
| `distinct_by_count` | 40.8 | 2.1 | 2.1 | 1.00× |
| `distinct_count` | 41.0 | 2.1 | 2.1 | 1.00× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 170.7 | 2.6 | 2.9 | 1.12× |
| `groupby_count` | 141.1 | 2.4 | 2.5 | 1.04× |
| `groupby_first` | — | 2.2 | 3.1 | 1.41× |
| `groupby_having_count` | 147.0 | 2.4 | 2.5 | 1.04× |
| `groupby_having_hidden_sum` | 174.1 | 2.5 | 2.8 | 1.12× |
| `groupby_max` | 172.0 | 2.4 | 2.7 | 1.13× |
| `groupby_min` | 174.2 | 2.4 | 2.7 | 1.13× |
| `groupby_multi_reducer` | 191.1 | 2.7 | 3.0 | 1.11× |
| `groupby_select_sum` | 198.8 | 3.2 | 3.7 | 1.16× |
| `groupby_sum` | 173.6 | 2.4 | 2.7 | 1.13× |
| `groupby_where_count` | 75.6 | 1.7 | 1.8 | 1.06× |
| `groupby_where_sum` | 86.8 | 1.7 | 1.8 | 1.06× |
| `indexed_lookup` | 1266.6 | 36139.0 | 104.1 | 0.00× |
| `join_count` | 38.0 | 36.2 | 13.3 | 0.37× |
| `last_match` | 0.00 | 0.60 | 1.4 | 2.33× |
| `long_count_aggregate` | 29.3 | 0.40 | 0.60 | 1.50× |
| `max_aggregate` | 30.6 | 0.60 | 0.50 | 0.83× |
| `min_aggregate` | 30.7 | 0.60 | 0.50 | 0.83× |
| `order_take_desc` | 37.8 | 0.70 | 1.4 | 2.00× |
| `reverse_take` | 0.00 | 0.00 | 1.1 | — |
| `select_count` | 0.10 | 0.00 | 0.00 | — |
| `select_where` | 105.6 | 4.1 | 5.5 | 1.34× |
| `select_where_count` | 32.4 | 0.40 | 0.60 | 1.50× |
| `select_where_order_take` | 36.4 | 0.70 | 1.4 | 2.00× |
| `select_where_sum` | 36.9 | 0.50 | 0.60 | 1.20× |
| `single_match` | 0.00 | 0.40 | 1.1 | 2.75× |
| `skip_take` | 0.30 | 0.00 | 0.00 | — |
| `skip_while_match` | 3.5 | 0.40 | 0.40 | 1.00× |
| `sort_first` | 37.5 | 0.40 | 1.3 | 3.25× |
| `sort_take` | 38.0 | 0.70 | 1.4 | 2.00× |
| `sum_aggregate` | 30.3 | 0.40 | 0.30 | 0.75× |
| `sum_where` | 33.0 | 0.40 | 0.60 | 1.50× |
| `take_count` | 1.8 | 0.10 | 0.10 | 1.00× |
| `take_count_filtered` | — | 0.00 | 0.00 | — |
| `take_sum_aggregate` | — | 0.00 | 0.00 | — |
| `take_while_match` | 8.0 | 0.20 | 0.30 | 1.50× |
| `to_array_filter` | 48.3 | 3.2 | 3.4 | 1.06× |
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
