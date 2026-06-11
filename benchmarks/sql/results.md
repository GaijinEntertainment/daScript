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
  group_by / reverse defer to tier-2). Under JIT, `keys`/`values` for-loop sources compile to an
  inline open-addressed slot walk (no per-element C++ iterator calls), so the m7 JIT column is
  fused codegen end to end.

`0.00` = early-exit terminator below timer resolution ("free"). Chain shapes are in
`benchmarks/README.md`; the splice arms each fires are in `doc/source/reference/linq_fold_patterns.rst`.

**JIT on Apple silicon is high-variance run-to-run — read the INTERP column as the reliable
signal, JIT deltas as indicative.**

<!-- BENCH:TABLES BEGIN -->
*Generated 2026-06-11 by `benchmarks/sql/_update_results.das` — ns/op; `—` = absent lane. Edit the prose around the markers, not the tables.*

## INTERP

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) | Table fold (m7) |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 34.9 | 5.9 | 5.9 | 60.5 | 158.9 | 19.8 |
| `all_match` | 27.8 | 3.5 | 3.5 | 56.5 | 156.6 | 15.8 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.2 | 6.1 | 8.7 | 58.8 | 157.2 | 17.2 |
| `bare_last` | — | 4.2 | 0.0 | 0.0 | 4.1 | 30.2 |
| `bare_order_where` | 279.9 | 117.8 | 125.7 | 302.3 | 292.5 | 163.9 |
| `chained_select_collapse` | — | 17.6 | 17.4 | 70.6 | 154.1 | 28.4 |
| `chained_where` | 36.5 | 6.6 | 7.1 | 105.8 | 177.6 | 23.9 |
| `contains_match` | 0.0 | 2.2 | 1.4 | 27.8 | 70.3 | 6.5 |
| `count_aggregate` | 29.7 | 4.2 | 4.1 | 64.1 | 158.1 | 20.3 |
| `cross_join` | 12594.5 | 3704.9 | — | 4030.6 | 4063.0 | — |
| `decs_count_bare_pred` | — | — | 4.1 | — | — | — |
| `distinct_by_count` | 41.6 | 15.8 | 15.7 | 70.7 | 156.5 | 27.3 |
| `distinct_by_order_take` | 240.5 | 22.2 | 23.4 | 124.6 | 159.8 | 49.5 |
| `distinct_by_order_to_array` | 240.7 | 22.1 | 23.4 | 125.1 | 163.6 | 49.2 |
| `distinct_count` | 41.2 | 15.6 | 15.6 | 70.6 | 161.2 | 27.5 |
| `distinct_count_pred` | 253.4 | 15.9 | 15.9 | 112.6 | 173.8 | 27.4 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.4 | 0.3 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 171.3 | 29.1 | 29.4 | 124.3 | 187.5 | 197.3 |
| `groupby_count` | 143.5 | 19.1 | 19.1 | 74.8 | 159.1 | 163.1 |
| `groupby_first` | 251.7 | 19.0 | 19.8 | 72.4 | 155.1 | 163.1 |
| `groupby_having_count` | 142.2 | 19.1 | 19.1 | 75.2 | 160.2 | 185.4 |
| `groupby_having_hidden_sum` | 175.9 | 22.2 | 22.3 | 119.0 | 183.5 | 215.6 |
| `groupby_having_post_where` | 172.8 | 20.4 | 20.5 | 115.3 | 181.3 | 194.2 |
| `groupby_max` | 173.1 | 24.9 | 24.8 | 120.5 | 184.1 | 201.8 |
| `groupby_min` | 173.6 | 25.6 | 25.2 | 120.7 | 184.3 | 204.0 |
| `groupby_multi_reducer` | 190.4 | 30.4 | 30.3 | 126.3 | 188.2 | 231.5 |
| `groupby_select_order` | 172.9 | 20.5 | 20.4 | 118.6 | 179.8 | 194.6 |
| `groupby_select_sum` | 198.2 | 38.5 | 38.8 | 102.1 | 185.0 | 188.0 |
| `groupby_sum` | 171.1 | 20.4 | 20.4 | 115.0 | 179.5 | 194.2 |
| `groupby_where_count` | 76.8 | 13.9 | 14.5 | 115.3 | 188.6 | 164.2 |
| `groupby_where_sum` | 87.4 | 14.2 | 14.8 | 116.7 | 187.1 | 179.4 |
| `join_count` | 38.6 | 51.8 | 63.9 | 112.3 | 185.6 | 64.2 |
| `join_groupby_count` | 158.4 | 76.9 | 88.5 | 178.1 | 225.8 | 259.3 |
| `join_groupby_to_array` | 190.4 | 78.3 | 90.6 | 215.6 | 212.2 | 290.2 |
| `join_probe` | — | — | — | — | — | 46.6 |
| `join_probe_build` | — | — | — | — | — | 79.5 |
| `join_select` | 150.9 | 73.5 | 84.6 | 187.9 | 207.1 | 223.5 |
| `join_where_count` | 39.8 | 61.9 | 75.8 | 161.2 | 192.9 | 79.8 |
| `last_match` | 0.0 | 5.8 | 13.9 | 65.3 | 157.9 | 30.9 |
| `long_count_aggregate` | 29.7 | 4.2 | 4.1 | 63.7 | 158.0 | 20.1 |
| `max_aggregate` | 31.0 | 6.1 | 6.8 | 58.8 | 157.6 | 17.0 |
| `min_aggregate` | 30.9 | 6.1 | 6.8 | 59.0 | 159.3 | 17.0 |
| `order_by_multi_key` | 338.1 | 274.4 | 282.8 | 459.3 | 445.2 | 341.9 |
| `order_distinct_take` | 138.5 | 15.7 | 98.9 | 72.8 | 155.0 | 31.7 |
| `order_reverse_normalized` | 38.4 | 16.2 | 19.9 | 70.5 | 162.1 | 33.1 |
| `order_take_desc` | 38.3 | 16.4 | 19.9 | 70.6 | 162.5 | 33.0 |
| `point_lookup` | — | — | — | — | — | 0.0 |
| `point_lookup_scan` | — | — | — | — | — | 8.3 |
| `reverse_distinct_by` | 296.9 | 21.1 | 27.7 | 71.7 | 154.5 | 44.4 |
| `reverse_take` | 0.1 | 0.0 | 0.2 | 0.0 | 26.2 | 58.5 |
| `reverse_take_select` | 0.0 | 0.0 | 0.2 | 0.0 | 26.2 | 58.7 |
| `select_count` | 0.1 | 0.0 | 2.2 | 63.4 | 2.2 | 0.0 |
| `select_many` | — | 189.8 | — | — | — | — |
| `select_where` | 199.2 | 11.2 | 19.2 | 197.4 | 186.5 | 37.8 |
| `select_where_count` | 33.0 | 5.2 | 7.5 | 65.2 | 150.0 | 23.2 |
| `select_where_order_take` | 37.0 | 12.2 | 14.9 | 72.5 | 163.1 | 34.7 |
| `select_where_sum` | 37.1 | 7.5 | 7.5 | 66.2 | 158.2 | 24.2 |
| `single_match` | 0.0 | 2.9 | 5.4 | 56.2 | 148.2 | 22.8 |
| `skip_take` | 0.5 | 0.1 | 0.2 | 3.1 | 2.8 | 0.3 |
| `skip_while_match` | 3.5 | 5.3 | 5.3 | 57.9 | 150.2 | 18.2 |
| `sort_first` | 38.4 | 11.0 | 13.4 | 65.7 | 162.3 | 31.7 |
| `sort_take` | 38.6 | 16.1 | 20.3 | 70.7 | 163.1 | 33.2 |
| `sort_take_select` | 38.6 | 16.4 | 20.2 | 70.9 | 161.9 | 33.2 |
| `sum_aggregate` | 30.0 | 2.1 | 2.1 | 54.8 | 156.9 | 13.5 |
| `sum_where` | 33.0 | 4.3 | 4.3 | 63.7 | 157.9 | 20.7 |
| `take_count` | 3.7 | 0.2 | 0.4 | 2.9 | 2.7 | 0.5 |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 1.4 | 1.1 | 0.3 |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 0.6 | 0.5 | 0.1 |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 0.7 | 0.6 | 0.2 |
| `take_while_match` | 7.8 | 2.4 | 2.5 | 29.0 | 72.6 | 16.9 |
| `to_array_filter` | 70.7 | 11.7 | 11.8 | 71.7 | 163.7 | 28.9 |
| `to_table` | — | 18.6 | 141.9 | 118.5 | 140.3 | 32.1 |
| `to_table_staged` | — | 54.7 | 56.6 | 143.3 | 165.1 | 68.5 |
| `where_join_count` | 41.6 | 29.4 | 41.0 | 132.1 | 171.8 | 46.7 |
| `zip_count_pred` | 39.4 | 15.8 | — | 316.5 | 317.6 | — |
| `zip_dot_product` | 49.8 | 12.6 | 10.6 | 312.4 | 313.7 | — |
| `zip_dot_product_3arg` | 50.2 | 12.7 | — | 312.5 | 313.9 | — |
| `zip_reverse_to_array` | — | 32.1 | — | 347.2 | 351.8 | — |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) | Table fold (m7) |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 35.0 | 0.3 | 0.7 | 29.7 | 27.3 | 7.3 |
| `all_match` | 27.7 | 0.3 | 0.2 | 18.8 | 25.3 | 7.2 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.3 | 1.0 | 3.6 | 18.8 | 24.4 | 7.4 |
| `bare_last` | — | 0.4 | 0.0 | 0.0 | 0.0 | 8.9 |
| `bare_order_where` | 185.9 | 33.8 | 35.0 | 106.0 | 51.7 | 68.2 |
| `chained_select_collapse` | — | 1.1 | 1.1 | 20.6 | 32.1 | 8.1 |
| `chained_where` | 36.7 | 0.6 | 0.9 | 36.4 | 31.8 | 10.4 |
| `contains_match` | 0.0 | 0.2 | 0.1 | 16.8 | 9.2 | 2.5 |
| `count_aggregate` | 29.4 | 0.3 | 0.6 | 29.5 | 25.1 | 7.3 |
| `cross_join` | 5962.9 | 719.2 | — | 833.9 | 771.0 | — |
| `decs_count_bare_pred` | — | — | 0.6 | — | — | — |
| `distinct_by_count` | 41.6 | 1.1 | 1.1 | 20.6 | 31.9 | 8.0 |
| `distinct_by_order_take` | 238.9 | 1.7 | 2.6 | 45.3 | 37.2 | 19.6 |
| `distinct_by_order_to_array` | 239.5 | 1.7 | 2.7 | 45.5 | 37.0 | 19.5 |
| `distinct_count` | 41.4 | 1.1 | 1.1 | 20.7 | 33.1 | 8.0 |
| `distinct_count_pred` | 252.1 | 1.1 | 1.3 | 37.7 | 43.6 | 8.0 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.1 | 0.0 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 171.5 | 1.5 | 1.8 | 37.2 | 44.8 | 51.8 |
| `groupby_count` | 162.3 | 1.3 | 1.5 | 20.7 | 32.2 | 43.5 |
| `groupby_first` | 251.8 | 1.3 | 2.3 | 20.7 | 34.0 | 43.4 |
| `groupby_having_count` | 142.7 | 1.3 | 1.5 | 20.6 | 33.4 | 46.2 |
| `groupby_having_hidden_sum` | 175.1 | 1.5 | 1.9 | 37.0 | 43.0 | 54.5 |
| `groupby_having_post_where` | 171.4 | 1.4 | 1.9 | 37.0 | 42.0 | 51.4 |
| `groupby_max` | 173.0 | 1.5 | 1.9 | 37.1 | 43.6 | 51.9 |
| `groupby_min` | 172.4 | 1.5 | 1.9 | 38.2 | 43.4 | 52.6 |
| `groupby_multi_reducer` | 193.2 | 1.6 | 1.9 | 37.2 | 43.7 | 60.8 |
| `groupby_select_order` | 170.5 | 1.4 | 1.6 | 37.9 | 41.9 | 51.8 |
| `groupby_select_sum` | 196.9 | 2.8 | 3.2 | 33.5 | 37.7 | 73.3 |
| `groupby_sum` | 171.5 | 1.4 | 1.9 | 37.9 | 42.0 | 52.1 |
| `groupby_where_count` | 76.5 | 0.9 | 1.3 | 37.2 | 39.7 | 53.7 |
| `groupby_where_sum` | 87.4 | 0.9 | 1.3 | 37.1 | 39.7 | 57.7 |
| `join_count` | 38.3 | 11.2 | 12.5 | 40.8 | 68.0 | 25.2 |
| `join_groupby_count` | 157.5 | 17.2 | 19.3 | 66.4 | 86.0 | 73.1 |
| `join_groupby_to_array` | 190.7 | 17.8 | 19.7 | 78.6 | 35.8 | 81.4 |
| `join_probe` | — | — | — | — | — | 16.7 |
| `join_probe_build` | — | — | — | — | — | 33.2 |
| `join_select` | 91.8 | 19.6 | 21.7 | 73.5 | 89.8 | 70.1 |
| `join_where_count` | 39.2 | 19.2 | 20.6 | 63.3 | 77.3 | 31.7 |
| `last_match` | 0.0 | 0.5 | 1.4 | 19.6 | 25.1 | 12.1 |
| `long_count_aggregate` | 30.0 | 0.3 | 0.6 | 29.4 | 25.1 | 7.3 |
| `max_aggregate` | 31.0 | 0.3 | 0.5 | 29.7 | 26.3 | 7.5 |
| `min_aggregate` | 31.0 | 0.3 | 0.5 | 29.7 | 26.2 | 7.4 |
| `order_by_multi_key` | 242.6 | 53.3 | 54.4 | 124.6 | 70.5 | 119.3 |
| `order_distinct_take` | 138.6 | 1.1 | 75.8 | 20.9 | 34.1 | 8.1 |
| `order_reverse_normalized` | 38.5 | 0.7 | 1.3 | 19.8 | 27.0 | 11.1 |
| `order_take_desc` | 38.8 | 0.7 | 1.3 | 19.8 | 26.9 | 10.0 |
| `point_lookup` | — | — | — | — | — | 0.0 |
| `point_lookup_scan` | — | — | — | — | — | 3.0 |
| `reverse_distinct_by` | 296.3 | 1.6 | 3.2 | 20.6 | 32.6 | 10.9 |
| `reverse_take` | 0.0 | 0.0 | 0.0 | 0.0 | 3.8 | 19.3 |
| `reverse_take_select` | 0.0 | 0.0 | 0.0 | 0.0 | 3.8 | 19.2 |
| `select_count` | 0.1 | 0.0 | 0.0 | 67.0 | 0.0 | 0.0 |
| `select_many` | — | 61.4 | — | — | — | — |
| `select_where` | 107.8 | 4.1 | 5.3 | 76.0 | 22.2 | 17.9 |
| `select_where_count` | 32.6 | 0.3 | 0.6 | 29.5 | 25.9 | 7.4 |
| `select_where_order_take` | 36.8 | 0.7 | 1.4 | 19.8 | 26.6 | 13.0 |
| `select_where_sum` | 37.4 | 0.4 | 0.6 | 20.4 | 24.8 | 7.5 |
| `single_match` | 0.0 | 0.4 | 1.1 | 45.9 | 22.2 | 9.7 |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.3 | 0.2 | 0.1 |
| `skip_while_match` | 3.5 | 0.4 | 0.4 | 46.1 | 21.7 | 7.8 |
| `sort_first` | 38.1 | 0.4 | 1.3 | 18.8 | 26.1 | 9.3 |
| `sort_take` | 38.6 | 0.7 | 1.3 | 19.8 | 27.0 | 9.7 |
| `sort_take_select` | 38.6 | 0.7 | 1.3 | 19.8 | 26.9 | 9.6 |
| `sum_aggregate` | 30.2 | 0.3 | 0.0 | 22.5 | 24.2 | 7.3 |
| `sum_where` | 32.9 | 0.3 | 0.6 | 29.6 | 25.8 | 7.3 |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.2 | 0.2 | 0.1 |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 0.5 | 0.1 | 0.0 |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 0.2 | 0.0 | 0.0 |
| `take_where_count` | 0.9 | 0.0 | 0.0 | 0.3 | 0.0 | 0.0 |
| `take_while_match` | 7.8 | 0.2 | 0.3 | 16.9 | 9.0 | 7.3 |
| `to_array_filter` | 48.4 | 3.3 | 3.3 | 22.2 | 33.7 | 13.2 |
| `to_table` | — | 14.1 | 37.1 | 49.6 | 52.2 | 20.6 |
| `to_table_staged` | — | 25.8 | 26.2 | 53.2 | 61.3 | 33.6 |
| `where_join_count` | 41.6 | 6.0 | 6.7 | 48.0 | 40.6 | 19.9 |
| `zip_count_pred` | 39.3 | 0.1 | — | 114.0 | 33.7 | — |
| `zip_dot_product` | 46.9 | 0.1 | 0.1 | 113.6 | 33.5 | — |
| `zip_dot_product_3arg` | 46.6 | 0.1 | — | 113.9 | 33.7 | — |
| `zip_reverse_to_array` | — | 4.5 | — | 125.3 | 50.7 | — |
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
- **m7 absent families** — `zip_*` / `cross_join` (lockstep pairing over an unordered slot walk is meaningless) and `select_many` (flat fixture, no nested array field; array-only). Everything else in the m7 column is instantiated — but read the `groupby_*` / `join_groupby_*` / reverse-family cells as the **tier-2 cascade cost**, not a fused emit: table group_by fusion and a backward slot walk are named deferred edges (see `LINQ_TO_TABLE.md`), so those cells are the numbers a fix would improve.
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
