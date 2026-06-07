# Benchmarks — SQL / Array / Decs / XML / JSON comparison

Five lanes run the same query families over one `Car` schema (n = 100 000 cars, 100 dealers,
5 brands); cells are ns/op, `—` = intentionally absent lane (see "Missing lanes"). The tables
between the `BENCH:TABLES` markers are machine-generated (see "How to re-run"); all other text
is hand-edited.

- **m1 SQL** — `_fold(db |> select_from(type<Car>) |> …)` over in-memory SQLite; `_fold` passes the chain to `_sql`.
- **m3f Array** — `_fold` over `each(array<Car>)`.
- **m4 Decs** — `_fold` over `from_decs_template(type<DecsCar>)` (per-archetype walk).
- **m5f XML** — `_fold` over `from_xml_node(root, type<Car>)` (`XmlAdapter` fuses + field-prunes).
- **m6f JSON** — `_fold` over `from_json(jv, type<Car>)` (`JsonAdapter`, same machinery, array walk).

`0.00` = early-exit terminator below timer resolution ("free"). Chain shapes are in
`benchmarks/README.md`; the splice arms each fires are in `doc/source/reference/linq_fold_patterns.rst`.

**JIT on Apple silicon is high-variance run-to-run — read the INTERP column as the reliable
signal, JIT deltas as indicative.**

<!-- BENCH:TABLES BEGIN -->
*Generated 2026-06-06 by `benchmarks/sql/_update_results.das` — ns/op; `—` = absent lane. Edit the prose around the markers, not the tables.*

