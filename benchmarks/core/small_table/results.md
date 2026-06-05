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
