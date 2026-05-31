# Benchmarks — SQL / Array / Decs / XML comparison

Updated 2026-05-31 (branch `bbatkin/linq-fold-xml-deferred-materialize`): **XML deferred materialization (handle-buffering).** The buffered reducers (`order_by`/`sort`/`reverse` + `take`/`first`, `distinct_by |> order_by`) built the full `Car` — including the `name` string clone — for *every* source element before the reducer kept only `K`; `from_xml_node` materializes all N. Fix: the reducer buffers a cheap **surrogate** — `(orderKey, xml_node)` for the order emits, a bare `xml_node` for reverse (no key) — and runs `build_xml_row` only for the `K` survivors. The comparator is the fixed `less(a._0, b._0)` on the precomputed key; `where`/`distinct` are consumed during the walk so they never enter the surrogate. The abstraction is **source-generic** (an "element handle"): machinery in `linq_fold_common`, four hooks per source (`defers_materialization`/`handle_type`/`current_handle_expr`/`materialize_handle`); only `XmlAdapter` overrides them (array/decs inherit the no-defer default and stay byte-identical — their backing store is already materialized, so they clone only ~K heap-entrants, confirmed by `benchmarks/micro/sort_distinct_take_shapes.das`). Wired into `emit_bounded_heap`/`emit_fused_prefilter`(distinct arm)/`emit_streaming_min`/`emit_reverse_buffer_inplace`. **Design validated by hand-coded micro-bench first** (`benchmarks/micro/sort_distinct_take_shapes.das`). Wins (m5f INTERP / JIT, 100 000 → K string clones): `sort_take` **338 → 69 / 17**, `order_take_desc` 343 → 70 / 17, `distinct_by_order_take` 354 → 127 / 48, `select_where_order_take` 228 → 71 / 18, `distinct_by_order_to_array` 356 → 128 / 46, `sort_first` 336 → 65 / 17, `reverse_take` 360 → 91 / 71. Not deferred (inherent floor / out of scope): `bare_order_where` (already at the materialize-under-guard survivor floor), `order_reverse_normalized` (order+reverse → all rows out), `reverse_distinct_by` (tier-2, no fused emit), `groupby_first` (group_by path). Other cell deltas are long-sweep thermal noise.

Fixture size: n = 100 000 (cars), 100 dealers, 5 brands. Each row is
one bench family in `benchmarks/sql/`; columns are nanoseconds per
logical operation. `—` marks an intentionally absent lane — see
"Notes on missing lanes" below.

The benchmarked chain shapes are summarised in
`benchmarks/README.md` and the splice arms each chain fires are
catalogued in `doc/source/reference/linq_fold_patterns.rst`.

- **SQL** — `_sql` macro over an in-memory SQLite database
  (`with_sqlite(":memory:")`).
- **Array** — `_fold` over `each(arr)` chain, in-memory `array<Car>`.
- **Decs** — `_fold` over `from_decs_template(type<DecsCar>)`,
  per-archetype walk.
- **XML (m5)** — the **un-fused** chain over `from_xml_node(root, type<Car>)`:
  the same linq pipeline as the array lane but without `_fold`
  (`unsafe(from_xml_node(…)) |> _where(…) |> … |> term()`), so it runs the
  tier-2 cascade — a full `Car` materialized per element (every attribute
  read + string clones), intermediate arrays per stage. The reference the
  fused lane is measured against.
- **XML fold (m5f)** — `_fold` over the same `from_xml_node` source. The
  `XmlAdapter` (`pugixml/linq_fold_xml`) fuses + field-prunes where it can.

Sub-nanosecond cells (`0.00`) reflect early-exit terminators that exit
before the timer resolution can measure them — they should be read as
"effectively free."

### Reading the XML lanes (m5 vs m5f)

m5 is the **un-fused** XML cascade; m5f is the **fused** `_fold`. The two run
identical logic, so the delta is purely what the `XmlAdapter` fusion buys:

- **m5f ≪ m5 → fusion works.** The adapter inlines the DOM walk and prunes the
  materialization to the fields the chain reads. This covers the `loop_or_count`
  family — `where` / `select` / `count` / `sum` / `min` / `max` / `average` /
  `aggregate` / `first` / `last` / `single` / `element_at` / `any` / `all` /
  `contains` / `take` / `skip` / `take_while` / `skip_while` (5–10× INTERP, e.g.
  `count_aggregate` 419 → 64; early-exit cells like `take_count_filtered` → ~1) —
  **the cascade families** `group_by` (incl. `having` / multi-reducer),
  `distinct[_by]`, `order_by[_descending]`, `sort`, `reverse`, which ride the
  cascade planners over the DOM walk, **and `join`** (incl. `group_by` with an
  upstream `join`) — the in-memory srcB is collected into a hash and srcA is
  probed from the DOM walk. The win tracks whether field-pruning fires:
  reducing/projecting/key-only shapes drop the per-row `name` clone (e.g.
  `groupby_sum` 462 → 115, `distinct_by_count` 392 → 71, `distinct_take` → 0,
  `join_count` 519 → 114 with 0 clones, `join_groupby_count` 581 → 182,
  `order_distinct_take` 2092 → 72 — a leading `_select(_.brand)` projection is
  absorbed into the source walk by `ProjectedSourceAdapter`, so even an
  `order |> distinct |> take` chain prunes to just the projected field).
  `join_select` 510 → 192 (2.65×) also fuses now: a bare `join |> select` (no
  terminator, iterator-typed) builds the field-pruned result in one pass and
  exposes it as a sequence (`wrapIter`), eliding tier-2's separate `join_to_array`
  — though its `name`-keeping selector leaves the per-row string-clone floor, so
  the win is the elided intermediate, not 0 clones.
- **m5f < m5 via materialize-under-guard.** When the whole row escapes but only behind a
  `where`-gate (`where(p) |> single/last/first`, `where(p) |> to_array`, `where(p) |> order_by`),
  the per-element walk reads just the predicate's fields and builds the full row (its `name` clone)
  only for *matching* elements — the clone count drops to the number of survivors, not the source
  length. `single_match` 330 → 57 (100k → 1 clone), `last_match` 335 → 221, `select_where` 339 → 227,
  `select_where_order_take` 341 → 228, `bare_order_where` 447 → 331.
- **m5f ≈ m5 → modest fusion only.** Whole-row escapes that are NOT behind a field-only gate keep the
  full per-element clone: `reverse |> to_array` (reverse buffers whole rows), un-filtered `sort` /
  `order_by` (no `where` to gate behind). Only the intermediate array is elided; the string-clone floor
  remains.

(The absolute XML numbers stay far above the array/decs lanes either way — XML
carries DOM-parse + per-element attribute reads + `string` clones the in-memory
lanes never pay. The m5↔m5f delta, not the XML-vs-array gap, is the fusion signal.)


<!-- BENCH:TABLES BEGIN -->
*Generated 2026-05-31 by `benchmarks/sql/_update_results.das` — ns/op; `—` = absent lane. Edit the prose around the markers, not the tables.*

