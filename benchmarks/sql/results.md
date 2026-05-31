# Benchmarks — SQL / Array / Decs / XML comparison

Updated 2026-05-30 (branch `bbatkin/linq-fold-xml-under-guard-materialize`): **XML materialize-under-guard.** When the whole row escapes the fold but ONLY inside a `where`-gate (`where(p) |> single/last/first`, `where(p) |> to_array`, `where(p) |> order_by`, …), `build_xml_materializer` used to build the full `Car` — including the `name` string clone — for *every* source element before running the gate, so a unique-id `single()` paid 100 000 clones to return one row. Fix: `try_xml_under_guard_materialize` recognizes the bare `if (fieldCond) { … }` body shape, reads just the predicate's fields cheaply per element (`peek_xml_field` — string guard fields come back as a borrowed temp `string#`, no clone; type-enforced so a predicate that tries to *store* the view is a compile error, forcing `clone_string`), and builds the full row only *inside* the matching branch — non-matching elements skip the clone entirely. **Design validated by hand-coded micro-bench first** (`benchmarks/micro/single_last_shapes.das`). Wins: `single_match` m5f **329.9 → 57.0** INTERP (5.8×, 100 000 → 1 string clone), **147.9 → 46.2** JIT; `last_match` 335.0 → 220.7 / 148.3 → 89.4 (clones only matching rows). The same `if (where) { whole-row }` shape lifts `select_where` 338.9 → 226.5, `select_where_order_take` 340.6 → 227.9, `bare_order_where` 447.5 → 330.6 for free. XML-only and pure (count/sum/where|>count don't trigger the whole-row path); no source-agnostic emit-contract change.

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
*Generated 2026-05-30 by `benchmarks/sql/_update_results.das` — ns/op; `—` = absent lane. Edit the prose around the markers, not the tables.*

