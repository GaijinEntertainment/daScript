---
slug: why-does-daslang-jit-produce-ir-that-resists-outer-loop-unrolling-and-tre-compared-to-clang-o3
title: Why does daslang JIT produce IR that resists outer-loop unrolling and TRE compared to clang -O3?
created: 2026-05-12
last_verified: 2026-05-12
links: []
---

**Concrete observations from /tmp/queen-ir/ analysis (queen.das vs queen_cpp.cpp, both LLVM 22.1.5 / -O3 / arm64):**

Clang fully unrolls the outer `for c in 0..N` loop in `addqueen` (8 inlined copies, constant `c` per copy) + TRE-eliminates the recursive tail call. The daslang JIT path keeps the loop rolled and the recursion as a `call+br`.

Even after PR #2634 (threading host CPU/features into the JIT TargetMachine), IR for `addqueen` is byte-identical to baseline — so TTI cost-model fallback isn't the bottleneck. The smoking gun: running `opt -passes='default<O3>'` (the same pipeline string) on the JIT-dumped post-opt IR DOES unroll and TRE-eliminate. So LLVM can do it; daslang's invocation of `LLVMRunPasses` is hitting a different code path than `opt`'s `runPassPipeline`. (Detail in [[why-does-llvmrunpasses-diverge-from-opt-runpasspipeline]].)

**Likely-suppressing things daslang emits that clang doesn't:**

1. **`[hint(alwaysinline)]` on the recursive `addqueen`** → function attr `alwaysinline hot`. The function can never actually inline (recursion blocks it), but the attr persists and suppresses loop unroll cost-model approval. **Verified**: dropping the hint allows the inner-loop unroll to fire (function body grows from 64→173 IR lines). Outer-loop unroll still doesn't fire.
2. **Duplicate GEP for `a[n]`** — daslang emits both `%dim_at8 = gep ... i64 (sext %n)` and `%dim_at = gep ... i64 (zext nneg %n)`. Same address, two GEPs, used in the n==0 fast-path block and the general block respectively. The optimizer doesn't CSE them.
3. **Dead `isplaceok` dllexport wrapper** — daslang emits the public ABI thunk (`<4 x float>` return, packed args) even when isplaceok is `private` to the module and inlined into addqueen. Consumes inliner budget; bloats the module.
4. **Custom memory effects** `memory(target_mem0: none, target_mem1: none)` on most JIT-emitted functions. Clang emits standard `memory(readwrite, inaccessiblemem: none)` only. Most LLVM passes don't understand `target_mem0/1` — likely conservative.
5. **Context arg threaded through every recursive call** — globals (`solutions++`) reach through `gep+load` from a context pointer arg, not a module global. Adds register pressure; harder to prove invariance.
6. **Missing metadata:** no `!tbaa` on loads/stores (only `!alias.scope`); no `!llvm.loop.mustprogress` on backedges; no `noundef` on integer params. Each one removes information the optimizer uses to prove transform safety.
7. **`isplaceok` inner-loop `i` is i32-phi with per-iteration zext to i64**, while clang phis directly in i64 and truncs once. Same number of conversions but different shape.

**Pursuit order (proven good first to investigate):**
1. **Path 3 — set `-mllvm` cl::opt globals at JIT init** via `LLVMParseCommandLineOptions` (`-unroll-threshold=N`, `-tail-call-elim-recursion-threshold=N`, etc.). daslang's prebuilt LLVM.dll may have different compiled-in cl::opt defaults than what opt uses post-parse.
2. **Drop `alwaysinline` for recursive functions in daslang codegen.** Filter the hint when the function is detected as recursive.
3. **De-duplicate the sext/zext GEPs** in daslang's `addqueen`-style code emit.
4. **Stop emitting public dllexport wrappers for `private` functions.**
5. Add `!llvm.loop.mustprogress` + `!tbaa` + `noundef` metadata at codegen time.

**Where this came from:** session 2026-05-12, PR #2634, comparison of `/tmp/queen-ir/queen_cpp.ll` vs `/tmp/queen-ir/queen-jit.ll`.

## Resolved 2026-05-12 (later same day)

The real driver was **bounds-check overhead**, not TRE/unroll or the original suspects. Adding per-function `[hint(unsafe_range_check)]` to both `isplaceok` and `addqueen` closes the gap and then some — JIT now beats clang -O3 on queen:

|                          | clang -O3 | JIT no hints | JIT both hints |
|---|---|---|---|
| `addqueen` body          | ~270 lines | 1022 | **376** (-64%) |
| Total IR                 | ~350 | 1524 | **777** |
| Recursive tail calls     | 14 | 14 | **8** (TRE absorbed 6 more) |
| `tailrecurse:` block     | No | Yes (1) | Yes (1) |
| Bounds-check thunks      | 0 | 12 | **3** (off hot path) |
| Per-run @ 100×           | 0.47 μs | 0.57 μs | **0.41 μs** |

The mandelbrot bench needed the same kind of unblocker on a different axis — adding `math::length` as an LLVM intrinsic (PR #2638) let LLVM see through `length(z) < 2.0` and DCE pure-result calls; with a result-sink in the harness JIT comes in at 0.466 ms vs C++ 0.626 ms on 10 runs.

**What was wrong about the original analysis:**
- (1) "alwaysinline blocks unroll" — `addqueen` in this codebase does not carry `alwaysinline`. The per-loop unroll hint (`[unroll_full]`) fires correctly without dropping anything.
- The "TRE doesn't fire" framing was wrong both ways: clang doesn't TRE either (14 tail calls, no `tailrecurse`); JIT actually TREs *more aggressively* than clang once bounds checks are out of the way (8 tail calls, 1 `tailrecurse`).
- LLVMRunPasses vs opt's runPassPipeline is real (see [[why-does-llvmrunpasses-behave-differently-from-opt-s-runpasspipeline-given-the-same-default-lt-o3-gt-pipeline-string]]) but is *not* what was blocking queen.

**Still correct from the original:**
- (2) duplicate sext/zext GEPs and (3) dead dllexport wrapper still appear in the JIT IR — they bloat the module but no longer matter for hot-path perf once bounds checks are gone.
- (5) context-arg threading still adds a register on every recursive call. Not load-bearing for the queen gap.
- (6) missing `!tbaa` / `noundef` metadata is real; cost is small enough that bounds-check removal dominates.

**Takeaway for "JIT slower than clang on tight algorithmic loops":** check whether array indexing on the hot path has implicit bounds checks first. `[hint(unsafe_range_check)]` is cheap to try and tends to be the dominant factor.

## Questions
- Why does daslang JIT produce IR that resists outer-loop unrolling and TRE compared to clang -O3?
- How do I close the JIT-vs-clang -O3 gap on tight algorithmic loops with array indexing?
