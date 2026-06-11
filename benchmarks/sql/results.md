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
  `join_probe_build` pair measures it; group_by / reverse defer to tier-2 until their stages land).

`0.00` = early-exit terminator below timer resolution ("free"). Chain shapes are in
`benchmarks/README.md`; the splice arms each fires are in `doc/source/reference/linq_fold_patterns.rst`.

**JIT on Apple silicon is high-variance run-to-run — read the INTERP column as the reliable
signal, JIT deltas as indicative.**

<!-- BENCH:TABLES BEGIN -->
*Generated 2026-06-11 by `benchmarks/sql/_update_results.das` — ns/op; `—` = absent lane. Edit the prose around the markers, not the tables.*

## INTERP

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) | Table fold (m7) |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 34.8 | 5.9 | 5.8 | 60.6 | 159.5 | 19.2 |
| `all_match` | 27.5 | 3.5 | 3.4 | 56.1 | 154.1 | 16.4 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.6 | 5.9 | 8.8 | 58.4 | 164.3 | 17.3 |
| `bare_last` | — | 4.2 | 0.0 | 0.0 | 4.2 | 30.6 |
| `bare_order_where` | 284.5 | 117.8 | 126.7 | 300.9 | 291.5 | 163.8 |
| `chained_select_collapse` | — | 18.3 | 17.5 | 70.4 | 162.2 | 28.0 |
| `chained_where` | 36.1 | 6.6 | 7.1 | 104.9 | 183.8 | 24.1 |
| `contains_match` | 0.0 | 2.2 | 1.4 | 29.1 | 72.0 | 6.6 |
| `count_aggregate` | 29.8 | 4.1 | 4.1 | 63.7 | 155.9 | 20.3 |
| `cross_join` | 12556.2 | 3697.8 | — | 4012.8 | 4069.8 | — |
| `decs_count_bare_pred` | — | — | 4.1 | — | — | — |
| `distinct_by_count` | 41.0 | 15.7 | 15.6 | 70.6 | 160.7 | 26.6 |
| `distinct_by_order_take` | 239.3 | 22.1 | 23.4 | 123.7 | 163.1 | 48.5 |
| `distinct_by_order_to_array` | 238.9 | 22.1 | 23.5 | 124.2 | 163.1 | 48.8 |
| `distinct_count` | 41.0 | 15.8 | 15.8 | 70.8 | 162.4 | 27.0 |
| `distinct_count_pred` | 254.3 | 15.8 | 15.9 | 112.2 | 177.8 | 26.8 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.4 | 0.3 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 171.8 | 29.2 | 29.3 | 123.7 | 197.4 | — |
| `groupby_count` | 141.9 | 19.5 | 19.5 | 75.0 | 167.5 | 162.7 |
| `groupby_first` | 252.6 | 19.5 | 20.2 | 72.2 | 162.7 | — |
| `groupby_having_count` | 141.8 | 19.5 | 19.5 | 74.8 | 169.1 | — |
| `groupby_having_hidden_sum` | 175.7 | 23.3 | 22.6 | 118.8 | 192.7 | — |
| `groupby_having_post_where` | 171.2 | 20.8 | 20.8 | 114.6 | 189.2 | — |
| `groupby_max` | 173.9 | 24.9 | 25.4 | 120.5 | 193.1 | — |
| `groupby_min` | 173.7 | 25.0 | 25.1 | 120.0 | 192.9 | — |
| `groupby_multi_reducer` | 190.8 | 30.2 | 30.6 | 124.9 | 196.2 | — |
| `groupby_select_order` | 170.9 | 20.8 | 20.8 | 114.8 | 188.6 | — |
| `groupby_select_sum` | 198.9 | 38.6 | 38.2 | 101.7 | 195.2 | — |
| `groupby_sum` | 170.8 | 20.8 | 20.8 | 114.9 | 188.4 | 192.8 |
| `groupby_where_count` | 76.0 | 14.1 | 14.3 | 116.6 | 186.3 | — |
| `groupby_where_sum` | 86.7 | 14.1 | 14.7 | 116.4 | 186.4 | — |
| `join_count` | 38.3 | 51.3 | 64.6 | 113.1 | 183.4 | 65.6 |
| `join_groupby_count` | 157.6 | 77.4 | 88.8 | 177.7 | 230.9 | — |
| `join_groupby_to_array` | 189.1 | 78.0 | 90.6 | 215.4 | 213.5 | — |
| `join_probe` | — | — | — | — | — | 47.3 |
| `join_probe_build` | — | — | — | — | — | 79.1 |
| `join_select` | 152.6 | 72.5 | 84.7 | 188.7 | 214.4 | — |
| `join_where_count` | 48.6 | 61.6 | 76.8 | 160.4 | 199.8 | 81.4 |
| `last_match` | 0.0 | 6.1 | 13.9 | 65.1 | 159.7 | 31.0 |
| `long_count_aggregate` | 29.1 | 4.1 | 4.1 | 63.4 | 154.3 | 21.2 |
| `max_aggregate` | 30.7 | 6.0 | 6.8 | 58.6 | 163.1 | 17.0 |
| `min_aggregate` | 31.2 | 6.0 | 6.9 | 58.7 | 163.6 | 17.0 |
| `order_by_multi_key` | 348.8 | 272.2 | 282.9 | 458.7 | 449.2 | 334.0 |
| `order_distinct_take` | 137.8 | 15.9 | 99.3 | 72.5 | 162.8 | 31.3 |
| `order_reverse_normalized` | 38.1 | 16.3 | 20.0 | 70.7 | 170.6 | — |
| `order_take_desc` | 38.5 | 16.2 | 20.4 | 70.1 | 170.4 | 33.3 |
| `point_lookup` | — | — | — | — | — | 0.0 |
| `point_lookup_scan` | — | — | — | — | — | 8.4 |
| `reverse_distinct_by` | 295.5 | 21.3 | 28.0 | 70.9 | 162.2 | — |
| `reverse_take` | 0.1 | 0.0 | 0.2 | 0.0 | 26.2 | 58.8 |
| `reverse_take_select` | 0.0 | 0.0 | 0.2 | 0.0 | 26.2 | — |
| `select_count` | 0.1 | 0.0 | 2.2 | 69.3 | 2.2 | 0.0 |
| `select_many` | — | 190.7 | — | — | — | — |
| `select_where` | 207.9 | 11.2 | 19.5 | 195.5 | 188.7 | 37.6 |
| `select_where_count` | 32.4 | 5.1 | 7.4 | 64.6 | 158.7 | 21.7 |
| `select_where_order_take` | 36.3 | 12.3 | 15.1 | 72.7 | 164.5 | 34.5 |
| `select_where_sum` | 37.2 | 7.5 | 7.5 | 66.5 | 164.6 | 23.3 |
| `single_match` | 0.0 | 2.9 | 5.5 | 58.4 | 151.5 | 22.6 |
| `skip_take` | 0.5 | 0.1 | 0.2 | 3.0 | 2.8 | 0.3 |
| `skip_while_match` | 3.5 | 5.3 | 5.3 | 59.9 | 153.1 | 18.3 |
| `sort_first` | 37.9 | 11.0 | 13.3 | 64.9 | 167.0 | 32.0 |
| `sort_take` | 38.4 | 16.3 | 20.9 | 70.5 | 171.5 | 33.3 |
| `sort_take_select` | 38.2 | 16.3 | 20.9 | 71.0 | 170.8 | 33.2 |
| `sum_aggregate` | 29.6 | 2.1 | 2.1 | 54.4 | 153.0 | 13.5 |
| `sum_where` | 32.1 | 4.4 | 11.5 | 63.8 | 154.6 | 21.3 |
| `take_count` | 3.9 | 0.2 | 0.4 | 2.9 | 2.7 | 0.5 |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 1.3 | 1.1 | 0.3 |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 0.6 | 0.5 | 0.1 |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 0.7 | 0.6 | 0.2 |
| `take_while_match` | 7.8 | 2.4 | 2.4 | 30.2 | 75.6 | 16.4 |
| `to_array_filter` | 70.2 | 11.8 | 11.8 | 71.5 | 165.1 | 29.0 |
| `where_join_count` | 41.2 | 29.1 | 41.7 | 132.7 | 168.6 | — |
| `zip_count_pred` | 39.3 | 15.9 | — | 315.0 | 321.2 | — |
| `zip_dot_product` | 46.2 | 12.6 | 10.6 | 309.2 | 319.0 | — |
| `zip_dot_product_3arg` | 46.2 | 12.8 | — | 309.4 | 320.7 | — |
| `zip_reverse_to_array` | — | 31.7 | — | 345.0 | 353.4 | — |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) | Table fold (m7) |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 35.0 | 0.3 | 0.6 | 21.7 | 27.1 | 13.5 |
| `all_match` | 27.9 | 0.3 | 0.2 | 18.1 | 26.2 | 13.5 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.5 | 1.0 | 3.6 | 18.1 | 25.7 | 13.5 |
| `bare_last` | — | 0.4 | 0.0 | 0.0 | 0.0 | 17.2 |
| `bare_order_where` | 188.1 | 35.3 | 35.5 | 106.7 | 53.3 | 79.0 |
| `chained_select_collapse` | — | 1.1 | 1.1 | 20.6 | 33.5 | 14.1 |
| `chained_where` | 36.1 | 0.6 | 0.8 | 35.7 | 32.0 | 17.7 |
| `contains_match` | 0.0 | 0.2 | 0.1 | 17.5 | 9.2 | 4.7 |
| `count_aggregate` | 29.6 | 0.3 | 0.6 | 20.6 | 26.4 | 13.5 |
| `cross_join` | 5976.1 | 733.7 | — | 837.5 | 767.7 | — |
| `decs_count_bare_pred` | — | — | 0.6 | — | — | — |
| `distinct_by_count` | 41.2 | 1.1 | 1.1 | 20.6 | 33.6 | 14.1 |
| `distinct_by_order_take` | 239.4 | 1.7 | 2.6 | 47.4 | 39.2 | 30.1 |
| `distinct_by_order_to_array` | 239.3 | 1.7 | 2.7 | 47.4 | 38.9 | 30.1 |
| `distinct_count` | 41.3 | 1.1 | 1.1 | 20.5 | 33.7 | 14.1 |
| `distinct_count_pred` | 252.4 | 1.1 | 1.3 | 37.4 | 43.4 | 14.1 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.1 | 0.0 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 170.7 | 1.6 | 1.9 | 35.9 | 44.3 | — |
| `groupby_count` | 141.5 | 1.3 | 1.5 | 20.6 | 32.7 | 42.9 |
| `groupby_first` | 252.2 | 1.3 | 2.3 | 20.6 | 33.3 | — |
| `groupby_having_count` | 141.3 | 1.3 | 1.5 | 20.6 | 33.3 | — |
| `groupby_having_hidden_sum` | 175.6 | 1.5 | 1.7 | 36.0 | 45.2 | — |
| `groupby_having_post_where` | 171.9 | 1.6 | 2.0 | 35.9 | 44.3 | — |
| `groupby_max` | 172.8 | 1.5 | 1.9 | 36.0 | 45.9 | — |
| `groupby_min` | 173.4 | 1.5 | 1.8 | 35.9 | 45.9 | — |
| `groupby_multi_reducer` | 190.6 | 1.6 | 2.0 | 36.2 | 46.1 | — |
| `groupby_select_order` | 170.6 | 1.4 | 1.9 | 35.7 | 44.2 | — |
| `groupby_select_sum` | 198.6 | 2.8 | 3.2 | 32.2 | 39.7 | — |
| `groupby_sum` | 170.3 | 1.4 | 1.7 | 35.8 | 44.2 | 51.5 |
| `groupby_where_count` | 76.0 | 0.9 | 1.3 | 36.1 | 41.8 | — |
| `groupby_where_sum` | 86.7 | 0.9 | 1.3 | 36.0 | 41.7 | — |
| `join_count` | 38.3 | 10.9 | 11.7 | 43.5 | 71.4 | 33.1 |
| `join_groupby_count` | 157.6 | 18.2 | 20.1 | 68.5 | 89.9 | — |
| `join_groupby_to_array` | 189.7 | 17.6 | 19.5 | 80.3 | 36.2 | — |
| `join_probe` | — | — | — | — | — | 24.2 |
| `join_probe_build` | — | — | — | — | — | 38.1 |
| `join_select` | 95.4 | 19.7 | 21.7 | 75.0 | 94.3 | — |
| `join_where_count` | 39.4 | 18.9 | 20.8 | 64.4 | 78.4 | 37.9 |
| `last_match` | 0.0 | 0.5 | 1.4 | 18.9 | 26.8 | 22.9 |
| `long_count_aggregate` | 29.0 | 0.3 | 0.6 | 20.5 | 26.4 | 13.5 |
| `max_aggregate` | 30.7 | 0.3 | 0.5 | 18.4 | 27.7 | 13.5 |
| `min_aggregate` | 30.7 | 0.3 | 0.5 | 18.4 | 27.7 | 13.5 |
| `order_by_multi_key` | 252.6 | 53.4 | 55.0 | 125.4 | 71.9 | 129.1 |
| `order_distinct_take` | 137.9 | 1.1 | 75.7 | 20.9 | 36.0 | 14.0 |
| `order_reverse_normalized` | 38.2 | 0.7 | 1.4 | 24.6 | 28.5 | — |
| `order_take_desc` | 38.1 | 0.7 | 1.4 | 24.6 | 28.4 | 17.7 |
| `point_lookup` | — | — | — | — | — | 0.0 |
| `point_lookup_scan` | — | — | — | — | — | 6.0 |
| `reverse_distinct_by` | 295.4 | 1.5 | 3.2 | 20.6 | 34.6 | — |
| `reverse_take` | 0.0 | 0.0 | 0.0 | 0.0 | 3.7 | 26.9 |
| `reverse_take_select` | 0.0 | 0.0 | 0.0 | 0.0 | 3.9 | — |
| `select_count` | 0.1 | 0.0 | 0.0 | 66.0 | 0.0 | 0.0 |
| `select_many` | — | 62.7 | — | — | — | — |
| `select_where` | 109.1 | 4.1 | 5.3 | 76.2 | 23.0 | 28.1 |
| `select_where_count` | 32.3 | 0.3 | 0.6 | 18.5 | 27.2 | 13.4 |
| `select_where_order_take` | 36.5 | 0.7 | 1.4 | 19.0 | 27.9 | 23.0 |
| `select_where_sum` | 37.1 | 0.4 | 0.6 | 18.0 | 26.3 | 13.4 |
| `single_match` | 0.0 | 0.4 | 1.1 | 46.3 | 23.2 | 17.4 |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.3 | 0.2 | 0.1 |
| `skip_while_match` | 3.5 | 0.4 | 0.4 | 45.8 | 22.7 | 13.3 |
| `sort_first` | 37.9 | 0.4 | 1.3 | 18.1 | 27.5 | 17.3 |
| `sort_take` | 37.9 | 0.7 | 1.4 | 24.6 | 28.3 | 17.8 |
| `sort_take_select` | 37.8 | 0.7 | 1.4 | 24.6 | 28.4 | 17.8 |
| `sum_aggregate` | 29.9 | 0.3 | 0.1 | 23.2 | 25.6 | 13.5 |
| `sum_where` | 32.1 | 0.3 | 0.6 | 18.5 | 27.2 | 13.4 |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.2 | 0.3 | 0.2 |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 0.4 | 0.1 | 0.1 |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 0.2 | 0.0 | 0.1 |
| `take_where_count` | 0.9 | 0.0 | 0.0 | 0.2 | 0.0 | 0.1 |
| `take_while_match` | 7.8 | 0.2 | 0.3 | 17.1 | 9.3 | 13.5 |
| `to_array_filter` | 47.4 | 3.3 | 3.3 | 21.5 | 35.1 | 20.2 |
| `where_join_count` | 39.4 | 5.8 | 6.8 | 49.7 | 42.3 | — |
| `zip_count_pred` | 39.4 | 0.1 | — | 117.0 | 33.9 | — |
| `zip_dot_product` | 46.5 | 0.1 | 0.1 | 117.1 | 33.8 | — |
| `zip_dot_product_3arg` | 46.4 | 0.1 | — | 116.9 | 33.7 | — |
| `zip_reverse_to_array` | — | 4.5 | — | 128.4 | 51.3 | — |
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
- **m7 absent families** — `zip_*` / `cross_join` (lockstep over an unordered slot walk is meaningless), `select_many` (flat fixture, no nested array field), `order_reverse_normalized` / `reverse_take_select` / `reverse_distinct_by` (no backward slot walk; `reverse_take` is kept as the single deferral marker), the group-by tail beyond `groupby_count`/`groupby_sum` (table group_by fusion is staged — see `LINQ_TO_TABLE.md`; the two marker cells track the tier-2 cost until then) plus the join-composition lanes (`join_select` / `where_join_count` would fuse today but aren't instantiated; `join_groupby_*` needs the staged group_by), `decs_count_bare_pred` (decs-only).
- **`point_lookup` / `point_lookup_scan` non-m7** — m7-only pair: only a table source has a key to probe (`where(kv.key == X)` + terminator → `key_exists` / `tab?[X]`, O(1)); the `_scan` twin forces the same query through the walk (compound `&&` predicate declines the probe) to show the gap. Other sources have no analog by design.
- **`join_probe` / `join_probe_build` non-m7** — m7-only A/B pair: a table srcB joined on its bare key probes the user's table per lead row (no internal join hash, no build loop); the `_build` twin feeds the identical rows pre-materialized to a kv array, forcing the hashed build. Other sources have no keyed-srcB analog by design.

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
