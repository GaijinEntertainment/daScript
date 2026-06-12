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
| `aggregate_match` | 35.0 | 5.9 | 5.8 | 60.8 | 159.8 | 19.1 |
| `all_match` | 27.8 | 3.5 | 3.4 | 56.4 | 155.6 | 15.9 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.6 | 6.1 | 8.7 | 58.7 | 164.0 | 17.3 |
| `bare_last` | — | 4.2 | 0.0 | 0.0 | 4.2 | 31.0 |
| `bare_order_where` | 280.6 | 116.8 | 125.5 | 300.5 | 289.3 | 162.6 |
| `chained_select_collapse` | — | 17.6 | 17.5 | 70.5 | 164.8 | 28.0 |
| `chained_where` | 36.7 | 6.6 | 7.1 | 105.6 | 183.3 | 24.0 |
| `contains_match` | 0.0 | 2.2 | 1.4 | 29.2 | 73.0 | 6.6 |
| `count_aggregate` | 29.8 | 4.2 | 4.1 | 63.9 | 154.3 | 20.2 |
| `cross_join` | 12641.5 | 3703.0 | — | 4040.3 | 4032.0 | — |
| `decs_count_bare_pred` | — | — | 4.2 | — | — | — |
| `distinct_by_count` | 41.1 | 15.8 | 15.8 | 71.2 | 162.6 | 26.5 |
| `distinct_by_order_take` | 245.5 | 22.2 | 23.6 | 123.7 | 162.2 | 48.7 |
| `distinct_by_order_to_array` | 247.9 | 22.1 | 23.5 | 125.1 | 163.8 | 48.6 |
| `distinct_count` | 41.5 | 15.6 | 15.8 | 71.2 | 161.9 | 27.1 |
| `distinct_count_pred` | 256.2 | 15.8 | 15.8 | 112.8 | 177.5 | 26.3 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.4 | 0.3 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 171.1 | 29.2 | 29.2 | 123.8 | 217.9 | 40.9 |
| `groupby_count` | 141.4 | 19.1 | 19.2 | 74.8 | 171.7 | 30.8 |
| `groupby_first` | 257.2 | 19.1 | 19.8 | 72.5 | 162.9 | 40.2 |
| `groupby_having_count` | 141.6 | 19.2 | 19.2 | 75.5 | 168.3 | 30.6 |
| `groupby_having_hidden_sum` | 176.8 | 22.2 | 22.6 | 118.4 | 192.2 | 33.5 |
| `groupby_having_post_where` | 172.0 | 20.4 | 20.5 | 114.7 | 188.7 | 31.7 |
| `groupby_max` | 174.6 | 24.8 | 24.9 | 119.4 | 193.4 | 34.2 |
| `groupby_min` | 174.1 | 25.5 | 25.3 | 120.0 | 191.8 | 34.3 |
| `groupby_multi_reducer` | 190.7 | 30.6 | 30.1 | 125.0 | 195.9 | 43.2 |
| `groupby_select_order` | 171.5 | 20.4 | 20.5 | 114.8 | 188.2 | 31.6 |
| `groupby_select_sum` | 198.6 | 38.5 | 38.6 | 101.9 | 193.6 | 49.4 |
| `groupby_sum` | 170.7 | 20.5 | 20.5 | 114.7 | 187.7 | 31.5 |
| `groupby_where_count` | 76.0 | 13.8 | 14.5 | 116.5 | 185.7 | 30.2 |
| `groupby_where_sum` | 87.1 | 14.2 | 14.8 | 117.5 | 186.2 | 30.6 |
| `join_count` | 38.6 | 52.3 | 64.4 | 112.9 | 183.8 | 65.0 |
| `join_groupby_count` | 158.9 | 77.1 | 89.0 | 178.8 | 230.9 | 260.6 |
| `join_groupby_to_array` | 191.1 | 78.5 | 90.7 | 215.8 | 212.8 | 290.5 |
| `join_probe` | — | — | — | — | — | 46.8 |
| `join_probe_build` | — | — | — | — | — | 80.8 |
| `join_select` | 150.7 | 73.7 | 84.2 | 194.2 | 214.5 | 223.2 |
| `join_where_count` | 39.6 | 62.0 | 75.7 | 161.1 | 198.9 | 80.2 |
| `last_match` | 0.0 | 5.7 | 14.0 | 65.4 | 160.2 | 31.4 |
| `long_count_aggregate` | 29.7 | 4.1 | 4.1 | 63.8 | 154.5 | 20.1 |
| `max_aggregate` | 31.0 | 6.1 | 6.9 | 59.0 | 163.6 | 16.9 |
| `min_aggregate` | 30.9 | 6.1 | 6.8 | 59.3 | 163.2 | 16.9 |
| `order_by_multi_key` | 341.9 | 274.9 | 283.2 | 459.1 | 445.7 | 333.6 |
| `order_distinct_take` | 138.9 | 15.9 | 100.1 | 73.0 | 163.1 | 31.0 |
| `order_reverse_normalized` | 38.5 | 16.3 | 20.0 | 70.7 | 170.8 | 32.9 |
| `order_take_desc` | 38.6 | 16.4 | 20.0 | 70.8 | 170.4 | 33.0 |
| `point_lookup` | — | — | — | — | — | 0.0 |
| `point_lookup_residual` | — | — | — | — | — | 0.0 |
| `point_lookup_scan` | — | — | — | — | — | 8.5 |
| `reverse_distinct_by` | 299.0 | 21.2 | 27.7 | 71.2 | 162.4 | 44.2 |
| `reverse_take` | 0.1 | 0.0 | 0.2 | 0.0 | 26.3 | 58.7 |
| `reverse_take_select` | 0.0 | 0.0 | 0.2 | 0.0 | 26.2 | 58.6 |
| `select_count` | 0.1 | 0.0 | 2.2 | 69.6 | 2.2 | 0.0 |
| `select_many` | — | 191.6 | — | — | — | — |
| `select_where` | 197.3 | 11.0 | 19.3 | 196.5 | 183.9 | 37.5 |
| `select_where_count` | 32.9 | 5.2 | 7.4 | 64.7 | 158.0 | 22.5 |
| `select_where_order_take` | 37.2 | 12.2 | 15.0 | 73.1 | 164.7 | 34.5 |
| `select_where_sum` | 37.4 | 7.5 | 7.5 | 66.7 | 162.3 | 23.3 |
| `single_match` | 0.0 | 2.8 | 5.4 | 58.7 | 150.6 | 22.8 |
| `skip_take` | 0.5 | 0.1 | 0.2 | 3.0 | 2.8 | 0.3 |
| `skip_while_match` | 3.5 | 5.2 | 5.3 | 60.3 | 153.6 | 18.2 |
| `sort_first` | 38.7 | 11.0 | 13.5 | 65.6 | 167.0 | 31.5 |
| `sort_take` | 38.7 | 16.1 | 20.2 | 71.1 | 170.6 | 32.7 |
| `sort_take_select` | 38.5 | 16.4 | 20.2 | 71.3 | 171.4 | 33.1 |
| `sum_aggregate` | 30.5 | 2.1 | 2.1 | 54.7 | 152.7 | 13.4 |
| `sum_where` | 33.1 | 4.3 | 4.3 | 63.7 | 154.0 | 21.0 |
| `take_count` | 3.6 | 0.2 | 0.4 | 2.9 | 2.7 | 0.5 |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 1.3 | 1.1 | 0.3 |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 0.6 | 0.5 | 0.1 |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 0.7 | 0.6 | 0.2 |
| `take_while_match` | 8.2 | 2.4 | 2.5 | 30.4 | 75.6 | 16.4 |
| `to_array_filter` | 70.9 | 11.7 | 11.8 | 71.6 | 164.5 | 29.2 |
| `to_table` | — | 18.6 | 143.9 | 117.9 | 143.7 | 32.5 |
| `to_table_staged` | — | 55.7 | 57.7 | 143.7 | 167.5 | 69.7 |
| `where_join_count` | 41.8 | 29.5 | 41.2 | 132.6 | 167.6 | 47.9 |
| `zip_count_pred` | 39.5 | 15.8 | — | 315.7 | 319.9 | — |
| `zip_dot_product` | 49.2 | 12.7 | 10.5 | 310.1 | 316.0 | — |
| `zip_dot_product_3arg` | 48.7 | 12.8 | — | 310.3 | 316.5 | — |
| `zip_reverse_to_array` | — | 31.9 | — | 344.9 | 350.7 | — |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) | Table fold (m7) |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 34.9 | 0.3 | 0.7 | 18.8 | 26.4 | 7.3 |
| `all_match` | 27.6 | 0.3 | 0.2 | 18.4 | 25.4 | 7.2 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.2 | 1.0 | 3.6 | 18.4 | 24.8 | 7.4 |
| `bare_last` | — | 0.4 | 0.0 | 0.0 | 0.0 | 8.8 |
| `bare_order_where` | 187.2 | 34.2 | 35.7 | 104.2 | 53.2 | 68.7 |
| `chained_select_collapse` | — | 1.1 | 1.1 | 20.6 | 33.7 | 8.3 |
| `chained_where` | 36.9 | 0.6 | 0.9 | 38.3 | 31.6 | 10.6 |
| `contains_match` | 0.0 | 0.2 | 0.1 | 17.5 | 8.9 | 2.5 |
| `count_aggregate` | 29.6 | 0.3 | 0.6 | 23.4 | 25.6 | 7.3 |
| `cross_join` | 5998.8 | 737.5 | — | 830.4 | 766.4 | — |
| `decs_count_bare_pred` | — | — | 0.6 | — | — | — |
| `distinct_by_count` | 41.3 | 1.1 | 1.1 | 20.6 | 33.8 | 8.1 |
| `distinct_by_order_take` | 241.8 | 1.7 | 2.6 | 44.7 | 38.8 | 19.4 |
| `distinct_by_order_to_array` | 241.0 | 1.7 | 2.7 | 45.2 | 38.6 | 19.7 |
| `distinct_count` | 41.6 | 1.1 | 1.1 | 20.7 | 33.6 | 8.1 |
| `distinct_count_pred` | 252.6 | 1.1 | 1.3 | 37.7 | 43.5 | 8.0 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.1 | 0.0 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 171.5 | 1.6 | 1.8 | 36.8 | 45.6 | 8.9 |
| `groupby_count` | 141.1 | 1.3 | 1.5 | 20.7 | 34.0 | 8.5 |
| `groupby_first` | 254.1 | 1.3 | 2.3 | 20.6 | 34.7 | 10.0 |
| `groupby_having_count` | 141.5 | 1.3 | 1.5 | 20.7 | 34.1 | 8.5 |
| `groupby_having_hidden_sum` | 175.3 | 1.5 | 1.9 | 36.6 | 45.2 | 8.6 |
| `groupby_having_post_where` | 171.9 | 1.4 | 1.9 | 36.6 | 44.3 | 8.5 |
| `groupby_max` | 174.1 | 1.5 | 1.9 | 36.8 | 45.6 | 8.5 |
| `groupby_min` | 175.0 | 1.5 | 2.0 | 36.8 | 46.2 | 8.5 |
| `groupby_multi_reducer` | 190.4 | 1.6 | 1.9 | 36.7 | 46.1 | 9.1 |
| `groupby_select_order` | 171.6 | 1.4 | 1.6 | 36.7 | 44.3 | 8.5 |
| `groupby_select_sum` | 199.2 | 2.8 | 3.2 | 33.0 | 39.7 | 23.0 |
| `groupby_sum` | 172.7 | 1.4 | 1.9 | 36.7 | 44.3 | 8.4 |
| `groupby_where_count` | 76.4 | 0.9 | 1.3 | 36.5 | 42.0 | 11.3 |
| `groupby_where_sum` | 87.5 | 0.9 | 1.3 | 36.6 | 42.0 | 11.2 |
| `join_count` | 38.3 | 10.8 | 12.8 | 40.9 | 70.9 | 25.1 |
| `join_groupby_count` | 157.8 | 17.3 | 19.5 | 66.5 | 90.0 | 73.5 |
| `join_groupby_to_array` | 191.5 | 18.4 | 19.8 | 78.1 | 36.2 | 80.8 |
| `join_probe` | — | — | — | — | — | 16.6 |
| `join_probe_build` | — | — | — | — | — | 31.5 |
| `join_select` | 92.8 | 19.6 | 21.7 | 73.1 | 95.2 | 69.5 |
| `join_where_count` | 48.8 | 19.1 | 20.9 | 62.5 | 76.7 | 31.5 |
| `last_match` | 0.0 | 0.5 | 1.4 | 19.3 | 26.0 | 12.0 |
| `long_count_aggregate` | 29.4 | 0.3 | 0.6 | 23.4 | 25.8 | 7.3 |
| `max_aggregate` | 30.9 | 0.3 | 0.5 | 19.1 | 27.1 | 7.5 |
| `min_aggregate` | 31.0 | 0.3 | 0.5 | 19.1 | 27.1 | 7.4 |
| `order_by_multi_key` | 248.9 | 53.1 | 54.3 | 123.5 | 71.6 | 119.2 |
| `order_distinct_take` | 138.5 | 1.1 | 75.3 | 21.0 | 36.0 | 8.1 |
| `order_reverse_normalized` | 38.3 | 0.7 | 1.4 | 22.4 | 27.8 | 9.8 |
| `order_take_desc` | 38.6 | 0.7 | 1.4 | 22.5 | 27.7 | 9.7 |
| `point_lookup` | — | — | — | — | — | 0.0 |
| `point_lookup_residual` | — | — | — | — | — | 0.0 |
| `point_lookup_scan` | — | — | — | — | — | 3.1 |
| `reverse_distinct_by` | 297.8 | 1.6 | 3.2 | 20.7 | 34.6 | 11.0 |
| `reverse_take` | 0.0 | 0.0 | 0.0 | 0.0 | 3.7 | 19.5 |
| `reverse_take_select` | 0.0 | 0.0 | 0.0 | 0.0 | 3.8 | 19.0 |
| `select_count` | 0.1 | 0.0 | 0.0 | 62.0 | 0.0 | 0.0 |
| `select_many` | — | 62.3 | — | — | — | — |
| `select_where` | 107.2 | 4.1 | 5.3 | 74.5 | 22.2 | 17.6 |
| `select_where_count` | 33.0 | 0.3 | 0.6 | 18.8 | 26.6 | 7.3 |
| `select_where_order_take` | 37.0 | 0.7 | 1.4 | 19.4 | 27.2 | 13.0 |
| `select_where_sum` | 37.1 | 0.4 | 0.6 | 18.5 | 25.5 | 7.5 |
| `single_match` | 0.0 | 0.4 | 1.1 | 46.8 | 22.3 | 9.0 |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.2 | 0.2 | 0.1 |
| `skip_while_match` | 3.4 | 0.4 | 0.4 | 46.1 | 21.9 | 7.7 |
| `sort_first` | 38.5 | 0.4 | 1.3 | 18.5 | 26.8 | 9.0 |
| `sort_take` | 38.7 | 0.7 | 1.4 | 22.5 | 27.9 | 9.4 |
| `sort_take_select` | 38.6 | 0.7 | 1.4 | 22.4 | 27.9 | 9.3 |
| `sum_aggregate` | 30.1 | 0.3 | 0.1 | 28.4 | 24.6 | 7.3 |
| `sum_where` | 32.7 | 0.3 | 0.6 | 18.9 | 26.4 | 7.3 |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.2 | 0.2 | 0.1 |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 0.5 | 0.1 | 0.0 |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 0.3 | 0.0 | 0.0 |
| `take_where_count` | 0.9 | 0.0 | 0.0 | 0.2 | 0.0 | 0.0 |
| `take_while_match` | 8.2 | 0.2 | 0.3 | 17.6 | 8.9 | 7.3 |
| `to_array_filter` | 48.9 | 3.3 | 3.4 | 21.8 | 35.3 | 13.0 |
| `to_table` | — | 14.0 | 37.2 | 49.0 | 54.1 | 20.9 |
| `to_table_staged` | — | 25.8 | 26.2 | 52.4 | 64.1 | 33.4 |
| `where_join_count` | 41.6 | 5.9 | 6.8 | 47.2 | 42.6 | 19.8 |
| `zip_count_pred` | 39.5 | 0.1 | — | 113.3 | 33.8 | — |
| `zip_dot_product` | 49.3 | 0.1 | 0.1 | 113.0 | 33.8 | — |
| `zip_dot_product_3arg` | 49.1 | 0.1 | — | 113.1 | 33.9 | — |
| `zip_reverse_to_array` | — | 4.6 | — | 126.3 | 51.9 | — |
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
