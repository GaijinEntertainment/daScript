# Benchmarks - SQL / Array / Decs / XML / JSON / Table comparison

Six lanes run the same query families over one `Car` schema (n = 100 000 cars, 100 dealers,
5 brands); cells are ns/op, ` - ` = intentionally absent lane (see "Missing lanes"). The tables
between the `BENCH:TABLES` markers are machine-generated (see "How to re-run"); all other text
is hand-edited.

Each lane lives in its own file (`array.das` / `decs.das` / `xml.das` / `json.das` / `sql.das` /
`table.das`) with the source fixture built once in `[init]`; the sweep runs one process per file,
so a lane is never contaminated by another lane's code in the same process (this is why JIT cells
are stable now).

- **m1 SQL** - `_fold(db |> select_from(type<Car>) |> ...)` over in-memory SQLite; `_fold` passes the chain to `_sql`.
- **m3f Array** - `_fold` over `each(array<Car>)`.
- **m4 Decs** - `_fold` over `from_decs_template(type<DecsCar>)` (per-archetype walk).
- **m5f XML** - `_fold` over `from_xml_node(root, type<Car>)` (`XmlAdapter` fuses + field-prunes).
- **m6f JSON** - `_fold` over `from_json(jv, type<Car>)` (`JsonAdapter`, same machinery, array walk).
- **m7 Table** - `_fold` over `each_kv(table<int; Car>)` (`TableAdapter`; kv usage-pruning picks keys-only /
  values-only / zipped slot walks; key-equality `where` + terminator folds to an O(1) probe, residual
  conjuncts right of the key-equality evaluating on the probed element only - the `point_lookup` /
  `point_lookup_residual` / `point_lookup_scan` trio measures it; joins fuse on either side, and a table srcB
  joined on its bare key probes the table instead of building the join hash - the `join_probe` /
  `join_probe_build` pair measures it; a trailing `to_table()` inserts straight into the result
  table with no intermediate array - the `to_table` / `to_table_staged` pair measures it;
  group_by fuses through `plan_group_by_core` with the usage-pruned slot walk as the bucket-fill
  loop; join+group_by and reverse defer to tier-2). Under JIT, `keys`/`values` for-loop sources
  compile to an inline open-addressed slot walk (no per-element C++ iterator calls), so the m7
  JIT column is fused codegen end to end.

`0.00` = early-exit terminator below timer resolution ("free"). Chain shapes are in
`benchmarks/README.md`; the splice arms each fires are in `doc/source/reference/linq_fold_patterns.rst`.

**Current tables: AMD Ryzen Threadripper 3990X (Windows, Release build), single sweep.**
Absolute cells move with the capture box and run-to-run within ~10-30% on the small ones -
the cross-lane ratios are the signal, not the second decimal. (On Apple silicon the JIT
column is additionally high-variance run-to-run; read INTERP as the reliable signal there.)

<!-- BENCH:TABLES BEGIN -->
*Generated 2026-08-22 by `benchmarks/sql/_update_results.das` - ns/op; ` - ` = absent lane. Edit the prose around the markers, not the tables.*

## INTERP

