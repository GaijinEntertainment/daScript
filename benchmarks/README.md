# Benchmark Index

> **Keep up to date:** When adding, removing, or renaming benchmark files under `benchmarks/`, update this file. See `skills/writing_benchmarks.md` for the rule.

Every `.das` benchmark file in this directory tree is listed below, grouped by subdirectory.

## core/hash/

| File | Description |
|---|---|
| `_common.das` | Shared helper module — random number generation, shuffling, intersection helpers (not a benchmark) |
| `_slot_map.das` | SlotMap template macro module — wraps hash maps as ID-based slot allocators (not a benchmark) |
| `test02.das` | Sequential insert + read of 600K elements across builtin table, cuckoo, and flat hash maps |
| `test03.das` | Insert 1M unique random numbers, clear, re-insert — builtin table, cuckoo, and flat hash maps |
| `test04.das` | Insert 1M unique random numbers then erase all — tests erase performance |
| `test05.das` | Overwrite same key (key=3) 300K times — single-key hot-path update performance |
| `test06.das` | Insert 1M random keys with 10% collisions, incrementing struct value — collision handling |
| `test07.das` | Insert 1M random keys with 50% collisions, incrementing struct value — heavy collision |
| `test08.das` | Insert 1M random numbers then look up all via key_exists — lookup-only performance |
| `test09.das` | Insert 1M random numbers then look up and sum via get() callback — read performance |
| `test10.das` | Pure insert of 1M unique random numbers — insert-only (no read, no clear) |
| `test11.das` | SlotMap adapter insert of 1M random numbers — monotonic uint64 IDs, entity allocation pattern |
| `test12.das` | Pathological key distribution — 8K sequential + random keys (100K total) with deliberately bad hash |

## core/bool_array/

| File | Description |
|---|---|
| `test01.das` | Push 1M alternating true/false — BoolArray vs array&lt;bool&gt; append performance |
| `test02.das` | Index access — count trues via a[i] in 1M elements — BoolArray vs array&lt;bool&gt; |
| `test03.das` | Iteration access — count trues via `for (e in a)` in 1M elements — BoolArray vs array&lt;bool&gt; |
| `test04.das` | Compound XOR (^^=) bit flip on 1M elements — BoolArray compound vs simple vs array&lt;bool&gt; |
| `test05.das` | Insert at index 0 — push 10K values at front — BoolArray vs array&lt;bool&gt; |
| `test06.das` | Erase at index 0 — erase 10K values from front — BoolArray vs array&lt;bool&gt; |

## core/table_key_value/

| File | Description |
|---|---|
| `test01.das` | `get_key(tab, v)` vs `keys(tab)+values(tab)` — single iterator with pointer arithmetic vs two parallel iterators (1K, 10K, 100K entries) |

## core/small_table/

Small-table regime micro-benchmarks (N from 1 to 64) — the load profile the large-table `core/hash/` suite does not cover. Used to locate the linear-vs-hashed crossover (`maxLinearCapacity`) and to measure the string-key and constant-key paths.

| File | Description |
|---|---|
| `test01.das` | Integer-keyed small tables — build (insert+alloc), positive lookup (hit), negative lookup (miss), swept across N |
| `test02.das` | String-keyed small tables — build/hit/miss sweep; hit queries with distinct-pointer keys (strcmp fires, realistic), keys pre-built outside the measured block |
| `test03.das` | Constant string-literal keys (`const_*`) vs runtime-array keys (`var_*`) — baseline for item 4 (precomputed-hash specialization of constant keys) |
| `test04.das` | Many small tables (decs/per-entity shape) — thousands of independent N-element tables, build (watch B/op for item 1 memory) and cold-sweep query (cache locality) |
| `test05.das` | Crossover prototype — inlined hash-only linear scan (`lin_*`) vs real open-addressed `?[]` (`hash_*`) over N, short and long keys; predicts `maxLinearCapacity` before packed mode exists |
| `test06.das` | Lookup-only (never insert) packed string-table find at N=8 — const/var keys, hit and miss; A/B/C harness for the JIT inlined packed find (`JIT_STRING_FIND` none/scalar/vector) |
| `test07.das` | Packed (N=8) integer-key find (hit/miss, no-hash SIMD key compare) and `tab[k]++` update fast-path (string + int) — the paths added when the inline find was extended to integer keys and to insert |

## core/array/

| File | Description |
|---|---|
| `test01.das` | emplace, emplace_grow, move, and reserve on arrays of locked vs unlocked struct elements |
| `test02.das` | push vs push_clone — int and struct with string field (10K elements, pre-reserved) |

## core/array_lock/

| File | Description |
|---|---|
| `test01.das` | emplace, emplace_grow, move, and reserve on arrays of locked vs `[skip_field_lock_check]` struct elements — measures array lock overhead |

