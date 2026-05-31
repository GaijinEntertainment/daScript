# Benchmarks — SQL / Array / Decs / XML comparison

Updated 2026-05-31 (branch `bbatkin/linq-fold-xml-defer-last-groupby`): **XML deferred materialization extended to `last()` + group-by first-per-group.** Building on the handle-buffering arc (#2945), the two remaining survivors-≪-N reducers now defer too. `last`/`last_or_default` cloned the whole `Car` on *every* match and kept only the final one — it now stores the node **handle** per match and runs `build_xml_row` once, for the single survivor. `group_by(k) |> select((key, first per group))` pinned the whole row (`slot := it`), forcing `wrap_source_loop` to build every element — it now materializes from the handle *inside the table miss-branch*, so the walk field-prunes to just the group key and builds one row per distinct group. Both ride the same four `SourceAdapter` hooks (`defers_materialization`/`handle_type`/`current_handle_expr`/`materialize_handle`); array/decs inherit the no-defer default and stay **byte-identical** (the group-by gate also requires the raw, unprojected bind, since the handle yields the raw element). **Design validated by hand-coded micro-bench first** (the `last_match` / `groupby_first` lanes in `benchmarks/micro/sort_distinct_take_shapes.das`). Wins (m5f INTERP / JIT, clones 100 000 → K): `last_match` **219 → 66 / 21** (K=1), `groupby_first` **339 → 72 / 22** (K=#brands). Still floored (inherent / out of scope): `bare_order_where` (at the materialize-under-guard survivor floor), `order_reverse_normalized` (order+reverse → all rows out), `reverse_distinct_by` (tier-2, no fused emit). Other cell deltas are long-sweep thermal noise.

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
  length. `single_match` 330 → 57 (100k → 1 clone), `select_where` 339 → 227, `bare_order_where` 447 → 331
  (`last_match` and `select_where_order_take` have since graduated to the handle-buffer floor — see banner).
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
| `aggregate_match` | 35.2 | 6.0 | 5.9 | 452.7 | 60.4 |
| `all_match` | 28.1 | 3.5 | 3.5 | 379.5 | 56.3 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.1 | 0.0 |
| `average_aggregate` | 30.7 | 5.9 | 8.8 | 428.3 | 60.2 |
| `bare_order_where` | 280.2 | 117.1 | 124.4 | 777.8 | 331.0 |
| `chained_select_collapse` | — | 17.8 | 17.9 | 546.7 | 70.6 |
| `chained_where` | 37.2 | 6.6 | 7.2 | 450.5 | 105.9 |
| `contains_match` | 0.0 | 2.2 | 1.4 | 409.3 | 28.5 |
| `count_aggregate` | 30.1 | 4.2 | 4.1 | 415.0 | 66.0 |
| `decs_count_bare_pred` | — | — | 4.2 | — | — |
| `distinct_by_count` | 42.1 | 15.6 | 16.2 | 392.8 | 70.7 |
| `distinct_by_order_take` | 240.2 | 21.9 | 23.6 | 397.5 | 126.5 |
| `distinct_by_order_to_array` | 242.5 | 21.9 | 23.3 | 396.8 | 127.4 |
| `distinct_count` | 41.8 | 16.1 | 15.8 | 476.4 | 71.0 |
| `distinct_count_pred` | 252.5 | 15.9 | 16.2 | 391.9 | 115.3 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 397.4 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 394.0 | 0.5 |
| `first_match` | 0.0 | 0.0 | 0.0 | 392.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 391.3 | 0.0 |
| `groupby_average` | 172.2 | 30.4 | 30.4 | 460.8 | 125.3 |
| `groupby_count` | 142.6 | 19.2 | 19.6 | 429.8 | 76.9 |
| `groupby_first` | 251.4 | 18.5 | 19.3 | 426.3 | 72.5 |
| `groupby_having_count` | 142.0 | 19.2 | 19.3 | 434.1 | 77.0 |
| `groupby_having_hidden_sum` | 175.6 | 24.2 | 24.2 | 460.9 | 122.0 |
| `groupby_having_post_where` | 173.5 | 18.7 | 18.7 | 458.9 | 116.1 |
| `groupby_max` | 174.3 | 24.9 | 25.0 | 466.6 | 120.9 |
| `groupby_min` | 174.3 | 25.2 | 25.2 | 469.6 | 120.8 |
| `groupby_multi_reducer` | 190.4 | 32.5 | 32.5 | 493.9 | 128.2 |
| `groupby_select_order` | 171.7 | 18.6 | 18.8 | 463.7 | 116.5 |
| `groupby_select_sum` | 201.7 | 36.5 | 36.2 | 513.2 | 101.3 |
| `groupby_sum` | 172.4 | 18.6 | 18.8 | 455.8 | 114.9 |
| `groupby_where_count` | 75.8 | 14.3 | 15.0 | 448.1 | 118.2 |
| `groupby_where_sum` | 86.9 | 14.2 | 14.8 | 469.1 | 119.2 |
| `indexed_lookup` | 1450.5 | 197428.3 | 481.7 | 38072480.7 | 5763768.6 |
| `join_count` | 38.9 | 51.4 | 64.7 | 519.3 | 114.5 |
| `join_groupby_count` | 158.4 | 80.5 | 91.1 | 577.2 | 182.6 |
| `join_groupby_to_array` | 191.5 | 78.2 | 91.9 | 608.3 | 218.8 |
| `join_select` | 149.6 | 71.9 | 86.2 | 550.4 | 191.4 |
| `join_where_count` | 39.7 | 61.5 | 76.4 | 567.5 | 165.5 |
| `last_match` | 0.0 | 5.8 | 14.0 | 416.6 | 65.8 |
| `long_count_aggregate` | 30.2 | 4.1 | 4.1 | 415.4 | 64.0 |
| `max_aggregate` | 31.1 | 5.9 | 6.9 | 441.5 | 58.8 |
| `min_aggregate` | 31.3 | 6.1 | 7.0 | 439.8 | 58.8 |
| `order_distinct_take` | 138.3 | 15.8 | 95.9 | 2205.3 | 73.1 |
| `order_reverse_normalized` | 38.8 | 16.4 | 20.1 | 1150.9 | 69.3 |
| `order_take_desc` | 38.7 | 16.3 | 20.0 | 1062.8 | 70.8 |
| `reverse_distinct_by` | 299.4 | 21.3 | — | 451.0 | 429.0 |
| `reverse_take` | 0.1 | 0.0 | 9.2 | 379.2 | 89.3 |
| `reverse_take_select` | 0.0 | 0.0 | 9.2 | 380.7 | 88.5 |
| `select_count` | 0.1 | 0.0 | 2.2 | 433.9 | 68.3 |
| `select_where` | 194.1 | 11.0 | 19.3 | 426.3 | 226.5 |
| `select_where_count` | 33.2 | 5.2 | 7.4 | 480.3 | 65.5 |
| `select_where_order_take` | 37.0 | 12.5 | 14.9 | 740.6 | 71.2 |
| `select_where_sum` | 37.7 | 7.4 | 7.6 | 482.2 | 67.5 |
| `single_match` | 0.0 | 2.8 | 5.6 | 379.3 | 57.7 |
| `skip_take` | 0.5 | 0.1 | 0.2 | 4.4 | 3.7 |
| `skip_while_match` | 3.5 | 5.2 | 5.4 | 405.1 | 59.4 |
| `sort_first` | 38.4 | 11.0 | 13.5 | 1066.3 | 64.8 |
| `sort_take` | 39.0 | 16.2 | 20.1 | 1065.0 | 69.3 |
| `sort_take_select` | 38.9 | 16.2 | 20.1 | 1068.7 | 70.8 |
| `sum_aggregate` | 30.8 | 2.1 | 2.1 | 428.3 | 54.4 |
| `sum_where` | 33.5 | 4.3 | 4.3 | 447.5 | 64.5 |
| `take_count` | 3.6 | 0.2 | 0.4 | 4.3 | 3.5 |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 393.6 | 1.3 |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 393.4 | 0.6 |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 4.7 | 0.7 |
| `take_while_match` | 7.9 | 2.4 | 2.4 | 225.7 | 29.8 |
| `to_array_filter` | 70.4 | 11.7 | 11.8 | 461.8 | 72.2 |
| `zip_count_pred` | 39.4 | 15.2 | — | 533.0 | 376.3 |
| `zip_dot_product` | — | 12.8 | 10.9 | 506.6 | 366.1 |
| `zip_dot_product_3arg` | — | 12.8 | — | 426.1 | 365.6 |
| `zip_reverse_to_array` | — | 30.8 | — | 528.4 | 401.3 |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML (m5) | XML fold (m5f) |
|---|---:|---:|---:|---:|---:|
| `aggregate_match` | 35.2 | 0.3 | 0.6 | 174.9 | 17.0 |
| `all_match` | 27.9 | 0.3 | 0.2 | 158.8 | 20.2 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.5 | 1.0 | 3.5 | 167.3 | 18.6 |
| `bare_order_where` | 184.2 | 34.1 | 34.3 | 276.4 | 123.0 |
| `chained_select_collapse` | — | 2.1 | 2.1 | 181.7 | 21.7 |
| `chained_where` | 36.9 | 0.6 | 0.8 | 177.1 | 34.3 |
| `contains_match` | 0.0 | 0.2 | 0.1 | 162.8 | 16.9 |
| `count_aggregate` | 30.0 | 0.3 | 0.6 | 168.4 | 17.6 |
| `decs_count_bare_pred` | — | — | 0.6 | — | — |
| `distinct_by_count` | 41.6 | 2.1 | 2.1 | 157.6 | 21.4 |
| `distinct_by_order_take` | 241.2 | 2.6 | 3.2 | 170.7 | 47.6 |
| `distinct_by_order_to_array` | 238.5 | 2.7 | 3.2 | 168.8 | 46.2 |
| `distinct_count` | 41.8 | 2.1 | 2.1 | 174.7 | 22.0 |
| `distinct_count_pred` | 253.4 | 2.1 | 2.3 | 159.6 | 40.8 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 161.1 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 168.3 | 0.2 |
| `first_match` | 0.0 | 0.0 | 0.0 | 167.5 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 167.3 | 0.0 |
| `groupby_average` | 170.1 | 2.6 | 2.9 | 187.0 | 37.5 |
| `groupby_count` | 141.7 | 2.4 | 2.5 | 169.7 | 24.1 |
| `groupby_first` | 255.5 | 2.2 | 3.1 | 170.1 | 22.2 |
| `groupby_having_count` | 141.6 | 2.4 | 2.5 | 173.3 | 21.8 |
| `groupby_having_hidden_sum` | 175.8 | 2.5 | 2.8 | 186.3 | 38.1 |
| `groupby_having_post_where` | 170.7 | 2.4 | 2.7 | 178.0 | 37.8 |
| `groupby_max` | 173.6 | 2.4 | 2.7 | 183.9 | 36.6 |
| `groupby_min` | 173.7 | 2.4 | 2.7 | 183.3 | 37.3 |
| `groupby_multi_reducer` | 189.9 | 2.7 | 3.0 | 186.7 | 38.2 |
| `groupby_select_order` | 171.7 | 2.4 | 2.7 | 183.7 | 38.4 |
| `groupby_select_sum` | 202.2 | 3.2 | 3.8 | 199.3 | 32.3 |
| `groupby_sum` | 172.2 | 2.4 | 2.7 | 179.6 | 37.0 |
| `groupby_where_count` | 75.9 | 1.5 | 1.8 | 178.3 | 37.3 |
| `groupby_where_sum` | 86.6 | 1.5 | 1.8 | 186.7 | 36.9 |
| `indexed_lookup` | 1253.0 | 32569.3 | 106.1 | 15845165.3 | 4663207.8 |
| `join_count` | 38.6 | 11.8 | 12.8 | 198.6 | 48.8 |
| `join_groupby_count` | 157.9 | 19.7 | 21.9 | 209.5 | 70.8 |
| `join_groupby_to_array` | 191.5 | 19.5 | 21.7 | 213.6 | 80.7 |
| `join_select` | 91.4 | 20.0 | 22.5 | 205.0 | 76.0 |
| `join_where_count` | 39.2 | 19.7 | 21.6 | 207.1 | 64.4 |
| `last_match` | 0.0 | 0.5 | 1.4 | 171.2 | 21.5 |
| `long_count_aggregate` | 29.8 | 0.3 | 0.6 | 166.9 | 26.9 |
| `max_aggregate` | 31.5 | 0.3 | 0.5 | 166.8 | 17.7 |
| `min_aggregate` | 31.1 | 0.3 | 0.5 | 166.9 | 17.6 |
| `order_distinct_take` | 137.7 | 2.1 | 73.9 | 637.3 | 22.8 |
| `order_reverse_normalized` | 38.7 | 0.7 | 1.3 | 387.6 | 19.2 |
| `order_take_desc` | 38.7 | 0.7 | 1.3 | 371.3 | 17.2 |
| `reverse_distinct_by` | 296.6 | 2.6 | — | 169.4 | 166.6 |
| `reverse_take` | 0.0 | 0.0 | 1.1 | 154.1 | 68.0 |
| `reverse_take_select` | 0.0 | 0.0 | 1.1 | 155.3 | 68.9 |
| `select_count` | 0.1 | 0.0 | 0.0 | 167.1 | 66.5 |
| `select_where` | 105.6 | 4.2 | 5.7 | 173.8 | 100.6 |
| `select_where_count` | 33.5 | 0.3 | 0.6 | 178.4 | 28.5 |
| `select_where_order_take` | 36.9 | 0.7 | 1.4 | 273.6 | 18.7 |
| `select_where_sum` | 37.3 | 0.4 | 0.6 | 176.8 | 19.0 |
| `single_match` | 0.0 | 0.4 | 1.1 | 158.9 | 46.3 |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.7 | 1.6 |
| `skip_while_match` | 3.5 | 0.4 | 0.4 | 160.6 | 46.6 |
| `sort_first` | 38.3 | 0.4 | 1.3 | 374.7 | 17.3 |
| `sort_take` | 38.6 | 0.7 | 1.4 | 375.6 | 18.1 |
| `sort_take_select` | 39.1 | 0.7 | 1.3 | 376.3 | 17.0 |
| `sum_aggregate` | 30.0 | 0.3 | 0.1 | 166.6 | 17.7 |
| `sum_where` | 33.4 | 0.3 | 0.6 | 176.6 | 17.7 |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.7 | 1.5 |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 164.9 | 0.4 |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 160.7 | 0.2 |
| `take_where_count` | 0.9 | 0.0 | 0.0 | 1.8 | 0.2 |
| `take_while_match` | 7.8 | 0.2 | 0.3 | 84.9 | 17.5 |
| `to_array_filter` | 47.4 | 3.2 | 3.4 | 179.7 | 20.4 |
| `zip_count_pred` | 39.5 | 0.1 | — | 184.4 | 150.3 |
| `zip_dot_product` | — | 0.1 | 0.1 | 180.4 | 151.8 |
| `zip_dot_product_3arg` | — | 0.1 | — | 169.4 | 152.2 |
| `zip_reverse_to_array` | — | 4.5 | — | 186.9 | 160.6 |
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
