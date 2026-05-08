# daslang Changelog

## 0.6.2 (May 2026)

### New Features

#### daSQLite: Typed SQLite Query Layer Expansion (#2481, #2485, #2487, #2489, #2492, #2496, #2507, #2509, #2511, #2518, #2524, #2528, #2534, #2551, #2553, #2561, #2563, #2564, #2566, #2568, #2575, #2581, #2592, #2595, #2603)

`modules/dasSQLITE` now extends `daslib/linq` with SQL-backed queries, letting familiar linq-style transforms compile down to typed SQLite operations. This release then broadens that foundation with a large tutorial-driven expansion across the rest of the SQLite layer.

- **Insert + query macros** — typed insert flows, raw `query` / `try_query` helpers, much deeper `_sql(...)` analysis with multi-quantifier (multi-Q) lowering, and a parity audit comparing in-memory linq vs. SQL emission
- **Read-side operators** — `distinct`, `take`, `skip`, ordering, aggregates, grouping, the full join family (`_left_join`, `_inner_join`, `_right_join`, `_full_outer_join`, `_cross_join`), set operations, multi-source queries, and `_in` / `_not_in` against captured collections via SQLite `json_each`
- **Write-side operations** — typed update/delete flows, transaction helpers, UPSERT with schema annotations for foreign keys, indexes, defaults, and computed columns, plus user-defined SQL functions via `[sql_function]` (auto-registered and visible inside `_sql` chains)
- **Custom storage types** — custom adapters plus `@sql_json` and `@sql_blob` field support, JSON-path descent inside `_sql`, and column metadata introspection
- **Schema introspection and migrations** — `[sql_table(schema_from=...)]` + `check_schema` validate live database schema against the daslang struct at compile time; `daslib/sqlite_migrate` ships versioned `[sql_migration]` blocks with typed ALTER macros (`add_column` / `drop_column` / `rename_column`) and full-table rebuilds via `[struct_convert]` + `convert_and_rename` + `[sql_table(legacy=true)]` for non-trivial schema changes
- **Operational SQLite features** — SQL fragment building, `ATTACH DATABASE`, FTS5 (`[sql_fts5]` + `text_match`), and pre-migration utilities

#### Style Lint and Unified Linting (#2386, #2390, #2391, #2417, #2441, #2516, #2517, #2533, #2538)

New compile-time linting now covers daslang style in addition to the existing correctness and performance passes.

- **`daslib/style_lint`** — detects non-idiomatic gen2 patterns such as unnecessary block pipes, declaration-then-assignment, array literal construction via repeated `push`, and long comment blocks
- **Unified lint flow** — shared warning collection, `nolint` suppression, comment-hygiene rules, and codebase-wide cleanup to drive warnings back down
- **Noise reduction** — one-line style enforcement was relaxed where it produced more friction than signal

#### Duplicate Detection Pipeline: `detect-dupe` and `find-dupe` (#2491, #2493, #2497, #2502, #2508, #2510, #2522)

New utility tooling helps find and triage duplicate daslang functions across large codebases.

- **`utils/detect-dupe`** — scans files, canonicalizes functions, and clusters exact and fuzzy duplicates with corpus export/import support
- **Pattern filtering** — suppresses boilerplate-heavy false positives and handles generics more cleanly
- **AI-assisted triage** — `utils/find-dupe` consumes detect-dupe reports and asks Claude to partition clusters into real duplicates vs false positives, with JSON and Markdown output
- **MCP integration** — export, duplicate detection, and judgment workflows are exposed to AI tooling

#### `daslib/clargs`: Declarative CLI Parsing (#2454, #2482, #2525)

A new command-line parsing module standardizes how daslang tools read flags.

- **Long + short flags** — generated `--flag` and `-f` aliases, help text, required-flag handling, and array/bool parsing
- **Execution-mode awareness** — `get_user_args()` handles both interpreter `--` invocation and standalone executables correctly
- **Tool migration** — utilities can share one parser instead of hand-rolling `get_command_line_arguments()` logic

