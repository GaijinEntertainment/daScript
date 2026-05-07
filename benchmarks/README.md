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

3-mode comparison: `_sql` macro over `:memory:` SQLite vs pure in-memory `array<T>` LINQ, with the array form measured both in its naive (intermediate-materializing) and `_fold`-fused shapes. Mirrors the `tests/dasSQLITE/parity_check_*.das` pattern but oriented to throughput.

| Mode | Source | Form |
|---|---|---|
| `m1` | `:memory:` SQLite | `_sql` — compile-time SQL emission, work pushed to the engine |
| `m3` | pre-populated `array<Car>` | plain LINQ chain — materializes intermediate filter/sort arrays |
| `m3f` | pre-populated `array<Car>` | `_fold` from `daslib/linq_boost` — fuses the chain into a single pass, in-place where possible |

| File | Description |
|---|---|
| `_common.das` | Shared `Car` `[sql_table]` + `fixture_db` / `fixture_array` (not a benchmark) |
| `select_where.das` | Filter chain — `_where(_.price > 500)` over 10K rows. Modest asymmetry; m3 walks every row. |
| `select_where_order_take.das` | Filter + sort + limit — `_where \|> _order_by(_.price) \|> take(10)`. SQL ORDER BY + LIMIT bounds work; m3 sorts the full filtered set. |
| `count_aggregate.das` | Aggregate — `count()` after `_where` over 100K rows. SQL pushes `COUNT(*)` to the engine returning one row; m3 materializes the full filtered array then counts it; m3f fuses where+count into one pass. Highest-asymmetry chain. |
