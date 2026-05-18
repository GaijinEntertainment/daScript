---
slug: what-daslib-operations-exist-for-partial-sort-nth-element-heap-ops-and-top-n-selection
title: What daslib operations exist for partial-sort, nth-element, heap ops, and top-N selection?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

Phase 0 of the linq_fold project (PR landing 2026-05-17) added the `<algorithm>` sort-family bindings. Operations now available:

**Sort-family (`require daslib/sort_boost`)** — array form, with and without custom comparator block:
- `partial_sort(arr, n[, cmp])` — sorts only the first N elements ascending. `O(M log N)` via `std::partial_sort` (typed) or `das_partial_sort_r` (any-cblock path for user structs).
- `nth_element(arr, n[, cmp])` — places the kth-smallest at position k; left ≤ kth ≤ right. Sub-O(M) overall.
- `make_heap(arr[, cmp])` / `push_heap(arr[, cmp])` / `pop_heap(arr[, cmp])` — binary max-heap on contiguous storage. `push_heap` assumes the new element was just appended; `pop_heap` moves max to last slot for caller to drop.

**Top-N selection (`require daslib/linq`)** — array + iterator sources:
- `top_n(src, n)` — N smallest by `<`.
- `top_n_by(src, n, key)` — N smallest by `key(element)`.
- Array source uses `partial_sort` under the hood (O(M log N), single allocation).
- Iterator source uses a bounded heap of size N during the scan (O(M log N), max N+1 elements resident).

**Dispatcher macros (`require daslib/sort_boost`)** — mirror `qsort` for the full surface:
- `qpartial_sort(value, n, block)`, `qnth_element(value, n, block)`, `qmake_heap(value, block)`, `qpush_heap(value, block)`, `qpop_heap(value, block)`. Each dispatches to the named user-facing function for `array<T>` / dim, or wraps in `temp_array(...)` for handled vector types.

**C++ binding shape** — all 5 ops follow the existing `__builtin_sort` template registered across 19 workhorse types (numeric + vector). Generic user-struct path goes through `das_qsort_r.h` byte-pointer templates (introselect for partial_sort/nth_element; standard sift-up/sift-down for heap ops).

**Out of Phase 0:** dim (`[]`) overloads are not provided for the daslib wrappers — use array form or call `__builtin_*` directly. Buffer-required emit modes in linq_fold (BufferTopN, BufferDistinct, etc.) that consume these primitives land in subsequent PRs (Phase 3+).

**Critical files:**
- `daslib/sort_boost.das:60-` — user-facing wrappers + q-prefix macros
- `daslib/linq.das:460-510` — `top_n` / `top_n_by` family
- `src/builtin/das_qsort_r.h:233-` — algorithm templates
- `include/daScript/simulate/aot.h:3390-` — cblock template surface
- `src/builtin/module_builtin_runtime_sort.cpp` — bindings + any-cblock wrappers

Project plan: `~/.claude/plans/merry-dazzling-curry.md`. Project context: `benchmarks/sql/LINQ.md`.

## Questions
- What daslib operations exist for partial-sort, nth-element, heap ops, and top-N selection?
