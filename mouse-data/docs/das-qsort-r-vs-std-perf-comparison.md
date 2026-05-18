---
slug: das-qsort-r-vs-std-perf-comparison
title: how do das_partial_sort_r / das_nth_element_r / das_make_heap_r perform compared to std::* equivalents, and what's the real-world cost of the custom impl?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

# das_*_r templates vs std::* — measured perf comparison

Source: `examples/sort/bench_sort_family.cpp` (standalone, runs in ~16s on M-series Mac). N ∈ {1K, 10K, 100K}, element sizes {int32, int64, 32B struct, 128B struct}.

## Headline numbers (das ÷ std ratio, worst case across sizes)

| op | das vs std | reading |
|---|---|---|
| `das_nth_element_r` | 0.95–1.85× | **Match** — beats std at N=100K |
| `das_make_heap_r` + drain | 1.6–2.8× slower | Acceptable overhead |
| `das_qsort_r` (smoothsort) | 2.3–5.7× slower | Pre-existing (Anton Yudintsev's musl port). Smoothsort vs std's introsort. Known cost. |
| **`das_partial_sort_r`** | **2–18× slower** | **Real implementation bug** (see below) |

## Why partial_sort is so slow

`das_partial_sort_r` is implemented as `das_nth_element_r(N) + das_qsort_r(first N)` — O(N) + O(topn·log·topn). Looks correct on paper, BUT:

`std::partial_sort` uses a **heap-of-size-topn** approach: scan all N elements, maintain a top-N max-heap, output sorted. O(N · log topn). When topn ≪ N (e.g. topn=10, N=100K) this **destroys** nth-element-then-sort because nth_element still pays O(N) with a big constant.

Concrete: at N=100K, topn=10 — std::partial_sort 39μs vs das_partial_sort_r 730μs = **18× gap**.

The same asymmetry shows in daslib LINQ benchmarks:
- `m3_topn_array` (calls das_partial_sort_r): 442 ns/op at M=100K, top-N=10
- `m3_topn_iter` (does heap-of-N manually via push_heap loop): 83 ns/op same params
- 5× perf difference, same algorithmic cause

## Why custom impl exists at all

`std::sort` / `std::partial_sort` / `std::nth_element` template on **iterator type**. They cannot operate on opaque `void*` data with runtime-known `width`. The daslang any-cblock path (user-defined struct types going through `addExtern<...any_cblock>` bindings) needs byte-pointer algorithms — that's exactly what `das_*_r` templates provide.

So the custom impl IS justified for the any-cblock path. The typed paths (workhorse types) in `module_builtin_runtime_sort.cpp` already specialize to `std::*` directly via the `STAMP_NUMERIC_OPS` / `STAMP_VECTOR_OPS` macros.

## Fix candidates (out of scope for Phase 0, planned follow-up)

1. **Rewrite `das_partial_sort_r` to heap-of-topn** — closes the 18× gap. ~20 LOC change, in the Phase-0-added section of `das_qsort_r.h` (NOT Anton's smoothsort).
2. **`das_make_heap_r` Floyd construction** — should match std but ~2× slower; revisit if profiling points here.
3. **`das_qsort_r` smoothsort** — pre-existing; out of scope to replace.

## When this matters

- `top_n_by(arr, N, key)` in `daslib/linq` — currently uses `das_partial_sort_r` under the hood for the array overload. The iterator overload uses heap-of-N manually and is much faster.
- BufferTopN emit mode in `linq_fold` (planned PR B) — wants the fast heap-of-N path for `[where][select]* |> order_by |> take(N)` fusion.

Run the bench yourself: `cmake -S examples/sort -B build/example_sort_bench && cmake --build build/example_sort_bench -j && ./build/example_sort_bench/example_sort_bench`.

## Questions
- how do das_partial_sort_r / das_nth_element_r / das_make_heap_r perform compared to std::* equivalents, and what's the real-world cost of the custom impl?
