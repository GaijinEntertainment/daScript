---
slug: libcxx-stdsort-block-partition-pdqsort
title: why is libc++ std::sort 2.5× faster than our das_qsort_r pdqsort-lite on workhorse types at N=100K?
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

Investigated 2026-05-17 in the PR #2706 follow-up: on macOS, the std::sort beating das_qsort_r by 2.5× at N=100K (int32/int64) is **libc++**, NOT libstdc++. libstdc++ ships plain Musser introsort and is roughly the same algorithm as ours. libc++ ships **block-quicksort pdqsort** since 2021 ([D93923](https://reviews.llvm.org/D93923), Kutenin) — same algorithm as Orson Peters' pdqsort + the Edelkamp/Weiß BlockQuicksort partition.

## Three specific techniques in libc++

Source: `/Library/Developer/CommandLineTools/SDKs/MacOSX26.4.sdk/usr/include/c++/v1/__algorithm/sort.h`.

### 1. `__bitset_partition` (sort.h:495) — branchless partition

Populate a `uint64_t` bitmask of comparison outcomes for 64 elements (branchless inner loop), THEN do swaps in a separate pass driven by `countr_zero` (tzcnt):

```cpp
// populate (no swaps, no branches)
for (int __j = 0; __j < 64;) {
  bool __comp_result = !__comp(*__iter, __pivot);
  __left_bitset |= (uint64_t(__comp_result) << __j);
  __j++; ++__iter;
}
// swap (predictable loop driven by tzcnt)
while (__left_bitset != 0 && __right_bitset != 0) {
  difference_type __tz_left  = __countr_zero(__left_bitset);
  __left_bitset              = __libcpp_blsr(__left_bitset);
  ...
  _Ops::iter_swap(__first + __tz_left, __last - __tz_right);
}
```

Cuts branch mispredictions from ~32/partition (50% rate over Hoare's `while (cmp) ++i; while (cmp) --j;`) to **one per 64 elements** (when the bitset empties). On random int32 at N=100K this is the dominant win.

### 2. Branchless small-sort kernels (sort.h:99–198)

Sub-ranges of 2/3/4/5 elements use sorting networks built from `__cond_swap`:

```cpp
// sort.h:67
bool __r = __c(*__x, *__y);
value_type __tmp = __r ? *__x : *__y;
*__y = __r ? *__y : *__x;
*__x = __tmp;
```

Clang lowers `bool ? T : T` on arithmetic types to `csel`/`cmov` — zero branches. Gated by `__use_branchless_sort` (sort.h:54): `contiguous_iterator && is_arithmetic && (std::less || std::greater)`. **Fundamentally typed.**

### 3. Tukey ninther + already-partitioned fast-path (sort.h:775, 809)

For `__len > 128`, pivot is median-of-three-medians-of-three (5× `__sort3` calls). After partition, returns a `bool already_partitioned`; caller then tries an **incomplete** insertion sort capped at 8 inversions before recursing. Pre-sorted/nearly-sorted runs collapse to O(N).

## What is and isn't portable

**Portable to byte-pointer (`(void*, width, cmp)` binding path):**
- Block-partition bitset loop — `__builtin_ctzll` is universal. Swaps go through byte_swap.
- Ninther pivot + already-partitioned bool — pure logic, works at any width.

**Requires typed access:**
- `__cond_swap` → cmov (compiler needs a real value type to emit conditional move)
- SIMD-vectorization of the 64-element populate-bitset loop (compiler needs constant width + inlined cmp to vectorize)

## Bake-off candidates (priority order)

| # | Candidate | Portable to byte-ptr? | Est. lift | Effort |
|---|---|---|---|---|
| D | Ninther + already-partitioned bolt-on to das_qsort_r | yes | small | half day |
| C | Block-partition pdqsort, byte-pointer | yes | **big — likely most of the 2.5×** | 2-3 days |
| B | Block-partition pdqsort, typed das_sort\<T\> | n/a (typed only) | biggest | 2-3 days on top of C |
| A | Dispatch typed das_sort\<T\> from binding for workhorse types | n/a (typed only) | medium | 1 day |
| E | Branchless `__cond_swap` small-sort kernels | n/a (typed only) | small decoration | half day |

Critical question the bake-off settles: if (C) ≈ (B), the gap is algorithmic (block partition wins regardless of typedness). If (B) ≫ (C), the gap is SIMD-vectorize.

## Verified comparison data (2026-05-17, M-series Mac, Apple clang, libc++)

`examples/sort/bench_sort_family.cpp` deep-dive at N=100K:

| type | std::sort | C qsort | das_qsort_r | das_sort\<T\> |
|---|---:|---:|---:|---:|
| int32_t | 1.86M | 7.49M (**4.03×**) | 4.77M (2.57×) | 4.86M (2.62×) |
| int64_t | 1.92M | 7.49M (**3.89×**) | 4.77M (2.48×) | 4.84M (2.52×) |
| P32 | 4.97M | 8.44M (1.70×) | 5.48M (1.10×) | 5.64M (1.13×) |
| P128 | 7.44M | 11.91M (1.60×) | 8.80M (1.18×) | 9.63M (1.30×) |

das_sort\<T\> (typed pdqsort-lite, same algorithm as das_qsort_r) is ≈ identical to das_qsort_r byte-pointer at every (N, type). The 2.5× gap is algorithm, not API. C qsort (the proper byte-pointer + runtime-cmp peer) is **2× SLOWER than das_qsort_r** — we beat libc qsort by a clean margin.

## Source pointers

- libc++ sort: `/Library/Developer/CommandLineTools/SDKs/MacOSX26.4.sdk/usr/include/c++/v1/__algorithm/sort.h` lines 54/67/99/274/302/495/717
- libstdc++ sort: `/opt/homebrew/include/c++/13/bits/stl_algo.h` lines 85/1792/1854/1893/1918/1942 (Musser introsort, unchanged since SGI STL era)
- pdqsort reference: github.com/orlp/pdqsort
- Block partition paper: Edelkamp & Weiß, "BlockQuicksort", ESA 2016 (arXiv:1604.06697)
- libc++ landing commit: [D93923](https://reviews.llvm.org/D93923) — design rationale lives in review comments
- Our impl for comparison: `src/builtin/das_qsort_r.h:159-220` (das_qsort_r) + `src/builtin/das_qsort_r.h:` (das_sort\<T\>, added 2026-05-17)

## Questions
- why is libc++ std::sort 2.5× faster than our das_qsort_r pdqsort-lite on workhorse types at N=100K?