#### Memory Leak Tracking and Diagnostics (#2436, #2448, #2449, #2453, #2462)

daslang now has a much clearer leak-debugging story for both language-side and C++-side allocations.

- **Leak-tracker mode** — `--das-profiler --das-profiler-leaks` records live allocations with captured daslang call stacks
- **Cheat-sheet documentation** — unified guidance explains GC leaks, heap reports, smart-pointer tracking, jobque leaks, and handle-registry dumps
- **Profiler and heap tooling** — better reporting and supporting leak-audit work across the runtime

#### daspkg: Project Bundling for Distribution (#2579, #2584, #2588, #2590)

`daspkg release` produces a redistributable bundle of a daslang project — a directory containing the standalone exe, every transitively-required `.shared_module` dylib, every asset matching the project's release globs, and every transitive dep package's release assets. The bundle runs on a machine with no daslang installed.

- **`daspkg release` command** — `release()` hook in `.das_package` declares `release_main`, `release_name`, `release_include` / `release_exclude` globs, and force-included shared modules
- **Exe-relative shared-module resolution** — `daslang -exe` resolves shared modules in three tiers (exe directory → `das_root` → absolute path), making relocated bundles portable
- **`-list-shared-modules`** — auxiliary flag walks `program_for_each_module` and writes a JSON manifest of every dylib the program touches; `daspkg release` consumes it for auto-detection
- **Project-local + relocated-bundle support** — `get_this_module_dir`, the JIT runtime, and the daspkg resolver all honor project-local layouts, so a moved or copied bundle continues to find its modules
- **Native shared deps** — packages can ship platform-specific dylibs alongside their `.das` sources via `release_include_dll`; dasPUGIXML is enabled by default for Info.plist generation on macOS

### Improvements

#### Strudel Audio Engine: Second Wave (#2403, #2423, #2425, #2426, #2427, #2428, #2429, #2430, #2435, #2440, #2457, #2464, #2519)

The new Strudel engine from `0.6.1` received a broad follow-up pass across synthesis, playback, docs, and memory behavior.

- Better synth and drum voices, more SF2 support, unique per-event HRTF binaural 3D positioning, offline/WAV rendering workflows, and further live/demo polish
- Reduced memory usage and multiple leak fixes in the threaded player, visualizer, and shutdown path
- Expanded documentation and examples for the newer audio stack

#### Compiler, AOT, and JIT Work (#2396, #2402, #2405, #2406, #2416, #2422, #2433, #2442, #2445, #2458, #2461, #2480, #2499, #2500, #2460, #2554, #2565, #2569, #2585, #2594, #2596, #2601, #2604, #2605)

A substantial runtime/compiler pass improved code generation, packaging flexibility, and test coverage.