## INTERP

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML (m5) | XML fold (m5f) |
|---|---:|---:|---:|---:|---:|
| `aggregate_match` | 34.4 | 5.9 | 5.9 | 451.0 | 60.5 |
| `all_match` | 27.7 | 3.5 | 3.5 | 376.2 | 56.2 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.1 | 0.0 |
| `average_aggregate` | 30.0 | 5.9 | 8.7 | 425.8 | 58.4 |
| `bare_order_where` | 279.0 | 116.7 | 124.5 | 771.6 | 329.2 |
| `chained_select_collapse` | — | 17.7 | 17.9 | 534.7 | 70.2 |
| `chained_where` | 36.7 | 6.6 | 7.2 | 450.5 | 105.9 |
| `contains_match` | 0.0 | 2.2 | 1.4 | 406.7 | 28.4 |
| `count_aggregate` | 29.5 | 4.2 | 4.1 | 413.8 | 64.8 |
| `decs_count_bare_pred` | — | — | 4.1 | — | — |
| `distinct_by_count` | 40.0 | 15.6 | 16.0 | 384.1 | 70.2 |
| `distinct_by_order_take` | 243.3 | 21.7 | 23.3 | 395.2 | 126.5 |
| `distinct_by_order_to_array` | 239.9 | 21.9 | 23.4 | 396.3 | 128.0 |
| `distinct_count` | 41.9 | 15.8 | 15.8 | 475.9 | 71.8 |
| `distinct_count_pred` | 253.8 | 15.8 | 15.9 | 390.2 | 116.0 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 389.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 384.6 | 0.4 |
| `first_match` | 0.0 | 0.0 | 0.0 | 392.5 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 391.7 | 0.0 |
| `groupby_average` | 173.0 | 30.2 | 30.2 | 458.1 | 125.9 |
| `groupby_count` | 142.1 | 19.1 | 19.3 | 422.6 | 76.9 |
| `groupby_first` | 250.4 | 18.4 | 19.1 | 424.1 | 339.2 |
| `groupby_having_count` | 142.4 | 19.2 | 19.2 | 440.8 | 77.3 |
| `groupby_having_hidden_sum` | 178.0 | 24.1 | 24.2 | 460.0 | 123.1 |
| `groupby_having_post_where` | 171.4 | 18.6 | 18.7 | 456.6 | 116.4 |
| `groupby_max` | 173.7 | 24.7 | 25.0 | 478.3 | 121.1 |
| `groupby_min` | 178.7 | 25.0 | 24.9 | 461.9 | 120.0 |
| `groupby_multi_reducer` | 193.7 | 32.6 | 32.2 | 493.6 | 127.5 |
| `groupby_select_order` | 172.5 | 18.6 | 19.1 | 460.5 | 116.2 |
| `groupby_select_sum` | 213.4 | 36.7 | 36.2 | 516.5 | 101.2 |
| `groupby_sum` | 172.3 | 18.6 | 18.8 | 455.3 | 115.6 |
| `groupby_where_count` | 76.2 | 14.4 | 15.0 | 443.0 | 116.7 |
| `groupby_where_sum` | 87.0 | 14.3 | 14.8 | 467.7 | 118.8 |
| `indexed_lookup` | 1491.0 | 198573.8 | 503.1 | 38004844.5 | 5811590.4 |
| `join_count` | 38.2 | 51.6 | 64.8 | 521.0 | 114.1 |
| `join_groupby_count` | 158.7 | 79.1 | 91.5 | 577.1 | 187.9 |
| `join_groupby_to_array` | 190.8 | 78.3 | 92.2 | 606.6 | 220.0 |
| `join_select` | 147.8 | 72.8 | 86.3 | 552.4 | 191.5 |
| `join_where_count` | 39.4 | 61.7 | 76.3 | 566.5 | 164.9 |
| `last_match` | 0.0 | 5.8 | 13.9 | 414.3 | 219.6 |
| `long_count_aggregate` | 30.2 | 4.2 | 4.2 | 464.2 | 65.6 |
| `max_aggregate` | 31.1 | 6.0 | 6.8 | 437.5 | 58.7 |
| `min_aggregate` | 31.1 | 6.0 | 6.8 | 436.2 | 58.2 |
| `order_distinct_take` | 138.7 | 15.7 | 90.9 | 2202.5 | 73.1 |
| `order_reverse_normalized` | 38.1 | 16.2 | 20.3 | 1152.5 | 69.4 |
| `order_take_desc` | 37.1 | 16.3 | 20.0 | 1064.2 | 69.9 |
| `reverse_distinct_by` | 301.3 | 21.5 | — | 450.5 | 426.4 |
| `reverse_take` | 0.1 | 0.0 | 9.3 | 379.3 | 91.3 |
| `reverse_take_select` | 0.0 | 0.0 | 9.3 | 379.6 | 93.7 |
| `select_count` | 0.1 | 0.0 | 2.2 | 432.7 | 70.8 |
| `select_where` | 192.8 | 11.0 | 19.4 | 423.8 | 224.9 |
| `select_where_count` | 32.7 | 5.1 | 7.4 | 479.5 | 65.5 |
| `select_where_order_take` | 37.0 | 12.5 | 15.1 | 739.2 | 71.3 |
| `select_where_sum` | 37.3 | 7.4 | 7.5 | 482.6 | 67.3 |
| `single_match` | 0.0 | 2.8 | 5.5 | 377.3 | 57.2 |
| `skip_take` | 0.5 | 0.1 | 0.2 | 4.4 | 3.7 |
| `skip_while_match` | 3.5 | 5.2 | 5.4 | 399.3 | 59.0 |
| `sort_first` | 37.6 | 11.0 | 13.2 | 1066.4 | 64.6 |
| `sort_take` | 38.5 | 16.1 | 19.9 | 1058.0 | 69.1 |
| `sort_take_select` | 37.5 | 15.9 | 20.1 | 1053.7 | 70.1 |
| `sum_aggregate` | 30.2 | 2.1 | 2.1 | 425.9 | 54.3 |
| `sum_where` | 32.7 | 4.3 | 4.3 | 446.5 | 64.5 |
| `take_count` | 3.7 | 0.2 | 0.4 | 4.3 | 3.5 |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 392.6 | 1.5 |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 388.8 | 0.6 |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 4.7 | 0.7 |
| `take_while_match` | 7.6 | 2.4 | 2.4 | 221.6 | 26.9 |
| `to_array_filter` | 70.2 | 11.7 | 11.8 | 458.8 | 73.0 |
| `zip_count_pred` | 38.9 | 15.0 | — | 530.5 | 373.4 |
| `zip_dot_product` | — | 12.7 | 10.7 | 505.2 | 363.2 |
| `zip_dot_product_3arg` | — | 12.6 | — | 427.6 | 365.9 |
| `zip_reverse_to_array` | — | 30.6 | — | 524.1 | 397.2 |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML (m5) | XML fold (m5f) |
|---|---:|---:|---:|---:|---:|
| `aggregate_match` | 35.5 | 0.3 | 0.6 | 174.3 | 17.2 |
| `all_match` | 28.1 | 0.3 | 0.2 | 159.0 | 20.0 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.1 | 1.0 | 3.6 | 168.5 | 18.6 |
| `bare_order_where` | 185.1 | 33.7 | 34.3 | 276.6 | 123.4 |
| `chained_select_collapse` | — | 2.1 | 2.1 | 181.9 | 21.6 |
| `chained_where` | 36.8 | 0.6 | 0.8 | 176.6 | 34.3 |
| `contains_match` | 0.0 | 0.2 | 0.1 | 163.8 | 17.9 |
| `count_aggregate` | 30.4 | 0.3 | 0.6 | 173.2 | 17.4 |
| `decs_count_bare_pred` | — | — | 0.6 | — | — |
| `distinct_by_count` | 41.7 | 2.1 | 2.1 | 158.4 | 21.4 |
| `distinct_by_order_take` | 246.1 | 2.6 | 3.2 | 170.8 | 47.6 |
| `distinct_by_order_to_array` | 240.7 | 2.7 | 3.3 | 170.5 | 46.2 |
| `distinct_count` | 42.4 | 2.1 | 2.1 | 172.9 | 21.8 |
| `distinct_count_pred` | 252.5 | 2.1 | 2.3 | 161.1 | 40.8 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 160.5 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 165.9 | 0.2 |
| `first_match` | 0.0 | 0.0 | 0.0 | 167.9 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 167.4 | 0.0 |
| `groupby_average` | 171.1 | 2.6 | 2.9 | 188.4 | 37.5 |
| `groupby_count` | 141.8 | 2.3 | 2.5 | 172.1 | 23.6 |
| `groupby_first` | 253.6 | 2.2 | 3.1 | 170.9 | 148.1 |
| `groupby_having_count` | 142.0 | 2.3 | 2.5 | 174.3 | 21.8 |
| `groupby_having_hidden_sum` | 175.5 | 2.5 | 2.8 | 183.3 | 38.1 |
| `groupby_having_post_where` | 174.8 | 2.4 | 2.7 | 179.6 | 37.7 |
| `groupby_max` | 173.8 | 2.4 | 2.7 | 184.6 | 36.5 |
| `groupby_min` | 173.9 | 2.4 | 2.7 | 186.3 | 37.3 |
| `groupby_multi_reducer` | 191.0 | 2.7 | 3.0 | 188.0 | 38.2 |
| `groupby_select_order` | 171.7 | 2.4 | 2.8 | 184.9 | 38.4 |
| `groupby_select_sum` | 199.3 | 3.2 | 3.7 | 199.3 | 32.2 |
| `groupby_sum` | 172.1 | 2.4 | 2.7 | 181.3 | 37.4 |
| `groupby_where_count` | 76.8 | 1.5 | 1.8 | 182.6 | 37.3 |
| `groupby_where_sum` | 87.8 | 1.5 | 1.8 | 186.2 | 37.0 |
| `indexed_lookup` | 1270.4 | 32939.6 | 106.7 | 15732462.3 | 4643707.3 |
| `join_count` | 38.8 | 11.8 | 12.8 | 196.0 | 48.0 |
| `join_groupby_count` | 157.5 | 19.7 | 22.0 | 209.4 | 70.8 |
| `join_groupby_to_array` | 190.1 | 19.8 | 22.0 | 215.1 | 80.9 |
| `join_select` | 92.3 | 20.1 | 22.6 | 205.0 | 78.4 |
| `join_where_count` | 39.7 | 19.7 | 21.6 | 208.7 | 64.6 |
| `last_match` | 0.0 | 0.5 | 1.4 | 168.5 | 88.4 |
| `long_count_aggregate` | 29.9 | 0.3 | 0.6 | 166.9 | 26.9 |
| `max_aggregate` | 31.3 | 0.3 | 0.5 | 167.3 | 17.6 |
| `min_aggregate` | 32.0 | 0.3 | 0.5 | 166.6 | 17.4 |
| `order_distinct_take` | 138.0 | 2.1 | 73.6 | 638.1 | 22.7 |
| `order_reverse_normalized` | 38.9 | 0.7 | 1.4 | 386.4 | 19.3 |
| `order_take_desc` | 38.9 | 0.7 | 1.3 | 370.3 | 17.3 |
| `reverse_distinct_by` | 297.4 | 2.6 | — | 170.8 | 166.2 |
| `reverse_take` | 0.0 | 0.0 | 1.1 | 154.3 | 70.6 |
| `reverse_take_select` | 0.0 | 0.0 | 1.1 | 155.0 | 68.5 |
| `select_count` | 0.1 | 0.0 | 0.0 | 167.6 | 66.9 |
| `select_where` | 106.4 | 4.2 | 5.6 | 174.5 | 96.0 |
| `select_where_count` | 33.2 | 0.3 | 0.6 | 178.0 | 28.4 |
| `select_where_order_take` | 36.7 | 0.7 | 1.4 | 274.3 | 18.3 |
| `select_where_sum` | 37.4 | 0.4 | 0.6 | 176.3 | 18.9 |
| `single_match` | 0.0 | 0.4 | 1.1 | 159.7 | 46.5 |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.7 | 1.6 |
| `skip_while_match` | 3.5 | 0.4 | 0.4 | 161.8 | 46.8 |
| `sort_first` | 38.2 | 0.4 | 1.3 | 378.8 | 17.2 |
| `sort_take` | 38.7 | 0.7 | 1.3 | 374.1 | 18.0 |
| `sort_take_select` | 38.8 | 0.7 | 1.4 | 379.2 | 17.3 |
| `sum_aggregate` | 30.2 | 0.3 | 0.1 | 166.6 | 17.7 |
| `sum_where` | 33.2 | 0.3 | 0.6 | 176.4 | 17.5 |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.7 | 1.6 |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 165.9 | 0.4 |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 160.6 | 0.2 |
| `take_where_count` | 0.9 | 0.0 | 0.0 | 1.8 | 0.2 |
| `take_while_match` | 7.8 | 0.2 | 0.3 | 84.7 | 17.3 |
| `to_array_filter` | 48.6 | 3.2 | 3.4 | 179.5 | 20.5 |
| `zip_count_pred` | 39.5 | 0.1 | — | 183.4 | 150.0 |
| `zip_dot_product` | — | 0.1 | 0.1 | 180.8 | 151.9 |
| `zip_dot_product_3arg` | — | 0.1 | — | 169.1 | 152.1 |
| `zip_reverse_to_array` | — | 4.5 | — | 187.2 | 162.6 |
<!-- BENCH:TABLES END -->

