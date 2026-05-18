---
slug: standalone-example-no-daslang-link
title: how do I add a fully standalone benchmark or example under /examples that does not link the daslang runtime (header-only consumer)?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

# Standalone example under /examples (no daslang runtime link)

The conventional pattern in /examples (crash, pathTracer, etc.) is to register the example in the **root** `CMakeLists.txt` via `include(examples/<name>/CMakeLists.txt)` (around line 1132). That style **requires** linking `libDaScriptDyn`, runs as part of the main DAS.sln build, and gets rebuilt with everything else.

If your example only consumes a header (e.g. `src/builtin/das_qsort_r.h` — the smoothsort + introselect + heap-on-byte-buffer templates), **don't register it in the root CMakeLists**. Make it fully standalone instead. Build it on its own without rebuilding the daslang world.

## Layout

```
examples/<name>/
  CMakeLists.txt           # standalone — no parent include
  bench_<thing>.cpp        # includes "<header_name>.h" only
```

## CMakeLists.txt template

```cmake
cmake_minimum_required(VERSION 3.16)
project(example_<name> CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

add_executable(example_<name> bench_<thing>.cpp)
target_include_directories(example_<name> PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/builtin)

if(MSVC)
    target_compile_options(example_<name> PRIVATE /O2 /W4)
else()
    target_compile_options(example_<name> PRIVATE -O3 -Wall -Wextra)
endif()
```

## Build + run

```sh
cmake -S examples/<name> -B build/example_<name> -DCMAKE_BUILD_TYPE=Release
cmake --build build/example_<name> -j 64
./build/example_<name>/example_<name>
```

Build time: a few seconds. No daslang dependency.

## When to use

- Header-only consumers — your example needs `das_qsort_r.h` or another self-contained header but no `Context`, `Program`, `Module`, etc.
- One-off perf comparisons / regression baselines that should NOT slow down the main daslang build by being part of it.
- Demos of standalone components that should remain compilable without the full daslang toolchain.

## When NOT to use

If your example needs to run a daslang script, create a `Module`, or call into the simulate/runtime — use the **registered** pattern instead (root `CMakeLists.txt` include + `TARGET_LINK_LIBRARIES libDaScriptDyn`). See `examples/crash/CMakeLists.txt` for the canonical registered example.

## Concrete reference

`examples/sort/` (added 2026-05-17 in PR #2706 follow-up branch) — standalone perf benchmark comparing std::sort/std::partial_sort/std::nth_element/std::make_heap+pop_heap against the das_*_r templates in das_qsort_r.h, no daslang link, ~16s end-to-end on M-series Mac.

## Questions
- how do I add a fully standalone benchmark or example under /examples that does not link the daslang runtime (header-only consumer)?
