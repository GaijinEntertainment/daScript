# Benchmarks — SQL / Array / Decs / XML / JSON / Table comparison

Six lanes run the same query families over one `Car` schema (n = 100 000 cars, 100 dealers,
5 brands); cells are ns/op, `—` = intentionally absent lane (see "Missing lanes"). The tables
between the `BENCH:TABLES` markers are machine-generated (see "How to re-run"); all other text
is hand-edited.

Each lane lives in its own file (`array.das` / `decs.das` / `xml.das` / `json.das` / `sql.das` /
`table.das`) with the source fixture built once in `[init]`; the sweep runs one process per file,
so a lane is never contaminated by another lane's code in the same process (this is why JIT cells
are stable now).

- **m1 SQL** — `_fold(db |> select_from(type<Car>) |> …)` over in-memory SQLite; `_fold` passes the chain to `_sql`.
- **m3f Array** — `_fold` over `each(array<Car>)`.
- **m4 Decs** — `_fold` over `from_decs_template(type<DecsCar>)` (per-archetype walk).
- **m5f XML** — `_fold` over `from_xml_node(root, type<Car>)` (`XmlAdapter` fuses + field-prunes).
- **m6f JSON** — `_fold` over `from_json(jv, type<Car>)` (`JsonAdapter`, same machinery, array walk).
- **m7 Table** — `_fold` over `each_kv(table<int; Car>)` (`TableAdapter`; kv usage-pruning picks keys-only /
  values-only / zipped slot walks; key-equality `where` + terminator folds to an O(1) probe — the
  `point_lookup` / `point_lookup_scan` pair measures it; joins fuse on either side, and a table srcB
  joined on its bare key probes the table instead of building the join hash — the `join_probe` /
  `join_probe_build` pair measures it; a trailing `to_table()` inserts straight into the result
  table with no intermediate array — the `to_table` / `to_table_staged` pair measures it;
  group_by fuses through `plan_group_by_core` with the usage-pruned slot walk as the bucket-fill
  loop; join+group_by and reverse defer to tier-2). Under JIT, `keys`/`values` for-loop sources
  compile to an inline open-addressed slot walk (no per-element C++ iterator calls), so the m7
  JIT column is fused codegen end to end.

`0.00` = early-exit terminator below timer resolution ("free"). Chain shapes are in
`benchmarks/README.md`; the splice arms each fires are in `doc/source/reference/linq_fold_patterns.rst`.

**JIT on Apple silicon is high-variance run-to-run — read the INTERP column as the reliable
signal, JIT deltas as indicative.**

<!-- BENCH:TABLES BEGIN -->
*Generated 2026-06-11 by `benchmarks/sql/_update_results.das` — ns/op; `—` = absent lane. Edit the prose around the markers, not the tables.*

