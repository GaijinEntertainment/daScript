---
slug: what-s-the-right-anti-dce-pattern-for-a-c-microbenchmark-inner-loop-so-the-optimizer-can-t-elide-it
title: What's the right anti-DCE pattern for a C++ microbenchmark inner loop so the optimizer can't elide it?
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

**Pattern:** after the timer stops, take a `volatile` observation of post-loop state. The volatile load forces the compiler to materialize the value, which transitively forces the timed loop body to have an observed effect.

```cpp
auto t0 = clk::now();
for (int it = 0; it < iters; it++) {
    for (size_t p = 0; p < pairs; p++) {
        fn(data.data() + (2*p) * width, data.data() + (2*p + 1) * width, width);
    }
}
auto t1 = clk::now();
// Anti-DCE: force a volatile observation of post-loop data so the optimizer
// can't prove the loop is dead.
volatile unsigned char observed = data.back();
(void)observed;
```

**Anti-pattern (broken):** comparing against a value the type can never hold.
```cpp
if (data.back() == 0xDEAD) std::fprintf(stderr, "%d", int(data[0]));
```
`data.back()` is `unsigned char` (0–255) and can never equal `0xDEAD` (57005). The optimizer constant-folds the condition to `false`, eliminates the if-branch, then has no post-loop observation of `data`, so the entire timed loop can be DCE'd. Benchmark reports unrealistically low times.

**Other valid patterns:**
- `asm volatile("" : : "r"(data.data()) : "memory")` — compiler fence + memory clobber (Google benchmark's `DoNotOptimize`/`ClobberMemory` use this)
- `std::cerr << uint64_t(data[0]) << "\n";` — actual I/O, but adds latency
- The `volatile unsigned char` load is the cheapest portable form

Reference: PR #2707 round 3 — Copilot caught the `0xDEAD` bug in `examples/sort/bench_byte_swap.cpp`. Commit `a11a3d79e`.</body>
<slug>anti-dce-bench-pattern-volatile-post-loop-observation</slug>
</invoke>

## Questions
- What's the right anti-DCE pattern for a C++ microbenchmark inner loop so the optimizer can't elide it?