## Notes on missing lanes (the `—` cells)

The reasons each cell is empty are also recorded as a comment in the
corresponding `.das` bench file; the bullets below quote that comment.
For deeper detail on SQL cells — what query each would lower to,
whether the gap is window-function / surface-limitation / by-design,
and which gaps could land in a single PR — see
[`sqlite_linq_gaps.md`](sqlite_linq_gaps.md).

- **`chained_select_collapse` SQL** — `_sql` rejects `distinct() |> count()`
  as non-translatable. The equivalent SQL `COUNT(DISTINCT computed-expr)`
  isn't currently emitted by sqlite_linq's surface. By design — no follow-up.
- **`decs_count_bare_pred` SQL / Array / XML (m5, m5f)** — covers a Theme 4
  root-cause fix specific to the decs lane (bare `from_decs_template(...).count(P)`
  with no upstream where/select previously bailed because
  `forExpr.iteratorVariables` was unpopulated). Array-side bare `count(P)`
  was always reachable; SQL `count(P)` is covered by `count_aggregate.das`
  with a where shape. Both XML lanes are absent because the family is
  decs-only (it exists to exercise a decs-walk root cause — no array/XML/SQL
  analog is meaningful). By design.
- **`indexed_lookup` m3f vs m4** — array's lane measures the unspliced
  linear scan (~204k ns/op), while decs uses `query(eid)` for O(1) lookup.
  The two cells compare fundamentally different algorithms — not a
  like-for-like benchmark.
