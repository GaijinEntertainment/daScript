# Benchmarks — SQL / Array / Decs / XML / JSON comparison

Five lanes run the same query families over one `Car` schema (n = 100 000 cars, 100 dealers,
5 brands); cells are ns/op, `—` = intentionally absent lane (see "Missing lanes"). The tables
between the `BENCH:TABLES` markers are machine-generated (see "How to re-run"); all other text
is hand-edited.

Each lane lives in its own file (`array.das` / `decs.das` / `xml.das` / `json.das` / `sql.das`) with
the source fixture built once in `[init]`; the sweep runs one process per file, so a lane is never
contaminated by another lane's code in the same process (this is why JIT cells are stable now).

- **m1 SQL** — `_fold(db |> select_from(type<Car>) |> …)` over in-memory SQLite; `_fold` passes the chain to `_sql`.
- **m3f Array** — `_fold` over `each(array<Car>)`.
- **m4 Decs** — `_fold` over `from_decs_template(type<DecsCar>)` (per-archetype walk).
- **m5f XML** — `_fold` over `from_xml_node(root, type<Car>)` (`XmlAdapter` fuses + field-prunes).
- **m6f JSON** — `_fold` over `from_json(jv, type<Car>)` (`JsonAdapter`, same machinery, array walk).

`0.00` = early-exit terminator below timer resolution ("free"). Chain shapes are in
`benchmarks/README.md`; the splice arms each fires are in `doc/source/reference/linq_fold_patterns.rst`.

**JIT on Apple silicon is high-variance run-to-run — read the INTERP column as the reliable
signal, JIT deltas as indicative.**

<!-- BENCH:TABLES BEGIN -->
*Generated 2026-06-06 by `benchmarks/sql/_update_results.das` — ns/op; `—` = absent lane. Edit the prose around the markers, not the tables.*