## INTERP

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML (m5) | XML fold (m5f) |
|---|---:|---:|---:|---:|---:|
| `aggregate_match` | 36.1 | 6.0 | 5.9 | 452.8 | 60.3 |
| `all_match` | 28.2 | 3.6 | 3.6 | 378.7 | 56.1 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.1 | 0.0 |
| `average_aggregate` | 30.3 | 5.9 | 8.8 | 428.2 | 59.8 |
| `bare_order_where` | 280.5 | 116.8 | 124.7 | 777.2 | 330.6 |
| `chained_select_collapse` | — | 17.7 | 17.7 | 538.7 | 70.3 |
| `chained_where` | 36.7 | 6.6 | 7.2 | 451.6 | 106.0 |
| `contains_match` | 0.0 | 2.3 | 1.3 | 410.8 | 28.1 |
| `count_aggregate` | 29.9 | 4.2 | 4.2 | 430.2 | 64.5 |
| `decs_count_bare_pred` | — | — | 4.1 | — | — |
| `distinct_by_count` | 41.5 | 15.8 | 16.0 | 390.0 | 70.4 |
| `distinct_by_order_take` | 239.2 | 21.7 | 23.4 | 397.9 | 354.0 |
| `distinct_by_order_to_array` | 240.5 | 21.8 | 23.3 | 397.8 | 356.1 |
| `distinct_count` | 41.6 | 15.8 | 15.7 | 473.0 | 71.0 |
| `distinct_count_pred` | 252.3 | 16.0 | 16.0 | 392.2 | 114.8 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 392.4 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 425.4 | 0.5 |
| `first_match` | 0.0 | 0.0 | 0.0 | 396.9 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 394.4 | 0.0 |
| `groupby_average` | 175.9 | 30.1 | 29.9 | 460.7 | 125.2 |
| `groupby_count` | 148.2 | 19.5 | 19.4 | 428.6 | 76.6 |
| `groupby_first` | 261.5 | 18.4 | 19.2 | 430.0 | 342.6 |
| `groupby_having_count` | 142.0 | 19.2 | 19.2 | 433.6 | 77.0 |
| `groupby_having_hidden_sum` | 175.2 | 24.2 | 24.1 | 467.4 | 122.3 |
| `groupby_having_post_where` | 171.2 | 18.6 | 18.6 | 462.7 | 116.4 |
| `groupby_max` | 173.0 | 24.8 | 25.1 | 467.0 | 120.7 |
| `groupby_min` | 174.2 | 25.1 | 25.2 | 469.6 | 121.2 |
| `groupby_multi_reducer` | 191.5 | 32.7 | 32.3 | 498.3 | 127.2 |
| `groupby_select_order` | 170.8 | 18.8 | 18.7 | 461.0 | 115.5 |
| `groupby_select_sum` | 199.6 | 36.7 | 35.8 | 512.8 | 101.0 |
| `groupby_sum` | 170.6 | 18.6 | 18.9 | 459.9 | 115.4 |
| `groupby_where_count` | 76.1 | 14.4 | 14.9 | 449.0 | 118.5 |
| `groupby_where_sum` | 86.5 | 14.2 | 14.7 | 468.3 | 118.5 |
| `indexed_lookup` | 1457.8 | 197574.4 | 479.5 | 38138258.0 | 5717939.6 |
| `join_count` | 38.6 | 51.1 | 64.4 | 519.2 | 116.7 |
| `join_groupby_count` | 157.3 | 78.8 | 91.4 | 578.0 | 181.7 |
| `join_groupby_to_array` | 190.4 | 78.3 | 92.3 | 622.4 | 217.8 |
| `join_select` | 148.9 | 72.1 | 92.2 | 550.8 | 190.1 |
| `join_where_count` | 39.7 | 61.1 | 75.7 | 565.9 | 165.9 |
| `last_match` | 0.0 | 5.9 | 14.4 | 415.6 | 220.7 |
| `long_count_aggregate` | 29.8 | 4.1 | 4.1 | 415.0 | 64.0 |
| `max_aggregate` | 31.1 | 6.0 | 6.9 | 439.7 | 58.4 |
| `min_aggregate` | 31.3 | 6.0 | 6.9 | 441.4 | 58.6 |
| `order_distinct_take` | 138.3 | 15.8 | 96.6 | 2195.0 | 72.5 |
| `order_reverse_normalized` | 38.7 | 16.2 | 19.9 | 1136.3 | 341.0 |
| `order_take_desc` | 38.8 | 16.3 | 20.9 | 1052.2 | 343.5 |
| `reverse_distinct_by` | 296.5 | 22.0 | — | 453.0 | 428.2 |
| `reverse_take` | 0.1 | 0.0 | 9.2 | 379.3 | 359.9 |
| `reverse_take_select` | 0.0 | 0.0 | 9.2 | 381.2 | 358.9 |
| `select_count` | 0.1 | 0.0 | 2.2 | 434.4 | 68.5 |
| `select_where` | 196.1 | 11.1 | 19.3 | 424.3 | 226.5 |
| `select_where_count` | 33.0 | 5.1 | 7.4 | 480.0 | 65.5 |
| `select_where_order_take` | 37.1 | 12.5 | 15.0 | 748.2 | 227.9 |
| `select_where_sum` | 37.4 | 7.4 | 7.5 | 485.4 | 67.1 |
| `single_match` | 0.0 | 2.9 | 5.4 | 382.3 | 57.0 |
| `skip_take` | 0.5 | 0.1 | 0.2 | 4.4 | 3.7 |
| `skip_while_match` | 3.5 | 5.2 | 5.3 | 403.8 | 58.8 |
| `sort_first` | 38.6 | 11.0 | 13.4 | 1087.1 | 336.5 |
| `sort_take` | 38.7 | 16.6 | 20.7 | 1070.4 | 338.8 |
| `sort_take_select` | 38.7 | 16.3 | 20.2 | 1066.2 | 345.8 |
| `sum_aggregate` | 30.5 | 2.1 | 2.1 | 428.3 | 53.9 |
| `sum_where` | 33.3 | 4.3 | 4.3 | 451.4 | 64.3 |
| `take_count` | 3.6 | 0.2 | 0.4 | 4.3 | 3.5 |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 395.5 | 1.3 |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 392.1 | 0.6 |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 4.7 | 0.7 |
| `take_while_match` | 7.8 | 2.4 | 2.4 | 226.8 | 29.8 |
| `to_array_filter` | 71.1 | 11.8 | 11.8 | 460.0 | 72.0 |
| `zip_count_pred` | 39.5 | 15.2 | — | 536.3 | 379.3 |
| `zip_dot_product` | — | 12.6 | 10.7 | 509.7 | 366.0 |
| `zip_dot_product_3arg` | — | 12.6 | — | 431.9 | 370.5 |
| `zip_reverse_to_array` | — | 31.1 | — | 529.3 | 403.7 |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML (m5) | XML fold (m5f) |
|---|---:|---:|---:|---:|---:|
| `aggregate_match` | 34.8 | 0.3 | 0.6 | 175.8 | 16.6 |
| `all_match` | 27.9 | 0.3 | 0.2 | 159.8 | 19.4 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.3 | 1.0 | 3.6 | 169.1 | 18.1 |
| `bare_order_where` | 186.8 | 34.0 | 35.4 | 279.6 | 124.8 |
| `chained_select_collapse` | — | 2.1 | 2.1 | 183.6 | 21.4 |
| `chained_where` | 37.0 | 0.6 | 0.8 | 178.7 | 33.9 |
| `contains_match` | 0.0 | 0.2 | 0.1 | 164.6 | 16.9 |
| `count_aggregate` | 29.7 | 0.3 | 0.6 | 170.4 | 16.7 |
| `decs_count_bare_pred` | — | — | 0.6 | — | — |
| `distinct_by_count` | 41.6 | 2.1 | 2.1 | 159.0 | 21.4 |
| `distinct_by_order_take` | 240.2 | 2.6 | 3.2 | 172.8 | 163.2 |
| `distinct_by_order_to_array` | 240.9 | 2.7 | 3.3 | 171.3 | 161.4 |
| `distinct_count` | 41.6 | 2.1 | 2.1 | 174.3 | 21.4 |
| `distinct_count_pred` | 253.2 | 2.1 | 2.3 | 161.6 | 40.4 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 161.4 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 167.4 | 0.2 |
| `first_match` | 0.0 | 0.0 | 0.0 | 169.7 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 167.7 | 0.0 |
| `groupby_average` | 171.4 | 2.6 | 2.9 | 185.3 | 36.4 |
| `groupby_count` | 143.0 | 2.4 | 2.5 | 176.3 | 23.2 |
| `groupby_first` | 252.1 | 2.2 | 3.1 | 175.7 | 146.9 |
| `groupby_having_count` | 141.7 | 2.4 | 2.5 | 177.9 | 21.4 |
| `groupby_having_hidden_sum` | 175.7 | 2.5 | 2.8 | 186.6 | 38.0 |
| `groupby_having_post_where` | 171.7 | 2.4 | 2.7 | 183.4 | 37.4 |
| `groupby_max` | 173.3 | 2.4 | 2.7 | 184.1 | 36.2 |
| `groupby_min` | 172.7 | 2.4 | 2.7 | 185.9 | 37.2 |
| `groupby_multi_reducer` | 190.1 | 2.7 | 3.0 | 192.1 | 37.9 |
| `groupby_select_order` | 171.7 | 2.4 | 2.7 | 184.2 | 38.3 |
| `groupby_select_sum` | 199.6 | 3.2 | 3.7 | 199.3 | 32.3 |
| `groupby_sum` | 171.8 | 2.4 | 2.7 | 185.1 | 37.0 |
| `groupby_where_count` | 76.0 | 1.5 | 1.8 | 179.3 | 37.3 |
| `groupby_where_sum` | 87.2 | 1.5 | 1.8 | 183.7 | 36.5 |
| `indexed_lookup` | 1267.4 | 32630.1 | 105.8 | 15892891.1 | 4634762.5 |
| `join_count` | 38.4 | 11.9 | 12.8 | 195.4 | 48.2 |
| `join_groupby_count` | 158.8 | 19.8 | 22.0 | 209.5 | 70.3 |
| `join_groupby_to_array` | 192.4 | 19.6 | 21.9 | 215.8 | 80.3 |
| `join_select` | 92.9 | 20.0 | 22.6 | 202.0 | 75.4 |
| `join_where_count` | 39.3 | 19.6 | 21.9 | 207.7 | 64.4 |
| `last_match` | 0.0 | 0.5 | 1.4 | 170.2 | 89.4 |
| `long_count_aggregate` | 29.4 | 0.3 | 0.6 | 168.9 | 26.7 |
| `max_aggregate` | 31.1 | 0.3 | 0.5 | 168.2 | 16.9 |
| `min_aggregate` | 31.1 | 0.3 | 0.5 | 167.8 | 16.9 |
| `order_distinct_take` | 141.8 | 2.1 | 74.2 | 653.3 | 26.1 |
| `order_reverse_normalized` | 38.5 | 0.7 | 1.3 | 391.9 | 144.7 |
| `order_take_desc` | 38.5 | 0.7 | 1.3 | 375.1 | 145.7 |
| `reverse_distinct_by` | 297.1 | 2.6 | — | 171.1 | 166.2 |
| `reverse_take` | 0.0 | 0.0 | 1.1 | 158.0 | 156.6 |
| `reverse_take_select` | 0.0 | 0.0 | 1.1 | 158.5 | 156.3 |
| `select_count` | 0.1 | 0.0 | 0.0 | 169.2 | 66.5 |
| `select_where` | 108.2 | 4.1 | 5.6 | 174.2 | 95.8 |
| `select_where_count` | 32.7 | 0.3 | 0.6 | 179.4 | 28.1 |
| `select_where_order_take` | 36.8 | 0.7 | 1.4 | 272.8 | 90.2 |
| `select_where_sum` | 37.3 | 0.4 | 0.6 | 178.0 | 18.6 |
| `single_match` | 0.0 | 0.4 | 1.1 | 161.1 | 46.2 |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.7 | 1.6 |
| `skip_while_match` | 3.4 | 0.4 | 0.4 | 162.7 | 46.5 |
| `sort_first` | 37.8 | 0.4 | 1.3 | 378.3 | 145.5 |
| `sort_take` | 38.5 | 0.7 | 1.4 | 378.6 | 145.1 |
| `sort_take_select` | 38.5 | 0.7 | 1.3 | 376.7 | 143.0 |
| `sum_aggregate` | 30.2 | 0.3 | 0.1 | 168.2 | 16.8 |
| `sum_where` | 32.9 | 0.3 | 0.6 | 177.3 | 16.6 |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.7 | 1.5 |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 167.1 | 0.4 |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 162.6 | 0.2 |
| `take_where_count` | 0.9 | 0.0 | 0.0 | 1.8 | 0.2 |
| `take_while_match` | 7.8 | 0.2 | 0.3 | 85.7 | 17.0 |
| `to_array_filter` | 48.4 | 3.2 | 3.4 | 181.0 | 20.1 |
| `zip_count_pred` | 39.4 | 0.1 | — | 185.1 | 151.7 |
| `zip_dot_product` | — | 0.1 | 0.1 | 182.6 | 153.1 |
| `zip_dot_product_3arg` | — | 0.1 | — | 170.5 | 153.4 |
| `zip_reverse_to_array` | — | 4.5 | — | 187.9 | 161.9 |
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