- **`reverse_distinct_by` Decs** — splice arm is array-only
  (`array_source` predicate in `daslib/linq_fold.das:3221`) because
  backward-index walk has no archetype analog. SQL lane closed by
  Arm B of this PR via MAX(pk) bare-aggregate.
- **`order_distinct_take` m4 vs m3f** — m4 (95.5 INTERP /
  74.9 JIT) vs m3f (15.8 INTERP / 2.1 JIT) is NOT apples-to-apples.
  `unique_key` (`daslib/linq.das:614`) short-circuits to a direct
  return for workhorse types and falls back to `"{a}"` string
  interpolation for everything else. Array m3f operates on
  `array<int>` (workhorse — direct hash, zero allocs per element);
  decs m4 operates on `DecsBrand` (struct — one string allocation
  per element). The 6× INTERP / 36× JIT m4 overhead is the
  per-entity string-hash cost, not decs-walk vs array-walk. For a
  key-based dedup variant on decs that avoids `unique_key`'s string
  path see `distinct_by_count` (`_distinct_by(_.brand)` over decs,
  m4 = 15.8 INTERP / 2.1 JIT = parity with the array fast path).
- **`zip_dot_product` SQL / `zip_dot_product_3arg` SQL** — the degenerate
  same-row form (`_select(_.price * _.year) |> sum()`) lowers, but
  `SUM(price*year)` over 100k rows is ~1e11 and the computed-projection
  result is read back as the expression's daslang type (`int`), which
  truncates. A faithful lane needs an int64-typed computed projection
  (`int64(_.price) * int64(_.year)`), which currently fails inference —
  **deferred to the computed-cast follow-up** at the end of the linq work.
