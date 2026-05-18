---
slug: das-throw-universal-panic-primitive-low-level-headers
title: what is the right primitive to fail/panic from a low-level daslang C++ header — don't use raw throw because embedded games disable exceptions
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

**Use `das::das_throw(const char* msg)`** — daslang's universal panic primitive. Declared in `include/daScript/das_config.h`, defined in `src/simulate/runtime_string.cpp`.

**Why NOT raw `throw`** in low-level headers:
- Games embedding daslang frequently disable C++ exceptions entirely (`-fno-exceptions` / EASTL no-exceptions mode). Raw `throw` won't compile.
- `daslang`'s OWN default is `DAS_ENABLE_EXCEPTIONS=0` (see `include/daScript/simulate/simulate.h:27-28`). Only Windows-no-LLVM and WASM set it to 1.
- EASTL has no `eastl::exception` or `eastl::out_of_range`, so even with exceptions enabled, `throw das::exception()` doesn't resolve in the EASTL config.

**What das_throw does:**
- `DAS_ENABLE_EXCEPTIONS=0` (default): setjmp/longjmp via thread-local `g_throwBuf`. If no try-catch is set up, calls `DAS_FATAL_ERROR("unhanded das_throw, %s\n", msg)`.
- `DAS_ENABLE_EXCEPTIONS=1`: `throw dasException(msg, LineInfo())` so existing `catch(dasException&)` handlers in `src/simulate/simulate_exceptions.cpp` pick it up.

**Constraint to know**: as of PR #2716, `das_throw` is declared unconditionally in `das_config.h` / `include_fmt.h` / `das_config_eastl/das_config.h`. Before that PR, the declaration was gated on `DAS_ENABLE_EXCEPTIONS=0`, so calling it from headers in WASM/Windows-no-LLVM builds would have failed at link time.

**FMT_THROW vs das_throw**: `FMT_THROW(x)` (from fmt library or daslang's override) takes an exception OBJECT and is conditionally `throw x` or `das::das_throw(x.what())`. das_throw takes a `const char*` directly — use this in low-level code where you don't have an exception object to construct.

**Pre-PR pattern that broke**: das_hash_map.h had `throw std::out_of_range("daslang_hash_map::at")` in `at()`. This compiled on macOS libc++ (transitive `<stdexcept>`) but failed on Linux libstdc++ (no transitive) and EASTL (no `eastl::out_of_range`). The fix in PR #2716: replace with `das::das_throw("daslang_hash_map::at")`.

## Questions
- what is the right primitive to fail/panic from a low-level daslang C++ header — don't use raw throw because embedded games disable exceptions
