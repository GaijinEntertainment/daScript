# Build & Debug

Repo-internal build, run, and crash-diagnostic reference. Read this when running `cmake`, debugging a `daslang` crash, flipping module flags, or tracking down an AOT hash mismatch. Day-to-day editing doesn't require it - only operations that touch the build system or runtime exit codes.

The repo builds on **Windows, Linux, macOS, iOS, Android, and WASM** (CI runs the full matrix). Most commands below are platform-neutral. Where a platform detail differs, it's noted.

## Build & run

- **Build system:** CMake. On Windows the default generator is MSVC (Visual Studio 2022); on Linux/macOS use Ninja or Make; on Android/iOS/WASM see the platform docs.
- **Generate**:
  - Windows: `cmake -B build` (default generator is MSVC; creates `build/DAS.sln`)
  - Linux/macOS: `cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release` (or omit `-G Ninja` to use the system default)
- **Build:** `cmake --build build --config Release -j 64 -- /nodeReuse:false` (the `/nodeReuse:false` is MSBuild-only; drop it on Ninja/Make)
- **Compiler binary:**
  - Windows MSVC (multi-config): `bin/Release/daslang.exe`
  - Linux/macOS (single-config): `build/daslang`
- **Live-reload host:** `bin/Release/daslang-live.exe` (Windows) / `build/daslang-live` (Linux/macOS) - same script runs in both; see `utils/daslang-live/main.cpp`
- **Run a script:** `<daslang-binary> path/to/script.das`
- **Compile-only check:** `<daslang-binary> -compile-only path/to/script.das` - compiles without simulation or execution, useful for syntax/type checking without needing a window or GL context. Use `-dry-run` to also simulate (but not execute).
- **Run tests:** `<daslang-binary> dastest/dastest.das -- --test path/to/test.das`
- **AOT tests (full):** `cmake --build build --config Release --target test_aot` then `<test_aot-binary> -use-aot dastest/dastest.das -- --use-aot --test tests`. The full binary is `EXCLUDE_FROM_ALL` (~1080 AOT TUs); the default build only makes `test_aot_subset` (tests/language - the per-PR CI gate; `--target run_tests_aot_subset` sweeps it)
- **IMPORTANT:** When adding a new test directory under `tests/`, register it in `tests/aot/CMakeLists.txt` for AOT compilation. See `skills/internal/aot_testing.md` for the step-by-step pattern. The nightly CI + `preflight --full` run ALL tests with AOT enabled - unregistered test directories cause `error[50101]: AOT link failed` there (per-PR CI won't catch it)

This skill uses `bin/Release/daslang.exe` in examples below (the dominant local-dev case); substitute the right path on other platforms.

## Build timing

- **Builds are slow** - clean builds take **15-25 minutes**, incremental builds take **2-10 minutes** depending on what changed
- **Always use `timeout: 0`** (no timeout) when running `cmake --build` commands. A build that hasn't finished is not stuck or broken, it's just compiling. A build killed mid-compile is worse than slow - it leaves truncated `.obj` files (see Build failures by symptom below)
- **Do not assume build failure** from lack of output - MSVC and most generators are silent during compilation and only print when there are warnings/errors or when it finishes
- For incremental builds after editing a single `.cpp` file, expect ~2-5 minutes. For changes touching headers, expect longer

### Shared OpenSSL cache (Windows/MSVC) - the big first-build lever

On **Windows/MSVC**, the dominant cost of a *fresh* build dir is **dasHV building OpenSSL 3.5.1 from source** (~15 min of the clean build): `modules/dasHV/CMakeLists.txt` defaults `OPENSSL_ROOT_DIR` to `${CMAKE_BINARY_DIR}/openssl` (per build dir), so every new `build*/` and every worktree rebuilds it. The configure prints which path it took - `dasHV: ... building 3.5.1 from source` vs `dasHV: using prebuilt OpenSSL <version> (include: <dir>) - skipping source build`.

To build OpenSSL **once and reuse it everywhere**, point it at a shared cache (it's an `IF(NOT OpenSSL_FOUND)` gate - first build populates the dir, all later builds `find_package` -> skip):
- **Env (covers CLI + VS Code):** set `DASLANG_OPENSSL_DIR=%LOCALAPPDATA%/daslang/openssl` once in your environment.
- **Per-configure:** `cmake ... -DOPENSSL_ROOT_DIR=<shared-dir>`.

The default (no env, no flag) stays per-build-dir, so **CI is unchanged** - its lanes use vcpkg (`vcpkg install openssl` + the vcpkg toolchain) or a cached from-source `build-clangcl/openssl` for the same effect. Linux/macOS use the system OpenSSL (brew/apt/mingw sysroot), so this is MSVC-only.

## Build failures by symptom

Each entry is symptom -> cause -> fix. Never `rm -rf build` as a first move - every one of these has a targeted repair.

- **MSVC `C1001` / `LNK1000` during "Generating code"** (Windows) - link-time codegen (LTCG) choking on a **stale incremental database** (`.ipdb`/`.iobj`) in a long-lived `build/`, *not* a code bug. The line `"no usable IPDB/IOBJ from previous compilation ... fall back to full compilation"` on a clean retry confirms it. Fix: clean-rebuild just the offending target - `cmake --build build --target <name> --clean-first` - rather than nuking `build/`. Commonly triggered when a config change (e.g. flipping a `DAS_*_DISABLED` flag) forces a recompile of an object whose stale LTCG state no longer matches.
- **MSVC `LNK1103` "debugging information corrupt" / `LNK1136`** (Windows) - a build killed mid-compile (tool timeout, manual interrupt) left a **truncated `.obj`** whose fresh timestamp incremental builds trust forever. Fix: `powershell utils/internal/build-heal/heal.ps1 [-Target <name>]` - it runs the build, deletes the `.obj` files named by corrupt-obj link errors, and retries.
- **MSVC `LNK1104` on `libDaScriptDyn*.dll`** (Windows) - the DLL is loaded by a live process: the MCP server host, a stray `daslang`/`daslang-live`. Shut the MCP server down (`mcp__daslang__shutdown`) and kill strays **by path, never by image name** - details in `skills/internal/make_pr.md`.
- **daslang crashes on every script run - even after a full clean rebuild - while `tests-cpp-small` stays green**: a **stale external-module `shared_module`** (built in its own `modules/<name>/_build` against pre-change headers) is ABI-drifted; shared-module auto-discovery loads it into every compile, `tests-cpp` never scans them. Find it by mtime (`ls -lt modules/*/[a-zA-Z]*.shared_module` - the odd one out predates the header change) and rebuild that module's `_build`. Under cdb the giveaway is the `ModLoad` of the stale `.shared_module` right before the AV (see Native crash triage below).

## Debugging runtime crashes

- **Always check the exit code** after running `daslang` - a crash may produce no output, looking like a silent success
  - Windows / PowerShell: `$LASTEXITCODE`
  - Bash / zsh (Linux, macOS, Git Bash): `$?`
- Exit code `-1073741819` (`0xC0000005`) on Windows = **Access Violation** - native crash (segfault). On Linux/macOS the equivalent shows up as exit code `139` (SIGSEGV) or similar. The bug is in C++ bindings or smart-pointer misuse
- **Don't truncate output** with `head`/`tail` - daslang stack traces and `DAS_GC_BREAK_ON_ID` traces are easily clipped. Capture full output, then `grep` if needed
- **`options log_infer_passes`** - append at the end of a failing `.das` file to dump per-pass infer activity (which generics got reified, when finalize ran, where lookups missed). Smaller and more targeted than `options log` for template/generic reification bugs. `options log` stays the right tool when you need the final program text

### Native crash triage (Windows)

When daslang itself AVs, get the faulting instruction before theorizing:

```text
"C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\cdb.exe" -o -c ".lines; g; .exr -1; ln @rip; kc 20; q" bin\Release\daslang.exe <args>
```

- `ln @rip` names the nearest symbol and source line; `kc` gives the call stack. Release LTCG inlines aggressively, so a thin stack (`daslang!main` only) means the attribution is useless - go to the faulting instruction instead.
- Watch the `ModLoad` lines just before the AV - a `.shared_module` loading right before the crash points at ABI drift (see Build failures by symptom above).
- **Heap corruption** (`RtlFreeHeap`/`RtlpHeapHandleError` in the stack - the crash site is the *victim*, not the culprit): arm full PageHeap so the corrupting WRITE faults instead - `gflags /p /enable daslang.exe /full` (elevated; gflags sits beside cdb), re-run under cdb, and **always disarm after** with `gflags /p /disable daslang.exe /full` - the IFEO entry is machine-wide, keyed by image name, and taxes every daslang run ~4x on memory until removed.

### JIT crash debugging

Use both debug rails for a long-running JIT repro:

```text
bin/Release/daslang.exe -jit -jit-stack path/to/main.das -- --jit-debug
```

- `-jit-stack` is a host flag placed before the script. It retains a logical daslang frame for every generated function and block so `Context::getStackWalk()` has useful JIT state.
- `--jit-debug` is an LLVM JIT option passed after the script separator. It emits CodeView/PDB debug information, generated function names, and `.das` file/line locations. On Windows the PDB lands beside the content-addressed DLL under `.jitted_scripts/`.
- **JIT DLL cache staleness** - the cache key folds the codegen version, per-function AOT hashes, loop hints, and fast-math, so `LLVM_JIT_CODEGEN_VERSION` bumps and tune-sidecar edits re-key automatically. Blunt fallback when behavior looks stale anyway: delete `.jitted_scripts/`. Always wipe it before declaring a JIT regression.
- JIT crash bundles should preserve the matching `.dll`, `.pdb`, `.map`, and retained `.o` together. The content hash changes when debug info is toggled, so artifacts from a non-debug cache entry do not decode a debug run.
- Windows Release C/C++ builds use `/Z7` plus linker `/DEBUG /OPT:REF /OPT:ICF`, producing side PDBs without changing optimized code. Keep the PDB that matches every shipped exe/DLL when diagnosing native runtime frames.

The implementation and remaining debugger roadmap are in `modules/dasLLVM/DEBUGGING.md`.

## Fast JIT iteration

Dev-tier rails that cut the edit-compile-run loop. Never benchmark through `--jit-opt-level=0`; a `-jit` benchmark that must see one-unit codegen passes `--jit-split-modules=0`, and a records run measures a released `-exe`, which always is one unit.

- **`--jit-opt-level=0`** (after the script separator) disables the LLVM IR pass pipeline AND drives the DLL path's codegen-side target machine (fast isel) - a true end-to-end O0 for `-jit` runs, several-fold faster to build. Exe (`-exe`) and AOT-object emission still pin codegen level 3 deliberately (shipped artifacts). At level 0 the injected tune-policy default becomes `fallback` (winners are raced under O3 codegen); `DAS_TUNE_POLICY` still overrides.
- **Split-module JIT is the default for the DLL path** (`--jit-split-modules=-1`; `=0` after the `--` separator selects the monolith, `options jit_split_modules = N` pins a script): codegen is partitioned per das-module - parallel emit makes the cold build several-fold faster, and the per-module obj cache (on by default under split; `--jit-obj-cache=0` disables) re-emits only from the FIRST CHANGED MODULE onward on a warm edit, and lets every program that shares a module reuse its partition (a test suite of one process per file compiles the engine once). **Invalidation is positional, so require order is the cache layout: place the module you are actively editing as LATE in the require chain as its dependencies allow** - a hot-edit module required early drags everything after it into every rebuild, one required last rebuilds nearly alone (canonical: the dasLLAMA umbrella requires the vulkan drivers last for exactly this reason). The cache holds ONE generation - reverting an edit re-emits from the reverted module on, same as the edit did. Split partitions lose cross-module inlining; `-exe` and AOT emission are always one unit, so the default touches `-jit` runs only.

## Front-end (AST) module cache

`daslang` and `daslang-live` cache the compiled AST module graph through the env-serializer rail, silently, at `.jitted_scripts/module_cache/<stem>-<hash>.dascache` relative to the cwd. The first run writes the file; later runs deserialize the post-infer modules instead of parsing them - roughly an order of magnitude on a large module graph, measured with an explicit `-module-cache`. Benchmarking a number that includes compile time? Pass `-no-module-cache`. The directory is capped at `DAS_MODULE_CACHE_LIMIT` megabytes (4096 by default; an engine root's record is ~200 MB): after each write the oldest records by mtime go, a record a run read counts as fresh, and an explicit `-module-cache` path is never pruned.

- **The hash keys the compile, not just the script:** the normalized script path; the host's command line up to `--` (`-v1syntax`, `-jit`, `-project` are different compiles; the script's own arguments after `--` are not); every `DAS*` environment variable (macros read the tune/JIT environment at compile time); and the running daslang binary's mtime and size, resolved through the OS rather than read off `argv[0]`. Same-named roots, a changed flag set, a changed environment and a rebuild each get their own file; a launch by bare name through `PATH` keys the same file as a launch by path.
- **Off:** `-no-module-cache` turns it off and beats an explicit `-module-cache <path>` on the same command line, so a tool that spawns `daslang` appends `-no-module-cache` as an override. Beside `-ser`/`-deser` the binary rejects the command line instead. It is off on its own under `-exe` (the exe is parsed as one compilation unit on purpose, so the optimizer sees the whole graph), `-compile-only` (preflight and lint spawn one daslang process per file, each on a different root, so no run is ever warm), `-documentation` (a one-shot generator - there is no second run for a cache to serve), `-use-aot` (an AOT-consuming run links the compiled functions against the AOT stubs by hash; it is an artifact run like `-exe`, and the link sees exactly the compile the stubs were generated from) and `--das-wait-debugger` (the serializer refuses a program that requires the debugger, so the cache would only print that refusal).
- **`-module-cache <path>`** (host flag, before the script) names the file explicitly, rides every mode including `-exe`, and prints the verdict: `deser: clean|partial|FALLBACK` and `ser: wrote <n> bytes` under `daslang`, `daslang-live: module cache ...` under `daslang-live`. It cannot be combined with `-ser`/`-deser`; the binary rejects that too.
- **Invalidation** is content, never a stat: on the sources, the size + hash of the bytes the compile's `FileAccess` serves; on the compile-time inputs a macro pins (`add_module_cache_dependency` - a file such as a tune sidecar or a config, read from the path itself, or a virtual input: `env:NAME` pins an environment variable's value, `arg:--flag` a command-line flag's occurrences; `daslib/build_const` pins every carrier it reads), the size + hash of that content. It is positional like the obj cache: an edited module recompiles itself and everything after it, and the cache rewrites itself. Content rather than mtime is why a byte-identical rewrite - a sidecar on exit, a `touch`, a checkout that restores the same bytes - does not churn the cache. A record also carries the `CodeOfPolicies` of the compile that wrote it and is never served to a compile with different policies (the per-record line says `compile policies changed`, the verdict `partial`, and the rewrite carries the new policies) - so an explicit `-module-cache` path shared between a plain run and a `-no-lint`, `-jit` or lint-tool compile rewrites itself on each switch rather than serving the other compile's modules. A served module's source lines come from the compile's `FileAccess` (file system, `.das_project` hook or in-memory access), never from the cache file.
- **`deser: partial`** - a record that cannot deserialize in a cold process (`modules/dasLLVM/bindings/llvm_func.das`, whose `[dasbind]` externs register into the dasbind builtin module during compile) reparses in place (~0.03s) without cutting the stream; so does a record written by a compile with different policies (above).
- **`-ser <path>` / `-deser <path>`** are the explicit write and read halves - the round-trip test instrument.
- **Composes with the split obj cache:** a warm `-jit` edit deserializes the unchanged prefix and re-emits only the tail partitions. The cache is one file you own - delete it when in doubt.

## Build configurations (module flags)

Optional modules are controlled by CMake flags (`DAS_*_DISABLED`). The active configuration lives in `.vscode/settings.json` under `cmake.configureSettings` (the "WIP" block is the active one; others are commented-out presets).

Key flags (defaults are MIXED - see the `option(DAS_*_DISABLED ...)` block near the top of CMakeLists.txt):
- Default `OFF` (module ENABLED by default): `DAS_HV_DISABLED` (dasHV - HTTP/WebSocket via libhv), `DAS_GLFW_DISABLED` (GLFW/OpenGL windowing), `DAS_PUGIXML_DISABLED` (XML), `DAS_AUDIO_DISABLED`, `DAS_STBIMAGE_DISABLED`, `DAS_STDDLG_DISABLED`
- Default `ON` (module DISABLED by default): `DAS_LLVM_DISABLED` (LLVM JIT), `DAS_CLANG_BIND_DISABLED` (Clang bindings), `DAS_SQLITE_DISABLED`
- dasImgui is in-tree (`modules/dasImgui`) with `DAS_IMGUI_DISABLED` (default `OFF` = enabled). It needs dasGlfw + dasClipboard - a derived gate in `modules/dasImgui/CMakeLists.txt` drops it when either is disabled. Its still-external dependents (dasImguiImplot, dasImguiNodeEditor) are cloned into `modules/` by consumers like the wasm playground (see `.github/workflows/wasmboy.yml`)

**To change modules:** edit the active `cmake.configureSettings` in `.vscode/settings.json`, then reconfigure. Or pass `-DFLAG=VALUE` to your `cmake -B build ...` command. VSCode CMake Tools will pick up settings changes automatically.

**Documentation generation** (`doc/reflections/das2rst.das`) requires `DAS_HV_DISABLED=OFF` and `DAS_PUGIXML_DISABLED=OFF` (both the default) because it documents all modules. If your local config disabled them, re-enable, rebuild `daslang`, run das2rst, then revert.

## AOT hash mismatches

When AOT fails with `error[50101]: AOT link failed`, the issue is a **semantic hash mismatch** between the generated C++ stubs and runtime. Each generated `.cpp` file has hash comments showing function hashes and dependency hashes. The runtime error also prints the same breakdown. Compare them to find the diverging function or dependency.

For the full debugging workflow, see `skills/internal/aot_hash_desync_debugging.md` (side-by-side SimNode dumps via `options log_nodes`/`log_nodes_aot_hash`, common shapes, C++ debug switches).

The AOT C++ emitter lives in **`daslib/aot_cpp.das`**, not in C++ (`include/daScript/ast/ast_aot_cpp.h` is the header alone). When codegen output diverges, edit `daslib/aot_cpp.das`.
