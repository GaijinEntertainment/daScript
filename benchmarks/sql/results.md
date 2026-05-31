# Benchmarks — SQL / Array / Decs / XML comparison

Updated 2026-05-30 (branch `bbatkin/linq-fold-join-select-iterator-fuse`): **XML `join |> select` with no terminator now fuses** instead of bailing to tier-2. An iterator-typed result (`_fold(from_xml |> _join |> _select)`, no `to_array`/`count`) used to hit the `!countOnly && expr_is_iterator` bail in `XmlAdapter.emit_join_hook` — falling to tier-2, which materialized full join tuples (`join_to_array`) then lazily projected. The bail existed because the emit returns an array but the `_fold` contract wants a sequence. Fix: thread `wrapIter = !countOnly && expr_is_iterator` into the invoke builder (which already supports `.to_sequence_move()`), so the fused single-pass build runs and its field-pruned result array is exposed as a sequence — no separate `join_to_array` pass. **Design validated by hand-coded micro-bench first** (`benchmarks/micro/join_select_shapes.das`): fused-materialize beats a streaming generator in every config (generator-resume overhead dominates; the per-element `from_xml` string clone is a flat, orthogonal ~23-34ns cost). Win: `join_select` m5f **510.7 → 192.4** INTERP (2.65×, 1 alloc/op), **196.9 → 76.2** JIT (2.58×). XML-only — array (eager-typed) and decs (`to_array`-terminated m4) already fused; decs's own iterator bail is left for a separate profile.

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
- **m5f ≈ m5 → modest fusion only.** Whole-row-buffering cascades (`reverse |> to_array` —
  reverse buffers whole rows so the row escapes → no pruning) give only the modest
  win (intermediate array elided, string-clone floor remains).

(The absolute XML numbers stay far above the array/decs lanes either way — XML
carries DOM-parse + per-element attribute reads + `string` clones the in-memory
lanes never pay. The m5↔m5f delta, not the XML-vs-array gap, is the fusion signal.)


<!-- BENCH:TABLES BEGIN -->
*Generated 2026-05-30 by `benchmarks/sql/_update_results.das` — ns/op; `—` = absent lane. Edit the prose around the markers, not the tables.*