| Benchmark | SQLite (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) | Table fold (m7) |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 49.0 | 8.7 | 8.9 | 103.1 | 194.9 | 27.9 |
| `all_match` | 37.8 | 4.8 | 4.8 | 99.6 | 178.8 | 23.9 |
| `any_match` | 0.1 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 43.3 | 8.5 | 12.0 | 107.9 | 183.8 | 29.7 |
| `bare_last` | - | 6.3 | 0.0 | 0.0 | 7.0 | 46.2 |
| `bare_order_where` | 452.6 | 181.4 | 193.5 | 586.0 | 696.8 | 300.0 |
| `chained_select_collapse` | - | 27.5 | 27.9 | 130.1 | 230.8 | 58.8 |
| `chained_where` | 49.0 | 8.9 | 9.6 | 183.6 | 284.9 | 33.1 |
| `contains_match` | 0.1 | 3.6 | 2.4 | 56.9 | 95.5 | 9.3 |
| `count_aggregate` | 42.1 | 6.2 | 6.1 | 108.4 | 199.7 | 29.8 |
| `cross_join` | 21843.2 | 5345.8 | - | 5878.5 | 7890.2 | - |
| `decs_count_bare_pred` | - | - | 6.1 | - | - | - |
| `distinct_by_count` | 64.2 | 24.9 | 25.5 | 138.5 | 242.2 | 48.2 |
| `distinct_by_order_take` | 399.1 | 31.5 | 33.8 | 228.6 | 314.9 | 77.2 |
| `distinct_by_order_to_array` | 400.6 | 31.4 | 32.7 | 228.3 | 341.2 | 77.8 |
| `distinct_count` | 64.9 | 24.8 | 26.2 | 137.8 | 247.8 | 57.8 |
| `distinct_count_pred` | 429.0 | 25.0 | 27.7 | 216.2 | 325.2 | 49.2 |
| `distinct_take` | 0.1 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.1 | 0.0 | 0.1 | 0.8 | 0.6 | 0.1 |
| `first_match` | 0.1 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.1 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 294.2 | 43.4 | 43.7 | 226.3 | 330.8 | 77.5 |
| `groupby_count` | 258.7 | 31.6 | 32.0 | 148.0 | 229.9 | 64.0 |
| `groupby_first` | 414.1 | 28.9 | 32.2 | 146.2 | 237.2 | 76.1 |
| `groupby_having_count` | 247.9 | 32.1 | 32.4 | 147.5 | 238.8 | 62.6 |
| `groupby_having_hidden_sum` | 299.2 | 36.0 | 36.1 | 218.0 | 342.2 | 65.2 |
| `groupby_having_post_where` | 293.2 | 31.1 | 31.8 | 214.9 | 351.6 | 56.4 |
| `groupby_max` | 298.2 | 38.6 | 39.3 | 240.7 | 354.2 | 71.6 |
| `groupby_min` | 301.5 | 41.8 | 40.1 | 217.7 | 296.6 | 72.4 |
| `groupby_multi_reducer` | 332.0 | 48.5 | 45.6 | 222.6 | 314.0 | 79.0 |
| `groupby_select_order` | 296.2 | 31.3 | 32.3 | 225.2 | 326.1 | 60.6 |
| `groupby_select_sum` | 302.3 | 51.6 | 50.6 | 166.2 | 263.1 | 74.7 |
| `groupby_sum` | 319.4 | 31.8 | 31.8 | 227.6 | 302.7 | 52.6 |
| `groupby_where_count` | 139.3 | 21.5 | 21.8 | 208.0 | 302.6 | 44.4 |
| `groupby_where_sum` | 177.1 | 21.3 | 21.9 | 198.8 | 302.7 | 44.6 |
| `join_count` | 58.6 | 58.4 | 74.5 | 183.3 | 247.9 | 90.8 |
| `join_groupby_count` | 274.8 | 102.1 | 118.4 | 295.5 | 393.2 | 355.9 |
| `join_groupby_to_array` | 330.1 | 102.7 | 122.2 | 369.7 | 550.0 | 523.9 |
| `join_probe` | - | - | - | - | - | 73.4 |
| `join_probe_build` | - | - | - | - | - | 106.5 |
| `join_select` | 269.5 | 93.8 | 114.3 | 367.2 | 449.1 | 443.3 |
| `join_where_count` | 57.3 | 74.0 | 95.6 | 275.3 | 344.3 | 119.4 |
| `last_match` | 0.1 | 9.1 | 19.8 | 106.5 | 183.5 | 50.3 |
| `long_count_aggregate` | 41.8 | 6.2 | 6.2 | 107.6 | 200.6 | 28.3 |
| `max_aggregate` | 48.5 | 10.2 | 11.9 | 108.3 | 174.0 | 28.5 |
| `min_aggregate` | 48.8 | 10.2 | 12.3 | 106.1 | 192.3 | 27.8 |
| `order_by_multi_key` | 524.3 | 424.8 | 450.4 | 771.8 | 883.3 | 595.5 |
| `order_distinct_take` | 235.9 | 25.4 | 218.6 | 145.6 | 225.3 | 52.2 |
| `order_reverse_normalized` | 65.7 | 21.9 | 22.1 | 136.3 | 202.3 | 65.0 |
| `order_take_desc` | 65.0 | 21.4 | 22.0 | 137.8 | 202.9 | 53.8 |
| `point_lookup` | - | - | - | - | - | 0.0 |
| `point_lookup_residual` | - | - | - | - | - | 0.0 |
| `point_lookup_scan` | - | - | - | - | - | 18.1 |
| `reverse_distinct_by` | 486.6 | 39.1 | 37.9 | 142.9 | 217.7 | 83.3 |
| `reverse_take` | 0.1 | 0.0 | 0.3 | 0.1 | 50.2 | 107.8 |
| `reverse_take_select` | 0.1 | 0.0 | 0.3 | 0.1 | 44.8 | 123.0 |
| `select_count` | 0.2 | 0.0 | 2.1 | 74.2 | 2.4 | 0.0 |
| `select_many` | - | 313.0 | - | - | - | - |
| `select_where` | 331.6 | 17.4 | 30.8 | 366.6 | 469.6 | 78.8 |
| `select_where_count` | 51.5 | 7.4 | 11.9 | 112.3 | 169.6 | 31.5 |
| `select_where_order_take` | 58.8 | 17.5 | 19.2 | 125.9 | 188.3 | 62.0 |
| `select_where_sum` | 63.2 | 11.7 | 11.9 | 111.9 | 169.7 | 36.1 |
| `single_match` | 0.1 | 4.8 | 8.0 | 105.9 | 168.8 | 45.3 |
| `skip_take` | 0.8 | 0.2 | 0.3 | 5.8 | 5.7 | 0.6 |
| `skip_while_match` | 6.3 | 7.4 | 7.7 | 112.5 | 192.0 | 29.0 |
| `sort_first` | 65.4 | 13.5 | 16.7 | 120.2 | 213.2 | 65.0 |
| `sort_take` | 65.3 | 22.7 | 22.6 | 143.0 | 223.2 | 69.7 |
| `sort_take_select` | 65.7 | 21.8 | 23.9 | 147.0 | 214.6 | 62.2 |
| `sum_aggregate` | 42.7 | 2.3 | 2.3 | 98.2 | 175.1 | 20.9 |
| `sum_where` | 45.1 | 6.2 | 6.3 | 112.2 | 167.6 | 28.2 |
| `take_count` | 6.4 | 0.4 | 0.6 | 5.4 | 4.4 | 1.1 |
| `take_count_filtered` | 1.8 | 0.2 | 0.2 | 2.1 | 1.5 | 0.6 |
| `take_sum_aggregate` | 1.4 | 0.1 | 0.1 | 1.0 | 0.7 | 0.2 |
| `take_where_count` | 1.4 | 0.1 | 0.1 | 1.1 | 0.9 | 0.3 |
| `take_while_match` | 17.3 | 3.0 | 3.0 | 52.6 | 93.0 | 26.4 |
| `to_array_filter` | 113.2 | 19.4 | 19.3 | 129.9 | 196.5 | 46.4 |
| `to_table` | - | 33.3 | 216.8 | 212.7 | 287.5 | 62.3 |
| `to_table_staged` | - | 78.7 | 85.1 | 258.7 | 319.9 | 107.0 |
| `where_join_count` | 57.2 | 37.5 | 53.4 | 222.3 | 284.9 | 61.3 |
| `zip_count_pred` | 62.0 | 29.1 | - | 617.6 | 781.8 | - |
| `zip_dot_product` | 77.6 | 21.1 | 16.6 | 588.7 | 788.6 | - |
| `zip_dot_product_3arg` | 77.8 | 21.1 | - | 585.9 | 916.4 | - |
| `zip_reverse_to_array` | - | 45.6 | - | 646.4 | 1026.9 | - |