## INTERP

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) |
|---|---:|---:|---:|---:|---:|
| `aggregate_match` | 35.0 | 5.9 | 6.0 | 58.9 | 161.2 |
| `all_match` | 28.2 | 3.6 | 3.4 | 54.5 | 155.1 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.3 | 5.9 | 8.8 | 56.7 | 164.8 |
| `bare_last` | — | 4.2 | 0.0 | 0.0 | 4.2 |
| `bare_order_where` | 280.1 | 120.8 | 127.7 | 302.7 | 291.3 |
| `chained_select_collapse` | — | 17.5 | 17.9 | 73.3 | 164.1 |
| `chained_where` | 36.8 | 6.6 | 7.2 | 107.1 | 186.3 |
| `contains_match` | 0.0 | 2.2 | 1.4 | 28.6 | 72.7 |
| `count_aggregate` | 30.0 | 4.1 | 4.2 | 62.3 | 155.3 |
| `cross_join` | 12610.5 | 3694.7 | — | 4022.2 | 4008.7 |
| `decs_count_bare_pred` | — | — | 4.1 | — | — |
| `distinct_by_count` | 41.6 | 15.7 | 15.8 | 70.6 | 164.2 |
| `distinct_by_order_take` | 238.6 | 22.1 | 23.5 | 126.7 | 167.8 |
| `distinct_by_order_to_array` | 240.1 | 22.0 | 23.7 | 126.7 | 164.2 |
| `distinct_count` | 41.7 | 15.7 | 15.5 | 70.8 | 162.1 |
| `distinct_count_pred` | 252.2 | 15.6 | 16.0 | 113.1 | 182.0 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.4 | 0.3 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 171.3 | 29.2 | 29.3 | 124.7 | 195.7 |
| `groupby_count` | 142.3 | 19.1 | 19.2 | 74.8 | 167.8 |
| `groupby_first` | 252.6 | 19.2 | 19.8 | 72.3 | 165.1 |
| `groupby_having_count` | 142.7 | 19.1 | 19.2 | 74.8 | 169.3 |
| `groupby_having_hidden_sum` | 176.4 | 22.6 | 22.8 | 120.8 | 193.1 |
| `groupby_having_post_where` | 171.7 | 19.2 | 19.2 | 116.2 | 189.7 |
| `groupby_max` | 173.6 | 24.8 | 24.9 | 121.1 | 194.3 |
| `groupby_min` | 175.3 | 24.7 | 24.9 | 121.1 | 193.8 |
| `groupby_multi_reducer` | 190.5 | 30.3 | 30.3 | 126.2 | 202.8 |
| `groupby_select_order` | 175.3 | 19.2 | 19.2 | 116.2 | 188.7 |
| `groupby_select_sum` | 202.0 | 39.1 | 38.3 | 100.3 | 194.9 |
| `groupby_sum` | 171.6 | 19.1 | 19.2 | 114.7 | 189.7 |
| `groupby_where_count` | 76.0 | 13.8 | 14.2 | 117.5 | 188.6 |
| `groupby_where_sum` | 86.6 | 14.2 | 14.6 | 117.3 | 187.5 |
| `join_count` | 38.3 | 51.5 | 64.8 | 113.7 | 184.8 |
| `join_groupby_count` | 157.4 | 77.1 | 90.2 | 178.4 | 227.5 |
| `join_groupby_to_array` | 191.7 | 78.9 | 90.6 | 215.6 | 213.9 |
| `join_select` | 155.3 | 72.6 | 86.0 | 188.5 | 218.8 |
| `join_where_count` | 39.4 | 62.9 | 76.8 | 161.6 | 201.3 |
| `last_match` | 0.0 | 5.8 | 14.1 | 65.7 | 161.0 |
| `long_count_aggregate` | 29.9 | 4.1 | 4.1 | 61.6 | 156.8 |
| `max_aggregate` | 31.3 | 6.0 | 6.8 | 57.2 | 164.0 |
| `min_aggregate` | 31.2 | 6.0 | 6.8 | 57.1 | 163.8 |
| `order_by_multi_key` | 339.7 | 272.4 | 280.2 | 458.8 | 445.8 |
| `order_distinct_take` | 139.0 | 15.6 | 98.1 | 86.0 | 163.7 |
| `order_reverse_normalized` | 38.5 | 16.2 | 20.5 | 69.9 | 170.9 |
| `order_take_desc` | 38.5 | 16.1 | 20.0 | 70.1 | 172.0 |
| `reverse_distinct_by` | 296.5 | 21.1 | 28.0 | 71.4 | 159.2 |
| `reverse_take` | 0.1 | 0.0 | 0.2 | 0.0 | 26.3 |
| `reverse_take_select` | 0.0 | 0.0 | 0.2 | 0.0 | 26.3 |
| `select_count` | 0.1 | 0.0 | 2.2 | 71.5 | 2.2 |
| `select_many` | — | 191.7 | — | — | — |
| `select_where` | 195.4 | 11.1 | 19.6 | 200.4 | 182.7 |
| `select_where_count` | 32.7 | 5.2 | 7.5 | 62.4 | 159.4 |
| `select_where_order_take` | 37.1 | 12.2 | 14.9 | 71.2 | 165.8 |
| `select_where_sum` | 37.3 | 7.5 | 7.5 | 64.6 | 164.1 |
| `single_match` | 0.0 | 2.8 | 5.4 | 57.8 | 152.2 |
| `skip_take` | 0.5 | 0.1 | 0.2 | 3.1 | 2.8 |
| `skip_while_match` | 3.5 | 5.2 | 5.3 | 59.6 | 155.4 |
| `sort_first` | 38.1 | 11.0 | 13.2 | 63.5 | 168.2 |
| `sort_take` | 38.6 | 16.4 | 20.7 | 70.6 | 173.1 |
| `sort_take_select` | 38.4 | 16.3 | 20.2 | 70.6 | 161.3 |
| `sum_aggregate` | 31.2 | 2.1 | 2.1 | 52.6 | 154.0 |
| `sum_where` | 33.1 | 4.3 | 4.3 | 61.7 | 156.7 |
| `take_count` | 3.6 | 0.2 | 0.4 | 3.0 | 2.8 |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 1.3 | 1.1 |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 0.6 | 0.6 |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 0.7 | 0.6 |
| `take_while_match` | 7.9 | 2.5 | 2.5 | 29.8 | 77.3 |
| `to_array_filter` | 70.5 | 11.8 | 11.8 | 69.5 | 165.6 |
| `where_join_count` | 41.5 | 30.5 | 41.4 | 131.3 | 171.1 |
| `zip_count_pred` | 39.3 | 15.9 | — | 320.1 | 321.4 |
| `zip_dot_product` | 47.0 | 12.8 | 10.6 | 316.0 | 317.3 |
| `zip_dot_product_3arg` | 48.3 | 12.9 | — | 330.4 | 316.9 |
| `zip_reverse_to_array` | — | 31.7 | — | 345.7 | 351.4 |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) |
|---|---:|---:|---:|---:|---:|
| `aggregate_match` | 35.3 | 0.3 | 0.7 | 28.1 | 27.0 |
| `all_match` | 28.1 | 0.3 | 0.2 | 16.5 | 26.2 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.4 | 1.0 | 3.6 | 19.5 | 25.2 |
| `bare_last` | — | 0.4 | 0.0 | 0.0 | 0.0 |
| `bare_order_where` | 188.0 | 34.1 | 35.4 | 105.5 | 53.6 |
| `chained_select_collapse` | — | 1.1 | 1.1 | 20.6 | 34.0 |
| `chained_where` | 36.7 | 0.6 | 0.9 | 40.8 | 32.0 |
| `contains_match` | 0.0 | 0.2 | 0.1 | 17.7 | 9.4 |
| `count_aggregate` | 29.7 | 0.4 | 0.6 | 26.6 | 26.3 |
| `cross_join` | 5987.5 | 732.1 | — | 855.0 | 763.4 |
| `decs_count_bare_pred` | — | — | 0.6 | — | — |
| `distinct_by_count` | 41.5 | 1.1 | 1.1 | 20.7 | 33.7 |
| `distinct_by_order_take` | 240.4 | 1.7 | 2.6 | 44.7 | 38.8 |
| `distinct_by_order_to_array` | 240.2 | 1.7 | 2.7 | 47.2 | 38.8 |
| `distinct_count` | 41.5 | 1.1 | 1.1 | 20.6 | 33.9 |
| `distinct_count_pred` | 258.0 | 1.1 | 1.3 | 37.8 | 43.7 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.1 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 170.7 | 1.5 | 1.8 | 37.7 | 44.3 |
| `groupby_count` | 142.0 | 1.3 | 1.5 | 20.6 | 34.2 |
| `groupby_first` | 252.6 | 1.3 | 2.3 | 20.7 | 34.9 |
| `groupby_having_count` | 142.7 | 1.3 | 1.5 | 20.7 | 33.9 |
| `groupby_having_hidden_sum` | 175.6 | 1.5 | 1.7 | 37.5 | 45.4 |
| `groupby_having_post_where` | 170.8 | 1.4 | 1.6 | 35.1 | 44.5 |
| `groupby_max` | 174.0 | 1.5 | 1.8 | 37.6 | 46.2 |
| `groupby_min` | 175.1 | 1.5 | 1.8 | 35.4 | 46.3 |
| `groupby_multi_reducer` | 190.8 | 1.6 | 2.0 | 37.7 | 45.9 |
| `groupby_select_order` | 171.8 | 1.4 | 2.0 | 37.5 | 44.5 |
| `groupby_select_sum` | 201.7 | 2.8 | 3.2 | 31.1 | 40.0 |
| `groupby_sum` | 171.8 | 1.4 | 1.9 | 37.3 | 44.2 |
| `groupby_where_count` | 75.4 | 0.9 | 1.3 | 34.7 | 42.0 |
| `groupby_where_sum` | 86.3 | 0.9 | 1.4 | 34.8 | 41.9 |
| `join_count` | 38.2 | 10.7 | 12.0 | 42.2 | 71.7 |
| `join_groupby_count` | 158.1 | 17.6 | 19.6 | 66.7 | 86.2 |
| `join_groupby_to_array` | 191.0 | 17.6 | 19.5 | 79.7 | 36.9 |
| `join_select` | 93.3 | 19.6 | 21.7 | 75.1 | 94.2 |
| `join_where_count` | 39.4 | 19.0 | 20.6 | 62.0 | 77.1 |
| `last_match` | 0.0 | 0.5 | 1.4 | 17.4 | 26.7 |
| `long_count_aggregate` | 30.1 | 0.3 | 0.6 | 16.7 | 26.2 |
| `max_aggregate` | 31.4 | 0.3 | 0.5 | 28.0 | 27.5 |
| `min_aggregate` | 31.3 | 0.3 | 0.5 | 16.9 | 27.4 |
| `order_by_multi_key` | 248.8 | 53.5 | 54.8 | 124.5 | 70.2 |
| `order_distinct_take` | 138.8 | 1.1 | 75.6 | 20.9 | 35.8 |
| `order_reverse_normalized` | 38.5 | 0.7 | 1.3 | 21.4 | 29.5 |
| `order_take_desc` | 38.4 | 0.7 | 1.3 | 21.3 | 28.3 |
| `reverse_distinct_by` | 295.3 | 1.6 | 3.2 | 20.6 | 34.9 |
| `reverse_take` | 0.0 | 0.0 | 0.0 | 0.0 | 3.6 |
| `reverse_take_select` | 0.0 | 0.0 | 0.0 | 0.0 | 3.6 |
| `select_count` | 0.1 | 0.0 | 0.0 | 66.1 | 0.0 |
| `select_many` | — | 63.3 | — | — | — |
| `select_where` | 106.5 | 4.2 | 5.5 | 77.2 | 23.1 |
| `select_where_count` | 32.4 | 0.3 | 0.6 | 16.6 | 26.8 |
| `select_where_order_take` | 37.1 | 0.7 | 1.4 | 20.4 | 27.6 |
| `select_where_sum` | 37.4 | 0.4 | 0.6 | 25.1 | 25.9 |
| `single_match` | 0.0 | 0.4 | 1.1 | 46.1 | 22.7 |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.3 | 0.2 |
| `skip_while_match` | 3.4 | 0.4 | 0.4 | 46.7 | 22.7 |
| `sort_first` | 38.2 | 0.4 | 1.3 | 16.7 | 27.2 |
| `sort_take` | 38.5 | 0.7 | 1.4 | 21.3 | 28.5 |
| `sort_take_select` | 38.3 | 0.7 | 1.3 | 21.4 | 28.1 |
| `sum_aggregate` | 30.0 | 0.3 | 0.1 | 20.7 | 25.0 |
| `sum_where` | 33.4 | 0.3 | 0.6 | 28.3 | 26.8 |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.3 | 0.3 |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 0.5 | 0.1 |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 0.2 | 0.0 |
| `take_where_count` | 0.9 | 0.0 | 0.0 | 0.2 | 0.0 |
| `take_while_match` | 7.8 | 0.2 | 0.3 | 18.5 | 9.2 |
| `to_array_filter` | 48.9 | 3.3 | 3.4 | 23.0 | 35.6 |
| `where_join_count` | 41.8 | 6.0 | 6.8 | 47.2 | 42.2 |
| `zip_count_pred` | 39.6 | 0.1 | — | 115.7 | 32.7 |
| `zip_dot_product` | 46.7 | 0.1 | 0.1 | 114.8 | 33.2 |
| `zip_dot_product_3arg` | 47.1 | 0.1 | — | 114.7 | 32.8 |
| `zip_reverse_to_array` | — | 4.6 | — | 122.8 | 51.7 |
<!-- BENCH:TABLES END -->