## INTERP

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML (m5) | XML fold (m5f) |
|---|---:|---:|---:|---:|---:|
| `aggregate_match` | 35.1 | 6.0 | 5.8 | 451.6 | 59.8 |
| `all_match` | 27.8 | 3.4 | 3.6 | 380.2 | 55.5 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.1 | 0.0 |
| `average_aggregate` | 31.3 | 6.0 | 8.8 | 431.5 | 57.9 |
| `bare_order_where` | 276.4 | 117.0 | 125.0 | 802.2 | 447.5 |
| `chained_select_collapse` | — | 17.6 | 17.9 | 538.1 | 70.2 |
| `chained_where` | 36.7 | 6.6 | 7.2 | 449.3 | 105.3 |
| `contains_match` | 0.0 | 2.2 | 1.4 | 411.1 | 28.2 |
| `count_aggregate` | 29.8 | 4.1 | 4.1 | 416.0 | 64.4 |
| `decs_count_bare_pred` | — | — | 4.1 | — | — |
| `distinct_by_count` | 41.6 | 15.8 | 16.0 | 389.5 | 70.2 |
| `distinct_by_order_take` | 254.8 | 21.9 | 23.3 | 401.0 | 359.0 |
| `distinct_by_order_to_array` | 239.9 | 21.9 | 23.2 | 399.7 | 357.1 |
| `distinct_count` | 41.5 | 21.0 | 15.7 | 473.4 | 71.1 |
| `distinct_count_pred` | 252.2 | 15.9 | 16.0 | 393.2 | 116.4 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 390.4 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 394.7 | 0.7 |
| `first_match` | 0.0 | 0.0 | 0.0 | 392.8 | 0.1 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 393.0 | 0.1 |
| `groupby_average` | 172.8 | 30.3 | 30.1 | 466.6 | 124.8 |
| `groupby_count` | 142.8 | 19.1 | 19.3 | 430.5 | 76.6 |
| `groupby_first` | 252.1 | 18.4 | 19.1 | 434.9 | 346.1 |
| `groupby_having_count` | 142.6 | 19.2 | 19.2 | 437.0 | 76.6 |
| `groupby_having_hidden_sum` | 176.4 | 24.0 | 24.2 | 467.5 | 122.4 |
| `groupby_having_post_where` | 172.1 | 18.7 | 18.7 | 464.7 | 115.6 |
| `groupby_max` | 174.6 | 24.7 | 25.1 | 474.9 | 120.8 |
| `groupby_min` | 173.8 | 25.2 | 25.4 | 471.2 | 120.8 |
| `groupby_multi_reducer` | 190.8 | 32.9 | 32.0 | 503.5 | 127.8 |
| `groupby_select_order` | 170.3 | 18.6 | 18.8 | 463.6 | 115.3 |
| `groupby_select_sum` | 200.1 | 36.9 | 36.0 | 512.6 | 102.0 |
| `groupby_sum` | 170.0 | 18.6 | 18.8 | 467.7 | 115.7 |
| `groupby_where_count` | 75.9 | 14.5 | 14.9 | 454.1 | 119.4 |
| `groupby_where_sum` | 87.3 | 14.2 | 14.8 | 469.3 | 123.3 |
| `indexed_lookup` | 1432.6 | 197689.9 | 481.3 | 38356136.6 | 5756561.1 |
| `join_count` | 38.5 | 51.3 | 65.8 | 519.3 | 114.1 |
| `join_groupby_count` | 157.8 | 79.2 | 91.1 | 581.4 | 182.1 |
| `join_groupby_to_array` | 193.3 | 78.5 | 91.4 | 609.3 | 217.1 |
| `join_select` | 150.2 | 72.3 | 85.6 | 549.8 | 192.4 |
| `join_where_count` | 39.5 | 61.4 | 76.3 | 576.5 | 164.6 |
| `last_match` | 0.0 | 5.9 | 14.0 | 419.0 | 335.0 |
| `long_count_aggregate` | 30.1 | 4.1 | 4.1 | 418.0 | 63.8 |
| `max_aggregate` | 31.2 | 5.9 | 6.8 | 441.5 | 58.3 |
| `min_aggregate` | 31.0 | 6.0 | 6.9 | 441.5 | 58.2 |
| `order_distinct_take` | 138.5 | 15.8 | 95.2 | 2207.0 | 72.5 |
| `order_reverse_normalized` | 38.3 | 16.2 | 19.9 | 1137.3 | 341.4 |
| `order_take_desc` | 38.5 | 16.2 | 20.1 | 1055.1 | 342.1 |
| `reverse_distinct_by` | 296.7 | 21.3 | — | 454.1 | 427.9 |
| `reverse_take` | 0.1 | 0.0 | 9.2 | 378.2 | 358.4 |
| `reverse_take_select` | 0.0 | 0.0 | 9.2 | 381.0 | 360.7 |
| `select_count` | 0.1 | 0.0 | 2.2 | 437.5 | 69.3 |
| `select_where` | 194.1 | 11.2 | 19.6 | 423.7 | 338.9 |
| `select_where_count` | 33.0 | 5.1 | 7.4 | 479.6 | 65.3 |
| `select_where_order_take` | 36.6 | 12.3 | 14.9 | 747.9 | 340.6 |
| `select_where_sum` | 37.3 | 7.4 | 7.5 | 483.2 | 67.6 |
| `single_match` | 0.0 | 2.8 | 5.5 | 388.3 | 329.9 |
| `skip_take` | 0.5 | 0.1 | 0.2 | 4.4 | 3.7 |
| `skip_while_match` | 3.5 | 5.2 | 5.4 | 404.6 | 59.1 |
| `sort_first` | 38.1 | 11.0 | 13.2 | 1090.8 | 340.0 |
| `sort_take` | 38.6 | 16.6 | 20.7 | 1097.0 | 366.2 |
| `sort_take_select` | 38.6 | 16.2 | 20.2 | 1071.1 | 344.2 |
| `sum_aggregate` | 30.3 | 2.1 | 2.1 | 430.7 | 53.9 |
| `sum_where` | 33.3 | 4.3 | 4.3 | 450.5 | 64.2 |
| `take_count` | 3.6 | 0.2 | 0.4 | 4.3 | 3.6 |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 393.8 | 1.3 |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 394.8 | 0.6 |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 4.7 | 0.7 |
| `take_while_match` | 7.8 | 2.4 | 2.4 | 229.1 | 30.0 |
| `to_array_filter` | 72.8 | 11.6 | 11.6 | 463.0 | 72.4 |
| `zip_count_pred` | 39.6 | 17.2 | — | 535.6 | 382.6 |
| `zip_dot_product` | — | 13.5 | 10.8 | 506.4 | 367.2 |
| `zip_dot_product_3arg` | — | 13.5 | — | 427.9 | 368.4 |
| `zip_reverse_to_array` | — | 31.5 | — | 529.3 | 399.6 |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML (m5) | XML fold (m5f) |
|---|---:|---:|---:|---:|---:|
| `aggregate_match` | 34.6 | 0.3 | 0.6 | 174.8 | 16.5 |
| `all_match` | 27.9 | 0.3 | 0.2 | 160.3 | 19.3 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.5 | 1.0 | 3.6 | 169.0 | 18.0 |
| `bare_order_where` | 187.6 | 33.9 | 35.3 | 281.0 | 180.5 |
| `chained_select_collapse` | — | 2.1 | 2.1 | 182.6 | 21.3 |
| `chained_where` | 36.7 | 0.6 | 0.8 | 178.1 | 33.9 |
| `contains_match` | 0.0 | 0.2 | 0.1 | 164.3 | 17.1 |
| `count_aggregate` | 29.4 | 0.3 | 0.6 | 169.9 | 16.5 |
| `decs_count_bare_pred` | — | — | 0.6 | — | — |
| `distinct_by_count` | 41.5 | 2.1 | 2.1 | 159.0 | 21.3 |
| `distinct_by_order_take` | 239.2 | 2.6 | 3.2 | 171.9 | 162.6 |
| `distinct_by_order_to_array` | 238.7 | 2.7 | 3.3 | 171.2 | 160.9 |
| `distinct_count` | 41.7 | 2.1 | 2.1 | 173.6 | 21.3 |
| `distinct_count_pred` | 250.9 | 2.1 | 2.3 | 160.8 | 40.4 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 161.7 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 165.5 | 0.3 |
| `first_match` | 0.0 | 0.0 | 0.0 | 169.7 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 167.9 | 0.0 |
| `groupby_average` | 171.1 | 2.6 | 2.9 | 185.3 | 36.3 |
| `groupby_count` | 142.8 | 2.4 | 2.5 | 175.3 | 23.2 |
| `groupby_first` | 254.3 | 2.2 | 3.1 | 176.4 | 147.3 |
| `groupby_having_count` | 141.7 | 2.3 | 2.5 | 178.2 | 21.4 |
| `groupby_having_hidden_sum` | 176.7 | 2.5 | 2.8 | 187.6 | 37.9 |
| `groupby_having_post_where` | 173.2 | 2.4 | 2.7 | 183.1 | 37.4 |
| `groupby_max` | 173.1 | 2.4 | 2.7 | 186.4 | 36.2 |
| `groupby_min` | 172.2 | 2.4 | 2.7 | 186.2 | 36.9 |
| `groupby_multi_reducer` | 191.1 | 2.7 | 3.0 | 193.8 | 37.9 |
| `groupby_select_order` | 171.2 | 2.4 | 2.7 | 184.1 | 38.0 |
| `groupby_select_sum` | 198.9 | 3.2 | 3.7 | 198.6 | 32.2 |
| `groupby_sum` | 170.1 | 2.4 | 2.7 | 185.2 | 36.8 |
| `groupby_where_count` | 76.0 | 1.5 | 1.8 | 179.3 | 37.0 |
| `groupby_where_sum` | 86.3 | 1.5 | 1.8 | 182.8 | 36.3 |
| `indexed_lookup` | 1235.6 | 32366.2 | 105.3 | 15728589.3 | 4679606.6 |
| `join_count` | 38.4 | 12.4 | 12.9 | 194.7 | 46.2 |
| `join_groupby_count` | 163.0 | 19.8 | 22.5 | 209.9 | 70.6 |
| `join_groupby_to_array` | 197.1 | 19.8 | 22.4 | 216.7 | 80.8 |
| `join_select` | 93.7 | 20.4 | 22.9 | 203.6 | 76.2 |
| `join_where_count` | 39.2 | 19.9 | 22.2 | 207.6 | 64.6 |
| `last_match` | 0.0 | 0.5 | 1.4 | 171.1 | 148.3 |
| `long_count_aggregate` | 29.7 | 0.3 | 0.6 | 168.3 | 26.7 |
| `max_aggregate` | 31.1 | 0.3 | 0.5 | 167.7 | 16.8 |
| `min_aggregate` | 31.0 | 0.3 | 0.5 | 168.1 | 16.8 |
| `order_distinct_take` | 139.8 | 2.1 | 75.3 | 639.7 | 22.3 |
| `order_reverse_normalized` | 38.6 | 0.7 | 1.4 | 390.7 | 144.2 |
| `order_take_desc` | 38.2 | 0.7 | 1.4 | 383.5 | 145.3 |
| `reverse_distinct_by` | 298.5 | 2.6 | — | 170.7 | 165.4 |
| `reverse_take` | 0.0 | 0.0 | 1.1 | 158.2 | 156.8 |
| `reverse_take_select` | 0.0 | 0.0 | 1.1 | 158.2 | 156.0 |
| `select_count` | 0.1 | 0.0 | 0.0 | 167.4 | 67.3 |
| `select_where` | 107.5 | 4.1 | 5.6 | 176.7 | 154.3 |
| `select_where_count` | 32.5 | 0.3 | 0.6 | 178.9 | 28.1 |
| `select_where_order_take` | 36.8 | 0.7 | 1.4 | 277.2 | 148.1 |
| `select_where_sum` | 37.2 | 0.4 | 0.6 | 177.9 | 18.6 |
| `single_match` | 0.0 | 0.4 | 1.1 | 161.0 | 147.9 |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.7 | 1.6 |
| `skip_while_match` | 3.5 | 0.4 | 0.4 | 161.5 | 46.5 |
| `sort_first` | 38.1 | 0.4 | 1.3 | 374.8 | 145.2 |
| `sort_take` | 38.6 | 0.7 | 1.3 | 377.8 | 144.8 |
| `sort_take_select` | 38.2 | 0.7 | 1.4 | 376.7 | 142.5 |
| `sum_aggregate` | 30.1 | 0.3 | 0.1 | 168.2 | 16.6 |
| `sum_where` | 32.6 | 0.3 | 0.6 | 177.4 | 16.5 |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.7 | 1.6 |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 166.8 | 0.4 |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 163.8 | 0.2 |
| `take_where_count` | 0.9 | 0.0 | 0.0 | 1.8 | 0.2 |
| `take_while_match` | 7.8 | 0.2 | 0.3 | 85.4 | 17.6 |
| `to_array_filter` | 48.7 | 3.3 | 3.4 | 178.6 | 20.1 |
| `zip_count_pred` | 39.2 | 0.1 | — | 184.5 | 151.2 |
| `zip_dot_product` | — | 0.1 | 0.1 | 181.8 | 153.2 |
| `zip_dot_product_3arg` | — | 0.1 | — | 171.0 | 154.2 |
| `zip_reverse_to_array` | — | 4.5 | — | 188.6 | 162.0 |
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