## JIT

| Benchmark | SQLite (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) | Table fold (m7) |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 73.9 | 0.4 | 1.1 | 38.4 | 50.8 | 9.4 |
| `all_match` | 49.9 | 0.5 | 0.6 | 36.2 | 46.3 | 8.8 |
| `any_match` | 0.1 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 56.4 | 0.7 | 2.7 | 33.7 | 46.4 | 9.2 |
| `bare_last` | - | 0.6 | 0.0 | 0.0 | 0.0 | 13.0 |
| `bare_order_where` | 283.2 | 69.1 | 73.3 | 168.0 | 185.0 | 101.7 |
| `chained_select_collapse` | - | 1.9 | 1.9 | 30.6 | 67.7 | 10.6 |
| `chained_where` | 53.8 | 0.6 | 0.9 | 46.1 | 75.1 | 12.6 |
| `contains_match` | 0.0 | 0.2 | 0.2 | 9.7 | 15.6 | 3.3 |
| `count_aggregate` | 43.9 | 0.3 | 0.9 | 41.8 | 44.4 | 8.7 |
| `cross_join` | 10646.6 | 1753.9 | - | 1475.1 | 1833.2 | - |
| `decs_count_bare_pred` | - | - | 0.8 | - | - | - |
| `distinct_by_count` | 73.0 | 1.7 | 1.9 | 30.7 | 72.8 | 10.2 |
| `distinct_by_order_take` | 480.4 | 2.8 | 4.5 | 58.3 | 105.0 | 23.3 |
| `distinct_by_order_to_array` | 400.0 | 2.5 | 4.7 | 56.9 | 96.8 | 23.9 |
| `distinct_count` | 71.9 | 1.9 | 1.8 | 31.0 | 83.7 | 10.3 |
| `distinct_count_pred` | 531.1 | 1.5 | 2.0 | 52.1 | 105.1 | 9.6 |
| `distinct_take` | 0.1 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.2 | 0.0 | 0.0 | 0.2 | 0.0 | 0.0 |
| `first_match` | 0.1 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.1 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 336.4 | 2.7 | 3.2 | 47.2 | 93.6 | 10.1 |
| `groupby_count` | 300.3 | 2.7 | 2.5 | 30.5 | 86.8 | 10.4 |
| `groupby_first` | 449.4 | 2.0 | 3.6 | 29.8 | 84.0 | 13.5 |
| `groupby_having_count` | 266.8 | 2.0 | 2.5 | 30.0 | 74.8 | 11.2 |
| `groupby_having_hidden_sum` | 390.8 | 2.0 | 3.4 | 46.6 | 100.2 | 10.8 |
| `groupby_having_post_where` | 327.0 | 1.9 | 2.1 | 45.6 | 99.4 | 11.4 |
| `groupby_max` | 301.6 | 2.4 | 2.7 | 44.8 | 81.7 | 11.3 |
| `groupby_min` | 328.7 | 2.1 | 3.2 | 47.3 | 85.1 | 12.6 |
| `groupby_multi_reducer` | 432.3 | 2.1 | 2.5 | 45.6 | 97.3 | 11.6 |
| `groupby_select_order` | 321.2 | 2.4 | 3.0 | 46.0 | 95.7 | 10.3 |
| `groupby_select_sum` | 336.0 | 5.2 | 5.4 | 38.9 | 67.9 | 23.0 |
| `groupby_sum` | 344.7 | 1.7 | 3.0 | 46.0 | 88.0 | 11.5 |
| `groupby_where_count` | 136.0 | 1.4 | 2.0 | 46.6 | 79.8 | 14.0 |
| `groupby_where_sum` | 144.2 | 1.5 | 1.4 | 47.5 | 88.4 | 13.5 |
| `join_count` | 71.0 | 6.8 | 7.7 | 44.0 | 93.3 | 23.4 |
| `join_groupby_count` | 348.0 | 13.2 | 17.5 | 77.7 | 153.8 | 126.2 |
| `join_groupby_to_array` | 403.0 | 15.5 | 19.5 | 97.6 | 173.5 | 137.4 |
| `join_probe` | - | - | - | - | - | 17.7 |
| `join_probe_build` | - | - | - | - | - | 29.8 |
| `join_select` | 180.7 | 25.1 | 24.8 | 106.8 | 175.7 | 127.6 |
| `join_where_count` | 74.0 | 12.2 | 12.5 | 71.0 | 140.2 | 29.1 |
| `last_match` | 0.1 | 0.6 | 1.9 | 37.0 | 58.4 | 15.7 |
| `long_count_aggregate` | 55.7 | 0.4 | 0.9 | 42.7 | 56.2 | 9.5 |
| `max_aggregate` | 62.6 | 0.4 | 0.5 | 42.0 | 57.2 | 9.4 |
| `min_aggregate` | 58.4 | 0.4 | 0.5 | 41.6 | 49.3 | 8.8 |
| `order_by_multi_key` | 411.3 | 132.1 | 98.7 | 211.5 | 211.8 | 203.4 |
| `order_distinct_take` | 253.0 | 2.3 | 165.4 | 32.0 | 59.6 | 11.4 |
| `order_reverse_normalized` | 68.0 | 1.4 | 1.4 | 40.1 | 59.5 | 13.3 |
| `order_take_desc` | 68.0 | 1.0 | 1.4 | 39.8 | 56.9 | 11.2 |
| `point_lookup` | - | - | - | - | - | 0.0 |
| `point_lookup_residual` | - | - | - | - | - | 0.0 |
| `point_lookup_scan` | - | - | - | - | - | 4.3 |
| `reverse_distinct_by` | 495.9 | 2.1 | 5.5 | 32.5 | 86.2 | 16.6 |
| `reverse_take` | 0.1 | 0.0 | 0.1 | 0.0 | 5.5 | 31.7 |
| `reverse_take_select` | 0.1 | 0.0 | 0.1 | 0.0 | 4.8 | 30.6 |
| `select_count` | 0.2 | 0.0 | 0.0 | 13.8 | 0.0 | 0.0 |
| `select_many` | - | 182.3 | - | - | - | - |
| `select_where` | 165.8 | 6.6 | 10.5 | 111.6 | 123.3 | 28.3 |
| `select_where_count` | 79.5 | 0.4 | 0.9 | 41.6 | 58.8 | 9.0 |
| `select_where_order_take` | 72.7 | 0.8 | 1.8 | 42.5 | 61.0 | 16.9 |
| `select_where_sum` | 67.6 | 0.6 | 0.9 | 43.4 | 59.3 | 9.0 |
| `single_match` | 0.0 | 0.5 | 1.2 | 28.8 | 41.1 | 12.3 |
| `skip_take` | 0.5 | 0.0 | 0.0 | 1.7 | 0.3 | 0.1 |
| `skip_while_match` | 10.1 | 0.5 | 0.5 | 30.2 | 49.9 | 9.3 |
| `sort_first` | 78.5 | 0.5 | 1.6 | 53.9 | 58.6 | 11.8 |
| `sort_take` | 77.4 | 1.4 | 1.7 | 57.0 | 58.4 | 12.7 |
| `sort_take_select` | 73.4 | 1.3 | 1.4 | 54.7 | 49.4 | 12.2 |
| `sum_aggregate` | 46.8 | 0.5 | 0.0 | 53.2 | 53.9 | 8.9 |
| `sum_where` | 48.3 | 0.6 | 0.8 | 44.1 | 53.1 | 8.7 |
| `take_count` | 3.5 | 0.2 | 0.2 | 1.7 | 0.5 | 0.3 |
| `take_count_filtered` | 2.1 | 0.0 | 0.0 | 0.4 | 0.1 | 0.1 |
| `take_sum_aggregate` | 1.6 | 0.0 | 0.0 | 0.3 | 0.0 | 0.0 |
| `take_where_count` | 1.7 | 0.0 | 0.0 | 0.2 | 0.1 | 0.0 |
| `take_while_match` | 18.3 | 0.3 | 0.3 | 9.1 | 25.7 | 9.2 |
| `to_array_filter` | 66.9 | 4.4 | 4.0 | 34.0 | 63.9 | 15.6 |
| `to_table` | - | 30.6 | 92.6 | 69.5 | 108.6 | 33.4 |
| `to_table_staged` | - | 42.1 | 41.7 | 80.8 | 116.0 | 49.8 |
| `where_join_count` | 57.4 | 3.9 | 5.7 | 57.6 | 100.4 | 19.1 |
| `zip_count_pred` | 63.4 | 0.1 | - | 176.0 | 201.3 | - |
| `zip_dot_product` | 111.6 | 0.1 | 0.1 | 174.3 | 197.5 | - |
| `zip_dot_product_3arg` | 81.1 | 0.1 | - | 172.8 | 193.0 | - |
| `zip_reverse_to_array` | - | 8.4 | - | 202.3 | 224.1 | - |
<!-- BENCH:TABLES END -->

