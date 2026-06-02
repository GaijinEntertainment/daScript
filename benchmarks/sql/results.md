# Benchmarks — SQL / Array / Decs / XML comparison

Updated 2026-06-02 (branch `bbatkin/linq-das-correlated-selectmany`): **the `%linq!` reader now emits the correlated SelectMany flatten — `from o in O from l in o.lines select f(o, l)` → `_select_many` over a new both-vars `select_many_pair` op** (the result selector sees both the outer and inner element). The `select_many` bench is swapped from PR1's inner-only `select_many` baseline to the both-vars `select_many_pair` form the correlated reader emits, with a zero-copy `each`-based collection selector (still UNFUSED — no `_fold` select_many splice): m3f array INTERP 189.3 / JIT 63.7. Two codegen-neutral library tweaks rode along — `group_by_lazy_impl` now `reserve`s its bucket buffer, and `to_array(iterator)` carries a `nolint:PERF006` — both clearing false-positive perf advisories surfaced by generic-instance mangling. `cross_join` and every other family are stable within noise (`cross_join` INTERP 12.7k/3.7k/—/4.1k, matching the prior sweep); remaining cell deltas are long-sweep thermal noise.

Earlier (branch `bbatkin/linq-das-uncorrelated-multifrom`): **the `%linq!` reader now emits the `cross_join` family's shape from `from c in A from b in B` (uncorrelated SelectMany).** This is a correctness feature, not a perf change — `_cross_join`'s new lambda type-injection only fires for *untyped* lambdas (the benchmark uses typed ones) and the `to_array(array<T>)` move from `sqlite_linq` to `builtin` is macro-time, so the engine codegen is byte-identical. Re-swept on the same machine to confirm: `cross_join` is stable within noise (INTERP 12.7k/3.7k/—/4.1k, JIT 6.0k/0.7k/—/0.9k — matching the prior sweep), and all other cell deltas are long-sweep thermal noise. No `_fold` cross splice yet (still UNFUSED, the fusion optimization is deferred).

Earlier (branch `bbatkin/linq-das-projections-bench`): **two new families — `cross_join` and `select_many` — establish the multiple-`from` (SelectMany) baseline.** `cross_join` measures the Cartesian product `cross_join(A, B, into)` across SQL / array / XML; `select_many` measures the correlated flatten `select_many(orders, $(o) => o.lines, …)` over an array of nested-collection rows. **Both are UNFUSED** — there is no `_fold` cross-join or select_many splice arm yet, so they run as the plain library calls (the in-memory lanes materialize the full result); this is the baseline the engine-integration PRs improve. `cross_join` uses a smaller n (1000 cars × 100 dealers = 100k pairs) because the product is O(n·m), so its absolute ns are not comparable to the n=100000 families. Empty cells are explained in "Notes on missing lanes". A one-line parity fix in `daslib/linq.das` rode along: the 3-arg `select_many_impl` now reserves `length(src)` up front like the 2-arg form (clears a PERF006).

Earlier (branch `bbatkin/linq-das-join`): **join now fuses a leading `_where`.** The `join_general` splice arm gained an optional leading `_where` slot — a `where` that filters the left source *before* a `_join` is wrapped into the per-A probe loop as `if (pred(a)) { <probe> }` (shared `build_join_standalone_pieces`, so array / decs / XML all inherit it), so the pre-join filter no longer materializes an intermediate filtered-srcA array. The new **`where_join_count`** family measures this (`_where(price > T) |> _join(...) |> count`) against the trailing-filter `join_where_count`; over SQL the same pre-join `where` pushes down to the left table's `WHERE`. All other cell deltas are long-sweep thermal noise — the existing join families (`join_count` / `join_select` / `join_where_count` / `join_groupby_*`) are unchanged, since the new slot is empty for them.

Earlier (branch `bbatkin/linq-fold-sql-passthrough`): **`_fold` routes a SQL source straight to `_sql`.** When a `_fold(...)` chain's source is a `[sql_table]`/`[sql_view]`/`[sql_fts5]` `select_from(db, type<Row>)`, `_fold` re-dispatches the *entire* chain to the `_sql` macro — pure pass-through, observationally identical to writing `_sql(...)` by hand. The **m1 (SQL) lane in every bench is authored as `_fold(db |> select_from(type<Car>) |> ...)`**; the generated SQL and code are identical (early-exit terminals — `first`/`any`/`contains`/`element_at`/`single`/`last` — stay `0.0`, i.e. `LIMIT 1` pushdown, not a full-table materialization). Detection lives in `sqlite/linq_fold_sql` (optional `require ?sqlite` + `static_if (typeinfo builtin_module_exists(sqlite))`, so non-sqlite builds are unaffected) and walks the chain's `arguments[0]` source spine for a SQL-table `select_from`.