## fusion/

| File | Description |
|---|---|
| `bench_v_ldu.das` | Fusion-engine `Op2At` array-indexed read at sizeof(T) ∈ {4,8,12,16} — int, int64, float3, float4. Used to compare DAS_FUSION=0 vs current `DAS_LDU_WORKHORSE` ladder vs `v_zero+memcpy(sizeof(CTYPE))` |

## sql/

Multi-lane comparison over the same `Car` schema: `_sql` macro over `:memory:` SQLite vs in-memory `array<Car>` linq splice vs decs (`[decs_template]`) linq splice vs XML (`from_xml_node`) vs JSON (`from_json`). Each bench builds the same data several ways via `_common.das` fixtures and runs the same query expression through each lane. See `benchmarks/sql/results.md` for the current ns/op numbers across both INTERP and JIT.

| Lane | Source | Form |
|---|---|---|
| `m1` (SQL) | `:memory:` SQLite | `_sql` macro — compile-time SQL emission, work pushed to the engine |
| `m3f` (Array) | pre-populated `array<Car>` | `_fold` over `each(arr).chain()` — fuses the chain into a single pass |
| `m4` (Decs) | decs entities via `[decs_template]` | `_fold` over `from_decs_template(type<DecsCar>).chain()` — fuses into a per-archetype walk |
| `m5f` (XML) | in-memory XML doc (`fixture_xml_string`) | `_fold` over `from_xml_node(root, type<Car>)` — `XmlAdapter` fuses + field-prunes the chain into one DOM walk |
| `m6f` (JSON) | pre-built `JsonValue?` array (`fixture_json`) | `_fold` over `from_json(jv, type<Car>)` — `JsonAdapter` fuses + field-prunes into one array walk (mirror of m5f; aliases heap strings instead of cloning) |

The `m3` lane (eager linq, no `_fold` splice) was dropped on 2026-05-23; the splice ladder closed the gap between m3f and m4 across the corpus, and m3 was no longer a useful comparison point.

