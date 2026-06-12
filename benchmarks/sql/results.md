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
  values-only / zipped slot walks; key-equality `where` + terminator folds to an O(1) probe, residual
  conjuncts right of the key-equality evaluating on the probed element only — the `point_lookup` /
  `point_lookup_residual` / `point_lookup_scan` trio measures it; joins fuse on either side, and a table srcB
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
| `aggregate_match` | 35.0 | 5.8 | 5.8 | 60.5 | 152.8 | 19.1 |
| `all_match` | 27.9 | 3.5 | 3.4 | 56.2 | 146.1 | 15.8 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.3 | 6.2 | 8.7 | 58.6 | 156.9 | 17.2 |
| `bare_last` | — | 4.2 | 0.0 | 0.0 | 4.2 | 31.0 |
| `bare_order_where` | 282.3 | 117.3 | 126.3 | 301.5 | 287.8 | 162.7 |
| `chained_select_collapse` | — | 17.5 | 17.6 | 70.5 | 160.3 | 28.1 |
| `chained_where` | 37.0 | 6.8 | 7.1 | 105.0 | 175.7 | 24.1 |
| `contains_match` | 0.0 | 2.2 | 1.4 | 27.8 | 68.4 | 6.5 |
| `count_aggregate` | 30.1 | 4.2 | 4.1 | 63.6 | 147.2 | 20.2 |
| `cross_join` | 12614.0 | 3686.9 | — | 4012.4 | 4033.2 | — |
| `decs_count_bare_pred` | — | — | 4.1 | — | — | — |
| `distinct_by_count` | 41.2 | 15.9 | 15.8 | 70.9 | 156.2 | 26.1 |
| `distinct_by_order_take` | 242.3 | 22.1 | 23.2 | 125.0 | 158.2 | 48.7 |
| `distinct_by_order_to_array` | 242.4 | 22.2 | 23.3 | 125.7 | 158.6 | 48.5 |
| `distinct_count` | 41.3 | 15.8 | 15.7 | 71.1 | 154.4 | 27.2 |
| `distinct_count_pred` | 253.7 | 16.0 | 15.7 | 112.5 | 171.2 | 26.1 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.4 | 0.3 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 173.5 | 29.2 | 29.2 | 123.7 | 187.6 | 40.9 |
| `groupby_count` | 142.8 | 19.7 | 19.1 | 74.7 | 159.0 | 30.8 |
| `groupby_first` | 253.1 | 19.1 | 19.8 | 72.3 | 155.3 | 40.1 |
| `groupby_having_count` | 141.5 | 19.2 | 19.1 | 75.1 | 158.6 | 30.7 |
| `groupby_having_hidden_sum` | 176.7 | 22.5 | 22.2 | 118.1 | 183.3 | 33.6 |
| `groupby_having_post_where` | 172.3 | 20.5 | 20.5 | 114.4 | 180.3 | 31.8 |
| `groupby_max` | 175.3 | 25.1 | 24.9 | 119.3 | 183.0 | 34.4 |
| `groupby_min` | 175.5 | 25.6 | 25.2 | 119.7 | 183.9 | 34.5 |
| `groupby_multi_reducer` | 192.5 | 30.7 | 30.1 | 125.0 | 187.9 | 43.0 |
| `groupby_select_order` | 172.4 | 20.5 | 20.5 | 114.8 | 180.3 | 31.6 |
| `groupby_select_sum` | 198.4 | 39.6 | 38.5 | 101.4 | 185.4 | 49.4 |
| `groupby_sum` | 175.0 | 20.5 | 20.4 | 113.9 | 179.9 | 31.5 |
| `groupby_where_count` | 76.5 | 13.9 | 14.5 | 116.2 | 178.8 | 29.9 |
| `groupby_where_sum` | 88.0 | 14.2 | 14.7 | 117.0 | 181.9 | 29.9 |
| `join_count` | 38.5 | 52.4 | 64.7 | 112.7 | 177.1 | 65.0 |
| `join_groupby_count` | 157.7 | 77.4 | 89.0 | 178.3 | 223.4 | 260.9 |
| `join_groupby_to_array` | 190.6 | 79.0 | 91.5 | 214.9 | 212.3 | 290.9 |
| `join_probe` | — | — | — | — | — | 47.1 |
| `join_probe_build` | — | — | — | — | — | 81.2 |
| `join_select` | 149.7 | 73.2 | 85.2 | 194.3 | 206.2 | 223.1 |
| `join_where_count` | 40.0 | 62.3 | 76.1 | 160.8 | 192.9 | 80.6 |
| `last_match` | 0.0 | 5.8 | 14.0 | 65.1 | 152.1 | 31.6 |
| `long_count_aggregate` | 29.8 | 4.2 | 4.1 | 63.5 | 147.5 | 20.7 |
| `max_aggregate` | 31.3 | 6.1 | 6.8 | 59.0 | 155.3 | 17.1 |
| `min_aggregate` | 31.3 | 6.1 | 6.8 | 93.0 | 155.1 | 17.2 |
| `order_by_multi_key` | 335.4 | 277.1 | 281.9 | 458.1 | 443.1 | 333.9 |
| `order_distinct_take` | 137.7 | 15.9 | 98.5 | 73.0 | 155.2 | 31.1 |
| `order_reverse_normalized` | 38.4 | 16.3 | 20.0 | 71.2 | 162.5 | 33.3 |
| `order_take_desc` | 38.7 | 16.5 | 20.0 | 70.1 | 162.6 | 33.4 |
| `point_lookup` | — | — | — | — | — | 0.0 |
| `point_lookup_residual` | — | — | — | — | — | 0.0 |
| `point_lookup_scan` | — | — | — | — | — | 8.6 |
| `reverse_distinct_by` | 297.2 | 21.2 | 27.8 | 70.8 | 155.4 | 43.7 |
| `reverse_take` | 0.1 | 0.0 | 0.2 | 0.0 | 26.2 | 58.4 |
| `reverse_take_select` | 0.0 | 0.0 | 0.2 | 0.0 | 26.2 | 58.5 |
| `select_count` | 0.1 | 0.0 | 2.2 | 64.4 | 2.2 | 0.0 |
| `select_many` | — | 191.1 | — | — | — | — |
| `select_where` | 198.1 | 11.5 | 19.4 | 196.0 | 184.9 | 37.7 |
| `select_where_count` | 33.1 | 5.2 | 7.4 | 64.8 | 152.9 | 22.3 |
| `select_where_order_take` | 37.0 | 12.3 | 14.9 | 72.5 | 157.6 | 34.5 |
| `select_where_sum` | 37.3 | 7.5 | 7.5 | 66.6 | 154.3 | 23.4 |
| `single_match` | 0.0 | 2.8 | 5.5 | 55.9 | 143.4 | 23.0 |
| `skip_take` | 0.5 | 0.1 | 0.2 | 3.0 | 2.8 | 0.3 |
| `skip_while_match` | 3.5 | 5.2 | 5.3 | 57.6 | 146.2 | 18.2 |
| `sort_first` | 38.3 | 11.1 | 13.3 | 65.1 | 159.4 | 31.7 |
| `sort_take` | 38.6 | 16.1 | 20.1 | 70.6 | 162.9 | 32.8 |
| `sort_take_select` | 38.6 | 16.4 | 20.1 | 70.7 | 162.8 | 33.1 |
| `sum_aggregate` | 30.1 | 2.1 | 2.1 | 54.9 | 145.8 | 13.4 |
| `sum_where` | 32.9 | 4.2 | 4.3 | 63.6 | 147.2 | 20.5 |
| `take_count` | 3.6 | 0.2 | 0.4 | 2.9 | 2.7 | 0.5 |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 1.3 | 1.1 | 0.3 |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 0.6 | 0.5 | 0.1 |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 0.7 | 0.6 | 0.2 |
| `take_while_match` | 8.2 | 2.5 | 2.5 | 28.9 | 70.3 | 16.4 |
| `to_array_filter` | 70.3 | 12.1 | 11.8 | 71.3 | 155.8 | 28.7 |
| `to_table` | — | 18.7 | 141.3 | 117.1 | 140.6 | 32.3 |
| `to_table_staged` | — | 56.0 | 57.6 | 143.0 | 165.4 | 69.0 |
| `where_join_count` | 41.8 | 29.6 | 41.3 | 132.8 | 162.5 | 46.9 |
| `zip_count_pred` | 39.9 | 15.8 | — | 315.5 | 319.1 | — |
| `zip_dot_product` | 47.0 | 12.6 | 10.7 | 308.7 | 315.5 | — |
| `zip_dot_product_3arg` | 46.9 | 12.6 | — | 308.9 | 314.2 | — |
| `zip_reverse_to_array` | — | 32.0 | — | 344.3 | 350.5 | — |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) | Table fold (m7) |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 34.9 | 0.3 | 0.7 | 18.8 | 27.3 | 7.3 |
| `all_match` | 27.7 | 0.3 | 0.2 | 18.4 | 25.1 | 7.2 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 29.8 | 1.0 | 3.5 | 18.3 | 24.4 | 7.4 |
| `bare_last` | — | 0.4 | 0.0 | 0.0 | 0.0 | 8.9 |
| `bare_order_where` | 185.3 | 33.8 | 34.8 | 104.0 | 51.8 | 68.4 |
| `chained_select_collapse` | — | 1.1 | 1.1 | 20.5 | 32.0 | 8.2 |
| `chained_where` | 36.8 | 0.6 | 0.9 | 38.2 | 31.8 | 10.6 |
| `contains_match` | 0.0 | 0.2 | 0.1 | 14.8 | 9.2 | 2.5 |
| `count_aggregate` | 29.3 | 0.3 | 0.6 | 23.2 | 25.1 | 7.2 |
| `cross_join` | 5967.1 | 712.3 | — | 827.0 | 759.4 | — |
| `decs_count_bare_pred` | — | — | 0.6 | — | — | — |
| `distinct_by_count` | 40.9 | 1.1 | 1.1 | 20.5 | 31.9 | 8.0 |
| `distinct_by_order_take` | 250.9 | 1.7 | 2.6 | 44.6 | 36.9 | 19.4 |
| `distinct_by_order_to_array` | 253.4 | 1.7 | 2.7 | 45.1 | 36.8 | 19.6 |
| `distinct_count` | 41.5 | 1.1 | 1.1 | 20.5 | 31.9 | 8.0 |
| `distinct_count_pred` | 262.0 | 1.1 | 1.3 | 37.6 | 41.6 | 8.1 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.1 | 0.0 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 175.0 | 1.5 | 1.8 | 36.5 | 42.8 | 8.8 |
| `groupby_count` | 143.1 | 1.3 | 1.5 | 20.5 | 32.2 | 8.4 |
| `groupby_first` | 263.5 | 1.3 | 2.3 | 20.5 | 33.1 | 10.0 |
| `groupby_having_count` | 143.6 | 1.3 | 1.5 | 20.5 | 32.7 | 8.4 |
| `groupby_having_hidden_sum` | 181.0 | 1.5 | 1.9 | 36.5 | 42.6 | 8.5 |
| `groupby_having_post_where` | 175.9 | 1.4 | 1.9 | 36.5 | 41.8 | 8.4 |
| `groupby_max` | 178.1 | 1.5 | 1.9 | 36.6 | 43.5 | 8.4 |
| `groupby_min` | 181.7 | 1.5 | 1.9 | 36.7 | 43.5 | 8.4 |
| `groupby_multi_reducer` | 194.0 | 1.6 | 1.9 | 36.7 | 43.4 | 8.9 |
| `groupby_select_order` | 175.6 | 1.4 | 1.6 | 36.4 | 41.9 | 8.3 |
| `groupby_select_sum` | 204.5 | 2.8 | 3.2 | 32.8 | 37.6 | 22.6 |
| `groupby_sum` | 176.0 | 1.4 | 1.9 | 36.6 | 41.8 | 8.3 |
| `groupby_where_count` | 75.9 | 0.9 | 1.3 | 36.4 | 39.7 | 11.2 |
| `groupby_where_sum` | 87.5 | 0.9 | 1.3 | 36.5 | 39.6 | 11.1 |
| `join_count` | 38.4 | 10.9 | 12.6 | 40.6 | 68.0 | 25.1 |
| `join_groupby_count` | 160.4 | 17.4 | 19.2 | 66.2 | 85.9 | 73.1 |
| `join_groupby_to_array` | 195.5 | 18.4 | 19.6 | 78.0 | 35.9 | 80.4 |
| `join_probe` | — | — | — | — | — | 16.5 |
| `join_probe_build` | — | — | — | — | — | 31.6 |
| `join_select` | 92.8 | 19.6 | 21.6 | 72.9 | 90.9 | 70.0 |
| `join_where_count` | 39.3 | 19.1 | 20.8 | 62.5 | 78.7 | 31.5 |
| `last_match` | 0.0 | 0.5 | 1.4 | 19.0 | 25.6 | 11.9 |
| `long_count_aggregate` | 29.4 | 0.3 | 0.6 | 23.2 | 25.1 | 7.2 |
| `max_aggregate` | 30.9 | 0.3 | 0.5 | 18.9 | 26.3 | 7.4 |
| `min_aggregate` | 30.9 | 0.3 | 0.5 | 18.9 | 26.3 | 7.3 |
| `order_by_multi_key` | 246.7 | 53.3 | 54.3 | 123.6 | 70.3 | 118.8 |
| `order_distinct_take` | 140.9 | 1.1 | 75.4 | 20.8 | 33.9 | 8.0 |
| `order_reverse_normalized` | 38.6 | 0.7 | 1.4 | 22.4 | 27.0 | 9.8 |
| `order_take_desc` | 38.5 | 0.7 | 1.4 | 22.4 | 27.1 | 9.6 |
| `point_lookup` | — | — | — | — | — | 0.0 |
| `point_lookup_residual` | — | — | — | — | — | 0.0 |
| `point_lookup_scan` | — | — | — | — | — | 3.1 |
| `reverse_distinct_by` | 309.2 | 1.6 | 3.2 | 20.5 | 32.8 | 11.0 |
| `reverse_take` | 0.0 | 0.0 | 0.0 | 0.0 | 3.7 | 19.4 |
| `reverse_take_select` | 0.0 | 0.0 | 0.0 | 0.0 | 3.8 | 18.9 |
| `select_count` | 0.1 | 0.0 | 0.0 | 62.5 | 0.0 | 0.0 |
| `select_many` | — | 61.4 | — | — | — | — |
| `select_where` | 110.2 | 4.1 | 5.3 | 74.5 | 22.1 | 17.6 |
| `select_where_count` | 32.9 | 0.3 | 0.6 | 18.6 | 26.0 | 7.3 |
| `select_where_order_take` | 36.9 | 0.7 | 1.4 | 19.2 | 26.6 | 12.9 |
| `select_where_sum` | 38.7 | 0.4 | 0.6 | 18.3 | 25.0 | 7.3 |
| `single_match` | 0.0 | 0.4 | 1.2 | 43.4 | 22.2 | 8.9 |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.2 | 0.2 | 0.1 |
| `skip_while_match` | 3.4 | 0.4 | 0.4 | 43.6 | 21.9 | 7.6 |
| `sort_first` | 38.3 | 0.4 | 1.3 | 18.3 | 26.2 | 8.9 |
| `sort_take` | 38.8 | 0.7 | 1.4 | 22.3 | 27.1 | 9.3 |
| `sort_take_select` | 38.4 | 0.7 | 1.4 | 22.3 | 26.9 | 9.2 |
| `sum_aggregate` | 30.1 | 0.3 | 0.1 | 28.3 | 24.3 | 7.2 |
| `sum_where` | 33.2 | 0.3 | 0.6 | 18.6 | 25.8 | 7.2 |
| `take_count` | 1.9 | 0.1 | 0.1 | 1.2 | 0.2 | 0.1 |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 0.5 | 0.1 | 0.0 |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 0.3 | 0.0 | 0.0 |
| `take_where_count` | 0.9 | 0.0 | 0.0 | 0.2 | 0.0 | 0.0 |
| `take_while_match` | 8.1 | 0.2 | 0.3 | 15.3 | 9.1 | 7.3 |
| `to_array_filter` | 48.4 | 3.2 | 3.3 | 21.6 | 33.4 | 12.9 |
| `to_table` | — | 14.0 | 37.2 | 48.7 | 52.2 | 20.8 |
| `to_table_staged` | — | 25.6 | 26.4 | 52.2 | 61.4 | 33.1 |
| `where_join_count` | 41.7 | 5.8 | 6.7 | 47.0 | 40.5 | 19.7 |
| `zip_count_pred` | 39.6 | 0.1 | — | 113.0 | 33.5 | — |
| `zip_dot_product` | 46.9 | 0.1 | 0.1 | 112.5 | 33.3 | — |
| `zip_dot_product_3arg` | 47.2 | 0.1 | — | 112.5 | 33.3 | — |
| `zip_reverse_to_array` | — | 4.6 | — | 123.6 | 50.2 | — |
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
- **`point_lookup` / `point_lookup_residual` / `point_lookup_scan` non-m7** — m7-only trio: only a table source has a key to probe (`where(kv.key == X)` + terminator → `key_exists` / `tab?[X]`, O(1)); the `_residual` twin adds a conjunct right of the key-equality (`key == X && residual`), which probes and evaluates the residual on the probed element only; the `_scan` control puts the residual conjunct FIRST, so the key-equality is not the leftmost conjunct and the probe matcher correctly declines to the walk. Other sources have no analog by design.
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
