---
slug: where-are-the-cross-compiler-bit-scan-and-popcount-helpers-in-daslang-s-c-headers
title: Where are the cross-compiler bit-scan and popcount helpers in daslang's C++ headers?
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

**`include/daScript/misc/platform.h:149-211`** — daslang's cross-compiler primitives. On MSVC they wrap `_BitScanForward64` / `_BitScanReverse64` / `__popcnt64` (with explicit branches for x86-32 and ARM64); on GCC/Clang they are `#define`s to the `__builtin_*` intrinsics.

Helpers:
- `das_clz(uint32_t)` / `das_clz64(uint64_t)` — leading-zero count (same return as `__builtin_clzll`)
- `das_ctz(uint32_t)` / `das_ctz64(uint64_t)` — trailing-zero count (same return as `__builtin_ctzll`)
- `das_popcount(uint32_t)` / `das_popcount64(uint64_t)` — population count

**Rule:** before writing any `#ifdef _MSC_VER` shim for `__builtin_ctzll` / `__builtin_clzll` / `__popcnt64`, **search `include/daScript/misc/platform.h` first**. This was the cause of PR #2707 CI failure round 2 — I wrote `__builtin_ctzll` directly in a new public header and MSVC choked. Fix was `#include "daScript/misc/platform.h"` + replace `__builtin_ctzll(x)` → `int(das_ctz64(x))` and `63 - __builtin_clzll(x)` → `int(63 - das_clz64(x))`.

**Same energy for SIMD/vector intrinsics:** check `include/vecmath/dag_vecMath_*.h` for hand-tuned wrappers before rolling your own.

Reference: PR #2707, commit `e69210c69`. Boris's standing rule (`feedback_cross_compiler_helpers_first.md`): always search before writing `#ifdef _MSC_VER` shims.</body>
<slug>cross-compiler-bitscan-popcount-helpers-daScript-misc-platform-h</slug>
</invoke>

## Questions
- Where are the cross-compiler bit-scan and popcount helpers in daslang's C++ headers?
