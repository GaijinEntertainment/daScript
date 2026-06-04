# daslang Changelog

## 0.6.3 (June 2026) — PRELIMINARY

> Preliminary changelog covering everything merged since 0.6.2 (#2618 – #2997). Subject to edit before release.

### New Features

#### LINQ Fusion Engine: `daslib/linq_fold` (#2687, #2689, #2691, #2696, #2697, #2712, #2714, #2721, #2723, #2724, #2725, #2727, #2728, #2729, #2730, #2732, #2733, #2738, #2741, #2750, #2760, #2765, #2766, #2769, #2770, #2772, #2775, #2784, #2785, #2806, #2809, #2822, #2824, #2833, #2834, #2837, #2839, #2841, #2848, #2851, #2852, #2857, #2861, #2862, #2865, #2866, #2875, #2878, #2881, #2883, #2885, #2886, #2887, #2888, #2891, #2893, #2895, #2896, #2899, #2901, #2903, #2913, #2914, #2915, #2917, #2919, #2924, #2926, #2935, #2936, #2939, #2941, #2943, #2945, #2946, #2947, #2948, #2950, #2952, #2955)

`_fold` is a new compile-time macro that fuses an entire LINQ chain into a single splice-emitted loop — no intermediate arrays, no per-stage iterator allocation. It became the universal fusion lane behind the whole `daslib/linq` surface, beating in-memory SQLite on headline benchmark shapes.

- **Operator coverage** — `where` / `select` / counter lanes; aggregates and early-exit terminators (`any` / `all` / `contains` / `first` / `count` / `long_count` / `last` / `single` / `element_at` / `aggregate`); `take` / `skip` / `take_while` / `skip_while`; the order/sort family, `reverse`, `distinct` / `distinct_by`, and `group_by` with `sum` / `min` / `max` / `first` / `average` / multi-reducer plus `_having`
- **Four sources, one engine** — array, `decs` (spliced into unrolled `for_each_archetype` loops, 10×+ over the eager bridge), XML (`from_xml_node`, with DOM-walk inlining + field pruning), and SQL (pure pass-through to `_sql`, gated on `_sql` being linked). Deferred/handle-buffering materialization keeps XML buffered reducers single-pass
- **N-ary `zip`** (#2737, #2738, #2741) — `zip` / `zip_to_array` extended from pairs to 4-through-8-way, array and iterator forms, each with an optional result selector; rides the shared int64 counter lane
- **Architecture** — migrated onto a unified `splice_patterns` pattern table + capability-method `SourceAdapter`s (G1→G3d), collapsing seven bespoke planner tables and the per-source terminator scaffolds into shared `linq_fold_common` / `_array` / `_decs` lanes

#### LINQ Query Syntax: C# `%linq!` (#2957, #2958, #2960, #2962, #2966, #2969, #2972, #2975, #2978, #2981, #2987, #2990, #2994, #2996)

`daslib/linq_das` adds a C#-style query expression that rewrites to a `_fold(...)` chain at compile time, built on the new **expression-level reader macros** (`%name! … %%`, #2957). Full clause-level parity shipped incrementally:

- `from … in`, repeatable `where` (#2996), `let` bindings (#2981), `select`
- `join … on … equals …` (#2969) and `join … into g` group-join / GroupJoin (#2994)
- multiple-`from` SelectMany, both uncorrelated (cross product, #2975) and correlated (flatten, #2978)
- `orderby` single-key (#2962) and multi-key composite (#2987); `group … by` (#2962); `into` group/select continuation (#2990)
- **Typed sources via `from_in`** — arrays, `decs`, XML, and SQL, with iterator output (#2960); SQL sources push `WHERE` / `GROUP BY` / `ORDER BY` down to SQLite

```das
let names <- %linq! from c in cars where c.price > 100 orderby c.name select c.name %%
```

#### 64-bit Arrays and Tables — the `longarr` arc (#2743, #2746, #2755, #2762, #2764, #2766, #2768, #2771, #2773, #2779, #2899)

`array<T>` / `table<K;V>` capacity, length, and index were widened to `int64` internally so collections can exceed `INT_MAX` / 4 GB.

- New `long_length` / `long_reserve` / `long_capacity` / `long_count` / `long_find_index` / `long_iter_range` shadow builtins, `_i64` AOT helpers, and `das_array_*_i64` / `das_table_*_i64` C-API entries
- `int | int64` disjunction-parameter dispatch (plus `typeinfo is_int` / `is_int64` / `is_float` / `is_double`) keeps the daslang surface single-overload while forking by base type; `builtin` / `array_boost` / `json_boost` / `strings_boost` / `decs` widened
- The 8.5k-test existing surface is preserved via shadow names rather than a hard widen; `MemoryModel` / heap byte counts and `alignMask` widened to 64-bit with 4 GB-cap guards

#### Sort Family: pdqsort, `<algorithm>`, and `stable_sort` (#2706, #2707, #2986)

- **`<algorithm>` bindings + `top_n`** (#2706, #2707) — new `sort_boost` API (`partial_sort`, `nth_element`, `make_heap` / `push_heap` / `pop_heap`, each with optional comparator) over a typed `das_sort<T>` block-partition pdqsort C++ binding, plus `top_n` / `top_n_by` selection in `daslib/linq`; array + iterator sources
- **`stable_sort`** (#2986) — new adaptive natural-run merge (timsort-lite, galloping merge): `das_stable_sort_r` (byte) and typed `das_stable_sort<T>`, with AOT support, tests, bench, and docs

#### dasOPENAI: OpenAI-compatible API Client (#2938, #2942, #2944)

New pure-daslang module over dasHV / json_boost — chat (streaming + tools + structured outputs), embeddings, models, completions, audio TTS/STT, moderations, images, and vision. Works against OpenAI, Ollama, OpenRouter, vLLM, and LM Studio. Streamed tool-call fragments are reassembled by index (#2944). Ships 12 examples, 7 mock-server-backed tutorials, and an RST reference.

#### New Libraries and Transports

- **`daslib/jsonrpc` + stdio live transport** (#2674, #2679) — unified JSON-RPC 2.0 library (request/response, line-dispatch, §6 batch) and a JSON-RPC stdio transport for daslang-live commands that needs no dasHV
- **`daslib/logger`** (#2908) — JSON-Lines structured logger; `logger_install_hook()` diverts `print` / `to_log` to a file (critical for stdio-transport processes like MCP). Adds C++ `iso8601_now()` and `popen_argv_pipe()` (bidirectional, shell-bypass subprocess pipes)
- **`daslib/toml` + `.lint_config`** (#2884) — TOML-1.0 parser (closed-table semantics); repo-level `.lint_config` (TOML) now configures lint, with STYLE005 off by default
- **`daslib/with_boost`** (#2880, #2889) — `with_` call macro binds one or more `arr[i]` / `tab[k]` element references inside a block under an automatic `lock`, so push/erase/resize panic instead of dangling; any arity, mixed array + table
- **LINQ over XML** (#2920) — `from_xml_node(root, type<Row>)` typed LINQ source over XML child elements in `dasPUGIXML` / `PUGIXML_boost`

#### C, Graphics, and Audio

- **C-API arrays & tables** (#2619, #2779) — `das_array_*` / `das_table_*` (with `_i64` completeness) build, lock, and borrow daslang collections from C; integration tutorial 14
- **APNG recording pipeline** (#2630, #2645, #2951) — streaming animated-PNG writer in dasStbImage (`stbi_apng_*`), `record_start` / `stop` / `status` live commands, an async PBO ring in dasOpenGL, and a synchronous frame-exact writer with fixed-delay capture
- **dasGLFW synthetic input** (#2630, #2633) — chain dispatcher plus synthetic mouse / keyboard / char input; `glfw_live` mouse and keyboard drivers (held-input release on stop)
- **dasAudio HRTF + simulated-3D mixing** (#2879) — `set_hrtf_budget(n)` routes the closest channels to HRTF slots and the rest to constant-power-pan / distance-attenuation 3D, with anti-flap margins, calibrated gain, and cross-context stats
- **dasStrudel widening + combinators** (#2995) — every scalar pattern modifier accepts `int | float | double`; exposes `set_X`, adds `freq` / `euclidRot` / `chop` / `slice` and `bd(3,8)` Euclidean mini-notation

### Improvements

#### Language and Compiler

- **Implicit integer-literal promotion** (#2876) — bare int literals now silently promote to any fitting numeric target (`int8/16/64`, `uint8/16/32/64`, `bitfield*`, `float`, `double`) at infer time across assignments, returns, var/struct/make-* inits and binary ops; range overflow is `error[30515]`, and `LINT011` warns on precision loss
- **`enum_trait` `operator !` / `bool()` for any enum** (#2825) — flag-op enums read like bitfields (`if (!(flags & MASK))`), width-safe across all backings; also folds const-foldable enum exprs at infer time
- **Lambdas are copyable** (#2731) — `=` / pass-by-value / `push` copy the pointer (alias, no `push_clone`); `delete lam` now requires `unsafe`, and the rule cascades to containers/structs holding a lambda
- **C++ keywords as identifiers** (#2735) — any C++ keyword that is not also a daslang reserved word can name vars, structs, fields, enums, and typedefs (`register`, `union`, `short`, …); C++-keyword function names compile via AOT mangling (#2655)
- **Unnamed → named tuple promotion** (#2642) — a positional tuple literal of bare variables is promoted to a named tuple when the names match the target fields
- **Optional `require`** (#2921) — `require ?guard target` loads `target` only when module `guard` is linked, else silently skips; pairs with `typeinfo builtin_module_exists`
- **One-liner block / EOF parsing** (#2925, #2651) — a single-line brace block no longer needs a trailing `;` before `}`, and a gen2 file whose final statement has no trailing newline now parses
- **`class` super-call discipline** (#2892, #2900) — CFG-aware lint requires exactly one `super(...)` on every control-flow path and rejects skipping an intermediate class; a chaining default ctor is synthesized for empty-derived classes only when safe (`options always_call_super` removed)
- **`uint8` / `uint16` / `uint64`-backed enum casts zero-extend** (#2739) — no longer sign-extend to negative values; fixed across interpreter, AOT, and JIT
- **Faster compile** (#2849, #2853) — hoisted per-module overload visibility + a lazy `findAlias` subtree cache (~19% faster), with the scratch bits excluded from semantic/AOT hashes
- **Clearer diagnostics** (#2989, #2650, #2626) — type-mismatch errors print short alias names with a single `where '<alias> = <def>'` clause (#2989, fixes #2571); assume-alias errors report at the use site (#2650); better for-in iterator-exhaustion message
- **`ref_time_ticks` → nanoseconds** (#2685) — monotonic ns on every platform, so raw subtraction is valid everywhere

#### JIT, AOT, and Build

- **Per-loop LLVM hints** (#2634, #2635, #2637, #2638, #2639) — `for` / `while` loops accept `unroll` / `vectorize` / `unroll_count=N` annotations driving `!llvm.loop.*` metadata; plus host-CPU TargetMachine, `mustprogress` backedges, cold panic paths, `math::length` intrinsic, and declared allocator memory effects
- **LLVM update + cross-compilation** (#2631, #2643, #2797, #2821) — bumped LLVM bindings, host-target-only initializers, and WASM JIT/exe backend support
- **LTO in Release** (#2858) and **clang-mingw64 toolchain** (#2838, #2840, #2846) — core + 7 modules + JIT build under clang-mingw64, with a Windows CI worker and dasClangBind regen (libclang 22.1.5)
- **JIT robustness** (#2793, #2808, #2811) — non-zero process exit on JIT failure/leaks, `-jit-no-cache` opt-out, and a `jit_table_at` DLLExport cache-hit fix

#### Runtime Libraries, Tooling, and Hosting

- **sqlite_linq LINQ-to-SQL coverage** (#2845, #2847, #2906, #2909, #2910, #2928, #2979, #2984) — `long_count` → `COUNT(*)`, `_distinct_by` as a chain operator + `COUNT(DISTINCT …)`, expression keys in `_group_by`, `_.<alias>` resolution after `_join`, computed-scalar `_select`, take/skip-before-aggregate subqueries, and workhorse-cast → SQLite `CAST` pushdown
- **`daslib/linq` mixed-source overloads** (#2931, #2932, #2934) — binary ops (join/union/intersect/except/concat/zip) and their `*_to_array` forms accepting one array + one iterator in either direction
- **daslang-live** (#2681, #2693, #2726, #2859, #2976) — post-`--` argv forwarding, `-project_root`, `--live-port=N` with a port-keyed lock, `POST /reset` (re-simulate without recompile, + a `reload_generation` counter), and a ModuleGroup-lifetime crash fix
- **Dynamic modules** (#2774, #2956, #2937, #2991) — `-load_module <path>` loads a module straight from its folder (no junction); order-independent loading via deferred-`dlopen` fixed-point retry; `compileDaScript` verifies modules are initialized; a `module_file_access` `option_blocked` hook
- **MCP** (#2620, #2701, #2718, #2908) — per-tool subprocess isolation, `project` / `project_root` threading through every tool, and stdio-audit hardening (logger + `popen_argv_pipe` + stdin isolation)
- **dasHV** (#2668, #2684, #2788, #2980) — enabled by default, env-gated log routing, libhv pin bump (trie-router fix), and async (non-blocking) HTTP handlers fixing a `stop()` / teardown deadlock
- **dastest** (#2894, #2897, #2968, #2974, #2985, #2988) — semi-isolated `--batch` mode, `--worker-index` / `--headless` forwarding (1-based, keeping live-API 9090 free), full pre-`--` flag preservation, crashing-subprocess stderr capture, and `--stack-on-exception`
- **das_hash_map** (#2716, #2722) — insert-only `map` / `set` variants (no tombstones/rehash), EASTL/no-exception build support, used for `Module::annotationData` / `requireModule`
- **JSON** (#2626, #2871, #2965) — lazier `JV` serialization, `vec2/3/4` emitted as `{x,y,z}` objects to match `JV<vec>`, and OR-combined flag enums serialized as their numeric value (was invalid empty)
- **`from_decs_template`** (#2745) and **`DelegateReturn` made public** (#2624)

#### Lint

A large rule-and-infrastructure expansion, swept clean across daslib / modules / tutorials / utils.

- **New rules** — PERF018 (direct iteration over `range(length)`), PERF019 (collapse double int-casts on bitfield ORs), PERF020 (redundant same-type workhorse cast), PERF021 (hoist common cast out of a ternary), PERF022 (`push_from` / `push_clone_from` / `emplace_from` bulk overloads), PERF023 (drop pre-clone before a qmacro splice), PERF024 (drop pre-clone before a `[clone(...)]`-annotated fn), PERF025 (redundant `string()` in interpolation), PERF014 extension (negated out-of-range char-class); STYLE005 (braceless early-exit), STYLE013 extension (`new`-allocated structs), STYLE020 / STYLE021 (`??` over `from_JV`, named-tuple `JV`), STYLE022 / STYLE023 (bitfield-as-field read/write), STYLE024 / STYLE025 / STYLE026 (redundant / over-broad / nested `unsafe`), STYLE027 (for-push-loop → comprehension), STYLE028 (`self->` inside class methods); LINT010 (dead-store), LINT011 (int-literal precision), LINT012 / LINT013 (unused function / block arguments)
- **Infrastructure** — `--enable` / `--disable` rule selection (#2758), the `.lint_config` TOML (#2884), self-spawn parallelization (43s → 14s, #2672), a daslang auto-fixer (~1900 mechanical rewrites, #2627, #2628), a self-lint check (#2813), an O(1) FileInfo line index + unified suppression API (#2670), the `[clone(...)]` function annotation (#2819), and a `.githooks/pre-push` hook mirroring CI (#2761, #2763)

### Bug Fixes

- **ThinLTO SROA miscompile** (#2922) — `atomic_signal_fence` before reading `value.m[idx]` in `das_index<Matrix>::at` stops ThinLTO/SROA reusing a stale register after an aliased-type write (manifested as wrong shoot direction in rel+ThinLTO AOT builds)
- **`eval_single_expression` crash on folded-away constexpr globals** (#2832) — `ExprVar(PI)`-style refs surviving in rtti-driven ASTs no longer deref a missing runtime slot
- **Empty-named AST nodes** (#2890) — new `invalid_empty_name` (`error[30296]`) catches empty/whitespace names across ~10 node kinds (e.g. the `:=`-in-named-arg bug)
- **Spurious `error[30151]` on `concept_assert` / `static_assert` under lint flags** (#2835); **clang-mingw64 `-fwrapv` signed-overflow UB** (#2840); **`ManagedVectorAnnotation::walk` TypeDecl leak** (#2863); **`ExprAt` null subexpression-type guard** (#2767)
- **AOT negative int32 index before unsigned wrap** (#2776); **`alignMask` uint32 truncation on ≥4 GB allocations** (#2762); **`long_count` truncation past 2³¹** (#2899)
- **Build / tooling** — dlopen `RTLD_GLOBAL` on POSIX (#2666), GC on serialization failure (#2669), AOT handled-type references (#2657), dasFormatter CRLF re-normalization unsticking the WASM build (#2829), cbind_boost backslash path strip (#2846, #2850), live_api POSIX shutdown hang (#2688), `migrate_93` disk-artifact leak (#2780), uriparser Windows path encoding (#2970), and assorted JIT/wasm-cross regressions (#2821)

### Site / Blog / Docs / Playground

- **Forge site redesign** (#2648, #2653, #2667, #2665, #2856, #2868) — new landing / blog / playground / mobile site, install-snippet fix, URL history, GoatCounter analytics, logo / slogan / favicon refresh, and a real daslang glyph
- **Playground** (#2749, #2797, #2804, #2810, #2827, #2927) — embedded `dastest` Test button gated on `[test]`, ~18 benchmark samples with a multi-platform cycler and deep-link, a JIT/interp toggle, `?example=` URL override, mobile gate dropped, and a 3-service URL-shortener fallback
- **Docs** (#2843, #2870, #2874, #2902, #2911, #2649, #2710) — `linq_decs` tutorial + living `linq_fold_patterns` reference, a stdlib-categorization CI gate (no "Uncategorized" sections), External Modules section, and Sphinx polish; news nav with a "N NEW" chip (#2907); Atom/RSS surfacing (#2682, #2683); `/daspkg` page (#2703, #2705)
- **Blog** — "The three horsemen" (#2660), "Internal tools" (#2678), "Do you even sort?" (#2708), "Its like dejavu all over again" (#2747), "Verbose much?" (#2807), "from macro hell" (#2867), "spaghetti of choice" (#2898), "10k tests" (#2930), "Kibble" (#2971), and GFM strikethrough rendering (#2973)

### Build / CI

- **Windows on Ninja + Defender exclusion** (#2961, #2963, #2967) across extended_checks / build / release, with sccache on Windows (#2964, #2982) and a no-filesystem build added to CI (#2959)
- **Caching and time** (#2786, #2815, #2982) — PR build-result caching, a CI-time reduction pass, and vcpkg/openssl caching everywhere
- **JIT CI scoping** (#2977) — skip JIT on win64 Debug / RelWithDebInfo, keep it on Release; **dasLLVM mingw fallback** to a prebuilt download (#2992); **eastl CI build fix** (#2949)
- **Packaging** (#2618, #2823, #2826, #2656) — stop publishing non-bundled release zips, a bundle install-rule smoke test (regressing the RC3 `cpp_search_config.das` breakage), `release.yml` narrowed to `[prereleased]`, and `install_name` alias / rpath scrubbing for release dylibs; untracked in-tree `modules/dasImgui` (now daspkg-managed, #2636)

### Examples and Tutorials

- **dasOPENAI** (#2938, #2942) — 12 examples + 7 mock-server tutorials; **dasStrudel tutorial rework** (#2997)
- **LINQ query tutorial** (#2990, #2994) — `tutorials/language/56_linq_query.das` + RST for the `%linq!` syntax; SQL operator and LINQ-over-XML tutorials (#2979, #2906, #2909, #2920)
- **Fourier viz** (#2695, #2699) — modernized to dasImgui boost-v2 with a dark theme; hash benchmark suite (#2722)

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

#### Style Lint and Unified Linting (#2386, #2390, #2391, #2417, #2441, #2516, #2517, #2533, #2538, #2612)

New compile-time linting now covers daslang style in addition to the existing correctness and performance passes.

- **`daslib/style_lint`** — detects non-idiomatic gen2 patterns such as unnecessary block pipes, declaration-then-assignment, array literal construction via repeated `push`, and long comment blocks
- **Unified lint flow** — shared warning collection, `nolint` suppression, comment-hygiene rules, and codebase-wide cleanup to drive warnings back down
- **Noise reduction** — one-line style enforcement was relaxed where it produced more friction than signal
- **Nine new perf/style rules** (#2612) — `PERF013`–`PERF017` and `STYLE016`–`STYLE019` extend the rule set with additional anti-patterns surfaced during the codebase sweep

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

#### Build, CI, and Web (#2578, #2600, #2602, #2611, #2614, #2616)

- **Tutorials build in CI** (#2578) — every tutorial now compiles in CI as part of the regular check run
- **Web target reuses the main `CMakeLists`** (#2600) — `daslang-web` picks up tests and tracks the same build configuration as the native targets, removing the duplicated CMake graph
- **MCP: parse-aware C++ source-search tools** (#2602) — `cpp_find_symbol`, `cpp_grep_usage`, `cpp_outline`, `cpp_goto_definition` join the existing daslang-side tools, with a configurable search root and git-signature staleness detection
- **MCP: auto-retry unqualified module names under `daslib/`** (#2616) — when a tool call references e.g. `fio` and the symbol can't be found, the server retries against `daslib/fio` before reporting a miss
- **`@live` extends to struct fields** (#2614) — `live_host` now annotates individual struct fields with `@live`, with per-field `init_hash` combining `f.init` and `g.init` so partial-struct reloads stay coherent

### Bug Fixes

- **Build, tooling, and CI fixes** (#2385, #2394, #2395, #2424, #2447, #2521, #2537, #2591, #2617) — package `.gitignore` handling, PEG standalone/LLVM issues, `require` fixes, `dasbind` fixes, glob dependency tracking, documentation/error-position corrections, daslang plugin: completion no longer silently bails when an external binding (e.g. OpenGL, GLSL preprocessor) is in scope, and `find_call_macro` null-deref fix with `macro_call` regression coverage
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
