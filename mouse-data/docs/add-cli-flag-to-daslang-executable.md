---
slug: add-cli-flag-to-daslang-executable
title: How do I add a new command-line flag to the daslang executable that maps to a CodeOfPolicies field?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

Touch four spots in `utils/daScript/main.cpp`:

**1. File-scope static** (near the other flag statics around lines 34-57):

```cpp
static bool logModuleCompileTime = false;
```

**2. Argv parser branch** in the main argv loop (`for ( int i=1; i < argc; ++i ) { if ( argv[i][0]=='-' ) { string cmd(argv[i]+1); ...`). Add an `else if` near related flags:

```cpp
} else if ( cmd=="log-compile-time" ) {
    logModuleCompileTime = true;
```

Single-dash style (`-log-compile-time`) for short or hyphenated flags. Double-dash (`--track-smart-ptr`) used for some longer ones. Existing convention is mostly single-dash — match the neighbors.

**3. Wire to policies in BOTH population sites:**

There are TWO CodeOfPolicies setup paths in `main.cpp`:
- `getPolicies()` near top of file — used by the AOT path (`das_aot_main`)
- `compile_and_run()` later — used by the normal run / JIT path

Both need the new line:

```cpp
policies.log_module_compile_time = logModuleCompileTime;
```

If you only update one, the flag silently does nothing in the other mode. (Easy to verify with `git grep -n "policies\.no_lint" utils/daScript/main.cpp` — every existing flag is set at both sites.)

**4. Help text** in `print_help()`:

```cpp
<< "    -log-compile-time  log detailed per-module compile-time breakdown (parse/infer/opt/macro/simulate)\n"
```

**Optional: also wire daslang-live** if the flag matters for the live-reload host. Its argv parser is `utils/daslang-live/main.cpp:648-680` and its `CodeOfPolicies` is constructed inside `compile_script()` ~line 171. Skip if the flag is daslang-only.

**Rebuild target:** `cmake --build build --config Release --target daslang -j 64` (≈30s incremental). The static lives in `daslang.cpp`'s translation unit — no header changes, no library re-link of dependents. If you also touched `module_builtin_rtti.cpp` for the underlying policy registration, the full target list grows (rebuild test_aot if you care about AOT path).

## Questions
- How do I add a new command-line flag to the daslang executable that maps to a CodeOfPolicies field?
