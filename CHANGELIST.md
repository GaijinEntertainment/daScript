# daslang Changelog

## 0.6.1 (April 2026)

### New Features

#### VSCode Extension: dastest Integration (#2279)

The daslang VSCode extension now integrates `dastest` directly into the editor — run tests and benchmarks from inline buttons, see results in the Test Explorer, and get per-test pass/fail feedback without leaving the IDE.

- **Inline run buttons** — click to run individual tests or benchmarks right from the editor
- **Test Explorer integration** — test results appear in VS Code's standard Test Explorer panel
- **JIT & isolated mode settings** — `dascript.jit` and `dascript.isolatedMode` toggles in VS Code Settings, read fresh on each test run (no reload needed)
- **Live logging** — real-time stdout/stderr streaming to a "daslang Tests" output channel (tests were previously silent until completion)
- **Cross-platform** compiler auto-detection (`daslang` on macOS/Linux, `daslang.exe` on Windows)
- **Better error messages** — distinguishes "compiler not found" vs "dastest not found"
- **dastest isolated mode fixes** — forward `--bench` flag to subprocesses, emit full per-test JSON results

#### daspkg: Package Manager for daslang (#2234, #2236, #2238, #2240, #2247, #2248, #2254, #2260, #2323)

A full-featured package manager for daslang, built entirely in daslang itself. Packages are installed from git repositories or a central index, with dependencies resolved transitively and tracked in a lockfile. Native C/C++ modules are built automatically via CMake.

**Core commands:**
- `install` — from local path, git URL, or package index name. Supports version pinning (`@v1.0`), semver constraints (`>=1.0,<2.0`), and branch pinning
- `update` / `upgrade` — re-resolve to latest compatible version with rollback on failure
- `remove` — clean removal with dependency cleanup
- `build` — CMake-based native module compilation with `doctor` for toolchain validation
- `check` / `list` — verify integrity, show installed packages
- `introduce` / `withdraw` — publish/unpublish packages to the community index (`borisbat/daspkg-index`)

**`.das_package` manifests** — executable daslang scripts (not static JSON) that define package metadata, SDK-aware build logic, version resolution, and dependencies via `require_package()`. This means packages can have conditional build steps, platform-specific logic, and programmatic version checks — all in daslang.

**`daslib/daspkg.das` API module** — provides `package_name`, `download_tag`, `require_package`, `cmake_build`, and other functions available inside `.das_package` scripts.