## INTERP

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) |
|---|---:|---:|---:|---:|---:|
| `aggregate_match` | 35.1 | 5.9 | 5.9 | 60.9 | 159.9 |
| `all_match` | 28.0 | 3.5 | 3.4 | 56.6 | 154.0 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.7 | 5.9 | 8.8 | 60.4 | 164.6 |
| `bare_last` | — | 4.2 | 0.0 | 0.0 | 4.2 |
| `bare_order_where` | 283.1 | 116.2 | 128.0 | 304.3 | 288.6 |
| `chained_select_collapse` | — | 17.8 | 17.8 | 70.4 | 162.7 |
| `chained_where` | 37.0 | 6.6 | 7.1 | 104.8 | 185.2 |
| `contains_match` | 0.0 | 2.3 | 1.5 | 29.1 | 73.0 |
| `count_aggregate` | 29.9 | 4.1 | 4.2 | 64.1 | 154.6 |
| `cross_join` | 12610.5 | 3738.5 | — | 4039.6 | 4042.5 |
| `decs_count_bare_pred` | — | — | 4.2 | — | — |
| `distinct_by_count` | 41.7 | 16.1 | 16.0 | 70.7 | 163.5 |
| `distinct_by_order_take` | 240.1 | 22.0 | 23.3 | 123.9 | 161.9 |
| `distinct_by_order_to_array` | 242.0 | 22.3 | 23.4 | 124.4 | 162.7 |
| `distinct_count` | 41.6 | 15.8 | 15.8 | 71.2 | 161.8 |
| `distinct_count_pred` | 252.2 | 15.8 | 15.9 | 112.2 | 178.7 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.4 | 0.3 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 173.2 | 29.5 | 29.4 | 122.6 | 195.2 |
| `groupby_count` | 142.1 | 19.2 | 19.2 | 74.5 | 167.8 |
| `groupby_first` | 252.4 | 19.1 | 19.8 | 71.7 | 163.3 |
| `groupby_having_count` | 141.9 | 19.1 | 19.2 | 74.2 | 167.7 |
| `groupby_having_hidden_sum` | 176.5 | 22.6 | 22.8 | 118.4 | 192.1 |
| `groupby_having_post_where` | 171.4 | 19.6 | 19.2 | 114.6 | 188.3 |
| `groupby_max` | 174.1 | 25.1 | 25.0 | 120.0 | 193.1 |
| `groupby_min` | 173.7 | 25.2 | 24.9 | 120.1 | 193.1 |
| `groupby_multi_reducer` | 190.3 | 30.9 | 30.6 | 124.8 | 196.7 |
| `groupby_select_order` | 171.8 | 19.2 | 19.1 | 115.1 | 189.8 |
| `groupby_select_sum` | 201.8 | 38.5 | 37.9 | 102.8 | 195.0 |
| `groupby_sum` | 172.7 | 19.1 | 19.1 | 115.3 | 188.2 |
| `groupby_where_count` | 76.3 | 13.9 | 14.2 | 116.0 | 186.3 |
| `groupby_where_sum` | 87.7 | 14.0 | 14.5 | 116.3 | 186.7 |
| `join_count` | 38.7 | 51.5 | 64.2 | 113.9 | 183.3 |
| `join_groupby_count` | 156.9 | 77.3 | 89.9 | 178.0 | 230.6 |
| `join_groupby_to_array` | 190.3 | 78.4 | 90.8 | 215.4 | 212.7 |
| `join_select` | 150.2 | 72.6 | 85.0 | 189.2 | 215.2 |
| `join_where_count` | 40.1 | 61.4 | 76.7 | 162.3 | 199.0 |
| `last_match` | 0.0 | 5.8 | 13.8 | 65.5 | 160.3 |
| `long_count_aggregate` | 29.9 | 4.2 | 4.1 | 64.0 | 154.6 |
| `max_aggregate` | 31.5 | 6.0 | 6.7 | 58.9 | 163.7 |
| `min_aggregate` | 31.2 | 6.0 | 6.8 | 59.2 | 162.9 |
| `order_by_multi_key` | 345.5 | 281.8 | 285.5 | 460.6 | 445.5 |
| `order_distinct_take` | 138.4 | 15.7 | 100.4 | 72.9 | 163.4 |
| `order_reverse_normalized` | 38.6 | 16.3 | 20.0 | 70.2 | 170.3 |
| `order_take_desc` | 38.5 | 16.2 | 20.0 | 70.1 | 171.7 |
| `reverse_distinct_by` | 296.5 | 21.3 | 27.6 | 70.9 | 162.9 |
| `reverse_take` | 0.1 | 0.0 | 0.2 | 0.0 | 26.0 |
| `reverse_take_select` | 0.0 | 0.0 | 0.2 | 0.0 | 26.1 |
| `select_count` | 0.1 | 0.0 | 2.2 | 69.7 | 2.2 |
| `select_many` | — | 191.3 | — | — | — |
| `select_where` | 196.2 | 11.2 | 19.4 | 196.7 | 183.3 |
| `select_where_count` | 33.0 | 5.8 | 7.4 | 65.0 | 157.8 |
| `select_where_order_take` | 37.3 | 12.3 | 14.9 | 72.8 | 167.2 |
| `select_where_sum` | 37.3 | 7.4 | 7.4 | 66.8 | 163.2 |
| `single_match` | 0.0 | 2.8 | 5.5 | 58.6 | 155.5 |
| `skip_take` | 0.5 | 0.1 | 0.2 | 3.0 | 2.8 |
| `skip_while_match` | 3.5 | 5.3 | 5.3 | 60.7 | 153.9 |
| `sort_first` | 38.5 | 11.1 | 13.3 | 64.8 | 168.2 |
| `sort_take` | 38.7 | 16.3 | 20.2 | 70.9 | 170.8 |
| `sort_take_select` | 38.5 | 16.3 | 20.7 | 71.3 | 171.0 |
| `sum_aggregate` | 30.3 | 2.1 | 2.1 | 54.4 | 153.3 |
| `sum_where` | 33.1 | 4.4 | 4.3 | 64.2 | 154.7 |
| `take_count` | 3.7 | 0.2 | 0.4 | 2.9 | 2.7 |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 1.3 | 1.1 |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 0.6 | 0.5 |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 0.7 | 0.6 |
| `take_while_match` | 7.9 | 2.4 | 2.4 | 30.3 | 77.7 |
| `to_array_filter` | 69.8 | 11.7 | 12.1 | 71.9 | 165.4 |
| `where_join_count` | 39.7 | 29.2 | 41.8 | 133.1 | 168.8 |
| `zip_count_pred` | 39.3 | 15.8 | — | 315.5 | 317.1 |
| `zip_dot_product` | 46.9 | 12.7 | 10.7 | 317.9 | 314.0 |
| `zip_dot_product_3arg` | 46.7 | 12.6 | — | 310.7 | 314.2 |
| `zip_reverse_to_array` | — | 31.8 | — | 344.1 | 349.6 |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) |
|---|---:|---:|---:|---:|---:|
| `aggregate_match` | 35.9 | 0.3 | 0.7 | 16.7 | 26.7 |
| `all_match` | 27.8 | 0.3 | 0.2 | 16.6 | 25.7 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.7 | 1.0 | 3.6 | 16.6 | 25.3 |
| `bare_last` | — | 0.4 | 0.0 | 0.0 | 0.0 |
| `bare_order_where` | 187.8 | 34.6 | 35.7 | 105.3 | 53.0 |
| `chained_select_collapse` | — | 1.1 | 1.1 | 21.4 | 34.0 |
| `chained_where` | 37.0 | 0.6 | 0.8 | 33.9 | 31.4 |
| `contains_match` | 0.0 | 0.2 | 0.1 | 17.3 | 9.3 |
| `count_aggregate` | 29.6 | 0.3 | 0.6 | 16.7 | 25.9 |
| `cross_join` | 5984.0 | 751.9 | — | 833.5 | 768.9 |
| `decs_count_bare_pred` | — | — | 0.6 | — | — |
| `distinct_by_count` | 42.0 | 1.1 | 1.1 | 21.4 | 34.3 |
| `distinct_by_order_take` | 238.6 | 1.7 | 2.9 | 45.9 | 40.7 |
| `distinct_by_order_to_array` | 241.0 | 1.7 | 2.7 | 46.1 | 40.3 |
| `distinct_count` | 41.5 | 1.1 | 1.1 | 21.4 | 33.0 |
| `distinct_count_pred` | 253.3 | 1.1 | 1.3 | 38.6 | 45.2 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.1 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 170.2 | 1.6 | 1.9 | 36.5 | 45.6 |
| `groupby_count` | 141.7 | 1.4 | 1.5 | 21.8 | 34.1 |
| `groupby_first` | 253.3 | 1.3 | 2.3 | 21.8 | 34.8 |
| `groupby_having_count` | 140.7 | 1.3 | 1.5 | 21.5 | 34.2 |
| `groupby_having_hidden_sum` | 175.1 | 1.5 | 1.7 | 36.4 | 45.3 |
| `groupby_having_post_where` | 169.9 | 1.4 | 1.9 | 36.3 | 44.5 |
| `groupby_max` | 173.3 | 1.5 | 1.9 | 36.4 | 45.9 |
| `groupby_min` | 173.0 | 1.5 | 1.8 | 36.4 | 46.0 |
| `groupby_multi_reducer` | 189.8 | 1.6 | 2.0 | 36.7 | 46.1 |
| `groupby_select_order` | 170.2 | 1.4 | 1.9 | 36.3 | 44.8 |
| `groupby_select_sum` | 198.8 | 2.8 | 3.2 | 31.3 | 40.2 |
| `groupby_sum` | 170.7 | 1.4 | 1.6 | 36.3 | 44.2 |
| `groupby_where_count` | 75.7 | 0.9 | 1.3 | 36.6 | 42.3 |
| `groupby_where_sum` | 87.0 | 0.9 | 1.3 | 36.6 | 43.7 |
| `join_count` | 39.0 | 10.8 | 11.9 | 42.9 | 75.7 |
| `join_groupby_count` | 156.8 | 17.2 | 19.2 | 69.8 | 95.1 |
| `join_groupby_to_array` | 190.7 | 18.3 | 20.1 | 80.7 | 37.6 |
| `join_select` | 93.3 | 20.0 | 21.8 | 75.3 | 100.1 |
| `join_where_count` | 39.8 | 19.0 | 20.7 | 63.1 | 81.0 |
| `last_match` | 0.0 | 0.5 | 1.4 | 17.5 | 26.2 |
| `long_count_aggregate` | 29.7 | 0.3 | 0.6 | 16.7 | 25.9 |
| `max_aggregate` | 31.2 | 0.3 | 0.5 | 16.7 | 27.3 |
| `min_aggregate` | 31.2 | 0.3 | 0.5 | 16.9 | 27.4 |
| `order_by_multi_key` | 250.8 | 54.1 | 54.9 | 124.3 | 72.9 |
| `order_distinct_take` | 140.2 | 1.1 | 75.2 | 21.6 | 36.8 |
| `order_reverse_normalized` | 38.5 | 0.7 | 1.4 | 21.7 | 28.1 |
| `order_take_desc` | 38.5 | 0.7 | 1.3 | 21.7 | 28.2 |
| `reverse_distinct_by` | 297.5 | 1.6 | 3.2 | 21.8 | 35.5 |
| `reverse_take` | 0.0 | 0.0 | 0.0 | 0.0 | 3.8 |
| `reverse_take_select` | 0.0 | 0.0 | 0.0 | 0.0 | 3.8 |
| `select_count` | 0.1 | 0.0 | 0.0 | 66.8 | 0.0 |
| `select_many` | — | 63.2 | — | — | — |
| `select_where` | 110.6 | 4.2 | 5.3 | 75.2 | 22.6 |
| `select_where_count` | 32.8 | 0.3 | 0.6 | 16.9 | 26.8 |
| `select_where_order_take` | 37.1 | 0.7 | 1.4 | 17.6 | 27.6 |
| `select_where_sum` | 38.3 | 0.4 | 0.6 | 16.6 | 25.6 |
| `single_match` | 0.0 | 0.4 | 1.1 | 46.5 | 22.4 |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.2 | 0.2 |
| `skip_while_match` | 3.5 | 0.4 | 0.4 | 46.5 | 22.2 |
| `sort_first` | 38.4 | 0.4 | 1.3 | 16.7 | 27.0 |
| `sort_take` | 38.9 | 0.7 | 1.4 | 21.7 | 28.0 |
| `sort_take_select` | 38.5 | 0.7 | 1.3 | 21.8 | 27.6 |
| `sum_aggregate` | 30.2 | 0.3 | 0.1 | 16.9 | 24.6 |
| `sum_where` | 33.2 | 0.3 | 0.6 | 16.6 | 26.4 |
| `take_count` | 1.9 | 0.1 | 0.1 | 1.2 | 0.2 |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 0.4 | 0.1 |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 0.2 | 0.0 |
| `take_where_count` | 0.9 | 0.0 | 0.0 | 0.2 | 0.0 |
| `take_while_match` | 7.8 | 0.2 | 0.3 | 17.0 | 9.1 |
| `to_array_filter` | 49.0 | 3.3 | 3.3 | 20.1 | 35.9 |
| `where_join_count` | 40.0 | 5.9 | 6.7 | 47.9 | 44.9 |
| `zip_count_pred` | 39.4 | 0.1 | — | 115.0 | 34.0 |
| `zip_dot_product` | 46.9 | 0.1 | 0.1 | 117.9 | 33.9 |
| `zip_dot_product_3arg` | 47.1 | 0.1 | — | 115.0 | 34.0 |
| `zip_reverse_to_array` | — | 4.7 | — | 126.6 | 51.1 |
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
