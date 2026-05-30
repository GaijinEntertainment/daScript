# Benchmarks — SQL / Array / Decs / XML comparison

Updated 2026-05-30 (branch `bbatkin/linq-fold-leading-select-absorption`): **a leading `_select(f)` source projection no longer blocks order/distinct fusion.** A chain whose first op projects the source (`source |> _select(f) |> order |> distinct |> take`) used to fall to tier-2 — the order-family matcher had no leading-select slot, so the whole chain materialized + sorted instead of fusing single-pass. The m3f/m4 bench lanes hid this by pre-projecting their source array; the XML lane can't (its source carries whole `Car`s), so `order_distinct_take` m5f sat at the cascade floor. Fix: an optional leading `srcsel` slot on the four `wrap_source_loop`-based order rows, plus a `ProjectedSourceAdapter` decorator that binds `projName = f(rawElem)` atop the per-element body and delegates the loop to the inner adapter — so every emit sees the projected element with no emit-fn changes, and XML field-pruning is preserved (f's `it.<field>` reads reach the materializer). Win: `order_distinct_take` m5f **2091.8 → 72.5** INTERP (28.9×, 0 string clones — reads only the projected `brand`), **629.2 → 22.3** JIT (28.2×); single-pass dedup-walk + bounded top-N replaces materialize-all + sort-all. The slot is optional, so non-leading-select chains stay byte-identical. Earlier note:

Updated 2026-05-30 (branch `bbatkin/linq-fold-join-unify-xml`): **XML `join` (and `group_by` with an upstream `join`) now fuses** — the last family the cascade arc deferred. Two parts: (1) an **architecture refactor** — the two parallel join splice patterns (`array_join_general` + `decs_join_general`) collapse into one `join_general` gated by a `can_join` capability, dispatching the source-specific body through a renamed `emit_join_hook` (a new source adds one override, not a parallel pattern); **zero perf delta** — each adapter's emitted AST is byte-identical (the gate moves `requires`→hook, and `run_splice_adapter` cascades to tier-2 identically on a failed `requires` or a null emit). (2) **XML join fuses** — `XmlAdapter.emit_join_hook` collects the in-memory srcB into a hash and probes from the field-pruned DOM walk; `build_group_by_adapter` returns an `XmlJoinAdapter` on the upstream-join arm so `join |> group_by` fuses single-pass. Wins (INTERP m5f): `join_count` 519 → **114** (4.6×, 0 string-clones — count needs only the join key), `join_where_count` 577 → **165** (3.5×), `join_groupby_count` 581 → **182** (3.2×), `join_groupby_to_array` 609 → **217** (2.8×). `join_select` stays modest (550 → 511) — its result selector keeps a `name` string field, so field-pruning can't drop the clone. Earlier note:

Updated 2026-05-30 (branch `bbatkin/linq-fold-xml-cascade-fusion`): **XML cascade fusion — `group_by` / `distinct[_by]` / `order_by[_descending]` / `sort` / `reverse` now fuse over the `XmlAdapter`.** Previously only the `loop_or_count` family fused (XML was hard-restricted to one splice pattern); the cascade families fell to the tier-2 cascade, so m5f ≈ m5. Two changes: (1) an **architecture refactor** — `AdapterKind`/`kind()` (which needed a new enum value + switch arm per source) is replaced by capability methods the adapter answers about itself (`can_group_by`, `can_reserve_by_length`, `can_ride_array_lane`, …), and the source adapter is threaded into the splice-pattern `requires` predicates so gating asks the adapter; a new source (`linq_json`, …) now only implements the methods. (2) **lift the XML splice gate** so the cascade planners run with the `XmlAdapter`, driving the DOM walk through `wrap_source_loop` — which **field-prunes for free**: where the chain reduces/projects (not the whole row), the per-element body reads only the touched attributes and the per-row `name` string-clone disappears. Wins (INTERP m5f): `groupby_sum` 462 → **115** (4×, 0 clones), `groupby_count` 431 → **77** (5.6×), `distinct_by_count` 392 → **71** (5.5×), `distinct_count` 473 → **71** (6.7×), `distinct_take` 394 → **0.0** (early-exit), `sort_take`/`order_take_desc` ~1080 → **~340** (3.2×). Whole-row-buffering shapes stay modest (`reverse_take` 379 → 359; `order_distinct_take` 2203 → 2082 — order buffers whole rows). **`join` (and `group_by` with an upstream `join`) still falls to tier-2** — the hashed collect+probe driven from the DOM walk is the next arc. Earlier note:

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
- **m5f ≈ m5 → modest fusion only.** Whole-row-buffering cascades (`reverse |> to_array` —
  reverse buffers whole rows so the row escapes → no pruning), and joins whose
  result keeps a string field (`join_select` 550 → 511 — the `(name, name)`
  selector clones `name`, so pruning can't drop it) give only the modest win
  (intermediate array elided, string-clone floor remains).

(The absolute XML numbers stay far above the array/decs lanes either way — XML
carries DOM-parse + per-element attribute reads + `string` clones the in-memory
lanes never pay. The m5↔m5f delta, not the XML-vs-array gap, is the fusion signal.)


<!-- BENCH:TABLES BEGIN -->
*Generated 2026-05-30 by `benchmarks/sql/_update_results.das` — ns/op; `—` = absent lane; Decs vs Array = m4/m3f. Edit the prose around the markers, not the tables.*

## INTERP

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML (m5) | XML fold (m5f) | Decs vs Array |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 35.1 | 6.0 | 5.8 | 451.6 | 59.8 | 0.97× |
| `all_match` | 27.8 | 3.4 | 3.6 | 380.2 | 55.5 | 1.04× |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.1 | 0.0 | — |
| `average_aggregate` | 31.3 | 6.0 | 8.8 | 431.5 | 57.9 | 1.47× |
| `bare_order_where` | 276.4 | 117.0 | 125.0 | 802.2 | 447.5 | 1.07× |
| `chained_select_collapse` | — | 17.6 | 17.9 | 538.1 | 70.2 | 1.01× |
| `chained_where` | 36.7 | 6.6 | 7.2 | 449.3 | 105.3 | 1.09× |
| `contains_match` | 0.0 | 2.2 | 1.4 | 411.1 | 28.2 | 0.62× |
| `count_aggregate` | 29.8 | 4.1 | 4.1 | 416.0 | 64.4 | 1.00× |
| `decs_count_bare_pred` | — | — | 4.1 | — | — | — |
| `distinct_by_count` | 41.6 | 15.8 | 16.0 | 389.5 | 70.2 | 1.02× |
| `distinct_by_order_take` | 254.8 | 21.9 | 23.3 | 401.0 | 359.0 | 1.06× |
| `distinct_by_order_to_array` | 239.9 | 21.9 | 23.2 | 399.7 | 357.1 | 1.06× |
| `distinct_count` | 41.5 | 21.0 | 15.7 | 473.4 | 71.1 | 0.75× |
| `distinct_count_pred` | 252.2 | 15.9 | 16.0 | 393.2 | 116.4 | 1.00× |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 390.4 | 0.0 | — |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 394.7 | 0.7 | — |
| `first_match` | 0.0 | 0.0 | 0.0 | 392.8 | 0.1 | — |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 393.0 | 0.1 | — |
| `groupby_average` | 172.8 | 30.3 | 30.1 | 466.6 | 124.8 | 0.99× |
| `groupby_count` | 142.8 | 19.1 | 19.3 | 430.5 | 76.6 | 1.01× |
| `groupby_first` | 252.1 | 18.4 | 19.1 | 434.9 | 346.1 | 1.04× |
| `groupby_having_count` | 142.6 | 19.2 | 19.2 | 437.0 | 76.6 | 1.00× |
| `groupby_having_hidden_sum` | 176.4 | 24.0 | 24.2 | 467.5 | 122.4 | 1.01× |
| `groupby_having_post_where` | 172.1 | 18.7 | 18.7 | 464.7 | 115.6 | 1.00× |
| `groupby_max` | 174.6 | 24.7 | 25.1 | 474.9 | 120.8 | 1.01× |
| `groupby_min` | 173.8 | 25.2 | 25.4 | 471.2 | 120.8 | 1.01× |
| `groupby_multi_reducer` | 190.8 | 32.9 | 32.0 | 503.5 | 127.8 | 0.97× |
| `groupby_select_order` | 170.3 | 18.6 | 18.8 | 463.6 | 115.3 | 1.01× |
| `groupby_select_sum` | 200.1 | 36.9 | 36.0 | 512.6 | 102.0 | 0.97× |
| `groupby_sum` | 170.0 | 18.6 | 18.8 | 467.7 | 115.7 | 1.01× |
| `groupby_where_count` | 75.9 | 14.5 | 14.9 | 454.1 | 119.4 | 1.03× |
| `groupby_where_sum` | 87.3 | 14.2 | 14.8 | 469.3 | 123.3 | 1.04× |
| `indexed_lookup` | 1432.6 | 197689.9 | 481.3 | 38356136.6 | 5756561.1 | 0.00× |
| `join_count` | 38.5 | 51.3 | 65.8 | 519.3 | 114.1 | 1.28× |
| `join_groupby_count` | 157.8 | 79.2 | 91.1 | 581.4 | 182.1 | 1.15× |
| `join_groupby_to_array` | 193.3 | 78.5 | 91.4 | 609.3 | 217.1 | 1.16× |
| `join_select` | 150.2 | 72.3 | 85.6 | 549.8 | 510.7 | 1.18× |
| `join_where_count` | 39.5 | 61.4 | 76.3 | 576.5 | 164.6 | 1.24× |
| `last_match` | 0.0 | 5.9 | 14.0 | 419.0 | 335.0 | 2.36× |
| `long_count_aggregate` | 30.1 | 4.1 | 4.1 | 418.0 | 63.8 | 1.00× |
| `max_aggregate` | 31.2 | 5.9 | 6.8 | 441.5 | 58.3 | 1.15× |
| `min_aggregate` | 31.0 | 6.0 | 6.9 | 441.5 | 58.2 | 1.14× |
| `order_distinct_take` | 138.5 | 15.8 | 95.2 | 2207.0 | 72.5 | 6.02× |
| `order_reverse_normalized` | 38.3 | 16.2 | 19.9 | 1137.3 | 341.4 | 1.23× |
| `order_take_desc` | 38.5 | 16.2 | 20.1 | 1055.1 | 342.1 | 1.24× |
| `reverse_distinct_by` | 296.7 | 21.3 | — | 454.1 | 427.9 | — |
| `reverse_take` | 0.1 | 0.0 | 9.2 | 378.2 | 358.4 | — |
| `reverse_take_select` | 0.0 | 0.0 | 9.2 | 381.0 | 360.7 | — |
| `select_count` | 0.1 | 0.0 | 2.2 | 437.5 | 69.3 | — |
| `select_where` | 194.1 | 11.2 | 19.6 | 423.7 | 338.9 | 1.75× |
| `select_where_count` | 33.0 | 5.1 | 7.4 | 479.6 | 65.3 | 1.45× |
| `select_where_order_take` | 36.6 | 12.3 | 14.9 | 747.9 | 340.6 | 1.22× |
| `select_where_sum` | 37.3 | 7.4 | 7.5 | 483.2 | 67.6 | 1.02× |
| `single_match` | 0.0 | 2.8 | 5.5 | 388.3 | 329.9 | 1.97× |
| `skip_take` | 0.5 | 0.1 | 0.2 | 4.4 | 3.7 | 1.45× |
| `skip_while_match` | 3.5 | 5.2 | 5.4 | 404.6 | 59.1 | 1.03× |
| `sort_first` | 38.1 | 11.0 | 13.2 | 1090.8 | 340.0 | 1.20× |
| `sort_take` | 38.6 | 16.6 | 20.7 | 1097.0 | 366.2 | 1.25× |
| `sort_take_select` | 38.6 | 16.2 | 20.2 | 1071.1 | 344.2 | 1.24× |
| `sum_aggregate` | 30.3 | 2.1 | 2.1 | 430.7 | 53.9 | 1.00× |
| `sum_where` | 33.3 | 4.3 | 4.3 | 450.5 | 64.2 | 1.00× |
| `take_count` | 3.6 | 0.2 | 0.4 | 4.3 | 3.6 | 1.90× |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 393.8 | 1.3 | 1.14× |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 394.8 | 0.6 | 1.05× |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 4.7 | 0.7 | 1.03× |
| `take_while_match` | 7.8 | 2.4 | 2.4 | 229.1 | 30.0 | 1.00× |
| `to_array_filter` | 72.8 | 11.6 | 11.6 | 463.0 | 72.4 | 1.00× |
| `zip_count_pred` | 39.6 | 17.2 | — | 535.6 | 382.6 | — |
| `zip_dot_product` | — | 13.5 | 10.8 | 506.4 | 367.2 | 0.80× |
| `zip_dot_product_3arg` | — | 13.5 | — | 427.9 | 368.4 | — |
| `zip_reverse_to_array` | — | 31.5 | — | 529.3 | 399.6 | — |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML (m5) | XML fold (m5f) | Decs vs Array |
|---|---:|---:|---:|---:|---:|---:|
| `aggregate_match` | 34.6 | 0.3 | 0.6 | 174.8 | 16.5 | 1.85× |
| `all_match` | 27.9 | 0.3 | 0.2 | 160.3 | 19.3 | 0.63× |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 | — |
| `average_aggregate` | 30.5 | 1.0 | 3.6 | 169.0 | 18.0 | 3.68× |
| `bare_order_where` | 187.6 | 33.9 | 35.3 | 281.0 | 180.5 | 1.04× |
| `chained_select_collapse` | — | 2.1 | 2.1 | 182.6 | 21.3 | 1.00× |
| `chained_where` | 36.7 | 0.6 | 0.8 | 178.1 | 33.9 | 1.35× |
| `contains_match` | 0.0 | 0.2 | 0.1 | 164.3 | 17.1 | 0.64× |
| `count_aggregate` | 29.4 | 0.3 | 0.6 | 169.9 | 16.5 | 1.91× |
| `decs_count_bare_pred` | — | — | 0.6 | — | — | — |
| `distinct_by_count` | 41.5 | 2.1 | 2.1 | 159.0 | 21.3 | 1.00× |
| `distinct_by_order_take` | 239.2 | 2.6 | 3.2 | 171.9 | 162.6 | 1.22× |
| `distinct_by_order_to_array` | 238.7 | 2.7 | 3.3 | 171.2 | 160.9 | 1.21× |
| `distinct_count` | 41.7 | 2.1 | 2.1 | 173.6 | 21.3 | 1.00× |
| `distinct_count_pred` | 250.9 | 2.1 | 2.3 | 160.8 | 40.4 | 1.08× |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 161.7 | 0.0 | — |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 165.5 | 0.3 | — |
| `first_match` | 0.0 | 0.0 | 0.0 | 169.7 | 0.0 | — |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 167.9 | 0.0 | — |
| `groupby_average` | 171.1 | 2.6 | 2.9 | 185.3 | 36.3 | 1.12× |
| `groupby_count` | 142.8 | 2.4 | 2.5 | 175.3 | 23.2 | 1.06× |
| `groupby_first` | 254.3 | 2.2 | 3.1 | 176.4 | 147.3 | 1.41× |
| `groupby_having_count` | 141.7 | 2.3 | 2.5 | 178.2 | 21.4 | 1.06× |
| `groupby_having_hidden_sum` | 176.7 | 2.5 | 2.8 | 187.6 | 37.9 | 1.13× |
| `groupby_having_post_where` | 173.2 | 2.4 | 2.7 | 183.1 | 37.4 | 1.12× |
| `groupby_max` | 173.1 | 2.4 | 2.7 | 186.4 | 36.2 | 1.13× |
| `groupby_min` | 172.2 | 2.4 | 2.7 | 186.2 | 36.9 | 1.11× |
| `groupby_multi_reducer` | 191.1 | 2.7 | 3.0 | 193.8 | 37.9 | 1.11× |
| `groupby_select_order` | 171.2 | 2.4 | 2.7 | 184.1 | 38.0 | 1.12× |
| `groupby_select_sum` | 198.9 | 3.2 | 3.7 | 198.6 | 32.2 | 1.15× |
| `groupby_sum` | 170.1 | 2.4 | 2.7 | 185.2 | 36.8 | 1.11× |
| `groupby_where_count` | 76.0 | 1.5 | 1.8 | 179.3 | 37.0 | 1.21× |
| `groupby_where_sum` | 86.3 | 1.5 | 1.8 | 182.8 | 36.3 | 1.20× |
| `indexed_lookup` | 1235.6 | 32366.2 | 105.3 | 15728589.3 | 4679606.6 | 0.00× |
| `join_count` | 38.4 | 12.4 | 12.9 | 194.7 | 46.2 | 1.04× |
| `join_groupby_count` | 163.0 | 19.8 | 22.5 | 209.9 | 70.6 | 1.14× |
| `join_groupby_to_array` | 197.1 | 19.8 | 22.4 | 216.7 | 80.8 | 1.13× |
| `join_select` | 93.7 | 20.4 | 22.9 | 203.6 | 196.9 | 1.12× |
| `join_where_count` | 39.2 | 19.9 | 22.2 | 207.6 | 64.6 | 1.12× |
| `last_match` | 0.0 | 0.5 | 1.4 | 171.1 | 148.3 | 2.60× |
| `long_count_aggregate` | 29.7 | 0.3 | 0.6 | 168.3 | 26.7 | 1.83× |
| `max_aggregate` | 31.1 | 0.3 | 0.5 | 167.7 | 16.8 | 1.50× |
| `min_aggregate` | 31.0 | 0.3 | 0.5 | 168.1 | 16.8 | 1.49× |
| `order_distinct_take` | 139.8 | 2.1 | 75.3 | 639.7 | 22.3 | 35.81× |
| `order_reverse_normalized` | 38.6 | 0.7 | 1.4 | 390.7 | 144.2 | 2.03× |
| `order_take_desc` | 38.2 | 0.7 | 1.4 | 383.5 | 145.3 | 1.95× |
| `reverse_distinct_by` | 298.5 | 2.6 | — | 170.7 | 165.4 | — |
| `reverse_take` | 0.0 | 0.0 | 1.1 | 158.2 | 156.8 | — |
| `reverse_take_select` | 0.0 | 0.0 | 1.1 | 158.2 | 156.0 | — |
| `select_count` | 0.1 | 0.0 | 0.0 | 167.4 | 67.3 | — |
| `select_where` | 107.5 | 4.1 | 5.6 | 176.7 | 154.3 | 1.35× |
| `select_where_count` | 32.5 | 0.3 | 0.6 | 178.9 | 28.1 | 1.86× |
| `select_where_order_take` | 36.8 | 0.7 | 1.4 | 277.2 | 148.1 | 1.87× |
| `select_where_sum` | 37.2 | 0.4 | 0.6 | 177.9 | 18.6 | 1.58× |
| `single_match` | 0.0 | 0.4 | 1.1 | 161.0 | 147.9 | 3.12× |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.7 | 1.6 | — |
| `skip_while_match` | 3.5 | 0.4 | 0.4 | 161.5 | 46.5 | 1.01× |
| `sort_first` | 38.1 | 0.4 | 1.3 | 374.8 | 145.2 | 3.36× |
| `sort_take` | 38.6 | 0.7 | 1.3 | 377.8 | 144.8 | 1.93× |
| `sort_take_select` | 38.2 | 0.7 | 1.4 | 376.7 | 142.5 | 1.93× |
| `sum_aggregate` | 30.1 | 0.3 | 0.1 | 168.2 | 16.6 | 0.15× |
| `sum_where` | 32.6 | 0.3 | 0.6 | 177.4 | 16.5 | 1.91× |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.7 | 1.6 | 1.63× |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 166.8 | 0.4 | — |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 163.8 | 0.2 | — |
| `take_where_count` | 0.9 | 0.0 | 0.0 | 1.8 | 0.2 | — |
| `take_while_match` | 7.8 | 0.2 | 0.3 | 85.4 | 17.6 | 1.52× |
| `to_array_filter` | 48.7 | 3.3 | 3.4 | 178.6 | 20.1 | 1.05× |
| `zip_count_pred` | 39.2 | 0.1 | — | 184.5 | 151.2 | — |
| `zip_dot_product` | — | 0.1 | 0.1 | 181.8 | 153.2 | 0.82× |
| `zip_dot_product_3arg` | — | 0.1 | — | 171.0 | 154.2 | — |
| `zip_reverse_to_array` | — | 4.5 | — | 188.6 | 162.0 | — |
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
