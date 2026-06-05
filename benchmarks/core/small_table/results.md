# Small-table benchmark results

Pre-plan measurements for the builtin-table perf work (minCapacity, linear/packed
small mode, hash-only compare, const-key hash). All numbers are ns/op on this
machine; treat as relative, not absolute. `[JIT]` = `-jit`, else interpreter.

Methodology notes:
- hit/miss lanes repeat the N-key sweep REPS=64× inside the measured block so fixed
  per-`run` harness overhead amortizes to ~0 and ns/op reflects one lookup at size N.
- String lookups query with **distinct-pointer** keys (content-equal, freshly built)
  so the `KeyCompare a==b` fast path misses and a real strcmp runs — the realistic
  case. Querying with the stored pointers hits the fast path and understates find by
  the strcmp (~5ns short key, ~23ns for a 38-char key).

## Baseline find cost (current hashed table)

| op | INTERP tail | JIT tail | notes |
|---|---|---|---|
| int hit/miss | ~10–11ns | ~4–5ns | O(1), flat across N |
| string hit (short key, distinct-ptr) | ~22–30ns | ~9–18ns | rises with N (probe growth) |

The strcmp tiebreak measured directly (38-char key, distinct vs same pointer):
**+22.7ns INTERP / +22.8ns JIT**. This is the cost item 3 (hash-only compare) removes.

## #3 minCapacity 8 → 4 (standalone, A/B build)

| N | min8 ns / B/op | min4 ns / B/op | verdict |
|---|---|---|---|
| 1 | 93.7 / 96 | 106.6 / **48** | half the memory, same rehash |
| 2 | 50.4 / 48 | 57.2 / **24** | half the memory, same rehash |
| 3 | 36.3 / 32 | 54.6 / 48 | **+18ns** extra rehash, no mem gain |
| 4 | 30.7 / 24 | 42.5 / 36 | **+12ns** extra rehash |
| 8 | 26.6 | 32.5 | +6ns; many-tables B/op 44 → 50 (worse) |
| 64 | 26.8 | 27.1 | amortized away |

**Conclusion: minCapacity 8→4 helps only N=1–2 (halves the alloc), regresses N≥3**
(one extra rehash to reach the *same* final capacity). Do NOT ship item 1 standalone;
it is only a clean win coupled with packed mode (cap-4 holds 4 at load factor 1.0,
no early rehash).

## #2 Many small tables (4096 tables × 8 elems, decs/per-entity shape)

| | INTERP | JIT | B/op |
|---|---|---|---|
| int_build | 30.0 | 18.2 | 44 |
| int_query (cold sweep) | 11.2 | 5.1 | 0 |
| str_build | 42.2 | 28.6 | 56 |
| str_query (cold sweep) | 20.5 | 14.1 | 0 |

Cold-sweep query ≈ hot single-table query (4096 tables ≈ 1–2MB fits L2, sequential
prefetch). **Cache locality is a minor factor at this scale; the item-1 lever is
memory (B/op), not cache.** The cache angle would only bite at far higher table
counts / randomized access.

## #4 Crossover prototype — predicts maxLinearCapacity

`lin_*` = inlined hash-only linear scan over array<uint64> (packed-find shape);
`hash_*` = real open-addressed `?[]`. **Only the JIT tier is a valid predictor** —
in INTERP the linear lane is interpreted das while the hashed lane is a single
compiled C++ SimNode, so INTERP unfairly buries linear (ignore those rows).

JIT (ns/op):

| N | lin_short | hash_short | lin_long | hash_long |
|---|---|---|---|---|
| 2  | 4.7 | 7.8 | 14.7 | 39.2 |
| 4  | 5.1 | 8.4 | 14.7 | 39.8 |
| 8  | 6.1 | 9.8 | 15.8 | 41.2 |
| 12 | 7.2 | 10.5 | 17.4 | 40.3 |
| 16 | 7.9 | 10.4 | 17.7 | 41.0 |
| 24 | 9.3 | 11.1 | 19.5 | 41.5 |
| 32 | 11.2 | 11.0 | 21.3 | 44.1 |

