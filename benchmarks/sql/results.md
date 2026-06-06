# Benchmarks — SQL / Array / Decs / XML / JSON comparison

Updated 2026-06-06 (branch `bbatkin/json-const-key`): **`read_json_field` now folds a literal field key into a const-key table lookup.** The fused JSON lane and `from_json_row`'s `apply_imm` both call `read_json_field(jv, "<field>", def)` with a string-literal key; `[constant_expression(key=false)]` folds that into a specialized copy whose `(jv ?as _object)?["<field>"]` lookup bakes the key bytes + hash at simulate time (`SafeTableIndex_WithHash`, verified via `options log_nodes`). **Two separate effects show up in this sweep.** **(1) The big m6f JIT drop is the packed small-table work that landed in master** since the prior sweep (`table: packed small-table mode` → `table: OP1 fusion for const-string-key WithHash`), *not* this branch: `count_aggregate` 36.3→25.6, `groupby_sum` 66.1→40.4, `max_aggregate` 35.8→22.7, `min_aggregate` 35.7→21.9 (≈−30–40% across the field-reading lanes) — a packed object table is a linear scan over 64-bit hashes (no strcmp) that the JIT inlines as a SIMD scan, so even a *runtime* key is already fast there. **(2) The const-key fold is a small INTERP gain** — a same-build A/B (master `.das` vs branch, isolating this change from the table work) puts it at ≈−8–10% on field-heavy lanes (`chained_where`, `distinct_count`, `join_select`) and **flat in JIT on this Apple-silicon machine**: the packed-table fast path was tuned off-mac, so the const-key JIT effect is deferred to a platform-specific follow-up. In interp (no SIMD scan) baking the key hash removes per-lookup hash work; against the ~150 ns field read — dominated by the `read_json_field`→`from_json_row` call frames + `JsValue` variant dispatch, not the ~11 ns table probe — that nets single digits, as expected. A one-line shared-macro fix rode along: the `constant_expression` macro emits the specialized call as `__::name` so the generated function resolves in `thisModule` (where `add_function` places it) — required because `read_json_field` is invoked from library-internal generic code (`from_json_row`'s `apply_imm`), whose call site is namespaced to the generic's origin module. Two caveats on this sweep. The **interp column is the reliable signal** — the JIT column on this Apple-silicon machine has high run-to-run variance (re-running XML m5f `sum_aggregate` spans 24–41, `select_count` 37–70), so read JIT deltas as indicative, not precise. And this is the first sweep on current master (+78 commits since the 2026-06-04 baseline), so some non-m6f cells reflect master drift, not this branch — notably an XML m5f JIT shift (`count_aggregate` 16.7→26.3, stable across re-runs) that predates and is unrelated to this change. Interp non-m6f cells are stable (`count_aggregate` m5f 64.5→61.5, `select_where` m1 196.6→196.4).

Earlier (branch `bbatkin/decs-reverse-random-index`): **the DecsAdapter now random-indexes the boundary archetype tail.** Bare `last()` / `last_or_default()` (no `where` / `select` / range) read the last non-empty archetype's `[size-1]` directly via `get_ro(arch, comp, def)[idx]` (`emit_decs_last_random_index`, O(num_archetypes)); `reverse |> take(K)` random-indexes the boundary archetype's tail `[size-K .. size)` instead of `continue`-walking its head (`build_decs_index_collect`). **`reverse_take` / `reverse_take_select` m4 9.2/9.3 → 0.2 INTERP (1.1 → 0.0 JIT), and the new `bare_last` m4 is 0.0** (the O(*n*) walk it replaces cost ~17 ns/elem INTERP / ~1.2 JIT — `benchmarks/micro/bare_last_decs_shapes.das`) — decs is now level with the XML backward-DOM path (m5f 0.0) and below array's still-walking `last()` (m3f 4.2). **Indexable sources only** — a `[decs_template]` field with a default-init compiles to `get_default_ro` (an iterator), so `decs_can_random_index` returns false and the chain falls back to the per-entity walk (behavior-identical); predicated / `_select`'d `last` likewise keeps the walk (the last-match position is unknown). Only the decs (m4) `bare_last` + `reverse_take[_select]` cells moved; all other cells are stable within long-sweep thermal noise.

