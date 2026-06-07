# Builtin-table benchmark notes

Current builtin `table` design (as shipped):
- **Non-string keys** are open-addressed at every capacity — a 1-byte control array
  (EMPTY / TOMBSTONE / OCCUPIED), no per-slot hash, direct key compare. No packed regime.
- **String keys** pack for capacity ≤ 8 (dense, exact 64-bit-hash compare, no strcmp), then
  open-address with a 32-bit hashKey per slot + strcmp tiebreak.
- **JIT inlines find and `tab[key]`** for all workhorse keys (int widths, bool, float, double)
  and strings — open-addressed probe inline; a write commits in place unless it must grow /
  rehash / the table is locked, which fall to the C++ helper. No C++ interop on the steady path.

ns/op on this machine (Apple silicon) — **relative, not absolute**. JIT is high run-to-run
variance; INTERP is the reliable signal.

## How to run

```bash
bin/daslang      dastest/dastest.das -- --bench --test benchmarks/core/<dir>   # INTERP
bin/daslang -jit dastest/dastest.das -- --bench --test benchmarks/core/<dir>   # JIT
```

`hash/` = large int / uint64 tables; `table/` = int; `small_table/` = small + string; also
`benchmarks/fusion/bench_table_index_i64.das`. `core/hash/test14` is the large string-vs-int
comparison; `core/small_table/test07` is the small int-vs-string comparison.

## Large tables (1M entries, `core/hash/test14`)

| op | int INTERP / JIT | string-fresh INTERP / JIT | string-same JIT |
|---|---|---|---|
| `get_sum` | 132.9 / 59.6 | 181.0 / 78.2 | 71.0 |
| `insert`  | 37.5 / 26.4  | 60.5 / 42.9  | — |

`_fresh` queries with distinct-pointer copies (real strcmp behind the 32-bit hashKey pre-filter
— the realistic case); `_same` reuses stored pointers (KeyCompare `a==b` shortcut), so the gap is
the strcmp. The string lane is work-dominated (FNV hash + strcmp, both intrinsic); inlining the
probe recovers the C++-call slice (string `get_sum` 82.7→78, `insert` 46.6→43 vs the pre-inline
build). Int is fully inline.

## Small tables (size 8, `core/small_table/test07`)

| op | int INTERP / JIT | string INTERP / JIT |
|---|---|---|
| hit    | 6.6 / 1.0 | — |
| update | 4.1 / 1.6 | 5.5 / 3.2 |

Small string tables are packed (inline SIMD 64-bit-hash scan); the int-over-string gap is
intrinsic string hashing, not interop.

## Optimizer barrier

Inlining the find removes the C++-call barrier, so LLVM can optimize through a lookup.
`key_exists` over 1M keys with an **unused** result folds to **0.0 ns/op** under JIT
(`core/hash/test08`) — the loop is eliminated — while the same probe whose value is **consumed**
measures ~60 ns (`get_sum`, `core/hash/test09`). The pre-inline C++ call blocked this.

## Problematic lanes

- `flat_hashmap_pathalogical` (`core/hash`) — deliberately bad hash distribution; the ~14000 ns/op
  is expected (worst-case probe chains), not a regression.
- The cuckoo / flat lanes in `core/hash` are alternative-hashmap baselines
  (`daslib/cuckoo_hash_table`, `daslib/flat_hash_table`), not the builtin table — compare
  like-for-like, not builtin-vs-cuckoo.