Fixture size: n = 100 000 (cars), 100 dealers, 5 brands. Each row is
one bench family in `benchmarks/sql/`; columns are nanoseconds per
logical operation. `—` marks an intentionally absent lane — see
"Notes on missing lanes" below.

The benchmarked chain shapes are summarised in
`benchmarks/README.md` and the splice arms each chain fires are
catalogued in `doc/source/reference/linq_fold_patterns.rst`.

- **SQL** — `_fold(db |> select_from(type<Car>) |> ...)` over an in-memory
  SQLite database (`with_sqlite(":memory:")`); the `[sql_table]` source makes
  `_fold` pass the whole chain through to the `_sql` macro.
- **Array** — `_fold` over `each(arr)` chain, in-memory `array<Car>`.
- **Decs** — `_fold` over `from_decs_template(type<DecsCar>)`,
  per-archetype walk.
- **XML fold (m5f)** — `_fold` over a `from_xml_node(root, type<Car>)` source. The
  `XmlAdapter` (`pugixml/linq_fold_xml`) fuses + field-prunes where it can.

Sub-nanosecond cells (`0.00`) reflect early-exit terminators that exit
before the timer resolution can measure them — they should be read as
"effectively free."

### Reading the XML fold lane (m5f)

The `XmlAdapter` fuses the chain into a single DOM walk and field-prunes the row
materialization to just the fields the chain reads. What it covers:

- **Fused families.** The `loop_or_count` family — `where` / `select` / `count` /
  `sum` / `min` / `max` / `average` / `aggregate` / `first` / `last` / `single` /
  `element_at` / `any` / `all` / `contains` / `take` / `skip` / `take_while` /
  `skip_while`; the cascade families `group_by` (incl. `having` / multi-reducer),
  `distinct[_by]`, `order_by[_descending]`, `sort`, `reverse`; and `join` (incl.
  `group_by` with an upstream `join`, where the in-memory srcB is collected into a
  hash and srcA is probed from the DOM walk).
- **Field-pruning wins.** Reducing / projecting / key-only shapes drop the per-row
  `name` clone entirely; a leading `_select(_.brand)` projection is absorbed into the
  source walk by `ProjectedSourceAdapter`, so even `order |> distinct |> take` prunes
  to just the projected field. A bare `join |> select` (iterator-typed) builds the
  field-pruned result in one pass and exposes it as a sequence (`wrapIter`), eliding
  tier-2's separate `join_to_array`.
- **Materialize-under-guard.** When the whole row escapes but only behind a `where`-gate
  (`where(p) |> single/last/first`, `where(p) |> to_array`, `where(p) |> order_by`), the
  walk reads just the predicate's fields and builds the full row only for *matching*
  elements — clone count drops to the survivor count, not the source length.
- **String-clone floor.** Whole-row escapes with no field-only gate (`reverse |> to_array`,
  un-filtered `sort` / `order_by`) keep the full per-element clone — only the intermediate
  array is elided.

(The absolute XML numbers stay above the array/decs lanes either way — XML carries
DOM-parse + per-element attribute reads + `string` clones the in-memory lanes never pay.)


<!-- BENCH:TABLES BEGIN -->
*Generated 2026-06-02 by `benchmarks/sql/_update_results.das` — ns/op; `—` = absent lane. Edit the prose around the markers, not the tables.*