## INTERP

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) | Table fold (m7) |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 34.7 | 5.9 | 6.1 | 60.5 | 158.9 | 19.9 |
| `all_match` | 27.8 | 3.5 | 3.5 | 56.1 | 157.6 | 16.1 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.6 | 6.1 | 8.7 | 58.7 | 157.7 | 17.2 |
| `bare_last` | — | 4.2 | 0.0 | 0.0 | 4.2 | 30.1 |
| `bare_order_where` | 279.8 | 117.0 | 125.6 | 302.7 | 296.8 | 164.3 |
| `chained_select_collapse` | — | 17.7 | 17.4 | 70.5 | 154.5 | 28.5 |
| `chained_where` | 36.6 | 6.6 | 7.1 | 105.5 | 177.6 | 23.9 |
| `contains_match` | 0.0 | 2.2 | 1.4 | 27.7 | 71.6 | 6.5 |
| `count_aggregate` | 29.4 | 4.2 | 4.1 | 64.2 | 162.2 | 20.3 |
| `cross_join` | 12628.8 | 3713.6 | — | 4051.3 | 4077.4 | — |
| `decs_count_bare_pred` | — | — | 4.1 | — | — | — |
| `distinct_by_count` | 41.4 | 15.7 | 15.7 | 70.6 | 159.1 | 27.2 |
| `distinct_by_order_take` | 242.2 | 22.0 | 23.4 | 124.9 | 158.7 | 49.5 |
| `distinct_by_order_to_array` | 240.8 | 21.9 | 23.4 | 125.2 | 160.5 | 49.5 |
| `distinct_count` | 41.6 | 15.5 | 15.6 | 70.6 | 154.4 | 27.4 |
| `distinct_count_pred` | 254.8 | 15.8 | 16.0 | 112.5 | 170.7 | 27.3 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.4 | 0.3 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 176.5 | 29.0 | 29.3 | 124.3 | 187.3 | 41.8 |
| `groupby_count` | 143.2 | 19.0 | 19.4 | 74.7 | 160.0 | 31.1 |
| `groupby_first` | 253.3 | 19.0 | 20.1 | 72.4 | 156.2 | 40.7 |
| `groupby_having_count` | 141.7 | 19.0 | 19.1 | 75.1 | 159.4 | 31.2 |
| `groupby_having_hidden_sum` | 176.8 | 22.2 | 22.3 | 118.8 | 183.3 | 34.2 |
| `groupby_having_post_where` | 174.3 | 20.3 | 20.4 | 114.7 | 180.1 | 32.4 |
| `groupby_max` | 175.5 | 24.7 | 24.9 | 120.5 | 184.2 | 35.0 |
| `groupby_min` | 173.9 | 25.5 | 25.2 | 121.0 | 183.6 | 34.8 |
| `groupby_multi_reducer` | 191.1 | 30.3 | 30.3 | 126.3 | 189.1 | 43.5 |
| `groupby_select_order` | 172.0 | 20.3 | 20.4 | 115.0 | 179.9 | 32.3 |
| `groupby_select_sum` | 201.3 | 38.5 | 38.5 | 102.1 | 185.6 | 50.3 |
| `groupby_sum` | 170.5 | 20.4 | 20.4 | 115.0 | 179.7 | 32.4 |
| `groupby_where_count` | 76.5 | 13.9 | 14.5 | 115.8 | 181.4 | 29.9 |
| `groupby_where_sum` | 87.2 | 14.2 | 14.8 | 116.6 | 181.9 | 31.3 |
| `join_count` | 38.3 | 52.0 | 65.0 | 112.3 | 177.5 | 65.1 |
| `join_groupby_count` | 157.7 | 77.3 | 88.9 | 177.9 | 224.8 | 260.4 |
| `join_groupby_to_array` | 191.6 | 79.1 | 91.3 | 215.2 | 211.3 | 289.4 |
| `join_probe` | — | — | — | — | — | 46.5 |
| `join_probe_build` | — | — | — | — | — | 79.1 |
| `join_select` | 150.4 | 74.1 | 85.0 | 188.7 | 211.5 | 222.8 |
| `join_where_count` | 39.7 | 62.3 | 76.2 | 161.1 | 193.0 | 79.6 |
| `last_match` | 0.0 | 5.8 | 14.0 | 65.2 | 159.4 | 30.8 |
| `long_count_aggregate` | 29.8 | 4.2 | 4.1 | 63.8 | 158.0 | 21.4 |
| `max_aggregate` | 31.3 | 6.1 | 6.8 | 58.8 | 157.3 | 16.9 |
| `min_aggregate` | 31.4 | 6.1 | 6.8 | 59.0 | 157.5 | 16.9 |
| `order_by_multi_key` | 341.1 | 274.6 | 283.0 | 459.8 | 450.9 | 334.7 |
| `order_distinct_take` | 138.7 | 15.7 | 99.0 | 72.6 | 155.7 | 31.6 |
| `order_reverse_normalized` | 38.4 | 16.3 | 20.0 | 70.5 | 162.8 | 32.9 |
| `order_take_desc` | 38.5 | 16.4 | 20.0 | 70.6 | 162.2 | 32.9 |
| `point_lookup` | — | — | — | — | — | 0.0 |
| `point_lookup_scan` | — | — | — | — | — | 8.3 |
| `reverse_distinct_by` | 296.8 | 21.1 | 28.3 | 71.3 | 155.8 | 43.8 |
| `reverse_take` | 0.1 | 0.0 | 0.2 | 0.0 | 26.2 | 58.5 |
| `reverse_take_select` | 0.0 | 0.0 | 0.2 | 0.0 | 26.2 | 58.6 |
| `select_count` | 0.1 | 0.0 | 2.2 | 65.1 | 2.2 | 0.0 |
| `select_many` | — | 189.4 | — | — | — | — |
| `select_where` | 195.0 | 11.2 | 19.3 | 197.8 | 188.5 | 37.6 |
| `select_where_count` | 33.0 | 5.2 | 7.4 | 65.3 | 150.2 | 23.0 |
| `select_where_order_take` | 37.0 | 12.2 | 14.8 | 72.7 | 162.8 | 34.6 |
| `select_where_sum` | 37.4 | 7.4 | 7.5 | 66.2 | 157.7 | 24.1 |
| `single_match` | 0.0 | 2.9 | 5.4 | 55.9 | 148.2 | 23.0 |
| `skip_take` | 0.5 | 0.1 | 0.2 | 3.1 | 2.8 | 0.3 |
| `skip_while_match` | 3.5 | 5.3 | 5.3 | 57.7 | 149.0 | 18.2 |
| `sort_first` | 38.2 | 11.0 | 13.4 | 65.6 | 159.4 | 31.6 |
| `sort_take` | 38.6 | 16.1 | 20.4 | 70.5 | 163.4 | 33.0 |
| `sort_take_select` | 38.2 | 16.4 | 20.3 | 71.0 | 163.1 | 33.1 |
| `sum_aggregate` | 30.2 | 2.1 | 2.1 | 54.5 | 156.8 | 13.5 |
| `sum_where` | 33.0 | 4.3 | 4.3 | 63.7 | 158.1 | 20.4 |
| `take_count` | 3.6 | 0.2 | 0.4 | 2.9 | 2.7 | 0.5 |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 1.4 | 1.1 | 0.3 |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 0.6 | 0.5 | 0.1 |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 0.7 | 0.6 | 0.2 |
| `take_while_match` | 7.8 | 2.4 | 2.5 | 29.0 | 72.5 | 16.6 |
| `to_array_filter` | 70.6 | 11.7 | 11.7 | 71.8 | 161.9 | 28.9 |
| `to_table` | — | 18.7 | 141.8 | 118.5 | 140.1 | 32.2 |
| `to_table_staged` | — | 54.6 | 56.6 | 143.0 | 164.0 | 68.6 |
| `where_join_count` | 39.7 | 29.5 | 41.1 | 132.2 | 166.3 | 46.8 |
| `zip_count_pred` | 39.1 | 15.8 | — | 317.6 | 318.8 | — |
| `zip_dot_product` | 46.9 | 12.6 | 10.6 | 312.4 | 315.1 | — |
| `zip_dot_product_3arg` | 46.8 | 12.7 | — | 311.9 | 315.7 | — |
| `zip_reverse_to_array` | — | 32.1 | — | 348.5 | 349.7 | — |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) | Table fold (m7) |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 34.7 | 0.3 | 0.7 | 29.6 | 25.8 | 7.2 |
| `all_match` | 27.5 | 0.3 | 0.2 | 18.8 | 24.9 | 7.2 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.1 | 1.0 | 3.6 | 18.5 | 24.5 | 7.4 |
| `bare_last` | — | 0.4 | 0.0 | 0.0 | 0.0 | 8.9 |
| `bare_order_where` | 184.9 | 33.8 | 35.0 | 105.9 | 51.7 | 68.4 |
| `chained_select_collapse` | — | 1.1 | 1.1 | 20.5 | 31.9 | 8.1 |
| `chained_where` | 36.6 | 0.6 | 0.9 | 36.3 | 29.9 | 10.6 |
| `contains_match` | 0.0 | 0.2 | 0.1 | 19.3 | 8.8 | 2.5 |
| `count_aggregate` | 29.8 | 0.3 | 0.6 | 29.1 | 25.1 | 7.3 |
| `cross_join` | 5967.3 | 717.5 | — | 831.1 | 766.7 | — |
| `decs_count_bare_pred` | — | — | 0.6 | — | — | — |
| `distinct_by_count` | 41.4 | 1.1 | 1.1 | 20.5 | 32.0 | 8.0 |
| `distinct_by_order_take` | 238.2 | 1.7 | 2.6 | 45.3 | 37.2 | 19.5 |
| `distinct_by_order_to_array` | 239.9 | 1.7 | 2.7 | 45.3 | 37.1 | 19.7 |
| `distinct_count` | 41.6 | 1.1 | 1.1 | 20.5 | 32.1 | 8.0 |
| `distinct_count_pred` | 252.3 | 1.1 | 1.3 | 37.6 | 41.8 | 8.0 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.1 | 0.0 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 175.4 | 1.5 | 1.8 | 37.0 | 42.9 | 8.9 |
| `groupby_count` | 153.2 | 1.3 | 1.5 | 20.5 | 32.3 | 8.4 |
| `groupby_first` | 252.8 | 1.3 | 2.3 | 20.5 | 32.9 | 10.0 |
| `groupby_having_count` | 141.8 | 1.3 | 1.5 | 20.5 | 32.3 | 8.5 |
| `groupby_having_hidden_sum` | 176.3 | 1.5 | 1.9 | 36.9 | 43.0 | 8.7 |
| `groupby_having_post_where` | 172.1 | 1.4 | 1.9 | 36.9 | 42.2 | 8.5 |
| `groupby_max` | 176.7 | 1.5 | 1.9 | 37.1 | 45.2 | 8.6 |
| `groupby_min` | 177.2 | 1.5 | 1.9 | 38.3 | 45.6 | 8.5 |
| `groupby_multi_reducer` | 191.7 | 1.6 | 1.9 | 37.2 | 43.7 | 9.0 |
| `groupby_select_order` | 176.6 | 1.4 | 1.6 | 37.8 | 41.9 | 8.4 |
| `groupby_select_sum` | 204.9 | 2.8 | 3.2 | 33.5 | 37.7 | 22.8 |
| `groupby_sum` | 171.6 | 1.4 | 1.9 | 37.7 | 42.0 | 8.4 |
| `groupby_where_count` | 76.9 | 0.9 | 1.3 | 37.1 | 39.7 | 11.2 |
| `groupby_where_sum` | 90.4 | 0.9 | 1.3 | 37.1 | 39.7 | 11.2 |
| `join_count` | 38.6 | 11.2 | 12.6 | 40.7 | 68.3 | 25.1 |
| `join_groupby_count` | 157.4 | 17.2 | 19.2 | 66.2 | 86.0 | 73.5 |
| `join_groupby_to_array` | 191.5 | 17.8 | 19.6 | 78.4 | 35.8 | 80.6 |
| `join_probe` | — | — | — | — | — | 16.6 |
| `join_probe_build` | — | — | — | — | — | 31.6 |
| `join_select` | 93.0 | 19.6 | 21.7 | 73.2 | 90.5 | 69.5 |
| `join_where_count` | 48.9 | 19.1 | 20.6 | 62.9 | 77.6 | 31.5 |
| `last_match` | 0.0 | 0.5 | 1.4 | 19.5 | 25.5 | 12.0 |
| `long_count_aggregate` | 29.7 | 0.3 | 0.6 | 29.3 | 25.1 | 7.3 |
| `max_aggregate` | 30.7 | 0.3 | 0.5 | 29.6 | 26.3 | 7.3 |
| `min_aggregate` | 31.0 | 0.3 | 0.5 | 29.6 | 26.3 | 7.4 |
| `order_by_multi_key` | 245.0 | 53.4 | 54.5 | 124.5 | 70.3 | 118.9 |
| `order_distinct_take` | 138.7 | 1.1 | 75.0 | 20.8 | 34.4 | 8.0 |
| `order_reverse_normalized` | 38.7 | 0.7 | 1.3 | 19.6 | 27.0 | 9.5 |
| `order_take_desc` | 38.7 | 0.7 | 1.3 | 19.6 | 27.0 | 9.3 |
| `point_lookup` | — | — | — | — | — | 0.0 |
| `point_lookup_scan` | — | — | — | — | — | 3.0 |
| `reverse_distinct_by` | 296.5 | 1.6 | 3.2 | 20.5 | 34.2 | 11.1 |
| `reverse_take` | 0.0 | 0.0 | 0.0 | 0.0 | 3.7 | 19.5 |
| `reverse_take_select` | 0.0 | 0.0 | 0.0 | 0.0 | 3.7 | 19.0 |
| `select_count` | 0.1 | 0.0 | 0.0 | 64.6 | 0.0 | 0.0 |
| `select_many` | — | 61.3 | — | — | — | — |
| `select_where` | 107.1 | 4.2 | 5.2 | 75.6 | 21.9 | 17.7 |
| `select_where_count` | 32.8 | 0.3 | 0.6 | 29.4 | 25.9 | 7.2 |
| `select_where_order_take` | 37.1 | 0.7 | 1.4 | 19.6 | 26.6 | 12.9 |
| `select_where_sum` | 37.2 | 0.4 | 0.6 | 20.2 | 24.9 | 7.4 |
| `single_match` | 0.0 | 0.4 | 1.1 | 44.2 | 22.3 | 9.2 |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.2 | 0.2 | 0.1 |
| `skip_while_match` | 3.5 | 0.4 | 0.4 | 44.4 | 21.8 | 7.6 |
| `sort_first` | 38.2 | 0.4 | 1.3 | 18.7 | 26.2 | 9.3 |
| `sort_take` | 38.4 | 0.7 | 1.3 | 19.6 | 27.2 | 9.6 |
| `sort_take_select` | 38.5 | 0.7 | 1.4 | 19.5 | 27.1 | 9.6 |
| `sum_aggregate` | 30.3 | 0.3 | 0.0 | 22.4 | 24.2 | 7.3 |
| `sum_where` | 33.1 | 0.3 | 0.6 | 29.5 | 25.8 | 7.3 |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.2 | 0.3 | 0.1 |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 0.5 | 0.1 | 0.0 |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 0.2 | 0.0 | 0.0 |
| `take_where_count` | 0.9 | 0.0 | 0.0 | 0.3 | 0.0 | 0.0 |
| `take_while_match` | 7.8 | 0.2 | 0.3 | 19.0 | 9.0 | 7.3 |
| `to_array_filter` | 48.4 | 3.3 | 3.3 | 22.0 | 33.5 | 13.0 |
| `to_table` | — | 14.1 | 36.9 | 49.4 | 52.0 | 21.1 |
| `to_table_staged` | — | 25.8 | 26.0 | 53.0 | 61.5 | 33.9 |
| `where_join_count` | 41.7 | 6.0 | 6.7 | 47.6 | 40.5 | 19.7 |
| `zip_count_pred` | 39.6 | 0.1 | — | 113.5 | 33.5 | — |
| `zip_dot_product` | 46.9 | 0.1 | 0.1 | 113.6 | 33.3 | — |
| `zip_dot_product_3arg` | 46.8 | 0.1 | — | 113.6 | 33.3 | — |
| `zip_reverse_to_array` | — | 4.5 | — | 124.9 | 50.6 | — |
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
- **m7 absent families** — `zip_*` / `cross_join` (lockstep pairing over an unordered slot walk is meaningless) and `select_many` (flat fixture, no nested array field; array-only). Everything else in the m7 column is instantiated, and the `groupby_*` family is a fused emit (`plan_group_by_core` over the usage-pruned slot walk). The remaining cascade cells are `join_groupby_*` (join |> group_by over a table lead declines) and the reverse family (no backward slot walk) — both named deferred edges (see `LINQ_TO_TABLE.md`), so those cells are the numbers a fix would improve.
- **`point_lookup` / `point_lookup_scan` non-m7** — m7-only pair: only a table source has a key to probe (`where(kv.key == X)` + terminator → `key_exists` / `tab?[X]`, O(1)); the `_scan` twin forces the same query through the walk (compound `&&` predicate declines the probe) to show the gap. Other sources have no analog by design.
- **`join_probe` / `join_probe_build` non-m7** — m7-only A/B pair: a table srcB joined on its bare key probes the user's table per lead row (no internal join hash, no build loop); the `_build` twin feeds the identical rows pre-materialized to a kv array, forcing the hashed build. Other sources have no keyed-srcB analog by design.
- **`to_table` / `to_table_staged` SQL** — `to_table` isn't an SQL terminator (`_sql` pass-through has no table sink). All in-memory sources are instantiated: array / XML / JSON / table fuse the insert-loop sink (`_staged` is the materialize-then-`to_table_move` shape every chain had before the sink arm); decs declines by design (explicit guard in its loop_or_count lane), so its `to_table` cell is the full tier-2 cascade — currently slower than its `_staged` twin, which fuses the array materialization first. That gap is the motivating number for a future decs sink hook.

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
