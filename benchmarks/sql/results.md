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
  `point_lookup` / `point_lookup_scan` pair measures it; group_by / join / reverse defer to tier-2
  until their stages land).

`0.00` = early-exit terminator below timer resolution ("free"). Chain shapes are in
`benchmarks/README.md`; the splice arms each fires are in `doc/source/reference/linq_fold_patterns.rst`.

**JIT on Apple silicon is high-variance run-to-run — read the INTERP column as the reliable
signal, JIT deltas as indicative.**

<!-- BENCH:TABLES BEGIN -->
*Generated 2026-06-11 by `benchmarks/sql/_update_results.das` — ns/op; `—` = absent lane. Edit the prose around the markers, not the tables.*

## INTERP

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) | Table fold (m7) |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 34.9 | 5.9 | 5.8 | 60.7 | 160.3 | 19.1 |
| `all_match` | 27.5 | 3.5 | 3.4 | 55.9 | 154.1 | 15.8 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.5 | 5.9 | 8.8 | 60.2 | 163.1 | 17.3 |
| `bare_last` | — | 4.2 | 0.0 | 0.0 | 4.2 | 29.2 |
| `bare_order_where` | 278.2 | 117.7 | 126.7 | 299.6 | 292.7 | 166.4 |
| `chained_select_collapse` | — | 17.7 | 17.4 | 70.4 | 168.3 | 27.8 |
| `chained_where` | 35.9 | 6.6 | 7.1 | 104.9 | 184.0 | 24.1 |
| `contains_match` | 0.0 | 2.3 | 1.5 | 29.1 | 72.4 | 6.6 |
| `count_aggregate` | 30.0 | 4.1 | 4.2 | 63.7 | 155.2 | 20.2 |
| `cross_join` | 12604.3 | 3685.2 | — | 4006.6 | 4040.5 | — |
| `decs_count_bare_pred` | — | — | 4.1 | — | — | — |
| `distinct_by_count` | 40.9 | 15.6 | 15.6 | 70.6 | 162.2 | 26.3 |
| `distinct_by_order_take` | 239.3 | 22.1 | 23.4 | 123.3 | 162.4 | 48.6 |
| `distinct_by_order_to_array` | 237.8 | 22.1 | 23.5 | 124.1 | 163.3 | 48.6 |
| `distinct_count` | 41.2 | 15.8 | 15.7 | 70.8 | 163.6 | 26.9 |
| `distinct_count_pred` | 252.2 | 15.7 | 15.9 | 112.1 | 178.4 | 26.3 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.4 | 0.3 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 170.5 | 29.3 | 29.3 | 122.7 | 197.8 | — |
| `groupby_count` | 141.6 | 19.5 | 19.4 | 74.7 | 169.0 | 163.3 |
| `groupby_first` | 252.3 | 19.4 | 20.1 | 71.8 | 163.5 | — |
| `groupby_having_count` | 141.3 | 19.5 | 19.5 | 74.3 | 169.3 | — |
| `groupby_having_hidden_sum` | 175.7 | 22.4 | 22.6 | 118.5 | 192.1 | — |
| `groupby_having_post_where` | 171.6 | 20.8 | 21.6 | 114.8 | 188.9 | — |
| `groupby_max` | 174.1 | 24.7 | 25.6 | 119.8 | 192.6 | — |
| `groupby_min` | 173.5 | 25.1 | 26.2 | 119.9 | 193.4 | — |
| `groupby_multi_reducer` | 189.9 | 30.2 | 30.6 | 125.1 | 196.0 | — |
| `groupby_select_order` | 174.3 | 20.8 | 20.8 | 114.6 | 189.8 | — |
| `groupby_select_sum` | 197.9 | 38.5 | 40.7 | 101.5 | 196.1 | — |
| `groupby_sum` | 171.2 | 20.7 | 20.8 | 115.0 | 190.5 | 192.9 |
| `groupby_where_count` | 75.7 | 14.0 | 14.3 | 115.5 | 187.7 | — |
| `groupby_where_sum` | 86.5 | 14.1 | 14.7 | 116.3 | 186.7 | — |
| `join_count` | 38.3 | 51.2 | 64.3 | 113.1 | 184.5 | 194.6 |
| `join_groupby_count` | 157.7 | 79.1 | 88.6 | 177.7 | 232.0 | — |
| `join_groupby_to_array` | 189.0 | 78.1 | 90.1 | 215.3 | 215.6 | — |
| `join_select` | 151.5 | 72.6 | 85.0 | 188.5 | 215.8 | — |
| `join_where_count` | 48.8 | 61.5 | 76.7 | 160.0 | 201.9 | 229.1 |
| `last_match` | 0.0 | 5.9 | 13.9 | 65.1 | 159.0 | 30.9 |
| `long_count_aggregate` | 28.9 | 4.1 | 4.2 | 63.3 | 154.6 | 20.3 |
| `max_aggregate` | 30.7 | 6.0 | 6.9 | 58.7 | 163.1 | 17.0 |
| `min_aggregate` | 30.6 | 6.0 | 6.9 | 58.6 | 163.3 | 17.1 |
| `order_by_multi_key` | 339.9 | 271.4 | 283.6 | 458.8 | 446.1 | 334.3 |
| `order_distinct_take` | 137.9 | 15.9 | 100.3 | 72.5 | 164.1 | 31.1 |
| `order_reverse_normalized` | 38.3 | 16.2 | 20.3 | 70.7 | 170.9 | — |
| `order_take_desc` | 38.2 | 16.2 | 20.6 | 70.1 | 170.2 | 33.3 |
| `point_lookup` | — | — | — | — | — | 0.0 |
| `point_lookup_scan` | — | — | — | — | — | 8.4 |
| `reverse_distinct_by` | 294.0 | 21.1 | 28.1 | 71.1 | 162.6 | — |
| `reverse_take` | 0.1 | 0.0 | 0.2 | 0.0 | 26.1 | 58.9 |
| `reverse_take_select` | 0.0 | 0.0 | 0.2 | 0.0 | 26.2 | — |
| `select_count` | 0.1 | 0.0 | 2.2 | 68.3 | 2.2 | 0.0 |
| `select_many` | — | 191.5 | — | — | — | — |
| `select_where` | 197.5 | 11.2 | 19.4 | 195.6 | 183.7 | 37.5 |
| `select_where_count` | 32.2 | 5.1 | 7.5 | 64.8 | 157.1 | 21.9 |
| `select_where_order_take` | 36.2 | 12.2 | 15.1 | 72.5 | 165.1 | 34.5 |
| `select_where_sum` | 37.1 | 7.5 | 7.5 | 66.4 | 162.2 | 23.3 |
| `single_match` | 0.0 | 2.9 | 5.5 | 58.5 | 151.1 | 22.8 |
| `skip_take` | 0.5 | 0.1 | 0.2 | 3.0 | 2.8 | 0.3 |
| `skip_while_match` | 3.5 | 5.3 | 5.3 | 60.2 | 153.8 | 18.3 |
| `sort_first` | 38.0 | 11.1 | 13.3 | 65.0 | 167.1 | 31.7 |
| `sort_take` | 38.2 | 16.3 | 21.1 | 70.2 | 170.7 | 33.2 |
| `sort_take_select` | 38.1 | 16.3 | 21.8 | 71.1 | 170.6 | 33.3 |
| `sum_aggregate` | 30.6 | 2.1 | 2.1 | 54.8 | 152.8 | 13.5 |
| `sum_where` | 32.9 | 4.4 | 4.3 | 63.4 | 154.2 | 20.6 |
| `take_count` | 3.6 | 0.2 | 0.4 | 2.9 | 2.7 | 0.5 |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 1.3 | 1.1 | 0.3 |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 0.6 | 0.5 | 0.1 |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 0.7 | 0.6 | 0.2 |
| `take_while_match` | 7.8 | 2.4 | 2.4 | 30.3 | 75.4 | 16.5 |
| `to_array_filter` | 70.0 | 11.8 | 11.7 | 71.3 | 164.9 | 28.7 |
| `where_join_count` | 41.2 | 29.0 | 42.0 | 132.1 | 168.9 | — |
| `zip_count_pred` | 39.2 | 15.9 | — | 313.8 | 322.0 | — |
| `zip_dot_product` | 46.1 | 12.6 | 10.6 | 308.6 | 319.3 | — |
| `zip_dot_product_3arg` | 46.1 | 12.8 | — | 309.7 | 319.0 | — |
| `zip_reverse_to_array` | — | 31.6 | — | 343.4 | 353.5 | — |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) | Table fold (m7) |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 35.1 | 0.3 | 0.6 | 21.8 | 26.0 | 13.4 |
| `all_match` | 27.8 | 0.3 | 0.2 | 18.1 | 25.2 | 13.5 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.1 | 1.0 | 3.6 | 18.1 | 24.6 | 13.5 |
| `bare_last` | — | 0.4 | 0.0 | 0.0 | 0.0 | 17.1 |
| `bare_order_where` | 185.6 | 34.0 | 35.2 | 106.5 | 53.5 | 78.9 |
| `chained_select_collapse` | — | 1.1 | 1.1 | 20.6 | 33.4 | 14.0 |
| `chained_where` | 36.2 | 0.6 | 0.8 | 35.6 | 31.4 | 17.7 |
| `contains_match` | 0.0 | 0.2 | 0.1 | 17.5 | 9.0 | 4.7 |
| `count_aggregate` | 29.3 | 0.3 | 0.6 | 20.5 | 25.3 | 13.5 |
| `cross_join` | 5962.8 | 733.1 | — | 836.0 | 773.4 | — |
| `decs_count_bare_pred` | — | — | 0.6 | — | — | — |
| `distinct_by_count` | 41.2 | 1.1 | 1.1 | 20.6 | 33.3 | 14.0 |
| `distinct_by_order_take` | 237.1 | 1.7 | 2.6 | 47.4 | 39.1 | 30.3 |
| `distinct_by_order_to_array` | 242.4 | 1.8 | 2.6 | 47.4 | 38.7 | 30.3 |
| `distinct_count` | 40.9 | 1.1 | 1.1 | 20.6 | 33.3 | 14.0 |
| `distinct_count_pred` | 250.6 | 1.1 | 1.3 | 37.7 | 43.5 | 14.0 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.2 | 0.0 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 171.1 | 1.6 | 1.9 | 35.9 | 45.7 | — |
| `groupby_count` | 141.4 | 1.3 | 1.5 | 20.6 | 33.9 | 45.7 |
| `groupby_first` | 250.7 | 1.3 | 2.3 | 20.6 | 34.3 | — |
| `groupby_having_count` | 141.5 | 1.3 | 1.5 | 20.6 | 33.8 | — |
| `groupby_having_hidden_sum` | 174.4 | 1.5 | 1.7 | 35.9 | 45.3 | — |
| `groupby_having_post_where` | 170.1 | 1.4 | 2.0 | 35.8 | 44.2 | — |
| `groupby_max` | 175.6 | 1.5 | 2.0 | 36.0 | 46.0 | — |
| `groupby_min` | 172.4 | 1.5 | 1.8 | 36.0 | 46.0 | — |
| `groupby_multi_reducer` | 189.6 | 1.6 | 2.0 | 36.1 | 46.1 | — |
| `groupby_select_order` | 170.1 | 1.4 | 1.9 | 35.9 | 44.3 | — |
| `groupby_select_sum` | 197.0 | 2.8 | 3.2 | 32.2 | 40.0 | — |
| `groupby_sum` | 170.5 | 1.4 | 1.6 | 35.9 | 43.4 | 54.2 |
| `groupby_where_count` | 75.6 | 0.9 | 1.3 | 36.0 | 41.7 | — |
| `groupby_where_sum` | 86.2 | 0.9 | 1.3 | 35.9 | 41.7 | — |
| `join_count` | 38.2 | 11.0 | 11.7 | 43.6 | 71.4 | 63.1 |
| `join_groupby_count` | 156.8 | 18.0 | 20.1 | 68.5 | 90.1 | — |
| `join_groupby_to_array` | 189.5 | 17.4 | 19.4 | 80.5 | 36.0 | — |
| `join_select` | 93.2 | 19.6 | 21.7 | 74.8 | 94.5 | — |
| `join_where_count` | 48.3 | 19.0 | 20.7 | 64.5 | 78.3 | 80.0 |
| `last_match` | 0.0 | 0.5 | 1.4 | 18.8 | 25.9 | 22.9 |
| `long_count_aggregate` | 28.8 | 0.3 | 0.6 | 20.6 | 25.4 | 13.5 |
| `max_aggregate` | 30.5 | 0.3 | 0.5 | 18.3 | 26.7 | 13.4 |
| `min_aggregate` | 30.6 | 0.3 | 0.5 | 18.3 | 26.6 | 13.5 |
| `order_by_multi_key` | 249.4 | 53.4 | 54.8 | 125.6 | 71.1 | 129.8 |
| `order_distinct_take` | 138.1 | 1.1 | 75.6 | 20.9 | 35.8 | 14.0 |
| `order_reverse_normalized` | 38.0 | 0.7 | 1.4 | 24.6 | 27.6 | — |
| `order_take_desc` | 37.9 | 0.7 | 1.3 | 24.6 | 27.9 | 17.8 |
| `point_lookup` | — | — | — | — | — | 0.0 |
| `point_lookup_scan` | — | — | — | — | — | 6.1 |
| `reverse_distinct_by` | 295.6 | 1.6 | 3.2 | 20.6 | 34.3 | — |
| `reverse_take` | 0.0 | 0.0 | 0.0 | 0.0 | 3.7 | 26.9 |
| `reverse_take_select` | 0.0 | 0.0 | 0.0 | 0.0 | 3.7 | — |
| `select_count` | 0.1 | 0.0 | 0.0 | 68.7 | 0.0 | 0.0 |
| `select_many` | — | 64.0 | — | — | — | — |
| `select_where` | 110.6 | 4.2 | 5.3 | 76.5 | 22.0 | 28.1 |
| `select_where_count` | 32.3 | 0.3 | 0.6 | 18.6 | 26.7 | 13.5 |
| `select_where_order_take` | 37.1 | 0.7 | 1.4 | 19.1 | 27.4 | 23.0 |
| `select_where_sum` | 36.9 | 0.4 | 0.6 | 18.2 | 25.2 | 13.4 |
| `single_match` | 0.0 | 0.4 | 1.1 | 46.3 | 22.2 | 17.3 |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.3 | 0.2 | 0.2 |
| `skip_while_match` | 3.5 | 0.4 | 0.4 | 46.7 | 21.7 | 13.3 |
| `sort_first` | 38.3 | 0.4 | 1.3 | 18.2 | 26.7 | 17.3 |
| `sort_take` | 38.2 | 0.7 | 1.4 | 24.7 | 27.8 | 17.8 |
| `sort_take_select` | 37.6 | 0.7 | 1.4 | 24.7 | 27.8 | 17.8 |
| `sum_aggregate` | 29.3 | 0.3 | 0.1 | 23.4 | 24.6 | 13.5 |
| `sum_where` | 31.8 | 0.3 | 0.6 | 18.6 | 26.4 | 13.4 |
| `take_count` | 1.9 | 0.1 | 0.1 | 1.2 | 0.2 | 0.2 |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 0.4 | 0.1 | 0.2 |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 0.2 | 0.0 | 0.1 |
| `take_where_count` | 0.9 | 0.0 | 0.0 | 0.2 | 0.0 | 0.1 |
| `take_while_match` | 7.7 | 0.2 | 0.3 | 17.3 | 9.0 | 13.4 |
| `to_array_filter` | 48.2 | 3.2 | 3.3 | 21.6 | 35.0 | 20.4 |
| `where_join_count` | 41.2 | 5.8 | 6.7 | 49.6 | 41.9 | — |
| `zip_count_pred` | 38.6 | 0.1 | — | 117.0 | 33.9 | — |
| `zip_dot_product` | 46.0 | 0.1 | 0.1 | 116.8 | 33.8 | — |
| `zip_dot_product_3arg` | 45.9 | 0.1 | — | 116.8 | 33.7 | — |
| `zip_reverse_to_array` | — | 4.6 | — | 128.3 | 51.4 | — |
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
- **m7 absent families** — `zip_*` / `cross_join` (lockstep over an unordered slot walk is meaningless), `select_many` (flat fixture, no nested array field), `order_reverse_normalized` / `reverse_take_select` / `reverse_distinct_by` (no backward slot walk; `reverse_take` is kept as the single deferral marker), the group-by tail beyond `groupby_count`/`groupby_sum` and joins beyond `join_count`/`join_where_count` (table group_by/join fusion is staged — see `LINQ_TO_TABLE.md`; the four marker cells track the tier-2 cost until then), `decs_count_bare_pred` (decs-only).
- **`point_lookup` / `point_lookup_scan` non-m7** — m7-only pair: only a table source has a key to probe (`where(kv.key == X)` + terminator → `key_exists` / `tab?[X]`, O(1)); the `_scan` twin forces the same query through the walk (compound `&&` predicate declines the probe) to show the gap. Other sources have no analog by design.

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