## INTERP

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) |
|---|---:|---:|---:|---:|
| `aggregate_match` | 34.3 | 5.9 | 5.9 | 58.0 |
| `all_match` | 27.5 | 4.0 | 3.3 | 55.2 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 29.7 | 5.8 | 10.0 | 59.4 |
| `bare_order_where` | 271.6 | 118.5 | 124.6 | 337.3 |
| `chained_select_collapse` | — | 17.7 | 17.5 | 70.0 |
| `chained_where` | 36.2 | 6.6 | 7.2 | 102.6 |
| `contains_match` | 0.0 | 2.2 | 1.4 | 29.0 |
| `count_aggregate` | 29.6 | 4.2 | 4.2 | 64.1 |
| `cross_join` | 12703.0 | 3702.9 | — | 4061.3 |
| `decs_count_bare_pred` | — | — | 4.2 | — |
| `distinct_by_count` | 41.5 | 15.8 | 15.9 | 72.2 |
| `distinct_by_order_take` | 244.1 | 21.6 | 23.4 | 125.6 |
| `distinct_by_order_to_array` | 240.5 | 22.0 | 23.8 | 127.3 |
| `distinct_count` | 41.3 | 15.9 | 15.8 | 70.4 |
| `distinct_count_pred` | 251.1 | 15.7 | 15.9 | 111.6 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.5 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 173.1 | 30.3 | 29.9 | 125.2 |
| `groupby_count` | 148.0 | 19.2 | 19.1 | 75.7 |
| `groupby_first` | 249.8 | 18.4 | 19.2 | 72.1 |
| `groupby_having_count` | 141.8 | 19.2 | 19.6 | 78.1 |
| `groupby_having_hidden_sum` | 177.0 | 23.7 | 24.0 | 123.0 |
| `groupby_having_post_where` | 173.2 | 18.4 | 18.7 | 114.8 |
| `groupby_max` | 173.7 | 32.7 | 25.0 | 121.5 |
| `groupby_min` | 210.6 | 24.9 | 25.0 | 141.8 |
| `groupby_multi_reducer` | 190.9 | 32.3 | 32.9 | 126.6 |
| `groupby_select_order` | 179.4 | 18.8 | 18.9 | 117.2 |
| `groupby_select_sum` | 209.5 | 35.9 | 36.0 | 98.6 |
| `groupby_sum` | 170.7 | 18.6 | 18.8 | 114.9 |
| `groupby_where_count` | 75.1 | 14.3 | 14.9 | 116.8 |
| `groupby_where_sum` | 85.9 | 14.2 | 14.7 | 118.5 |
| `indexed_lookup` | 1404.4 | 205182.8 | 501.6 | 5938416.7 |
| `join_count` | 37.7 | 51.1 | 64.0 | 112.4 |
| `join_groupby_count` | 157.6 | 78.3 | 90.6 | 178.9 |
| `join_groupby_to_array` | 191.1 | 78.6 | 90.8 | 214.7 |
| `join_select` | 147.0 | 76.3 | 85.1 | 188.5 |
| `join_where_count` | 38.8 | 63.3 | 75.4 | 160.7 |
| `last_match` | 0.0 | 5.8 | 14.0 | 65.1 |
| `long_count_aggregate` | 29.2 | 4.2 | 4.2 | 63.7 |
| `max_aggregate` | 30.4 | 6.1 | 6.8 | 58.0 |
| `min_aggregate` | 30.6 | 6.0 | 6.8 | 58.0 |
| `order_distinct_take` | 138.1 | 15.8 | 93.3 | 73.9 |
| `order_reverse_normalized` | 37.8 | 16.2 | 19.9 | 69.4 |
| `order_take_desc` | 37.8 | 16.1 | 19.9 | 69.5 |
| `reverse_distinct_by` | 292.5 | 21.3 | 28.1 | 73.7 |
| `reverse_take` | 0.1 | 0.0 | 9.2 | 99.1 |
| `reverse_take_select` | 0.0 | 0.0 | 9.2 | 98.8 |
| `select_count` | 0.1 | 0.0 | 2.2 | 77.9 |
| `select_many` | — | 189.3 | — | — |
| `select_where` | 193.6 | 10.9 | 19.3 | 223.8 |
| `select_where_count` | 32.1 | 5.2 | 7.4 | 61.7 |
| `select_where_order_take` | 36.1 | 12.3 | 15.0 | 70.4 |
| `select_where_sum` | 36.7 | 7.4 | 7.5 | 63.8 |
| `single_match` | 0.0 | 2.8 | 5.6 | 58.8 |
| `skip_take` | 0.5 | 0.1 | 0.2 | 3.7 |
| `skip_while_match` | 3.4 | 5.2 | 5.4 | 60.9 |
| `sort_first` | 37.5 | 11.0 | 13.3 | 63.9 |
| `sort_take` | 37.8 | 16.4 | 20.0 | 68.2 |
| `sort_take_select` | 37.4 | 16.2 | 19.9 | 70.0 |
| `sum_aggregate` | 29.8 | 2.1 | 2.1 | 55.3 |
| `sum_where` | 32.9 | 4.3 | 4.3 | 61.1 |
| `take_count` | 3.6 | 0.2 | 0.4 | 3.5 |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 1.3 |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 0.6 |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 0.7 |
| `take_while_match` | 7.8 | 2.4 | 2.5 | 30.1 |
| `to_array_filter` | 70.1 | 11.7 | 11.8 | 71.7 |
| `where_join_count` | 40.8 | 29.4 | 41.6 | 135.4 |
| `zip_count_pred` | 38.8 | 15.0 | — | 374.0 |
| `zip_dot_product` | — | 12.8 | 10.4 | 364.7 |
| `zip_dot_product_3arg` | — | 12.7 | — | 409.5 |
| `zip_reverse_to_array` | — | 30.8 | — | 397.3 |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) |
|---|---:|---:|---:|---:|
| `aggregate_match` | 34.9 | 0.3 | 0.6 | 16.6 |
| `all_match` | 27.5 | 0.3 | 0.2 | 16.6 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 29.8 | 1.0 | 3.5 | 17.8 |
| `bare_order_where` | 184.2 | 34.5 | 35.0 | 124.5 |
| `chained_select_collapse` | — | 2.1 | 2.1 | 21.2 |
| `chained_where` | 36.1 | 0.6 | 0.8 | 35.7 |
| `contains_match` | 0.0 | 0.2 | 0.1 | 18.3 |
| `count_aggregate` | 29.2 | 0.3 | 0.6 | 16.6 |
| `cross_join` | 5952.3 | 730.1 | — | 877.6 |
| `decs_count_bare_pred` | — | — | 0.6 | — |
| `distinct_by_count` | 41.2 | 2.1 | 2.1 | 21.8 |
| `distinct_by_order_take` | 239.8 | 2.6 | 3.2 | 46.3 |
| `distinct_by_order_to_array` | 238.2 | 2.7 | 3.2 | 47.4 |
| `distinct_count` | 41.1 | 2.1 | 2.1 | 21.3 |
| `distinct_count_pred` | 249.2 | 2.1 | 2.3 | 39.5 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.1 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.2 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 173.7 | 2.6 | 2.9 | 36.4 |
| `groupby_count` | 140.9 | 2.4 | 2.5 | 21.3 |
| `groupby_first` | 250.0 | 2.2 | 3.1 | 21.8 |
| `groupby_having_count` | 140.9 | 2.3 | 2.5 | 22.3 |
| `groupby_having_hidden_sum` | 173.6 | 2.5 | 2.8 | 37.3 |
| `groupby_having_post_where` | 169.2 | 2.4 | 2.7 | 35.5 |
| `groupby_max` | 172.6 | 2.4 | 2.7 | 37.8 |
| `groupby_min` | 171.0 | 2.4 | 2.7 | 35.9 |
| `groupby_multi_reducer` | 188.7 | 2.7 | 3.0 | 35.9 |
| `groupby_select_order` | 169.8 | 2.4 | 2.7 | 35.6 |
| `groupby_select_sum` | 200.9 | 3.2 | 3.9 | 33.2 |
| `groupby_sum` | 170.7 | 2.4 | 2.7 | 35.6 |
| `groupby_where_count` | 75.6 | 1.5 | 1.8 | 36.3 |
| `groupby_where_sum` | 86.3 | 1.5 | 1.8 | 36.9 |
| `indexed_lookup` | 1234.7 | 32482.7 | 106.4 | 4881806.5 |
| `join_count` | 37.8 | 11.8 | 13.1 | 47.3 |
| `join_groupby_count` | 157.7 | 19.5 | 21.9 | 68.8 |
| `join_groupby_to_array` | 191.7 | 19.9 | 22.0 | 81.2 |
| `join_select` | 92.1 | 20.0 | 22.5 | 73.8 |
| `join_where_count` | 39.0 | 19.4 | 22.0 | 65.7 |
| `last_match` | 0.0 | 0.5 | 1.4 | 21.0 |
| `long_count_aggregate` | 29.5 | 0.3 | 0.6 | 16.7 |
| `max_aggregate` | 32.0 | 0.3 | 0.5 | 24.4 |
| `min_aggregate` | 30.9 | 0.3 | 0.5 | 20.4 |
| `order_distinct_take` | 144.9 | 2.1 | 75.8 | 21.4 |
| `order_reverse_normalized` | 37.9 | 0.7 | 1.3 | 17.0 |
| `order_take_desc` | 37.8 | 0.7 | 1.3 | 16.9 |
| `reverse_distinct_by` | 331.7 | 2.6 | 5.0 | 21.9 |
| `reverse_take` | 0.0 | 0.0 | 1.1 | 72.6 |
| `reverse_take_select` | 0.0 | 0.0 | 1.1 | 71.4 |
| `select_count` | 0.1 | 0.0 | 0.0 | 70.1 |
| `select_many` | — | 63.7 | — | — |
| `select_where` | 105.6 | 4.1 | 5.5 | 95.1 |
| `select_where_count` | 32.5 | 0.3 | 0.6 | 16.6 |
| `select_where_order_take` | 36.3 | 0.7 | 1.3 | 17.5 |
| `select_where_sum` | 36.8 | 0.4 | 0.6 | 18.9 |
| `single_match` | 0.0 | 0.4 | 1.1 | 48.8 |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.6 |
| `skip_while_match` | 3.5 | 0.4 | 0.4 | 48.8 |
| `sort_first` | 37.5 | 0.4 | 1.3 | 16.8 |
| `sort_take` | 38.2 | 0.7 | 1.4 | 18.3 |
| `sort_take_select` | 37.9 | 0.7 | 1.3 | 17.0 |
| `sum_aggregate` | 29.8 | 0.3 | 0.0 | 26.4 |
| `sum_where` | 32.8 | 0.3 | 0.6 | 18.5 |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.5 |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 0.5 |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 0.2 |
| `take_where_count` | 0.9 | 0.0 | 0.0 | 0.2 |
| `take_while_match` | 7.7 | 0.2 | 0.3 | 18.0 |
| `to_array_filter` | 47.4 | 3.2 | 3.9 | 20.1 |
| `where_join_count` | 40.9 | 6.3 | 8.4 | 48.0 |
| `zip_count_pred` | 38.7 | 0.1 | — | 151.6 |
| `zip_dot_product` | — | 0.1 | 0.1 | 151.1 |
| `zip_dot_product_3arg` | — | 0.1 | — | 152.0 |
| `zip_reverse_to_array` | — | 4.5 | — | 161.3 |
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
- **`cross_join` Decs (m4)** — a standalone `from_decs_template` source yields
  anonymous row tuples, so there is no clean typed-lambda cross form
  (`_cross_join` requires typed lambdas and is not `_fold`-integrated yet). The
  decs lane arrives with the cross-join `_fold` engine integration; SQL / array /
  XML only for now.
