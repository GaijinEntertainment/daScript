# Benchmarks — SQL / Array / Decs comparison

Generated 2026-05-24 from `4b13eed9a` (Theme 2 — trailing-`_where` extension).
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
| `aggregate_match` | 35.1 | 6.0 | 5.8 | 0.97× |
| `all_match` | 27.8 | 3.6 | 3.5 | 0.97× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 29.9 | 6.4 | 10.4 | 1.62× |
| `bare_order_where` | 278.4 | 119.0 | 126.8 | 1.07× |
| `chained_where` | 36.0 | 6.7 | 6.7 | 1.00× |
| `contains_match` | 0.00 | 2.2 | 1.4 | 0.64× |
| `count_aggregate` | 29.2 | 4.1 | 4.1 | 1.00× |
| `distinct_by_count` | 41.0 | 15.7 | 16.1 | 1.03× |
| `distinct_count` | 41.0 | 16.1 | 16.0 | 0.99× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 171.0 | 30.3 | 30.5 | 1.01× |
| `groupby_count` | 143.6 | 19.8 | 19.5 | 0.98× |
| `groupby_first` | — | 18.6 | 19.3 | 1.04× |
| `groupby_having_count` | 142.7 | 19.2 | 19.3 | 1.01× |
| `groupby_having_hidden_sum` | 177.5 | 24.5 | 24.1 | 0.98× |
| `groupby_max` | 176.1 | 25.2 | 25.4 | 1.01× |
| `groupby_min` | 176.4 | 25.1 | 25.4 | 1.01× |
| `groupby_multi_reducer` | 191.6 | 32.5 | 32.6 | 1.00× |
| `groupby_select_sum` | 210.3 | 36.8 | 36.5 | 0.99× |
| `groupby_sum` | 173.0 | 18.8 | 18.8 | 1.00× |
| `groupby_where_count` | 75.6 | 14.8 | 15.0 | 1.01× |
| `groupby_where_sum` | 86.9 | 14.3 | 14.8 | 1.03× |
| `indexed_lookup` | 1499.4 | 204476.6 | 495.4 | 0.00× |
| `join_count` | 38.2 | 122.4 | 64.1 | 0.52× |
| `last_match` | 0.00 | 5.9 | 14.0 | 2.37× |
| `long_count_aggregate` | 29.6 | 4.3 | 4.1 | 0.95× |
| `max_aggregate` | 30.5 | 6.1 | 6.9 | 1.13× |
| `min_aggregate` | 30.4 | 6.4 | 6.9 | 1.08× |
| `order_take_desc` | 37.8 | 16.0 | 20.1 | 1.26× |
| `reverse_take` | 0.10 | 0.00 | 9.3 | — |
| `select_count` | 0.10 | 0.00 | 2.9 | — |
| `select_where` | 193.4 | 11.2 | 22.2 | 1.98× |
| `select_where_count` | 32.6 | 5.2 | 7.4 | 1.42× |
| `select_where_order_take` | 36.3 | 12.2 | 14.8 | 1.21× |
| `select_where_sum` | 37.1 | 7.8 | 7.5 | 0.96× |
| `single_match` | 0.00 | 2.9 | 5.5 | 1.90× |
| `skip_take` | 0.50 | 0.10 | 0.20 | 2.00× |
| `skip_while_match` | 3.5 | 5.3 | 5.3 | 1.00× |
| `sort_first` | 37.9 | 11.5 | 13.5 | 1.17× |
| `sort_take` | 38.1 | 16.4 | 20.4 | 1.24× |
| `sum_aggregate` | 30.1 | 2.2 | 2.1 | 0.95× |
| `sum_where` | 33.0 | 4.3 | 4.3 | 1.00× |
| `take_count` | 3.7 | 0.20 | 0.40 | 2.00× |
| `take_count_filtered` | — | 0.20 | 0.20 | 1.00× |
| `take_sum_aggregate` | — | 0.10 | 0.10 | 1.00× |
| `take_while_match` | 8.0 | 2.5 | 2.5 | 1.00× |
| `to_array_filter` | 70.3 | 11.8 | 11.8 | 1.00× |
| `zip_dot_product` | — | 8.0 | 4.8 | 0.60× |