- **`zip_reverse_to_array` SQL / Decs** — `reverse()` has no SQL order key
  (relational rows are unordered without an `ORDER BY`), and zip is not
  naturally expressible over a single archetype walk. By design, no follow-up.
- **`zip_*` XML lanes (m5, m5f)** — now landed (see the top banner). Each zip
  bench zips the XML `Car` price-stream against a synthetic int array via the
  mixed `zip(iterator, array)` overload; m5f modestly beats m5 (the zip splice
  partially fuses over XML), with both lanes still paying the unpruned `Car`
  materialization. The remaining `—` zip cells are **SQL (m1)** and **Decs
  (m4)**: `zip` is not a relational op and not expressible over a single
  archetype walk (see the two bullets above).

## Accepted architectural floors (m4 vs m3f)

Three cells where m4 stays ≥1.5× m3f INTERP and the gap is structural,
not a splice/emit oversight. Documented here so the close-out doesn't
keep chasing them.

- **`last_match` m4 +8.2 ns INTERP (m3f 5.7 → m4 13.9, 2.44×)** — m3f's
  iterator `last()` walks an `array<T>` and rebinds a single `var last : TT -&`
  reference on each match (O(1) per match, one final clone at return).
  m4 walks `for_each_archetype` and emits a 6-column lockstep
  `for (car_id, car_name, car_price, car_brand, car_year, car_dealer_id in
  get_default_ro(arch, ...) × 6)` — every element pays for all six
  `get_default_ro` advances regardless of match status, then on match
  constructs a tuple and `:=` clone-assigns it (which deep-clones the
  string field). The fetch-all-columns model is the cost of decs columnar
  storage; reducing it would require redesigning the per-element walk to
  fetch the predicate column first and defer the rest until match —
  significant infrastructure change for an 8 ns cell.
