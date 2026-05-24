# Benchmarks — SQL / Array / Decs comparison

Generated 2026-05-23 from master `1c8ff9119`. Fixture size: n = 100 000
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
| `aggregate_match` | 35.1 | 6.0 | 5.9 | 0.98× |
| `all_match` | 27.7 | 3.6 | 3.5 | 0.97× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 31.2 | 5.8 | 8.7 | 1.50× |
| `bare_order_where` | 274.0 | 118.4 | 126.3 | 1.07× |
| `chained_where` | 36.4 | 8.3 | 8.0 | 0.96× |
| `contains_match` | 0.00 | 2.3 | 1.4 | 0.61× |
| `count_aggregate` | 29.7 | 4.1 | 4.1 | 1.00× |
| `distinct_by_count` | — | 15.9 | 15.8 | 0.99× |
| `distinct_count` | 41.2 | 16.0 | 15.8 | 0.99× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 174.3 | 30.2 | 30.1 | 1.00× |
| `groupby_count` | 140.9 | 19.2 | 19.3 | 1.01× |
| `groupby_first` | — | 18.4 | 19.1 | 1.04× |
| `groupby_having_count` | 140.0 | 19.1 | 19.2 | 1.01× |
| `groupby_having_hidden_sum` | 181.1 | 24.8 | 23.9 | 0.96× |
| `groupby_max` | 173.8 | 24.9 | 25.2 | 1.01× |
| `groupby_min` | 173.5 | 25.0 | 25.4 | 1.02× |
| `groupby_multi_reducer` | 188.5 | 32.3 | 33.3 | 1.03× |
| `groupby_select_sum` | — | 36.8 | 36.2 | 0.98× |
| `groupby_sum` | 185.6 | 19.2 | 18.7 | 0.97× |
| `groupby_where_count` | 75.9 | 14.6 | 14.9 | 1.02× |
| `groupby_where_sum` | 86.6 | 14.5 | 14.6 | 1.01× |
| `indexed_lookup` | 1449.1 | 203294.7 | 470.6 | 0.00× |
| `join_count` | 38.2 | 122.1 | — | — |
| `last_match` | 0.00 | 6.0 | 14.2 | 2.37× |
| `long_count_aggregate` | 29.3 | 4.4 | 4.5 | 1.02× |
| `max_aggregate` | 30.5 | 6.0 | 6.9 | 1.15× |
| `min_aggregate` | 31.1 | 6.2 | 6.9 | 1.11× |
| `order_take_desc` | 40.2 | 15.8 | 20.3 | 1.28× |
| `reverse_take` | 0.10 | 0.00 | 9.2 | — |
| `select_count` | 0.10 | 0.00 | 2.2 | — |
| `select_where` | 197.9 | 11.1 | 20.0 | 1.80× |
| `select_where_count` | 33.7 | 5.2 | 7.4 | 1.42× |
| `select_where_order_take` | 36.7 | 12.2 | 15.0 | 1.23× |
| `select_where_sum` | 36.8 | 7.4 | 7.4 | 1.00× |
| `single_match` | 0.00 | 3.0 | 5.5 | 1.83× |
| `skip_take` | 0.50 | 0.10 | 0.20 | 2.00× |
| `skip_while_match` | 3.6 | 5.2 | 5.3 | 1.02× |
| `sort_first` | 38.5 | 11.0 | 13.4 | 1.22× |
| `sort_take` | 37.6 | 16.3 | 20.5 | 1.26× |
| `sum_aggregate` | 29.8 | 2.2 | 2.4 | 1.09× |
| `sum_where` | 32.8 | 4.2 | 4.3 | 1.02× |
| `take_count` | 3.6 | 0.20 | 0.40 | 2.00× |
| `take_count_filtered` | — | 0.20 | 0.20 | 1.00× |
| `take_sum_aggregate` | — | 0.10 | 0.10 | 1.00× |
| `take_while_match` | 8.0 | 2.4 | 2.5 | 1.04× |
| `to_array_filter` | 70.5 | 11.8 | 11.7 | 0.99× |
| `zip_dot_product` | — | 8.2 | 4.8 | 0.59× |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | Decs vs Array |
|---|---:|---:|---:|---:|
| `aggregate_match` | 215.7 | 4.4 | 7.4 | 1.68× |
| `all_match` | 303.8 | 3.8 | 2.1 | 0.55× |
| `any_match` | 0.00 | 0.00 | 0.00 | — |
| `average_aggregate` | 51.4 | 1.6 | 5.9 | 3.69× |
| `bare_order_where` | 383.2 | 66.3 | 66.0 | 1.00× |
| `chained_where` | 79.6 | 1.3 | 1.9 | 1.46× |
| `contains_match` | 0.00 | 0.40 | 0.20 | 0.50× |
| `count_aggregate` | 57.8 | 0.80 | 1.2 | 1.50× |
| `distinct_by_count` | — | 4.2 | 5.8 | 1.38× |
| `distinct_count` | 76.3 | 3.9 | 4.0 | 1.03× |
| `distinct_take` | 0.00 | 0.00 | 0.00 | — |
| `element_at_match` | 0.00 | 0.00 | 0.00 | — |
| `first_match` | 0.00 | 0.00 | 0.00 | — |
| `first_or_default_match` | 0.00 | 0.00 | 0.00 | — |
| `groupby_average` | 172.9 | 2.6 | 2.9 | 1.12× |
| `groupby_count` | 141.6 | 2.4 | 2.5 | 1.04× |
| `groupby_first` | — | 2.2 | 3.1 | 1.41× |
| `groupby_having_count` | 141.8 | 2.4 | 2.5 | 1.04× |
| `groupby_having_hidden_sum` | 176.6 | 2.6 | 2.8 | 1.08× |
| `groupby_max` | 174.4 | 2.4 | 2.7 | 1.13× |
| `groupby_min` | 173.4 | 2.4 | 2.7 | 1.13× |
| `groupby_multi_reducer` | 189.8 | 2.7 | 3.0 | 1.11× |
| `groupby_select_sum` | — | 3.2 | 3.7 | 1.16× |
| `groupby_sum` | 171.9 | 2.4 | 2.7 | 1.13× |
| `groupby_where_count` | 75.7 | 1.7 | 1.8 | 1.06× |
| `groupby_where_sum` | 86.8 | 1.7 | 1.8 | 1.06× |
| `indexed_lookup` | 1253.3 | 35303.2 | 103.9 | 0.00× |
| `join_count` | 38.0 | 36.4 | — | — |
| `last_match` | 0.00 | 0.50 | 1.4 | 2.80× |
| `long_count_aggregate` | 29.0 | 0.40 | 0.60 | 1.50× |
| `max_aggregate` | 30.3 | 0.70 | 0.50 | 0.71× |
| `min_aggregate` | 30.5 | 0.70 | 0.50 | 0.71× |
| `order_take_desc` | 38.0 | 0.70 | 1.4 | 2.00× |
| `reverse_take` | 0.00 | 0.00 | 1.1 | — |
| `select_count` | 0.10 | 0.00 | 0.00 | — |
| `select_where` | 105.6 | 4.2 | 5.5 | 1.31× |
| `select_where_count` | 32.3 | 0.40 | 0.60 | 1.50× |
| `select_where_order_take` | 36.2 | 0.70 | 1.4 | 2.00× |
| `select_where_sum` | 36.8 | 0.50 | 0.60 | 1.20× |
| `single_match` | 0.00 | 0.40 | 1.1 | 2.75× |
| `skip_take` | 0.30 | 0.00 | 0.00 | — |
| `skip_while_match` | 3.5 | 0.40 | 0.40 | 1.00× |
| `sort_first` | 37.9 | 0.40 | 1.3 | 3.25× |
| `sort_take` | 38.5 | 0.70 | 1.4 | 2.00× |
| `sum_aggregate` | 30.5 | 0.40 | 0.30 | 0.75× |
| `sum_where` | 33.0 | 0.40 | 0.60 | 1.50× |
| `take_count` | 1.8 | 0.10 | 0.10 | 1.00× |
| `take_count_filtered` | — | 0.00 | 0.00 | — |
| `take_sum_aggregate` | — | 0.00 | 0.00 | — |
| `take_while_match` | 8.0 | 0.20 | 0.30 | 1.50× |
| `to_array_filter` | 48.0 | 3.3 | 3.4 | 1.03× |
| `zip_dot_product` | — | 0.50 | 0.50 | 1.00× |

## Notes on missing lanes (the `—` cells)

The reasons each cell is empty are also recorded as a comment in the
corresponding `.das` bench file; the bullets below quote that comment.

- **`distinct_by_count` SQL** — sqlite_linq does not currently lower
  `_distinct_by(...) |> count()`. Probe error: `_sql: unsupported chain
  root or operator. Got: __::linq\`distinct_by\`...`. Follow-up TODO
  2026-05-23: add a dispatch arm in sqlite_linq for
  distinct/distinct_by.
- **`groupby_first` SQL** — no direct SQL aggregator for "first
  source-order row per group". Window functions (`ROW_NUMBER() OVER
  (PARTITION BY brand ORDER BY id)` + outer `WHERE rn=1`) would be the
  SQL equivalent; sqlite_linq does not currently lower window
  functions. Follow-up TODO 2026-05-23.
- **`groupby_select_sum` SQL** — sqlite_linq's `_group_by` requires
  `_.Field` or a tuple of `_.Field`s; the expression key (`_ % 100`) is
  rejected. Probe error: `_sql: _group_by: key must be \`_.Field\` or a
  tuple of \`_.Field\`s; got: (_ % 100)`. Follow-up TODO 2026-05-23:
  expression-key support in sqlite_linq lowering.
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
