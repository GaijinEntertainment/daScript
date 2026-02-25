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