**Hash-only linear scan beats hashed find up to N≈24–32 for short keys, and well
beyond 32 for long keys** (where the ~23ns strcmp the linear scan skips dominates).
This is a **conservative lower bound**: the linear lane is handicapped by a bound
`hash()` call + bounds-checked array indexing that a real C++ packed find would not
have. Linear-scan cost grows only ~0.2ns/element.

**Conclusion: maxLinearCapacity = 8 is very safe, 16 is comfortable, and even 32 is
defensible — especially for string keys. The crossover win is dominated by skipping
strcmp (item 3), which is why it scales with key length.**

## Packed mode — MEASURED (post-implementation, `maxLinearCapacity = 8`, `minCapacity = 8`)

Re-ran `test01`/`test02`/`test04` against the packed build (items 2 + 3 landed: dense
insertion-order slots at load factor 1.0, hash-only string compare, swap-remove erase,
promote to open-addressing on the 9th insert). Baseline columns are the hashed-table
numbers above. Same machine, same methodology.

### test04 — many small tables (4096 × 8, the decs/per-entity shape) — the headline

| op | INTERP base → packed | JIT base → packed | B/op base → packed |
|---|---|---|---|
| int_build | 30.0 → **24.2** (−19%) | 18.2 → **14.9** (−18%) | 44 → **20** (−55%) |
| int_query | 11.2 → 11.4 (flat)     | 5.1 → 5.9 (+16%, scan tax) | 0 → 0 |
| str_build | 42.2 → **30.2** (−28%) | 28.6 → **13.7** (−52%) | 56 → **24** (−57%) |
| str_query | 20.5 → 20.0 (−2%)      | 14.1 → **7.5** (−47%) | 0 → 0 |

**Memory roughly halves across the board** (load factor 1.0 + cap-8 vs cap-16-at-0.5).
String build/query roughly halve in JIT. The only regression is int_query (+0.8ns JIT):
the linear scan walks the slots while the hashed path was O(1), and int keys get no
strcmp-skip to pay for it. In INTERP the SimNode-dispatch floor masks both effects.

### test01 / test02 — single-table N sweep (packed for N ≤ 8, hashed N ≥ 12)

- **int build** (JIT): `build/8` 16.1ns vs baseline-min8 26.6ns; B/op 12 vs 96-at-N=1.
- **int hit/miss** (JIT): packed `hit/8` 6.5ns, `miss/8` 7.7ns — slightly above the hashed
  N≥12 floor (~4ns). The scan tax, small in absolute terms.
- **str hit** (JIT): packed `hit/8` 7.2ns, flat in N; hashed regime in the same run rises
  (`hit/12` 9.5 → `hit/64` 15.6). Win confirmed even on **short** keys (`key_0`..`key_7`);
  it grows with key length (the +22.8ns long-key strcmp the hash-only path skips).
- INTERP shows the same ordering with the dispatch floor compressing the deltas.

**Verdict: ship.** Build + memory win is uniform and large (the decs case ~halves both);
the string-find win is real and length-scaling; the lone cost is a sub-nanosecond int-query
scan tax that the build/memory win dwarfs. `maxLinearCapacity = 8` is validated; 16 remains
defensible for string-heavy workloads (revisit in sub-phase 3 follow-up alongside
`minCapacity 8→4`).

## Hash intrinsic — A/B MEASURED (JIT, fast 16-bit-load `$::hash` on vs off)

A/B by toggling the `$::hash` registration in `llvm_jit_intrin.das` (cache cleared
between runs); "on" = inlined 16-bit-load intrinsic, "off" = runtime call into
`hash_blockz64`. Numbers are stable over 3 runs (the first cold-codegen run reads
high — discard it).

