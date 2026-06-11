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
  values-only / zipped slot walks; group_by / join / reverse defer to tier-2 until their stages land).

`0.00` = early-exit terminator below timer resolution ("free"). Chain shapes are in
`benchmarks/README.md`; the splice arms each fires are in `doc/source/reference/linq_fold_patterns.rst`.

**JIT on Apple silicon is high-variance run-to-run — read the INTERP column as the reliable
signal, JIT deltas as indicative.**

<!-- BENCH:TABLES BEGIN -->
*Generated 2026-06-10 by `benchmarks/sql/_update_results.das` — ns/op; `—` = absent lane. Edit the prose around the markers, not the tables.*

## INTERP

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) | Table fold (m7) |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 34.7 | 5.9 | 5.8 | 60.1 | 152.3 | 19.0 |
| `all_match` | 27.3 | 3.5 | 3.4 | 55.6 | 147.0 | 15.8 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 29.8 | 5.9 | 8.8 | 58.3 | 156.2 | 17.2 |
| `bare_last` | — | 4.2 | 0.0 | 0.0 | 4.2 | 29.2 |
| `bare_order_where` | 277.1 | 118.1 | 126.8 | 300.9 | 292.2 | 166.4 |
| `chained_select_collapse` | — | 17.7 | 17.4 | 70.1 | 155.4 | 27.8 |
| `chained_where` | 35.8 | 6.6 | 7.1 | 104.2 | 174.7 | 24.1 |
| `contains_match` | 0.0 | 2.2 | 1.4 | 27.5 | 68.5 | 6.6 |
| `count_aggregate` | 29.2 | 4.1 | 4.1 | 63.4 | 147.5 | 20.2 |
| `cross_join` | 13122.7 | 3685.9 | — | 3995.6 | 4066.2 | — |
| `decs_count_bare_pred` | — | — | 4.1 | — | — | — |
| `distinct_by_count` | 40.8 | 15.6 | 15.6 | 70.2 | 154.0 | 26.4 |
| `distinct_by_order_take` | 240.7 | 22.1 | 23.4 | 122.7 | 161.6 | 48.5 |
| `distinct_by_order_to_array` | 239.2 | 22.2 | 23.5 | 123.6 | 161.7 | 48.4 |
| `distinct_count` | 40.7 | 15.9 | 15.7 | 70.5 | 155.8 | 26.9 |
| `distinct_count_pred` | 251.0 | 16.1 | 15.8 | 111.5 | 178.0 | 26.3 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.4 | 0.3 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 173.3 | 29.3 | 29.3 | 122.9 | 190.0 | — |
| `groupby_count` | 143.5 | 19.4 | 19.4 | 75.4 | 161.0 | 162.6 |
| `groupby_first` | 251.7 | 19.5 | 20.1 | 72.1 | 156.9 | — |
| `groupby_having_count` | 140.7 | 19.5 | 19.5 | 74.7 | 161.2 | — |
| `groupby_having_hidden_sum` | 176.1 | 22.5 | 22.6 | 118.0 | 183.5 | — |
| `groupby_having_post_where` | 172.8 | 20.8 | 20.8 | 114.1 | 180.4 | — |
| `groupby_max` | 173.5 | 24.8 | 25.3 | 119.7 | 185.2 | — |
| `groupby_min` | 173.8 | 25.2 | 25.1 | 119.8 | 184.7 | — |
| `groupby_multi_reducer` | 189.5 | 30.5 | 30.6 | 124.3 | 188.4 | — |
| `groupby_select_order` | 169.9 | 20.8 | 20.8 | 114.3 | 180.9 | — |
| `groupby_select_sum` | 196.9 | 38.6 | 38.1 | 101.6 | 186.6 | — |
| `groupby_sum` | 170.5 | 21.2 | 20.8 | 114.4 | 180.2 | 192.8 |
| `groupby_where_count` | 75.6 | 14.1 | 14.3 | 115.2 | 177.8 | — |
| `groupby_where_sum` | 86.4 | 14.1 | 14.6 | 116.2 | 178.1 | — |
| `join_count` | 38.0 | 51.2 | 64.2 | 112.7 | 176.9 | 195.0 |
| `join_groupby_count` | 157.7 | 86.1 | 88.2 | 177.4 | 221.8 | — |
| `join_groupby_to_array` | 194.9 | 80.3 | 91.7 | 214.8 | 212.1 | — |
| `join_select` | 150.3 | 72.4 | 84.4 | 187.8 | 209.0 | — |
| `join_where_count` | 39.0 | 61.6 | 76.7 | 159.8 | 193.6 | 229.1 |
| `last_match` | 0.0 | 5.9 | 13.9 | 64.9 | 152.3 | 31.0 |
| `long_count_aggregate` | 28.7 | 4.1 | 4.1 | 63.3 | 147.5 | 20.3 |
| `max_aggregate` | 30.6 | 6.0 | 6.8 | 58.4 | 156.1 | 17.0 |
| `min_aggregate` | 30.5 | 6.0 | 6.8 | 58.4 | 155.1 | 17.0 |
| `order_by_multi_key` | 338.7 | 272.3 | 286.1 | 457.7 | 448.2 | 333.0 |
| `order_distinct_take` | 138.4 | 15.9 | 99.2 | 72.4 | 156.5 | 31.0 |
| `order_reverse_normalized` | 37.9 | 16.3 | 20.0 | 70.4 | 162.9 | — |
| `order_take_desc` | 37.8 | 16.3 | 20.3 | 69.8 | 163.3 | 33.2 |
| `reverse_distinct_by` | 294.1 | 21.2 | 28.0 | 70.8 | 155.4 | — |
| `reverse_take` | 0.1 | 0.0 | 0.2 | 0.0 | 26.1 | 58.7 |
| `reverse_take_select` | 0.0 | 0.0 | 0.2 | 0.0 | 26.1 | — |
| `select_count` | 0.1 | 0.0 | 2.2 | 64.8 | 2.2 | 0.0 |
| `select_many` | — | 191.0 | — | — | — | — |
| `select_where` | 194.7 | 11.5 | 19.3 | 195.9 | 185.7 | 37.5 |
| `select_where_count` | 32.3 | 5.1 | 7.4 | 64.6 | 150.7 | 21.8 |
| `select_where_order_take` | 36.2 | 12.2 | 15.0 | 72.3 | 158.5 | 34.4 |
| `select_where_sum` | 37.1 | 7.5 | 7.5 | 66.3 | 160.5 | 23.2 |
| `single_match` | 0.0 | 2.9 | 5.5 | 56.9 | 151.1 | 22.8 |
| `skip_take` | 0.5 | 0.1 | 0.2 | 3.0 | 2.8 | 0.3 |
| `skip_while_match` | 3.5 | 5.3 | 5.3 | 57.3 | 146.6 | 18.2 |
| `sort_first` | 37.6 | 11.1 | 13.3 | 64.6 | 159.5 | 31.7 |
| `sort_take` | 38.0 | 16.2 | 20.9 | 70.2 | 161.9 | 33.0 |
| `sort_take_select` | 37.6 | 16.3 | 20.9 | 70.8 | 162.7 | 33.3 |
| `sum_aggregate` | 29.7 | 2.1 | 2.1 | 54.3 | 146.7 | 13.4 |
| `sum_where` | 31.9 | 4.3 | 4.3 | 63.6 | 148.1 | 20.5 |
| `take_count` | 3.6 | 0.2 | 0.4 | 2.9 | 2.7 | 0.5 |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 1.3 | 1.1 | 0.3 |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 0.6 | 0.5 | 0.1 |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 0.7 | 0.6 | 0.2 |
| `take_while_match` | 7.8 | 2.4 | 2.4 | 28.8 | 71.4 | 16.8 |
| `to_array_filter` | 70.3 | 11.8 | 11.7 | 71.1 | 157.4 | 28.8 |
| `where_join_count` | 41.0 | 29.0 | 41.5 | 133.0 | 163.1 | — |
| `zip_count_pred` | 39.0 | 15.8 | — | 313.5 | 319.6 | — |
| `zip_dot_product` | 46.1 | 12.6 | 10.5 | 308.6 | 317.2 | — |
| `zip_dot_product_3arg` | 46.1 | 12.8 | — | 308.7 | 316.5 | — |
| `zip_reverse_to_array` | — | 31.6 | — | 343.1 | 351.0 | — |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) | Table fold (m7) |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 35.0 | 0.3 | 0.6 | 21.7 | 27.3 | 13.6 |
| `all_match` | 27.8 | 0.3 | 0.2 | 18.1 | 25.9 | 13.5 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 29.9 | 1.0 | 3.6 | 18.0 | 24.4 | 13.4 |
| `bare_last` | — | 0.4 | 0.0 | 0.0 | 0.0 | 17.1 |
| `bare_order_where` | 186.2 | 34.0 | 35.3 | 106.3 | 52.4 | 78.7 |
| `chained_select_collapse` | — | 1.1 | 1.1 | 20.4 | 33.0 | 14.0 |
| `chained_where` | 35.9 | 0.6 | 0.8 | 35.5 | 31.5 | 17.6 |
| `contains_match` | 0.0 | 0.2 | 0.1 | 14.8 | 9.2 | 4.7 |
| `count_aggregate` | 29.5 | 0.3 | 0.6 | 20.4 | 25.1 | 13.4 |
| `cross_join` | 5964.4 | 734.4 | — | 834.2 | 772.7 | — |
| `decs_count_bare_pred` | — | — | 0.6 | — | — | — |
| `distinct_by_count` | 41.0 | 1.1 | 1.1 | 20.4 | 32.0 | 14.0 |
| `distinct_by_order_take` | 237.4 | 1.7 | 2.6 | 48.4 | 37.1 | 29.9 |
| `distinct_by_order_to_array` | 237.2 | 1.7 | 2.7 | 47.5 | 36.8 | 30.0 |
| `distinct_count` | 40.8 | 1.1 | 1.1 | 20.5 | 31.9 | 14.0 |
| `distinct_count_pred` | 249.8 | 1.1 | 1.3 | 37.6 | 41.7 | 14.0 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.1 | 0.0 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 170.1 | 1.5 | 1.9 | 35.7 | 43.0 | — |
| `groupby_count` | 141.1 | 1.3 | 1.5 | 20.5 | 32.2 | 43.0 |
| `groupby_first` | 251.0 | 1.3 | 2.3 | 20.5 | 32.9 | — |
| `groupby_having_count` | 141.1 | 1.3 | 1.5 | 20.5 | 32.1 | — |
| `groupby_having_hidden_sum` | 173.9 | 1.5 | 1.7 | 35.8 | 42.7 | — |
| `groupby_having_post_where` | 170.2 | 1.4 | 1.9 | 35.8 | 41.8 | — |
| `groupby_max` | 172.3 | 1.5 | 1.9 | 35.9 | 43.6 | — |
| `groupby_min` | 173.0 | 1.5 | 1.8 | 35.8 | 43.6 | — |
| `groupby_multi_reducer` | 191.8 | 1.6 | 1.9 | 36.1 | 43.7 | — |
| `groupby_select_order` | 170.5 | 1.4 | 1.9 | 35.8 | 42.0 | — |
| `groupby_select_sum` | 195.5 | 2.8 | 3.2 | 32.3 | 37.6 | — |
| `groupby_sum` | 169.8 | 1.4 | 1.6 | 35.8 | 42.0 | 51.2 |
| `groupby_where_count` | 75.7 | 0.9 | 1.3 | 35.9 | 39.7 | — |
| `groupby_where_sum` | 86.4 | 0.9 | 1.3 | 35.9 | 39.6 | — |
| `join_count` | 37.9 | 11.0 | 11.7 | 43.4 | 68.3 | 62.9 |
| `join_groupby_count` | 156.2 | 18.2 | 20.0 | 68.3 | 86.7 | — |
| `join_groupby_to_array` | 189.2 | 17.5 | 19.4 | 80.2 | 36.1 | — |
| `join_select` | 92.8 | 19.6 | 21.6 | 74.4 | 94.1 | — |
| `join_where_count` | 39.1 | 18.9 | 20.6 | 64.5 | 77.9 | 80.0 |
| `last_match` | 0.0 | 0.5 | 1.4 | 18.6 | 25.9 | 22.9 |
| `long_count_aggregate` | 28.7 | 0.3 | 0.6 | 20.4 | 26.6 | 13.4 |
| `max_aggregate` | 30.6 | 0.3 | 0.5 | 18.1 | 26.7 | 13.4 |
| `min_aggregate` | 30.6 | 0.3 | 0.5 | 18.2 | 26.3 | 13.4 |
| `order_by_multi_key` | 247.0 | 53.4 | 54.8 | 125.3 | 70.3 | 128.9 |
| `order_distinct_take` | 137.9 | 1.1 | 75.6 | 20.9 | 34.1 | 14.0 |
| `order_reverse_normalized` | 37.8 | 0.7 | 1.3 | 24.6 | 27.0 | — |
| `order_take_desc` | 38.0 | 0.7 | 1.3 | 24.5 | 26.9 | 17.7 |
| `reverse_distinct_by` | 295.4 | 1.5 | 3.2 | 20.4 | 32.7 | — |
| `reverse_take` | 0.0 | 0.0 | 0.0 | 0.0 | 3.7 | 26.8 |
| `reverse_take_select` | 0.0 | 0.0 | 0.0 | 0.0 | 3.8 | — |
| `select_count` | 0.1 | 0.0 | 0.0 | 63.4 | 0.0 | 0.0 |
| `select_many` | — | 61.5 | — | — | — | — |
| `select_where` | 110.5 | 4.3 | 5.3 | 76.1 | 22.1 | 27.9 |
| `select_where_count` | 32.1 | 0.3 | 0.6 | 18.4 | 25.9 | 13.3 |
| `select_where_order_take` | 36.3 | 0.7 | 1.4 | 18.9 | 26.6 | 22.9 |
| `select_where_sum` | 37.0 | 0.4 | 0.6 | 17.9 | 24.9 | 13.3 |
| `single_match` | 0.0 | 0.4 | 1.1 | 43.4 | 22.2 | 17.2 |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.3 | 0.2 | 0.1 |
| `skip_while_match` | 3.5 | 0.4 | 0.4 | 43.5 | 21.8 | 13.2 |
| `sort_first` | 37.7 | 0.4 | 1.4 | 17.9 | 26.1 | 17.1 |
| `sort_take` | 38.0 | 0.7 | 1.5 | 24.5 | 26.8 | 17.7 |
| `sort_take_select` | 37.8 | 0.7 | 1.3 | 24.5 | 26.9 | 17.7 |
| `sum_aggregate` | 29.6 | 0.3 | 0.1 | 23.3 | 24.3 | 13.4 |
| `sum_where` | 32.1 | 0.3 | 0.6 | 18.4 | 25.9 | 13.3 |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.2 | 0.3 | 0.4 |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 0.4 | 0.1 | 0.2 |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 0.2 | 0.0 | 0.1 |
| `take_where_count` | 0.8 | 0.0 | 0.0 | 0.2 | 0.0 | 0.1 |
| `take_while_match` | 7.8 | 0.2 | 0.3 | 14.7 | 9.0 | 13.3 |
| `to_array_filter` | 47.1 | 3.3 | 3.3 | 21.3 | 33.6 | 20.0 |
| `where_join_count` | 39.0 | 5.8 | 6.7 | 49.5 | 40.6 | — |
| `zip_count_pred` | 39.1 | 0.1 | — | 116.7 | 33.5 | — |
| `zip_dot_product` | 46.3 | 0.1 | 0.1 | 116.6 | 33.4 | — |
| `zip_dot_product_3arg` | 46.1 | 0.1 | — | 116.5 | 33.4 | — |
| `zip_reverse_to_array` | — | 4.6 | — | 127.7 | 50.0 | — |
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
