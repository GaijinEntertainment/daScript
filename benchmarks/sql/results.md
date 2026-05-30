# Benchmarks — SQL / Array / Decs / XML comparison

Updated 2026-05-30 (branch `bbatkin/linq-fold-zip-iterator-preservation`): **zip XML lanes (m5, m5f) now landed** across all four `zip_*` benches. The blocker was a real bug, not just an artificial source: `zip` had no mixed `(iterator, array)` overload, so `zip(xmlIter, arr)` — the natural ceremony-free form — failed to resolve, and `_fold`'s array-rewrite couldn't lower it (it produced `zip_to_array(iterator, array)`, also unoverloaded). The fix adds 2-source mixed `zip`/`zip_to_array` overloads (iterator-preserving; the private `zip_impl` already walked mixed sources). Each zip bench now zips the XML `Car` price-stream against a synthetic int array. **Finding: m5f modestly beats m5** (INTERP ~10–24%, JIT ~3–14% — e.g. `zip_dot_product` 508.7 → 431.3 INTERP, 181.6 → 165.0 JIT), so the zip splice *does* partially fuse over XML (unlike the cascade families, where m5f ≈ m5), but both lanes still pay the full unpruned `Car` materialization — every row clones its unused `name` string (~888k string-bytes/op) — which dominates the absolute cost. Field-pruning the zip source is the next lever. Earlier note:

Updated 2026-05-30 (branch `bbatkin/xml-bench-coverage`): **full XML coverage — both XML lanes (m5, m5f) now wired across every applicable family (67), up from 5.** Two changes: (1) **`m5` redefined** from the hand-written plain loop to the *un-fused tier-2 linq cascade* over the XML iterator — `unsafe(from_xml_node(root, type<Car>)) |> _where(…) |> … |> term()` — the same chain as m3f/m5f but without `_fold`, so **m5 vs m5f is now a clean fused-vs-not comparison on identical logic** (the prior hand-loop was an apples-to-oranges hand-optimal floor). The 5 pre-existing hand-loop m5 lanes were reworked to match. (2) **m5f added** to every family that had an array fold. The headline finding (see "Reading the XML lanes" below): the `XmlAdapter` **fuses the `loop_or_count` family** — where/select/aggregate/count/sum/min/max/average/first/element_at/any/all/contains/take/skip — where m5f runs **5–10× faster than m5** (e.g. INTERP `count_aggregate` 419 → **64**, `aggregate_match` 453 → **65**, `take_count_filtered` 395 → **1.3**), but it **does NOT fuse the cascade families** — group_by / distinct / order / sort / reverse / join — where **m5f ≈ m5** (e.g. `groupby_sum` 462.9 vs 465.1, `distinct_by_count` 393.4 vs 393.5, `sort_take` 1069 vs 1069, `join_count` 521 vs 484): the adapter falls back to full per-element materialization, so folding buys nothing. Those `m5f ≈ m5` cells are the map for the *next* arc (widening `XmlAdapter` to fuse the cascade families). **zip stayed `—` in this PR** — its source is a synthetic `make_ints(n)` int stream, not Car/XML, and the mixed `zip(iterator, array)` overload was still missing; both were addressed in the follow-up noted above. Earlier note:

Updated 2026-05-29 (branch `bbatkin/sqlite-linq-computed-select-subqueries`): **two `_sql` surface gains + 5 m1 backfills.** (1) Computed-scalar `_select` — `_select(_.a + _.b) |> sum/min/max/average/count()` now lowers (pred_to_sql renders the arithmetic into the aggregate argument) instead of being rejected as "`_.Field` only". (2) take/skip BEFORE an aggregate — `_where(P) |> take(n) |> count()` / `_select(_.X) |> take(n) |> sum()` wrap the bounded rows into an inner subquery (`SELECT COUNT(*) FROM (SELECT * FROM t WHERE P LIMIT n)`) so the LIMIT applies pre-aggregate, instead of the no-op-LIMIT rejection. New `m1` lanes: `join_select` (into-projection), `take_where_count`, `take_count_filtered`, `take_sum_aggregate`, `zip_count_pred` (degenerate same-row interpretation). The zip dot-product **sum** lanes (`zip_dot_product`, `zip_dot_product_3arg`) stay `—`: `SUM(price*year)` overflows int32 at n=100k and needs an int64-typed computed projection (`int64(...)` in `_select` currently fails inference) — deferred to the computed-cast follow-up. SQL coverage 62 → 67 / 72. Earlier note:

Updated 2026-05-29 (branch `bbatkin/linq-fold-xml-field-pruning`): **field-pruning landed (pass 2b)** — `XmlAdapter` now scans the chain body for the `Row` fields it actually reads and emits one `read_xml_field` per referenced field into scalar locals, dropping the per-element struct and every unread attribute read (notably the `string` clones). The four prunable XML families drop sharply and **JIT now beats SQLite**: `select_where_sum` m5f JIT 149.8 → **18.6** (m1 38.0), `aggregate_match` 153.9 → **21.6** (m1 35.0), `select_where_count` 152.1 → **27.5** (m1 33.2), `take_count_filtered` 3.1 → **0.4** — all alloc-free (the m5 baseline did 2.6M string clones over 100k rows; m5f does 0). `single_match` does **not** prune (`.single()` returns the whole matched row → whole-`it` escape → full `build_xml_row`), but its m5 baseline was **corrected this pass**: it previously did a find-first `break` (~`TARGET_ID` elements, yet still divided by n → a misleadingly tiny ns/op), and now scans-all with single() semantics like the fold lanes. So the fused m5f (146 JIT) correctly edges the generator m5 (152) instead of trailing a fake 0.1. Earlier note:

Updated 2026-05-29 (branch `bbatkin/linq-fold-xml-adapter`): added the **XML fold (m5f)** lane — the fused `_fold` path over `from_xml_node` (pass 2a). `XmlAdapter` (in `pugixml/linq_fold_xml`, pulled into `daslib/linq_fold` via the optional `require ?pugixml` + `static_if (typeinfo builtin_module_exists(pugixml))`) emits an inlined DOM child-element walk in place of the generator. The 2a scaffold still fully materializes each `Row` (the `name` string clone the query never reads is retained), so m5f was a modest win over the m5 baseline (generator + iterator overhead removed); pass 2b (above) adds the field-pruning that drops the wasted attribute reads + string clones. The matrices below are now regenerated by `benchmarks/sql/_update_results.das` from `--bench-format json` output — see "How to re-run". Earlier note:

Updated 2026-05-28 (branch `bbatkin/from-xml-node-linq-source`): added the **XML (m5)** lane — a non-fold baseline that iterates `from_xml_node(root, type<Car>)` (the new typed LINQ-over-XML source in `pugixml/PUGIXML_boost`) and runs the query with a plain loop. v1 has no `_fold` for XML, so m5 is the reference the fused XML lane (`m5f`, pass 2) will be measured against; it is wired into 5 representative single-source files so far (`select_where_count`, `select_where_sum`, `aggregate_match`, `single_match`, `take_count_filtered`) — every other row's m5 cell is `—`. The full-scan cells (~356 ns INTERP) are dominated by materializing all 100k `Car` rows — including a `name` string clone per row the query never reads — which is exactly the per-element materialization a fused lane elides; the early-exit cells (`take_count_filtered`; `single_match`'s baseline was later corrected to a full single() scan — see the top note) only materialize rows up to the break.

Prior baseline generated from PRs `bbatkin/linq-fold-array-join-splice` (chunk N+3 — linq_fold array-side `_join` splice + cross-arm `_join |> _group_by` via new `ArrayJoin` SourceAdapter) + follow-up `bbatkin/linq-fold-join-emit-dedup` (refactor: shared standalone + adapter helpers) + follow-up `bbatkin/linq-fold-decs-reverse-take-select` (extend decs skip-into-tail to handle trailing `_select`) + symmetric m3f follow-up `bbatkin/linq-fold-array-reverse-take-select` (extend array-side R6 backward-index walk to handle trailing `_select`). The first two close the m3f-vs-m4 parity gap across the entire `join_*` family — all 5 cells m3f beats m4 in both INTERP and JIT:

| Cell | m3f INTERP before / after | m3f JIT before / after |
|---|---:|---:|
| `join_count` | 128.2 → **51.0** | 34.6 → **11.6** |
| `join_select` | 148.1 → **72.0** | 41.6 → **19.7** |
| `join_where_count` | 148.5 → **60.1** | 41.2 → **20.4** |
| `join_groupby_count` | 186.2 → **78.3** | 47.2 → **19.4** |
| `join_groupby_to_array` | 217.8 → **78.1** | 55.5 → **19.5** |

Drift across the rest of the matrix is at measurement floor (±5% INTERP, ±10% JIT). The splice is gated on `array_source` + primitive equi-key + (for cross-arm) `upstream_join` capture, so non-join chains aren't touched.
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
- **Decs vs Array** — ratio `decs_ns / array_ns`. Values below 1× mean
  decs wins; values above mean array wins.

Sub-nanosecond cells (`0.00`) reflect early-exit terminators that exit
before the timer resolution can measure them — they should be read as
"effectively free."

### Reading the XML lanes (m5 vs m5f)

m5 is the **un-fused** XML cascade; m5f is the **fused** `_fold`. The two run
identical logic, so the delta is purely what the `XmlAdapter` fusion buys:

- **m5f ≪ m5 → fusion works.** The adapter inlines the DOM walk and prunes the
  materialization to the fields the chain reads. This covers the entire
  `loop_or_count` family — `where` / `select` / `count` / `sum` / `min` / `max`
  / `average` / `aggregate` / `first` / `last` / `single` / `element_at` / `any`
  / `all` / `contains` / `take` / `skip` / `take_while` / `skip_while`. Typical
  INTERP wins are 5–10× (e.g. `count_aggregate` 419 → 64, `aggregate_match`
  453 → 65; early-exit cells like `take_count_filtered` collapse to ~1).
- **m5f ≈ m5 → tier-2 fallback (no fusion).** The adapter has no specialized
  arm for the chain, so `_fold` falls back to full per-element materialization —
  identical work to the un-fused lane. This is every **cascade family**:
  `group_by` (incl. `having` / multi-reducer), `distinct[_by]`, `order_by[_descending]`,
  `sort`, `reverse`, and `join`. These rows are the **work-to-do map** for the
  next arc (teaching `XmlAdapter` to fuse the cascade families); the number is
  the honest un-fused cost, not an absent lane.

(The absolute XML numbers stay far above the array/decs lanes either way — XML
carries DOM-parse + per-element attribute reads + `string` clones the in-memory
lanes never pay. The m5↔m5f delta, not the XML-vs-array gap, is the fusion signal.)


<!-- BENCH:TABLES BEGIN -->
*Generated 2026-05-30 by `benchmarks/sql/_update_results.das` — ns/op; `—` = absent lane; Decs vs Array = m4/m3f. Edit the prose around the markers, not the tables.*