**Global module install** (#2323) — `daspkg install --global` installs packages to `das_root/modules/`, shared across all projects. Local installs auto-use compatible global versions (zero clone, zero build). Version mismatches error with suggestions. C++ runtime shadow detection warns when a module exists both locally and globally — local always wins.

**Ecosystem migration** — modules previously shipped as git submodules (`dasImgui`, `dasImguiNodeEditor`, `dasMinfft`, `dasTelegram`) have been extracted into standalone daspkg packages with their own repos and `.das_package` manifests. The corresponding `DAS_*_DISABLED` CMake flags have been removed. Install via `daspkg install dasImgui` etc.

**Example packages** in `examples/daspkg/`:
- `graphics/` — ImGui example (depends on `dasImgui`)
- `node-editor/` — imgui-node-editor (transitive dep on `dasImgui`)
- `minfft/` — FFT via `das-minfft` index package
- `daspkg-claude/` — Telegram bot using Claude API with compile-check sandboxing

**Infrastructure:**
- Ordered lockfile preserving installation order for sequential upgrades
- Dependency cycle detection with clear error messages
- Logging to `modules/.daspkg.log` with `--verbose` for debug output
- `force_rmdir` — fixes failure to remove git repos with read-only objects on Windows
- `.daspkg_standalone` marker — prevents auto-discovery of standalone-build modules
- 151 unit tests + 70 integration tests

#### daslang-live: Live-Reloading Application Host (#2286, #2290, #2291, #2293, #2295, #2299, #2302)

New `daslang-live.exe` — a thin C++ host with compile → init → update → shutdown lifecycle, automatic GC, pause-on-exception, and hot reload on file changes. The same script runs with both `daslang.exe` (shipped) and `daslang-live.exe` (development).

- **`modules/dasLiveHost/`** — C++ dynamic module providing lifecycle API (`request_exit`, `is_reload`, `get_dt`, `get_fps`, persistent byte store, etc.)
- **`live/` module namespace** — `live/live_commands`, `live/live_api`, `live/live_watch`, `live/decs_live`, `live/glfw_live`, `live/opengl_live`
- **`@live` variable macro** — annotate globals with `@live` for automatic serialization across live reloads, replacing ~60 lines of manual `[before_reload]`/`[after_reload]` boilerplate
- **REST API** — `/status`, `/error`, `/reload`, `/pause`, `/unpause`, `/command`, `/shutdown` endpoints. Unknown endpoints return JSON help listing all endpoints with curl examples
- **Error guard** — `/command`, `/pause`, `/unpause` return HTTP 503 with compilation/exception error text
- **Exception handling** — runtime exceptions during `init()`, `update()`, `[after_reload]`, or live commands enter error state, report via `/error`, and recover on next reload
- **Single-instance enforcement** via named mutex (Windows) / flock (Unix)
- **Dependency file watching** — all compiled `.das` files tracked (not just the main script)
- **`-cwd` flag** — changes working directory to script's folder
- **4x MSAA** enabled by default in `glfw_live` window creation
- **38 unit tests** in `tests/live_host/`

#### AST Pattern Matching: `daslib/ast_match` (#2330, #2332, #2333)

New module for pattern matching on compiled AST using the same tag system as `qmacro` (`$e`, `$v`, `$i`, `$t`, `$c`, `$f`, `$b`, `$a`) but in reverse — tags extract values instead of substituting them.

- Three macros: `qmatch` (single expression), `qmatch_block` (block with wildcards), `qmatch_function` (compiled function)
- Wildcards: `_wildcard()` (0+), `_wildcard1()` (1+), `_optional()` (0–1), `_any()` (exactly 1)
- Strict argument matching with `$a(rest)` for flexible tail
- Constant constructor matching (folded and unfolded forms)
- Comprehension matching: array `[for...]`, table `{for...}`, generator `[iterator for...]`
- Block/lambda/local-function discrimination via `mmFlags`
- Post-inference local function matching via `qm_convert_local_function`
- 305 tests across 17 test files, full language reference documentation

#### Eliminate Builtin `.das` Compilation — Compiler-Free Standalone Executables (#2304)

All built-in `.das` wrappers (fio, rtti, network, math helpers, etc.) have been moved out of `compileBuiltinModule` and into regular `daslib/` modules. This is a foundational architectural change with three major consequences:

- **Faster compilation** — ~10ms faster on static builds (35ms → 25ms for hello world), ~7ms on dynamic. The compiler no longer parses and type-checks built-in `.das` code on every startup.
- **Standalone executables without the compiler** — `libDaScript` can now be split into a runtime-only library and a compiler library. Standalone executables (AOT-compiled, JIT-compiled, or via LLVM) no longer need to carry the full compiler just because built-in modules had `.das` wrappers. This enables significantly smaller binaries for deployed applications.
- **Minimal runtime environments** — possible to run a daScript context with only C++-bound modules, no `.das` parsing at all. Opens the door for embedded/constrained targets.

Requires updating bare `require fio` / `require rtti` to `require daslib/fio` / `require daslib/rtti_core` in user code. The `rtti` module was renamed to `rtti_core` to avoid conflicts.

#### Performance Lint Module: `daslib/perf_lint` (#2315, #2317, #2318)

Compile-time AST visitor that detects common performance anti-patterns:

- **PERF001**: string `+=` in loop (O(n^2) allocations)
- **PERF002**: `character_at` with loop variable index (O(n) per call; use `peek_data` instead)
- **PERF003**: `character_at` anywhere (informational; suggests `first_character` for index 0)
- **PERF004**: string interpolation reassignment in loop (O(n^2) allocations)
- **PERF005**: `length(string)` in while condition (strlen each iteration)
- **PERF006**: `push`/`push_clone`/`emplace` in for loop without `reserve()`
- **PERF007**: unnecessary `string(das_string)` in comparison (das_string supports `==`/`!=` directly)
- **PERF008**: unnecessary `get_ptr()` for `is`/`as` type checks (smart_ptr supports them directly)
- **PERF009**: redundant move-init variable immediately returned (use `return <- expr` directly)

Expression chain walking, field path tracking, closure-aware detection, `inferStack` reporting. Two usage modes: `require daslib/perf_lint` for auto-lint, or standalone `utils/perf_lint/main.das` for batch checking. Full codebase sweep applied all fixes.

#### Pointer Safe-At Indexing (#2280)

- `a?[index]` for pointer types — returns null if `a` is null, otherwise `a + index` (pointer arithmetic). Requires `unsafe`. Supports int, uint, int64, uint64 indices.
- **Bug fix**: bounded safe-at types (array, fixed array, vector) accepted int64/uint64 indices but `SimNode_SafeAt` truncated to 32-bit. Fixed by restricting bounded types to int/uint only.
- Implemented across interpreter, AOT, and JIT.

#### C API Type Introspection (#2334)

- Expose `TypeInfo`, `StructInfo`, `EnumInfo`, `FuncInfo` as opaque handles in `daScriptC.h`
- ~40 new C functions for querying struct layouts (field names, types, offsets), enum values, function signatures
- `das_base_type` enum with `static_assert` guards against C/C++ drift
- Tutorial 11: struct layout inspection, nested structs, engine-side allocation using discovered offsets

#### New Builtins

- **`get_module_file_name(name)`** (#2294) — returns source file path of a module. Enables module-relative asset resolution.
- **`get_key(tab, value_ref)`** (#2322) — O(1) table value-to-key lookup via pointer arithmetic on parallel key/value arrays
- **`first_character(string)`** (#2315) — O(1) first character access (both `string` and `das_string` overloads)
- **`with_das_string(block)`** (#2315) — creates a temporary `das_string` for block scope
- **`system()`** (#2293) — fire-and-forget process launch in `fio` module
- **`daslib/command_line`** (#2328) — `get_das_exe()` returns path to the daslang interpreter

### Improvements

#### Compilation & Runtime

- **Faster CondFolding** (#2309) — optimization phase handles multiple transformations per pass instead of one, reducing total optimization passes.
- **Optimized `character_at`** (#2315) — C++ implementation scans only to index instead of full `strlen`.
- **Fix access flags for array/table lock/unlock** (#2310) — split into const/non-const overloads, fixing incorrect variable access flag inference.
- **One-liner and table lints are warnings** (#2284) — demoted from errors until next major release, controllable via `CodeOfPolicies`.

#### File I/O

- **Explicit error reporting for fio** (#2282) — removed all try-catch from C++17 filesystem functions, added Go-style `string& error` out-parameters to 16 functions. 35 new tests.
- **Fix `stat`/`fstat` side effects** (#2319) — changed from `modifyExternal` to `modifyArgumentAndExternal` for correct const inference.

#### MCP Server (AI Tool Integration)

- **7 live tools** (#2293) — `live_launch`, `live_status`, `live_error`, `live_reload`, `live_pause`, `live_command`, `live_shutdown`
- **`lint` tool** (#2319) — AI-assisted code quality checking (includes both paranoid lint and perf_lint in one pass) with duplicate message deduplication
- **`shutdown` tool** (#2293) — shuts down MCP server for code change pickup
- **`failures_only` param for `run_test`** (#2328)
- **Fix ast-grep detection** (#2326) — fallback to `.venv/Scripts/sg` / `.venv/bin/sg`
- **Fix standalone exe subprocess hang** (#2328) — `get_das_exe()` prevents launching another MCP server instead of `daslang.exe`
- **Unique temp files** (#2328) — `ref_time_ticks()` in names to avoid collisions

#### stb Libraries

- **stbtt glyph shape API** (#2291) — expose `stbtt_GetCodepointShape`, vertex struct, vertex type constants
- **Safe stb_truetype API** (#2312) — two-layer safe API: low-level `stbtt_*` wrappers confining `unsafe`, high-level `Font` methods with zero `unsafe`. 70 tests.

#### DECS (Entity Component System)

- **Bulk entity creation** (#2290) — `create_entities`T` API bypasses ComponentMap, resolves component indices once, writes directly into archetype storage. ~10x faster (174ns vs 1772ns for 1000 entities under JIT).
- **Multi-dimensional fixed array serialization** (#2295) — `daslib/archive.das` supports 2D–6D fixed arrays

#### Documentation & Install

- **daslang-live documentation** (#2314) — full RST page covering lifecycle, API, modules, REST API, CLI, examples
- **CMake install overhaul** (#2313) — complete examples, module install fixes, tutorial install rules, utility install
- **Updated CLI help** (#2324) — all command-line switches documented for both `daslang` and `daslang-live`
- **Lint sweep** (#2317, #2321) — all perf_lint and lint warnings fixed across daslib/, modules/, examples/, tutorials/

#### CI & Infrastructure

- **Custom org runners** (#2307, #2308) — `ubuntu-latest-fat`, `ubuntu-24.04-arm-fat`, `windows-latest-fat`, macOS xlarge
- **Correct CI timeouts** (#2283) — increased to 900s for dastest, 1h for isolated mode
- **`daslang_static` in CI** (#2292) — Release-only testing
- **das-fmt Release-only** (#2292) — run once on Linux only
- **Extended CI checks** (#2331) — additional test configurations

### Bug Fixes

- **AOT `das_null_coalescing`** (#2286) — template used wrong type source (`defaultValue->type` instead of expression result type). Fixed in both C++ and daScript AOT generators.
- **ASAN support on MSVC** (#2301) — fix `NO_ASAN_INLINE` macro (was missing `__declspec(no_sanitize_address)` for MSVC), forward ASAN flags to libhv ExternalProject.
- **ModuleGroup use-after-free** (#2301) — `ModuleGroup` was stack-local in `compile_script()` but outlived by `Program`/`Context`. Moved into `CompileResult`.
- **Fuzzer bugs** (#2311) — clone semantics won't pipe to ascend/new; `getBaseSizeOf64`/`getAlignOfField` correctly crash on type-macros.
- **Fusion ASAN false positives** (#2306) — `NO_ASAN_INLINE` on fusion call/return methods using `v_ldu` on sub-16-byte data.
- **Cross-platform path hardcoding** (#2289) — replace `bin/Release/daslang.exe` with `get_command_line_arguments()[0]` in tests.
- **`array_mark_locked`** (#2289) — use proper API instead of manual `Array` field setup in dasAudio/dasSQLITE.
- **JIT standalone executables** (#2327) — various fixes for JIT-compiled standalone exe generation.
- **JIT support for all nodes** (#2281) — additional node types supported in JIT backend.
- **Handled const string type emission** (#2287) — manually emit type for handled const string.
- **MCP shared module caching** — `ignore_shared_modules` prevents stale structs across `compile_check` calls.

### Examples

- **Arcanoid** (#2286, #2290, #2303) — full Breakout game with 3D rendering, DECS entities, procedural audio, power-ups, planar shadows, particles, ball trail, menu/pause states, 11 live commands. Rounded bricks, shield powerup, pickup VFX, sticky paddle.
- **Sequence board game** (#2295, #2296, #2297, #2300) — complete Sequence implementation with 2-4 players, card placement, jack logic, win detection. Three bots (random, greedy, lookahead), ELO tournament runner, parallel evolution strategy for weight tuning, 176 tests.

### Utilities

- **`utils/migrate-tables/`** (#2298) — two-pass tool transforming `tab.insert(key, tab.get_value(key) + val)` patterns into `tab[key] += val` compound assignments. 22 ast-grep rules + Python script for compound expressions.