| File | Description |
|---|---|
| `_common.das` | Shared `Car` `[sql_table]` + `[decs_template]` + `Dealer` schema + fixture builders (not a benchmark) |
| `aggregate_match.das` | `_where + aggregate(seed, op)` — user-supplied binary reducer over a filtered slice |
| `all_match.das` | `all(P)` with always-true predicate — full scan, returns true |
| `any_match.das` | `any(P)` — first-hit early exit |
| `average_aggregate.das` | `average(_.price)` — single-row scalar reduce |
| `bare_order_where.das` | `_where + _order_by(_.price)` — fused prefilter + sort, no take |
| `chained_where.das` | `_where + _where + count` — two filter stages then count |
| `contains_match.das` | `contains(needle)` — early-exit equality scan |
| `count_aggregate.das` | `count()` — engine pushes `COUNT(*)`; array/decs fuse where+count |
| `distinct_by_count.das` | `_distinct_by(_.brand) \|> count()` (Array/Decs only; SQL TODO) |
| `distinct_count.das` | `_select(_.brand).distinct() \|> count()` — projection then dedup |
| `distinct_take.das` | `_select(_.brand).distinct().take(N)` — early-exit dedup |
| `element_at_match.das` | `_where + element_at(N)` — skip then take 1 |
| `first_match.das` | `_where + first()` — first-hit |
| `first_or_default_match.das` | `_where + first_or_default(d)` — first-hit with default |
| `groupby_average.das` | `_group_by(_.brand) + _select(AvgPrice = avg)` |
| `groupby_count.das` | `_group_by(_.brand) + _select(N = count)` |
| `groupby_first.das` | `_group_by(_.brand) + _select(FirstCar = first per group)` (Array/Decs only; SQL TODO) |
| `groupby_having_count.das` | `_group_by + _having(length >= 5) + _select` |
| `groupby_having_hidden_sum.das` | `_group_by + _having(sum > 50000) + _select` |
| `groupby_max.das` | `_group_by(_.brand) + _select(MaxPrice = max)` |
| `groupby_min.das` | `_group_by(_.brand) + _select(MinPrice = min)` |
| `groupby_multi_reducer.das` | `_group_by + 4-slot named tuple` — count + sum + max + … fused |
| `groupby_select_sum.das` | `_select(_.price) + _group_by(_ % 100) + _select((K, S=sum))` (Array/Decs only; SQL TODO — expression keys) |
| `groupby_sum.das` | `_group_by(_.brand) + _select(TotalPrice = sum)` |
| `groupby_where_count.das` | `_where + _group_by + _select(N = length)` |
| `groupby_where_sum.das` | `_where + _group_by + _select(TotalPrice = sum)` |
| `indexed_lookup.das` | `_where(_.id == K)` against the PRIMARY KEY — SQLite uses PK b-tree; Array/Decs scan linearly |
| `join_count.das` | `_join(cars, dealers, on = c.dealer_id == d.id) + count()` (Decs lane absent — TODO: decs join machinery) |
| `last_match.das` | `_where + last()` — carry-last terminator |
| `long_count_aggregate.das` | `long_count()` — int64 counter |
| `max_aggregate.das` | `max(_.price)` — streaming max |
| `min_aggregate.das` | `min(_.price)` — streaming min |
| `order_by_multi_key.das` | `_where + _order_by_keys((_.brand, _.price), mask)` — multi-key composite stable_sort, no take (single-key stays unstable; SQL pushes `ORDER BY c1, c2`) |
| `order_take_desc.das` | `_order_by_descending(_.price) + take(N)` — top-N largest |
| `reverse_take.das` | `reverse + take(N)` — tail N rows |
| `select_count.das` | `_select + count` — projection then counter (DCE'd in some lanes) |
| `select_where.das` | `_where + _select` — filter then project |
| `select_where_count.das` | `_select(2*price) + _where(> T) + count` — where-after-select |
| `select_where_order_take.das` | `_where + _select + _order_by + take(N)` — full chain |
| `select_where_sum.das` | `_select(2*price) + _where(> T) + sum` — where-after-select fused with accumulator |
| `single_match.das` | `single` — assert-one-element with full scan |
| `skip_take.das` | `skip(M) + take(K) + to_array` — windowing |
| `skip_while_match.das` | `skip_while(P) + count` — predicate-driven skip |
| `sort_first.das` | `_order_by + first` — streaming-min (no buffer) |
| `sort_take.das` | `_order_by + take(N)` — bounded-heap (size N) |
| `sum_aggregate.das` | `sum(_.price)` — engine pushes `SUM(price)`; array/decs accumulator |
| `sum_where.das` | `_where + _select + sum` — three-stage |
| `take_count.das` | `take(N) + to_array` — bounded materialization |
| `take_count_filtered.das` | `_where + take(N) + count` (Array/Decs only; SQL semantically distinct — LIMIT-on-aggregate) |
| `take_sum_aggregate.das` | `take(N) + sum` (Array/Decs only; SQL semantically distinct) |
| `take_while_match.das` | `take_while(P) + count` — predicate-driven take |
| `to_array_filter.das` | `_where + _select + to_array` — three-stage materialize |
| `zip_dot_product.das` | `zip(a, b) + _select(_._0 * _._1) + sum` (Array/Decs only; zip is not relational, no SQL form) |

## micro/

Standalone micro-benchmarks — self-contained (no `_common`), kept out of `sql/` so they don't interfere with the `results.md` table-building sweep. Used to validate a design choice with hand-coded "fake code" (the AST the macro would emit) before building the macro.

| File | Description |
|------|-------------|
| `join_select_shapes.das` | `join \|> select` output shapes over XML: C tier-2 (materialize tuples) vs A fused-materialize vs B streaming generator — A wins in every config; streaming is slower (generator overhead), string clone is a flat orthogonal cost |
| `json_source_shapes.das` | `from c : Car in jv where … select c.name` over a JSON array: A hand field-pruned vs B hand full-row `from_JV` vs C the shipped JsonAdapter via `_fold` — field-pruning wins 3.7× (A vs B: INTERP 177 vs 650 ns/op; JIT 49 vs 183). C (the macro) fuses to A's shape and slightly beats it (157/37). Validated the field-pruning design; now also a regression check that the adapter matches hand-optimal |
| `single_last_shapes.das` | `where(pred) \|> single/last` over XML: C full-materialize-per-element vs A materialize-under-guard (read predicate field cheaply, build the full row only for matching elements) — A wins (single 6.1× INTERP / 100k→1 clone; last 1.66× / clones only matching rows) |
| `apply_vs_apply_imm.das` | `apply` (inline ref-local `let/var field &`) vs `apply_imm` (assume-aliased) over a struct field walk — both inline (no per-field invoke), but assume carries no per-field local so it is ~25% faster INTERP (54 vs 41 ns/op), identical JIT (2.0). vs the old per-field-invoke `apply` (113 ns/op) both are ~2× faster. Justifies keeping `apply_imm` as the struct-only hot path |
| `serialize_apply.das` | `JV(struct)` serialization — regression guard for dropping the function-escaping `return` from `to_JV(struct)` so its `apply` block inlines instead of falling to the per-field invoke fallback. Inline 1161/579 ns/op (INTERP/JIT) vs invoke-fallback 2083/1424 = **1.79× / 2.46× faster, −2 allocs/op**. `to_XML(struct)` got the same rewrite (build-gated, not benched here) |
