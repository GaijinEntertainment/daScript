# Benchmarks — SQL / Array / Decs comparison

Generated 2026-05-24 from `59c4f3f98` (terminal-`_select` extension).
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
| `aggregate_match` | 35.2 | 6.0 | 5.8 | 0.97× |
| `all_match` | 28.1 | 3.6 | 3.5 | 0.97× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 29.8 | 5.9 | 8.8 | 1.49× |
| `bare_order_where` | 277.3 | 119.0 | 127.0 | 1.07× |
| `chained_where` | 36.2 | 6.7 | 6.7 | 1.00× |
| `contains_match` | 0.00 | 2.2 | 1.4 | 0.64× |
| `count_aggregate` | 29.4 | 4.1 | 4.1 | 1.00× |
| `distinct_by_count` | 40.9 | 15.8 | 16.1 | 1.02× |
| `distinct_count` | 41.2 | 16.0 | 15.9 | 0.99× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 172.4 | 30.3 | 30.4 | 1.00× |
| `groupby_count` | 142.7 | 20.1 | 19.5 | 0.97× |
| `groupby_first` | — | 18.6 | 19.3 | 1.04× |
| `groupby_having_count` | 143.3 | 19.2 | 19.3 | 1.01× |
| `groupby_having_hidden_sum` | 176.9 | 24.5 | 24.2 | 0.99× |
| `groupby_max` | 173.5 | 25.1 | 25.4 | 1.01× |
| `groupby_min` | 174.8 | 25.0 | 25.4 | 1.02× |
| `groupby_multi_reducer` | 188.9 | 32.5 | 32.8 | 1.01× |
| `groupby_select_sum` | 205.2 | 36.6 | 36.6 | 1.00× |
| `groupby_sum` | 173.5 | 18.7 | 18.9 | 1.01× |
| `groupby_where_count` | 75.7 | 14.7 | 14.9 | 1.01× |
| `groupby_where_sum` | 87.1 | 14.2 | 14.7 | 1.04× |
| `indexed_lookup` | 1448.5 | 224799.0 | 482.3 | 0.00× |
| `join_count` | 38.0 | 121.9 | 64.3 | 0.53× |
| `last_match` | 0.00 | 5.9 | 14.0 | 2.37× |
| `long_count_aggregate` | 29.4 | 4.2 | 4.1 | 0.98× |
| `max_aggregate` | 30.6 | 6.1 | 6.9 | 1.13× |
| `min_aggregate` | 30.7 | 6.2 | 7.0 | 1.13× |
| `order_take_desc` | 38.0 | 15.9 | 20.1 | 1.26× |
| `reverse_take` | 0.10 | 0.00 | 9.2 | — |
| `select_count` | 0.10 | 0.00 | 2.2 | — |
| `select_where` | 192.9 | 11.2 | 19.8 | 1.77× |
| `select_where_count` | 32.5 | 5.2 | 8.0 | 1.54× |
| `select_where_order_take` | 36.4 | 12.3 | 15.1 | 1.23× |
| `select_where_sum` | 37.5 | 7.5 | 7.6 | 1.01× |
| `single_match` | 0.00 | 2.9 | 5.6 | 1.93× |
| `skip_take` | 0.50 | 0.10 | 0.20 | 2.00× |
| `skip_while_match` | 3.4 | 5.3 | 5.3 | 1.00× |
| `sort_first` | 37.5 | 11.0 | 13.4 | 1.22× |
| `sort_take` | 38.1 | 16.3 | 20.3 | 1.25× |
| `sum_aggregate` | 29.5 | 2.2 | 2.1 | 0.95× |
| `sum_where` | 32.4 | 4.3 | 4.3 | 1.00× |
| `take_count` | 3.6 | 0.20 | 0.40 | 2.00× |
| `take_count_filtered` | — | 0.20 | 0.20 | 1.00× |
| `take_sum_aggregate` | — | 0.10 | 0.10 | 1.00× |
| `take_while_match` | 7.9 | 2.5 | 2.5 | 1.00× |
| `to_array_filter` | 70.5 | 13.2 | 12.1 | 0.92× |
| `zip_dot_product` | — | 8.0 | 4.8 | 0.60× |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | Decs vs Array |
|---|---:|---:|---:|---:|
| `aggregate_match` | 34.3 | 0.40 | 0.70 | 1.75× |
| `all_match` | 27.5 | 0.30 | 0.20 | 0.67× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 29.9 | 1.0 | 3.6 | 3.60× |
| `bare_order_where` | 184.5 | 33.7 | 35.2 | 1.04× |
| `chained_where` | 36.0 | 0.60 | 0.80 | 1.33× |
| `contains_match` | 0.00 | 0.20 | 0.10 | 0.50× |
| `count_aggregate` | 29.1 | 0.40 | 0.60 | 1.50× |
| `distinct_by_count` | 40.9 | 2.1 | 2.1 | 1.00× |
| `distinct_count` | 41.1 | 2.1 | 2.1 | 1.00× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 171.0 | 2.6 | 2.9 | 1.12× |
| `groupby_count` | 141.2 | 2.4 | 2.5 | 1.04× |
| `groupby_first` | — | 2.2 | 3.1 | 1.41× |
| `groupby_having_count` | 141.5 | 2.4 | 2.5 | 1.04× |
| `groupby_having_hidden_sum` | 176.7 | 2.5 | 2.9 | 1.16× |
| `groupby_max` | 174.4 | 2.4 | 2.7 | 1.13× |
| `groupby_min` | 173.5 | 2.4 | 2.7 | 1.13× |
| `groupby_multi_reducer` | 189.7 | 2.7 | 3.0 | 1.11× |
| `groupby_select_sum` | 224.9 | 3.2 | 3.7 | 1.16× |
| `groupby_sum` | 172.3 | 2.4 | 2.7 | 1.13× |
| `groupby_where_count` | 75.5 | 1.7 | 1.8 | 1.06× |
| `groupby_where_sum` | 86.6 | 1.7 | 1.8 | 1.06× |
| `indexed_lookup` | 1283.1 | 35684.7 | 103.4 | 0.00× |
| `join_count` | 37.9 | 36.7 | 13.4 | 0.37× |
| `last_match` | 0.00 | 0.50 | 1.4 | 2.80× |
| `long_count_aggregate` | 29.1 | 0.40 | 0.60 | 1.50× |
| `max_aggregate` | 30.5 | 0.60 | 0.50 | 0.83× |
| `min_aggregate` | 30.5 | 0.70 | 0.50 | 0.71× |
| `order_take_desc` | 38.2 | 0.70 | 1.4 | 2.00× |
| `reverse_take` | 0.00 | 0.00 | 1.1 | — |
| `select_count` | 0.10 | 0.00 | 0.00 | — |
| `select_where` | 104.7 | 4.1 | 5.6 | 1.37× |
| `select_where_count` | 32.1 | 0.40 | 0.60 | 1.50× |
| `select_where_order_take` | 35.9 | 0.70 | 1.4 | 2.00× |
| `select_where_sum` | 36.4 | 0.50 | 0.60 | 1.20× |
| `single_match` | 0.00 | 0.40 | 1.1 | 2.75× |
| `skip_take` | 0.30 | 0.00 | 0.00 | — |
| `skip_while_match` | 3.4 | 0.40 | 0.40 | 1.00× |
| `sort_first` | 37.6 | 0.40 | 1.3 | 3.25× |
| `sort_take` | 38.2 | 0.70 | 1.4 | 2.00× |
| `sum_aggregate` | 30.1 | 0.40 | 0.30 | 0.75× |
| `sum_where` | 32.4 | 0.40 | 0.60 | 1.50× |
| `take_count` | 1.8 | 0.10 | 0.10 | 1.00× |
| `take_count_filtered` | — | 0.00 | 0.00 | — |
| `take_sum_aggregate` | — | 0.00 | 0.00 | — |
| `take_while_match` | 7.9 | 0.20 | 0.30 | 1.50× |
| `to_array_filter` | 48.1 | 3.3 | 3.4 | 1.03× |
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