- **`select_many` SQL (m1) / Decs (m4) / XML (m5f)** — the correlated flatten
  needs a per-element nested collection (an `array<…>` field). SQL has no
  nested-collection flatten (correlated select_many over SQL is rejected); a decs
  component has no `array<…>` field; and the XML linq source `from_xml_node`
  materializes rows from flat attributes only (`build_xml_row`), so it cannot
  populate a nested field. Array-only — a source-shape gap, by design for these
  backends.
- **`decs_count_bare_pred` SQL / Array / XML (m5f)** — covers a Theme 4
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
- **`reverse_distinct_by` m4 / m5f** — the array backward-index walk
  is array-only (`array_source`), but the FORWARD keep-last splice
  (R-2b, `emit_reverse_distinct_forward_keeplast`, gated
  `non_array_source`) fuses any forward source single-pass: OVERWRITE
  `table<key; (seq, val)>` per element, sort survivors by descending
  seq. decs `m4` is now lit (27.6 / 5.0, near the array fast path);
  XML `m5f` 429 → 74.5 / 166.6 → 22.2 via handle-deferral (build only
  the K survivors). SQL `m1` uses the MAX(pk) bare-aggregate.
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
- **`zip_*` XML lane (m5f)** — each zip
  bench zips the XML `Car` price-stream against a synthetic int array via the
  mixed `zip(iterator, array)` overload; the zip splice partially fuses over XML,
  still paying the unpruned `Car` materialization. The remaining `—` zip cells are
  **SQL (m1)** and **Decs (m4)**: `zip` is not a relational op and not expressible
  over a single archetype walk (see the two bullets above).

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
