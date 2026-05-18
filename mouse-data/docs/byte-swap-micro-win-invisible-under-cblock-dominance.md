---
slug: byte-swap-micro-win-invisible-under-cblock-dominance
title: why does the byte_swap sized-dispatch micro-improvement (140× at W=4 in isolation) not show up in any real daslang sort benchmark?
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

A byte_swap variant that demolishes chunked-256 in isolation (`examples/sort/bench_byte_swap.cpp` — 140× faster at W=4, 30× at W=8, never slower) was invisible in every real-world sort benchmark when dropped into `src/builtin/das_qsort_r.h` (2026-05-17 bake-off, Phase 0.2 of PR #2706 follow-up).

**Measured deltas (chunked-256 → sized-dispatch):**

| benchmark | metric | before | after |
|---|---|---:|---:|
| `bench_sort_family.cpp` int32/100K sort | das/std ratio | 2.57× | 2.56× |
| `bench_sort_family.cpp` P32/100K sort | das/std ratio | 1.07× | 1.07× |
| `benchmarks/sort/sort.das` struct+cblock 100K | ns/op | 279 | 281 |
| `benchmarks/sort/nth_element.das` k=50000/100K | ns/op | 19 | 19 |
| `benchmarks/sort/heap_ops.das` bounded_topn/100K | ns/op | 27 | 27 |

Two distinct reasons, depending on the path:

### 1. C++ bench (bench_sort_family.cpp): constant propagation

The bench calls `das_qsort_r(a.data(), a.size(), sizeof(T), das_cmp<T>())`. `sizeof(T)` is a compile-time constant. With `-O3` and the `static inline` byte_swap, the compiler propagates `sizeof(T)` all the way down into the byte_swap body. The OLD chunked loop:

```cpp
while (width) {
    size_t chunk = sizeof(tmp) < width ? sizeof(tmp) : width;
    memcpy(tmp, a, chunk); memcpy(a, b, chunk); memcpy(b, tmp, chunk);
    a += chunk; b += chunk; width -= chunk;
}
```

at width=4 becomes: 1 iteration, chunk=4, 3× constant-size memcpy → identical machine code to the new switch arm. The micro-bench shows the cost because it calls byte_swap through a `Fn` function-pointer template parameter, defeating constant propagation. **The micro-bench measures dispatch cost; the real bench measures inlined-and-folded cost.**

### 2. daslang runtime path (any_cblock binding): callback dominance

`module_builtin_runtime_sort.cpp` calls `das_qsort_r(anyData, length, elementSize, lambda)` where elementSize IS runtime. Here the sized switch DOES win (5-10ns saved per swap vs the runtime-width loop). For 100K elements with ~1.7M swaps, that's ~10ms of theoretical savings.

But the lambda does `context->invokeEx(cmp, bargs, ...)` per comparison — script-block invocation with bargs marshaling. Measured cost per call dominates the swap by ~100×. The 27.9ms total for `sort_struct_by_key/100K` is overwhelmingly comparator-callback time; shaving 10ms off byte_swap would be visible but wasn't reproduced in repeated runs (281 vs 279 within noise).

### Implication for future perf work

- Micro-benchmark a primitive in isolation → not predictive of integrated impact when (a) callers know the parameter at compile time, or (b) some other operation in the same path dominates.
- If byte_swap is your suspected bottleneck, write a daslang benchmark that uses a CHEAP comparator (e.g. ints with `<`, going through the workhorse path, or a typed C++ harness that defeats constant propagation). The any_cblock path will never reveal byte_swap wins.
- The actual sort-vs-std gap on workhorse types (2.5× slower) is NOT byte_swap. Likely candidates: partition algorithm choice (libstdc++ may use block-quicksort partition), cmp callback inlining quality. Future bake-offs targeting that gap should NOT spend cycles on byte_swap variants.

### Decision

Boris (2026-05-17): keep the sized dispatch anyway. Net-neutral but never worse; cleaner code at small widths; header comment documents the bake-off so future maintainers don't redo the experiment. The 30-140× micro-win remains real for any future caller that defeats constant-prop (e.g. a direct C++ binding with runtime width).

### Source measurements

- Header: `src/builtin/das_qsort_r.h` byte_swap
- Micro-bench: `examples/sort/bench_byte_swap.cpp` (chunked256/chunked64/words64/sized/hybrid candidates at W∈{4,8,16,32,64,128,256,512})
- Integration: `examples/sort/bench_sort_family.cpp` + `benchmarks/sort/*.das`
- Related card: [[qsort-byte-swap-implementations-survey]] — production-impl survey from the bake-off research

## Questions
- why does the byte_swap sized-dispatch micro-improvement (140× at W=4 in isolation) not show up in any real daslang sort benchmark?