## Missing lanes (the `—` cells)

Each empty cell's reason is also in the bench `.das` file's comment; SQL gaps are detailed in
[`sqlite_linq_gaps.md`](sqlite_linq_gaps.md).

- **`bare_last` SQL** — SQLite has no inherent row order; bare `last()` is meaningful only for ordered sources (array/decs/XML/JSON). By design.
- **`chained_select_collapse` SQL** — `_sql` doesn't emit `COUNT(DISTINCT expr)`. By design.
- **`cross_join` Decs** — standalone `from_decs_template` yields anonymous tuples; no typed-lambda cross form / `_fold` integration yet.
- **`select_many` SQL / Decs / XML / JSON** — needs a per-element nested `array<…>` field the flat `Car` schema, decs components, and DOM-attr sources don't have. Array-only by design.
- **`decs_count_bare_pred` non-decs** — decs-only family (exercises a decs-walk root cause); no meaningful analog elsewhere.
- **`reverse_distinct_by` m4 / m5f** — array uses the backward-index walk; non-array sources fuse the forward keep-last splice (decs 27.6/5.0, XML 74.5/22.2); SQL uses MAX(pk).
- **`order_distinct_take` m4 vs m3f** — `unique_key` hashes workhorse keys directly (array `int`) but string-interpolates structs (decs `DecsBrand`); the gap is per-element string hashing, not decs-walk. `distinct_by_count` is the key-based variant (m4 parity).
- **`zip_reverse_to_array` / `zip_*` SQL / Decs** — `reverse` has no SQL order key; zip is not relational / not expressible over one archetype walk. By design. (XML/JSON zip lanes are lit, partially fused.)