- Continued push toward more daslang-authored AOT logic and cleaner AOT/JIT boundaries
- Better standalone-exe behavior, transitive JIT type resolution, prologue sizing, and function registration
- More build configurations (`RelWithDebInfo`, static PIC), more daslib AOT coverage, and platform-specific AOT fixes
- **Tuple-strict typer** (#2565) — tuple field names are now part of the type, so `tuple<a:int;b:int>` and `tuple<x:int;y:int>` no longer collide and unnamed tuples remain distinct from named ones
- **`Option<T>` / `Result<T,E>` via `[template_tuple]`** (#2601) — both types are now structural named-tuples generated through a generic mechanism, replacing bespoke handling
- **Call-site block arrow body** (#2554) — `def f(...) : T => expr` shorthand parses alongside the existing block form
- **Error reporting audit** (#2596) — diagnostics retagged for consistency, with `Program::deduplicateErrors` suppressing repeated identical errors from the same site
- **`super` walks past empty intermediates** (#2594) — `super` now skips empty intermediate base classes when looking for a method
- **Block-form global variable annotations** (#2604) — `@field` annotations now propagate to globals declared inside `variable { ... }` blocks
- **`cbind` prefixes** (#2585) — generated bindings can be namespaced with a per-include prefix
- **Compiler refactor** (#2460, #2569, #2605) — `Program` and `Context` extracted from `simulate` / `ast` into their own translation units; serialization unified with eden / daNetGame; large-file split for build-time and editor-friendliness

#### GC / AST / Ownership Cleanup (#2400, #2404, #2407, #2409, #2410, #2411, #2415, #2420, #2421, #2470, #2506)

Core compiler internals continued the transition away from older `smart_ptr`-style AST ownership, with the biggest user-facing result being that macros became much easier to write.

- `TypeDecl`, `Expression`, annotations, and related AST paths were moved deeper into `gc_node`-style ownership
- Macro, visitor, binding, and validator code no longer has to fight as many `smart_ptr`-era ownership patterns and pointer conversions
- Tracker stability improved when GC is active, reducing false crashes during debugging

#### Runtime Libraries and Infrastructure (#2393, #2398, #2399, #2412, #2413, #2414, #2431, #2443, #2451, #2455, #2466, #2467, #2474, #2488, #2494, #2495, #2512, #2514, #2536, #2539, #2555, #2572, #2576, #2577)

A broad utility pass landed across libraries, docs, and developer workflow.

- **dasPEG** — more tests, docs, CI coverage, and standalone/LLVM fixes
- **Core libraries** — validated numeric conversions in `daslib/strings_convert`, continued `Option` / `Result` work including non-copyable support, and C API completeness fixes; `daslib/clargs` now returns `Result` from parsing for cleaner error handling
- **Runtime data structures** — new in-tree `das_hash_map` backend to avoid empty-construction allocations and behave better in thread-local usage
- **Path-aware glob in `daslib/fio`** (#2576, #2577) — `match_glob`, `glob`, `glob_filtered`, plus `expand_glob` / `parse_file_list` promoted from the MCP utility into the stdlib; byte-loops collapsed to `find` / `replace` with a new `skills/strings.md` rollup
- **Math: common numeric functions** (#2555) — `sinh`, `cosh`, `tanh`, `asinh`, `acosh`, `atanh`, `log10`, `log1p`, `expm1`, `cbrt`, `trunc`, `hypot`, `fmod`, `remainder` added to the math module
- **das-fmt vendored in-tree** — `utils/das-fmt/` now holds a local copy of the formatter; CI runs against it instead of an external clone
- **Tooling/docs** — filesystem guidance, handle-registry tutorial work, class-boost coverage, integer-returning `main()` for tools, compilation progress reporting, and assorted AST/class-method polish

#### Build, CI, and Web (#2578, #2600, #2602)

- **Tutorials build in CI** (#2578) — every tutorial now compiles in CI as part of the regular check run
- **Web target reuses the main `CMakeLists`** (#2600) — `daslang-web` picks up tests and tracks the same build configuration as the native targets, removing the duplicated CMake graph
- **MCP: parse-aware C++ source-search tools** (#2602) — `cpp_find_symbol`, `cpp_grep_usage`, `cpp_outline`, `cpp_goto_definition` join the existing daslang-side tools, with a configurable search root and git-signature staleness detection

### Bug Fixes

- **Build, tooling, and CI fixes** (#2385, #2394, #2395, #2424, #2447, #2521, #2537, #2591) — package `.gitignore` handling, PEG standalone/LLVM issues, `require` fixes, `dasbind` fixes, glob dependency tracking, documentation/error-position corrections, and daslang plugin: completion no longer silently bails when an external binding (e.g. OpenGL, GLSL preprocessor) is in scope
- **Runtime/compiler correctness** (#2387, #2392, #2486, #2520, #2526, #2531, #2587, #2593) — JIT global-function arguments, handled-type property write propagation, function-lookup cache pointer reuse, `runWithCatch` state cleanup, ASAN/diagnostic follow-up, fix #2583 (standalone-exe shutdown crash), and fix #2582 (top-level `let`-init now correctly registers builtins)
- **Fusion engine** — TSan-safe `v_ldu` via `DAS_LDU_WORKHORSE`, `call1` / `call2` `loadSize` stamped from `fnPtr->debugInfo`, and a TSan suppression for the over-read in fusion call/return shells
- **Language/runtime edge cases** (#2444, #2446, #2463, #2468) — `finally` loop rework, clearer inference failure on bad calls, GCC reference shadow fixes, and strict-weak-ordering cleanup
- **Graphics and platform fixes** (#2540) — OpenGL/package integration cleanup after the daspkg migration work
- **Architecture-specific** — skipped `#pragma float_control` on `__e2k__` to silence warnings on the Elbrus toolchain

### Examples

- **Asteroids** (#2552, #2556) — full asteroid-shooter game with waves, audio, and powerups; later migrated to DECS for the entity model with a polish pass on visuals and powerup VFX
- **Pacman** (#2589) — classic arcade game added to the `examples/` lineup
- **River Run** (#2560, #2562, #2567) — top-down river shooter built across three rounds: initial gameplay (river splits, combat, VFX, audio, HUD, collisions), then scenery / bonus / engine sound / tuning, and finally shadows, pickup burst, rich music, 3D HUD, and a juice pass

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

#### daStrudel: Pattern Music Engine (#2362)

A TidalCycles-inspired pattern music engine with SF2 SoundFont support, MIDI playback, and a redesigned two-mode player architecture. Patterns are pure functions from time spans to events — composable, lazy, and side-effect-free.

**SF2 SoundFont support:**
- Load General MIDI soundfonts (`strudel_load_sf2`) — 128 GM presets, multi-layered instrument zones
- Per-voice reverb and chorus sends from SF2 generator parameters
- Exclusive class handling (hi-hat choke) and strudel cut groups for voice management
- Note-off scheduling with sample-accurate timing
- SF2 state persistence across live reloads via Archive serialization (~140MB GM font survives hot reload)

**Two-mode player architecture:**
- **Main-thread mode** — `strudel_create_channel()` + `strudel_tick()` from your render loop. Direct access to per-track PCM and pianoroll hap data. No channels, no lockboxes, no `void*` casts.
- **Threaded mode** — `strudel_init(fn)` spawns a dedicated audio thread. `strudel_play()` runs the tick loop with buffer-level sleep. Channel-based command dispatch for live pattern changes.

**Visualizer combinators:**
- `pianoroll("name")`, `spectrum()`, `scope()`, `vectorscope()`, `drums()` — pass-through pattern combinators using a rolling `g_vis_index`. Each registers what visualization the track wants; `strudel_add_track()` bumps the index.
- `pianoroll()` wraps the pattern to capture haps during evaluation into a direct array (same thread, no channels). Other combinators are pure pass-throughs that mark track preferences.
- `vis_name()`, `vis_color()` — set display metadata per track.
- Panel names displayed only when explicitly specified.

**MIDI player** (`strudel_midi_player.das`):
- Load and play `.mid` files through SF2 soundfont rendering
- Per-channel gain, pan, looping, audio visualization channels
- Integrates with the visualizer via the same audio analysis pipeline

**Examples** (6 demos in `examples/daStrudel/`):
- `player_demo` — minimal threaded playback (load samples, play pattern, shutdown)
- `synth_demo` — 100% synthesized, no sample files needed (drums + bass + lead)
- `sf2_demo` — SF2 lo-fi chill beat (Am-F-C-G with GM percussion, piano, bass, strings)
- `piano_demo` — "Ode to Joy" with pitched piano samples via `seq()` bar notation
- `strudel_live` / `strudel_sf2_live` — live-reload with REST commands (play/stop/fade tracks by name)
- `strudel_visualizer` — OpenGL visualizer with 5-track pianoroll, spectrum, drums, scope, vectorscope panels. Main-thread mode with direct data access.

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
