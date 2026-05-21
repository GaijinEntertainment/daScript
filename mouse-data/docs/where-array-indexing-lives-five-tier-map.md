---
slug: where-array-indexing-lives-five-tier-map
title: Where in daslang's source does array indexing actually happen — when I touch one I need to touch them all?
created: 2026-05-21
last_verified: 2026-05-21
links: []
---

Daslang has **five independent copies** of array-indexing logic, one per execution / build tier. Any semantic change to bounds-checking, index-width handling, or diagnostics must sweep all five, or the bug fix will fail on whichever tier the test mode lands in.

## The five tiers

| Tier | File | Symbol | Notes |
|---|---|---|---|
| AOT C++ emission | `include/daScript/simulate/aot.h` | `TArray<TT>::operator()`, `safe_index`; `TDim::operator()`; `das_default_vector_index`, `das_ati`/`das_atci`, `das_vec_index`, `das_index<Matrix>` | What `[unused]` AOT-compiled C++ runs. `size` is `uint64_t` for `TArray` — real corruption window for >4GB arrays. Others have `uint32_t` size — diagnostic-only. |
| Interpreter — non-fused | `include/daScript/simulate/runtime_array.h:14` (`SimNode_ArrayAt::compute`), `simulate_nodes.h:835` (`SimNode_At` for dim), 852 (`SafeAt`), 868 (`AtR2V`), 961 (vector index macro) | The fallback path. Hit by Debug builds and many ARM permutations that don't enable all the fusion templates. | 
| Interpreter — fused | `src/simulate/simulate_fusion_at_array.cpp` (6 macro instantiations for `Op2ArrayAt`), `src/simulate/simulate_fusion_at.cpp` (6 for `Op2At` dim/vec) | What Release x64 typically picks for `var x = arr[i]`. Separate copy of the bounds check inside each macro definition. |
| JIT | `modules/dasLLVM/daslib/llvm_jit.das` — `check_range` (the shared bounds-check IR-emitter), called from `visitExprAt` at three sites (dim/array/vector) | LLVM-emitted IR. `normalize_int_widths` controls i32→i64 widening; SExt vs ZExt choice depends on whether the daslang index type is signed. |
| AST compile-time const-fold | `src/ast/ast_simulate.cpp:~1915` (inside `ExprAt`'s simulation case) | For `arr[<const>]` daslang folds the bounds check at compile time and reports `error[50300]: index out of range`. Read the const at its natural width (`tInt` → int32, `tInt64` → int64, `tUInt` → uint32, `tUInt64` → uint64); don't narrow. |

## How a typical pattern bug surfaces

The `uint32_t(neg_int)` wrap bug surfaced in PR-I (#2776) in **every one** of these tiers — same shape (`uint32_t idx = uint32_t(index); if (idx >= size) ...`), copy-pasted. The fix landed sequentially over three CI rounds:

1. Round 1 fixed aot.h, passed Release x64 + Linux.
2. Round 2 added fusion files + JIT + const-fold; passed Release ARM.
3. Round 3 caught Debug ARM, which dropped through the fused path into the **base** `SimNode_At` in `simulate_nodes.h` — yet another copy I'd missed.

Lesson: when you find one of these, grep for the format string (e.g. `"array index out of range"`, `"index out of range"`, `"vector index out of range"`) across `include/daScript/simulate/`, `src/simulate/`, `src/ast/`, `modules/dasLLVM/` to find ALL the copies. Don't trust that Release x64 passing means the fix is complete — Debug + ARM use different SimNode permutations.

## Why so many copies?

Each tier has different costs the inner-loop can't pay:
- AOT: per-template specialization, inlined into user C++ — the format strings literally get inlined into every call site.
- Interp non-fused: a generic `SimNode` virtual dispatch — runtime polymorphism, can't share code with the typed AOT path.
- Interp fused: peephole-fused `op2 = arr[idx]` patterns where the bounds check is inlined into the assignment node directly, removing one indirection.
- JIT: emits LLVM IR directly — has access to LLVM intrinsics (e.g. `ICmp UGE`) that the C++ versions can't use.
- AST const-fold: runs at compile time, has no Context, no runtime — different return contract (error to `program->error`, not `throw_error_at`).

Consolidation would mean a perf regression in at least one tier. The duplication is load-bearing.

## Questions
- Where in daslang's source does array indexing actually happen — when I touch one I need to touch them all?