- **`select_where` m4 +8.2 ns INTERP (m3f 10.9 → m4 19.1, 1.75×)** — same
  root cause as `last_match`: 6-column lockstep fetch per element plus
  per-match `push_clone(tuple(...))` (string clone) into the output
  buffer. The `to_array` shape can't avoid materializing each match, so
  the per-match clone is structural; the per-element fetch cost is the
  same decs columnar overhead.
- **`order_distinct_take` m4 +78 ns INTERP, 35× JIT** — `unique_key`
  string-interpolation path on non-workhorse `DecsBrand` keys (already
  documented in "Notes on missing lanes" above — not closeable without
  a struct-aware hashing scheme).

`reverse_take_select` USED to be on this list (m4 was +14 ns INTERP at
the catch-all path). It is NOT a floor — closed on the decs side by
extending the decs skip-into-tail fast path to handle a trailing
`_select` (was previously gated to bail out when termsel was present,
forcing fall-through to the expensive full-buffer-then-reverse-then-
resize-then-project path that did N push_clones with string clones for
N=100K to keep just K=10). The symmetric m3f side was closed in the
follow-up by extending the array-side R6 backward-index walk to accept
a trailing `_select(F)` slot: K raw source elements push_clone into a
srcElem-typed scratch buffer during the backward loop, then a post-loop
`build_terminal_select_tail` projection pass fills a separate projElem-
typed output buffer (K projection push_clones). Two-buffer/two-pass
mirrors the decs sibling and R1-R4 catch-all discipline (all source
reads complete before any projection runs — impure `_select` semantics
match across the three paths). Both m3f and m4 are now sub-resolution
per-op (the 2K push_clones for K=10 amortize over chunk_size=100K and
round to 0.0 ns/op in INTERP and JIT).

## How to re-run

The two matrices above (between the ``BENCH:TABLES`` markers) are
regenerated by **`benchmarks/sql/_update_results.das`** from the
machine-readable benchmark output — never hand-edit the table cells.
From the repo root, on macOS / Linux:

```bash
# INTERP — one BenchmarkRunStats JSON object per line
rm -f /tmp/bench_interp.json
for f in benchmarks/sql/*.das; do
  bn=$(basename "$f")
  [[ "$bn" == _* ]] && continue
  build/daslang dastest/dastest.das -- --bench --bench-format json --test "$f" \
    >> /tmp/bench_interp.json 2>/dev/null
done

# JIT (-jit goes BEFORE dastest.das)
rm -f /tmp/bench_jit.json
for f in benchmarks/sql/*.das; do
  bn=$(basename "$f")
  [[ "$bn" == _* ]] && continue
  build/daslang -jit dastest/dastest.das -- --bench --bench-format json --test "$f" \
    >> /tmp/bench_jit.json 2>/dev/null
done

# regenerate the INTERP/JIT tables in place (ns/op = time_ns / n)
build/daslang benchmarks/sql/_update_results.das -- \
  --interp /tmp/bench_interp.json --jit /tmp/bench_jit.json --date YYYY-MM-DD
```

Edit the prose around the markers (this section, the header narrative,
"Notes on missing lanes") by hand; the tool only rewrites the tables.

If JIT fails to compile (e.g. `error[30341]: no matching functions or
generics: host_jit_triple()`), the `bin/daslang` binary is stale
relative to `src/builtin/module_jit.cpp` — rebuild with
`cmake --build build --config Release -j 64` and retry.
