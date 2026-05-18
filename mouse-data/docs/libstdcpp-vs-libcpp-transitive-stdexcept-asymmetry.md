---
slug: libstdcpp-vs-libcpp-transitive-stdexcept-asymmetry
title: why does my low-level daslang header compile on macOS but fail on Linux with "no member named X in namespace das" — transitive include differences
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

**Symptom**: A header that compiles fine on macOS (clang + libc++) fails on Linux (clang/gcc + libstdc++) with errors like:
```
error: no member named 'out_of_range' in namespace 'das'
```

**Cause**: Standard library implementations differ in what's pulled in transitively.
- **libc++** (macOS, Emscripten/WASM): `<vector>` / `<string>` / `<map>` typically pull `<stdexcept>` transitively, making `std::out_of_range`, `std::runtime_error` visible.
- **libstdc++** (Linux GCC default): does NOT pull `<stdexcept>` from those headers. You must `#include <stdexcept>` explicitly.

Same asymmetry applies to `<exception>`, `<new>` (placement new declarations), some `<type_traits>` features depending on version.

**In daslang specifically**: `include/das_hash_map/das_hash_map.h` was pulled via `das_config.h`'s `<vector>`, `<utility>`, etc. — but on Linux, those don't bring `<stdexcept>` along. PR #2716's first iteration removed the local `<stdexcept>` thinking it was redundant; CI immediately failed on the Linux jobs. macOS local build had said "all clean".

**Diagnosis tip**: when CI fails Linux-only on what looks like a namespace-resolution issue, suspect transitive-include differences. Check:
```bash
grep -rn "X" /path/to/libcxx-include/<vector>  # is X declared transitively in libc++?
grep -rn "X" /path/to/libstdcpp-include/vector # vs libstdc++?
```

**Fix options**:
1. Add the explicit `#include <stdexcept>` (or whatever) in the header that needs it.
2. Avoid the symbol entirely (PR #2716 chose this — switched from `std::out_of_range` to `das::das_throw`, which has no `<stdexcept>` dependency).

**Don't rely on "macOS local says it works"** for include hygiene questions. The CI matrix exists because the toolchains diverge in ways the lone-dev macOS build won't catch.

## Questions
- why does my low-level daslang header compile on macOS but fail on Linux with "no member named X in namespace das" — transitive include differences
