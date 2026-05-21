---
slug: uint32-cast-negative-int-wrap-bounds-check-bug-pattern
title: What's the uint32_t(neg_int) wrap bug pattern in daslang's bounds checks, and what does the fix look like?
created: 2026-05-21
last_verified: 2026-05-21
links: []
---

Anywhere daslang does `uint32_t idx = uint32_t(index); if (idx >= size) ...` for a signed int32 index, there are **two** latent bugs:

## Corruption when size > UINT32_MAX

For arrays/tables whose `size` field is `uint64_t` (post-PR-A: `Array`, `Table`):

```cpp
int32_t index = -2;
uint32_t idx = uint32_t(-2);   // = 0xFFFFFFFE = 4294967294
if (idx >= size)               // 4294967294 >= 5'000'000'000 -> FALSE
    throw_error(...);          // doesn't fire
return data[index];            // data[-2] — silent OOB read/write
```

The bounds check passes silently, the code uses the *signed* `index` for pointer arithmetic, and you get an off-by-N-before-start access into adjacent heap. Reproduces only when the array actually exceeds 4 GB elements (so the bug needs `DASLANG_HUGE_HEAP_TESTS=1` + `options persistent_heap = true` to surface in tests).

## Diagnostic-only for size < UINT32_MAX

For all the smaller collections (`TDim`, vector, matrix, std::vector binding):

```cpp
uint32_t idx = uint32_t(-2);   // = 4294967294
if (idx >= size)               // 4294967294 >= 10 -> TRUE
    throw_error("index out of range, %u of %u", idx, size);
//                                   ^^^^^^^^^^^^^^^ prints 4294967294 instead of -2
```

Bounds check fires correctly (no corruption), but the error message shows the unsigned wrap value instead of the original `-2`. Every developer who passes a negative index thinks the runtime is confused.

## Fix shape (both bugs, same edit)

```cpp
if ( index < 0 || uint*_t(index) >= size )
    throw_error("... %d of %llu", index, (unsigned long long)size);
```

- Detect `<0` explicitly **before** the unsigned cast.
- Cast width: `uint64_t(index)` if `size` is uint64_t (TArray); `uint32_t(index)` if `size` is uint32_t (dim/vector/matrix).
- Format: `%d` for signed int slot, `%lld` for int64, `%u` / `%llu` for unsigned size — **never** mix `%u` against an `int` template parameter (variadic UB, will trip `-Wformat` on stricter builds).

## Where it lives

Five tiers in daslang, all carry copies of this pattern — see [[where-array-indexing-lives-five-tier-map]]. The grep that surfaces them all is the format string itself: `"index out of range"` across `include/daScript/simulate/`, `src/simulate/`, `src/ast/`, `modules/dasLLVM/`. Don't fix one and stop — every copy is a separate code path that some test mode (Debug, ARM, sanitizer, JIT) will reach.

For the JIT version, the same bug appears as `LLVMBuildZExt` widening i32→i64; signed indices need `LLVMBuildSExt` instead — see [[jit-check-range-sext-vs-zext-signed-index-widening]].

## Test pattern

Two SUBCASE shapes from `tests-cpp/small/test_aot_int_narrowing.cpp`:

**Corruption test** — no real allocation needed:
```cpp
TArray<uint8_t> arr;
uint8_t fake_buf[32] = {};
arr.data = (char*)(fake_buf + 16);     // mid-stride so neg index doesn't underflow page
arr.size = 5'000'000'000ULL;            // > UINT32_MAX
auto * p = TArray<uint8_t>::safe_index(&arr, -2, nullptr);
CHECK(p == nullptr);  // master: returns non-null (bug); fix: nullptr
```

**Diagnostic test** — small array, real `Context`:
```cpp
Context ctx(1024);
TArray<int32_t> arr;
int32_t buf[10] = {};
arr.data = (char*)buf; arr.size = 10; arr.capacity = 10;
bool ok = ctx.runWithCatch([&](){ (void)arr(int32_t(-2), &ctx); });
CHECK_FALSE(ok);
CHECK(strstr(ctx.getException(), "-2") != nullptr);
CHECK(strstr(ctx.getException(), "4294967294") == nullptr);
```

## Questions
- What's the uint32_t(neg_int) wrap bug pattern in daslang's bounds checks, and what does the fix look like?
