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
  group_by / reverse defer to tier-2).

`0.00` = early-exit terminator below timer resolution ("free"). Chain shapes are in
`benchmarks/README.md`; the splice arms each fires are in `doc/source/reference/linq_fold_patterns.rst`.

**JIT on Apple silicon is high-variance run-to-run — read the INTERP column as the reliable
signal, JIT deltas as indicative.**

<!-- BENCH:TABLES BEGIN -->
*Generated 2026-06-11 by `benchmarks/sql/_update_results.das` — ns/op; `—` = absent lane. Edit the prose around the markers, not the tables.*

## INTERP

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) | Table fold (m7) |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 35.0 | 5.9 | 5.9 | 60.5 | 159.7 | 19.0 |
| `all_match` | 27.7 | 3.5 | 3.4 | 56.1 | 153.8 | 15.8 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.1 | 6.0 | 8.8 | 60.1 | 163.7 | 17.2 |
| `bare_last` | — | 4.2 | 0.0 | 0.0 | 4.2 | 30.1 |
| `bare_order_where` | 278.1 | 117.1 | 126.5 | 302.8 | 288.8 | 163.0 |
| `chained_select_collapse` | — | 17.9 | 17.6 | 70.7 | 172.6 | 27.9 |
| `chained_where` | 36.9 | 6.6 | 7.1 | 105.4 | 183.4 | 23.8 |
| `contains_match` | 0.0 | 2.2 | 1.4 | 29.0 | 72.4 | 6.5 |
| `count_aggregate` | 29.7 | 4.2 | 4.1 | 63.6 | 154.3 | 20.1 |
| `cross_join` | 12597.0 | 3721.0 | — | 4040.3 | 4113.3 | — |
| `decs_count_bare_pred` | — | — | 4.2 | — | — | — |
| `distinct_by_count` | 41.6 | 16.4 | 15.8 | 70.8 | 162.9 | 26.9 |
| `distinct_by_order_take` | 241.1 | 22.1 | 23.7 | 124.2 | 162.4 | 49.2 |
| `distinct_by_order_to_array` | 241.0 | 22.2 | 23.8 | 125.0 | 163.2 | 48.9 |
| `distinct_count` | 41.8 | 15.7 | 15.9 | 70.7 | 162.9 | 27.1 |
| `distinct_count_pred` | 253.4 | 15.9 | 15.9 | 112.7 | 179.4 | 26.7 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.4 | 0.3 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 173.6 | 29.2 | 29.3 | 123.6 | 195.4 | 198.4 |
| `groupby_count` | 144.5 | 19.2 | 19.2 | 75.0 | 168.4 | 164.3 |
| `groupby_first` | 253.9 | 19.1 | 19.8 | 72.7 | 163.4 | 164.1 |
| `groupby_having_count` | 142.6 | 19.2 | 19.2 | 75.4 | 168.9 | 186.7 |
| `groupby_having_hidden_sum` | 176.8 | 22.2 | 22.9 | 118.6 | 192.0 | 216.5 |
| `groupby_having_post_where` | 172.4 | 20.5 | 20.5 | 114.6 | 188.7 | 194.8 |
| `groupby_max` | 175.4 | 24.9 | 25.2 | 120.0 | 192.4 | 202.4 |
| `groupby_min` | 175.2 | 24.9 | 25.3 | 120.7 | 193.2 | 204.5 |
| `groupby_multi_reducer` | 192.0 | 30.8 | 30.2 | 125.6 | 196.8 | 232.3 |
| `groupby_select_order` | 172.8 | 20.5 | 20.5 | 115.2 | 188.1 | 195.3 |
| `groupby_select_sum` | 199.8 | 38.7 | 38.7 | 102.3 | 193.5 | 191.2 |
| `groupby_sum` | 176.8 | 20.5 | 20.5 | 114.9 | 188.1 | 194.5 |
| `groupby_where_count` | 76.2 | 13.8 | 14.5 | 116.0 | 185.7 | 165.2 |
| `groupby_where_sum` | 87.7 | 14.1 | 14.9 | 116.5 | 187.3 | 180.6 |
| `join_count` | 38.0 | 51.3 | 64.7 | 113.3 | 183.5 | 66.0 |
| `join_groupby_count` | 160.1 | 76.7 | 89.9 | 178.6 | 230.9 | 259.6 |
| `join_groupby_to_array` | 194.1 | 78.4 | 91.4 | 216.3 | 212.7 | 290.0 |
| `join_probe` | — | — | — | — | — | 46.6 |
| `join_probe_build` | — | — | — | — | — | 79.9 |
| `join_select` | 150.3 | 72.7 | 86.0 | 190.7 | 215.4 | 222.9 |
| `join_where_count` | 39.1 | 61.6 | 79.4 | 161.2 | 198.1 | 80.1 |
| `last_match` | 0.0 | 5.9 | 13.9 | 64.8 | 159.6 | 31.0 |
| `long_count_aggregate` | 29.5 | 4.1 | 4.1 | 63.2 | 155.1 | 20.2 |
| `max_aggregate` | 30.8 | 6.2 | 7.0 | 58.6 | 163.2 | 17.4 |
| `min_aggregate` | 31.1 | 6.2 | 6.8 | 58.6 | 163.5 | 17.3 |
| `order_by_multi_key` | 336.9 | 274.1 | 281.9 | 458.6 | 445.5 | 335.6 |
| `order_distinct_take` | 140.6 | 15.9 | 99.4 | 72.3 | 163.8 | 31.6 |
| `order_reverse_normalized` | 38.6 | 16.3 | 20.0 | 70.1 | 170.7 | 33.1 |
| `order_take_desc` | 38.3 | 16.5 | 20.6 | 70.1 | 170.9 | 33.1 |
| `point_lookup` | — | — | — | — | — | 0.0 |
| `point_lookup_scan` | — | — | — | — | — | 8.4 |
| `reverse_distinct_by` | 308.2 | 21.2 | 27.9 | 70.8 | 163.1 | 44.6 |
| `reverse_take` | 0.1 | 0.0 | 0.2 | 0.0 | 26.4 | 58.9 |
| `reverse_take_select` | 0.0 | 0.0 | 0.2 | 0.0 | 26.3 | 58.6 |
| `select_count` | 0.1 | 0.0 | 2.2 | 68.5 | 2.2 | 0.0 |
| `select_many` | — | 192.1 | — | — | — | — |
| `select_where` | 197.4 | 11.2 | 19.4 | 196.4 | 183.1 | 37.8 |
| `select_where_count` | 32.6 | 5.1 | 7.4 | 64.4 | 157.5 | 22.8 |
| `select_where_order_take` | 36.6 | 12.5 | 15.1 | 72.3 | 164.9 | 35.1 |
| `select_where_sum` | 37.1 | 7.4 | 7.5 | 66.3 | 162.5 | 23.6 |
| `single_match` | 0.0 | 2.8 | 5.4 | 58.0 | 151.0 | 22.8 |
| `skip_take` | 0.5 | 0.1 | 0.2 | 3.0 | 2.8 | 0.3 |
| `skip_while_match` | 3.4 | 5.3 | 5.3 | 59.9 | 153.2 | 18.3 |
| `sort_first` | 37.9 | 11.1 | 13.4 | 65.2 | 166.1 | 32.2 |
| `sort_take` | 38.3 | 16.3 | 20.4 | 70.3 | 171.0 | 33.1 |
| `sort_take_select` | 38.3 | 16.3 | 20.2 | 70.7 | 170.5 | 33.2 |
| `sum_aggregate` | 30.2 | 2.1 | 2.1 | 53.9 | 153.3 | 13.5 |
| `sum_where` | 32.8 | 4.2 | 4.3 | 63.4 | 154.2 | 20.5 |
| `take_count` | 3.6 | 0.2 | 0.4 | 2.9 | 2.7 | 0.5 |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 1.3 | 1.1 | 0.3 |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 0.6 | 0.5 | 0.1 |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 0.7 | 0.6 | 0.2 |
| `take_while_match` | 7.8 | 2.4 | 2.4 | 30.1 | 75.7 | 16.4 |
| `to_array_filter` | 70.3 | 11.8 | 11.8 | 70.9 | 163.7 | 29.0 |
| `to_table` | — | 18.7 | 144.0 | 118.2 | 144.3 | 32.2 |
| `to_table_staged` | — | 54.8 | 56.8 | 144.8 | 166.8 | 69.0 |
| `where_join_count` | 41.2 | 29.1 | 41.8 | 131.7 | 167.5 | 46.8 |
| `zip_count_pred` | 39.4 | 15.9 | — | 317.3 | 319.1 | — |
| `zip_dot_product` | 46.6 | 12.7 | 10.6 | 314.0 | 316.5 | — |
| `zip_dot_product_3arg` | 46.8 | 12.8 | — | 313.0 | 316.7 | — |
| `zip_reverse_to_array` | — | 31.7 | — | 349.3 | 351.4 | — |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) | Table fold (m7) |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 35.0 | 0.3 | 0.7 | 29.8 | 27.2 | 13.5 |
| `all_match` | 27.9 | 0.3 | 0.2 | 18.8 | 26.2 | 13.5 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.2 | 1.0 | 3.5 | 18.8 | 25.7 | 13.5 |
| `bare_last` | — | 0.4 | 0.0 | 0.0 | 0.0 | 17.2 |
| `bare_order_where` | 185.1 | 34.2 | 35.0 | 105.5 | 53.0 | 78.8 |
| `chained_select_collapse` | — | 1.1 | 1.1 | 20.6 | 33.9 | 14.0 |
| `chained_where` | 36.9 | 0.6 | 0.8 | 36.6 | 32.1 | 17.8 |
| `contains_match` | 0.0 | 0.2 | 0.1 | 17.5 | 9.4 | 4.7 |
| `count_aggregate` | 29.5 | 0.3 | 0.6 | 29.5 | 26.4 | 13.5 |
| `cross_join` | 5991.6 | 734.4 | — | 834.6 | 771.2 | — |
| `decs_count_bare_pred` | — | — | 0.6 | — | — | — |
| `distinct_by_count` | 42.1 | 1.1 | 1.1 | 20.6 | 33.9 | 14.1 |
| `distinct_by_order_take` | 249.6 | 1.7 | 2.6 | 45.2 | 39.0 | 30.3 |
| `distinct_by_order_to_array` | 252.5 | 1.7 | 2.7 | 45.5 | 38.9 | 30.2 |
| `distinct_count` | 41.7 | 1.1 | 1.1 | 20.6 | 33.7 | 14.1 |
| `distinct_count_pred` | 265.8 | 1.1 | 1.3 | 37.8 | 43.6 | 14.0 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.1 | 0.0 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 177.2 | 1.6 | 1.9 | 37.2 | 45.6 | 51.9 |
| `groupby_count` | 145.8 | 1.3 | 1.5 | 20.6 | 34.1 | 43.9 |
| `groupby_first` | 265.0 | 1.3 | 2.3 | 20.7 | 34.6 | 43.7 |
| `groupby_having_count` | 144.6 | 1.3 | 1.5 | 20.7 | 34.1 | 46.7 |
| `groupby_having_hidden_sum` | 180.4 | 1.5 | 1.7 | 37.0 | 45.4 | 55.0 |
| `groupby_having_post_where` | 177.4 | 1.4 | 2.0 | 37.0 | 44.2 | 51.4 |
| `groupby_max` | 179.1 | 1.5 | 1.9 | 37.1 | 46.0 | 52.0 |
| `groupby_min` | 179.2 | 1.5 | 1.8 | 37.0 | 46.1 | 52.4 |
| `groupby_multi_reducer` | 195.2 | 1.6 | 2.0 | 37.1 | 45.9 | 61.3 |
| `groupby_select_order` | 176.3 | 1.4 | 1.9 | 37.0 | 44.4 | 51.4 |
| `groupby_select_sum` | 205.9 | 2.8 | 3.2 | 33.2 | 39.7 | 73.0 |
| `groupby_sum` | 175.9 | 1.4 | 1.6 | 37.0 | 44.5 | 51.9 |
| `groupby_where_count` | 76.5 | 0.9 | 1.3 | 37.2 | 41.9 | 52.2 |
| `groupby_where_sum` | 87.7 | 0.9 | 1.3 | 36.9 | 42.0 | 56.1 |
| `join_count` | 38.7 | 11.0 | 11.7 | 40.9 | 71.4 | 31.8 |
| `join_groupby_count` | 160.1 | 17.4 | 19.7 | 66.4 | 90.1 | 72.9 |
| `join_groupby_to_array` | 194.1 | 17.9 | 19.8 | 78.4 | 36.1 | 81.1 |
| `join_probe` | — | — | — | — | — | 24.2 |
| `join_probe_build` | — | — | — | — | — | 39.8 |
| `join_select` | 94.0 | 19.7 | 21.8 | 72.2 | 94.4 | 70.1 |
| `join_where_count` | 39.6 | 19.3 | 20.6 | 63.2 | 78.2 | 38.0 |
| `last_match` | 0.0 | 0.5 | 1.4 | 19.6 | 26.9 | 22.8 |
| `long_count_aggregate` | 29.8 | 0.3 | 0.6 | 29.4 | 26.5 | 13.8 |
| `max_aggregate` | 31.0 | 0.3 | 0.5 | 29.8 | 27.9 | 13.5 |
| `min_aggregate` | 31.2 | 0.3 | 0.5 | 29.8 | 27.7 | 13.5 |
| `order_by_multi_key` | 251.0 | 54.8 | 54.8 | 124.4 | 71.8 | 129.5 |
| `order_distinct_take` | 142.6 | 1.1 | 75.8 | 21.0 | 35.8 | 14.0 |
| `order_reverse_normalized` | 38.7 | 0.7 | 1.4 | 19.8 | 28.6 | 17.8 |
| `order_take_desc` | 38.6 | 0.7 | 1.3 | 19.7 | 28.4 | 17.8 |
| `point_lookup` | — | — | — | — | — | 0.0 |
| `point_lookup_scan` | — | — | — | — | — | 6.0 |
| `reverse_distinct_by` | 297.0 | 1.6 | 3.1 | 20.6 | 34.6 | 18.8 |
| `reverse_take` | 0.0 | 0.0 | 0.0 | 0.0 | 3.8 | 27.0 |
| `reverse_take_select` | 0.0 | 0.0 | 0.0 | 0.0 | 3.9 | 27.1 |
| `select_count` | 0.1 | 0.0 | 0.0 | 68.1 | 0.0 | 0.0 |
| `select_many` | — | 62.7 | — | — | — | — |
| `select_where` | 108.3 | 4.1 | 5.3 | 75.4 | 23.1 | 28.2 |
| `select_where_count` | 32.9 | 0.3 | 0.6 | 29.9 | 27.2 | 13.5 |
| `select_where_order_take` | 37.0 | 0.7 | 1.4 | 19.8 | 27.9 | 23.3 |
| `select_where_sum` | 37.4 | 0.4 | 0.6 | 20.4 | 26.2 | 13.4 |
| `single_match` | 0.0 | 0.4 | 1.1 | 46.1 | 23.2 | 17.4 |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.3 | 0.2 | 0.2 |
| `skip_while_match` | 3.5 | 0.4 | 0.4 | 46.0 | 22.2 | 13.3 |
| `sort_first` | 38.3 | 0.4 | 1.3 | 18.9 | 27.5 | 17.3 |
| `sort_take` | 38.3 | 0.7 | 1.4 | 19.7 | 28.5 | 17.8 |
| `sort_take_select` | 38.4 | 0.7 | 1.3 | 19.8 | 28.4 | 17.7 |
| `sum_aggregate` | 30.4 | 0.3 | 0.1 | 23.3 | 25.6 | 13.5 |
| `sum_where` | 33.1 | 0.3 | 0.6 | 29.5 | 27.1 | 13.5 |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.2 | 0.2 | 0.3 |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 0.5 | 0.1 | 0.2 |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 0.2 | 0.0 | 0.1 |
| `take_where_count` | 0.9 | 0.0 | 0.0 | 0.3 | 0.0 | 0.1 |
| `take_while_match` | 7.8 | 0.2 | 0.3 | 17.3 | 9.3 | 13.5 |
| `to_array_filter` | 48.5 | 3.3 | 3.4 | 22.2 | 35.4 | 20.3 |
| `to_table` | — | 14.1 | 37.4 | 49.7 | 54.3 | 29.2 |
| `to_table_staged` | — | 25.8 | 26.1 | 53.5 | 64.1 | 42.1 |
| `where_join_count` | 39.6 | 5.8 | 6.8 | 47.7 | 42.1 | 26.9 |
| `zip_count_pred` | 39.2 | 0.1 | — | 112.6 | 34.2 | — |
| `zip_dot_product` | 46.9 | 0.1 | 0.1 | 112.4 | 34.1 | — |
| `zip_dot_product_3arg` | 46.9 | 0.1 | — | 112.4 | 34.1 | — |
| `zip_reverse_to_array` | — | 4.6 | — | 123.5 | 51.8 | — |
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