### Bare `hash(string)` — `core/hash/test13`, 256 runtime keys

| key | intrinsic on | call (off) | verdict |
|---|---|---|---|
| short (`k0`..`k255`) | **1.9ns** | 2.9ns | −34% — call overhead removed |
| long (38-char) | 12.7ns | 12.1ns | ~on par (the win is no-call-overhead; the
  MSVC-optimized `hash_blockz64` loop body is otherwise as good as the JIT's) |

So the 16-bit version meets the bar: faster on short (the common key shape), on par
on long. The earlier byte-by-byte version lost on long; the 16-bit load (half the
iterations) closes that gap.

### Table lookup `tab?[key]` — `core/small_table/test03`, 8-elem table

The intrinsic does **not** reach table lookups, and the numbers confirm it: with vs
without the `$::hash` intrinsic, every lane is identical (3-run stable).

| lane | intrinsic on | intrinsic off |
|---|---|---|
| const_short | 7.0 | 7.0 |
| var_short | 7.0 | 7.0 |
| const_long | 16.2 | 16.7 |
| var_long | 16.5 | 17.0 |

Why: `tab?[key]` lowers to `build_table_find` → the C++ `jit_table_find(tab, key,
valueTypeSize, ctx)` (llvm_jit.das:1856), which takes the **key** and computes the
hash *internally*. The JIT-side `$::hash` intrinsic is never invoked on this path, so
toggling it changes nothing. `const_* == var_*` for the same reason — nothing folds.

(An earlier single-run A/B mis-measured this as −30-38%; that "off" run was an outlier
under background load. The 3-run numbers above are the truth: zero current effect.)

**This was the gap — now closed.** Added `jit_string_table_find_with_hash` /
`jit_string_table_at_with_hash` (C++, take a precomputed hash) and routed string-key
`?[]`/`[]` through them, emitting `hash(key)` via `build_string_hash` (the shared
intrinsic IR). String-only, find+at — 2 functions, not the whole matrix.

### Table lookup `tab?[key]` — with-hash lowering MEASURED (JIT, 3-run stable)

| lane | before (hash in C++) | with-hash | Δ |
|---|---|---|---|
| const_short | 7.0 | **4.7** | −33% |
| const_long | 16.2 | **4.5** | **−72%** |
| var_short | 7.0 | 6.3 | −10% |
| var_long | 16.5 | 17.2 | ~flat |

**Pure item 4 is realized:** a literal key's hash folds to an immediate and disappears —
`const_long` (4.5) now equals `const_short` (4.7), i.e. key length no longer costs
anything for constant keys. `var_*` (runtime keys) still hash; the inlined loop ≈ the old
C++ call (cf. test13), so the straight-line microbench shows little there — the runtime-key
win is loop hoisting / CSE of an invariant key, which this per-key-once bench doesn't expose.

## Inlined packed find — 3-way A/B/C MEASURED (JIT, `JIT_STRING_FIND` none/scalar/vector)

String-key `?[]` find previously always called the C++ helper (which hashes, then for a
packed table runs `PackedFind`). We can inline that fixed 8-slot uint32 hash scan on the JIT
side when the table is small (`cap != 0 && cap <= 8`), falling back to the C++ call for
empty/hashed. Three variants — `NONE` (always C++), `SCALAR` (unrolled 8× select fold),
`VECTOR` (`<8 x i32>` compare + `cttz`). Lookup-only benches, ns/op:

| lane | NONE | SCALAR | VECTOR |
|---|---|---|---|
| const_hit (t06)  | 4.4 | 2.9 | **1.4** |
| const_miss (t06) | 5.9 | 2.6 | **1.2** |
| var_hit (t06)    | 7.0 | 6.9 | **4.8** |
| var_miss (t06)   | 7.8 | 6.7 | **4.7** |
| const_short (t03)| 4.7 | 2.9 | **1.4** |
| const_long (t03) | 4.4 | 3.0 | **1.3** |
| var_short (t03)  | 6.2 | 7.0 | **4.6** |
| var_long (t03)   | 17.2 | 20.0 | 17.8 |

**VECTOR wins across the board and is the pick.**
- Constant keys drop to **~1.3ns** (folded-hash immediate + one SIMD compare) — −70% vs the
  C++ call. `const_miss` is the sharpest (5.9 → 1.2): the C++ path pays call overhead + a full
  scan on a miss, while the vector compare costs the same hit or miss.
- Runtime short keys: ~4.7ns vs ~7 (−33%).
- `var_long` is *hash*-dominated (a long runtime key costs ~14ns to hash), so the find win is
  masked — VECTOR ≈ NONE (17.8 vs 17.2). SCALAR even regresses there (20.0) from its extra
  scalar ops.

**SCALAR** helps constants but is a wash-to-regression on runtime keys — not worth shipping.
VECTOR shipped; the NONE/SCALAR paths and the `JIT_STRING_FIND` toggle were stripped (no dead
code). Gate is `cap != 0 && cap <= TABLE_MAX_LINEAR_CAPACITY` (mirrors C++ `find`: cap==0 → −1, then
packed); the inlined scan reduces the 64-bit hash to the 32-bit `TableHashKey` exactly as
`hashToHashKey` does. Covers `?[]`, `key_exists`, and `find` (all route through
`build_table_find`); `[]`/insert keeps the C++ `at` path.

## Integer-key find + tab[key] update — MEASURED (JIT, test07)

The inline packed find was extended to integer keys (a `<8 x KeyT>` compare of the key data
over `[0,size)` with a liveness mask — no hash on the packed path) and to the insert path
(`tab[key]`: SIMD find first; a hit returns the slot, a miss inserts via
`reserveAfterPackedMiss`, skipping the C++ packed dedup).

| lane | ns/op | note |
|---|---|---|
| int_hit  | 1.8 | pure SIMD key compare, **no hash** (vs ~5.1 baseline C++ int find → −65%) |
| int_miss | 1.6 | same cost as hit (branch-free) |
| int_update (`tab[k]++`) | 4.5 | find-first hit → slot, no C++ `at` call |
| str_update (`tab[k]++`) | 4.1 | same fast-path on the string `at` |

Integer find is the standout: with no hash to compute, a small int-keyed lookup is ~1.7ns —
the SIMD compare *is* the whole operation. Inserts of genuinely new keys also skip the
redundant C++ `PackedFind` dedup the JIT already performed. Integer keys only
(`int`/`uint` ×8/16/32/64); float/double/struct keep the C++ path.

## Correctness: 32-bit hashKey collisions — confirm on find (SUPERSEDED by §"64-bit packed hash")

> **Superseded.** The confirm-on-find / promote-on-collision design below was the intermediate
> step. It is **replaced** by the 64-bit packed hash (final section) — packed string find now
> compares a full 64-bit hash exactly, with no strcmp and no confirm. Read this section for the
> *why* (32-bit hashKey is too narrow to trust); read §"64-bit packed hash" for shipped behavior.

The hash-only string path above is **incorrect**: `TableHashKey` is `uint32_t`, so distinct keys
can share a hashKey and a hash-only match returns a false positive. Fix (all tiers):
- packed string find returns a hash *candidate*; the caller confirms it with one `KeyCompare`
  (C++) / bound `jit_string_equal` (JIT). Misses with no hash match never reach the confirm.
- `reserve` promotes a packed table to open addressing on the first hashKey collision (a
  confirmed hash-match with a *different* key), so a packed table never holds two same-hashKey
  slots — the single candidate is always exact. The int/data path is unaffected (exact compare,
  no hash, no confirm).

**Benchmark fix:** the `var_*` HIT lanes previously queried with the *stored* key pointer
(`let k = KEYS[i]`), so the confirm hit `KeyCompare`'s `a==b` pointer-identity fast path and
**never ran strcmp** — flattering every string hit. They now query distinct-pointer copies
(`clone_string`), the realistic case (a key from I/O / parsing is never the stored pointer). The
`var_*` MISS lanes and the int lanes are unaffected (a miss never confirms; ints never strcmp).

Corrected JIT ns/op (warm, 3-run avg), vs the hash-only (wrong) figures and the pre-plan hashed
baseline (§"Baseline find cost"). `confirmed` = honest distinct-pointer measurement:

| lane | hashed baseline | hash-only (WRONG) | confirmed (correct) |
|---|---|---|---|
| const_hit (t06)  | ~9–18 | 1.4 | **6.8** |
| const_miss (t06) | ~9–18 | 1.2 | **1.1** |
| var_hit (t06)    | ~9–18 | 4.8 | **10.6** |
| var_miss (t06)   | ~9–18 | 4.7 | **4.0** |
| const_short (t03)| ~9–18 | 1.4 | **6.8** |
| const_long (t03) | ~9–18 | 1.3 | **20.8** |
| var_short (t03)  | ~9–18 | 4.6 | **10.5** |
| var_long (t03)   | ~9–18 | 17.8 | **43.5** |
| int_hit (t07)    | ~4–5 | 1.8 | **1.8** |
| int_miss (t07)   | ~4–5 | 1.6 | **1.7** |
| int_update (t07) | — | 4.5 | **3.8** |
| str_update (t07) | — | 4.1 | **9.9** |

Reading it:
- **Misses and the whole int path are unchanged** — the confirm only fires on a hash-candidate
  hit, and ints never hash/confirm. `const_miss` ~1.1ns and `int_hit` ~1.8ns stand.
- **The const-vs-var gap is now the runtime hash** (both lanes pay one strcmp): short keys
  ~3.7ns (10.5 − 6.8), the 38-char key **~22.7ns** (43.5 − 20.8). That gap is exactly item 4's
  target — folding a constant key's hash to an immediate. `const_long` ≈ 20.8 is almost entirely
  the strcmp (hash folds to 0), so **strcmp(38) ≈ 20ns and hash(38) ≈ 22ns** — both O(len) walks
  of the same bytes.
- **String hits pay one strcmp confirm** — the cost the hash-only path skipped (incorrectly).
  The confirm is a *bound call*; its fixed overhead is a large slice of the short-key cost
  (`const_hit` 6.8 for a ≤7-char key), so an inlined/generated `string==` is the obvious next
  lever — it cannot help the long-key strcmp itself, but removes the call overhead on short keys.
- **Vs the hashed baseline:** packed wins on misses (skips strcmp entirely: ~1.1–4ns vs ~9–18)
  and on int (~1.8 vs ~4–5). For string hits packed pays the *same* one strcmp a hashed hit pays
  on a cheaper scan, so it is at worst a wash and better for short keys; long-key hits are
  hash+strcmp-dominated either way (the scan saving is in the noise).

The §"Inlined packed find — 3-way" VECTOR column and the `str_update` row in §"Integer-key find"
are the pre-confirm (hash-only) measurements; keep them for the SIMD-scan A/B but read the table
above for shipped behavior.

## Realistic literal-keyed "JSON" read — const-string pooling (test08)

4096 small string-keyed records (6 fields, packed), summed by addressing each field with a string
LITERAL. daslang pools constant strings, so the literal that *built* a field (`tab["foo"] = 1`)
and the literal that *reads* it (`tab?["foo"]` / `tab["foo"]`) are the same pointer — the find's
candidate confirm hits KeyCompare's `a==b` fast path and skips strcmp. This is the common case for
fixed-field-name access (config, JSON-shaped data, per-entity property maps).

The `bad_*` lanes are the control: same scan (const literals) but the table is built with keys
assembled at runtime (`var foo = "fo"; foo += "o"`), so the stored key is a distinct heap pointer,
`a==b` misses, and a real strcmp runs every lookup (the table-from-parsed-data case).

| lane | INTERP | JIT | path |
|---|---|---|---|
| json_find (good, const-pull `a==b`)   | ~11.8 | **~3.6** | find (ExprSafeAt) |
| json_at (good)                        | ~10.0 | **~3.4** | at/index (ExprAt) |
| bad_json_find (distinct ptr, strcmp)  | ~14.0 | ~6.3 | find |
| bad_json_at (distinct ptr, strcmp)    | ~12.5 | ~5.8 | at |
| **strcmp tax (bad − good)**           | **~2.3** | **~2.5** | short 3–4 char key |

- Literal-keyed access is **~3ns JIT / ~10–12ns interp** — near int-key cost, and **~3× faster
  than the distinct-pointer string hit** (test06 `var_hit` 10.6). The win is the `a==b` confirm
  (no strcmp) on a cheap short-key hash + the SIMD packed scan; `json_at` edges `json_find` by the
  `?? 0` the find lane carries.
- **The strcmp tax is tier-independent (~2.5ns for a 3–4 char key)** — it is the same C++ compare
  on both tiers; the ~3.3× interp/JIT gap is fixed per-node dispatch layered on top, not the table
  op. So const-pull is worth ~2.5ns/lookup on a hit, on every tier.
- const-pull also makes these keys' hash a foldable constant — the unexploited item-4 headroom
  sitting under the good lanes.

## 64-bit packed hash — strcmp eliminated (SHIPPED, supersedes the confirm approach)

The confirm approach (32-bit `TableHashKey` + a `KeyCompare` / bound `jit_string_equal` confirm on
every hash-candidate hit, promote-to-open-addressing on the first hashKey collision) is **replaced**.
Packed small tables (`capacity <= TABLE_MAX_LINEAR_CAPACITY`) now store a **64-bit** hash per slot
(uniform for *all* key types — the width is a pure function of capacity, so type-erased free/GC sites
need no keytype); large tables keep the 32-bit `TableHashKey`. Packed string find compares the full
64-bit hash **exactly** and returns the slot — **no strcmp, no confirm, no collision-promote** (a
64-bit hash collision between two distinct live keys in an ≤8-slot table is not a real-world event).
Packed workhorse find compares key *data* (the 64-bit hash is stored-but-unused on that path — the
+32B/table it costs is reclaimed by the deferred workhorse-no-hash follow-up).

The clean-tail invariant makes the string scan branch-free over all 8 slots: alloc / erase /
`table_clear` clear the full 64-bit width, a real hash is always `> HASH_KILLED64`, so the dead tail
(value 0) never matches — no size mask needed.

### JIT ns/op — 64-bit packed vs the confirm build (3-run warm)

| lane | hashed baseline | confirm (32-bit + strcmp) | **64-bit packed** | Δ vs confirm |
|---|---|---|---|---|
| const_hit (t06)  | ~9–18 | 6.8  | **2.3**  | −66% |
| const_miss (t06) | ~9–18 | 1.1  | **1.8**  | +0.7 (no strcmp either way) |
| var_hit (t06)    | ~9–18 | 10.6 | **5.5**  | −48% |
| var_miss (t06)   | ~9–18 | 4.0  | **5.6**  | +1.6 |
| const_short (t03)| ~9–18 | 6.8  | **3.5**  | −49% |
| const_long (t03) | ~9–18 | 20.8 | **2.2**  | **−89%** (strcmp(38) gone) |
| var_short (t03)  | ~9–18 | 10.5 | **8.8**  | −16% |
| var_long (t03)   | ~9–18 | 43.5 | **20.6** | **−53%** (now pure hash, no strcmp) |
| int_hit (t07)    | ~4–5  | 1.8  | **2.6**  | +0.8 (data compare unchanged; noise) |
| int_miss (t07)   | ~4–5  | 1.7  | **1.9**  | ~flat |
| int_update (t07) | —     | 3.8  | **4.3**  | +0.5 |
| str_update (t07) | —     | 9.9  | **5.7**  | −42% |

Reading it:
- **String hits no longer strcmp.** `const_long` 20.8 → **2.2** is the whole story: that 20.8 was
  almost entirely `strcmp(38)`, and it's gone — `const_long` (2.2) now equals `const_short` (3.5),
  key length is free for constant keys. `var_long` 43.5 → **20.6** drops by the same ~23ns strcmp; the
  residual 20.6 is the *runtime hash* of the 38-char key (the find itself is the cheap 64-bit compare).
- **Short-key string hits** drop ~−16 to −66% — the confirm's bound-call overhead is gone with it.
- **The int data path is unchanged** (SIMD key-data compare, no hash); the sub-nanosecond up-tick on
  the int lanes is run-to-run noise plus the wider (64- vs 32-bit) hash array touching one more line.

### INTERP ns/op — 64-bit packed (per-SimNode dispatch floor compresses the deltas)

| lane | INTERP | | lane | INTERP |
|---|---|---|---|---|
| const_hit  | 12.0 | | int_hit    | 11.2 |
| const_miss | 14.9 | | int_miss   | 13.3 |
| var_hit    | 12.6 | | int_update | 10.6 |
| var_miss   | 13.7 | | str_update | 11.1 |
| const_short| 12.0 | | const_long | 21.8 |
| var_short  | 11.8 | | var_long   | 29.2 |

INTERP `const_long` 21.8 / `var_long` 29.2: the strcmp is gone here too (`const_long` was ~strcmp-
dominated under confirm), the residual long-key cost is the hash. The ~10–15ns floor on the short
lanes is fixed SimNode dispatch, not the table op.

### Realistic JSON read — good vs bad now CONVERGE (test08, the headline)

The §"Realistic literal-keyed JSON read" `bad_*` control existed to isolate the strcmp tax: `good`
keys are const-pooled (`a==b` skips strcmp), `bad` keys are runtime-assembled distinct pointers
(parsed-data shape, strcmp every lookup). **With no strcmp anywhere, that distinction collapses** —
both lanes compare the 64-bit hash:

| lane | INTERP confirm → 64-bit | JIT confirm → 64-bit |
|---|---|---|
| json_find (good)     | 11.8 → **11.2** | 3.6 → **2.1** |
| json_at (good)       | 10.0 → **10.0** | 3.4 → **1.7** |
| bad_json_find (parsed)| 14.0 → **11.4** | 6.3 → **2.3** |
| bad_json_at (parsed) | 12.5 → **10.0** | 5.8 → **1.5** |
| **good/bad gap (JIT)** | | **2.5 → ~0.2** |

The parsed-data case (`bad_json_find` 6.3 → **2.3** JIT) was the one paying strcmp; it now matches the
const-pooled case (`json_find` 2.1). **A table built from parsed/IO keys reads as fast as one built
from string literals** — the strcmp tax that const-pooling used to dodge no longer exists for anyone.

## Synthesis for the plan

- **Items 1 + 2 are coupled.** Packed small mode (insertion-dense slots, load factor
  1.0, linear hash-only scan, swap-remove erase, promote to open-addressing past the
  threshold) is the structural core. minCapacity 4 only makes sense inside it.
- **Item 3 is the dominant string win** and is the main reason linear scan wins; it
  rides inside packed mode (trust the 64-bit hash, skip strcmp — small-table mode only).
- **Item 4** (const-key precomputed hash) validated independently: a JIT hash intrinsic
  const-folds a literal-key hash to an immediate (free). Currently disabled; wire it in
  with a find-with-precomputed-hash seam when we build item 4.
- **Threshold:** start at 8 (conservative), 16 well-supported by the data. A single
  global value captures most of the win for both key types; per-type is optional polish.
