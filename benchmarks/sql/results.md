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
| `aggregate_match` | 34.9 | 5.9 | 5.8 | 60.8 | 158.9 | 19.5 |
| `all_match` | 27.7 | 3.5 | 3.4 | 56.0 | 153.2 | 15.9 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.3 | 5.9 | 8.7 | 58.5 | 163.4 | 17.3 |
| `bare_last` | — | 4.2 | 0.0 | 0.0 | 4.2 | 30.1 |
| `bare_order_where` | 282.9 | 118.2 | 125.0 | 300.5 | 290.8 | 163.1 |
| `chained_select_collapse` | — | 17.8 | 17.5 | 70.4 | 161.7 | 27.7 |
| `chained_where` | 41.5 | 6.6 | 7.1 | 104.8 | 182.1 | 24.0 |
| `contains_match` | 0.0 | 2.2 | 1.4 | 28.9 | 71.5 | 6.6 |
| `count_aggregate` | 29.6 | 4.3 | 4.1 | 63.5 | 154.0 | 20.2 |
| `cross_join` | 12896.3 | 3681.4 | — | 4018.5 | 4096.4 | — |
| `decs_count_bare_pred` | — | — | 4.1 | — | — | — |
| `distinct_by_count` | 41.4 | 15.7 | 15.7 | 70.4 | 161.3 | 26.8 |
| `distinct_by_order_take` | 239.9 | 22.3 | 23.3 | 123.9 | 162.0 | 48.8 |
| `distinct_by_order_to_array` | 237.8 | 22.3 | 23.3 | 124.3 | 162.5 | 48.8 |
| `distinct_count` | 41.8 | 15.9 | 15.7 | 70.7 | 161.8 | 27.0 |
| `distinct_count_pred` | 252.1 | 15.8 | 15.6 | 111.9 | 176.7 | 26.6 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.4 | 0.3 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 171.0 | 29.4 | 29.0 | 123.0 | 196.4 | — |
| `groupby_count` | 142.4 | 19.2 | 19.1 | 74.8 | 167.1 | 164.5 |
| `groupby_first` | 251.1 | 19.2 | 19.7 | 72.1 | 162.2 | — |
| `groupby_having_count` | 142.0 | 19.1 | 19.1 | 74.7 | 166.3 | — |
| `groupby_having_hidden_sum` | 176.6 | 22.3 | 22.3 | 118.0 | 187.9 | — |
| `groupby_having_post_where` | 173.2 | 20.5 | 20.4 | 114.4 | 187.4 | — |
| `groupby_max` | 173.5 | 24.9 | 24.8 | 119.6 | 191.4 | — |
| `groupby_min` | 173.8 | 25.3 | 24.8 | 119.6 | 192.5 | — |
| `groupby_multi_reducer` | 190.5 | 30.4 | 30.0 | 124.7 | 196.1 | — |
| `groupby_select_order` | 172.1 | 20.5 | 20.4 | 114.3 | 188.6 | — |
| `groupby_select_sum` | 199.6 | 38.5 | 38.0 | 101.5 | 194.4 | — |
| `groupby_sum` | 172.1 | 20.5 | 20.4 | 114.6 | 187.6 | 194.6 |
| `groupby_where_count` | 76.4 | 14.1 | 14.2 | 115.1 | 185.8 | — |
| `groupby_where_sum` | 87.5 | 14.2 | 14.5 | 116.0 | 186.7 | — |
| `join_count` | 38.4 | 51.4 | 63.6 | 112.9 | 183.8 | 65.4 |
| `join_groupby_count` | 158.4 | 77.8 | 87.8 | 177.4 | 233.1 | — |
| `join_groupby_to_array` | 189.8 | 78.7 | 89.6 | 214.7 | 214.1 | — |
| `join_probe` | — | — | — | — | — | 46.9 |
| `join_probe_build` | — | — | — | — | — | 79.5 |
| `join_select` | 151.8 | 72.8 | 84.9 | 189.5 | 217.4 | — |
| `join_where_count` | 39.7 | 61.7 | 78.7 | 160.5 | 199.8 | 81.6 |
| `last_match` | 0.0 | 5.9 | 14.0 | 65.0 | 159.2 | 31.0 |
| `long_count_aggregate` | 29.9 | 4.1 | 4.1 | 63.4 | 154.0 | 20.1 |
| `max_aggregate` | 31.1 | 6.0 | 6.8 | 58.7 | 162.1 | 16.9 |
| `min_aggregate` | 31.0 | 6.0 | 6.9 | 58.7 | 162.9 | 17.0 |
| `order_by_multi_key` | 340.9 | 270.9 | 279.5 | 459.2 | 446.7 | 336.4 |
| `order_distinct_take` | 138.7 | 15.9 | 98.6 | 72.6 | 162.8 | 31.6 |
| `order_reverse_normalized` | 38.8 | 16.3 | 19.8 | 70.9 | 169.9 | — |
| `order_take_desc` | 38.5 | 16.3 | 19.9 | 70.1 | 170.8 | 33.3 |
| `point_lookup` | — | — | — | — | — | 0.0 |
| `point_lookup_scan` | — | — | — | — | — | 8.3 |
| `reverse_distinct_by` | 295.3 | 21.3 | 28.2 | 71.1 | 161.9 | — |
| `reverse_take` | 0.1 | 0.0 | 0.2 | 0.0 | 26.1 | 58.5 |
| `reverse_take_select` | 0.0 | 0.0 | 0.2 | 0.0 | 26.1 | — |
| `select_count` | 0.1 | 0.0 | 2.2 | 68.3 | 2.2 | 0.0 |
| `select_many` | — | 191.7 | — | — | — | — |
| `select_where` | 204.1 | 11.2 | 19.3 | 197.1 | 183.4 | 37.7 |
| `select_where_count` | 32.5 | 5.1 | 7.4 | 64.9 | 156.9 | 22.7 |
| `select_where_order_take` | 37.1 | 12.3 | 14.8 | 72.8 | 165.4 | 35.3 |
| `select_where_sum` | 37.1 | 7.5 | 7.5 | 66.5 | 161.9 | 25.0 |
| `single_match` | 0.0 | 2.9 | 5.5 | 58.2 | 151.2 | 22.6 |
| `skip_take` | 0.5 | 0.1 | 0.2 | 3.1 | 2.8 | 0.3 |
| `skip_while_match` | 3.5 | 5.3 | 5.3 | 60.0 | 153.2 | 18.2 |
| `sort_first` | 38.4 | 11.1 | 13.3 | 65.1 | 166.7 | 32.2 |
| `sort_take` | 38.7 | 16.3 | 20.0 | 70.8 | 170.4 | 33.1 |
| `sort_take_select` | 38.7 | 16.3 | 20.1 | 71.3 | 170.6 | 33.3 |
| `sum_aggregate` | 30.5 | 2.1 | 2.1 | 54.6 | 153.2 | 13.4 |
| `sum_where` | 33.2 | 4.3 | 4.2 | 63.4 | 154.6 | 20.4 |
| `take_count` | 3.8 | 0.2 | 0.4 | 2.9 | 2.7 | 0.5 |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 1.3 | 1.1 | 0.3 |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 0.6 | 0.5 | 0.1 |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 0.7 | 0.6 | 0.2 |
| `take_while_match` | 7.8 | 2.4 | 2.4 | 30.1 | 76.2 | 16.4 |
| `to_array_filter` | 71.1 | 11.8 | 11.7 | 71.3 | 164.3 | 28.9 |
| `to_table` | — | — | — | — | — | 32.5 |
| `to_table_staged` | — | — | — | — | — | 68.3 |
| `where_join_count` | 41.5 | 28.8 | 40.9 | 132.1 | 167.0 | — |
| `zip_count_pred` | 39.5 | 15.8 | — | 318.5 | 320.2 | — |
| `zip_dot_product` | 47.2 | 12.6 | 10.8 | 312.7 | 318.6 | — |
| `zip_dot_product_3arg` | 47.1 | 12.7 | — | 312.8 | 317.5 | — |
| `zip_reverse_to_array` | — | 31.4 | — | 348.8 | 352.2 | — |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) | Table fold (m7) |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 35.1 | 0.3 | 0.6 | 22.8 | 26.2 | 13.5 |
| `all_match` | 27.9 | 0.3 | 0.2 | 17.5 | 25.3 | 13.6 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.5 | 1.0 | 3.5 | 17.4 | 24.7 | 13.5 |
| `bare_last` | — | 0.4 | 0.0 | 0.0 | 0.0 | 17.1 |
| `bare_order_where` | 186.2 | 34.1 | 35.0 | 104.9 | 52.8 | 78.8 |
| `chained_select_collapse` | — | 1.1 | 1.1 | 20.6 | 33.5 | 14.0 |
| `chained_where` | 36.9 | 0.6 | 0.8 | 34.7 | 31.3 | 17.8 |
| `contains_match` | 0.0 | 0.2 | 0.1 | 17.5 | 8.9 | 4.7 |
| `count_aggregate` | 29.7 | 0.3 | 0.6 | 17.5 | 25.5 | 13.4 |
| `cross_join` | 5965.9 | 731.0 | — | 833.2 | 770.0 | — |
| `decs_count_bare_pred` | — | — | 0.6 | — | — | — |
| `distinct_by_count` | 41.7 | 1.1 | 1.1 | 20.6 | 33.5 | 14.0 |
| `distinct_by_order_take` | 239.3 | 1.7 | 2.6 | 46.3 | 38.8 | 30.1 |
| `distinct_by_order_to_array` | 240.2 | 1.7 | 2.7 | 46.4 | 38.7 | 30.3 |
| `distinct_count` | 41.6 | 1.1 | 1.1 | 20.6 | 33.5 | 14.0 |
| `distinct_count_pred` | 251.7 | 1.1 | 1.3 | 37.7 | 43.8 | 14.0 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.1 | 0.0 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 171.5 | 1.5 | 1.9 | 35.5 | 45.5 | — |
| `groupby_count` | 142.1 | 1.3 | 1.5 | 20.6 | 33.8 | 42.7 |
| `groupby_first` | 251.9 | 1.3 | 2.3 | 20.6 | 34.4 | — |
| `groupby_having_count` | 141.3 | 1.3 | 1.5 | 20.6 | 33.9 | — |
| `groupby_having_hidden_sum` | 176.9 | 1.5 | 1.7 | 35.5 | 45.2 | — |
| `groupby_having_post_where` | 171.1 | 1.4 | 1.9 | 35.5 | 44.1 | — |
| `groupby_max` | 173.4 | 1.5 | 1.9 | 35.5 | 45.8 | — |
| `groupby_min` | 172.8 | 1.5 | 1.8 | 35.6 | 45.8 | — |
| `groupby_multi_reducer` | 190.2 | 1.6 | 1.9 | 35.8 | 46.1 | — |
| `groupby_select_order` | 170.9 | 1.4 | 1.9 | 35.4 | 44.3 | — |
| `groupby_select_sum` | 200.0 | 2.8 | 3.2 | 31.8 | 39.9 | — |
| `groupby_sum` | 170.9 | 1.4 | 1.6 | 35.5 | 44.3 | 51.2 |
| `groupby_where_count` | 76.3 | 0.9 | 1.3 | 35.6 | 41.8 | — |
| `groupby_where_sum` | 87.6 | 0.9 | 1.3 | 35.6 | 41.9 | — |
| `join_count` | 38.2 | 10.9 | 11.8 | 42.6 | 71.5 | 32.2 |
| `join_groupby_count` | 156.9 | 17.6 | 19.5 | 68.3 | 89.8 | — |
| `join_groupby_to_array` | 189.8 | 17.5 | 19.4 | 79.3 | 36.1 | — |
| `join_probe` | — | — | — | — | — | 24.0 |
| `join_probe_build` | — | — | — | — | — | 38.1 |
| `join_select` | 94.0 | 19.6 | 21.7 | 73.8 | 95.2 | — |
| `join_where_count` | 39.8 | 18.9 | 20.8 | 63.5 | 78.3 | 37.8 |
| `last_match` | 0.0 | 0.5 | 1.4 | 18.2 | 25.9 | 22.9 |
| `long_count_aggregate` | 29.2 | 0.3 | 0.6 | 17.5 | 25.5 | 13.4 |
| `max_aggregate` | 31.0 | 0.3 | 0.5 | 17.4 | 27.1 | 13.4 |
| `min_aggregate` | 31.1 | 0.3 | 0.5 | 17.4 | 27.0 | 13.5 |
| `order_by_multi_key` | 250.0 | 53.1 | 54.7 | 123.6 | 71.3 | 129.4 |
| `order_distinct_take` | 138.1 | 1.1 | 75.3 | 20.9 | 35.7 | 14.0 |
| `order_reverse_normalized` | 38.5 | 0.7 | 1.3 | 22.0 | 27.7 | — |
| `order_take_desc` | 38.2 | 0.7 | 1.3 | 22.0 | 27.5 | 17.8 |
| `point_lookup` | — | — | — | — | — | 0.0 |
| `point_lookup_scan` | — | — | — | — | — | 6.0 |
| `reverse_distinct_by` | 295.7 | 1.5 | 3.2 | 20.5 | 34.4 | — |
| `reverse_take` | 0.0 | 0.0 | 0.0 | 0.0 | 3.8 | 26.9 |
| `reverse_take_select` | 0.0 | 0.0 | 0.0 | 0.0 | 3.9 | — |
| `select_count` | 0.1 | 0.0 | 0.0 | 67.0 | 0.0 | 0.0 |
| `select_many` | — | 62.5 | — | — | — | — |
| `select_where` | 110.7 | 4.1 | 5.3 | 74.8 | 22.1 | 27.9 |
| `select_where_count` | 32.6 | 0.3 | 0.6 | 17.4 | 26.3 | 13.4 |
| `select_where_order_take` | 36.7 | 0.7 | 1.3 | 18.4 | 27.3 | 23.1 |
| `select_where_sum` | 37.2 | 0.4 | 0.6 | 17.4 | 25.6 | 13.4 |
| `single_match` | 0.0 | 0.4 | 1.1 | 46.2 | 22.3 | 17.3 |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.3 | 0.3 | 0.2 |
| `skip_while_match` | 3.4 | 0.4 | 0.4 | 46.0 | 21.8 | 13.2 |
| `sort_first` | 38.4 | 0.4 | 1.3 | 17.4 | 26.7 | 17.2 |
| `sort_take` | 38.6 | 0.7 | 1.3 | 22.0 | 27.9 | 17.8 |
| `sort_take_select` | 38.3 | 0.7 | 1.3 | 21.9 | 27.7 | 17.8 |
| `sum_aggregate` | 30.6 | 0.3 | 0.1 | 17.7 | 24.9 | 13.5 |
| `sum_where` | 33.0 | 0.3 | 0.6 | 17.4 | 26.3 | 13.4 |
| `take_count` | 1.9 | 0.1 | 0.1 | 1.2 | 0.3 | 0.2 |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 0.4 | 0.1 | 0.1 |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 0.2 | 0.0 | 0.1 |
| `take_where_count` | 0.9 | 0.0 | 0.0 | 0.2 | 0.0 | 0.1 |
| `take_while_match` | 7.8 | 0.2 | 0.3 | 17.4 | 8.9 | 13.4 |
| `to_array_filter` | 48.9 | 3.2 | 3.3 | 20.8 | 35.5 | 20.2 |
| `to_table` | — | — | — | — | — | 28.8 |
| `to_table_staged` | — | — | — | — | — | 41.6 |
| `where_join_count` | 41.3 | 5.7 | 6.8 | 48.8 | 41.8 | — |
| `zip_count_pred` | 39.8 | 0.1 | — | 115.3 | 33.8 | — |
| `zip_dot_product` | 47.3 | 0.1 | 0.1 | 115.4 | 33.8 | — |
| `zip_dot_product_3arg` | 47.1 | 0.1 | — | 115.3 | 33.7 | — |
| `zip_reverse_to_array` | — | 4.5 | — | 127.0 | 51.4 | — |
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
- **m7 absent families** — `zip_*` / `cross_join` (lockstep over an unordered slot walk is meaningless), `select_many` (flat fixture, no nested array field), `order_reverse_normalized` / `reverse_take_select` / `reverse_distinct_by` (no backward slot walk; `reverse_take` is kept as the single deferral marker), the group-by tail beyond `groupby_count`/`groupby_sum` (table group_by fusion is a named deferred edge — see `LINQ_TO_TABLE.md`; the two marker cells track the tier-2 cost) plus the join-composition lanes (`join_select` / `where_join_count` would fuse today but aren't instantiated; `join_groupby_*` needs the deferred group_by), `decs_count_bare_pred` (decs-only).
- **`point_lookup` / `point_lookup_scan` non-m7** — m7-only pair: only a table source has a key to probe (`where(kv.key == X)` + terminator → `key_exists` / `tab?[X]`, O(1)); the `_scan` twin forces the same query through the walk (compound `&&` predicate declines the probe) to show the gap. Other sources have no analog by design.
- **`join_probe` / `join_probe_build` non-m7** — m7-only A/B pair: a table srcB joined on its bare key probes the user's table per lead row (no internal join hash, no build loop); the `_build` twin feeds the identical rows pre-materialized to a kv array, forcing the hashed build. Other sources have no keyed-srcB analog by design.
- **`to_table` / `to_table_staged` non-m7** — m7-only A/B pair for the `to_table()` sink: the fused insert-loop lands the kv chain straight in the result table (reserve from O(1) length); the `_staged` twin materializes the same projection to an array first, then converts via the consuming builtin `to_table_move` — the shape every chain had before the sink arm. The sink itself works over any direct-loop source (the array lane fuses it too); only the bench pair is table-scoped.

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