## Missing lanes (the ` - ` cells)

Each empty cell's reason is also in the bench `.das` file's comment; SQL gaps are detailed in
[`sqlite_linq_gaps.md`](../../history/linq_fold/sqlite_linq_gaps.md).

- **`bare_last` SQL** - SQLite has no inherent row order; bare `last()` is meaningful only for ordered sources (array/decs/XML/JSON). By design.
- **`chained_select_collapse` SQL** - `_sql` doesn't emit `COUNT(DISTINCT expr)`. By design.
- **`cross_join` Decs** - standalone `from_decs_template` yields anonymous tuples; no typed-lambda cross form / `_fold` integration yet.
- **`select_many` SQL / Decs / XML / JSON** - needs a per-element nested `array<...>` field the flat `Car` schema, decs components, and DOM-attr sources don't have. Array-only by design.
- **`decs_count_bare_pred` non-decs** - decs-only family (exercises a decs-walk root cause); no meaningful analog elsewhere.
- **`reverse_distinct_by` m4 / m5f** - array uses the backward-index walk; non-array sources fuse the forward keep-last splice (decs 27.6/5.0, XML 74.5/22.2); SQL uses MAX(pk).
- **`order_distinct_take` m4 vs m3f** - `unique_key` hashes workhorse keys directly (array `int`) but string-interpolates structs (decs `DecsBrand`); the gap is per-element string hashing, not decs-walk. `distinct_by_count` is the key-based variant (m4 parity).
- **`zip_reverse_to_array` / `zip_*` SQL / Decs** - `reverse` has no SQL order key; zip is not relational / not expressible over one archetype walk. By design. (XML/JSON zip lanes are lit, partially fused.)
- **m7 absent families** - `zip_*` / `cross_join` (lockstep pairing over an unordered slot walk is meaningless) and `select_many` (flat fixture, no nested array field; array-only). Everything else in the m7 column is instantiated, and the `groupby_*` family is a fused emit (`plan_group_by_core` over the usage-pruned slot walk). The remaining cascade cells are `join_groupby_*` (join |> group_by over a table lead declines) and the reverse family (no backward slot walk) - both named deferred edges (see `history/linq_fold/LINQ_TO_TABLE.md`), so those cells are the numbers a fix would improve.
- **`point_lookup` / `point_lookup_residual` / `point_lookup_scan` non-m7** - m7-only trio: only a table source has a key to probe (`where(kv.key == X)` + terminator -> `key_exists` / `tab?[X]`, O(1)); the `_residual` twin adds a conjunct right of the key-equality (`key == X && residual`), which probes and evaluates the residual on the probed element only; the `_scan` control puts the residual conjunct FIRST, so the key-equality is not the leftmost conjunct and the probe matcher correctly declines to the walk. Other sources have no analog by design.
- **`join_probe` / `join_probe_build` non-m7** - m7-only A/B pair: a table srcB joined on its bare key probes the user's table per lead row (no internal join hash, no build loop); the `_build` twin feeds the identical rows pre-materialized to a kv array, forcing the hashed build. Other sources have no keyed-srcB analog by design.
- **`to_table` / `to_table_staged` SQL** - `to_table` isn't an SQL terminator (`_sql` pass-through has no table sink). All in-memory sources are instantiated: array / XML / JSON / table fuse the insert-loop sink (`_staged` is the materialize-then-`to_table_move` shape every chain had before the sink arm); decs declines by design (explicit guard in its loop_or_count lane), so its `to_table` cell is the full tier-2 cascade - currently slower than its `_staged` twin, which fuses the array materialization first. That gap is the motivating number for a future decs sink hook.