## Accepted floors

A few m4 (decs) cells stay >=1.5x their m3f (array) INTERP counterpart for structural reasons —
decs columnar storage fetches all N columns per element (`last_match`, `select_where`), and
`order_distinct_take` pays `unique_key` string hashing on struct keys. Structural, not splice/emit
gaps; not chased further.

## How to re-run

The two matrices (between the `BENCH:TABLES` markers) are regenerated by
`benchmarks/sql/_update_results.das` from machine-readable bench output — never hand-edit the cells.

```bash
rm -f /tmp/bench_interp.json /tmp/bench_jit.json
for f in benchmarks/sql/*.das; do
  [[ "$(basename "$f")" == _* ]] && continue
  bin/daslang      dastest/dastest.das -- --bench --bench-format json --test "$f" >> /tmp/bench_interp.json 2>/dev/null
  bin/daslang -jit dastest/dastest.das -- --bench --bench-format json --test "$f" >> /tmp/bench_jit.json   2>/dev/null
done
bin/daslang benchmarks/sql/_update_results.das -- --interp /tmp/bench_interp.json --jit /tmp/bench_jit.json --date YYYY-MM-DD
```

Edit the prose (header, "Missing lanes", this section) by hand; the tool rewrites only the tables.
If JIT fails to compile (`host_jit_triple()` missing), `bin/daslang` is stale vs
`src/builtin/module_jit.cpp` — rebuild and retry.
