# Benchmarks — SQL / Array / Decs / XML comparison

Updated 2026-06-03 (branch `bbatkin/linq-das-multi-key-orderby`): **multi-key `orderby` — `from c in src orderby c.k1, c.k2 descending …` now emits one `_order_by_keys((k1, k2, …), descMask)` op** (compile-time `uint` mask; bit *i* → key *i* descending, LSB = first key). A multi-key order lowers to a **single composite-comparator `stable_sort`** (C# `OrderBy`/`ThenBy` parity — equal full-composite-key rows keep input order); the SQL lane pushes `ORDER BY c1, c2 DESC, …` down to SQLite. **Single-key `orderby` is byte-identical — it keeps the cheaper *unstable* `sort`, so there is no regression**: `bare_order_where` / `order_take_desc` / `sort_take` are all stable within noise vs the prior sweep. The new **`order_by_multi_key`** family measures the worst case for the stable algorithm — `_where(price > T) |> _order_by_keys((_.brand, _.price), 0u)`, a full sort with NO `take` (`brand` has 5 values, so the first key produces dense ties the second resolves). Its delta vs the single-key `bare_order_where` baseline combines two effects — the 1-key→2-key comparator *and* unstable→stable — so it is not a pure stability tax: INTERP 339.9/271.1/280.3/483.5 vs 274.6/117.0/125.8/332.8 (SQL/array/decs/XML), JIT 250.8/53.5/54.8/143.0 vs 186.1/34.0/35.3/125.5. SQL's increase is SQLite's own 2-column `ORDER BY` (still pushed down, no in-memory sort). All other families are stable within long-sweep thermal noise — the `order_by_keys` path is new, so no existing family routes through it.

Earlier (branch `bbatkin/sql-computed-cast-pushdown`): **`pred_to_sql` now lowers daslang workhorse casts to SQLite `CAST`** — `int*`/`uint*` → `CAST(x AS INTEGER)`, `float`/`double` → `CAST(x AS REAL)`, `string` → `CAST(x AS TEXT)`. The daslang cast also drives the result read-back type, so the previously-deferred `zip_dot_product` / `zip_dot_product_3arg` **SQL `m1` lanes are now lit**: `_select(int64(_.price) * int64(_.year)) |> sum()` reads the 64-bit `SUM` back faithfully (the plain int projection truncated at n=100k, ~1e11 ≫ int32). This is a purely additive `pred_to_sql` entry (only cast-bearing chains change), so every other family's codegen is byte-identical and non-zip cells are stable within thermal noise.

Earlier (branch `bbatkin/linq-das-correlated-selectmany`): **the `%linq!` reader now emits the correlated SelectMany flatten — `from o in O from l in o.lines select f(o, l)` → `_select_many` over a new both-vars `select_many_pair` op** (the result selector sees both the outer and inner element). The `select_many` bench is swapped from PR1's inner-only `select_many` baseline to the both-vars `select_many_pair` form the correlated reader emits, with a zero-copy `each`-based collection selector (still UNFUSED — no `_fold` select_many splice): m3f array INTERP 189.3 / JIT 63.7. Two codegen-neutral library tweaks rode along — `group_by_lazy_impl` now `reserve`s its bucket buffer, and `to_array(iterator)` carries a `nolint:PERF006` — both clearing false-positive perf advisories surfaced by generic-instance mangling. `cross_join` and every other family are stable within noise (`cross_join` INTERP 12.7k/3.7k/—/4.1k, matching the prior sweep); remaining cell deltas are long-sweep thermal noise.

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
*Generated 2026-06-03 by `benchmarks/sql/_update_results.das` — ns/op; `—` = absent lane. Edit the prose around the markers, not the tables.*

## INTERP

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) |
|---|---:|---:|---:|---:|
| `aggregate_match` | 34.5 | 6.0 | 6.1 | 58.4 |
| `all_match` | 27.4 | 3.5 | 3.5 | 67.7 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 29.8 | 5.9 | 8.8 | 58.4 |
| `bare_order_where` | 274.6 | 117.0 | 125.8 | 332.8 |
| `chained_select_collapse` | — | 18.0 | 17.7 | 71.4 |
| `chained_where` | 36.3 | 6.7 | 7.3 | 103.3 |
| `contains_match` | 0.0 | 2.3 | 1.4 | 29.2 |
| `count_aggregate` | 29.1 | 4.2 | 4.2 | 64.7 |
| `cross_join` | 12710.8 | 3729.6 | — | 4167.4 |
| `decs_count_bare_pred` | — | — | 4.2 | — |
| `distinct_by_count` | 41.5 | 15.9 | 16.1 | 90.0 |
| `distinct_by_order_take` | 242.7 | 22.0 | 23.2 | 126.6 |
| `distinct_by_order_to_array` | 240.3 | 21.9 | 23.4 | 126.5 |
| `distinct_count` | 41.4 | 15.7 | 15.9 | 70.7 |
| `distinct_count_pred` | 295.4 | 15.8 | 15.8 | 111.6 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.5 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 172.4 | 30.5 | 30.1 | 125.7 |
| `groupby_count` | 142.0 | 19.2 | 19.3 | 76.8 |
| `groupby_first` | 250.4 | 18.5 | 19.2 | 72.0 |
| `groupby_having_count` | 141.5 | 19.2 | 19.2 | 79.5 |
| `groupby_having_hidden_sum` | 175.8 | 23.8 | 24.1 | 123.6 |
| `groupby_having_post_where` | 172.8 | 18.8 | 18.6 | 116.2 |
| `groupby_max` | 173.1 | 25.0 | 25.1 | 120.9 |
| `groupby_min` | 172.5 | 25.0 | 28.9 | 121.5 |
| `groupby_multi_reducer` | 189.4 | 31.9 | 32.6 | 126.4 |
| `groupby_select_order` | 169.9 | 18.8 | 18.7 | 117.7 |
| `groupby_select_sum` | 205.0 | 36.2 | 36.1 | 103.1 |
| `groupby_sum` | 170.5 | 18.6 | 18.6 | 114.4 |
| `groupby_where_count` | 76.0 | 14.4 | 14.9 | 119.4 |
| `groupby_where_sum` | 86.9 | 14.2 | 14.7 | 117.8 |
| `indexed_lookup` | 1462.2 | 205420.4 | 491.9 | 5849551.8 |
| `join_count` | 38.5 | 51.4 | 64.4 | 112.6 |
| `join_groupby_count` | 158.1 | 78.3 | 90.6 | 179.9 |
| `join_groupby_to_array` | 190.8 | 78.2 | 90.3 | 215.1 |
| `join_select` | 150.0 | 72.4 | 85.6 | 192.0 |
| `join_where_count` | 39.3 | 62.0 | 75.1 | 160.4 |
| `last_match` | 0.0 | 5.8 | 14.0 | 65.8 |
| `long_count_aggregate` | 30.0 | 4.1 | 4.1 | 64.4 |
| `max_aggregate` | 31.1 | 6.2 | 6.8 | 58.0 |
| `min_aggregate` | 31.1 | 6.0 | 6.8 | 58.1 |
| `order_by_multi_key` | 339.9 | 271.1 | 280.3 | 483.5 |
| `order_distinct_take` | 137.5 | 15.7 | 93.6 | 73.2 |
| `order_reverse_normalized` | 38.6 | 16.3 | 20.1 | 69.7 |
| `order_take_desc` | 38.6 | 16.2 | 20.0 | 69.6 |
| `reverse_distinct_by` | 297.7 | 21.9 | 28.8 | 74.3 |
| `reverse_take` | 0.1 | 0.0 | 9.2 | 90.3 |
| `reverse_take_select` | 0.0 | 0.0 | 9.2 | 90.3 |
| `select_count` | 0.1 | 0.0 | 2.2 | 70.2 |
| `select_many` | — | 191.2 | — | — |
| `select_where` | 199.6 | 11.1 | 19.3 | 225.1 |
| `select_where_count` | 32.9 | 5.2 | 7.5 | 62.2 |
| `select_where_order_take` | 37.0 | 12.4 | 15.0 | 70.5 |
| `select_where_sum` | 37.5 | 7.4 | 7.6 | 64.1 |
| `single_match` | 0.0 | 2.8 | 5.5 | 57.6 |
| `skip_take` | 0.5 | 0.1 | 0.2 | 3.9 |
| `skip_while_match` | 3.4 | 5.3 | 5.4 | 59.2 |
| `sort_first` | 38.3 | 11.1 | 13.3 | 64.4 |
| `sort_take` | 38.5 | 16.4 | 20.1 | 68.8 |
| `sort_take_select` | 38.3 | 16.3 | 20.1 | 70.4 |
| `sum_aggregate` | 30.3 | 2.1 | 2.1 | 54.0 |
| `sum_where` | 33.2 | 4.3 | 4.3 | 61.4 |
| `take_count` | 3.6 | 0.2 | 0.4 | 3.5 |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 1.3 |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 0.6 |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 0.7 |
| `take_while_match` | 7.9 | 2.4 | 2.4 | 29.8 |
| `to_array_filter` | 71.0 | 11.7 | 11.8 | 72.5 |
| `where_join_count` | 41.7 | 30.0 | 41.5 | 136.0 |
| `zip_count_pred` | 39.3 | 15.2 | — | 375.1 |
| `zip_dot_product` | 46.9 | 12.6 | 10.9 | 369.9 |
| `zip_dot_product_3arg` | 47.3 | 12.8 | — | 374.4 |
| `zip_reverse_to_array` | — | 31.0 | — | 400.9 |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) |
|---|---:|---:|---:|---:|
| `aggregate_match` | 35.0 | 0.3 | 0.6 | 17.1 |
| `all_match` | 27.9 | 0.3 | 0.2 | 16.4 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.5 | 1.0 | 3.6 | 17.8 |
| `bare_order_where` | 186.1 | 34.0 | 35.3 | 125.5 |
| `chained_select_collapse` | — | 2.1 | 2.1 | 21.3 |
| `chained_where` | 36.5 | 0.6 | 0.8 | 36.0 |
| `contains_match` | 0.0 | 0.2 | 0.1 | 17.6 |
| `count_aggregate` | 29.6 | 0.3 | 0.6 | 16.5 |
| `cross_join` | 5947.4 | 730.1 | — | 884.6 |
| `decs_count_bare_pred` | — | — | 0.6 | — |
| `distinct_by_count` | 42.1 | 2.1 | 2.1 | 21.8 |
| `distinct_by_order_take` | 240.4 | 2.6 | 3.2 | 46.7 |
| `distinct_by_order_to_array` | 240.4 | 2.7 | 3.3 | 47.6 |
| `distinct_count` | 41.6 | 2.1 | 2.1 | 21.5 |
| `distinct_count_pred` | 251.7 | 2.1 | 2.3 | 39.5 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.1 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.2 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 171.2 | 2.6 | 2.9 | 36.4 |
| `groupby_count` | 142.0 | 2.4 | 2.5 | 21.4 |
| `groupby_first` | 251.1 | 2.2 | 3.1 | 21.8 |
| `groupby_having_count` | 142.7 | 2.4 | 2.5 | 22.4 |
| `groupby_having_hidden_sum` | 175.6 | 2.5 | 2.8 | 37.6 |
| `groupby_having_post_where` | 169.9 | 2.4 | 2.7 | 35.7 |
| `groupby_max` | 174.2 | 2.4 | 2.7 | 37.9 |
| `groupby_min` | 174.7 | 2.4 | 2.7 | 36.0 |
| `groupby_multi_reducer` | 190.0 | 2.7 | 3.0 | 36.2 |
| `groupby_select_order` | 171.1 | 2.4 | 2.7 | 35.7 |
| `groupby_select_sum` | 202.4 | 3.2 | 3.9 | 32.7 |
| `groupby_sum` | 170.5 | 2.4 | 2.7 | 35.7 |
| `groupby_where_count` | 76.3 | 1.5 | 1.8 | 36.5 |
| `groupby_where_sum` | 87.3 | 1.5 | 1.8 | 37.0 |
| `indexed_lookup` | 1246.1 | 33112.3 | 108.4 | 4689740.5 |
| `join_count` | 38.5 | 11.8 | 12.8 | 47.1 |
| `join_groupby_count` | 157.9 | 19.6 | 21.9 | 68.9 |
| `join_groupby_to_array` | 189.9 | 19.6 | 21.8 | 80.8 |
| `join_select` | 93.3 | 20.2 | 22.7 | 74.0 |
| `join_where_count` | 39.5 | 19.6 | 22.0 | 65.9 |
| `last_match` | 0.0 | 0.5 | 1.4 | 21.0 |
| `long_count_aggregate` | 30.2 | 0.3 | 0.6 | 16.6 |
| `max_aggregate` | 31.2 | 0.3 | 0.5 | 24.6 |
| `min_aggregate` | 31.3 | 0.3 | 0.5 | 16.8 |
| `order_by_multi_key` | 250.8 | 53.5 | 54.8 | 143.0 |
| `order_distinct_take` | 138.7 | 2.1 | 75.5 | 21.3 |
| `order_reverse_normalized` | 38.5 | 0.7 | 1.4 | 16.9 |
| `order_take_desc` | 38.5 | 0.7 | 1.4 | 16.9 |
| `reverse_distinct_by` | 301.6 | 2.6 | 5.0 | 22.0 |
| `reverse_take` | 0.0 | 0.0 | 1.1 | 70.4 |
| `reverse_take_select` | 0.0 | 0.0 | 1.1 | 70.4 |
| `select_count` | 0.1 | 0.0 | 0.0 | 67.8 |
| `select_many` | — | 62.9 | — | — |
| `select_where` | 107.6 | 4.2 | 5.5 | 96.2 |
| `select_where_count` | 33.1 | 0.3 | 0.6 | 16.5 |
| `select_where_order_take` | 36.9 | 0.7 | 1.4 | 17.5 |
| `select_where_sum` | 37.7 | 0.4 | 0.6 | 18.5 |
| `single_match` | 0.0 | 0.4 | 1.1 | 46.6 |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.6 |
| `skip_while_match` | 3.5 | 0.4 | 0.4 | 46.9 |
| `sort_first` | 38.2 | 0.4 | 1.3 | 16.6 |
| `sort_take` | 39.6 | 0.7 | 1.3 | 18.1 |
| `sort_take_select` | 38.6 | 0.7 | 1.4 | 16.9 |
| `sum_aggregate` | 30.5 | 0.3 | 0.1 | 17.8 |
| `sum_where` | 33.2 | 0.3 | 0.6 | 16.5 |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.6 |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 0.5 |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 0.2 |
| `take_where_count` | 0.9 | 0.0 | 0.0 | 0.2 |
| `take_while_match` | 7.8 | 0.2 | 0.3 | 17.7 |
| `to_array_filter` | 48.6 | 3.3 | 3.4 | 20.2 |
| `where_join_count` | 41.6 | 6.4 | 7.4 | 48.3 |
| `zip_count_pred` | 39.5 | 0.1 | — | 152.0 |
| `zip_dot_product` | 46.7 | 0.1 | 0.1 | 153.1 |
| `zip_dot_product_3arg` | 46.8 | 0.1 | — | 151.7 |
| `zip_reverse_to_array` | — | 4.5 | — | 161.1 |
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