## Accepted floors

A few m4 (decs) cells stay >=1.5x their m3f (array) INTERP counterpart for structural reasons -
decs columnar storage fetches all N columns per element (`last_match`, `select_where`), and
`order_distinct_take` pays `unique_key` string hashing on struct keys. Structural, not splice/emit
gaps; not chased further.

## How to re-run

The two matrices (between the `BENCH:TABLES` markers) are regenerated by
`benchmarks/sql/_update_results.das` from machine-readable bench output - never hand-edit the cells.

```bash
rm -f /tmp/bench_interp.json /tmp/bench_jit.json
for f in benchmarks/sql/*.das; do
  [[ "$(basename "$f")" == _* ]] && continue
  bin/daslang      dastest/dastest.das -- --bench --bench-format json --test "$f" >> /tmp/bench_interp.json 2>/dev/null
  bin/daslang -jit dastest/dastest.das -- --bench --bench-format json --test "$f" >> /tmp/bench_jit.json   2>/dev/null
done
bin/daslang benchmarks/sql/_update_results.das -- --interp /tmp/bench_interp.json --jit /tmp/bench_jit.json --date YYYY-MM-DD \
    --site-json site/files/performance_bench.json --machine "<capture box>"
```

`--site-json` also regenerates the record behind daslang.io/performance.html from the same
sweep, so the page and these tables can never disagree; `--machine` names the capture box on
the page. Skip both only for a local scratch run you are not committing.

Edit the prose (header, "Missing lanes", this section) by hand; the tool rewrites only the tables.
If JIT fails to compile (`host_jit_triple()` missing), `bin/daslang` is stale vs
`src/builtin/module_jit.cpp` - rebuild and retry.
