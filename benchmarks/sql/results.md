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
| `aggregate_match` | 35.1 | 5.9 | 6.0 | 58.9 | 161.3 |
| `all_match` | 27.9 | 3.5 | 3.4 | 59.3 | 155.0 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.5 | 5.9 | 8.8 | 56.3 | 168.3 |
| `bare_last` | — | 4.2 | 0.0 | 0.0 | 4.2 |
| `bare_order_where` | 280.1 | 120.7 | 127.6 | 302.3 | 293.3 |
| `chained_select_collapse` | — | 17.6 | 17.9 | 75.2 | 164.1 |
| `chained_where` | 36.6 | 6.6 | 7.2 | 103.0 | 190.1 |
| `contains_match` | 0.0 | 2.2 | 1.4 | 28.5 | 72.8 |
| `count_aggregate` | 30.1 | 4.1 | 4.1 | 61.7 | 156.6 |
| `cross_join` | 12624.7 | 3685.5 | — | 4026.4 | 4014.9 |
| `decs_count_bare_pred` | — | — | 4.1 | — | — |
| `distinct_by_count` | 41.6 | 15.7 | 15.8 | 70.7 | 163.2 |
| `distinct_by_order_take` | 245.5 | 22.1 | 23.6 | 126.5 | 162.6 |
| `distinct_by_order_to_array` | 240.8 | 22.0 | 23.8 | 126.7 | 164.4 |
| `distinct_count` | 41.9 | 16.0 | 15.7 | 70.9 | 161.9 |
| `distinct_count_pred` | 251.1 | 15.6 | 16.0 | 112.7 | 183.7 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.4 | 0.3 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 170.5 | 29.0 | 29.2 | 124.6 | 194.6 |
| `groupby_count` | 141.4 | 20.3 | 20.4 | 74.2 | 168.6 |
| `groupby_first` | 250.0 | 20.3 | 21.1 | 72.0 | 166.0 |
| `groupby_having_count` | 141.9 | 20.3 | 20.4 | 74.4 | 168.5 |
| `groupby_having_hidden_sum` | 175.1 | 22.4 | 22.5 | 119.4 | 193.0 |
| `groupby_having_post_where` | 171.0 | 20.3 | 20.4 | 115.7 | 189.4 |
| `groupby_max` | 172.4 | 25.1 | 25.0 | 120.4 | 194.2 |
| `groupby_min` | 173.7 | 24.7 | 24.8 | 120.5 | 193.8 |
| `groupby_multi_reducer` | 190.1 | 30.2 | 30.2 | 126.5 | 198.1 |
| `groupby_select_order` | 170.6 | 20.3 | 20.4 | 115.8 | 188.9 |
| `groupby_select_sum` | 199.4 | 40.0 | 39.9 | 98.5 | 197.0 |
| `groupby_sum` | 170.6 | 20.3 | 20.3 | 114.9 | 188.4 |
| `groupby_where_count` | 75.7 | 13.7 | 14.1 | 116.9 | 187.4 |
| `groupby_where_sum` | 86.4 | 14.1 | 14.9 | 116.4 | 187.3 |
| `indexed_lookup` | 1474.7 | 195987.9 | 483.3 | 5788595.2 | 15195386.8 |
| `join_count` | 38.1 | 51.2 | 64.5 | 113.3 | 186.4 |
| `join_groupby_count` | 156.5 | 77.2 | 89.4 | 178.1 | 235.7 |
| `join_groupby_to_array` | 194.5 | 78.7 | 90.2 | 214.5 | 211.4 |
| `join_select` | 149.9 | 72.1 | 85.1 | 187.8 | 218.4 |
| `join_where_count` | 39.1 | 62.7 | 76.1 | 161.4 | 198.5 |
| `last_match` | 0.0 | 5.8 | 14.1 | 65.5 | 160.1 |
| `long_count_aggregate` | 29.7 | 4.1 | 4.1 | 61.4 | 156.6 |
| `max_aggregate` | 31.0 | 6.1 | 6.8 | 57.5 | 165.4 |
| `min_aggregate` | 31.2 | 5.9 | 6.9 | 58.1 | 163.5 |
| `order_by_multi_key` | 339.9 | 273.2 | 282.0 | 459.6 | 445.2 |
| `order_distinct_take` | 139.0 | 15.6 | 100.7 | 72.9 | 164.1 |
| `order_reverse_normalized` | 38.6 | 16.2 | 20.1 | 69.9 | 172.3 |
| `order_take_desc` | 38.6 | 16.1 | 20.0 | 70.5 | 171.7 |
| `reverse_distinct_by` | 295.9 | 21.8 | 27.8 | 70.9 | 164.4 |
| `reverse_take` | 0.1 | 0.0 | 0.2 | 0.0 | 26.2 |
| `reverse_take_select` | 0.0 | 0.0 | 0.2 | 0.0 | 26.0 |
| `select_count` | 0.1 | 0.0 | 2.2 | 68.3 | 2.2 |
| `select_many` | — | 191.3 | — | — | — |
| `select_where` | 195.7 | 11.1 | 19.5 | 200.8 | 184.6 |
| `select_where_count` | 32.8 | 5.1 | 7.4 | 62.2 | 157.8 |
| `select_where_order_take` | 37.0 | 12.2 | 15.0 | 71.7 | 169.4 |
| `select_where_sum` | 37.2 | 7.5 | 7.5 | 64.7 | 163.3 |
| `single_match` | 0.0 | 2.8 | 5.4 | 57.6 | 151.5 |
| `skip_take` | 0.5 | 0.1 | 0.2 | 3.1 | 2.8 |
| `skip_while_match` | 3.5 | 5.2 | 5.3 | 59.7 | 155.6 |
| `sort_first` | 38.0 | 11.0 | 13.2 | 63.5 | 167.2 |
| `sort_take` | 38.6 | 16.4 | 20.6 | 70.3 | 175.4 |
| `sort_take_select` | 38.6 | 16.3 | 20.2 | 70.3 | 168.6 |
| `sum_aggregate` | 30.3 | 2.1 | 2.2 | 52.5 | 154.3 |
| `sum_where` | 33.3 | 4.3 | 4.3 | 61.9 | 154.7 |
| `take_count` | 3.6 | 0.2 | 0.4 | 3.0 | 2.7 |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 1.3 | 1.1 |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 0.6 | 0.5 |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 0.7 | 0.6 |
| `take_while_match` | 7.9 | 2.5 | 2.5 | 29.9 | 76.2 |
| `to_array_filter` | 70.7 | 11.8 | 11.8 | 69.7 | 165.6 |
| `where_join_count` | 41.7 | 29.9 | 41.7 | 131.5 | 170.0 |
| `zip_count_pred` | 39.4 | 15.9 | — | 320.5 | 322.4 |
| `zip_dot_product` | 47.0 | 12.7 | 10.6 | 316.0 | 317.6 |
| `zip_dot_product_3arg` | 46.8 | 12.9 | — | 314.8 | 317.3 |
| `zip_reverse_to_array` | — | 31.8 | — | 344.8 | 360.8 |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) |
|---|---:|---:|---:|---:|---:|
| `aggregate_match` | 34.4 | 0.3 | 0.7 | 28.2 | 26.0 |
| `all_match` | 27.9 | 0.3 | 0.2 | 16.5 | 25.2 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.3 | 1.0 | 3.6 | 19.4 | 25.0 |
| `bare_last` | — | 0.4 | 0.0 | 0.0 | 0.0 |
| `bare_order_where` | 187.9 | 34.2 | 35.3 | 105.2 | 53.5 |
| `chained_select_collapse` | — | 1.1 | 1.1 | 20.6 | 32.5 |
| `chained_where` | 37.0 | 0.6 | 0.9 | 40.5 | 30.6 |
| `contains_match` | 0.0 | 0.2 | 0.1 | 18.0 | 9.5 |
| `count_aggregate` | 29.5 | 0.3 | 0.6 | 26.5 | 25.6 |
| `cross_join` | 5998.2 | 755.3 | — | 859.2 | 782.9 |
| `decs_count_bare_pred` | — | — | 0.6 | — | — |
| `distinct_by_count` | 41.5 | 1.1 | 1.1 | 20.5 | 32.7 |
| `distinct_by_order_take` | 243.6 | 1.7 | 2.6 | 44.8 | 38.1 |
| `distinct_by_order_to_array` | 238.8 | 1.7 | 2.7 | 47.2 | 38.3 |
| `distinct_count` | 41.6 | 1.1 | 1.1 | 20.5 | 32.8 |
| `distinct_count_pred` | 252.1 | 1.1 | 1.3 | 37.7 | 43.2 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.1 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 171.0 | 1.6 | 1.8 | 37.7 | 43.9 |
| `groupby_count` | 144.6 | 1.3 | 1.5 | 20.6 | 33.2 |
| `groupby_first` | 251.8 | 1.3 | 2.3 | 20.6 | 33.7 |
| `groupby_having_count` | 142.7 | 1.3 | 1.5 | 20.7 | 32.9 |
| `groupby_having_hidden_sum` | 174.1 | 1.5 | 1.7 | 37.5 | 44.8 |
| `groupby_having_post_where` | 174.7 | 1.4 | 1.6 | 35.1 | 43.9 |
| `groupby_max` | 172.6 | 1.5 | 1.8 | 37.4 | 45.7 |
| `groupby_min` | 173.3 | 1.5 | 1.8 | 35.4 | 45.6 |
| `groupby_multi_reducer` | 190.3 | 1.6 | 2.0 | 37.8 | 45.3 |
| `groupby_select_order` | 175.4 | 1.4 | 2.0 | 37.4 | 43.5 |
| `groupby_select_sum` | 200.3 | 2.8 | 3.2 | 30.9 | 39.2 |
| `groupby_sum` | 171.0 | 1.4 | 1.9 | 37.5 | 43.3 |
| `groupby_where_count` | 76.0 | 0.9 | 1.3 | 34.9 | 40.8 |
| `groupby_where_sum` | 87.1 | 0.9 | 1.4 | 34.9 | 41.0 |
| `indexed_lookup` | 1239.2 | 32566.9 | 104.8 | 4690531.3 | 2286552.4 |
| `join_count` | 38.5 | 10.9 | 11.8 | 42.2 | 71.8 |
| `join_groupby_count` | 157.4 | 18.0 | 20.0 | 66.5 | 87.6 |
| `join_groupby_to_array` | 190.6 | 17.5 | 19.4 | 79.3 | 38.9 |
| `join_select` | 93.6 | 19.6 | 21.8 | 75.1 | 94.3 |
| `join_where_count` | 39.2 | 19.2 | 21.2 | 61.9 | 77.9 |
| `last_match` | 0.0 | 0.5 | 1.4 | 17.3 | 26.1 |
| `long_count_aggregate` | 29.6 | 0.3 | 0.6 | 16.6 | 25.1 |
| `max_aggregate` | 31.3 | 0.3 | 0.5 | 27.9 | 26.6 |
| `min_aggregate` | 31.2 | 0.3 | 0.5 | 16.8 | 26.5 |
| `order_by_multi_key` | 246.1 | 53.7 | 54.6 | 124.6 | 72.0 |
| `order_distinct_take` | 137.8 | 1.1 | 75.9 | 20.9 | 35.0 |
| `order_reverse_normalized` | 38.2 | 0.7 | 1.3 | 21.3 | 27.4 |
| `order_take_desc` | 38.2 | 0.7 | 1.3 | 21.2 | 27.6 |
| `reverse_distinct_by` | 304.7 | 1.6 | 3.2 | 20.6 | 34.5 |
| `reverse_take` | 0.0 | 0.0 | 0.0 | 0.0 | 3.7 |
| `reverse_take_select` | 0.0 | 0.0 | 0.0 | 0.0 | 3.7 |
| `select_count` | 0.1 | 0.0 | 0.0 | 67.8 | 0.0 |
| `select_many` | — | 63.4 | — | — | — |
| `select_where` | 107.4 | 4.2 | 5.5 | 77.1 | 21.8 |
| `select_where_count` | 32.6 | 0.3 | 0.6 | 16.6 | 27.1 |
| `select_where_order_take` | 36.9 | 0.7 | 1.4 | 20.4 | 27.8 |
| `select_where_sum` | 37.2 | 0.4 | 0.6 | 25.0 | 25.7 |
| `single_match` | 0.0 | 0.4 | 1.1 | 45.9 | 22.8 |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.3 | 0.2 |
| `skip_while_match` | 3.4 | 0.4 | 0.4 | 47.0 | 22.2 |
| `sort_first` | 39.0 | 0.4 | 1.3 | 16.7 | 27.1 |
| `sort_take` | 38.6 | 0.7 | 1.4 | 21.3 | 28.5 |
| `sort_take_select` | 38.6 | 0.7 | 1.4 | 21.3 | 27.4 |
| `sum_aggregate` | 30.2 | 0.3 | 0.1 | 20.7 | 25.3 |
| `sum_where` | 33.2 | 0.3 | 0.6 | 28.1 | 26.9 |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.3 | 0.2 |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 0.4 | 0.1 |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 0.2 | 0.0 |
| `take_where_count` | 0.9 | 0.0 | 0.0 | 0.2 | 0.0 |
| `take_while_match` | 7.9 | 0.2 | 0.3 | 17.0 | 9.4 |
| `to_array_filter` | 48.8 | 3.3 | 3.4 | 23.0 | 35.4 |
| `where_join_count` | 41.8 | 5.9 | 6.8 | 47.2 | 42.9 |
| `zip_count_pred` | 39.4 | 0.1 | — | 116.2 | 32.8 |
| `zip_dot_product` | 46.8 | 0.1 | 0.1 | 114.2 | 33.2 |
| `zip_dot_product_3arg` | 46.6 | 0.1 | — | 115.4 | 32.8 |
| `zip_reverse_to_array` | — | 4.6 | — | 122.7 | 51.8 |
<!-- BENCH:TABLES END -->

## Missing lanes (the `—` cells)

Each empty cell's reason is also in the bench `.das` file's comment; SQL gaps are detailed in
[`sqlite_linq_gaps.md`](sqlite_linq_gaps.md).

- **`bare_last` SQL** — SQLite has no inherent row order; bare `last()` is meaningful only for ordered sources (array/decs/XML/JSON). By design.
- **`chained_select_collapse` SQL** — `_sql` doesn't emit `COUNT(DISTINCT expr)`. By design.
- **`cross_join` Decs** — standalone `from_decs_template` yields anonymous tuples; no typed-lambda cross form / `_fold` integration yet.
- **`select_many` SQL / Decs / XML / JSON** — needs a per-element nested `array<…>` field the flat `Car` schema, decs components, and DOM-attr sources don't have. Array-only by design.
- **`decs_count_bare_pred` non-decs** — decs-only family (exercises a decs-walk root cause); no meaningful analog elsewhere.
- **`indexed_lookup` m3f vs m4** — array = linear scan (~204k ns), decs = `query(eid)` O(1); not like-for-like.
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