Earlier (branch `bbatkin/xml-reverse-emission`): **the XmlAdapter now walks the DOM backward for bounded-reverse shapes.** `reverse |> take(K) [|> select]` and a **no-predicate** `last()` / `reverse |> first` fuse to an O(K) backward walk via pugixml's O(1) `last_child()` / `previous_sibling()` — visiting only the kept tail instead of scanning all *n* forward then reversing. This closes the gap JSON's array-backed random access used to hold: **`reverse_take` m5f 85.8 → 0.0, `reverse_take_select` m5f 84.6 → 0.0 INTERP** (71.0 / 69.1 → 0.0 JIT), now level with the array / JSON tail walk. **Predicated `[where] |> last` deliberately stays on the forward walk** — reverse DOM traversal is ~2× cache-hostile per node (pugixml lays nodes out in forward document order, so `previous_sibling` defeats the prefetcher; profiled `count.backward` 119 vs `count.forward` 63 ns/elem INTERP), so a match far from the end would regress ~2.6× — `last_match` m5f is unchanged (66.3). **This sweep also captures the deferred apply-hybrid (#3008) read-path inline** that landed after the previous sweep: the `from_xml_node` field/row materializer now inlines its per-field `apply` block instead of routing each field through a per-field `invoke`, so XML m5f field-reading lanes drop broadly vs the previous (pre-#3008) sweep — `select_where` 253.3 → **196.0**, `bare_order_where` 332.5 → **303.8** INTERP, and the single-field reduce / aggregate lanes settle at ~16.5 ns under JIT (`count_aggregate` 27.2 → **16.5**, `sum_aggregate` 23.6 → **16.8**, `max_aggregate` 24.6 → **16.7**). Only the XML read-path (m5f) and the two reverse lanes moved; m1 / m3f / m4 / m6f cells are stable within long-sweep thermal noise.

Earlier (branch `bbatkin/linq-json-self-materialize`): **`from_json` is now a by-name flat query source (Option B)** — the fused `JsonAdapter` and `from_json` / `read_json_field` materialize each JSON object the way `from_xml_node` does (top-level fields read by key, via `apply_imm` over the struct fields reading each `JsValue` straight off the variant), and **never honor a whole-row `from_JV(Row)` override** (the escape hatch is an explicit `[for (e in jv.value as _array); from_JV(e, type<Row>)]` first). Two wins compound. **(1) By-name materialize** removes the old per-result-row `from_JV` allocation (`select_where` 405 → 21 B/op) that ran ~n times on **lanes that build a large result set** — a full projection-to-array or a sort-everything. **(2) A direct per-field read** then attacks the per-field floor that (1) left untouched: `read_json_field` now reads a `table<string; JsonValue?>` slot pointer via `(jv ?as _object)?[key]` (null ⇔ key absent) instead of routing through the `operator ?[]` call frame + a `JVNull()`-sentinel `_null`-tag round-trip, and `from_json_row` reads each basic value straight off the `JsValue` variant — probing the `TT`-matching tag first (JSON ints land in `_longint`, reals in `_number` — a `static_if`, no runtime branch) — instead of the `from_JV` scalar wrapper. That drops a per-field interpreter call frame (plus the absent-key allocation), so **every field-reading lane falls ~15–31% INTERP**, not just the big-materialize ones: `select_where` 697.3 → **192.1** (−72%), `bare_order_where` 780.6 → **302.8** (−61%), and the reduce / count / group lanes that the by-name pass left flat now drop too (`count_aggregate` 214.0 → **156.3**, `sum_aggregate` 198.4 → **144.5**, `groupby_sum` 276.0 → **201.6**). Lanes that read **no field** are unchanged (`select_count` 2.2, `reverse_take` 25.2 — the array-backed O(K) tail walk). **Under JIT the per-field path is already inlined, so m6f is flat** (`aggregate_match` 38.3 → 41.4, `select_where` 38.0 → 37.4 — jitter). **Net vs XML:** the big-materialize lanes that trailed XML ~3× in INTERP now reach roughly parity (`select_where` 192.1 vs XML ~226–253, `bare_order_where` 302.8 vs 332.5), and **under JIT stay JSON-faster** (`select_where` 37.4 vs 95.2 = 0.39×, `bare_order_where` 58.3 vs 123.8 = 0.47×) — the das-side by-name build compiles to native while XML's per-row C++ assembly is fixed. The single-field-int **reduce** lanes still trail XML in INTERP (`count_aggregate` 156.3 vs 64.6 ≈ 2.4×, down from ~3×) and ~1.2× under JIT — the irreducible string-hashed `table` lookup vs XML's C++ pointer read. Only the m6f (JSON) path changed; every m1 / m3f / m4 / m5f cell is stable within long-sweep thermal noise (the `select_where` m5f 226.5 → 253.3 jitter is one such — XML is untouched here).

Earlier (branch `bbatkin/linq-json-bench`): **JSON is now the 5th lane — `m6f` (`_fold` over `from_json(jv, type<Car>)`) lands on all 74 families that carry an XML `m5f` lane**, so the `JsonAdapter` (`daslib/linq_fold_json`) is now directly comparable to the `XmlAdapter`. The two adapters are near-1:1 (same fused families, same field-pruning, same materialize-under-guard, same fused join + `join |> group_by`); the bench isolates where the *cost* differs, and it splits cleanly along **two axes**. **(1) Per-element loop step:** XML advances its child list with a **C++ pugixml call per element** (`next_sibling` + a `node_element` kind-check — visible in the `select_count` codegen, a bare `++acc` walk that still costs 69.4 ns/elem INTERP), while JSON iterates a flat das `array<JsonValue?>` at 2.2 ns/elem. **(2) Per-field read:** XML reads an attribute by C++ pointer (cheap on top of the already-paid loop step), while JSON's `read_json_field` does a string-hashed `table<string; JsonValue?>` lookup + `from_JV` unwrap per field — the heavy part of the JSON lane (the `sum_aggregate` − `select_count` INTERP delta, ~196 ns, is essentially one JSON field read). **Neither path has an O(1) count/length shortcut — both walk every element.** So **field-heavy shapes favor XML, field-light shapes favor JSON.** On bulk reduce / group-by / full-row materialize (many field reads/element) JSON trails XML **~3.3× in INTERP** (median; `count_aggregate` 64.2→214.0, `sum_aggregate` 53.9→198.4) — JSON's ~200 ns reducer matches the Phase-0 hand-optimal *shape A*, so this is XML's cheap-field-read advantage, not adapter overhead. **Under JIT the das-side table lookups compile to native while XML's C++ boundary is fixed, so the gap closes to 1.80× median and JSON flips to faster on 9 families** (XML m5f / JSON m6f JIT), all field-light: `select_count` **0.03×** (INTERP 69.4→2.2 — zero field reads, so only the flat-walk-vs-C++-walk loop-step gap shows), the predicate-gated early-exits `single_match` 0.73× / `contains_match` 0.84× / `skip_while_match` 0.75× / `take_while_match` 0.85× / `indexed_lookup` 0.81× / `chained_where` 0.56×, `join_groupby_to_array` 0.57×, and `reverse_take` / `reverse_take_select` **0.05×** (66.9→3.7). **The `reverse_take` win is an adapter gap, not a DOM limit:** JSON random-indexes the last K from its backing array (O(K)) where the XmlAdapter walks all n forward then reverses — even though pugixml exposes O(1) `last_child()` + `previous_sibling()`, so a backward O(K) XML walk is a latent optimization left on the table. JSON also does **zero `string` clones** — a JSON string is already a heap-owned das string so `from_JV` aliases the pointer (`0 SB/op` / `0 strings/op` where XML clones ~n `name` strings out of the C++ document). Net: XML wins bulk reduce / materialize / sort even under JIT (its per-field C++ read is unbeatable for throughput), JSON wins anything field-light — a count, an early-exit gate, or a take its `array`-backed random access serves in O(K). No engine codegen changed — the only `.das` deltas are `fixture_json` + 74 `run_m6f` lanes + the `_update_results.das` lane list, so every existing m1/m3f/m4/m5f cell is stable within long-sweep thermal noise.

Earlier (branch `bbatkin/linq-das-group-join`): **`group_join` is now C# GroupJoin (outer) and the `_fold` join splice fuses it.** `group_join_impl` keeps every left row — an unmatched left pairs with an empty group (was inner, silently dropping unmatched). The `join_general` splice slot now accepts `group_join` as well as `join` (an `isGroupJoin` flag threads through `build_join_standalone_pieces`: push `result(a, bucket)` once + `if (!get(...)) push result(a, empty)`), so a `%linq! … join … into g select (…, g |> length / g |> select(…) |> sum)` fuses — including a pre-join `where` — into one hash-build + probe with no intermediate. Array sources only (`_group_join` has no SQL push-down — it rejects; decs/xml route group joins to tier-2). The new **`group_join_count`** family isolates the outer cost with two same-sized lanes — `matched` (every left row matched) vs `unmatched` (all left rows miss → empty groups): they are **within noise** (INTERP 29.1 vs 28.9, JIT 18.2 vs 18.3 ns/op) — the outer-empty emit costs the same as a matched bucket-length read, so outer semantics is effectively free (the hash build over n=100k dominates). Existing join families are **byte-identical** — the splice change is guarded by `isGroupJoin`, so regular-join codegen is unchanged (the `test_linq_fold_ast` shape assertions confirm); not re-swept (no codegen delta).

Earlier (branch `bbatkin/linq-das-multi-key-orderby`): **multi-key `orderby` — `from c in src orderby c.k1, c.k2 descending …` now emits one `_order_by_keys((k1, k2, …), descMask)` op** (compile-time `uint` mask; bit *i* → key *i* descending, LSB = first key). A multi-key order lowers to a **single composite-comparator `stable_sort`** (C# `OrderBy`/`ThenBy` parity — equal full-composite-key rows keep input order); the SQL lane pushes `ORDER BY c1, c2 DESC, …` down to SQLite. **Single-key `orderby` is byte-identical — it keeps the cheaper *unstable* `sort`, so there is no regression**: `bare_order_where` / `order_take_desc` / `sort_take` are all stable within noise vs the prior sweep. The new **`order_by_multi_key`** family measures the worst case for the stable algorithm — `_where(price > T) |> _order_by_keys((_.brand, _.price), 0u)`, a full sort with NO `take` (`brand` has 5 values, so the first key produces dense ties the second resolves). Its delta vs the single-key `bare_order_where` baseline combines two effects — the 1-key→2-key comparator *and* unstable→stable — so it is not a pure stability tax: INTERP 339.9/271.1/280.3/483.5 vs 274.6/117.0/125.8/332.8 (SQL/array/decs/XML), JIT 250.8/53.5/54.8/143.0 vs 186.1/34.0/35.3/125.5. SQL's increase is SQLite's own 2-column `ORDER BY` (still pushed down, no in-memory sort). All other families are stable within long-sweep thermal noise — the `order_by_keys` path is new, so no existing family routes through it.

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
- **JSON fold (m6f)** — `_fold` over a `from_json(jv, type<Car>)` source where `jv` is a
  pre-built `JsonValue?` array of objects (`fixture_json`). The `JsonAdapter`
  (`daslib/linq_fold_json`) fuses + field-prunes the same shapes as the XML lane — same
  `SourceAdapter` machinery, swapping the DOM walk for a `jsrc.value as _array` walk and
  `read_json_field` / `from_JV` for the attribute reads.

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
- **Bounded-reverse backward walk.** `reverse |> take(K) [|> select]` and a no-predicate
  `last()` / `reverse |> first` walk the DOM backward (`last_child` / `previous_sibling`, O(1)
  each) and stop after K elements — O(K), not the old O(*n*)-forward-then-reverse-then-resize. So
  `reverse_take` / `reverse_take_select` m5f are now sub-resolution (0.0). Backward is
  **unnamed-only** (no last-named-child primitive); a named `from_xml_node(root, "tag", …)`
  reverse falls back to the buffer-all path. Predicated `[where] |> last` keeps the forward walk
  (reverse traversal is ~2× cache-hostile per node — a far-from-end match would regress).

(The absolute XML numbers stay above the array/decs lanes either way — XML carries
DOM-parse + per-element attribute reads + `string` clones the in-memory lanes never pay.)

### Reading the JSON fold lane (m6f)

The `JsonAdapter` is a near-1:1 mirror of `XmlAdapter` — same fused families, same
field-pruning, same materialize-under-guard, same fused join (incl. `join |> group_by`).
The cost difference is a **two-axis split** — per-element loop step vs per-field read — and the
two lanes sit on opposite corners:

- **Loop step: JSON cheap, XML costly.** JSON iterates a flat das `array<JsonValue?>` (a plain
  index advance, ~2.2 ns/elem). XML advances its child list with a **C++ pugixml call per element**
  (`next_sibling` + a `node_element` kind-check) — ~60–69 ns/elem *before any field is read*. The
  `select_count` family isolates this: a bare element count (no field read) is JSON 2.2 vs XML 69.4
  INTERP, **0.03×**, purely the loop-step gap.
- **Field read: JSON costly, XML cheap.** Each JSON field read is a `read_json_field` →
  `(jv ?as _object)?[key]` string-hashed `table<string; JsonValue?>` lookup + a direct `JsValue`-variant
  read (~142 ns/field INTERP — the `sum_aggregate` − `select_count` delta is essentially one such read;
  was ~190 before `read_json_field` dropped the `operator ?[]` frame + `from_JV` wrapper). XML reads an
  attribute by C++ pointer, nearly free on top of the loop step it already paid. So **field-light
  shapes** (count, predicate-gated early-exit) favor JSON; **field-heavy shapes** (bulk reduce,
  whole-row `to_array` with its 6 lookups/row) favor XML.
- **No string clones (JSON).** A JSON string value is already a heap-owned das string in the same
  context as the fold output, so `from_JV(…, type<string>)` aliases the pointer rather than copying.
  The whole-row materialize shapes that make XML clone ~n `name` strings cost JSON **zero** string
  allocations (`0 SB/op`, `0 strings/op`).
- **Reverse: now at parity.** `reverse |> take(K)` was JSON's biggest win — its array-backed source
  random-indexes the last K (O(K)). As of `bbatkin/xml-reverse-emission` the XmlAdapter matches it
  with an O(K) backward DOM walk (`last_child()` + `previous_sibling()`, both O(1) in pugixml — see the
  XML lane notes above), so `reverse_take` / `reverse_take_select` m5f are now 0.0 on both lanes. (`count`,
  by contrast, is genuinely structural — pugixml has no `child_count()` and no `operator[]` on
  `xml_node`, so XML can't shortcut it.)
- **No parse step in the measured block.** Like XML (which parses the document outside `run`), the
  JSON tree is built by `fixture_json` outside the timer; both lanes measure only the
  walk + materialize + fold, so the m5f/m6f cells are directly comparable.


<!-- BENCH:TABLES BEGIN -->
*Generated 2026-06-06 by `benchmarks/sql/_update_results.das` — ns/op; `—` = absent lane. Edit the prose around the markers, not the tables.*

## INTERP

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) |
|---|---:|---:|---:|---:|---:|
| `aggregate_match` | 34.4 | 5.9 | 5.9 | 59.2 | 159.0 |
| `all_match` | 27.8 | 3.6 | 3.6 | 54.2 | 156.3 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.2 | 5.9 | 8.7 | 56.6 | 164.8 |
| `bare_last` | — | 4.2 | 0.0 | 0.0 | 4.2 |
| `bare_order_where` | 278.3 | 117.9 | 126.1 | 302.6 | 286.9 |
| `chained_select_collapse` | — | 22.7 | 22.4 | 84.1 | 179.7 |
| `chained_where` | 36.5 | 6.7 | 7.2 | 103.0 | 182.9 |
| `contains_match` | 0.0 | 2.3 | 1.5 | 29.1 | 71.5 |
| `count_aggregate` | 29.8 | 4.2 | 4.3 | 61.5 | 155.5 |
| `cross_join` | 12498.3 | 3636.7 | — | 3957.2 | 3961.2 |
| `decs_count_bare_pred` | — | — | 4.3 | — | — |
| `distinct_by_count` | 41.1 | 20.5 | 20.4 | 80.3 | 167.1 |
| `distinct_by_order_take` | 251.6 | 21.9 | 23.3 | 126.5 | 156.5 |
| `distinct_by_order_to_array` | 249.2 | 22.1 | 23.4 | 126.5 | 155.4 |
| `distinct_count` | 41.0 | 20.3 | 20.4 | 80.5 | 166.5 |
| `distinct_count_pred` | 262.8 | 20.2 | 20.6 | 121.7 | 178.0 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.4 | 0.3 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 173.7 | 27.2 | 27.3 | 134.0 | 193.0 |
| `groupby_count` | 141.5 | 19.8 | 19.8 | 82.7 | 170.5 |
| `groupby_first` | 249.8 | 20.1 | 20.9 | 83.1 | 170.7 |
| `groupby_having_count` | 143.5 | 19.8 | 19.8 | 82.8 | 170.0 |
| `groupby_having_hidden_sum` | 175.3 | 21.5 | 21.8 | 127.9 | 188.9 |
| `groupby_having_post_where` | 171.0 | 19.8 | 19.8 | 122.9 | 186.4 |
| `groupby_max` | 172.0 | 23.7 | 24.2 | 129.3 | 198.6 |
| `groupby_min` | 172.6 | 23.7 | 24.1 | 128.9 | 192.1 |
| `groupby_multi_reducer` | 189.6 | 28.0 | 28.6 | 134.1 | 196.8 |
| `groupby_select_order` | 170.3 | 19.8 | 19.9 | 122.5 | 187.4 |
| `groupby_select_sum` | 199.2 | 38.7 | 39.2 | 99.0 | 188.1 |
| `groupby_sum` | 170.6 | 19.9 | 19.9 | 122.7 | 186.3 |
| `groupby_where_count` | 75.4 | 14.3 | 14.6 | 122.3 | 183.3 |
| `groupby_where_sum` | 86.4 | 14.7 | 14.9 | 128.0 | 182.8 |
| `indexed_lookup` | 1463.2 | 226114.9 | 470.7 | 5485734.3 | 14463210.9 |
| `join_count` | 38.4 | 51.3 | 64.3 | 113.1 | 176.2 |
| `join_groupby_count` | 161.2 | 85.0 | 97.7 | 186.7 | 224.7 |
| `join_groupby_to_array` | 189.8 | 86.0 | 98.3 | 221.2 | 222.2 |
| `join_select` | 144.6 | 70.7 | 83.3 | 185.8 | 195.8 |
| `join_where_count` | 37.7 | 61.3 | 74.7 | 160.3 | 184.1 |
| `last_match` | 0.0 | 5.8 | 13.8 | 64.7 | 139.4 |
| `long_count_aggregate` | 27.7 | 4.2 | 4.2 | 60.8 | 136.9 |
| `max_aggregate` | 28.7 | 5.9 | 6.8 | 55.8 | 148.3 |
| `min_aggregate` | 28.8 | 5.8 | 6.7 | 55.9 | 146.3 |
| `order_by_multi_key` | 314.3 | 266.1 | 270.6 | 446.6 | 431.1 |
| `order_distinct_take` | 132.4 | 20.0 | 97.8 | 81.9 | 154.0 |
| `order_reverse_normalized` | 35.8 | 15.6 | 19.3 | 68.9 | 170.1 |
| `order_take_desc` | 39.0 | 16.1 | 19.9 | 70.4 | 176.0 |
| `reverse_distinct_by` | 296.7 | 26.3 | 26.7 | 81.8 | 169.6 |
| `reverse_take` | 0.1 | 0.0 | 0.2 | 0.0 | 26.1 |
| `reverse_take_select` | 0.0 | 0.0 | 0.2 | 0.0 | 26.0 |
| `select_count` | 0.1 | 0.0 | 2.2 | 68.6 | 2.2 |
| `select_many` | — | 192.4 | — | — | — |
| `select_where` | 196.4 | 11.3 | 19.6 | 203.1 | 192.4 |
| `select_where_count` | 32.8 | 5.2 | 7.7 | 62.6 | 159.2 |
| `select_where_order_take` | 37.0 | 12.4 | 14.9 | 71.0 | 164.5 |
| `select_where_sum` | 37.4 | 7.7 | 7.7 | 64.6 | 163.1 |
| `single_match` | 0.0 | 2.9 | 5.6 | 57.7 | 151.4 |
| `skip_take` | 0.5 | 0.1 | 0.2 | 3.1 | 2.8 |
| `skip_while_match` | 3.4 | 5.4 | 5.5 | 59.6 | 155.3 |
| `sort_first` | 38.0 | 11.4 | 13.3 | 64.0 | 167.1 |
| `sort_take` | 38.6 | 16.4 | 20.7 | 70.9 | 171.9 |
| `sort_take_select` | 38.7 | 16.4 | 20.2 | 70.7 | 168.0 |
| `sum_aggregate` | 30.2 | 2.1 | 2.1 | 52.6 | 153.8 |
| `sum_where` | 33.1 | 4.4 | 4.4 | 62.2 | 156.0 |
| `take_count` | 3.6 | 0.2 | 0.4 | 3.0 | 2.7 |
| `take_count_filtered` | 1.1 | 0.2 | 0.2 | 1.3 | 1.1 |
| `take_sum_aggregate` | 0.8 | 0.1 | 0.1 | 0.7 | 0.5 |
| `take_where_count` | 0.9 | 0.1 | 0.1 | 0.7 | 0.6 |
| `take_while_match` | 7.8 | 2.5 | 2.5 | 30.0 | 76.7 |
| `to_array_filter` | 70.7 | 12.7 | 12.7 | 70.0 | 164.2 |
| `where_join_count` | 41.6 | 29.8 | 41.5 | 132.3 | 170.1 |
| `zip_count_pred` | 39.4 | 17.5 | — | 322.5 | 322.6 |
| `zip_dot_product` | 47.1 | 12.5 | 10.7 | 316.1 | 316.8 |
| `zip_dot_product_3arg` | 47.1 | 12.5 | — | 316.1 | 316.3 |
| `zip_reverse_to_array` | — | 31.8 | — | 346.9 | 350.1 |

## JIT

| Benchmark | SQL (m1) | Array (m3f) | Decs (m4) | XML fold (m5f) | JSON fold (m6f) |
|---|---:|---:|---:|---:|---:|
| `aggregate_match` | 35.1 | 0.3 | 0.7 | 23.7 | 25.5 |
| `all_match` | 28.4 | 0.3 | 0.2 | 16.9 | 25.6 |
| `any_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `average_aggregate` | 30.5 | 1.0 | 3.6 | 18.8 | 24.1 |
| `bare_last` | — | 0.4 | 0.0 | 0.0 | 0.0 |
| `bare_order_where` | 186.0 | 34.1 | 35.9 | 105.1 | 51.7 |
| `chained_select_collapse` | — | 1.2 | 1.2 | 19.8 | 34.0 |
| `chained_where` | 36.7 | 0.6 | 0.9 | 36.3 | 31.1 |
| `contains_match` | 0.0 | 0.2 | 0.1 | 17.6 | 9.2 |
| `count_aggregate` | 30.0 | 0.3 | 0.6 | 26.3 | 25.6 |
| `cross_join` | 5987.6 | 723.0 | — | 854.9 | 781.5 |
| `decs_count_bare_pred` | — | — | 0.6 | — | — |
| `distinct_by_count` | 41.6 | 1.2 | 1.2 | 19.7 | 33.5 |
| `distinct_by_order_take` | 239.0 | 3.9 | 5.0 | 47.7 | 33.7 |
| `distinct_by_order_to_array` | 241.0 | 4.0 | 4.8 | 47.4 | 34.2 |
| `distinct_count` | 41.7 | 1.2 | 1.2 | 19.8 | 34.0 |
| `distinct_count_pred` | 252.9 | 1.1 | 1.3 | 37.6 | 43.4 |
| `distinct_take` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `element_at_match` | 0.0 | 0.0 | 0.0 | 0.1 | 0.0 |
| `first_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `first_or_default_match` | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 |
| `groupby_average` | 172.7 | 2.0 | 2.2 | 36.5 | 48.6 |
| `groupby_count` | 142.9 | 1.6 | 1.8 | 20.2 | 35.8 |
| `groupby_first` | 251.6 | 1.3 | 2.4 | 19.9 | 34.6 |
| `groupby_having_count` | 141.8 | 1.6 | 1.8 | 20.2 | 35.7 |
| `groupby_having_hidden_sum` | 176.9 | 1.8 | 2.1 | 36.8 | 49.7 |
| `groupby_having_post_where` | 174.1 | 1.7 | 2.0 | 36.9 | 46.7 |
| `groupby_max` | 173.6 | 1.7 | 2.0 | 36.7 | 46.8 |
| `groupby_min` | 168.2 | 1.6 | 2.0 | 35.8 | 40.7 |
| `groupby_multi_reducer` | 184.6 | 2.0 | 2.3 | 35.9 | 41.9 |
| `groupby_select_order` | 164.1 | 1.6 | 2.0 | 35.4 | 40.3 |
| `groupby_select_sum` | 188.0 | 4.0 | 3.7 | 33.1 | 35.0 |
| `groupby_sum` | 165.0 | 1.6 | 2.0 | 35.2 | 40.4 |
| `groupby_where_count` | 77.1 | 1.0 | 1.4 | 37.4 | 42.5 |
| `groupby_where_sum` | 86.6 | 1.1 | 1.4 | 37.4 | 42.2 |
| `indexed_lookup` | 1243.4 | 32518.7 | 109.0 | 4707921.9 | 2202829.8 |
| `join_count` | 38.3 | 13.0 | 13.8 | 41.4 | 74.8 |
| `join_groupby_count` | 157.3 | 20.1 | 22.6 | 67.4 | 91.1 |
| `join_groupby_to_array` | 189.8 | 20.1 | 22.5 | 80.6 | 40.3 |
| `join_select` | 93.1 | 21.1 | 23.4 | 75.9 | 127.3 |
| `join_where_count` | 39.7 | 22.1 | 23.9 | 65.6 | 81.8 |
| `last_match` | 0.0 | 0.5 | 1.4 | 19.6 | 25.7 |
| `long_count_aggregate` | 30.2 | 0.3 | 0.6 | 23.1 | 25.7 |
| `max_aggregate` | 29.3 | 0.3 | 0.5 | 23.3 | 22.7 |
| `min_aggregate` | 29.4 | 0.3 | 0.5 | 23.3 | 21.9 |
| `order_by_multi_key` | 231.2 | 52.2 | 53.7 | 121.3 | 68.0 |
| `order_distinct_take` | 133.2 | 1.1 | 74.2 | 20.5 | 27.6 |
| `order_reverse_normalized` | 35.8 | 0.7 | 1.3 | 19.0 | 22.8 |
| `order_take_desc` | 35.7 | 0.7 | 1.3 | 18.9 | 23.2 |
| `reverse_distinct_by` | 281.2 | 1.6 | 3.5 | 19.2 | 28.4 |
| `reverse_take` | 0.0 | 0.0 | 0.0 | 0.0 | 3.5 |
| `reverse_take_select` | 0.0 | 0.0 | 0.0 | 0.0 | 3.5 |
| `select_count` | 0.1 | 0.0 | 0.0 | 37.1 | 0.0 |
| `select_many` | — | 55.3 | — | — | — |
| `select_where` | 100.0 | 3.9 | 5.3 | 73.2 | 19.2 |
| `select_where_count` | 30.5 | 0.3 | 0.6 | 22.8 | 21.5 |
| `select_where_order_take` | 34.2 | 0.7 | 1.3 | 18.7 | 22.6 |
| `select_where_sum` | 35.0 | 0.4 | 0.6 | 20.1 | 21.4 |
| `single_match` | 0.0 | 0.3 | 1.1 | 33.7 | 18.6 |
| `skip_take` | 0.3 | 0.0 | 0.0 | 1.2 | 0.2 |
| `skip_while_match` | 3.3 | 0.4 | 0.4 | 31.2 | 18.7 |
| `sort_first` | 35.8 | 0.4 | 1.3 | 17.9 | 22.3 |
| `sort_take` | 35.9 | 0.7 | 1.3 | 18.9 | 23.0 |
| `sort_take_select` | 39.4 | 0.7 | 1.3 | 18.8 | 22.5 |
| `sum_aggregate` | 29.8 | 0.3 | 0.0 | 26.2 | 24.9 |
| `sum_where` | 32.7 | 0.3 | 0.6 | 24.2 | 25.6 |
| `take_count` | 1.8 | 0.1 | 0.1 | 1.2 | 0.2 |
| `take_count_filtered` | 1.1 | 0.0 | 0.0 | 0.4 | 0.1 |
| `take_sum_aggregate` | 0.8 | 0.0 | 0.0 | 0.2 | 0.0 |
| `take_where_count` | 0.8 | 0.0 | 0.0 | 0.2 | 0.0 |
| `take_while_match` | 7.5 | 0.2 | 0.3 | 10.5 | 7.0 |
| `to_array_filter` | 45.8 | 3.2 | 3.3 | 21.2 | 30.8 |
| `where_join_count` | 39.0 | 7.0 | 7.5 | 49.1 | 41.2 |
| `zip_count_pred` | 39.2 | 0.1 | — | 115.6 | 35.8 |
| `zip_dot_product` | 47.0 | 0.1 | 0.1 | 114.6 | 36.0 |
| `zip_dot_product_3arg` | 47.0 | 0.1 | — | 114.9 | 35.9 |
| `zip_reverse_to_array` | — | 4.6 | — | 122.8 | 47.7 |
<!-- BENCH:TABLES END -->

## Notes on missing lanes (the `—` cells)

The reasons each cell is empty are also recorded as a comment in the
corresponding `.das` bench file; the bullets below quote that comment.
For deeper detail on SQL cells — what query each would lower to,
whether the gap is window-function / surface-limitation / by-design,
and which gaps could land in a single PR — see
[`sqlite_linq_gaps.md`](sqlite_linq_gaps.md).

- **`bare_last` SQL (m1)** — bare `.last()` has no SQL analog: SQLite has no
  inherent row ordering, so `_sql` rejects bare `reverse()` / `last()` (use
  `_order_by_descending(...)` for sort reversal). Bare last is only meaningful for
  sources with a natural order — array (index), decs (archetype storage order),
  XML (document order), JSON (array order). By design.
- **`chained_select_collapse` SQL** — `_sql` rejects `distinct() |> count()`
  as non-translatable. The equivalent SQL `COUNT(DISTINCT computed-expr)`
  isn't currently emitted by sqlite_linq's surface. By design — no follow-up.
- **`cross_join` Decs (m4)** — a standalone `from_decs_template` source yields
  anonymous row tuples, so there is no clean typed-lambda cross form
  (`_cross_join` requires typed lambdas and is not `_fold`-integrated yet). The
  decs lane arrives with the cross-join `_fold` engine integration; SQL / array /
  XML / JSON only for now (the JSON lane runs the same UNFUSED `cross_join_to_array`
  library call as the array/XML lanes — there is no `_fold` cross splice yet).
- **`select_many` SQL (m1) / Decs (m4) / XML (m5f) / JSON (m6f)** — the correlated
  flatten needs a per-element nested collection (an `array<…>` field). SQL has no
  nested-collection flatten (correlated select_many over SQL is rejected); a decs
  component has no `array<…>` field; the XML linq source `from_xml_node`
  materializes rows from flat attributes only (`build_xml_row`); and `from_json`
  likewise reads the flat `Car` schema, so neither can populate a nested field.
  Array-only — a source-shape gap, by design for these backends.
- **`decs_count_bare_pred` SQL / Array / XML (m5f) / JSON (m6f)** — covers a Theme 4
  root-cause fix specific to the decs lane (bare `from_decs_template(...).count(P)`
  with no upstream where/select previously bailed because
  `forExpr.iteratorVariables` was unpopulated). Array-side bare `count(P)`
  was always reachable; SQL `count(P)` is covered by `count_aggregate.das`
  with a where shape. The XML / JSON lanes are absent because the family is
  decs-only (it exists to exercise a decs-walk root cause — no array/XML/JSON/SQL
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
- **`zip_*` XML lane (m5f) / JSON lane (m6f)** — each zip
  bench zips the `Car` price-stream against a synthetic int array via the
  mixed `zip(iterator, array)` overload; the zip splice partially fuses over both XML
  and JSON, still paying the unpruned `Car` materialization (so both lanes are lit, JSON
  trading XML's string clones for object-key lookups as elsewhere). The remaining `—` zip
  cells are **SQL (m1)** and **Decs (m4)**: `zip` is not a relational op and not expressible
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
match across the three paths). m3f went sub-resolution then (the 2K
push_clones for K=10 amortize over chunk_size=100K and round to 0.0).
m4 followed only once the boundary archetype was **random-indexed**
rather than `continue`-walked (`reverse_take[_select]` m4 9.3 → 0.2
INTERP, 1.1 → 0.0 JIT — see the header note); the projection amortizes
to 0.0 on both.

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