## INTERP

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML (m5) | XML fold (m5f) | Decs vs Array |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 35.3 | 6.0 | 5.8 | 453.4 | 65.0 | 0.98× |
| `all_match` | 28.2 | 3.5 | 3.5 | 379.6 | 56.0 | 1.00× |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.1 | 0.0 | — |
| `average_aggregate` | 30.7 | 5.9 | 8.8 | 430.0 | 59.8 | 1.49× |
| `bare_order_where` | 280.9 | 119.6 | 128.5 | 774.4 | 713.1 | 1.07× |
| `chained_select_collapse` | — | 17.9 | 17.8 | 539.6 | 407.7 | 0.99× |
| `chained_where` | 36.8 | 6.6 | 7.2 | 451.7 | 106.1 | 1.08× |
| `contains_match` | 0.0 | 2.3 | 1.4 | 415.5 | 29.0 | 0.61× |
| `count_aggregate` | 29.9 | 4.1 | 4.1 | 419.5 | 64.5 | 0.99× |
| `decs_count_bare_pred` | — | — | 4.1 | — | — | — |
| `distinct_by_count` | 47.6 | 16.0 | 15.9 | 393.4 | 393.5 | 0.99× |
| `distinct_by_order_take` | 262.2 | 21.9 | 23.3 | 401.1 | 402.5 | 1.06× |
| `distinct_by_order_to_array` | 241.1 | 22.1 | 23.5 | 402.1 | 401.1 | 1.06× |
| `distinct_count` | 42.2 | 16.0 | 16.0 | 475.4 | 406.5 | 1.00× |
| `distinct_count_pred` | 252.9 | 15.9 | 16.0 | 397.2 | 399.0 | 1.01× |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 391.4 | 408.1 | — |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 395.3 | 0.7 | — |
| `first_match` | 0.0 | 0.0 | 0.0 | 392.2 | 0.1 | — |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 393.8 | 0.1 | — |
| `groupby_average` | 171.5 | 30.5 | 30.1 | 464.8 | 469.3 | 0.99× |
| `groupby_count` | 143.0 | 19.2 | 19.3 | 431.5 | 437.7 | 1.00× |
| `groupby_first` | 255.2 | 18.5 | 19.2 | 429.7 | 435.4 | 1.04× |
| `groupby_having_count` | 141.8 | 19.3 | 19.2 | 437.0 | 446.5 | 1.00× |
| `groupby_having_hidden_sum` | 175.6 | 24.3 | 24.2 | 470.0 | 480.6 | 0.99× |
| `groupby_having_post_where` | 175.3 | 18.7 | 18.7 | 459.1 | 464.2 | 1.00× |
| `groupby_max` | 174.1 | 24.9 | 25.1 | 468.0 | 473.4 | 1.01× |
| `groupby_min` | 174.1 | 25.2 | 25.1 | 469.0 | 475.6 | 1.00× |
| `groupby_multi_reducer` | 190.3 | 32.7 | 32.4 | 499.8 | 505.4 | 0.99× |
| `groupby_select_order` | 171.3 | 18.6 | 18.7 | 458.8 | 462.4 | 1.00× |
| `groupby_select_sum` | 199.7 | 36.4 | 36.4 | 512.2 | 474.6 | 1.00× |
| `groupby_sum` | 171.6 | 18.6 | 18.8 | 462.9 | 465.1 | 1.01× |
| `groupby_where_count` | 75.5 | 14.4 | 15.1 | 458.5 | 437.2 | 1.05× |
| `groupby_where_sum` | 87.5 | 14.4 | 14.8 | 470.4 | 451.5 | 1.03× |
| `indexed_lookup` | 1444.1 | 198093.3 | 493.7 | 38299637.9 | 5768781.3 | 0.00× |
| `join_count` | 38.0 | 51.5 | 64.3 | 521.1 | 483.9 | 1.25× |
| `join_groupby_count` | 157.8 | 78.7 | 91.1 | 581.6 | 546.4 | 1.16× |
| `join_groupby_to_array` | 191.5 | 78.5 | 90.0 | 614.1 | 581.5 | 1.15× |
| `join_select` | 150.2 | 72.2 | 86.3 | 552.7 | 510.6 | 1.19× |
| `join_where_count` | 39.8 | 61.1 | 75.8 | 569.7 | 511.5 | 1.24× |
| `last_match` | 0.0 | 5.9 | 14.0 | 422.5 | 337.8 | 2.37× |
| `long_count_aggregate` | 29.7 | 4.1 | 4.1 | 421.1 | 63.9 | 1.00× |
| `max_aggregate` | 31.3 | 6.0 | 6.8 | 447.1 | 62.9 | 1.14× |
| `min_aggregate` | 31.4 | 6.0 | 6.8 | 445.1 | 58.3 | 1.15× |
| `order_distinct_take` | 138.5 | 15.8 | 93.9 | 2199.0 | 2089.0 | 5.94× |
| `order_reverse_normalized` | 38.6 | 16.7 | 20.2 | 1139.0 | 1084.3 | 1.21× |
| `order_take_desc` | 38.6 | 16.2 | 19.9 | 1065.9 | 1065.4 | 1.23× |
| `reverse_distinct_by` | 297.4 | 21.3 | — | 453.7 | 427.4 | — |
| `reverse_take` | 0.1 | 0.0 | 9.2 | 378.3 | 393.4 | — |
| `reverse_take_select` | 0.0 | 0.0 | 9.2 | 384.2 | 395.9 | — |
| `select_count` | 0.1 | 0.0 | 2.2 | 440.6 | 68.4 | — |
| `select_where` | 194.0 | 11.0 | 19.4 | 426.1 | 340.0 | 1.76× |
| `select_where_count` | 32.9 | 5.1 | 7.4 | 481.7 | 65.4 | 1.45× |
| `select_where_order_take` | 36.8 | 12.3 | 14.9 | 743.2 | 710.4 | 1.21× |
| `select_where_sum` | 37.4 | 7.4 | 7.5 | 480.7 | 67.3 | 1.01× |
| `single_match` | 0.0 | 2.8 | 5.5 | 384.1 | 330.2 | 1.97× |
| `skip_take` | 0.5 | 0.1 | 0.2 | 4.4 | 3.7 | 1.47× |
| `skip_while_match` | 3.5 | 5.3 | 5.3 | 408.5 | 59.0 | 1.00× |
| `sort_first` | 38.2 | 11.0 | 13.2 | 1065.0 | 1073.9 | 1.20× |
| `sort_take` | 38.7 | 16.3 | 20.6 | 1069.1 | 1069.0 | 1.27× |
| `sort_take_select` | 38.4 | 16.2 | 20.1 | 1076.7 | 1083.9 | 1.24× |
| `sum_aggregate` | 30.4 | 2.1 | 2.1 | 432.5 | 54.2 | 0.99× |
| `sum_where` | 33.3 | 4.3 | 4.3 | 450.8 | 64.4 | 0.99× |
| `take_count` | 3.6 | 0.2 | 0.4 | 4.3 | 3.6 | 1.82× |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 394.7 | 1.3 | 1.14× |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 391.7 | 0.6 | 1.05× |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 4.8 | 0.7 | 1.01× |
| `take_while_match` | 7.9 | 2.4 | 2.4 | 226.8 | 32.3 | 0.98× |
| `to_array_filter` | 72.8 | 14.9 | 15.0 | 465.8 | 74.9 | 1.00× |
| `zip_count_pred` | 39.4 | 17.2 | — | 535.4 | 451.9 | — |
| `zip_dot_product` | — | 13.5 | 10.7 | 508.7 | 431.3 | 0.79× |
| `zip_dot_product_3arg` | — | 13.5 | — | 431.5 | 392.6 | — |
| `zip_reverse_to_array` | — | 31.1 | — | 529.6 | 403.3 | — |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML (m5) | XML fold (m5f) | Decs vs Array |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 35.0 | 0.3 | 0.6 | 175.3 | 16.7 | 1.84× |
| `all_match` | 28.0 | 0.3 | 0.2 | 160.0 | 19.5 | 0.63× |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | — |
| `average_aggregate` | 30.0 | 1.0 | 3.6 | 168.7 | 18.2 | 3.76× |
| `bare_order_where` | 186.7 | 34.5 | 35.0 | 282.7 | 268.9 | 1.01× |
| `chained_select_collapse` | — | 2.1 | 2.1 | 183.8 | 162.7 | 0.99× |
| `chained_where` | 36.6 | 0.6 | 0.8 | 178.2 | 33.8 | 1.33× |
| `contains_match` | 0.0 | 0.2 | 0.1 | 163.5 | 17.4 | 0.64× |
| `count_aggregate` | 29.8 | 0.3 | 0.6 | 169.8 | 16.7 | 1.90× |
| `decs_count_bare_pred` | — | — | 0.6 | — | — | — |
| `distinct_by_count` | 41.8 | 2.1 | 2.1 | 158.4 | 158.7 | 1.00× |
| `distinct_by_order_take` | 240.3 | 2.7 | 3.2 | 170.0 | 169.8 | 1.22× |
| `distinct_by_order_to_array` | 240.3 | 2.7 | 3.3 | 174.4 | 170.5 | 1.21× |
| `distinct_count` | 41.9 | 2.1 | 2.1 | 168.5 | 161.2 | 1.00× |
| `distinct_count_pred` | 253.5 | 2.1 | 2.3 | 161.5 | 161.0 | 1.08× |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 160.0 | 163.4 | — |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 165.6 | 0.3 | — |
| `first_match` | 0.0 | 0.0 | 0.0 | 169.9 | 0.0 | — |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 168.6 | 0.0 | — |
| `groupby_average` | 171.7 | 2.6 | 2.9 | 186.7 | 184.8 | 1.12× |
| `groupby_count` | 142.7 | 2.4 | 2.5 | 174.9 | 173.2 | 1.06× |
| `groupby_first` | 252.1 | 2.2 | 3.1 | 175.0 | 173.4 | 1.41× |
| `groupby_having_count` | 142.9 | 2.4 | 2.5 | 178.7 | 175.6 | 1.06× |
| `groupby_having_hidden_sum` | 175.6 | 2.5 | 2.8 | 187.7 | 184.7 | 1.13× |
| `groupby_having_post_where` | 171.8 | 2.4 | 2.7 | 183.6 | 181.9 | 1.12× |
| `groupby_max` | 173.3 | 2.4 | 2.7 | 183.2 | 181.2 | 1.11× |
| `groupby_min` | 173.1 | 2.4 | 2.7 | 186.0 | 184.3 | 1.12× |
| `groupby_multi_reducer` | 190.2 | 2.7 | 3.0 | 191.5 | 190.7 | 1.11× |
| `groupby_select_order` | 175.4 | 2.4 | 2.7 | 181.6 | 180.2 | 1.11× |
| `groupby_select_sum` | 199.5 | 3.2 | 3.7 | 203.0 | 191.9 | 1.16× |
| `groupby_sum` | 171.9 | 2.4 | 2.7 | 184.7 | 183.8 | 1.11× |
| `groupby_where_count` | 76.3 | 1.5 | 1.8 | 180.9 | 175.6 | 1.21× |
| `groupby_where_sum` | 87.4 | 1.5 | 1.8 | 187.5 | 182.3 | 1.20× |
| `indexed_lookup` | 1262.0 | 32575.9 | 107.2 | 15863960.9 | 4704659.3 | 0.00× |
| `join_count` | 38.5 | 12.3 | 13.0 | 193.5 | 185.9 | 1.05× |
| `join_groupby_count` | 157.2 | 19.8 | 22.3 | 208.4 | 199.3 | 1.13× |
| `join_groupby_to_array` | 189.1 | 20.3 | 22.5 | 217.8 | 208.5 | 1.11× |
| `join_select` | 93.5 | 20.5 | 23.1 | 203.2 | 196.1 | 1.13× |
| `join_where_count` | 39.5 | 20.0 | 22.4 | 207.3 | 196.3 | 1.12× |
| `last_match` | 0.0 | 0.5 | 1.4 | 171.3 | 148.6 | 2.59× |
| `long_count_aggregate` | 29.9 | 0.3 | 0.6 | 168.6 | 26.7 | 1.84× |
| `max_aggregate` | 31.1 | 0.3 | 0.5 | 167.9 | 16.8 | 1.49× |
| `min_aggregate` | 30.9 | 0.3 | 0.5 | 167.6 | 17.0 | 1.50× |
| `order_distinct_take` | 138.7 | 2.2 | 75.4 | 648.4 | 622.4 | 34.84× |
| `order_reverse_normalized` | 38.6 | 0.7 | 1.4 | 394.0 | 384.7 | 1.95× |
| `order_take_desc` | 38.6 | 0.7 | 1.4 | 375.5 | 376.9 | 1.94× |
| `reverse_distinct_by` | 296.7 | 2.6 | — | 170.8 | 166.1 | — |
| `reverse_take` | 0.0 | 0.0 | 1.1 | 158.1 | 161.3 | — |
| `reverse_take_select` | 0.0 | 0.0 | 1.1 | 156.2 | 156.8 | — |
| `select_count` | 0.1 | 0.0 | 0.0 | 167.6 | 66.3 | — |
| `select_where` | 108.1 | 4.1 | 5.5 | 176.6 | 154.4 | 1.33× |
| `select_where_count` | 33.1 | 0.3 | 0.6 | 179.0 | 28.1 | 1.87× |
| `select_where_order_take` | 36.9 | 0.7 | 1.4 | 274.6 | 268.1 | 1.86× |
| `select_where_sum` | 37.3 | 0.4 | 0.6 | 178.1 | 19.2 | 1.57× |
| `single_match` | 0.0 | 0.4 | 1.1 | 161.2 | 147.8 | 3.23× |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.7 | 1.6 | — |
| `skip_while_match` | 3.4 | 0.4 | 0.4 | 161.6 | 46.7 | 1.01× |
| `sort_first` | 38.1 | 0.4 | 1.3 | 378.0 | 374.4 | 3.35× |
| `sort_take` | 38.6 | 0.7 | 1.4 | 382.3 | 381.2 | 1.93× |
| `sort_take_select` | 38.5 | 0.7 | 1.4 | 379.0 | 374.5 | 1.94× |
| `sum_aggregate` | 30.4 | 0.3 | 0.1 | 168.3 | 16.7 | 0.15× |
| `sum_where` | 33.0 | 0.3 | 0.6 | 177.9 | 16.6 | 1.97× |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.7 | 1.6 | 1.56× |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 167.0 | 0.4 | — |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 162.9 | 0.2 | — |
| `take_where_count` | 0.9 | 0.0 | 0.0 | 1.8 | 0.2 | — |
| `take_while_match` | 7.8 | 0.2 | 0.3 | 85.6 | 17.6 | 1.52× |
| `to_array_filter` | 48.7 | 3.3 | 3.4 | 178.9 | 20.2 | 1.04× |
| `zip_count_pred` | 39.4 | 0.1 | — | 185.6 | 168.2 | — |
| `zip_dot_product` | — | 0.1 | 0.1 | 181.6 | 165.0 | 0.77× |
| `zip_dot_product_3arg` | — | 0.1 | — | 170.4 | 165.0 | — |
| `zip_reverse_to_array` | — | 4.6 | — | 189.5 | 163.0 | — |
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
- **`indexed_lookup` Decs vs Array ratio** — the ratio is reported as
  `0.00×` because array's lane measures the unspliced linear scan
  (~204k ns/op), while decs uses `query(eid)` for O(1) lookup. The
  ratio is mathematically tiny but the comparison is between two
  fundamentally different algorithms — not a like-for-like benchmark.
- **`reverse_distinct_by` Decs** — splice arm is array-only
  (`array_source` predicate in `daslib/linq_fold.das:3221`) because
  backward-index walk has no archetype analog. SQL lane closed by
  Arm B of this PR via MAX(pk) bare-aggregate.
- **`order_distinct_take` Decs vs Array ratio** — m4 (95.5 INTERP /
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

# regenerate the INTERP/JIT tables in place (ns/op = time_ns / n, m4/m3f ratio derived)
build/daslang benchmarks/sql/_update_results.das -- \
  --interp /tmp/bench_interp.json --jit /tmp/bench_jit.json --date YYYY-MM-DD
```

Edit the prose around the markers (this section, the header narrative,
"Notes on missing lanes") by hand; the tool only rewrites the tables.

If JIT fails to compile (e.g. `error[30341]: no matching functions or
generics: host_jit_triple()`), the `bin/daslang` binary is stale
relative to `src/builtin/module_jit.cpp` — rebuild with
`cmake --build build --config Release -j 64` and retry.