## JIT
| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | Decs vs Array |
|---|---:|---:|---:|---:|
| `aggregate_match` | 34.3 | 0.40 | 0.70 | 1.75× |
| `all_match` | 27.4 | 0.40 | 0.20 | 0.50× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 29.8 | 1.0 | 3.6 | 3.60× |
| `bare_order_where` | 187.4 | 33.8 | 35.0 | 1.04× |
| `chained_where` | 36.1 | 0.60 | 0.80 | 1.33× |
| `contains_match` | 0.00 | 0.20 | 0.10 | 0.50× |
| `count_aggregate` | 29.1 | 0.40 | 0.60 | 1.50× |
| `distinct_by_count` | 40.8 | 2.1 | 2.1 | 1.00× |
| `distinct_count` | 41.2 | 2.1 | 2.1 | 1.00× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 171.0 | 2.6 | 2.9 | 1.12× |
| `groupby_count` | 142.0 | 2.4 | 2.5 | 1.04× |
| `groupby_first` | — | 2.2 | 3.1 | 1.41× |
| `groupby_having_count` | 141.3 | 2.4 | 2.5 | 1.04× |
| `groupby_having_hidden_sum` | 175.4 | 2.5 | 2.8 | 1.12× |
| `groupby_max` | 172.6 | 2.4 | 2.7 | 1.13× |
| `groupby_min` | 173.8 | 2.4 | 2.7 | 1.13× |
| `groupby_multi_reducer` | 190.9 | 2.7 | 3.0 | 1.11× |
| `groupby_select_sum` | 207.6 | 3.2 | 3.7 | 1.16× |
| `groupby_sum` | 170.5 | 2.4 | 2.7 | 1.13× |
| `groupby_where_count` | 76.1 | 1.7 | 1.8 | 1.06× |
| `groupby_where_sum` | 87.0 | 1.7 | 1.9 | 1.12× |
| `indexed_lookup` | 1258.1 | 35549.6 | 103.3 | 0.00× |
| `join_count` | 37.9 | 36.1 | 13.4 | 0.37× |
| `last_match` | 0.00 | 0.50 | 1.4 | 2.80× |
| `long_count_aggregate` | 36.6 | 0.40 | 0.70 | 1.75× |
| `max_aggregate` | 48.1 | 0.70 | 0.50 | 0.71× |
| `min_aggregate` | 31.7 | 0.70 | 0.50 | 0.71× |
| `order_take_desc` | 37.9 | 0.70 | 1.4 | 2.00× |
| `reverse_take` | 0.00 | 0.00 | 1.1 | — |
| `select_count` | 0.10 | 0.00 | 0.00 | — |
| `select_where` | 105.6 | 4.7 | 5.5 | 1.17× |
| `select_where_count` | 32.4 | 0.40 | 0.60 | 1.50× |
| `select_where_order_take` | 36.4 | 0.70 | 1.4 | 2.00× |
| `select_where_sum` | 36.8 | 0.50 | 0.60 | 1.20× |
| `single_match` | 0.00 | 0.40 | 1.1 | 2.75× |
| `skip_take` | 0.30 | 0.00 | 0.00 | — |
| `skip_while_match` | 3.4 | 0.40 | 0.40 | 1.00× |
| `sort_first` | 37.4 | 0.40 | 1.3 | 3.25× |
| `sort_take` | 37.9 | 0.70 | 1.4 | 2.00× |
| `sum_aggregate` | 29.9 | 0.40 | 0.40 | 1.00× |
| `sum_where` | 33.0 | 0.40 | 0.60 | 1.50× |
| `take_count` | 1.8 | 0.10 | 0.10 | 1.00× |
| `take_count_filtered` | — | 0.00 | 0.00 | — |
| `take_sum_aggregate` | — | 0.00 | 0.00 | — |
| `take_while_match` | 8.0 | 0.20 | 0.30 | 1.50× |
| `to_array_filter` | 48.5 | 3.3 | 3.4 | 1.03× |
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
