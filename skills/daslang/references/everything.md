# daslang modules

Generated from the module documentation - a fix goes into the `//!` comment or the handmade page behind a line, then the file is regenerated.
One section per module: what the module is for, then its public symbols grouped as the reference manual groups them, one line each. A symbol with several overloads appears once.

- [PUGIXML_boost](#pugixml_boost) - The PUGIXML_BOOST module provides high-level daScript helpers on top of the low-level `pugixml` C++ bindings.
- [algorithm](#algorithm) - The ALGORITHM module provides array and collection manipulation algorithms including sorting, searching, set operations, element removal, and more.
- [ansi_colors](#ansi_colors) - The ANSI_COLORS module provides helpers for wrapping strings with ANSI escape codes for colored and styled terminal output.
- [apply](#apply) - The APPLY module provides the `apply` macro for iterating over struct, tuple, and variant fields at compile time.
- [apply_in_context](#apply_in_context) - The APPLY_IN_CONTEXT module extends apply operations to work across different execution contexts, enabling cross-context function invocation with packed arguments.
- [archive](#archive) - The ARCHIVE module implements general-purpose serialization infrastructure.
- [array_boost](#array_boost) - The ARRAY_BOOST module extends array operations with temporary array views over fixed-size arrays and C++ handled vectors, emptiness checks, sub-array views, and arithmetic operators on fixed-size arrays.
- [assert_once](#assert_once) - The ASSERT_ONCE module provides the `assert_once` macro — an assertion that triggers only on its first failure.
- [ast](#ast) - The AST module provides access to the abstract syntax tree representation of daslang programs.
- [ast_block_to_loop](#ast_block_to_loop) - The AST_BLOCK_TO_LOOP module provides an AST transformation macro that converts block-based iteration patterns into explicit loop constructs.
- [ast_boost](#ast_boost) - The AST_BOOST module provides high-level utilities for working with the AST.
- [ast_cursor](#ast_cursor) - Position-based AST queries.
- [ast_match](#ast_match) - AST pattern matching via reverse reification.
- [ast_used](#ast_used) - The AST_USED module implements analysis passes that determine which AST nodes are actually used in the program.
- [async_boost](#async_boost) - The ASYNC_BOOST module implements an async/await pattern for daslang using generator-based cooperative multitasking.
- [audio](#audio) - Module audio
- [audio_boost](#audio_boost) - Module audio_boost
- [audio_record](#audio_record) - Module audio_record
- [audio_wav](#audio_wav) - Module audio_wav
- [base64](#base64) - The BASE64 module implements Base64 encoding and decoding.
- [bitfield_boost](#bitfield_boost) - The BITFIELD_BOOST module provides utility macros for working with bitfield types including conversion between bitfield values and strings, and iteration over set bits.
- [bitfield_trait](#bitfield_trait) - The BITFIELD_TRAIT module implements reflection utilities for bitfield types: converting bitfield values to and from human-readable strings, iterating over individual set bits, and constructing bitfield values from string names.
- [bool_array](#bool_array) - The BOOL_ARRAY module provides a compact boolean array implementation using bit-packing.
- [build_const](#build_const) - The BUILD_CONST module provides compile-time constants from the build environment.
- [builtin](#builtin) - The BUILTIN module contains core runtime functions available in all daslang programs without explicit `require`.
- [class_boost](#class_boost) - The CLASS_BOOST module provides macros for extending class functionality, including the `[serialize_as_class]` annotation for automatic serialization and common class patterns like abstract method enforcement.
- [clipboard_core](#clipboard_core) - The clipboard module provides synchronous access to the operating system's text clipboard.
- [command_line](#command_line) - Command-line utilities for daslang scripts and standalone executables.
- [constant_expression](#constant_expression) - The CONSTANT_EXPRESSION module provides the `[constant_expression]` function annotation.
- [consume](#consume) - The CONSUME module implements the `consume` pattern, which moves ownership of containers and other moveable values while leaving the source in a default-constructed state.
- [contracts](#contracts) - The CONTRACTS module provides compile-time type constraints for generic function arguments.
- [coroutines](#coroutines) - The COROUTINES module provides coroutine infrastructure including the `[coroutine]` function annotation, `yield_from` for delegating to sub-coroutines, and `co_await` for composing asynchronous generators.
- [cpp_bind](#cpp_bind) - The CPP_BIND module provides utilities for generating daslang bindings to C++ code.
- [cross_context](#cross_context) - The CROSS_CONTEXT module requires a `shared` module after the prerequisite walk - from a macro, a simulate macro or an `[init]`, never from a running script, which has no compile for the module to join - and calls into its macro context by function name, so a module that is expensive to bring up and only sometimes needed comes in at the point that decides it needs it.
- [cuckoo_hash_table](#cuckoo_hash_table) - The CUCKOO_HASH_TABLE module implements a cuckoo hash table data structure.
- [dap](#dap) - The DAP module implements the Debug Adapter Protocol (DAP) for integrating daslang with external debuggers.
- [das_source_formatter](#das_source_formatter) - The DAS_SOURCE_FORMATTER module implements source code formatting for daslang.
- [das_source_formatter_fio](#das_source_formatter_fio) - The DAS_SOURCE_FORMATTER_FIO module extends the source formatter with file I/O capabilities, enabling formatting of daslang source files on disk.
- [dashv](#dashv) - The DASHV module provides HTTP and WebSocket networking built on top of the `libhv`_ library.
- [dashv_boost](#dashv_boost) - The DASHV_BOOST module provides high-level daScript wrapper classes for the low-level `dashv` C++ bindings.
- [dasllama](#dasllama) - CPU large-language-model inference in pure daslang: load a GGUF model, tokenize, run the transformer, sample — or hold a full chat — validated token-for-token against llama.cpp on every supported family.
- [dasllama_tts](#dasllama_tts) - Text to speech in pure daslang: load a converted StyleTTS2-lineage GGUF (KittenTTS nano and mini, Kokoro-82M), run text through the das-native front end (normalizer, part-of-speech tagger, grapheme-to-phoneme), and synthesize mono f32 PCM per sentence chunk, timed per model stage.
- [debug_eval](#debug_eval) - The DEBUG_EVAL module provides runtime expression evaluation for debugging purposes.
- [debugapi](#debugapi) - The DEBUGAPI module provides the debug agent infrastructure — creating, installing, and communicating with persistent debug agents that live in their own forked contexts.
- [decs](#decs) - The DECS module implements a Data-oriented Entity Component System.
- [decs_boost](#decs_boost) - The DECS_BOOST module provides convenience macros and syntactic sugar for the DECS entity component system, including simplified component registration, entity creation, and system definition patterns.
- [decs_state](#decs_state) - The DECS_STATE module extends DECS with state machine support for entities.
- [defer](#defer) - The DEFER module implements the `defer` pattern — the ability to schedule cleanup code to run at scope exit, similar to Go's `defer`.
- [delegate](#delegate) - C#-like multicast delegate (multicast callback) via type macro.
- [dynamic_cast_rtti](#dynamic_cast_rtti) - The DYNAMIC_CAST_RTTI module implements runtime dynamic casting between class types using RTTI information.
- [enum_trait](#enum_trait) - The ENUM_TRAIT module provides reflection utilities for enumerations: iterating over all values, converting between enum values and strings, and building lookup tables.
- [faker](#faker) - Random test-data generator.
- [fio](#fio) - The FIO module implements file input/output and filesystem operations.
- [flat_hash_table](#flat_hash_table) - The FLAT_HASH_TABLE module implements a flat hash table — open addressing with linear probing.
- [functional](#functional) - The FUNCTIONAL module implements lazy iterator adapters and higher-order function utilities including `filter`, `map`, `reduce`, `fold`, `scan`, `flatten`, `flat_map`, `enumerate`, `chain`, `pairwise`, `iterate`, `islice`, `cycle`, `repeat`, `sorted`, `sum`, `any`, `all`, `tap`, `for_each`, `find`, `find_index`, and `partition`.
- [fuzzer](#fuzzer) - The FUZZER module implements fuzz testing infrastructure for daslang programs.
- [generic_return](#generic_return) - The GENERIC_RETURN module provides the `[generic_return]` annotation that allows generic functions to automatically deduce their return type from the body.
- [gltf_processed](#gltf_processed) - Backend-neutral glTF preprocessing that packs and optionally encodes geometry, compresses textures, and persists validated monolithic or split-file assets.
- [gltf_types](#gltf_types) - dasGLTF loads `glTF 2.0`_ models — `.glb` (binary), `.gltf` (JSON), external or base64-embedded buffers — into a **backend-neutral scene**, with no new native code (it builds on `json_boost`, `base64`, `fio`, `stbimage` and `math`).
- [if_not_null](#if_not_null) - The IF_NOT_NULL module provides a null-safe call macro.
- [instance_function](#instance_function) - The INSTANCE_FUNCTION module provides the `[instance_function]` annotation for creating bound method-like functions.
- [interfaces](#interfaces) - The INTERFACES module implements interface-based polymorphism for daslang.
- [is_local](#is_local) - The IS_LOCAL module provides compile-time checks for whether a variable is locally allocated (on the stack) versus heap-allocated.
- [jobque](#jobque) - The JOBQUE module provides low-level job queue and threading primitives.
- [jobque_boost](#jobque_boost) - The JOBQUE_BOOST module provides high-level job queue abstractions built on the low-level `jobque` primitives: `new_job` / `new_thread` (which capture a lambda and clone the context for the worker), `with_wait_group` / `done`, `parallel_for` and the `team_parallel_*` family, and typed `push` / `pop` / `gather` over a `Stream`.
- [jobque_profile](#jobque_profile) - The JOBQUE_PROFILE module wraps the low-level `jobque_trace_*` builtins into a self-describing profiling API: named **categories** with colors for the op tags stamped via `profile_tag`, and **markers** — instant "unit" events such as `token` or `frame` — so saved traces carry their own legend and are navigable unit-to-unit.
- [json](#json) - The JSON module implements JSON parsing and serialization.
- [json_boost](#json_boost) - The JSON_BOOST module extends JSON support with operator overloads for convenient field access (`?.` / `?[]`), null-coalescing (`??`), and generic conversions in both directions: `JV(value)` builds a `JsonValue?` tree from a struct, tuple, array, table, or vector, and `from_JV(js, default)` reads one back into a typed value.
- [jsonrpc](#jsonrpc) - The JSON-RPC module is a transport-agnostic JSON-RPC 2.0 implementation (https://www.jsonrpc.org/specification).
- [linq](#linq) - The LINQ module provides query-style operations on sequences: filtering (`where_`), projection (`select`), sorting (`order`, `order_by`), deduplication (`distinct`), pagination (`skip`, `take`), aggregation (`sum`, `average`, `aggregate`), and element access (`first`, `last`).
- [linq_boost](#linq_boost) - The LINQ_BOOST module extends LINQ with pipe-friendly macros using underscore syntax for inline predicates and selectors.
- [linq_fold](#linq_fold) - The LINQ_FOLD module provides the `_fold` and `_old_fold` call macros that rewrite LINQ pipelines into optimized loop forms — `_fold` is the active fusion macro, `_old_fold` is a frozen pre-rewrite baseline kept for benchmark comparison as `_fold` evolves toward splice-mode fusion.
- [lint](#lint) - The LINT module implements static analysis checks for daslang code.
- [lint_config](#lint_config) - The lint_config module loads `{get_das_root()}/.lint_config` (a TOML file with a `[rules]` table of booleans) and folds it into a `disabled_codes` set consumed by the three lint pass-macros (`daslib/lint`, `daslib/perf_lint`, `daslib/style_lint`), by the standalone runner `utils/lint/main.das`, and by the MCP `lint` tool.
- [logger](#logger) - The logger module provides a structured, file-backed logging facility for daslang tools (MCP server, daslang-live, dastest, etc).
- [lpipe](#lpipe) - The LPIPE module provides the `lpipe` macro for passing multiple block arguments to a single function call.
- [macro_boost](#macro_boost) - The MACRO_BOOST module provides utility macros for macro authors, including pattern matching on AST nodes, code generation helpers, and common transformation patterns used when writing compile-time code.
- [match](#match) - The MATCH module implements pattern matching on variants, structs, tuples, arrays, and scalar values.
- [math](#math) - The MATH module contains floating point math functions and constants (trigonometry, exponentials, clamping, interpolation, noise, and vector/matrix operations).
- [math_bits](#math_bits) - The MATH_BITS module provides bit-level reinterpretation between integer and floating point representations — `int_bits_to_float`, `uint_bits_to_float`, `float_bits_to_int`, `float_bits_to_uint` (plus the 64-bit `double` forms and 2/3/4-lane vector overloads) — as well as the `cast_to_*` helpers that pack and unpack values through a `float4` payload.
- [math_boost](#math_boost) - The MATH_BOOST module adds geometric types (`AABB`, `AABR`, `Ray`), intersection tests (`is_intersecting`), plane helpers (`plane_dot`, `plane_normalize`, `plane_from_point_normal`, `planar_shadow`), color space conversion (`linear_to_SRGB`, `RGBA_TO_UCOLOR`, `UCOLOR_TO_RGBA`), and view/projection matrix construction (`look_at_lh`, `look_at_rh`, `perspective_rh`, `ortho_rh`).
- [md_boost](#md_boost) - The MD_BOOST module provides Markdown generation helpers: GitHub-flavored table rendering with aligned columns and per-column alignment, plus small text utilities (bold, cell-separator escaping).
- [meshoptimizer](#meshoptimizer) - Checked array bindings for meshoptimizer 1.2 vertex remapping, vertex-cache and vertex-fetch optimization, and vertex and index buffer codecs.
- [module_group](#module_group) - The MODULE_GROUP module calls into every member of a module group.
- [network](#network) - The NETWORK module implements networking facilities including HTTP client/server and low-level socket operations.
- [only_nttp](#only_nttp) - The ONLY_NTTP module provides the `[only_nttp]` function annotation — a guard for benchmark kernels.
- [openai_common](#openai_common) - A pure-daslang client for OpenAI-compatible REST APIs — OpenAI itself, plus any server that speaks the same surface (Ollama, OpenRouter, Kokoro, LM Studio, vLLM, …).
- [option](#option) - Monadic `Option<T>` — represents a value that may or may not be present.
- [peg](#peg) - The PEG module is a parser generator based on `Parsing Expression Grammars`_.
- [profiler](#profiler) - The PROFILER module provides CPU profiling infrastructure for measuring function execution times.
- [profiler_boost](#profiler_boost) - The PROFILER_BOOST module extends profiling with high-level macros for scoped timing (`profile_block`), function-level profiling annotations, and formatted output of profiling results.
- [pugixml](#pugixml) - The PUGIXML module provides XML parsing, navigation, manipulation, and XPath query support built on top of the `pugixml`_ C++ library.
- [quote](#quote) - The QUOTE module provides quasiquotation support for AST construction.
- [random](#random) - The RANDOM module implements pseudo-random number generation using a linear congruential generator with vectorized state (`int4`).
- [raster](#raster) - SIMD-accelerated pixel operations — gather, scatter, copy, channel conversion, and BPC conversion.
- [refactor](#refactor) - The REFACTOR module implements automated code refactoring transformations.
- [regex](#regex) - The REGEX module implements regular expression matching and searching.
- [regex_boost](#regex_boost) - The REGEX_BOOST module extends regular expressions with the `%regex~` reader macro for compile-time regex construction.
- [remove_call_args](#remove_call_args) - The REMOVE_CALL_ARGS module provides AST transformation macros that remove specific arguments from function calls at compile time.
- [result](#result) - Monadic `Result<T, E>` — a value (`ok`) or an error (`err`).
- [rst](#rst) - The RST module implements the documentation generation pipeline for daslang.
- [rtti](#rtti) - The RTTI module exposes runtime type information and program introspection facilities.
- [safe_addr](#safe_addr) - The SAFE_ADDR module provides compile-time checked pointer operations.
- [sha_256](#sha_256) - FIPS 180-4 SHA-256 in pure daslang: one-shot hashing of strings and byte arrays to lowercase hex, plus a streaming init/update/final state for data that arrives in pieces.
- [soa](#soa) - The SOA (Structure of Arrays) module transforms array-of-structures data layouts into structure-of-arrays layouts for better cache performance.
- [sort_boost](#sort_boost) - The SORT_BOOST module provides the `qsort` macro that uniformly sorts built-in arrays, dynamic arrays, and C++ handled vectors using the same syntax.
- [spirv_reflect](#spirv_reflect) - Shader reflection for the dasSpirv SPIR-V backend: an API-neutral description of a shader's descriptor bindings and push-constant ranges, emitted alongside the SPIR-V blob and consumed by the host (e.g.
- [static_let](#static_let) - The STATIC_LET module implements the `static_let` pattern — local variables that persist across function calls, similar to C `static` variables.
- [stbimage](#stbimage) - Low-level image I/O and resizing bindings for stb_image, stb_image_write, and stb_image_resize2.
- [stbimage_boost](#stbimage_boost) - High-level image API built on stbimage and raster.
- [stbimage_ttf](#stbimage_ttf) - High-level TrueType font loading and text rendering API.
- [stbtruetype](#stbtruetype) - Low-level TrueType font bindings for stb_truetype.
- [stddlg](#stddlg) - The stddlg module binds the operating system's standard dialogs and the system tray (menu bar on macOS, StatusNotifierItem on Linux desktops): message boxes, open and save file pickers, and a single tray icon with a tooltip, a context menu, click events and notifications.
- [stringify](#stringify) - The STRINGIFY module provides the `%stringify~` reader macro for embedding multi-line string literals verbatim.
- [strings](#strings) - The STRINGS module implements string formatting, conversion, searching, and modification routines.
- [strings_boost](#strings_boost) - The STRINGS_BOOST module extends string handling with splitting (`split`, `split_by_chars`), joining (`join`), padding (`wide`), multi-substring replacement (`replace_multiple`), and edit distance (`levenshtein_distance`).
- [strings_convert](#strings_convert) - The STRINGS_CONVERT module provides soft-failing string-to-numeric conversions that return `Result<T; ConversionError>` instead of panicking or silently returning zero.
- [strudel_event](#strudel_event) - Module strudel_event
- [strudel_live](#strudel_live) - Module strudel_live
- [strudel_midi](#strudel_midi) - Module strudel_midi
- [strudel_midi_player](#strudel_midi_player) - Module strudel_midi_player
- [strudel_mini](#strudel_mini) - Module strudel_mini
- [strudel_pattern](#strudel_pattern) - Module strudel_pattern
- [strudel_player](#strudel_player) - Module strudel_player
- [strudel_samples](#strudel_samples) - Module strudel_samples
- [strudel_scales](#strudel_scales) - Module strudel_scales
- [strudel_scheduler](#strudel_scheduler) - Module strudel_scheduler
- [strudel_sf2](#strudel_sf2) - Module strudel_sf2
- [strudel_sf2_voice](#strudel_sf2_voice) - Module strudel_sf2_voice
- [strudel_synth](#strudel_synth) - Module strudel_synth
- [strudel_time](#strudel_time) - Module strudel_time
- [temp_strings](#temp_strings) - The TEMP_STRINGS module provides temporary string construction that avoids heap allocations.
- [templates](#templates) - The TEMPLATES module implements template instantiation utilities for daslang code generation.
- [templates_boost](#templates_boost) - The TEMPLATES_BOOST module extends template utilities with high-level macros for common code generation patterns, including template function generation, type-parameterized struct creation, and compile-time code expansion.
- [texture_blocks](#texture_blocks) - Portable BC1, BC3, BC4, and BC5 texture compression with complete mip chains, deterministic cache keys, serialized validation, and RGBA8 decoding.
- [toml](#toml) - The TOML module parses `TOML 1.0`_ into the same `JsonValue?` tree shape produced by `daslib/json`, so existing `json_boost` accessors (`v ?? def`, `from_JV`, etc.) work on TOML inputs as-is.
- [tty](#tty) - The TTY module answers whether a stream is attached to a real terminal, and how wide that terminal is.
- [type_traits](#type_traits) - The TYPE_TRAITS module provides compile-time type introspection and manipulation.
- [typemacro_boost](#typemacro_boost) - The TYPEMACRO_BOOST module provides infrastructure for defining type macros — custom compile-time type transformations.
- [unroll](#unroll) - The UNROLL module implements compile-time loop unrolling.
- [uriparser](#uriparser) - The URIPARSER module provides URI parsing and manipulation based on the uriparser library.
- [uriparser_boost](#uriparser_boost) - The URIPARSER_BOOST module extends URI handling with convenience functions for common operations like building URIs from components, extracting query parameters, and resolving relative paths.
- [utf8_utils](#utf8_utils) - The UTF8_UTILS module provides Unicode UTF-8 string utilities including character iteration, codepoint extraction, byte length calculation, and validation of UTF-8 encoded text.
- [validate_code](#validate_code) - The VALIDATE_CODE module implements AST validation passes that check for common code quality issues, unreachable code, missing return statements, and other semantic errors beyond what the type checker verifies.
- [with_boost](#with_boost) - The WITH_BOOST module provides the `with_` call macro: bind one or more array / table element references inside a block, with an automatic container lock around the body so push / erase / resize / clear inside the body panic at runtime instead of silently dangling.

## builtin

The BUILTIN module contains core runtime functions available in all daslang programs without explicit `require`. It includes:


### Type aliases

- `half` - `half` spells the `float16` scalar without reserving the identifier — it is a builtin type alias, so locals and fields named `half` keep working.
- `print_flags` - This bitfield specifies how exactly values are to be printed

### Constants

- `DAS_MAX_FUNCTION_ARGUMENTS` - Maximum number of arguments a function can accept, used to pre-allocate stack space for function call arguments.
- `INT_MIN` - Minimum representable value of a signed 32-bit integer (`int`), equal to -2147483648.
- `INT_MAX` - Maximum representable value of a signed 32-bit integer (`int`), equal to 2147483647.
- `UINT_MAX` - Maximum representable value of an unsigned 32-bit integer (`uint`), equal to 4294967295.
- `LONG_MIN` - Minimum representable value of a signed 64-bit integer (`int64`).
- `LONG_MAX` - Maximum representable value of a signed 64-bit integer (`int64`).
- `ULONG_MAX` - Maximum representable value of an unsigned 64-bit integer (`uint64`).
- `FLT_MIN` - Smallest positive non-zero normalized value of the `float` type; for the most negative value use `-FLT_MAX`.
- `FLT_MAX` - Maximum finite representable value of the `float` (32-bit floating-point) type.
- `DBL_MIN` - Smallest positive non-zero normalized value of the `double` type; for the most negative value use `-DBL_MAX`.
- `DBL_MAX` - Maximum finite representable value of the `double` (64-bit floating-point) type.
- `LOG_CRITICAL` - Log level constant for critical errors such as panics, fatal failures, and shutdown notifications.
- `LOG_ERROR` - Log level constant for recoverable error conditions that do not require immediate shutdown.
- `LOG_WARNING` - Log level constant for warnings about potential problems, API misuse, or non-fatal error conditions.
- `LOG_INFO` - Log level constant for general informational messages about normal program operation.
- `LOG_DEBUG` - Log level constant for debug-level diagnostic messages useful during development.
- `LOG_TRACE` - Log level constant for the most verbose tracing and diagnostic output, typically used for detailed debugging.
- `VEC_SEP` - Read-only string constant used as the separator between vector components when printing; defaults to `","`.
- `HOST_FAST_MATH` - True when the runtime binary itself was compiled with relaxed float flags (`-ffast-math` or `/fp:fast`).
- `print_flags_debugger` - Predefined set of print_flags configured to match the output formatting used by the `debug` function.

### Handled structures

- `HashBuilder` - Helper structure to facilitate calculating hash values.

### Function annotations

- `deprecated` - Marks a function as deprecated, causing a compilation warning when referenced and excluding it from the final compiled context.
- `temp_string_result` - Declares that the function's result is always a fresh string allocation (or null) - never a passthrough of an input, never an interned or cached string, never retained by the callee.
- `never_alias_cmres` - Declares that the function never aliases cmres (copy-or-move result), disabling aliasing safety checks for the return value.
- `unused_argument` - Suppresses unused-argument warnings or errors for specific function parameters, providing a workaround when strict code policies are enabled.
- `export` - Forces a function to be exported and retained in the final compiled context, even if it is not directly called.
- `jit` - Explicitly forces the annotated function to be compiled using the JIT compiler, overriding default compilation decisions.
- `no_env` - Declares that the function, and everything it calls, must not read the environment.
- `hot_path` - Declares that the function, and everything it calls, must not allocate on the heap, read the environment, or perform I/O.
- `expect_dim` - Contract annotation requiring a function argument to be a fixed-size (statically dimensioned) array.
- `sideeffects` - Declares that the function has side effects, preventing the compiler from optimizing away or reordering its calls.
- `arch` - Binds the function to a section of an architecture document: `[arch(at="<doc>.md#<anchor>")]`, where the path resolves relative to the citing file's folder.
- `local_only` - Restricts a function to accept only local `make` expressions such as structure initializers and tuple constructors.
- `macro_function` - Marks a function as part of the macro subsystem, excluding it from the final compiled context unless it is explicitly referenced.
- `generic` - Forces a function to be treated as generic regardless of its argument types, causing it to be instanced in each calling module.
- `unsafe_outside_of_for` - Marks a function as unsafe to call outside of a source-level `for` loop, enforcing iterator-context usage.
- `finalize` - Registers a function to be called automatically when the context is shut down, for cleanup and resource release.
- `no_io` - Declares that the function, and everything it calls, must not perform console or file I/O.
- `no_aot` - Prevents ahead-of-time (AOT) C++ code generation for the annotated function, keeping it interpreted only.
- `marker` - Attaches arbitrary key-value annotation arguments to a function, typically used by macros to tag functions with metadata.
- `inline` - Splices the function body into every direct call site during compilation, in every execution tier.
- `pinvoke` - Marks a function as a platform invoke (pinvoke) entry, enabling cross-context calls via `invoke_in_context`, `invoke_debug_agent_method`, and `invoke_debug_agent_function`.
- `run` - Forces the function to be evaluated at compile time, ensuring its body executes during compilation rather than at runtime.
- `clone` - Marks named parameters as cloned internally by the function (e.g.
- `no_alloc` - Declares that the function, and everything it calls, must not touch the heap — no array or table growth, no `new` or `delete`, no string interpolation, no lambda capture frame.
- `expect_ref` - Contract annotation requiring a function argument to be passed by reference.
- `unsafe_operation` - Marks a function as an unsafe operation, requiring callers to wrap the call in an `unsafe` block.
- `type_function` - Marks a function as a type function, meaning it operates on types at compile time and does not generate runtime code.
- `nodiscard` - Enforces that the return value of the function must be used by the caller; discarding the result produces a compilation error.
- `never_inline` - Keeps the function out of best-effort (automatic) inlining: block-literal call-site splicing and the heuristic plain-call tier (options auto_inline_functions) both skip it.
- `hybrid` - Marks a function as a hybrid call target so that AOT generates indirect calls to it, allowing the function to be patched without recompiling dependent AOT code.
- `init` - Registers a function to be called automatically during context initialization, before any user code runs.
- `no_jit` - Prevents JIT compilation for the annotated function, forcing it to run in interpreted mode.
- `hint` - Provides optimization hints to the compiler for the annotated function via annotation arguments.
- `_macro` - Marks a function to be executed during the macro compilation pass, similar to `[init]` but running at macro time.
- `unsafe_when_not_clone_array` - Marks a function as unsafe to call outside of an array `clone` operation, restricting its usage context.
- `unsafe_deref` - Optimization annotation that removes null-pointer checks, bounds checks on array and string indexing, and similar safety validations.
- `expect_any_vector` - Contract annotation restricting a function argument to accept only `das::vector` template types.
- `builtin_array_sort` - Internal function annotation that provides the sorting implementation used by the built-in `sort` function.
- `alias_cmres` - Declares that the function always aliases cmres (copy-or-move result), disabling cmres return optimizations for it.
- `cold_path` - Declares that the function is not on a hot path, pruning the PERF026-028 walk at that call.
- `no_lint` - Skips all lint-pass checks for the annotated function, suppressing any lint warnings or errors it would produce.

### Call macros

- `memzero` - Fills a region of memory with zeros, used internally for default-initializing values.
- `make_function_unsafe` - Propagates the `unsafe` requirement to the calling function, making any function that calls it also require an `unsafe` block.
- `verify` - Assertion that preserves the evaluated expression even when asserts are disabled, ensuring side effects are never optimized out.
- `__builtin_table_set_insert` - Internal function annotation that implements the low-level key insertion for set-style tables (tables with keys only).
- `invoke` - Invokes a block, function pointer, or lambda, dispatching the call through the appropriate calling convention.
- `static_assert` - Compile-time assertion that produces a compilation error with an optional message when the condition is false.
- `debug` - Prints the human-readable representation of a value to the log and returns that same value, allowing inline debugging in expressions.
- `assert` - Runtime assertion that panics with an optional error message when the first argument evaluates to false; can be disabled globally.
- `__builtin_table_key_exists` - Internal function annotation that implements the low-level key presence check for the `key_exists` operation.
- `__builtin_table_find` - Internal function annotation that implements the low-level table lookup for the `find` operation.
- `__builtin_table_erase` - Internal function annotation that implements the low-level table entry removal for the `erase` operation.
- `concept_assert` - Compile-time assertion that reports the error at the call site of the asserted function rather than at the assert line itself.

### Reader macros

- `_esc` - Reader macro that returns the raw string content without processing escape sequences, e.g.

### Typeinfo macros

- `rtti_classinfo` - Typeinfo macro that generates RTTI `TypeInfo` metadata required for class initialization and reflection.

### Handled types

- `clock` - Handled type wrapping `das::Time`, which encapsulates the C `time_t` value for calendar time representation.
- `das_string` - Handled type wrapping `das::string` (typically `std::string`), providing heap-allocated mutable string storage.

### Structure macros

- `no_default_initializer` - Prevents the compiler from generating a default initializer for the annotated structure.
- `macro_interface` - Marks a class hierarchy as a macro interface, preventing it and its descendants from being exported by default.
- `cpp_layout` - Forces the structure to use C++ memory layout rules (alignment and padding) instead of native daslang layout.
- `persistent` - Allocates the structure on the C++ heap (via `new`) instead of the daslang context heap, allowing it to outlive the context.
- `comment` - No-op structure annotation that holds annotation arguments as metadata without affecting code generation.
- `safe_when_uninitialized` - Declares that the structure is safe to access before explicit initialization, suppressing uninitialized-use errors.

### Containers

- `back` - Accesses and returns a const temporary reference to the last element of the temporary dynamic array `a`.
- `capacity` - Returns the current capacity of the `table` — the number of key-value pairs it can hold before triggering a reallocation.
- `clear` - Removes every element from the array.
- `copy_to_local` - Copies the value `a` and returns it as a new local value on the stack, which can be used to work around aliasing issues where a reference might be invalidated.
- `each` - Creates an iterator that yields temporary references to each element of the temporary dynamic array `a`.
- `each_enum` - Creates an iterator that yields every value of the enumeration type inferred from `tt`, allowing iteration over all members of an enum.
- `each_kv` - Iterates over a table as `(key, value)` named tuples.
- `each_ref` - Wraps a lambda `lam` into an iterator that yields references to each value rather than copies.
- `emplace` - Moves `value` into the dynamic array `Arr` using move semantics, appending it to the end.
- `emplace_default` - Constructs a new default-initialized element in the table `tab` at the given `key`, only if that key does not already exist.
- `emplace_from` - Bulk-moves every element of source `varr` (an array or fixed-size C-array) into `Arr` via `<-`.
- `emplace_new` - Moves a smart pointer `value` into the end of the array `Arr`, constructing the entry in-place and returning a reference to it.
- `empty` - Checks whether the array has no elements and returns `true` if so.
- `ensure_capacity` - Grows the capacity of `Arr` to at least `newSize` elements, at least doubling it on each growth so an append-by-resize loop stays amortized O(1).
- `erase` - Removes the entry with key `at` from the table `Tab`, returning `true` if the key was found and erased.
- `erase_if` - Iterates over the array `arr` and removes all elements for which the block `blk` returns `true`.
- `find_index` - Searches the dynamic array `arr` for the first occurrence of `key` and returns its index, or -1 if not found.
- `find_index_if` - Returns the index of the first element in the fixed array `arr` for which `blk` returns `true`, or -1 if no element matches.
- `get` - Looks up `at` in the table `Tab` and, if found, invokes `blk` with a reference to the value; returns `true` if the key existed, `false` otherwise.
- `get_key` - Returns the key associated with a value reference obtained during table iteration.
- `get_value` - Retrieves the value associated with key `at` from the table `Tab`.
- `get_with_default` - Looks up key `at` in the table `Tab`, inserting a default-initialized entry if the key is absent, then invokes `blk` with a mutable reference to the value.
- `has_value` - Consumes elements from the iterator `a` and returns `true` if any element equals `key`.
- `insert` - Inserts the value `val` into the table `Tab` under key `at`.
- `insert_clone` - Inserts or updates an entry in the table `Tab` at key `at` by cloning the mutable value `val` into the table.
- `insert_default` - Inserts key `key` with the given const `value` into table `tab` only if the key does not already exist; existing entries are left unchanged.
- `is_scratch` - Reads the container's scratch mark back.
- `key_exists` - Checks whether the key `at` exists in the table `Tab` and returns `true` if found.
- `keys` - Creates an iterator over all keys of the mutable table `a`, allowing enumeration of the table's key set.
- `length` - Returns the number of elements currently stored in a table or dynamic array `a`.
- `lock` - Locks a constant table for the duration of `blk`, preventing structural modifications while providing read-only access through a temporary reference.
- `lock_forever` - Permanently locks a table, preventing any future insertions, deletions, or structural modifications, and returns a temporary reference to it.
- `long_capacity` - Returns the current capacity of a dynamic `array` — the number of elements it can hold before triggering a reallocation — as `int64`.
- `long_find_index` - Returns the int64 index of the first element in `arr` equal to `key`, or `-1` if not present.
- `long_length` - Returns the number of elements currently stored in a dynamic array `array`, as `int64`.
- `modify` - Looks up `at` in `Tab` and, if found, invokes `blk` with the current value, replacing it with the value returned by the block.
- `move_to_local` - Moves the value referenced by `a` onto the stack as a local copy and returns it, clearing the original; useful for resolving aliasing issues.
- `move_to_ref` - Moves `b` into the reference `a`; if `b` is a value type rather than a reference, it is copied instead of moved.
- `next` - Advances the iterator `it` and stores the next element in `value`, returning true if an element was retrieved or false if the iterator is exhausted or null.
- `nothing` - Produces an empty iterator of the same element type as `it` that yields no elements.
- `pop` - Removes and discards the last element of `Arr`, reducing its length by one.
- `push` - Appends a constant `value` to the end of dynamic array `Arr`, copying it into place.
- `push_clone` - Clones and appends element `b` to the container `A`, using deep copy semantics rather than move or shallow copy.
- `push_clone_from` - Bulk-appends two source arrays `a` and `b` to `Arr` via clone in one call, reserving the combined length up front.
- `push_from` - Bulk-appends two source arrays `a` and `b` to `Arr` via copy in one call, reserving the combined length up front so the whole append makes a single allocation.
- `remove_value` - Searches `arr` for the first element equal to `key` and removes it, returning true if an element was found and removed or false otherwise.
- `reserve` - Pre-allocates memory in `Tab` to hold at least `newSize` entries without rehashing, improving performance of subsequent insertions.
- `resize` - Resizes dynamic array `Arr` to `newSize` elements; new elements beyond the previous length are zero-initialized, and excess elements are removed.
- `resize_and_init` - Resizes dynamic array `Arr` to `newSize` elements, default-initializing any newly added elements.
- `resize_no_init` - Resizes dynamic array `Arr` to `newSize` elements without initializing newly added entries, leaving their memory contents undefined.
- `scratch_ensure_capacity` - ensure_capacity through the eager path: same doubling policy, but old buffers are freed immediately even under very_safe_context.
- `scratch_reserve` - reserve through the eager path: the old buffer is freed immediately even under very_safe_context, and the capacity is exact (reserve never rounds).
- `scratch_resize` - resize through the eager path: reserves the exact final size first, so a grow frees the old buffer immediately even under very_safe_context; a shrink never reallocates.
- `set_scratch` - Marks the container as scratch: the owner promises no interior alias survives a grow, so growth frees the old buffer eagerly even under very_safe_context.
- `sort` - Sorts a dynamic array in place in ascending order using the default comparison for its element type.
- `stable_sort` - Sorts a dynamic array in place in ascending order (default `<`), preserving the input order of equal elements (stable sort).
- `subarray` - Returns a temporary sub-range of the dynamic array `a` defined by the signed range `r`, providing access to elements from `r.x` up to but not including `r.y`.
- `to_array` - Converts a fixed-size array `a` into a new dynamic array by cloning each element.
- `to_array_move` - Converts a mutable container `a` into a new dynamic array, moving elements when possible instead of cloning.
- `to_table` - Converts a fixed-size array of key-value tuples `a` into a `table<keyT, valT>` by cloning each key and value.
- `to_table_move` - Converts a mutable dynamic array of keys `a` into a set-style `table<keyT, void>`, moving its elements and freeing the source array.
- `values` - Returns a read-only iterator over all values in a `table<keyT, valT>`, yielding each value by const reference.

### das::string manipulation

- `peek` - Provides zero-copy read access to the contents of a `das_string` by invoking `block` with a temporary string reference, avoiding allocation.

### Heap reporting

- `frame_position` - Returns the call site's frame position -- the value the garbage collector's locals gate reads from a stack frame's line handoff -- resolved at simulate time within the calling function's own numbering.
- `heap_allocation_count` - Returns the total number of heap allocations performed by the current context since it was created.
- `heap_allocation_stats` - Returns heap allocation statistics as a `urange64`, where the `x` component is total bytes allocated and the `y` component is total bytes freed.
- `heap_bytes_allocated` - Returns the number of bytes currently in use on the heap (allocated minus freed), not counting reserved but unused memory.
- `heap_collect` - Triggers garbage collection on the context heap; when `string_heap` is `true` the string heap is also collected, and when `validate` is `true` additional validation checks are performed.
- `heap_depth` - Returns the number of generations (depth of the allocation chain) in the context's regular heap.
- `heap_report` - Prints a diagnostic report of current heap usage and allocation statistics to the output log.
- `heap_total_allocated` - Total bytes the context's value heap has reserved from the OS (aligned), including currently-free space.
- `max_unreserved_size` - Returns the context's `max_unreserved_size` limit in bytes: an array `resize` that has to grow past this many bytes without a prior `reserve` panics.
- `memory_report` - Prints a report of memory allocations for the current context; when `errorsOnly` is true, only GC-related errors are included.
- `set_max_unreserved_size` - Sets the context's `max_unreserved_size` limit to `bytes`: an array `resize` that has to grow past this many bytes without a prior `reserve` panics.
- `string_heap_allocation_count` - Returns the total number of individual string allocations performed on the current context's string heap.
- `string_heap_allocation_stats` - Returns string heap allocation statistics as a `urange64` where `x` is total bytes allocated and `y` is total bytes deleted.
- `string_heap_bytes_allocated` - Returns the total number of bytes currently allocated in the current context's string heap.
- `string_heap_depth` - Returns the number of generational layers (depth) in the current context's string heap.
- `string_heap_report` - Prints a detailed report of string heap usage, including allocation counts and byte statistics, to the log output.
- `string_heap_total_allocated` - Total bytes the context's string heap has reserved from the OS (aligned), including currently-free space.
- `tag_array` - Labels the array's current heap block with `name` so it appears under that label in heap reports instead of the generic "array".
- `tag_table` - Labels the table's current heap block with `name` so it appears under that label in heap reports instead of the generic "table".

### 16/8-bit lattice converts

- `byte2_sat` - Converts an `int2` to `byte2` with saturation: each lane is clamped to the `int8` range instead of truncating.
- `byte3_sat` - Converts an `int3` to `byte3` with saturation: each lane is clamped to the `int8` range instead of truncating.
- `byte4_sat` - Converts an `int4` to `byte4` with saturation: each lane is clamped to the `int8` range instead of truncating.
- `byte8_sat` - Converts a `short8` to `byte8` with saturation: each lane is clamped to the `int8` range instead of truncating.
- `half8_hi` - Unpacks the high four fp16 lanes of a `half8` into a `float4` (exact widening).
- `half8_lo` - Unpacks the low four fp16 lanes of a `half8` into a `float4` (exact widening).
- `short2_sat` - Converts a `int2` to `short2` with saturation: each lane is clamped to the `int16` range instead of truncating.
- `short3_sat` - Converts a `int3` to `short3` with saturation: each lane is clamped to the `int16` range instead of truncating.
- `short4_sat` - Converts a `int4` to `short4` with saturation: each lane is clamped to the `int16` range instead of truncating.
- `ubyte2_sat` - Converts a `uint2` to `ubyte2` with saturation: each lane is clamped to the `uint8` range instead of truncating.
- `ubyte3_sat` - Converts a `uint3` to `ubyte3` with saturation: each lane is clamped to the `uint8` range instead of truncating.
- `ubyte4_sat` - Converts a `uint4` to `ubyte4` with saturation: each lane is clamped to the `uint8` range instead of truncating.
- `ubyte8_sat` - Converts a `ushort8` to `ubyte8` with saturation: each lane is clamped to the `uint8` range instead of truncating.
- `ushort2_sat` - Converts a `uint2` to `ushort2` with saturation: each lane is clamped to the `uint16` range instead of truncating.
- `ushort3_sat` - Converts a `uint3` to `ushort3` with saturation: each lane is clamped to the `uint16` range instead of truncating.
- `ushort4_sat` - Converts a `uint4` to `ushort4` with saturation: each lane is clamped to the `uint16` range instead of truncating.

### 16/8-bit lattice integer dots

- `idot` - Exact integer dot product of all 16 signed int8 lanes: widens each lane pair to 32-bit, multiplies, and sums into a single `int`.
- `idot4` - Packed integer dot product in groups of four: lane `i` of the result is the exact sum `a[4i]*b[4i] + ...
- `shuffle` - Per-lane byte table select: lane `i` of the result is `lut[idx[i] & 15]` — the NEON `tbl1` / SSE `pshufb` semantic with the index masked to the 16-entry table.

### GC0 infrastructure

- `gc0_reset` - Clears the entire gc0 storage, invalidating all previously saved pointers and smart pointers stored within it.
- `gc0_restore_ptr` - Retrieves a raw pointer previously saved in gc0 storage under the specified `name`, returning `null` if not found.
- `gc0_restore_smart_ptr` - Retrieves a `smart_ptr<void>` previously saved in gc0 storage under the specified `name`.
- `gc0_save_ptr` - Stores a raw pointer `data` into gc0 storage under the specified `name`, allowing it to be retrieved later with `gc0_restore_ptr`.
- `gc0_save_smart_ptr` - Stores a `smart_ptr<void>` `data` into gc0 storage under the specified `name`, allowing it to be retrieved later with `gc0_restore_smart_ptr`.

### Smart ptr infrastructure

- `add_ptr_ref` - Wraps a raw pointer `src` of type `TT?` into a `smart_ptr<TT>` by incrementing the reference count.
- `get_const_ptr` - Extracts a constant raw pointer of type `TT?` from the given `smart_ptr<TT>`, without affecting reference counting.
- `get_ptr` - Extracts a mutable raw pointer of type `TT?` from the given mutable `smart_ptr<TT>`, without affecting reference counting.
- `move` - Moves the smart pointer `src` into the smart pointer `dest`, nullifying the previous contents of `dest` and transferring ownership from `src`.
- `move_new` - Moves a newly constructed smart pointer value `src` into `dest`, used to initialize a `smart_ptr` from a `new` expression.
- `smart_ptr_clone` - Clones the smart pointer `src` into smart pointer `dest`, incrementing the internal reference count to share ownership.
- `smart_ptr_is_valid` - Checks whether the smart pointer `dest` holds a non-null reference to valid data, returning true if it does.
- `smart_ptr_use_count` - Returns the current reference count of the object managed by `ptr`, indicating how many smart pointers share ownership.

### Macro infrastructure

- `is_building_documentation` - Returns true while the host is compiling a documentation or reflection root.
- `is_compiling` - Returns `true` if the current context is in the process of being compiled, allowing compile-time logic to distinguish from runtime execution.
- `is_compiling_macros` - Returns `true` if the current context is being compiled and the compiler is currently executing the macro pass.
- `is_compiling_macros_in_module` - Returns `true` if the current context is being compiled during the macro pass and the compiler is processing the module specified by `name`.
- `is_folding` - Returns `true` if the compiler is currently performing its constant folding optimization pass.
- `is_in_completion` - Returns `true` if the compiler is running in completion mode, generating lexical information for a text editor's code-completion system.
- `is_in_lint_check` - Returns true if the program is being compiled in lint-check mode (i.e.
- `is_reporting_compilation_errors` - Returns `true` if the context failed to compile and the inference pass is currently reporting compilation errors.

### Profiler

- `collect_profile_info` - Collects profiling information gathered by the built-in line profiler and returns it as a formatted string containing execution counts and timing data.
- `dump_profile_info` - Prints the execution counts and timing data for all lines collected by the built-in line profiler to the standard output.
- `profile` - Executes `block` a total of `count` times under the given `category` label, prints the timing, and returns the minimum elapsed time in seconds across all iterations.
- `reset_profiler` - Resets all counters and accumulated data in the built-in profiler to zero.

### System infrastructure

- `aot_enabled` - Checks whether ahead-of-time (AOT) compilation is enabled for the current program and returns true if it is.
- `breakpoint` - Triggers a debugger breakpoint by calling `os_debugbreakpoint`, which is a link-time dependency expected to be provided by the host application or debugger tool.
- `diagnostics_to_file` - Sends the runtime's own diagnostics -- a leak dump, a fatal, what it reports about itself -- to `path`, appending to it, and returns false without changing where they go when the file cannot be opened.
- `diagnostics_to_stderr` - Sends the runtime's own diagnostics -- a leak dump, a fatal, what it reports about itself -- to standard error, for a program whose standard output carries a protocol rather than its own output.
- `error` - Outputs the string `text` to the context's error stream, similar to `print` but directed to the error output channel.
- `eval_main_loop` - Executes the application main loop by repeatedly invoking `block` until it returns `false`; on Emscripten targets, uses the platform-specific main loop mechanism instead.
- `feint` - No-op replacement for `print`.
- `get_das_root` - Returns the file-system path to the daslang root directory, where `daslib` and other standard libraries are located.
- `get_das_version` - Returns the daslang SDK version as a string in "major.minor.patch" format (e.g.
- `get_stackwalk` - Returns the current call stack as a string — the same report `stackwalk` prints, captured instead of logged.
- `is_in_aot` - Returns `true` if the compiler is currently generating ahead-of-time (AOT) compiled code.
- `is_intern_strings` - Returns `true` if string interning is enabled in the current context, meaning identical strings share the same memory.
- `is_safe_hash` - Returns true when the runtime computes string hashes with the safe byte-at-a-time algorithm rather than the faster word-at-a-time one.
- `panic` - will cause panic.
- `print` - Outputs `text` to the current context's log, typically printing to standard output.
- `shared_module_extension` - Returns the file extension used for shared (dynamic) modules on the current platform.
- `sprint` - Converts `value` to its string representation using the specified `flags` to control formatting, and returns the result as a string.
- `sprint_json` - Serializes `value` directly to a JSON string, bypassing intermediate representation for speed; set `humanReadable` to true for indented output.
- `sscan_json` - Parses a JSON string directly into `value` using RTTI type information — no intermediate `JsonValue?` representation.
- `stackwalk` - Prints the current call stack to the log; set `args` to include function arguments and `vars` to include local variable values in the output.
- `terminate` - Immediately terminates execution of the current daslang context.
- `to_compiler_log` - Outputs `text` to the compiler's log stream, typically used from within macro code during compilation.
- `to_log` - Outputs `text` to the logging infrastructure at the specified `level` (e.g.

### Memory manipulation

- `hash` - Computes a 64-bit FNV-1a hash of the given `int8` value and returns it as `uint64`.
- `hash_combine64` - Folds `value` into the running `hash` and returns the new accumulator, i.e.
- `intptr` - Converts a function or lambda value `f` to the `uint64` integer representation of the address it carries.
- `lock_data` - Locks a constant array and invokes `blk` with a read-only pointer `p` to the array's contiguous data and its size `s`, allowing direct memory-level read access.
- `map_to_array` - Constructs a temporary mutable array of type `TT` over raw memory at `data` with `len` elements, and passes it to `blk` without copying the underlying data.
- `map_to_ro_array` - Constructs a temporary read-only array of type `TT` over raw memory at `data` with `len` elements, and passes it to `blk` without copying the underlying data.
- `memcmp` - Compares `size` bytes of memory at `left` and `right`, returning -1 if `left` is less, 1 if `left` is greater, or 0 if both regions are identical.
- `memcpy` - Copies `size` bytes of memory from the address pointed to by `right` into the address pointed to by `left`.
- `memmove` - Copies `size` bytes of memory from the address pointed to by `right` into the address pointed to by `left`; the two ranges may overlap, which `memcpy` does not allow.
- `set_variant_index` - Overwrites the internal type discriminator of `variant` to `index`, changing which alternative the variant is considered to hold.
- `variant_index` - Returns the zero-based index indicating which alternative the variant currently holds.

### Binary serializer

- `binary_load` - Deserializes `obj` from the binary representation stored in `data` (an array of uint8 bytes).
- `binary_save` - Serializes `obj` into a binary representation and passes the resulting uint8 byte array to the block `subexpr`.

### Path and command line

- `get_command_line_arguments` - Returns an array of strings containing the command-line arguments passed to the program.
- `with_argv` - Sets `argc`, `argv` to first argument, for the `body` block.

### Time and date

- `format_time` - Formats a `clock` value as local-time text via `strftime`.
- `get_clock` - Returns the current calendar time as a `clock` value representing the number of seconds since 00:00 UTC, January 1, 1970 (the Unix epoch).
- `get_time_nsec` - Computes the elapsed time in nanoseconds since the reference point `ref`, which is typically obtained from `ref_time_ticks`.
- `get_time_usec` - Computes the elapsed time in microseconds since the reference point `ref`, which is typically obtained from `ref_time_ticks`.
- `iso8601_now` - Returns the current UTC wallclock time as an ISO 8601 string with millisecond precision: `"YYYY-MM-DDTHH:MM:SS.mmmZ"` (24 characters, trailing `Z` for UTC).
- `mktime` - Converts the calendar date and time specified by `year`, `month`, `mday`, `hour`, `min`, and `sec` into a `clock` value representing time since epoch.
- `ref_time_ticks` - Returns a monotonic timestamp in nanoseconds since an unspecified epoch (the same epoch within a process; not comparable across processes or reboots).

### Platform queries

- `cpu_supports` - Report whether the RUNNING CPU supports a hardware feature by name — cpuid truth on x86/x86_64, OSXSAVE-honest ("avx*" names are true only when the OS also saves the register state they need).
- `das_is_dll_build` - Checks whether the current build is configured as a DLL (dynamic library) build, which determines if daslib symbols are available for the JIT compiler.
- `das_is_exceptions_enabled` - Reports whether the runtime was built with DAS_ENABLE_EXCEPTIONS=1, i.e.
- `get_architecture_name` - Returns the name of the CPU architecture the program is running on, such as `"x86_64"`, `"x86"`, `"arm64"`, `"arm"`, `"wasm32"`, or `"unknown"`.
- `get_context_share_counter` - Returns the use-count of the shared context, which is incremented each time a thread accesses it; useful for tracking concurrent context usage.
- `get_cross_platform_name` - Returns `"emscripten"` when the program is being cross-compiled to WebAssembly (detected from a `--jit-target` argument naming a `wasm` or `emscripten` triple), and an empty string otherwise; module initializers use it to register web-specific native bindings during a cross-compile.
- `get_platform_name` - Returns the name of the operating system the program is running on, such as `"windows"`, `"linux"`, `"darwin"`, `"emscripten"`, or `"unknown"`.
- `get_running_platform_name` - Returns the name of the operating system the program is currently running on, such as `"windows"`, `"linux"`, `"darwin"`, `"emscripten"`, or `"unknown"`.
- `get_target_architecture_name` - Returns the architecture component of the compile target: `"wasm64"`, `"wasm32"`, `"x86_64"`, `"arm64"` (for both the `aarch64-` and `arm64-` triples), `"x86"`, `"arm"`, or `"unknown"`; the host's own architecture when no `--jit-target` names another.
- `get_target_triple` - Returns the LLVM target triple the program is being compiled for, such as `"wasm64-unknown-emscripten"`, read from the command line's `--jit-target` (the last one wins); `""` for the host.
- `is_standalone_exe` - Returns `true` when the current binary was produced by `daslang -exe` (a standalone executable).

### String formatting

- `fmt` - Formats a `double` value as a string using the given `format` specifier (following libfmt / C++20 `std::format` syntax).

### Argument consumption

- `consume_argument` - Marks argument `a` as consumed, signaling to the compiler that it will not be used after this call, which enables move optimizations and avoids unnecessary clones.

### Lock checking

- `lock_count` - Returns the current internal lock count for the given `array`, indicating how many active locks prevent it from being resized.

### Bit operations

- `__bit_set` - Sets or clears the bits specified by `mask` in the 16-bit bitfield `value`, turning them on if `on` is true or off if `on` is false.
- `clz` - Counts the number of leading zero bits in the 64-bit unsigned integer `bits`, returning 64 if the value is zero.
- `ctz` - Counts the number of trailing zero bits in the 64-bit unsigned integer `bits`, returning 64 if the value is zero.
- `mul128` - Multiplies two 64-bit unsigned integers `a` and `b`, returning the full 128-bit result as a `urange64` containing the low and high 64-bit halves.
- `popcnt` - Counts and returns the number of set (1) bits in the 64-bit unsigned integer `bits`.

### Intervals

- `interval` - Constructs a `range64` value from the two `int64` endpoints `arg0` (inclusive) and `arg1` (exclusive).

### RTTI

- `class_rtti_size` - Examines the RTTI (runtime type information) associated with the class at `ptr` and returns the size in bytes of its TypeInfo structure.

### Initialization and finalization

- `using` - Creates a temporary `das_string` and passes it to the block, automatically managing its lifetime for the duration of the call.

### Algorithms

- `count` - Creates an infinite iterator that yields integer values starting from `start` and incrementing by `step` on each iteration, intended for use as a counter alongside other sequences in a `for` loop.
- `iter_range` - Creates a `range` from `0` to the length of the given iterable `foo`, useful for index-based iteration over containers.
- `long_iter_range` - Returns an `int64` range iterator over the valid indices of an iterable `foo` (typically an array or table).
- `swap` - Exchanges the values of `a` and `b` in place, leaving each variable holding the other's former value.
- `ucount` - Creates an infinite iterator over unsigned integers beginning at `start` and incrementing by `step` on each iteration.

### Memset

- `memset128` - Fills memory at `left` with `count` copies of the 128-bit `uint4` vector `value`.
- `memset16` - Fills memory at `left` with `count` copies of the 16-bit `value`.
- `memset32` - Fills memory at `left` with `count` copies of the 32-bit `value`.
- `memset64` - Fills memory at `left` with `count` copies of the 64-bit `value`.
- `memset8` - Fills memory at `left` with `count` copies of the 8-bit `value`, equivalent to the C `memset` function.

### Malloc

- `free` - Frees memory previously allocated with `malloc`, following C-style manual memory management semantics.
- `malloc` - Allocates a block of uninitialized memory of the specified `size` in bytes, C-style, and returns a raw pointer to it; must be freed with `free`.
- `malloc_usable_size` - Returns the usable size in bytes of the memory block pointed to by `ptr`, as reported by the underlying allocator.

### Compilation and AOT

- `compiling_file_name` - Returns the file name of the source file currently being compiled, useful for compile-time metaprogramming and diagnostics.
- `compiling_module_name` - Returns the name of the module currently being compiled, useful for compile-time metaprogramming and diagnostics.
- `get_module_file_name` - Returns the source file path of the module with the given name, or empty string if the module is not found or has no file (e.g.
- `reset_aot` - Notifies the compiler that ahead-of-time code generation has finished, restoring normal compilation mode.
- `set_aot` - Notifies the compiler that ahead-of-time code generation is now in progress.

### GC

- `gc_active_root_count` - Returns the number of active GC roots across all threads.
- `gc_thread_root_count` - Returns the number of GC roots on the current thread.
- `gc_thread_root_report` - Prints a summary report of GC roots on the current thread to the log.
- `gc_thread_root_report_detailed` - Prints a detailed report of GC roots and their nodes on the current thread to the log.
- `gc_total_id` - Returns the total number of GC node IDs allocated since program start.

## math

The MATH module contains floating point math functions and constants (trigonometry, exponentials, clamping, interpolation, noise, and vector/matrix operations). Floating point math in general is not bit-precise: the compiler may optimize permutations, replace divisions with multiplications, and some functions are not bit-exact. Use `double` precision types when exact results are required.


### Constants

- `PI` - The single-precision float constant pi (3.14159265...), representing the ratio of a circle's circumference to its diameter.
- `DBL_PI` - The double-precision constant pi (3.141592653589793...), representing the ratio of a circle's circumference to its diameter.
- `FLT_EPSILON` - The smallest single-precision float value epsilon such that 1.0f + epsilon != 1.0f, approximately 1.1920929e-7.
- `DBL_EPSILON` - The smallest double-precision value epsilon such that 1.0 + epsilon != 1.0, approximately 2.2204460492503131e-16.

### Handled structures

- `float4x4` - floating point matrix with 4 rows and 4 columns
- `float3x4` - floating point matrix with 4 rows and 3 columns
- `float3x3` - floating point matrix with 3 rows and 3 columns

### Horizontal reduce (float2, float3, float4)

- `hadd` - Horizontal sum: returns the sum of the vector's components.
- `hmax` - Horizontal maximum: returns the largest of the vector's components.
- `hmin` - Horizontal minimum: returns the smallest of the vector's components.

### all numerics (uint*, int*, float*, double)

- `max` - Returns the component-wise maximum of two values, supporting scalar double, float, int, int64, uint, uint64 and vector float2, float3, float4 types.
- `min` - Returns the component-wise minimum of two values, supporting scalar double, float, int, int64, uint, uint64 and vector float2, float3, float4 types.

### float* and double

- `abs` - Returns the absolute value of x, component-wise for vectors and across numeric scalar overloads.
- `acos` - Returns the arccosine of x in radians, component-wise for float2/float3/float4 vectors, and for float/double scalars.
- `acosh` - Returns the inverse hyperbolic cosine of x, component-wise for float2/float3/float4 vectors, and for float/double scalars.
- `asin` - Returns the arc sine of x in radians, for double precision argument.
- `asinh` - Returns the inverse hyperbolic sine of x, component-wise for float2/float3/float4 vectors, and for float/double scalars.
- `atan` - Returns the arctangent of x in radians, with the result in the range [-pi/2, pi/2]; works with float and double.
- `atan2` - Returns the arc tangent of y/x in radians, for double precision arguments, using the signs of both arguments to determine the quadrant.
- `atanh` - Returns the inverse hyperbolic tangent of x, component-wise for float2/float3/float4 vectors, and for float/double scalars.
- `cbrt` - Returns the cube root of x, component-wise for float2/float3/float4 vectors, and for float/double scalars.
- `ceil` - Returns the smallest integer value not less than x, component-wise for vectors, and for float/double scalars.
- `cos` - Returns the cosine of x, where x is specified in radians; works with float and double.
- `cosh` - Returns the hyperbolic cosine of x, component-wise for float2/float3/float4 vectors, and for float/double scalars.
- `degrees` - Converts the input from radians to degrees (multiplies by 180 / PI); computed component-wise for float2, float3, and float4 vector types.
- `exp` - Returns e raised to x, component-wise for float2/float3/float4 vectors, and for float/double scalars.
- `exp2` - Returns 2 raised to x, component-wise for float2/float3/float4 vectors, and for float/double scalars.
- `expm1` - Returns exp(x)-1 with improved accuracy near zero, component-wise for float2/float3/float4 vectors, and for float/double scalars.
- `floor` - Returns the largest integer value not greater than x, component-wise for vectors, and for float/double scalars.
- `fmod` - Returns the floating-point remainder of x/y using truncation-style quotient (same sign behavior as %), component-wise for vectors and for float/double scalars.
- `hypot` - Returns sqrt(x*x + y*y), component-wise for float2/float3/float4 vectors, and for float/double scalar pairs.
- `is_finite` - Returns true if the double value x is finite (not NaN and not infinity).
- `is_nan` - Returns true if x is NaN (Not a Number), checked component-wise for float2, float3, and float4 vector types; works with float and double scalars.
- `log` - Returns the natural (base-e) logarithm of x; the input must be positive; computed component-wise for float2, float3, and float4 vector types; works with float and double scalars.
- `log10` - Returns the base-10 logarithm of x, component-wise for float2/float3/float4 vectors, and for float/double scalars.
- `log1p` - Returns ln(1+x) with improved accuracy near zero, component-wise for float2/float3/float4 vectors, and for float/double scalars.
- `log2` - Returns the base-2 logarithm of x; the input must be positive; computed component-wise for float2, float3, and float4 vector types; works with float and double scalars.
- `pow` - Returns x raised to the power of y for scalar double, float, or vector float2, float3, float4 types; domain requires x >= 0 for non-integer y values.
- `radians` - Converts the input from degrees to radians (multiplies by PI / 180); computed component-wise for float2, float3, and float4 vector types.
- `rcp` - Returns the reciprocal (1/x) of a scalar float or each component of a float2, float3, or float4 vector.
- `remainder` - Returns the IEEE 754 remainder of x/y (round-to-nearest quotient), component-wise for vectors and for float/double scalars.
- `safe_acos` - Returns the arccosine of x in radians, clamping the input to the valid domain [-1, 1] to prevent NaN results from out-of-range values.
- `safe_asin` - Returns the arc sine of x clamped to [-1,1], for double precision argument, avoiding NaN for out-of-range inputs.
- `saturate` - Clamps the scalar double, float, or each component of a float2, float3, float4 vector to the [0, 1] range, returning 0 for values below 0 and 1 for values above 1.
- `sign` - Returns the sign of x component-wise: -1 for negative, 0 for zero, and 1 for positive.
- `sin` - Returns the sine of the angle x given in radians for double or float, with output in the range [-1, 1].
- `sincos` - Computes both the sine and cosine of the angle x in radians simultaneously, writing the results to output parameters s and c, for float or double types.
- `sinh` - Returns the hyperbolic sine of x, component-wise for float2/float3/float4 vectors, and for float/double scalars.
- `smoothstep` - Performs a smooth Hermite interpolation between 0 and 1 when x is between edge0 and edge1: returns 0 for x <= edge0, 1 for x >= edge1, and the smooth cubic t*t*(3 - 2*t) (where t = clamp((x - edge0) / (edge1 - edge0), 0, 1)) in between.
- `sqrt` - Returns the square root of a scalar double, float, or each component of a float2, float3, or float4 vector; input must be non-negative.
- `step` - Returns 0.0 when x is strictly less than edge, otherwise 1.0; computed component-wise for float2, float3, and float4 vector types.
- `tan` - Returns the tangent of the angle x given in radians for double or float; undefined at odd multiples of pi/2.
- `tanh` - Returns the hyperbolic tangent of x, component-wise for float2/float3/float4 vectors, and for float/double scalars.
- `trunc` - Rounds x toward zero, component-wise for float2/float3/float4 vectors, and for float/double scalars.

### float* only

- `atan2_est` - Returns an estimated arc tangent of y/x for each component of the float2 vectors.
- `atan_est` - Returns a fast estimated arctangent of x in radians, trading some precision for speed; the result approximates the range [-pi/2, pi/2].
- `ceili` - Returns the smallest integer not less than x (rounds toward positive infinity), converting the double argument to an int result.
- `exp_est` - Returns a fast estimate of e raised to `x` - a 4th-order polynomial in 2^x with a relative error within 1e-4 - for code that trades precision for speed the way `rcp_est` does.
- `exp_std` - Returns e raised to `x` through the C library's `expf` - bit-for-bit what the JIT's `exp` produces, where the interpreter and AOT compute `exp` with the vecmath polynomial.
- `float3x3-` - Returns the component-wise arithmetic negation of a matrix, flipping the sign of every element; works with float3x3, float3x4, and float4x4 matrix types.
- `float3x4-` - Returns the component-wise arithmetic negation of a matrix, flipping the sign of every element; works with float3x3, float3x4, and float4x4 matrix types.
- `float4x4-` - Returns the component-wise arithmetic negation of a matrix, flipping the sign of every element; works with float3x3, float3x4, and float4x4 matrix types.
- `floori` - Returns the largest integer not greater than x, converting the double argument to an int result.
- `fract` - Returns the fractional part of x (equivalent to x - floor(x)), computed component-wise for float2, float3, and float4 vector types; works with float and double scalars.
- `pow_est` - Returns a fast estimate of x raised to the power y for each component of a float2, float3, or float4 vector, trading precision for speed; unlike pow it drops the sign of a negative base, which makes it exactly the GLSL pow semantic.
- `rcp_est` - Returns a fast hardware estimate of the reciprocal (1/x) of a scalar float or each component of a float2, float3, or float4 vector, trading precision for speed.
- `round` - Returns the rounded value of x to the nearest integer; works with float and double scalars.
- `roundi` - Rounds the double x to the nearest integer value and returns the result as an int.
- `rsqrt` - Returns the reciprocal square root (1/sqrt(x)) of a scalar float or each component of a float2, float3, or float4 vector.
- `rsqrt_est` - Returns a fast hardware estimate of the reciprocal square root (1/sqrt(x)) of a scalar float or each component of a float2, float3, or float4 vector, trading precision for speed.
- `trunci` - Truncates the float x toward zero to the nearest integer and returns the result as an int.

### float3 only

- `cross` - Returns the cross product of two float3 vectors, producing a float3 vector perpendicular to both inputs with magnitude equal to the area of the parallelogram they span.
- `distance` - Returns the Euclidean distance between two vectors as a float scalar; works with float2, float3, and float4 vector types.
- `distance_sq` - Returns the squared Euclidean distance between two vectors as a float scalar, avoiding the square root for faster distance comparisons; works with float2, float3, and float4 vector types.
- `inv_distance` - Returns the reciprocal of the Euclidean distance between two vectors (1 / distance(x, y)) as a float; works with float2, float3, and float4 vector types.
- `inv_distance_sq` - Returns the reciprocal of the squared Euclidean distance between two vectors (1 / distance_sq(x, y)) as a float; works with float2, float3, and float4 vector types.
- `reflect` - Computes the reflection of float2 or float3 vector v off a surface with unit normal n, returning the reflected vector as v - 2*dot(v,n)*n.
- `refract` - Computes the refraction direction of vector v through a surface with unit normal n using Snell's law with index of refraction ratio nint.

### float2, float3, float4

- `dot` - Returns the dot product (scalar product) of two vectors as a float; works with float2, float3, and float4 vector types.
- `fast_normalize` - Returns a unit-length vector in the same direction as x using a fast approximation; does not check for zero-length input; works with float2, float3, and float4 vector types.
- `inv_length` - Returns the reciprocal of the length of the vector (1 / length(x)) as a float; works with float2, float3, and float4 vector types.
- `inv_length_sq` - Returns the reciprocal of the squared length of the vector (1 / length_sq(x)) as a float; works with float2, float3, and float4 vector types.
- `length` - Returns the Euclidean length (magnitude) of the vector as a float; works with float2, float3, and float4 vector types.
- `length_sq` - Returns the squared Euclidean length of the vector as a float, equivalent to dot(x, x) and avoiding the square root for faster magnitude comparisons; works with float2, float3, and float4 vector types.
- `normalize` - Returns a unit-length vector with the same direction as the input float2, float3, or float4 vector; behavior is undefined if the input vector has zero length.

### Noise functions

- `uint32_hash` - Returns a well-distributed uint hash of the input uint seed using an improved integer hash function suitable for hash tables and procedural generation.
- `uint_noise_1D` - Generates a deterministic uint hash value from a 1D integer position and a uint seed, suitable for repeatable procedural noise.
- `uint_noise_2D` - Generates a deterministic uint hash value from 2D integer coordinates (x, y) and a uint seed, suitable for repeatable procedural noise.
- `uint_noise_3D` - Generates a deterministic uint hash value from 3D integer coordinates (x, y, z) and a uint seed, suitable for repeatable procedural noise.

### lerp/mad/clamp

- `clamp` - Returns the value t clamped to the inclusive range [a, b], equivalent to min(max(t, a), b); works with float, double, float2, float3, float4, int, int64, uint, and uint64 types.
- `lerp` - Performs linear interpolation between a and b using the factor t, returning a + (b - a) * t; when t is 0 the result is a, when t is 1 the result is b; works component-wise with float, double, float2, float3, and float4 types.
- `mad` - Computes the multiply-add operation `a * b + c`.

### Matrix operations

- `float3x3!=` - Returns true if two float3x3 matrices are not equal, comparing all elements component-wise.
- `float3x3*` - Transforms a float3 vector by a 3x3 matrix.
- `float3x3==` - Returns true if two float3x3 matrices are exactly equal, comparing all elements component-wise.
- `float3x4!=` - Returns true if two float3x4 matrices are not equal, comparing all elements component-wise.
- `float3x4*` - Transforms a float3 vector by a 3x4 matrix.
- `float3x4==` - Returns true if two float3x4 matrices are exactly equal, comparing all elements component-wise.
- `float4x4!=` - Returns true if two float4x4 matrices are not equal, comparing all elements component-wise.
- `float4x4*` - Transforms a float4 vector by a 4x4 matrix.
- `float4x4==` - Returns true if two float4x4 matrices are exactly equal, comparing all elements component-wise.

### Matrix initializers

- `identity3x3` - Returns a float3x3 identity matrix with ones on the diagonal and zeros elsewhere.
- `identity3x4` - Returns a float3x4 identity transformation matrix with the rotation part set to identity and the translation set to zero.
- `identity4x4` - Returns a float4x4 identity matrix with ones on the diagonal and zeros elsewhere.

### Matrix manipulation

- `compose` - Constructs a float4x4 transformation matrix from a float3 translation position, a float4 quaternion rotation, and a float3 scale.
- `decompose` - Decomposes a float4x4 transformation matrix into its float3 translation position, float4 quaternion rotation, and float3 scale components., writing the results into the output arguments rot and pos.
- `determinant` - Returns the determinant of a float3x3 matrix as a float scalar; a zero determinant indicates the matrix is singular and non-invertible.
- `identity` - Sets the given float3x3 matrix to the identity transformation and returns it.
- `inverse` - Returns the inverse of a matrix, such that multiplying the original by its inverse yields the identity; works with float3x4 and float4x4 matrix types.
- `look_at` - Constructs a float4x4 look-at view transformation matrix from eye position, target position, and up vector.
- `orthonormal_inverse` - Returns the inverse of a float3x3 orthonormal matrix (each axis is unit length and mutually perpendicular), computed more efficiently than a general matrix inverse.
- `persp_forward` - Returns a forward (standard) perspective projection float4x4 matrix constructed from horizontal scale wk, vertical scale hk, near plane zn, and far plane zf.
- `persp_reverse` - Returns a reverse-depth perspective projection float4x4 matrix constructed from horizontal scale wk, vertical scale hk, near plane zn, and far plane zf, mapping the far plane to 0 and the near plane to 1.
- `rotate` - Rotates a float3 vector v by the 3x3 rotation part of the float3x4 matrix m, ignoring the translation component.
- `translation` - Constructs a float4x4 matrix representing a pure translation by the given float3 offset.
- `transpose` - Returns the transpose of a float3x3 or float4x4 matrix, swapping rows and columns.

### Quaternion operations

- `euler_from_quat` - Converts a float4 quaternion to Euler angles, returning a float3 representing rotation around the x, y, and z axes in radians.
- `quat` - Extracts the rotation part of a float3x4 matrix and returns it as a float4 quaternion in (x, y, z, w) format.
- `quat_conjugate` - Returns the conjugate of the float4 quaternion q by negating its xyz components, which for unit quaternions equals the inverse rotation.
- `quat_from_euler` - Creates a float4 quaternion from a float3 of Euler angles (pitch, yaw, roll) given in radians.
- `quat_from_unit_arc` - Creates a float4 quaternion representing the shortest rotation arc from unit-length float3 vector v0 to unit-length float3 vector v1; both input vectors must be normalized.
- `quat_from_unit_vec_ang` - Creates a float4 quaternion representing a rotation of ang radians around the unit-length float3 axis vector v.
- `quat_mul` - Returns the float4 quaternion product of q1 and q2, representing the combined rotation of q2 followed by q1.
- `quat_mul_vec` - Rotates a float3 vector v by the float4 quaternion q and returns the resulting float3 vector.
- `quat_slerp` - Performs spherical linear interpolation between float4 quaternions a and b by factor t in the range [0,1], returning a smoothly interpolated float4 quaternion.

### Packing and unpacking

- `pack_float_to_byte` - Packs a float4 vector into a single uint by converting each component to an 8-bit byte value, mapping the XYZW components to the four bytes of the result.
- `unpack_byte_to_float` - Unpacks the four bytes of a uint into a float4 vector, converting each 8-bit byte value to its corresponding floating-point component.

## fio

The FIO module implements file input/output and filesystem operations. It provides functions for reading and writing files (`fopen`, `fread`, `fwrite`), directory management (`mkdir`, `dir`), path manipulation (`path_join`, `base_name`, `dir_name`), and file metadata queries (`stat`, `file_size`).


### Type aliases

- `file` - Type alias for FILE const? used as the standard file handle parameter type across fio functions.
- `fs_result_bool` - A variant type that holds either a bool value on success, or an error string on failure.
- `fs_result_int64` - A variant type that holds either an int64 value on success, or an error string on failure.
- `fs_result_string` - A variant type that holds either a string value on success, or an error string on failure.
- `process`

### Constants

- `seek_set` - Constant for fseek that positions the file pointer relative to the beginning of the file by the given offset.
- `seek_cur` - Constant for fseek that positions the file pointer relative to its current position by the given offset.
- `seek_end` - Constant for fseek that positions the file pointer relative to the end of the file by the given offset.
- `popen_timed_out` - Sentinel exit code returned by `popen_timeout` when the process was killed due to exceeding the timeout.
- `process_running` - The value `process_poll` and `process_wait` return while the child is still running.
- `df_magic` - Magic number constant used to identify daslang binary file format.

### Structures

- `df_header` - Obsolete header structure used by fsave and fload for binary serialization with type identification.

### Handled structures

- `FStat.size` - Returns the size of the file in bytes.
- `FStat.atime` - Returns the last access time of the file as a clock value.
- `FStat.ctime` - Returns the creation time of the file as a clock value.
- `FStat.mtime` - Returns the last modification time of the file as a clock value.
- `FStat.is_reg` - Returns true if the file status indicates a regular file.
- `FStat.is_dir` - Returns true if the file status indicates a directory.
- `FStat.is_exec` - Returns true if the file status carries an execute permission - any of the owner, group or other execute bits on POSIX; on Windows, an executable extension (`.exe`, `.cmd`, `.bat`, `.com`).
- `FStat` - `stat` and `fstat` return file information in this structure.
- `DiskSpaceInfo` - Disk space information for a filesystem.

### Handled types

- `FILE` - Opaque handle wrapping the platform-specific C FILE type used by all low-level file I/O functions.
- `SubProcess` - A long-lived child process spawned by `spawn_process`: an opaque handle a supervisor polls, drains and signals across many ticks, unlike the block-scoped `popen_argv`.

### Direct IO and mapping advice

- `dwrite_append` - Append `bytes` from `data` at the writer's current end.
- `dwrite_band` - Hand out the writer's own staging band so a producer can build bytes in place instead of building a buffer and appending a copy.
- `dwrite_close` - Flush the writer's tail (sector-padded on cache-bypassing platforms), truncate the file to exactly what was appended, and free the handle.
- `dwrite_commit` - Account `bytes` written into the pointer `dwrite_band` handed out; a full band flushes to the device automatically.
- `dwrite_open` - Open `path` for cache-bypassing sequential append (`FILE_FLAG_NO_BUFFERING` on Windows, `F_NOCACHE` on macOS, `posix_fallocate` plus `fadvise` elsewhere).
- `dwrite_stat` - Where the writer spent its time so far: `which` 0 = staging nanoseconds (memcpy into the bounce band), 1 = syscall nanoseconds, 2 = bytes written directly from caller memory, 3 = bytes that went through the bounce band.
- `prefetch_map` - Advisory readahead over a mapped range: asks the OS to fault `bytes` starting at `base` into the page cache ahead of use (`PrefetchVirtualMemory` on Windows, `madvise(MADV_WILLNEED)` on POSIX).

### File manipulation

- `copy_file` - Copies a file from src to dst.
- `copy_file_result` - Copies a file from src to dst.
- `equivalent` - Returns true if two paths refer to the same file or directory.
- `equivalent_result` - Returns true if two paths refer to the same file or directory.
- `fbinary` - Switches the stream to binary mode, so the bytes written are the bytes sent and the bytes read are the bytes received: no newline translation, no end-of-file character.
- `fclose` - Closes the given FILE pointer and releases its associated resources, equivalent to C fclose.
- `feof` - Returns true if the end-of-file indicator has been set on the given FILE pointer, equivalent to C feof.
- `fexist` - Returns true if a file or directory exists at the given path.
- `fflush` - Flushes any buffered output data for the given FILE pointer to the underlying file, equivalent to C fflush.
- `fgets` - Reads and returns the next line as a string from the given FILE pointer, equivalent to C fgets.
- `file_size` - Returns the size of a file in bytes.
- `file_size_result` - Returns the size of a file in bytes.
- `fload` - Loads binary data from a file using the df_header format into the provided buffer.
- `fmap` - Memory-maps the contents of the given FILE pointer and provides the data as an array of uint8 inside the block.
- `fmap_close` - Unmaps a mapping produced by `fmap_open`.
- `fmap_open` - Memory-maps the file at `path` read-only and returns the mapping's base pointer, writing the mapped byte count through `size`.
- `fmap_open_rw` - The writable twin of `fmap_open`: memory-maps the file at `path` as a SHARED read-write view and returns the mapping's base pointer, writing the mapped byte count through `size`.
- `fopen` - Opens the file at `name` with the given `mode` (e.g.
- `for_each_registered_dynamic_module` - Iterates over all registered dynamic modules, invoking the block with the library path, C++ module name, and daslang module name for each entry.
- `for_each_registered_native_path` - Iterates over all registered native path mappings, invoking the block with the module name, source prefix, and destination prefix for each entry.
- `fpoll` - Waits up to `timeout_ms` milliseconds for the stream to become readable, and returns true when a read would not block: data is available, or the stream is at its end (a closed pipe reads empty).
- `fprint` - Writes the given text string to the specified FILE pointer, equivalent to print but targeting a file.
- `fread` - Reads raw binary data from a file into an array of POD elements.
- `fsave` - Obsolete; saves the provided buffer data to a file in binary format.
- `fseek` - Repositions the file pointer of the given FILE to the specified offset relative to the mode (seek_set, seek_cur, or seek_end) and returns the new position.
- `fstat` - Returns the file status (size, modification time, etc.) for an open file handle.
- `fstderr` - Returns the FILE pointer corresponding to the standard error stream.
- `fstdin` - Returns the FILE pointer corresponding to the standard input stream.
- `fstdout` - Returns the FILE pointer corresponding to the standard output stream.
- `ftell` - Returns the current byte offset of the file pointer for the given FILE, equivalent to C ftell.
- `funbuffered` - Turns off the C stream buffer on the stream, so every read goes to the descriptor and `fpoll` sees every byte the stream holds: nothing waits in a stdio buffer the poll cannot see.
- `fwrite` - Writes raw binary data from a POD value to a file.
- `getchar` - Reads and returns the next character from standard input as an integer, equivalent to C getchar.
- `is_reparse_point` - Returns true if the path is a symbolic link — and on Windows, any reparse point: a junction, a mount point, or a less common flavor such as a cloud-file placeholder.
- `is_reparse_point_result`
- `is_symlink` - Returns true if the path is a symbolic link.
- `is_symlink_result` - Returns true if the path is a symbolic link.
- `long_fread`
- `long_fwrite`
- `remove` - Deletes the file at the specified path and returns true if it was removed successfully.
- `remove_result` - Removes a file at the specified path.
- `rename` - Renames or moves a file from old_name to new_name and returns true on success.
- `rename_result` - Renames or moves a file or directory from src to dst.
- `set_mtime` - Sets the last modification time of a file or directory.
- `set_mtime_result`
- `stat` - Returns the file status (size, modification time, etc.) for a file at the given path.

### Path manipulation

- `base_name` - Extracts and returns the final component of a file path, equivalent to POSIX basename.
- `dir_name` - Extracts and returns the directory component of a file path, equivalent to POSIX dirname.
- `extension` - Returns the file extension including the dot (e.g.
- `get_full_file_name` - Returns the fully resolved and normalized absolute path for the given file path string.
- `is_absolute` - Returns true if the path is an absolute path (rooted).
- `normalize` - Returns the lexically normalized path with redundant separators, "." and ".." resolved.
- `parent` - Returns the parent directory of the path, or empty string if at root level.
- `path_join` - Joins two path components with the platform-native separator.
- `relative` - Returns a relative path from base to path.
- `relative_result` - Returns a relative path from base to path.
- `replace_extension` - Returns a new path with the file extension replaced by new_ext (include the dot, e.g.
- `stem` - Returns the filename without its extension (e.g.
- `tilde_home`
- `to_generic_path`

### Directory manipulation

- `chdir` - Changes the current working directory to the specified path and returns true on success.
- `dir` - Iterates over all entries in the directory at the given path, invoking the block with each filename.
- `dir_rec` - Recursively iterates all entries in a directory tree, invoking the block with each path (relative to root) and is_dir flag.
- `getcwd` - Returns the absolute path of the current working directory as a string.
- `mkdir` - Creates a single directory at the specified path and returns true if it was created successfully.
- `mkdir_rec` - Recursively creates the directory at the specified path along with any missing parent directories, returning true on success.
- `mkdir_result` - Creates a single directory at the specified path.
- `rmdir` - Removes an empty directory.
- `rmdir_rec` - Recursively removes a directory and all its contents (files and subdirectories).
- `rmdir_rec_result` - Recursively removes a directory and all its contents.
- `rmdir_result` - Removes an empty directory at the specified path.

### Glob and pattern matching

- `expand_glob`
- `glob`
- `glob_filtered`
- `is_glob_pattern`
- `match_glob`
- `parse_file_list`

### Filesystem queries

- `create_temp_directory` - Creates a uniquely-named temporary directory with the given prefix.
- `create_temp_directory_result` - Creates a uniquely-named temporary directory with the given prefix.
- `create_temp_file` - Creates a uniquely-named temporary file with the given prefix.
- `create_temp_file_result` - Creates a uniquely-named temporary file with the given prefix.
- `disk_space` - Queries disk space for the filesystem containing the given path.
- `temp_directory` - Returns the path to the system temporary directory.
- `temp_directory_result` - Returns the path to the system temporary directory.

### Terminal queries

- `is_terminal` - Returns true when the standard stream `fd` (0 for input, 1 for output, 2 for error) is attached to a terminal.
- `terminal_width` - Returns the terminal width in columns, or 0 when nothing reports one — the stream is not a terminal, or the platform query failed.

### OS specific routines

- `close_process` - Frees the handle.
- `exit` - Terminates the program immediately with the specified integer exit code, equivalent to C exit.
- `exit_now` - Ends the process at once with the given exit code: stdout and stderr are flushed, and nothing else runs - no `atexit` handler, no module teardown, no leak dump.
- `get_env_variable` - Returns the string value of the environment variable with the given name, or an empty string if undefined.
- `has_env_variable` - Returns true if an environment variable with the given name is defined in the current process environment.
- `popen` - Opens a pipe to the given shell command, provides the resulting FILE pointer to the block, and returns the process exit code.
- `popen_argv` - Spawns a subprocess directly via argv (Windows: CreateProcess; Unix: fork+execvp), bypassing the shell entirely so arguments are not subject to `$()`, backtick, or cmd.exe quote-stripping.
- `popen_argv_pipe` - Spawns a subprocess with bidirectional pipes connected to its stdin and stdout (child stderr is merged into stdout).
- `popen_binary` - Opens a pipe to the given shell command in binary mode, provides the resulting FILE pointer to the block, and returns the process exit code.
- `popen_timeout` - Opens a process for reading, kills the entire process tree if it exceeds `timeout` seconds.
- `process_alive` - Whether a process with this id exists - any process, not only a child - for a single-instance guard reading a pid file.
- `process_drain` - Hands `block` every complete line of the child's merged stdout+stderr that is ready right now, one call per line with the newline stripped, and never blocks: a partial line waits in the handle for the next drain, and a last line with no newline is delivered once the child closes its output.
- `process_kill` - Kills the child and everything it spawned: `SIGKILL` to its process group on POSIX, `TerminateJobObject` with exit code 9 on Windows.
- `process_pid` - The child's process id, for a pid file or a log line.
- `process_poll` - The child's exit code if it has exited, else `process_running`; never waits.
- `process_terminate` - Asks the child and everything it spawned to stop: `SIGTERM` to its process group on POSIX, `TerminateJobObject` with exit code 15 on Windows, where nothing gentler reaches a console-less process.
- `process_wait` - Waits up to `timeout` seconds for the child to exit and returns its exit code, or `process_running` when the timeout passes first.
- `run_and_capture`
- `sanitize_command_line` - Escapes and sanitizes a command-line argument string to prevent shell injection.
- `set_env_variable` - Sets an environment variable in the current process (the `setenv`/`_putenv` family).
- `sleep` - Suspends execution of the current thread for the specified number of milliseconds.
- `spawn_argv` - Launches a detached subprocess directly from an argument array without invoking a shell.
- `spawn_process` - Spawns `argv` as a long-lived child and returns its handle.
- `system` - Runs a shell command via the C runtime `system()` function.
- `with_process`

### Dynamic modules

- `describe_pending_dynamic_modules` - One line per dynamic module whose shared library failed to load during the module scan, with the loader error; empty when nothing is pending.
- `no_manifest` - Called from a `.das_module` descriptor's `initialize`: marks the descriptor as one that runs on every start, so the module scan keeps no manifest of its registrations and never replays them.
- `register_dynamic_module` - Loads a shared library from the given path and registers it as a daslang module under the specified name, making it available for require.
- `register_module_group` - Registers `member` - a require path, spelled the way a `require` would spell it - under the group name `group`, so `require [group]` in any module brings it in; `guard`, when given, is the member's own guard, what `require ?guard member` would carry (a module name, or a path when it holds a `/`), evaluated at every compile.
- `register_native_path` - Registers a path prefix mapping for a module, redirecting file resolution from the src prefix to the dst prefix.

## clipboard_core

The clipboard module provides synchronous access to the operating system's text clipboard. Text is always exchanged as UTF-8, including supplementary Unicode characters such as emoji.


### Type aliases

- `ClipboardCapabilities`
- `ClipboardFormats`

### Enumerations

- `ClipboardStatus`

### Structures

- `ClipboardImage`
- `ClipboardContent`
- `ClipboardMemory`

### UTF-8 system clipboard

- `clipboard_capabilities`
- `clipboard_clear`
- `clipboard_clone_content`
- `clipboard_content_has_data`
- `clipboard_dispose_content`
- `clipboard_get_content`
- `clipboard_get_text`
- `clipboard_memory_clear`
- `clipboard_memory_dispose`
- `clipboard_memory_read`
- `clipboard_memory_write`
- `clipboard_set_content`
- `clipboard_set_text`
- `clipboard_validate_content`

## stddlg

The stddlg module binds the operating system's standard dialogs and the system tray (menu bar on macOS, StatusNotifierItem on Linux desktops): message boxes, open and save file pickers, and a single tray icon with a tooltip, a context menu, click events and notifications.


### Enumerations

- `TrayEventKind` - What a tray event reports: how the icon or its menu was used.

### Handled structures

- `TrayEvent` - One tray event as handed to the tray_poll block.

### Standard dialogs

- `dlg_init` - Initializes the dialog backend (GTK on Linux); call once before the first dialog.
- `get_dlg_ok_cancel_from_user` - Shows a message box with OK and Cancel (Yes and No on GTK); returns true for OK.
- `get_dlg_ok_from_user` - Shows a message box with the given caption and body and a single OK button; returns true once dismissed, false where no dialog backend exists.
- `get_dlg_open_file` - Shows the system open-file picker starting at the given folder; the filter is a | separated list of extensions.
- `get_dlg_save_file` - Shows the system save-file picker starting at the given file name and folder; the filter is a | separated list of extensions.

### Tray icon

- `tray_available` - True when this process can reach a tray host: a session bus (DBUS_SESSION_BUS_ADDRESS) plus a loadable libdbus-1 on Linux, the main thread of a window-server session on macOS, an interactive desktop with a taskbar on Windows.
- `tray_create` - Creates the process's single tray icon with the given tooltip and returns true; a second call while the icon exists only replaces the tooltip.
- `tray_destroy` - Removes the tray icon and releases the backend; a later tray_create starts over.
- `tray_menu_add` - Appends a menu entry: its id (positive, unique within the menu; the call panics otherwise) is reported back in a menu event, then its label, whether it can be chosen, and whether it shows a check mark.
- `tray_menu_add_separator` - Appends a separator line to the menu being built.
- `tray_menu_clear` - Starts a new context menu; nothing changes on screen until tray_menu_commit.
- `tray_menu_commit` - Publishes the menu built since tray_menu_clear as the icon's context menu.
- `tray_notify` - Posts a desktop notification with the given title and body: a balloon on Windows (true once the shell accepted it), org.freedesktop.Notifications on Linux (true once the daemon answered), osascript on macOS (true once the script ran).
- `tray_poll` - Pumps the tray's platform events on the calling thread - the thread that called tray_create - and invokes the block once per event, in order.
- `tray_set_icon` - Replaces the icon with RGBA8 pixels, row-major, width by height; each side is 1 to 1024 and the array holds exactly width * height * 4 bytes, or the call panics.
- `tray_set_tooltip` - Replaces the tooltip (also the title a Linux host shows).

## network

The NETWORK module implements networking facilities including HTTP client/server and low-level socket operations. It provides `Server` and `Client` classes with event-driven callbacks for handling connections, requests, and responses.


### Handled structures

- `NetworkClient` - The native TCP client behind the `Client` class: one outgoing connection, driven by `client_connect`, `client_tick`, `client_send` and `client_close`.
- `NetworkServer` - Base implementation of the server.

### Low level NetworkServer IO

- `make_server` - Creates a new `Server` instance.
- `server_init` - Initializes the server to listen on the specified port.
- `server_is_connected` - Returns `true` if the server has an active client connection.
- `server_is_open` - Returns `true` if the server is listening on its bound port.
- `server_restore` - Restores a server from an orphaned or interrupted state.
- `server_send` - Sends data from the server to the connected client.
- `server_tick` - Processes pending network I/O; must be called periodically for the server to function.

### Low level NetworkClient IO

- `client_close` - Closes the client's socket.
- `client_connect` - Opens a TCP connection to `host` (a host name, an IPv4 or IPv6 literal, or `localhost`) on `port`, trying each address the name resolves to in turn.
- `client_is_connected` - Returns true while the client holds an open socket: after a successful `client_connect` and before `client_close` or the peer's close was seen by `client_tick`.
- `client_send` - Sends `size` bytes from `data` to the peer, looping until every byte is out.
- `client_tick` - Reads everything the socket holds without blocking, handing each chunk to `onData`.
- `make_client` - Creates the native client behind a `Client` class instance: stores it in the instance's `_client` field and wires the instance's `onConnect`, `onDisconnect`, `onData`, `onError` and `onLog` methods as the socket's callbacks.
- `probe_local_port` - Binds a throwaway TCP socket to `port` on every interface, as a listener would, and returns the port it got: the same port when it is free, the one the system picked when `port` is 0, and -1 when the port is taken or `host` (an IPv4 address, or `localhost`) does not resolve.

## dashv

The DASHV module provides HTTP and WebSocket networking built on top of the `libhv`_ library. It exposes WebSocket client/server types, HTTP request/response handling, route registration, cookie and form-data helpers, and an HTTP client for making outbound requests.


### Enumerations

- `http_method` - HTTP request method.
- `http_status` - HTTP response status code.
- `ws_opcode` - WebSocket frame opcode.
- `ws_session_type` - WebSocket session type.

### Handled structures

- `HttpRequest` - HTTP request with URL, method, headers, body, and parameters.
- `HttpContext` - HTTP request/response context passed to route handlers.
- `HttpMessage` - Base type for HTTP messages, providing header access.
- `HttpResponse` - HTTP response with status code, headers, and body.
- `HttpResponseWriter` - HTTP response writer for streaming responses.

### Handled types

- `WebSocketClient` - Opaque handle to an outbound WebSocket connection.
- `WebSocketChannel` - Opaque handle to a server-side WebSocket client connection.
- `WebSocketServer` - Opaque handle to a running HTTP/WebSocket server.

### WebSocket client

- `close` - Closes a connected WebSocket channel.
- `destroy_web_socket_client` - Releases the client handle.
- `is_connected` - Returns `true` if the WebSocket client is currently connected.
- `make_web_socket_client` - Creates a new low-level WebSocket client bound to a daScript class instance.
- `open` - Opens a WebSocket connection to the specified URL.
- `send` - Sends a message frame on the WebSocket channel with the given opcode and fin flag.
- `tick` - Drains one batch of queued HTTP/WebSocket events and invokes the user callbacks.

### WebSocket server lifecycle

- `destroy_web_socket_server` - Releases the server handle.
- `make_web_socket_server` - Creates a new low-level WebSocket/HTTP server on the given port, optionally with TLS.
- `set_bind_host` - Selects the WebSocket server interface to bind before start.
- `start` - Starts the server's background workers.
- `stop` - Stops the server and drains pending events.

### Handle operations

- `WebSocketChannel!=` - Handle inequality: returns true if the two handles refer to different channels (or at least one is null).
- `WebSocketChannel==` - Handle equality: returns true if both refer to the same channel.
- `WebSocketClient!=` - Handle inequality: returns true if the two handles refer to different clients (or at least one is null).
- `WebSocketClient==` - Handle equality: returns true if both refer to the same client.
- `WebSocketServer!=` - Handle inequality: returns true if the two handles refer to different servers (or at least one is null).
- `WebSocketServer==` - Handle equality: returns true if both refer to the same live server.
- `is_alive` - Returns true if the channel handle still refers to a live peer connection.

### HTTP route registration

- `ANY` - Registers a handler that matches any HTTP method (GET/POST/PUT/etc) for the given URL path.
- `DELETE` - Registers a DELETE route handler, or performs an HTTP DELETE client request.
- `GET` - Registers a GET route handler, or performs an HTTP GET client request.
- `HEAD` - Registers an HTTP HEAD handler for the given URL path.
- `PATCH` - Registers an HTTP PATCH handler for the given URL path.
- `POST` - Registers a POST route handler, or performs an HTTP POST client request.
- `PUT` - Registers a PUT route handler, or performs an HTTP PUT client request.
- `SSE` - Registers a Server-Sent Events (SSE) streaming handler for the given URL path.
- `STREAM` - Registers a streaming HTTP route: the handler runs on the server's tick thread and drives an HttpResponseWriter to emit Server-Sent Events, chunked data, or one buffered body.

### HTTP server configuration

- `STATIC` - Registers a static-file route: serves files under dir at the URL prefix path.
- `allow_cors` - Enables cross-origin resource sharing (CORS) on all server responses.
- `set_document_root` - Sets the root directory used for serving static files.
- `set_error_page` - Sets the file served when the server returns an error response.
- `set_home_page` - Sets the file served when a request maps to a directory (e.g.
- `set_index_of` - Enables directory listing for the given directory path.

### HTTP response helpers

- `DATA` - Sends raw data as the response body with the given status code.
- `JSON` - Sends a JSON response with the given status code.
- `REDIRECT` - Sends an HTTP redirect response to the specified location.
- `SERVE_FILE` - Sends a file as the response body, setting the content type automatically.
- `TEXT_PLAIN` - Sends a plain-text response with the given status code.
- `set_content_type` - Sets the Content-Type header on a response or request.
- `set_header` - Sets a response or request header.

### HTTP client requests

- `request` - Sends an HTTP request configured via an `HttpRequest` object and invokes the block with the response.
- `request_cb` - Sends an HTTP request and invokes `on_body` with each body chunk as a string as it arrives, then calls `on_complete` with the final response.
- `status_message` - Returns the status message string for the given HTTP response.

### Message and header access

- `HttpResponse.content_length` - Returns the content length of the HTTP response body.
- `client_ip` - Peer address of the connection the request arrived on, as recorded by the server when it accepted the socket.
- `each_header` - Iterates over all headers of an HTTP message, invoking the block with each key-value pair.
- `get_header` - Returns the value of a specific HTTP header from a message.

### Request configuration

- `allow_redirect` - Enables or disables automatic following of HTTP redirects.
- `each_param` - Iterates over all query parameters of an HTTP request, invoking the block with each key-value pair.
- `get_param` - Returns the value of a query parameter from an HTTP request.
- `set_basic_auth` - Sets HTTP Basic authentication credentials on a request.
- `set_bearer_token_auth` - Sets a Bearer token authentication header on a request.
- `set_connect_timeout` - Sets the connection timeout in seconds.
- `set_param` - Sets a query parameter on an HTTP request.
- `set_timeout` - Sets the overall request timeout in seconds.

### Cookies

- `add_cookie` - Adds a cookie to a request or response, optionally with domain, path, max-age, secure, and httponly flags.
- `each_cookie` - Iterates over all cookies, invoking the block with each name-value pair.
- `get_cookie` - Returns the value of a cookie by name from a request.

### Form data

- `each_form_field` - Iterates over all form fields of a server-side request, invoking the block with name, filename, and content type.
- `get_form_data` - Returns the value of a form field from a server-side request.
- `get_url_encoded` - Returns the value of a URL-encoded form field from a server-side request.
- `save_form_file` - Saves an uploaded form file to disk.
- `set_form_data` - Sets a form field value on a multipart request.
- `set_form_file` - Attaches a file to a multipart request.
- `set_url_encoded` - Sets a URL-encoded form field on a request.

### SSE response writer

- `close_writer` - Finishes a streamed response and releases the writer, closing the connection.
- `end_headers` - Ends the response header block, sending the status line and one header (key and value), so the body can then be streamed with write_chunked.
- `is_writer_connected` - Returns true while the writer's client connection is still open (false for null, unknown, or released writers).
- `release_writer` - Releases a chunked/SSE response writer previously obtained from the server.
- `respond` - Sends a complete buffered response (status code, content type, and whole body) through the streaming response writer.
- `set_writer_keepalive_timeout` - Sets how long a retained streaming response writer may remain inactive before it expires.
- `sse_event` - Writes one Server-Sent Events frame (the data payload plus an optional event name; an empty name emits an unnamed event) to the streaming response writer.
- `write_chunked` - Writes one chunk of body data to the streaming response writer using chunked transfer encoding.

## openai_common

A pure-daslang client for OpenAI-compatible REST APIs — OpenAI itself, plus any server that speaks the same surface (Ollama, OpenRouter, Kokoro, LM Studio, vLLM, …). Built on `dashv` for HTTP and `daslib/json_boost` for serialization.


### Structures

- `OpenAIClient` - Connection + auth config.
- `OpenAIError` - Unified error.
- `Usage` - Token accounting returned with most responses.
- `HttpOutcome` - Internal transport result of a JSON request (`post_json` / `get_json`).
- `BytesOutcome` - Internal transport result of a binary request (`post_for_bytes`).
- `ResponseFormat` - Chat `response_format`: `_type` is "text" | "json_object" | "json_schema".
- `FunctionDef` - A tool's function declaration.
- `Tool` - A tool the model may call.
- `ToolCallFunction` - The function a tool call targets.
- `ToolCall` - A tool call requested by the model.
- `ChatMessage` - A single chat message.
- `ChatCompletionRequest` - Request body for `/chat/completions`.
- `ChatChoice` - One completion choice.
- `ChatCompletionResponse` - Response body for a non-streaming `/chat/completions` call.
- `ChatResult` - Outcome of `chat`: when `ok` is true `response` is valid, else inspect `error`.
- `ToolCallFunctionDelta` - Incremental function fields in a streamed tool-call delta.
- `ToolCallDelta` - One streamed tool-call fragment.
- `ChatMessageDelta` - Incremental message fields in a streamed chunk (object == "chat.completion.chunk").
- `ChunkChoice` - One choice within a streamed chunk.
- `ChatCompletionChunk` - One streamed delta frame.
- `ChatStreamResult` - Outcome of `chat_stream`: `content` is the full accumulated assistant text.
- `EmbeddingRequest` - Request body for `/embeddings`.
- `EmbeddingUsage` - Token accounting for an embeddings request.
- `EmbeddingData` - One input's embedding.
- `EmbeddingResponse` - Response body for `/embeddings`.
- `EmbeddingResult` - Outcome of `embeddings`: when `ok` is true `response` is valid, else inspect `error`.
- `Model` - A single model entry.
- `ModelsResult` - Outcome of `list_models`: `models` valid when `ok` is true.
- `ModelResult` - Outcome of `retrieve_model`: `model` valid when `ok` is true.
- `CompletionRequest` - Request body for the legacy `/completions` endpoint.
- `CompletionChoice` - One generated completion.
- `CompletionResponse` - Response body for `/completions`.
- `CompletionResult` - Outcome of `completions`: `response` valid when `ok` is true.
- `SpeechRequest` - Request body for `/audio/speech`.
- `SpeechResult` - Outcome of `speech`: `audio` holds the raw bytes when `ok` is true.
- `TranscriptionResult` - Outcome of `transcribe` / `translate`: `text` valid when `ok` is true.
- `ModerationRequest` - Request body for `/moderations`.
- `ModerationCategoryResult` - One moderation verdict.
- `ModerationResponse` - Response body for `/moderations`.
- `ModerationResult` - Outcome of `moderations`: `response` valid when `ok` is true.
- `ImageRequest` - Request body for `/images/generations`.
- `ImageData` - One generated image.
- `ImageResponse` - Response body for `/images/generations`.
- `ImageResult` - Outcome of `generate_image`: `response` valid when `ok` is true.

### Client and transport

- `configure_request` - Applies auth (Bearer), the optional OpenAI-Organization header, and the client timeout to a manually-built request — shared by every transport path so they behave identically.
- `get_and_decode` - GETs `path`, then decodes a 2xx response into `response` (see `post_and_decode`).
- `get_json` - GETs `path` (with auth + org header + timeout) and returns the raw outcome.
- `openai_client` - Builds a client for the given base URL (no trailing slash) and optional API key.
- `parse_error_body` - Decodes an OpenAI `{"error":{...}}` body into an `OpenAIError` (falls back to a plain HTTP error).
- `percent_encode` - Percent-encodes a string for safe use as a single URL path segment (e.g.
- `post_and_decode` - POSTs a JSON body, then decodes a 2xx response into `response`.
- `post_for_bytes` - POSTs a JSON body and returns the raw response bytes (e.g.
- `post_json` - POSTs a JSON body and returns the raw outcome (status + body, or error).

### Chat completions

- `chat` - Sends a non-streaming `/chat/completions` request and decodes the response.
- `chat_stream` - Streams a `/chat/completions` response, invoking `on_delta` with each text increment as it arrives.
- `chat_text` - Convenience: one user turn, returns the assistant's text ("" on error).
- `json_object_format` - Returns the `response_format` value for JSON mode (assign to `req.response_format`).

### Embeddings

- `embed` - Convenience: embed a single string, returns the vector (empty on error).
- `embeddings` - Sends an `/embeddings` request and decodes the response.

### Models

- `list_models` - Lists available models (GET /models).
- `retrieve_model` - Retrieves a single model by id (GET /models/{id}).

### Legacy completions

- `complete` - Convenience: completes `prompt`, returns the text ("" on error).
- `completions` - Sends a legacy `/completions` request and decodes the response.

### Audio (TTS and STT)

- `speak` - Convenience: synthesize `text` with `voice`, returns audio bytes (empty on error).
- `speech` - Synthesizes speech (POST /audio/speech); returns the raw audio bytes.
- `transcribe` - Transcribes the audio file at `file_path` (POST /audio/transcriptions).
- `translate` - Translates the audio file at `file_path` into English (POST /audio/translations).

### Moderations

- `moderations` - Classifies text for policy violations (POST /moderations).

### Image generation

- `generate_image` - Generates images from a prompt (POST /images/generations).

### Vision

- `chat_vision` - One-shot vision request: a single user turn with text plus an image (http(s) URL or a `data:` URL).
- `vision_request_body` - Builds the JSON body for a one-shot vision request (a user turn with text + one image).

## stbimage

Low-level image I/O and resizing bindings for stb_image, stb_image_write, and stb_image_resize2.


### Enumerations

- `stbir_datatype` - Pixel data type for resize operations.
- `stbir_edge` - Edge handling mode for resize operations.
- `stbir_filter` - Resampling filter for resize operations.
- `stbir_pixel_layout` - Pixel channel layout for resize operations.

### Image loading

- `stbi_load` - Load an image from file, returning pixel data as uint8.
- `stbi_load_16` - Load an image from file as 16-bit unsigned integer data.
- `stbi_load_16_from_memory` - Load an image from a memory buffer as 16-bit unsigned integer data.
- `stbi_load_from_memory` - Load an image from a memory buffer as uint8.
- `stbi_load_gif_from_memory` - Load all frames of an animated GIF from a memory buffer.
- `stbi_loadf` - Load an image from file as HDR float data.
- `stbi_loadf_from_memory` - Load an image from a memory buffer as HDR float data.

### Image info

- `stbi_info` - Query image dimensions and channel count from a file without loading pixel data.
- `stbi_info_from_memory` - Query image dimensions and channel count from a memory buffer without decoding.
- `stbi_is_16_bit` - Check if a file contains 16-bit image data.
- `stbi_is_16_bit_from_memory` - Check if a memory buffer contains 16-bit image data.
- `stbi_is_hdr` - Check if a file contains HDR image data.
- `stbi_is_hdr_from_memory` - Check if a memory buffer contains HDR image data.

### Memory management

- `stbi_failure_reason` - Return a description of the last load failure.
- `stbi_image_free` - Free pixel data allocated by stbi_load and related functions.

### Load settings

- `stbi_convert_iphone_png_to_rgb` - Set whether to convert iPhone PNG format to standard RGB.
- `stbi_set_flip_vertically_on_load` - Set whether loaded images should be flipped vertically.
- `stbi_set_unpremultiply_on_load` - Set whether to unpremultiply alpha on load (iPhone PNG).

### HDR gamma and scale

- `stbi_hdr_to_ldr_gamma` - Set gamma value for HDR to LDR conversion.
- `stbi_hdr_to_ldr_scale` - Set scale factor for HDR to LDR conversion.
- `stbi_ldr_to_hdr_gamma` - Set gamma value for LDR to HDR conversion.
- `stbi_ldr_to_hdr_scale` - Set scale factor for LDR to HDR conversion.

### File writing

- `stbi_write_bmp` - Write image data to a BMP file.
- `stbi_write_hdr` - Write HDR float image data to an HDR file.
- `stbi_write_jpg` - Write image data to a JPEG file with specified quality.
- `stbi_write_png` - Write image data to a PNG file.
- `stbi_write_tga` - Write image data to a TGA file.

### Write to memory

- `stbi_write_bmp_to_memory` - Encode image data as BMP to a memory buffer.
- `stbi_write_jpg_to_memory` - Encode image data as JPEG to a memory buffer.
- `stbi_write_png_to_memory` - Encode image data as PNG to a memory buffer.
- `stbi_write_tga_to_memory` - Encode image data as TGA to a memory buffer.

### Write settings

- `stbi_flip_vertically_on_write` - Set whether to flip images vertically when writing.
- `stbi_write_get_force_png_filter` - Get current PNG filter mode setting.
- `stbi_write_get_png_compression_level` - Get current PNG compression level.
- `stbi_write_get_tga_with_rle` - Get current TGA RLE compression setting.
- `stbi_write_set_force_png_filter` - Force a specific PNG filter mode (-1 for auto).
- `stbi_write_set_png_compression_level` - Set PNG compression level (default 8).
- `stbi_write_set_tga_with_rle` - Set whether TGA files use RLE compression.

### Image resizing

- `stbir_resize` - Resize image with full control over data type, layout, edge mode, and filter.
- `stbir_resize_float_linear` - Resize a float image in linear color space.
- `stbir_resize_uint8_linear` - Resize a uint8 image in linear color space.
- `stbir_resize_uint8_srgb` - Resize a uint8 image with sRGB gamma correction.

### Texture block compression

- `stb_compress_blocks` - Compress one tightly packed RGBA8 image into GPU texture blocks, replicating edge pixels when a dimension is not divisible by four.

### Animated PNG (APNG) writer

- `stbi_apng_begin` - Begin streaming APNG encoding to `filename`.
- `stbi_apng_dropped` - Return the running count of frames dropped because the encoder thread's bounded queue was full.
- `stbi_apng_end` - Finalize the APNG file: drain the encoder thread, backpatch the `acTL` frame count, write `IEND`, and free the writer.
- `stbi_apng_frame` - Queue one frame on `writer`.

## meshoptimizer

Checked array bindings for meshoptimizer 1.2 vertex remapping, vertex-cache and vertex-fetch optimization, and vertex and index buffer codecs. The API accepts triangle-list `uint` indices and packed vertex bytes with a nonzero, 4-byte-aligned stride of at most 256 bytes.


### Vertex remapping

- `meshopt_generate_remap` - Build a vertex remap table from triangle-list `indices` and packed `vertices`, returning the number of unique vertices.
- `meshopt_remap_indices` - Apply a vertex remap table to triangle-list `indices` and resize `output` to the index count.
- `meshopt_remap_vertices` - Apply `remap` to packed `vertices` and resize `output` to `count * stride` bytes.

### Mesh optimization

- `meshopt_optimize_cache` - Reorder triangle-list `indices` in place to improve post-transform vertex-cache locality while preserving each triangle's orientation.
- `meshopt_optimize_fetch` - Reorder `indices` and packed `vertices` in place for sequential vertex fetch, discard unreferenced vertices, and return the resulting vertex count.

### Vertex codec

- `meshopt_decode_vertices` - Decode `count` packed vertex records of `stride` bytes into `output` and return true on success.
- `meshopt_encode_vertices` - Encode packed vertex records into the meshoptimizer vertex codec and resize `output` to the encoded byte count.

### Index codec

- `meshopt_decode_indices` - Decode exactly `count` triangle-list indices into `output` and return true on success.
- `meshopt_encode_indices` - Encode triangle-list `indices` with the meshoptimizer index codec and resize `output` to the encoded byte count.

## raster

SIMD-accelerated pixel operations — gather, scatter, copy, channel conversion, and BPC conversion.


### Gather

- `gather` - Gather pixels from source using an index array into destination.

### Scatter

- `scatter` - Scatter pixels from source to destination using an index array.

### Scatter with mask

- `scatter_neq_mask` - Scatter pixels to destination where mask value differs from pixel value.

### Store with mask

- `store_neq_mask` - Store a single pixel at indexed positions where mask differs.

### Gather-scatter

- `gather_scatter` - Gather pixels from source and scatter to destination using index arrays.

### Gather-scatter with mask

- `gather_scatter_neq_mask` - Gather-scatter pixels where mask value differs from gathered pixel.

### Gather-store with mask

- `gather_store_neq_mask` - Gather from source and store where mask differs, using a single value.

### Gather-store with stride

- `gather_store_stride` - Gather pixels with a stride offset between source elements.

### Byte gather-store

- `u8x4_gather_store` - Gather 4-byte pixels and store to sequential destination.

### Copy reversed

- `rast_copy_reversed_1` - Copy pixel rows in reverse order (vertical flip) for 1-byte pixels.
- `rast_copy_reversed_12` - Copy pixel rows in reverse order (vertical flip) for 12-byte pixels.
- `rast_copy_reversed_16` - Copy pixel rows in reverse order (vertical flip) for 16-byte pixels.
- `rast_copy_reversed_2` - Copy pixel rows in reverse order (vertical flip) for 2-byte pixels.
- `rast_copy_reversed_3` - Copy pixel rows in reverse order (vertical flip) for 3-byte pixels.
- `rast_copy_reversed_4` - Copy pixel rows in reverse order (vertical flip) for 4-byte pixels.
- `rast_copy_reversed_6` - Copy pixel rows in reverse order (vertical flip) for 6-byte pixels.
- `rast_copy_reversed_8` - Copy pixel rows in reverse order (vertical flip) for 8-byte pixels.

### Copy rect

- `rast_copy_rect_1` - Copy a rectangular region between pixel buffers for 1-byte pixels.
- `rast_copy_rect_12` - Copy a rectangular region between pixel buffers for 12-byte pixels.
- `rast_copy_rect_16` - Copy a rectangular region between pixel buffers for 16-byte pixels.
- `rast_copy_rect_2` - Copy a rectangular region between pixel buffers for 2-byte pixels.
- `rast_copy_rect_3` - Copy a rectangular region between pixel buffers for 3-byte pixels.
- `rast_copy_rect_4` - Copy a rectangular region between pixel buffers for 4-byte pixels.
- `rast_copy_rect_6` - Copy a rectangular region between pixel buffers for 6-byte pixels.
- `rast_copy_rect_8` - Copy a rectangular region between pixel buffers for 8-byte pixels.

### Channel conversion

- `rast_convert_channels_f32` - Convert between channel counts for float pixel data.
- `rast_convert_channels_u16` - Convert between channel counts for uint16 pixel data.
- `rast_convert_channels_u8` - Convert between channel counts for uint8 pixel data.

### BPC conversion

- `rast_convert_f32_to_u16` - Convert pixel data from float (0.0-1.0) to uint16 (0-65535) with clamping.
- `rast_convert_f32_to_u8` - Convert pixel data from float (0.0-1.0) to uint8 (0-255) with clamping.
- `rast_convert_u16_to_f32` - Convert pixel data from uint16 (0-65535) to float (0.0-1.0).
- `rast_convert_u16_to_u8` - Convert pixel data from uint16 (0-65535) to uint8 (0-255).
- `rast_convert_u8_to_f32` - Convert pixel data from uint8 (0-255) to float (0.0-1.0).
- `rast_convert_u8_to_u16` - Convert pixel data from uint8 (0-255) to uint16 (0-65535).

### Span raster

- `rast_hspan_masked_solid_u8` - Rasterize a horizontal span of solid color with per-pixel mask.
- `rast_hspan_masked_u8` - Rasterize a horizontal span with per-pixel mask.
- `rast_hspan_u8` - Rasterize a horizontal span of uint8 pixels.

### Fill rect

- `rast_fill_rect_1` - Fill a rectangle in a 1-channel uint8 image with a solid byte value.
- `rast_fill_rect_4` - Fill a rectangle in a 4-channel RGBA image with a packed uint32 color value.

### Alpha blit

- `rast_blit_alpha` - Alpha-blend a 1-channel source onto a 4-channel RGBA destination using SIMD.

## stbtruetype

Low-level TrueType font bindings for stb_truetype. Provides font loading, glyph packing into atlases, font metrics, codepoint metrics, and bitmap rasterization. Use `stbimage_ttf` for a high-level API.


### Constants

- `STBTT_vmove` - Vertex type constant for a move-to command (start of a new contour).
- `STBTT_vline` - Vertex type constant for a line-to command (straight line segment).
- `STBTT_vcurve` - Vertex type constant for a quadratic Bezier curve segment.
- `STBTT_vcubic` - Vertex type constant for a cubic Bezier curve segment.

### Handled structures

- `stbtt_aligned_quad` - Screen-space quad with texture coordinates, produced by GetPackedQuad.
- `stbtt_vertex_das` - A glyph outline vertex with endpoint coordinates, control points, and a vertex type.
- `stbtt_fontinfo` - Font info structure initialized by InitFont.
- `stbtt_packedchar` - Packed character data produced by PackFontRange.
- `stbtt_pack_context` - Opaque packing context used by PackBegin, PackFontRange, and PackEnd.

### Font loading

- `stbtt_GetFontOffsetForIndex` - Return the byte offset of the Nth font in a collection.
- `stbtt_GetNumberOfFonts` - Return the number of fonts in a TrueType collection (.ttc) file.
- `stbtt_InitFont` - Initialize a font info structure from raw font data at the given byte offset.

### Font packing

- `stbtt_GetPackedQuad` - Get the screen-space quad and texture coordinates for a packed character.
- `stbtt_PackBegin` - Initialize a packing context for packing font glyphs into an atlas bitmap.
- `stbtt_PackEnd` - Clean up a packing context after all fonts have been packed.
- `stbtt_PackFontRange` - Pack a range of characters from a font into the atlas at the specified pixel height.
- `stbtt_PackSetOversampling` - Set horizontal and vertical oversampling for subsequent PackFontRange calls.
- `stbtt_PackSetSkipMissingCodepoints` - Control whether missing codepoints produce warning rectangles (default) or are silently skipped during packing.

### Font metrics

- `stbtt_GetFontBoundingBox` - Get the bounding box of all glyphs in the font in unscaled coordinates.
- `stbtt_GetFontVMetrics` - Get vertical metrics for the font: ascent, descent, and line gap in unscaled coordinates.
- `stbtt_ScaleForMappingEmToPixels` - Compute the scale factor to map font design units (em) to the given pixel size.
- `stbtt_ScaleForPixelHeight` - Compute the scale factor to produce a font whose ascent-to-descent distance equals the given pixel height.

### Codepoint metrics

- `stbtt_FindGlyphIndex` - Return the glyph index for a Unicode codepoint.
- `stbtt_GetCodepointBox` - Get the bounding box of a codepoint's glyph in unscaled coordinates.
- `stbtt_GetCodepointHMetrics` - Get horizontal metrics for a codepoint: advance width and left side bearing in unscaled coordinates.
- `stbtt_GetCodepointKernAdvance` - Get the kerning adjustment between two codepoints in unscaled coordinates.

### Codepoint bitmaps

- `stbtt_FreeBitmap` - Free a bitmap allocated by GetCodepointBitmap or GetCodepointBitmapSubpixel.
- `stbtt_GetCodepointBitmap` - Render a codepoint glyph into a newly allocated 1-channel bitmap at the given scale.
- `stbtt_GetCodepointBitmapBox` - Get the bounding box of the bitmap that would be generated for a codepoint at the given scale.
- `stbtt_GetCodepointBitmapBoxSubpixel` - Get the bounding box of the bitmap for a codepoint at the given scale with sub-pixel offsets.
- `stbtt_GetCodepointBitmapSubpixel` - Render a codepoint glyph bitmap with sub-pixel positioning at the given scale and fractional offsets.
- `stbtt_MakeCodepointBitmap` - Render a codepoint glyph into a pre-allocated buffer with the given dimensions and stride.
- `stbtt_MakeCodepointBitmapSubpixel` - Render a codepoint glyph into a pre-allocated buffer with sub-pixel positioning.

### Glyph shape

- `stbtt_GetCodepointShape` - Iterates over the vertices of a glyph outline for the given Unicode codepoint, invoking the block for each vertex.
- `stbtt_GetCodepointShape_count` - Returns the number of vertices in the glyph outline for the given Unicode codepoint.

## uriparser

The URIPARSER module provides URI parsing and manipulation based on the uriparser library. It supports parsing URI strings into components (scheme, host, path, query, fragment), normalization, resolution of relative URIs, and GUID generation.


### Handled structures

- `UriHostDataA` - Host data portion of the URI (IPv4 or IPv6, or some future data).
- `UriIp4Struct` - IPv4 address portion of the URI.
- `UriUriA` - URI base class, contains all URI data.
- `UriPathSegmentStructA` - Part of the path portion of the URI.
- `UriTextRangeA` - Range of text in the URI.
- `Uri.empty` - Returns `true` if the `Uri` object contains no URI data.
- `Uri.size` - Returns the string length of the URI.
- `Uri.status` - Returns the parse status code of the `Uri` object.
- `Uri` - URI implementation.
- `UriIp6Struct` - IPv6 address portion of the URI.

### Initialization and finalization

- `clone` - Creates a deep copy of the given `Uri` object.
- `finalize` - Releases all resources held by the `Uri` object.
- `using` - Creates a scoped `Uri` variable that is automatically finalized at end of block.

### Escape and unescape

- `escape_uri` - Percent-encodes reserved and special characters in the URI string.
- `unescape_uri` - Decodes percent-encoded characters in the URI string.

### Uri manipulations

- `add_base_uri` - Resolves a relative URI against a base URI, producing an absolute URI.
- `normalize` - Normalizes a `Uri` in place, removing redundant `/`, `.`, and `..` path segments.
- `normalize_uri` - Returns a normalized copy of the URI string with redundant `/`, `.`, and `..` segments removed.
- `remove_base_uri` - Computes a relative URI by removing the base URI prefix from an absolute URI.
- `string` - Converts a `Uri` object to its string representation.
- `strip_uri` - Removes the query string and fragment from the URI.
- `uri_for_each_query_kv` - Iterates over each key-value pair in the URI's query string, invoking a block for each.

### File name conversions

- `file_name_to_uri` - Converts a platform-native file path to a `file://` URI string.
- `to_file_name` - Converts a `Uri` to a platform-native file path.
- `to_unix_file_name` - Converts a `Uri` to a Unix-style file path.
- `to_windows_file_name` - Converts a `Uri` to a Windows-style file path.
- `unix_file_name_to_uri` - Converts a Unix-style file path to a `file://` URI string.
- `uri_from_file_name` - Converts a platform-native file path to a `file://` URI string.
- `uri_from_unix_file_name` - Converts a Unix-style file path to a `file://` URI string.
- `uri_from_windows_file_name` - Converts a Windows-style file path to a `file://` URI string.
- `uri_to_file_name` - Converts a URI string to a platform-native file path.
- `uri_to_unix_file_name` - Converts a URI string to a Unix-style file path.
- `uri_to_windows_file_name` - Converts a URI string to a Windows-style file path.
- `windows_file_name_to_uri` - Converts a Windows-style file path to a `file://` URI string.

### GUID

- `make_new_guid` - Generates a new random GUID/UUID string.

## pugixml

The PUGIXML module provides XML parsing, navigation, manipulation, and XPath query support built on top of the `pugixml`_ C++ library. It exposes document loading/saving, DOM-style node and attribute access, text content helpers, and a full XPath 1.0 evaluation engine.


### Constants

- `parse_minimal` - Minimal parsing mode: only elements and PCDATA are parsed.
- `parse_pi` - Parse processing instructions (`<?...?>`).
- `parse_comments` - Parse comments (`<!--...-->`).
- `parse_cdata` - Parse CDATA sections (`<![CDATA[...]]>`).
- `parse_ws_pcdata` - Parse whitespace-only PCDATA nodes.
- `parse_escapes` - Parse character and entity references (`&amp;`, `&#123;`, etc.).
- `parse_eol` - Normalize line endings to `\n`.
- `parse_wconv_attribute` - Normalize whitespace in attribute values (convert tabs/newlines to spaces).
- `parse_wnorm_attribute` - Normalize and collapse whitespace in attribute values.
- `parse_declaration` - Parse XML declarations (`<?xml ...?>`).
- `parse_doctype` - Parse DOCTYPE declarations.
- `parse_ws_pcdata_single` - Parse whitespace-only PCDATA as a single node.
- `parse_trim_pcdata` - Trim leading and trailing whitespace from PCDATA.
- `parse_fragment` - Parse as document fragment (allows multiple root elements).
- `parse_embed_pcdata` - Embed PCDATA value in the element node instead of creating a child.
- `parse_merge_pcdata` - Merge adjacent PCDATA nodes into one.
- `parse_default` - Default parsing flags: parse_cdata | parse_escapes | parse_wconv_attribute | parse_eol.
- `parse_full` - Full parsing: all possible constructs are parsed.
- `format_indent` - Indent the output nodes according to tree depth.
- `format_write_bom` - Write an encoding byte-order mark (BOM) at the start.
- `format_raw` - Raw output: no indentation or newlines.
- `format_no_declaration` - Omit the XML declaration (`<?xml ...?>`) from output.
- `format_no_escapes` - Do not escape special characters in output.
- `format_save_file_text` - Use platform-native line endings when saving to file.
- `format_indent_attributes` - Indent attributes on separate lines.
- `format_no_empty_element_tags` - Always use `<tag></tag>` instead of `<tag/>` for empty elements.
- `format_skip_control_chars` - Skip control characters during serialization.
- `format_attribute_single_quote` - Use single quotes for attribute values.
- `format_default` - Default formatting flags: format_indent.

### Enumerations

- `xml_encoding` - Character encoding used for XML input/output operations.
- `xml_node_type` - DOM node type identifying the kind of XML node.
- `xml_parse_status` - Parsing result status codes indicating success or the kind of error encountered.
- `xpath_value_type` - XPath expression return type.

### Handled structures

- `xml_document` - Owns the entire DOM tree.
- `xml_attribute.name` - Returns the attribute name.
- `xml_attribute.value` - Returns the attribute value as a string.
- `xml_attribute.empty` - Returns true if the attribute handle is empty (null).
- `xml_attribute` - Handle to a single XML attribute (name-value pair) on an element node.
- `xpath_variable_set` - A set of named XPath variables (bool, number, string) that can be bound to a compiled xpath_query.
- `xpath_node_set` - An ordered collection of xpath_node results returned by XPath evaluation.
- `xml_text.empty` - Returns true if the attribute handle is empty (null).
- `xml_text.get` - Returns the text content as a raw string.
- `xml_text` - Accessor for the text content of an element node.
- `xml_node.name` - Returns the attribute name.
- `xml_node.value` - Returns the attribute value as a string.
- `xml_node._type` - Returns the node type (element, pcdata, cdata, comment, etc.).
- `xml_node.empty` - Returns true if the attribute handle is empty (null).
- `xml_node` - Lightweight handle to a DOM node (element, text, comment, etc.).
- `xpath_node` - A single result from an XPath query.
- `xml_parse_result.description` - Returns a human-readable description of the parse result status.
- `xml_parse_result` - Result of a parsing operation.
- `xpath_query` - A compiled XPath 1.0 expression.

### Document operations

- `document_as_node` - Returns the document as an xml_node, allowing direct node operations on the document root.
- `load_document` - Loads an XML document from a file.
- `load_string` - Parses an XML string into the document.
- `reset` - Resets the document, removing all nodes and freeing memory.
- `save_file` - Saves the document to a file with the specified indentation, flags, and encoding.
- `save_string` - Serializes the entire document to a string with the specified formatting.
- `xml_document const?.document_element` - Returns the root element of the document (i.e.

### Node lookup

- `append_child` - Appends a new child element with the given name to the node.
- `child` - Returns the first child element with the given name, or an empty node handle if not found.
- `find_child_by_attribute` - Finds the first child element that has an attribute matching the given name and value.
- `first_element_by_path` - Navigates a slash-separated element path (e.g.
- `prepend_child` - Prepends a new child element with the given name to the node.
- `remove_child` - Removes the given child node from this node.
- `xml_node.first_child` - Returns the first child node of this element.
- `xml_node.last_child` - Returns the last child node of this element.

### Node navigation

- `next_sibling` - Returns the next sibling element with the given name, or an empty node if not found.
- `path` - Returns the absolute path of the node from the document root, using the given delimiter.
- `previous_sibling` - Returns the previous sibling element with the given name, or an empty node if not found.
- `print_to_string` - Serializes the node (and its subtree) to an XML string with the specified formatting.
- `xml_node.next_sibling` - Returns the next sibling node in document order.
- `xml_node.parent` - Returns the parent node of this element.
- `xml_node.previous_sibling` - Returns the previous sibling node in document order.
- `xml_node.root` - Returns the root node of the document this node belongs to.
- `xpath_node.parent` - Returns the parent element of this XPath result node.

### Node mutation

- `insert_child_after` - Inserts a new child element with the given name after the specified sibling.
- `insert_child_before` - Inserts a new child element with the given name before the specified sibling.
- `remove_children` - Removes all child nodes from this node.
- `set_name` - Changes the name (tag) of the node or attribute.
- `set_value` - Sets the value of the node or attribute.

### Attribute access

- `append_attribute` - Adds a new attribute with the given name at the end of the node's attribute list.
- `attribute` - Returns the attribute with the given name, or an empty attribute handle if not found.
- `insert_attribute_after` - Inserts a new attribute with the given name after the specified attribute.
- `insert_attribute_before` - Inserts a new attribute with the given name before the specified attribute.
- `prepend_attribute` - Adds a new attribute with the given name at the beginning of the node's attribute list.
- `remove_attribute` - Removes the specified attribute handle from the node.
- `remove_attributes` - Removes all attributes from the node.
- `xml_attribute.next_attribute` - Returns the next attribute in the element's attribute list.
- `xml_attribute.previous_attribute` - Returns the previous attribute in the element's attribute list.
- `xml_node.first_attribute` - Returns the first attribute of this element node.
- `xml_node.last_attribute` - Returns the last attribute of this element node.
- `xpath_node.attribute` - Returns the attribute associated with this XPath result node, if any.

### Copy and move

- `append_copy` - Appends a deep copy of the given attribute or node as the last child.
- `append_move` - Moves the given node to become the last child of this node.
- `prepend_copy` - Prepends a deep copy of the given attribute or node as the first child.
- `prepend_move` - Moves the given node to become the first child of this node.

### Value reading

- `as_bool` - Returns the attribute or text value as a bool, or *default_value* if conversion fails.
- `as_double` - Returns the attribute or text value as a double, or *default_value* if conversion fails.
- `as_float` - Returns the attribute or text value as a float, or *default_value* if conversion fails.
- `as_int` - Returns the attribute value as an int, or *default_value* if conversion fails.
- `as_int64` - Returns the text value as a 64-bit signed integer, or *default_value* if conversion fails.
- `as_string` - Returns the attribute or text value as a string, or *default_value* if empty.
- `as_uint` - Returns the attribute or text value as a uint, or *default_value* if conversion fails.
- `as_uint64` - Returns the text value as a 64-bit unsigned integer, or *default_value* if conversion fails.
- `child_value` - Returns the text content of the first child element with the given name.
- `xml_node.child_value` - Returns the text content of the first PCDATA/CDATA child of this node.
- `xml_node.text` - Returns an xml_text accessor for the text content of this element.
- `xml_text.data` - Returns the data node that holds the actual character data for this xml_text.

### Value writing

- `set` - Sets the text content or XPath variable value.

### XPath compilation and evaluation

- `evaluate_boolean` - Evaluates the compiled XPath query against the given node and returns a bool result.
- `evaluate_node` - Evaluates the compiled XPath query and returns the first matching xpath_node.
- `evaluate_node_set` - Evaluates the compiled XPath query and returns all matching nodes as an xpath_node_set.
- `evaluate_number` - Evaluates the compiled XPath query against the given node and returns a numeric result.
- `evaluate_string` - Evaluates the compiled XPath query against the given node and returns a string result.
- `xpath_compile` - Compiles an XPath expression string into an xpath_query.
- `xpath_query const?.result_description` - Returns a human-readable error description if the XPath query failed to compile.
- `xpath_query const?.result_offset` - Returns the character offset in the query string where the compilation error occurred.
- `xpath_query const?.return_type` - Returns the XPath result type (node_set, number, string, or boolean).

### XPath selection

- `select_node` - Selects the first node matching the XPath query string or compiled query.
- `select_nodes` - Selects all nodes matching the XPath query and returns them as an xpath_node_set.
- `xpath_node.node` - Returns the xml_node associated with this XPath result, if any.

### XPath node set operations

- `at` - Returns the xpath_node at the given zero-based index in the node set.
- `sort` - Sorts the xpath_node_set in document order (or reverse document order if *reverse* is true).
- `xpath_node_set const?.empty` - Returns true if the xpath_node_set contains no results.
- `xpath_node_set const?.first` - Returns the first xpath_node in the set (in document order).
- `xpath_node_set const?.size` - Returns the number of xpath_node entries in the node set.

### Construction and RAII

- `using` - Constructs an xml_document, passes it to the block, then destroys it automatically.

### Handle validity and comparison

- `xml_attribute!=` - Returns true if two xml_attribute handles refer to different attributes.
- `xml_attribute.hash_value` - Returns a hash value for the attribute handle, usable as a table key.
- `xml_attribute.ok` - Returns true if the attribute handle is valid (non-null).
- `xml_attribute==` - Returns true if two xml_attribute handles refer to the same attribute.
- `xml_node!=` - Returns true if two xml_node handles refer to different DOM nodes.
- `xml_node.hash_value` - Returns a hash value for the node handle, usable as a table key.
- `xml_node.offset_debug` - Returns the byte offset of this node in the original parsed XML source.
- `xml_node.ok` - Returns true if the node handle is valid (non-null).
- `xml_node==` - Returns true if two xml_node handles refer to the same DOM node.
- `xml_text.ok` - Returns true if the text handle is valid (non-null).
- `xpath_node.ok` - Returns true if the xpath_node result is valid (non-null).
- `xpath_query const?.ok` - Returns true if the XPath query compiled successfully.

## audio

Module audio


### Constants

- `MA_SAMPLE_RATE` - Audio output sample rate in Hz (default 48000, configured at compile time via CMake).

### Enumerations

- `ConvReverbQuality` - Quality / cost tier for the convolution reverb, chosen at conv_reverb_init.
- `I3DL2Preset` - I3DL2 reverb environment preset.
- `ma_channel_mix_mode` - Channel mixing strategy when converting between channel counts.
- `ma_dither_mode` - Dithering mode for format conversion.
- `ma_format` - Audio sample format.
- `ma_resample_algorithm` - Sample rate conversion algorithm.
- `ma_result` - Miniaudio result code.

### Handled structures

- `ma_chorus` - Stereo chorus processor with 4 modulated delay taps.
- `ma_bitcrush` - Stereo bitcrusher with bit-depth reduction and sample-and-hold decimation.
- `ma_compressor` - Feed-forward compressor with envelope follower and soft knee.
- `I3DL2ReverbProperties` - I3DL2 reverb parameters.
- `ma_volume_mixer` - Volume and pan processor with fade support.
- `ma_tremolo` - Sine-LFO stereo tremolo (amplitude modulation).
- `ma_sf2_biquad` - SF2 biquad resonant filter (transposed direct form II).
- `ma_decoder_config` - Audio decoder configuration.
- `I3DL2Reverb` - I3DL2 reverb processor instance.
- `ma_delay` - Miniaudio stereo delay effect with an internal ring buffer for storing delayed samples.
- `ma_resampler` - Sample rate converter instance.
- `ma_channel_converter_config` - Channel converter configuration.
- `ma_sf2_envelope` - SF2 ADSR envelope generator.
- `ma_bandpass` - Stereo biquad bandpass filter state.
- `ma_djfilter` - DJ-style one-knob filter sweeping from low-pass through bypass to high-pass.
- `ma_channel_converter` - Channel layout converter instance (e.g.
- `ConvolutionReverb` - Partitioned FFT convolution reverb with synthetic impulse response.
- `ma_chorus_config` - Chorus effect configuration.
- `ma_resampler_config` - Sample rate converter configuration.
- `ma_decoder` - Audio decoder instance.
- `ma_hrtf` - HRTF binaural 3D audio processor using CIPIC database.
- `ma_sf2_voice` - SF2 voice -- sample playback with pitch, envelopes, filter, and LFO modulation.
- `ma_limiter` - Look-ahead brick-wall limiter.
- `ma_phaser` - Four-stage all-pass phaser with resonant feedback and triangle LFO.
- `ma_waveshaper` - tanh-based stereo waveshaper for soft saturation.

### Audio device

- `audio_is_single_threaded` - Returns whether this audio system came up single-threaded — the device callback (and audio command processing) runs on the main thread with no separate audio thread.
- `mixer_context` - Get the audio mixer thread's context.
- `sound_finalize` - Shut down the audio device and release resources.
- `sound_initalize` - Initialize the audio device with a mixer callback.
- `sound_playback_underrun_frames` - Number of output frames the device played as silence because the mixer had not refilled the ring in time.
- `sound_set_null_device` - Forces the audio system to use miniaudio's null backend — a timer-driven playback device with no real hardware — on the next audio_system_create / with_audio_system.

### Capture

- `sound_is_recording` - True while a capture device is open and recording.
- `sound_record_available` - Number of captured frames currently buffered and waiting to be read.
- `sound_record_device_count` - Enumerate the available capture (microphone) devices and return how many there are.
- `sound_record_device_is_default` - True if the capture device at the given index is the system default input device.
- `sound_record_device_name` - Human-readable name of the capture device at the given index, from the most recent sound_record_device_count enumeration.
- `sound_record_overflow_frames` - Number of frames the audio thread dropped because the ring buffer was full (the read loop fell behind).
- `sound_record_read` - Drain buffered capture frames into the given array, up to its length, and return the number of frames read.
- `sound_record_start` - Open the capture (microphone) device and begin recording interleaved float samples into a ring buffer.
- `sound_record_stop` - Stop recording and close the capture device.

### Decoder

- `ma_decoder_config_init` - Create a decoder config with the specified output format, channels, and sample rate.
- `ma_decoder_config_init_default` - Create a decoder config with default settings (auto-detect format).
- `ma_decoder_get_available_frames` - Get the number of frames available for reading.
- `ma_decoder_get_cursor_in_pcm_frames` - Get the current read position in PCM frames.
- `ma_decoder_get_length_in_pcm_frames` - Get the total length of the audio in PCM frames.
- `ma_decoder_init_file` - Initialize a decoder from an audio file path.
- `ma_decoder_init_memory` - Initialize a decoder from an in-memory audio buffer.
- `ma_decoder_read_pcm_frames` - Read PCM frames from the decoder into a buffer.
- `ma_decoder_seek_to_pcm_frame` - Seek to a specific PCM frame position.
- `ma_decoder_uninit` - Uninitialize a decoder and release its resources.

### Resampler

- `ma_resampler_config_init` - Create a resampler config with format, channels, input rate, output rate, and algorithm.
- `ma_resampler_disable_linear_filtering` - Disable linear interpolation filtering on the resampler config.
- `ma_resampler_get_expected_output_frame_count` - Get how many output frames will be produced from the given input frames.
- `ma_resampler_get_input_latency` - Get the input latency in frames.
- `ma_resampler_get_output_latency` - Get the output latency in frames.
- `ma_resampler_get_required_input_frame_count` - Get how many input frames are needed to produce the given output frames.
- `ma_resampler_init` - Initialize a resampler from a config.
- `ma_resampler_process_pcm_frames` - Process PCM frames through the resampler, converting between sample rates.
- `ma_resampler_set_rate` - Change the input and output sample rates dynamically.
- `ma_resampler_set_rate_ratio` - Set the resampling ratio directly as a float.
- `ma_resampler_uninit` - Uninitialize a resampler.

### Channel converter

- `ma_channel_converter_config_init` - Create a channel converter config for converting between channel layouts.
- `ma_channel_converter_init` - Initialize a channel converter from a config.
- `ma_channel_converter_process_pcm_frames` - Convert PCM frames between channel layouts (e.g.
- `ma_channel_converter_uninit` - Uninitialize a channel converter.

### Volume mixer

- `ma_volume_mixer_init` - Initialize a volume mixer with the specified number of channels.
- `ma_volume_mixer_process_pcm_frames` - Apply volume, pan, and fading to PCM frames.
- `ma_volume_mixer_set_channels` - Set the number of channels for the volume mixer.
- `ma_volume_mixer_set_linear_pan` - Enable or disable linear panning mode on the volume mixer.
- `ma_volume_mixer_set_pan` - Set stereo pan position (-1.0 left to 1.0 right).
- `ma_volume_mixer_set_pan_immediate` - Set the stereo pan position on the volume mixer immediately without any smoothing or interpolation, where -1.0 is full left and 1.0 is full right.
- `ma_volume_mixer_set_volume` - Set the volume level (0.0 to 1.0+).
- `ma_volume_mixer_set_volume_over_time` - Set volume with a fade over the specified number of frames.
- `ma_volume_mixer_uninit` - Uninitialize a volume mixer.

### Limiter

- `ma_limiter_get_required_input_frame_count` - Get how many input frames are needed for the given output frames.
- `ma_limiter_init` - Initialize a look-ahead limiter with threshold, attack, release, and sample rate.
- `ma_limiter_init_linear` - Initialize a limiter in linear (pass-through) mode.
- `ma_limiter_process_pcm_frames` - Process PCM frames through the limiter.
- `ma_limiter_uninit` - Uninitialize a limiter.

### HRTF

- `ma_hrtf_init` - Initialize HRTF processor for binaural 3D audio at the given sample rate.
- `ma_hrtf_process_frames` - Process mono input frames through HRTF, producing stereo output with spatial positioning.
- `ma_hrtf_set_direction` - Set the sound source direction as azimuth and elevation in degrees.
- `ma_hrtf_uninit` - Uninitialize HRTF processor and release resources.

### I3DL2 Reverb

- `get_preset` - Get the I3DL2ReverbProperties for a named preset.
- `process_mono` - Process mono audio through the reverb, producing stereo wet output.
- `process_stereo` - Process stereo audio through the reverb.
- `set_properties` - Set reverb parameters from an I3DL2ReverbProperties struct.
- `set_sample_rate` - Set the reverb processor's sample rate.

### Convolution Reverb

- `conv_reverb_get_max_ir` - Returns the current maximum impulse response length in seconds.
- `conv_reverb_init` - Initialize a convolution reverb with a synthetic impulse response.
- `conv_reverb_process` - Process interleaved stereo audio through the convolution reverb.
- `conv_reverb_process_mono` - Process mono audio through the convolution reverb (left IR only).
- `conv_reverb_set_max_ir` - Set the maximum impulse response length in seconds (default 8.0).
- `conv_reverb_uninit` - Release all resources (FFT workspace, arena buffers) owned by the convolution reverb.

### Chorus

- `chorus_config_default` - Get the default chorus configuration.
- `chorus_init` - Initialize a 4-tap stereo chorus processor at the given sample rate.
- `chorus_process` - Process stereo audio through the chorus effect.
- `chorus_set_config` - Update chorus parameters (rate, depth, feedback, delay, wet mix).

### Delay

- `delay_init` - Initialize a stereo delay effect with the given sample rate, delay time in seconds, and feedback (decay) amount, allocating the internal ring buffer.
- `delay_process` - Process interleaved stereo frames through the delay effect, reading from the input buffer and writing the wet/dry mixed result to the output buffer for the specified number of frames.
- `delay_set_params` - Reinitialize the delay effect with a new delay time and feedback amount, freeing and reallocating the internal ring buffer to match the updated parameters.
- `delay_uninit` - Free the internal ring buffer and resources of a delay effect, releasing all memory allocated by delay_init.

### Per-voice effects

- `bandpass_init` - Initialize a stereo biquad bandpass filter at the given sample rate with default 1 kHz center frequency and Q of 1.
- `bandpass_process` - Process an interleaved stereo buffer in place through the bandpass filter for the given number of frames.
- `bandpass_setup` - Recompute the bandpass biquad coefficients for the given center frequency in Hz and quality factor Q.
- `bitcrush_init` - Initialize a stereo bitcrusher with bit-depth reduction and sample-and-hold decimation, both disabled by default.
- `bitcrush_process` - Process an interleaved stereo buffer in place through the bitcrusher, applying bit-depth quantization and coarse sample-rate reduction.
- `compressor_init` - Initialize a feed-forward compressor with envelope follower and soft knee at the given sample rate; starts in bypass (threshold 0 dB).
- `compressor_process` - Process an interleaved stereo buffer in place through the compressor, applying peak-detected gain reduction with the configured threshold, ratio, knee, attack, and release.
- `djfilter_init` - Initialize a DJ-style one-knob filter at the given sample rate, centered at the bypass position (0.5).
- `djfilter_process` - Process an interleaved stereo buffer in place through the DJ filter, sweeping from low-pass (position 0) through bypass (0.5) to high-pass (position 1).
- `phaser_init` - Initialize a 4-stage all-pass phaser with feedback at the given sample rate, defaulting to 1 Hz LFO rate, 0.75 depth, 1 kHz center, and 2000-cent sweep.
- `phaser_process` - Process an interleaved stereo buffer in place through the phaser, applying the LFO-swept all-pass chain with resonant feedback.
- `tremolo_init` - Initialize a sine-LFO stereo tremolo at the given sample rate, disabled by default (rate 0 Hz, full depth).
- `tremolo_process` - Process an interleaved stereo buffer in place through the tremolo, modulating amplitude with a sine LFO at the configured rate and depth.
- `waveshaper_init` - Initialize a tanh-based stereo waveshaper with drive set to zero (bypass).
- `waveshaper_process` - Process an interleaved stereo buffer in place through the waveshaper, applying gain-compensated tanh saturation proportional to drive.

### SF2 voice

- `ma_sf2_biquad_setup` - Configure the SF2 biquad filter at the given sample rate.
- `ma_sf2_biquad_setup_hpf` - Configure the SF2 biquad filter as a high-pass filter at the given normalized cutoff frequency (0.0 to 0.5 of sample rate).
- `ma_sf2_biquad_tick` - Process one sample through the SF2 biquad filter and return the filtered output value.
- `ma_sf2_envelope_init` - Initialize an SF2 ADSR envelope to default state.
- `ma_sf2_envelope_start` - Start the envelope at the given sample rate.
- `ma_sf2_voice_end_quick` - Immediately end the voice with a fast fade-out.
- `ma_sf2_voice_init` - Initialize an SF2 voice at the given sample rate.
- `ma_sf2_voice_is_finished` - Check if the voice has finished playing (all envelopes completed).
- `ma_sf2_voice_note_off` - Trigger note-off, starting the release phase of all envelopes.
- `ma_sf2_voice_render` - Render PCM frames from the voice into an output buffer.
- `ma_sf2_voice_render_send` - Render with reverb send.
- `ma_sf2_voice_render_send2` - Render with reverb and chorus sends.
- `ma_sf2_voice_render_send3` - Render SF2 voice samples into four separate output buffers (dry, reverb, chorus, and delay sends) simultaneously, applying the corresponding gain to each send.

## audio_boost

Module audio_boost


### Type aliases

- `SID` - Sound ID — a unique handle for every playing sound.
- `AudioSystemChannels`

### Constants

- `SPEED_OF_SOUND`
- `MAX_SPEED_OF_OBJECT`
- `MA_CHANNELS`
- `MA_HRTF`
- `INVALID_SID`

### Enumerations

- `AudioChannelState`
- `ReverbPreset`

### Structures

- `AudioChannelStatus`
- `AudioSystemStats`
- `Attenuation`

### Audio system lifecycle

- `audio_system_create` - Low-level: creates the audio system.
- `audio_system_finalize` - Low-level: shuts down the audio system.
- `audio_system_release_context` - Release the mixer context reference.
- `audio_user_args` - The audio knobs every app gets from its user args: `--null-audio` (run on the null backend, no device) and `--volume V` (master volume, clamped 0..1).
- `with_audio_system` - Initializes the audio system, runs the block, then shuts down.

### Sound playback

- `play_3d_sound_from_file` - plays 3D sound from file.
- `play_3d_sound_from_pcm` - plays 3D sound from PCM data.
- `play_3d_sound_from_pcm_stream` - Create a 3D PCM streaming channel.
- `play_3d_sound_loop_from_pcm` - plays 3D looping sound from PCM data.
- `play_sound_from_file` - plays sound from file.
- `play_sound_from_pcm` - plays sound from PCM data.
- `play_sound_from_pcm_stream` - Create a PCM streaming channel.
- `play_sound_loop_from_pcm` - plays looping sound from PCM data.

### Sound control

- `set_global_pause` - set global pause of sounds
- `set_global_pitch` - set global pitch of sounds
- `set_global_volume` - Set global master volume (multiplier for all currently-playing sounds).
- `set_ignore_global_volume` - Make a specific channel ignore the global master volume (used by editor preview so muting the game's master volume does not silence the asset preview).
- `set_pan` - set pan of sound
- `set_pause` - pause or unpause sound; `time` is the fade duration in seconds (default ~2ms to avoid clicks)
- `set_pitch` - set pitch of sound
- `set_playback_position` - set playback position for sound (in frames)
- `set_volume` - set volume of sound
- `stop` - stop sound

### 3D audio

- `set_head_position` - set head position for 3D sound
- `set_position` - set sound position for 3D sound

### HRTF

- `hrtf_budget_classify` - Decide whether a 3D channel at the given closest-to-head `rank` should run HRTF or simulated 3D, given the current `budget` and whether the channel was HRTF on the previous frame.
- `set_hrtf_budget` - Set the maximum number of 3D channels routed through HRTF each frame; the rest run simulated 3D (constant-power pan + distance attenuation, no convolution).

### Reverb

- `set_reverb` - set reverb for sound
- `set_reverb_preset` - set reverb from preset for sound

### Chorus

- `set_chorus` - set chorus effect for sound
- `set_chorus_default` - set chorus effect with default settings for sound

### Attenuation

- `compute_attenuation` - compute attenuation given distance
- `default_attenuation` - default attenuation 1 / (d + 1)
- `inverse_distance_attenuation` - DEFAULT ATTENUATION MODEL fades with inverse distance, i.e.
- `inverse_square_attenuation` - fades with inverse square distance, i.e.
- `linear_attenuation` - fades linearly with distance, i.e 1 - d / dmax
- `quadratic_attenuation` - fades quadratically with distance, i.e 1 - d^2 / dmax^2

### Status monitoring

- `clear_status` - Drop the stored snapshot; the box reads back empty until the next publish.
- `set_audio_stats_box` - Register a box to receive periodic AudioSystemStats updates from the audio thread.
- `set_status_update` - Publish this sound's status into `status` until `unset_status_update`.
- `unset_status_update` - unset status for sound

### Command batching

- `batch` - Execute a block of audio commands as a single atomic batch.
- `begin_batch` - Deprecated.
- `end_batch` - Deprecated.

### PCM stream

- `append_box_to_pcm` - append samples from lock box to PCM stream.
- `append_to_pcm` - append samples to PCM stream

### Decoding

- `decode_audio` - Decode an audio file (MP3, WAV, FLAC, Vorbis) from raw bytes to PCM float samples.

### Sound ID

- `generate_sound_sid` - generate unique ID for sound

### Internal stream management

- `adopt_audio_thread_command_stream` - A reload's restore: the reference the old context's global held on the stream passes to this context's global, none is taken - the old context is gone without a release, so `set_audio_thread_command_stream` here would leave one reference per reload behind and the real finalize would refuse to delete the stream.
- `get_audio_command_stream`
- `get_sound_sid`
- `set_audio_thread_command_stream`
- `set_sound_sid`

## audio_wav

Module audio_wav


### WAV I/O

- `read_wav` - Read a WAV file into float samples.
- `write_wav` - Write float samples as a 16-bit PCM WAV file.

## audio_record

Module audio_record


### Structures

- `AudioDeviceInfo` - A capture (microphone) device, as returned by `sound_record_list_devices`.

### Recording

- `record_to_wav` - Record `seconds` of audio from the microphone into a 16-bit PCM WAV file.
- `sound_record_list_devices` - Enumerate available capture (microphone) devices.

## strudel_event

Module strudel_event


### Structures

- `Event` - Sound/sample name (`"bd"`, `"sd"`, `"sawtooth"`, `"sine"`, ...).

### ADSR and delay resolution

- `resolve_adsr` - Resolve ADSR fields from their "user set / unset" form (-1 = unset) into concrete envelope parameters.
- `resolve_delaytime` - Resolve delay time: use `delaytime` if set (>= 0), otherwise derive from `delaysync` (in cycles) and the current `cps` so the echo locks to tempo.

## strudel_time

Module strudel_time


### Structures

- `TimeSpan`
- `Hap`

### Cycle math

- `cyclePos` - Fractional position within the current cycle (0.0–1.0).
- `sameCycle` - Cycle number containing time position `t` (integer part as double).
- `splitSpans` - Split a time span at integer cycle boundaries.

## strudel_live

Module strudel_live


## strudel_scales

Module strudel_scales


### Scale and note selection

- `degree_to_note` - Convert a scale degree to a MIDI note number.
- `get_scale_intervals_by_name` - Return the semitone-interval table for a named scale (`"major"`, `"minor"`, `"dorian"`, `"pentatonic"`, `"blues"`, `"chromatic"`, ...).
- `parse_root` - Parse a root-note string (`"C"`, `"Eb4"`, `"Fs3"`, ...) into a MIDI note number.
- `scale` - Alias for `set_scale`.
- `scale_pattern` - Shorthand for `n(notation) |> set_scale(scale_def) |> set_sound(sound)`.
- `set_scale` - Treat each event's `note` field as a scale degree and map it to a MIDI note using `scale_def` (format `"Root:Type"`, e.g.

## strudel_mini

Module strudel_mini


### Enumerations

- `TokenKind`

### Structures

- `Token`

### Tokenizer and parser

- `parse_note_name` - Parse a note name with explicit octave (`"c3"`, `"eb4"`, `"fs2"`) into a MIDI number.
- `parse_note_name_default` - Parse a note name allowing the octave to be omitted (falls back to `default_oct`).
- `tokenize` - Tokenize a mini-notation string into an array of `Token`.

### Fluent DSL constructors

- `n` - Parse mini-notation as numeric values (each atom's `note` field holds the number).
- `note` - Alias for `note_pattern`.
- `note_pattern` - Parse mini-notation for note patterns.
- `s` - Parse mini-notation and return a `Pattern` of sound events.
- `seq` - Parse mini-notation with `|` bar separators for sequential melodies.
- `set_s` - Strudel-style `.s()` with mini-notation.
- `set_vowel` - Apply a vowel-formant filter.
- `vowel` - Alias for `set_vowel` (string overload).

### Cycle-rate variants

- `every_degrade` - Randomly drop events from the pattern every `n`th cycle.
- `every_fast` - Apply `fast(factor)` to the pattern every `n`th cycle.
- `every_slow` - Apply `slow(factor)` to the pattern every `n`th cycle.

## strudel_samples

Module strudel_samples


### Structures

- `Sample`
- `SampleBank`

### Loading

- `load_audio_file` - Decode a single audio file (WAV/MP3/FLAC/OGG) from disk into a Sample.
- `load_audio_memory` - Decode audio bytes (WAV/MP3/FLAC/OGG) from memory into a Sample.
- `load_sound` - Load a single named sound from a directory of audio files into the bank.
- `load_sound_memory` - Decode audio bytes and add the result as a single-variation sound in the bank.

### Rendering

- `render_sample` - Render a sample slice at the given playback speed as interleaved stereo.

## strudel_synth

Module strudel_synth


### Constants

- `SAMPLE_RATE`
- `TWO_PI`
- `OSC_TURN_DOWN`
- `VOWEL_NUM_FORMANTS`
- `VOWEL_MAKEUP_GAIN`

### Enumerations

- `OscType`

### Structures

- `PinkNoiseState`
- `FormantBiquad`
- `VowelFilter`
- `FormantData`
- `VoiceFX`
- `OscVoice`

### Pitch conversion

- `note_to_freq` - Convert a MIDI note number to frequency in Hz (A4 = 69 = 440 Hz, middle C = 60).

### Noise generators

- `noise` - Linear congruential pseudo-random noise in [-1.0, 1.0].

### Drum renderers

- `render_bd`
- `render_cowbell`
- `render_cp`
- `render_crash`
- `render_hh`
- `render_oh`
- `render_ride`
- `render_rimshot`
- `render_sd`
- `render_tambourine`
- `render_tom`

### Oscillator type

- `to_osc_type` - Map a strudel sound name ("sine", "sawtooth", ...) to an `OscType`; unknown names return `osc_unknown`.

### Voice FX chain

- `fx_apply` - Apply the enabled effects to an interleaved stereo buffer in place.
- `fx_apply_to_samples` - Apply Event-driven effects to a pre-rendered stereo buffer as a one-shot.
- `fx_init_from_event` - Configure a VoiceFX chain from the relevant Event fields (crush/coarse/shape/djf/bpf/phaser/tremolo/compressor).

### Oscillator voice

- `osc_render_chunk` - Render one chunk of the oscillator voice additively into the output buffer and the reverb/delay/chorus send buffers.
- `osc_voice_init_filters` - Configure the voice's low-pass and high-pass biquads from Event lpf/hpf cutoffs and lpq/hpq resonances.
- `osc_voice_init_supersaw` - Seed the supersaw unison phases with randomised offsets so the detuned voices don't start in phase.

### Vowel filter

- `formant_biquad_setup_bpf` - Compute bandpass coefficients for the given centre frequency and Q.
- `formant_biquad_tick` - Process one sample through the biquad and return the output.
- `vowel_filter_init` - Configure a VowelFilter for the named vowel, deactivating it if the vowel is unknown.
- `vowel_filter_tick` - Process one sample through the vowel filter: sum the 5 parallel formant bandpass outputs and apply the makeup gain.
- `vowel_get_formant_data` - Look up formant parameters for a vowel name ("a", "e", "i", "o", "u", "ae", "aa", "oe", "ue", "y", "uh", "un", "en", "an", "on").

### Voice rendering

- `mono_to_stereo` - Upmix a mono buffer to interleaved stereo by duplicating each sample to both channels.
- `render_event_stereo` - Render an Event into an interleaved stereo PCM buffer using the built-in drum synths.

## strudel_scheduler

Module strudel_scheduler


### Structures

- `Voice`
- `StrudelSF2Meta`
- `OrbitBus` - Convolution reverb instance (null until a voice requests reverb).
- `Scheduler`

### Orbit effects

- `get_orbit_chorus` - Fetch the chorus instance for the given orbit (for inspection or tests).
- `get_orbit_delay` - Fetch the delay instance for the given orbit (for inspection or tests).
- `get_orbit_reverb` - Fetch the reverb instance for the given orbit (for inspection or tests).
- `init_delay` - Initialize delay on the scheduler's default orbit (bus 0) with the given time/feedback.
- `init_reverb` - Initialize reverb on the scheduler's default orbit (bus 0).

### Scratch marking

- `mark_orbit_scratch`
- `mark_pools_scratch`

### Orbit levels

- `advance_orbit_gains`
- `fade_orbit`
- `orbit_gain`
- `set_orbit_gain`

### Reverb quality override

- `set_force_reverb_quality` - Force all orbit reverbs to a quality tier (0=high, 1=medium, 2=low), overriding per-event roomquality().
- `set_force_reverb_stages` - Force the medium-tier allpass cascade depth on all orbit reverbs, overriding per-event roomstages().

### Scheduler lifecycle

- `finalize` - Release every resource owned by the scheduler: pre-rendered sample voices (each carrying a large PCM buffer), SF2/oscillator voice pools, per-orbit effect buses (reverb / delay / chorus) and their scratch buffers, and the per-tick output buffer.
- `shutdown_scheduler` - Explicit alias for `finalize` — kept for callers that release a long-lived Scheduler (audio thread context, track pool) outside of `var inscope`.
- `tick` - Advance one audio block: query the pattern for new events, spawn voices, mix all voices through the per-orbit reverb/delay/chorus buses into sched.output.

## strudel_player

Module strudel_player


### Structures

- `AudioChunk`

### Lifecycle

- `strudel_create_channel` - Create the PCM stream for main-thread playback.
- `strudel_init` - Spawn the strudel playback thread and inject shared resources (sample bank, SF2, audio channel).
- `strudel_shutdown` - Shut down playback: signal the strudel thread to stop, release stream/status/lockboxes, free all tracks.

### Playback control

- `strudel_command` - Send a user command string from the main thread to the strudel playback thread.
- `strudel_noop_cmd` - No-op command handler — default argument when no user command dispatcher is needed.
- `strudel_play` - Blocking multi-track tick loop for threaded playback.
- `strudel_set_pause` - Pause or resume audio output.
- `strudel_tick` - Single main-thread tick: query all tracks, render audio, mix, append to the PCM stream.
- `strudel_tick_offline` - Offline tick: same as strudel_tick but without an audio driver.

### Tempo and timing

- `strudel_get_playback_time` - Get current playback wall-clock time and tempo as (wall_time, cps).
- `strudel_set_bpm` - Set tempo in beats per minute (assumes 4 beats per cycle).
- `strudel_set_cps` - Set tempo directly in cycles per second.
- `strudel_set_look_ahead` - Set how far ahead (in seconds) to query the pattern for upcoming events.

### Volume

- `strudel_set_volume` - Set the master playback volume, optionally ramping to it over the given time in seconds.

### Asset loading

- `strudel_get_sf2_path` - Return the path of the currently loaded SoundFont (empty string if none).
- `strudel_has_sf2` - Return true if a SoundFont has been loaded.
- `strudel_load_sample_dir` - Load every audio file under root as a named sample (filename becomes the sound name).
- `strudel_load_sf2` - Load a SoundFont (.sf2) file for SF2-based synthesis.
- `strudel_load_sound` - Load a single audio sample from disk and register it under the given name.

### Track management

- `strudel_add_track` - Add a track playing the given pattern at the given gain.
- `strudel_add_track_immediate` - Like strudel_add_track but aligns the track's cycle origin to the current cycle floor, guaranteeing that a note at degree 0 fires on the very first scheduler tick.
- `strudel_fade_track` - Fade a track's gain to target_gain over the given time in seconds (time=0 snaps instantly).
- `strudel_remove_track` - Remove a track by index (shuts down its scheduler and sets the slot to null).

### One-shots

- `strudel_active_tracks` - Number of currently-live tracks (non-null slots).
- `strudel_one_shot` - Fire `pat` ONCE as a self-retiring, fire-and-forget track: it plays for `len_cycles` cycles (1 = its first cycle), goes silent, and the track auto-removes once every voice has finished ringing — so the release tail is never cut.

### Orbit levels

- `strudel_fade_orbit` - Fade an orbit's continuous output level to target_gain over `time` seconds on the given track (time=0 snaps).
- `strudel_orbit_gain` - Current continuous output level for an orbit on the given track (1.0 when unset).
- `strudel_set_orbit_gain` - Set an orbit's continuous output level immediately (no ramp) on the given track.

### State serialization

- `strudel_restore_sf2_state` - Restore a previously serialized SoundFont from a byte array (no-op on empty data).
- `strudel_restore_state` - Restore previously saved playback state (wall time, CPS, sample bank) from a byte array.
- `strudel_save_sf2_state` - Serialize the currently loaded SoundFont to a byte array (empty if none loaded).
- `strudel_save_state` - Serialize playback state (wall time, CPS, sample bank) into a byte array; shuts down playback first.

### Debug and diagnostics

- `strudel_debug_master_peak` - Log the peak amplitude of the current master PCM buffer (debug helper, main-thread mode).
- `strudel_debug_memory` - Enable or disable periodic heap/string-memory logging during playback.
- `strudel_debug_output_peak` - Log the peak amplitude of each track's scheduler output (debug helper).
- `strudel_debug_sample_peaks` - Log the peak amplitude of every loaded sample in the bank (debug helper).
- `strudel_debug_voices` - Log the number of active sample/oscillator voices on each track (debug helper).
- `strudel_get_worker_heap_bytes` - The playback worker's heap, as of its last tick; 0 in main-thread mode, where the caller's own heap is the one in play.
- `strudel_reset_memory_baseline` - Reset memory-tracking baseline to the current heap state.

## strudel_midi

Module strudel_midi


### Enumerations

- `MidiEventKind`

### Structures

- `MidiEvent`
- `MidiTrack`
- `MidiFile`

### Parsing

- `load_midi` - Load and parse a MIDI file from disk.
- `parse_midi` - Parse MIDI data from a byte array into a MidiFile structure.

### Track operations

- `merge_tracks` - Merge all tracks into a single sorted event list for sequential playback.

### Binary readers

- `read_byte` - Read a single byte from data at cursor, advancing cursor.
- `read_u16` - Read a big-endian 16-bit unsigned integer.
- `read_u32` - Read a big-endian 32-bit unsigned integer.
- `read_vlq` - Read a MIDI variable-length quantity (up to 4 bytes).

## strudel_midi_player

Module strudel_midi_player


### Structures

- `GmPreset`
- `MidiVoice`
- `MidiChannelState`
- `MidiPlaybackState`

### Lifecycle

- `midi_init` - Start the MIDI playback thread and attach it to the running audio stream.
- `midi_init_reverb` - Initialise the module-global reverb and chorus effects for offline rendering (calling `midi_tick` without running the thread).
- `midi_shutdown` - Stop the playback thread and release its stream/status; blocks until the audio thread confirms shutdown.

### Playback control

- `midi_is_playing` - Return true while the MIDI playback thread is running.
- `midi_play` - Parse a .mid file from disk and submit it for playback under `name`.
- `midi_play_data` - Parse a .mid file from an in-memory byte array and submit it for playback under `name`.
- `midi_set_pause` - Pause or resume all MIDI playback (the underlying audio stream continues with silence while paused).
- `midi_stop` - Stop a single track by name, or shut down the whole playback thread when `name` is empty.
- `midi_tick` - Advance playback by `chunk_seconds`, process pending events, render all active voices and return an interleaved stereo PCM chunk.

### Audio setup

- `midi_set_audio_vis_channel` - Register a channel that will receive rendered audio chunks for visualisation; call before `midi_init`.
- `midi_set_volume` - Set the linear gain of a named track, optionally fading over `fade` seconds (0 = instantaneous).

### Asset loading

- `midi_load_samples` - Populate the module-global sample bank with piano samples (`note_{n}`) and GM drum samples under `media_path`.
- `midi_load_sf2` - Load an SF2 soundfont from disk into the module-global slot; returns true on success.

### Event processing

- `init_playback` - Initialise a `MidiPlaybackState` from a parsed `MidiFile`, merging its tracks and resetting per-channel state to GM defaults.
- `process_events` - Dispatch all MIDI events up to the current playback tick: note-on/off, program changes, CCs, pitch bend, tempo.

### Drum and GM preset rendering

- `gm_preset` - Map a General MIDI program number (0-127) to a `GmPreset` used by the oscillator path.
- `midi_adsr` - Evaluate the MIDI-style ADSR at `elapsed` seconds since note-on and `note_off_elapsed` seconds since note-off.
- `render_drum_hit` - Render a GM channel-10 drum hit for `note` at the given velocity as a mono PCM buffer.

## strudel_sf2

Module strudel_sf2


### Constants

- `SF2_SAMPLE_MONO`
- `SF2_SAMPLE_RIGHT`
- `SF2_SAMPLE_LEFT`
- `SF2_SAMPLE_LINKED`
- `SF2_SAMPLE_OGG_VORBIS`

### Enumerations

- `SF2GenOper`

### Structures

- `SF2SampleHeader`
- `SF2Generator`
- `SF2Modulator`
- `SF2Zone`
- `SF2InstrumentDef`
- `SF2PresetDef`
- `SF2File`

### Loading

- `sf2_load` - Load and parse an SF2 soundfont file from disk.
- `sf2_parse` - Parse SF2 data from a byte array.

### Preset and zone lookup

- `sf2_find_preset` - Find a preset by bank and program number.
- `sf2_find_zones` - Find instrument zones matching a key and velocity for a given preset.

### Generator access

- `get_generator` - Get the value of a generator from a zone.
- `get_generator_range` - Get the range (low, high) of a range-type generator.
- `has_generator` - Check whether a zone has a specific generator.

### Unit conversion

- `centibels_to_linear` - Convert centibels to linear amplitude.
- `cents_to_hz` - Convert SF2 cents to Hz (relative to 8.176 Hz).
- `timecents_to_seconds` - Convert SF2 timecents to seconds.

### Modulator source helpers

- `sf2_mod_src_bipolar` - Return true if the modulator source polarity is bipolar (bit 9 set).
- `sf2_mod_src_cc` - Return true if the modulator source is a MIDI CC (bit 7 set).
- `sf2_mod_src_index` - Extract the source index (bits 0-6) from a packed modulator source operand.
- `sf2_mod_src_negative` - Return true if the modulator source direction is negative (bit 8 set).
- `sf2_mod_src_type` - Extract the modulator source curve type (bits 10-15): 0=linear, 1=concave, 2=convex, 3=switch.

## strudel_sf2_voice

Module strudel_sf2_voice


### Enumerations

- `SF2EnvStage`

### Structures

- `SF2Envelope`
- `SF2LFO`
- `SF2VoiceAttenState`

### Envelope

- `sf2_env_start`
- `sf2_env_tick`

### LFO

- `sf2_lfo_tick`

### Modulators

- `sf2_apply_default_modulators` - Apply the 10 SF2 spec default modulators (8.4.1-8.4.10) that are not overridden by any zone modulator.
- `sf2_apply_modulators` - Accumulate all modulator outputs from a zone into `gen_offsets` indexed by destination generator.
- `sf2_collect_atten_modulators` - Collect every modulator targeting initialAttenuation across all zone layers plus unoverridden defaults.
- `sf2_default_cc_values` - Initialise a CC array to General MIDI defaults (CC7=100, CC10=64, CC11=127, pitchwheel slot=8192).
- `sf2_mod_get_value` - Evaluate a single SF2 modulator: amount * transform(src1) * transform(src2).
- `sf2_mod_identity_match` - Two SF2 modulators share identity when src1, src2, and destination all match.
- `sf2_recalc_atten` - Recalculate voice attenuation (linear gain) from cached modulators and current CC values.
- `sf2_velocity_attenuation` - SF2 default velocity-to-attenuation curve (concave, 960 cB range).

### Voice creation

- `sf2_create_c_voice` - Resolve all SF2 generators/modulators for a note and populate the C `ma_sf2_voice` runtime state.
- `sf2_get_chorus_send` - Resolve the chorus effects-send amount across zone layers, normalised to [0..1].
- `sf2_get_exclusive_class` - Resolve the exclusiveClass generator across all zone layers.
- `sf2_get_reverb_send` - Resolve the reverb effects-send amount across zone layers, normalised to [0..1].

## strudel_pattern

Module strudel_pattern


### Type aliases

- `Pattern` - A Pattern is a pure function from a query `TimeSpan` to the `Hap`s (events with timing info) that fall within it.
- `PatternTransform` - A function pattern → pattern.

### Pattern construction

- `atom` - Shorthand for `pure(Event(s=name))` — a pattern with one event per cycle using the named sound.
- `pure` - Lifts an `Event` into a `Pattern` that emits that value once per cycle, spanning the entire cycle.
- `silence` - The empty pattern — produces no haps for any query span.

### Time manipulation

- `cat` - Sequences patterns across cycles — with N patterns, pattern `i` plays during cycle `i mod N`, each stretched to one cycle.
- `fast` - Speeds up `pat` by factor `n` — the pattern repeats `n` times per cycle.
- `fastcat` - Like `cat` but squeezes all patterns into a single cycle, side by side.
- `fmap` - Applies `fn` to the `Event` value of every hap produced by `pat`, preserving timing.
- `slow` - Slows down `pat` by factor `n` — `pat` takes `n` cycles to complete once.
- `stack` - Layers all patterns simultaneously — every pattern plays at once.
- `weighted_fastcat` - Like `fastcat` but each element's cycle slice is proportional to its weight.

### Combinators

- `add` - Alias for `transpose` — adds `semitones` to each event's note.
- `bjorklund` - Bjorklund's algorithm — distributes `k` onsets as evenly as possible across `n` slots.
- `choose` - Randomly (deterministically) picks one pattern from `pats` per cycle.
- `chooseCycles` - Alias for `choose` — one random pick per cycle.
- `chop` - Subdivides each event in place into `n` back-to-back slices of its sample (slice `i` = `begin..end` window `i/n`).
- `chunk` - Divides each cycle into `n` chunks, applying `transform` to a different chunk each cycle (rotating).
- `combineWith` - For each hap in `pat`, samples `val_pat` at the hap's onset and applies `fn(event, sampledValue)` to produce the output event.
- `combineWithStr` - Like `combineWith` but samples a string from `val_pat`'s `event.s` field.
- `compress` - Squeezes `pat` into the cycle-time window `[b, e]`.
- `degrade` - Randomly drops events from `pat` with probability `prob` (0..1).
- `degradeBy` - Alias for `degrade` with explicit probability.
- `early` - Shifts `pat` earlier by `offset` cycles (strudel.cc's `<~`).
- `echo` - Repeats every event `times` times, offset by `time` cycles each, with each repeat scaled by `feedback`.
- `euclid` - Euclidean rhythm — `pat` plays at `k` evenly-spaced onsets over `n` subdivisions of each cycle.
- `euclidRot` - Euclidean rhythm like `euclid`, then rotated left by `rot` steps.
- `every` - Plays `pat_on` on every `n`-th cycle, `pat_off` otherwise.
- `hurry` - Combined `fast` + `set_speed` — speeds up both pattern timing AND sample playback by `factor`.
- `innerJoin` - Pattern monadic join — for each hap in `param_pat`, builds an inner pattern via `fn(hap.note)` and intersects its haps with the outer hap's part.
- `iter` - Rotates `pat` left by `1/n` cycle each cycle.
- `iterBack` - Like `iter` but rotates `pat` right by `1/n` cycle each cycle.
- `jux` - Stereo split: original is panned left, `transform(pat)` is panned right.
- `late` - Shifts `pat` later by `offset` cycles (strudel.cc's `~>`).
- `layer` - Stacks `pats` on top of `base`, all playing simultaneously.
- `linger` - Loops the first `t`-cycle fraction of `pat` to fill each cycle.
- `mask` - Keeps only events from `pat` whose timespan overlaps a truthy event in `mask_pat`; `f`, `false` and `0` steps mask out.
- `off` - Overlays `pat` with a time-shifted copy transformed by `transform`.
- `often` - Applies `transform` to each event with 75% probability.
- `once` - Play `pat` exactly once (its first cycle), then silence forever.
- `palindrome` - Plays `pat` forward on even cycles, reversed on odd cycles.
- `playFor` - Gate `pat` to its first `n` cycles (onsets in [0, n)), then silence forever.
- `ply` - Repeats each event `n` times within its own timespan.
- `randcat` - Alias for `choose` — randomly picks one pattern per cycle.
- `rarely` - Applies `transform` to each event with 25% probability.
- `rev` - Reverses event positions within each cycle.
- `scramble` - Like `shuffle` but with replacement — each of the `n` slots independently picks a random subdivision.
- `shuffle` - Randomly permutes `n` equal subdivisions of each cycle (Fisher-Yates, deterministic per cycle).
- `slice` - Cuts the sample into `n` slices and plays them in the order/rhythm of `idx_pat` (whose `note` values are slice indices).
- `sometimes` - Applies `transform` to each event with 50% probability.
- `sometimesby` - Applies `transform` to each event with probability `prob` (0..1).
- `striate` - Cuts each sample into `n` slices and plays them in sequence.
- `struct_` - Restructures `pat` to the rhythmic positions of `bool_pat`'s truthy haps — values come from `pat`, timing from the mask; `f`, `false` and `0` steps are off.
- `stut` - Alias for `echo` with argument order `(times, feedback, time)`.
- `sub` - Subtracts `semitones` from each event's note.
- `superimpose` - Stacks `pat` with `transform(pat)`, both playing simultaneously.
- `transpose` - Shifts every event's MIDI note by `semitones`.
- `wchoose` - Weighted random pick from `pats` per cycle using parallel `weights` array.
- `when_cycle` - Applies `transform` to `pat` on cycles where `cond(cycle)` is true, leaving others untouched.

### Setter primitives

- `set_attack` - Sets the amp-envelope `attack` time (seconds) on every event in `pat`.
- `set_begin` - Pattern-valued `set_begin` — samples `mod_pat` at each event onset to set sample start position (0..1).
- `set_bpf` - Sets the band-pass filter cutoff `bpf` (Hz) on every event in `pat`.
- `set_bpq` - Pattern-valued `set_bpq` — samples `mod_pat` at each event onset to set band-pass filter Q.
- `set_chorus` - Sets the chorus send amount `chorus` (0..1) on every event in `pat`.
- `set_compressor` - Sets the compressor threshold `compressor` (dB; >=0 bypasses, negative activates) on every event in `pat`.
- `set_compressor_attack` - Pattern-valued `set_compressor_attack` — samples `mod_pat` at each event onset to set compressor attack (seconds).
- `set_compressor_knee` - Pattern-valued `set_compressor_knee` — samples `mod_pat` at each event onset to set compressor knee (dB).
- `set_compressor_ratio` - Pattern-valued `set_compressor_ratio` — samples `mod_pat` at each event onset to set compressor ratio.
- `set_compressor_release` - Pattern-valued `set_compressor_release` — samples `mod_pat` at each event onset to set compressor release (seconds).
- `set_crush` - Sets the bit-`crush` amount on every event in `pat` (lower = more bit-depth reduction).
- `set_decay` - Sets the amp-envelope `decay` time (seconds) on every event in `pat`.
- `set_delay` - Sets the delay send amount (`delay_amount`, 0..1) on every event in `pat`.
- `set_delayfeedback` - Sets the delay feedback `delayfeedback` (0..<1) on every event in `pat`.
- `set_delaytime` - Sets the delay time `delaytime` (seconds) on every event in `pat`.
- `set_djf` - Pattern-valued `set_djf` — samples `mod_pat` at each event onset to set DJ-filter position (<0.5 = LP, >0.5 = HP).
- `set_end` - Pattern-valued `set_end` — samples `mod_pat` at each event onset to set sample end position (0..1).
- `set_fm` - Sets the FM modulation depth `fm` (modulator amplitude) on every event in `pat`.
- `set_fmh` - Sets the FM harmonicity ratio `fmh` (modulator frequency / carrier frequency) on every event in `pat`.
- `set_freq` - Sets the explicit oscillator frequency `freq` (Hz) on every event in `pat`, overriding `note`.
- `set_gain` - Sets the `gain` (amplitude, 0..1) field on every event in `pat`.
- `set_hpf` - Sets the high-pass filter cutoff `hpf` (Hz) on every event in `pat`.
- `set_hpq` - Sets the high-pass filter resonance `hpq` (Q factor) on every event in `pat`.
- `set_hrtf` - Sets HRTF azimuth and elevation in one call and engages HRTF positioning.
- `set_hrtf_azimuth` - Sets the HRTF azimuth (degrees, -180..180) on every event in `pat` and engages HRTF positioning.
- `set_hrtf_elevation` - Sets the HRTF elevation (degrees, -90..90) on every event in `pat` and engages HRTF positioning.
- `set_lpf` - Sets the low-pass filter cutoff `lpf` (Hz) on every event in `pat`.
- `set_lpq` - Sets the low-pass filter resonance `lpq` (Q factor) on every event in `pat`.
- `set_note` - Sets the MIDI `note` number on every event in `pat`.
- `set_orbit` - Sets the effect-bus routing index `orbit` on every event in `pat`.
- `set_pan` - Sets the `pan` (stereo position: 0=left, 0.5=center, 1=right) field on every event in `pat`.
- `set_phaser` - Sets the phaser rate `phaser` (Hz, 0 disables) on every event in `pat`.
- `set_phasercenter` - Pattern-valued `set_phasercenter` — samples `mod_pat` at each event onset to set phaser center frequency.
- `set_phaserdepth` - Pattern-valued `set_phaserdepth` — samples `mod_pat` at each event onset to set phaser depth.
- `set_phasersweep` - Pattern-valued `set_phasersweep` — samples `mod_pat` at each event onset to set phaser sweep range.
- `set_release` - Sets the amp-envelope `release` time (seconds) on every event in `pat`.
- `set_room` - Sets the reverb send amount `room` (0..1) on every event in `pat`.
- `set_roomquality` - Sets the reverb quality tier on every event: "high" (dual-IR, default), "medium" (single mono IR + an allpass decorrelation cascade, ~2x cheaper; depth set by roomstages), or "low" (Freeverb, cheapest).
- `set_roomsize` - Sets the reverb room size `roomsize` (larger = more sustained reverb) on every event in `pat`.
- `set_roomstages` - Sets the medium-tier allpass cascade depth on every event (1–8; 0 selects the default of 5).
- `set_sf2_expression` - Pattern-valued `set_sf2_expression` — samples `mod_pat` at each event onset to set SF2 expression CC11.
- `set_sf2_mod_wheel` - Pattern-valued `set_sf2_mod_wheel` — samples `mod_pat` at each event onset to set SF2 mod wheel CC1.
- `set_sf2_pitch_bend` - Pattern-valued `set_sf2_pitch_bend` — samples `mod_pat` at each event onset to set SF2 pitch bend.
- `set_shape` - Sets the waveshape/distortion amount `shape` (0..1) on every event in `pat`.
- `set_sound` - Sets the sound name `s` on every event in `pat`.
- `set_speed` - Sets the sample playback `speed` on every event in `pat` (1.0 = normal, 2.0 = double pitch/rate).
- `set_sustain` - Sets the amp-envelope `sustain` level (0..1) on every event in `pat`.
- `set_tremolo` - Sets the tremolo rate `tremolo` (Hz, 0 disables — amplitude modulation) on every event in `pat`.
- `set_tremolodepth` - Pattern-valued `set_tremolodepth` — samples `mod_pat` at each event onset to set tremolo depth.
- `set_velocity` - Sets the `velocity` (MIDI-like expression, 0..1) field on every event in `pat`.

### Fluent control: dynamics and routing

- `begin` - Fluent shorthand for `set_begin`.
- `cut` - Fluent shorthand for `set_cut`.
- `end_pos` - Fluent shorthand for `set_end`.
- `freq` - Fluent shorthand for `set_freq`.
- `gain` - Fluent shorthand for `set_gain`.
- `note` - Fluent shorthand for `set_note`.
- `orbit` - Fluent shorthand for `set_orbit`.
- `pan` - Fluent shorthand for `set_pan`.
- `sound` - Fluent shorthand for `set_sound`.
- `speed` - Fluent shorthand for `set_speed`.
- `vel` - Alias for `velocity`.
- `velocity` - Fluent shorthand for `set_velocity`.

### Fluent control: 3D positioning (HRTF)

- `hrtf` - Fluent shorthand for `set_hrtf` — sets HRTF azimuth + elevation and engages 3D positioning; `pan` still controls source width.
- `hrtf_azimuth` - Fluent shorthand for `set_hrtf_azimuth`.
- `hrtf_elevation` - Fluent shorthand for `set_hrtf_elevation`.

### Fluent control: effects sends

- `chorus` - Fluent shorthand for `set_chorus`.
- `delay` - Fluent shorthand for `set_delay`.
- `delayfeedback` - Fluent shorthand for `set_delayfeedback`.
- `delaysync` - Fluent shorthand for `set_delaysync`.
- `delaytime` - Fluent shorthand for `set_delaytime`.
- `dfb` - Alias for `delayfeedback`.
- `dt` - Alias for `delaytime`.
- `room` - Fluent shorthand for `set_room`.
- `roomquality` - Fluent shorthand for `set_roomquality`.
- `roomsize` - Fluent shorthand for `set_roomsize`.
- `roomstages` - Fluent shorthand for `set_roomstages`.
- `size` - Alias for `roomsize`.

### Fluent control: filters

- `bpf` - Fluent shorthand for `set_bpf`.
- `bpq` - Fluent shorthand for `set_bpq`.
- `djf` - Fluent shorthand for `set_djf`.
- `hp` - Alias for `hpf`.
- `hpf` - Fluent shorthand for `set_hpf`.
- `hpq` - Fluent shorthand for `set_hpq`.
- `lp` - Alias for `lpf`.
- `lpf` - Fluent shorthand for `set_lpf`.
- `lpq` - Fluent shorthand for `set_lpq`.
- `resonance` - Alias for `lpq`.

### Fluent control: envelope

- `att` - Alias for `attack`.
- `attack` - Fluent shorthand for `set_attack`.
- `dec` - Alias for `decay`.
- `decay` - Fluent shorthand for `set_decay`.
- `rel` - Alias for `release`.
- `release` - Fluent shorthand for `set_release`.
- `sus` - Alias for `sustain`.
- `sustain` - Fluent shorthand for `set_sustain`.

### Fluent control: synthesis and shaping

- `coarse` - Fluent shorthand for `set_coarse`.
- `crush` - Fluent shorthand for `set_crush`.
- `fm` - Fluent shorthand for `set_fm`.
- `fmh` - Fluent shorthand for `set_fmh`.
- `shape` - Fluent shorthand for `set_shape`.

### Fluent control: modulation FX

- `compressor` - Fluent shorthand for `set_compressor`.
- `compressorAttack` - camelCase fluent shorthand for `set_compressor_attack`.
- `compressorKnee` - camelCase fluent shorthand for `set_compressor_knee`.
- `compressorRatio` - camelCase fluent shorthand for `set_compressor_ratio`.
- `compressorRelease` - camelCase fluent shorthand for `set_compressor_release`.
- `compressor_attack` - snake_case fluent shorthand for `set_compressor_attack`.
- `compressor_knee` - snake_case fluent shorthand for `set_compressor_knee`.
- `compressor_ratio` - snake_case fluent shorthand for `set_compressor_ratio`.
- `compressor_release` - snake_case fluent shorthand for `set_compressor_release`.
- `ph` - Alias for `phaser`.
- `phaser` - Fluent shorthand for `set_phaser`.
- `phasercenter` - Fluent shorthand for `set_phasercenter`.
- `phaserdepth` - Fluent shorthand for `set_phaserdepth`.
- `phasersweep` - Fluent shorthand for `set_phasersweep`.
- `phc` - Alias for `phasercenter`.
- `phd` - Alias for `phaserdepth`.
- `phs` - Alias for `phasersweep`.
- `trem` - Alias for `tremolo`.
- `tremdepth` - Alias for `tremolodepth`.
- `tremolo` - Fluent shorthand for `set_tremolo`.
- `tremolodepth` - Fluent shorthand for `set_tremolodepth`.

### Fluent control: SF2

- `sf2` - Fluent shorthand for `set_sf2` by GM instrument name.
- `sf2_bank` - Fluent shorthand for `set_sf2_bank`.
- `sf2_expression` - Fluent shorthand for `set_sf2_expression`.
- `sf2_mod_wheel` - Fluent shorthand for `set_sf2_mod_wheel`.
- `sf2_pitch_bend` - Fluent shorthand for `set_sf2_pitch_bend`.

### Signals

- `cosine` - Unipolar cosine signal `0..1`.
- `cosine2` - Bipolar cosine signal `-1..1`.
- `irand` - Random integer signal in `0..n-1`, deterministic.
- `isaw` - Inverted unipolar sawtooth signal — falls from 1.0 to 0.0 each cycle.
- `isaw2` - Bipolar inverted sawtooth signal `-1..1`.
- `itri` - Inverted unipolar triangle signal — starts at 1.0, dips to 0.0 at midpoint.
- `itri2` - Bipolar inverted triangle signal `-1..1`.
- `perlin` - Unipolar Perlin-noise signal `0..1`.
- `rand` - Unipolar deterministic random signal `0..1` — hash-based, same time yields same value.
- `rand2` - Bipolar deterministic random signal `-1..1` — hash-based.
- `range` - Scales a signal pattern from `0..1` to `lo..hi`.
- `run` - Discrete ramp `0..n-1` within each cycle — `n` haps per cycle with values `0, 1, ..., n-1`.
- `saw` - Unipolar sawtooth signal `0..1`.
- `saw2` - Bipolar sawtooth signal `-1..1`.
- `signal` - Builds a continuous-signal pattern from `fn(t) : float`.
- `signal_cosine` - Unipolar cosine signal, mapped to `0..1` over one cycle.
- `signal_perlin` - Deterministic Perlin-style noise mapped to `0..1`.
- `signal_range` - Scales a signal pattern from `0..1` to `lo..hi`.
- `signal_saw` - Sawtooth signal — ramps `0..1` linearly within each cycle.
- `signal_sine` - Unipolar sine signal, mapped to `0..1` over one cycle.
- `signal_tri` - Triangle signal — rises `0→1` in the first half of each cycle, falls `1→0` in the second half.
- `sine` - Unipolar sine signal `0..1`.
- `sine2` - Bipolar sine signal `-1..1`.
- `square` - Unipolar square signal (1.0 first half, 0.0 second half).
- `square2` - Bipolar square signal `-1..1`.
- `tri` - Unipolar triangle signal `0..1`.
- `tri2` - Bipolar triangle signal `-1..1`.

## peg

The PEG module is a parser generator based on `Parsing Expression Grammars`_. Define grammars directly in daslang using the `parse` macro --- the compiler generates a packrat parser at compile time.  No external tools, no runtime code generation.


### Structures

- `ParsingError`

### Matching primitives

- `get_current_char`
- `matches`
- `move`
- `reached_EOF`
- `reached_EOL`

### Whitespace

- `skip_taborspace`
- `skip_whitespace`

### Literal matching

- `match_decimal_literal` - Simple lexing of decimal integers, doesn't check for overflow
- `match_double_literal` - Matches doubles in the form of [-+]? [0-9]* .? [0-9]+ ([eE] [-+]? [0-9]+)? The number is not checked to be representable as defined in IEEE-754
- `match_string_literal` - Tries to match everything inside ""

### Tracing and logging

- `log_fail`
- `log_info`
- `log_plain`
- `log_success`
- `tabulate`

## gltf_types

dasGLTF loads `glTF 2.0`_ models — `.glb` (binary), `.gltf` (JSON), external or base64-embedded buffers — into a **backend-neutral scene**, with no new native code (it builds on `json_boost`, `base64`, `fio`, `stbimage` and `math`).


### Enumerations

- `GltfAlphaMode`
- `GltfInterp`
- `GltfPath`
- `GltfPrimitiveMode`

### Structures

- `GltfVertex`
- `GltfPrimitive`
- `GltfMesh`
- `GltfMaterial`
- `GltfImage`
- `GltfSampler`
- `GltfTexture`
- `GltfNode`
- `GltfSkin`
- `GltfAnimSampler`
- `GltfAnimChannel`
- `GltfAnimation`
- `GltfScene`

### Loading

- `load_gltf` - Load a glTF 2.0 file (.gltf, .glb, or base64-embedded .gltf) into a backend-agnostic `GltfScene`.
- `load_gltf_from_memory` - Load a glTF 2.0 document (.gltf JSON, .glb binary, or base64-embedded .gltf) from an in-memory byte buffer.

### Scene, skinning and animation

- `evaluate_animation` - Evaluate animation `animation` at time `t` (seconds): sample each channel and write the result into its target node's TRS, then recompose those nodes' local matrices and refresh every world transform.
- `gltf_joint_matrix` - Skinning matrix for joint slot `j` of skin `skinIndex`: jointNode.world * inverseBind[j].
- `gltf_scene_bounds` - World-space axis-aligned bounds (min, max) over every mesh vertex, using each node's `world` transform.
- `update_world_transforms` - Recompute every node's `world` transform by walking the hierarchy from the scene roots (world = parent.world * local).

### Accessor byte readers

- `gltf_comp_size` - Byte size of one accessor component for the given glTF componentType.
- `gltf_rd_f32`
- `gltf_rd_i16`
- `gltf_rd_i8`
- `gltf_rd_u16`
- `gltf_rd_u32`
- `gltf_rd_u8`
- `gltf_read_comp_float` - Read one component at byte offset `o` as float, applying glTF `normalized` integer→float conversion (unsigned: /max; signed: max(v/max, -1)).
- `gltf_read_comp_uint` - Read one component at byte offset `o` as uint (index accessors: u8/u16/u32).
- `gltf_type_ncomp` - Number of components for a glTF accessor `type` string (SCALAR/VECn/MATn).

## texture_blocks

Portable BC1, BC3, BC4, and BC5 texture compression with complete mip chains, deterministic cache keys, serialized validation, and RGBA8 decoding.


### Constants

- `BLOCK_TEXTURE_VERSION`

### Structures

- `BlockMip` - One block-compressed mip level and its logical pixel dimensions.
- `BlockTexture` - A complete BC-compressed texture with a deterministic content key and mip chain.

### Compression and caching

- `block_texture_key` - Compute the deterministic cache key for tightly packed RGBA8 pixels and their compression settings.
- `cached_block_texture` - Load a valid content-keyed `.das_tex` entry from `directory`, or compress and atomically populate it on a miss.
- `compress_block_texture` - Compress tightly packed RGBA8 pixels into BC1/BC3/BC4/BC5 (`format` 0..3), generating a box-filtered mip chain through 1x1.

### Validation and decoding

- `block_texture_valid` - Return true when version, dimensions, format, mip count, and every encoded byte size satisfy the serialized block-texture contract.
- `decode_block_mip` - Decode one BC mip to tightly packed RGBA8 pixels.

## gltf_processed

Backend-neutral glTF preprocessing that packs and optionally encodes geometry, compresses textures, and persists validated monolithic or split-file assets.


### Constants

- `DAS_GLTF_VERSION`
- `DAS_GLTF_PROCESSOR_VERSION`
- `VERTEX_UV`
- `VERTEX_TANGENT`
- `VERTEX_SKIN`
- `VERTEX_UV1`
- `VERTEX_COLOR`

### Structures

- `ProcessingProfile` - Controls which optional vertex streams and storage encodings survive preprocessing.
- `ProcessedPrimitive` - Portable packed geometry for one primitive, optionally meshoptimizer-encoded.
- `ProcessedMesh` - A named mesh containing portable processed primitives.
- `ProcessedAsset` - Backend-neutral processed glTF data: scene metadata, packed geometry, and BC textures.
- `ProcessedCatalogEntry` - Identifies one processed asset and the key that produced it.
- `ProcessedCatalog` - Versioned list of processed assets for tooling and package manifests.
- `ProcessedManifest` - Split-file manifest for geometry and independently shared texture payloads.

### Validation and persistence

- `load_and_process_gltf` - Load an existing processed file by extension, otherwise parse and process a source glTF.
- `load_processed` - Load and validate a `.das_glb` binary or split `.das_gltf` manifest.
- `processed_valid` - Check serialized version, size limits, packed strides and raw byte counts, compressed textures, and scene references.
- `save_processed` - Save a validated asset as one `.das_glb` binary or as a `.das_gltf` manifest plus geometry and texture files.

### Geometry processing

- `process_gltf` - Convert a loaded scene into backend-neutral processed geometry and block textures.
- `process_primitive` - Pack one glTF primitive according to `flags`, deduplicate triangle vertices, and optionally optimize ordering and encode storage.
- `processed_indices` - Return decoded uint32 indices for `p`, rejecting codec failures and indices outside the declared vertex range with a panic.
- `processed_stride` - Return the packed vertex stride for the `VERTEX_*` attribute bit mask.
- `processed_vertex_bytes` - Return decoded packed vertex bytes for `p`.
- `unpack_processed_geometry` - Reconstruct ordinary `GltfPrimitive` vertex and index arrays from a processed asset while cloning its scene metadata.

## spirv_reflect

Shader reflection for the dasSpirv SPIR-V backend: an API-neutral description of a shader's descriptor bindings and push-constant ranges, emitted alongside the SPIR-V blob and consumed by the host (e.g. dasVulkan) to auto-build descriptor-set and pipeline layouts. The same per-global classification that emits the SPIR-V fills the reflection, so the host never re-declares set/binding/type by hand. Serialized to an `array<uint>` (riding the same module-global-capture rail as the blob) and decoded once at layout-build time; the wire form is versioned so producer/consumer drift fails loudly.


### Type aliases

- `SpirvStageFlags`

### Constants

- `REFLECTION_MAGIC`

### Enumerations

- `SpirvDescriptorKind`

### Structures

- `SpirvDescriptorBinding`
- `SpirvPushConstantRange`
- `SpirvReflection`

### Serialization

- `decode_reflection`
- `encode_reflection`

### Shader blob literals

- `unpack_spirv_words`

## dasllama

CPU large-language-model inference in pure daslang: load a GGUF model, tokenize, run the transformer, sample — or hold a full chat — validated token-for-token against llama.cpp on every supported family. Run with `-jit`; `examples/dasLLAMA/run.das` and `chat.das` show the canonical program shape.


### Type aliases

- `DlimImageInfo`
- `GpuTierStatus`
- `GpuTierWant`
- `GpuModelMarks`

### Model loading and sessions

- `caps` - What `model` honestly supports at the chat layer (see LlmCaps) — e.g.
- `create_batch_workspace` - Create the caller-owned scratch that `eval_batch` steps through — one per concurrent batch, reused across calls (buffers grow to the largest batch seen).
- `create_kv_pool` - Create a caller-owned PAGED KV pool over `model`'s cache geometry.
- `create_session` - Create a fresh session (KV cache + scratch) sized to `model.config.seq_len` — one model, many independent conversations.
- `load_model` - Load a model AND its tokenizer from a GGUF file — architecture and tokenizer backend are auto-selected from metadata; `mode` picks the weight quantization.
- `release_kv_pages` - Return `session`'s KV pages to its pool (no-op on flat sessions).
- `setup_dasllama_jobque` - Configure the job queue for dasLLAMA's fork/join matmul dispatch: pooled fork contexts, batched dispatch, the worker spin-before-park window (`jobque_spin_us`; 0 disables).

### Prefix cache

- `create_prefix_cache` - Create a prefix cache for the paged sessions of one `create_kv_pool` pool: finished streams donate KV pages (`prefix_insert`), later requests with the same prefix attach them (`prefix_attach`) instead of re-prefilling.
- `prefix_attach` - Attach the longest cached prefix of `prompt` to a FRESH paged `session` of `pool`: matched pages join the session's block table and `n_past` advances past them, so the caller prefills only the tail.
- `prefix_chain_list` - Snapshot of the cache's donated chains for dashboards: per donation — page-covered token count, live pages, hit count, born/last-hit ticks, and the caller-provided preview.
- `prefix_held_groups` - Pages the cache currently holds (== pool groups retained for reuse).
- `prefix_insert` - Donate a finished session's KV pages to the cache.
- `prefix_release` - Release every cached page back to `pool` and clear the cache (pages still used by live sessions stay alive until those sessions release them).

### Tokenizer

- `decode` - Decode a token-id sequence back to text with the model's tokenizer.
- `encode` - Encode `text` to token ids with the model's tokenizer.
- `piece` - Decode a single token to its text piece — the streaming counterpart of `decode`.

### Evaluation and sampling

- `eval` - THE eval primitive: run `tokens` at the session's current position and advance it.
- `eval_batch` - One synchronous batched decode step: row i evals `tokens[i]` at `sessions[i]`'s current position, advancing each by one — B conversations through ONE pass of the weights (GEMVs batch into GEMMs).
- `eval_embd` - `eval`'s embedding-input twin: prefill `npos` pre-built embedding rows (`npos × dim`, token-major) at the session's current position and advance it — the multimodal splice entry.
- `eval_embd_span` - `eval_embd` for a prompt carrying one NON-CAUSAL image span: rows `[span_lo, span_hi)` prefill with every query attending the whole span, the text around them causally — the gemma vision decode shape (non-causal media chunk).
- `eval_embd_span_mrope` - `eval_embd_span`'s qwen mrope twin: the span rows rope as a `grid`-shaped merged image (position advance `max(grid.x, grid.y)`, tracked on the session for every later eval); same span mask, only the angles differ.
- `sample` - Sample the next token from `session.logits` per `params`: penalties, then temperature/top-k/top-p/min-p and a CDF draw — or greedy argmax when `params.temp <= 0` (`SamplingParams()` defaults are greedy).
- `set_seed` - Seed the session's sampling RNG for reproducible generation.
- `stats` - Timing of the most recent `generate`/`respond` call on `session`: prompt/generated token counts, time to first token, prefill and generation tok/s.

### Generation

- `generate` - Stream-generate up to `max_tokens` from `prompt`, invoking the trailing block per token with `(id, piece)`; return `false` from the block to stop early.
- `generate_embd` - `generate`'s embedding-prefill twin: prefill `npos` pre-built embedding rows (the multimodal splice — see `eval_embd`), then stream-sample exactly like `generate`.

### Embeddings

- `embed` - Mean-pooled, L2-normalized sentence embedding of `text` (`model.config.dim` floats): the decoder's last-layer hidden state (post-final RMSNorm), averaged then unit-normalized.

### Vision and audio encoders

- `encode_audio` - `encode_image`'s audio twin: 16 kHz mono samples through the carried family's encoder into `out` = npos × proj_dim soft tokens (returns npos).
- `encode_image` - The whole image path in one call — geometry, letterbox, normalize, encode — into `out` = npos × proj_dim soft tokens (returns npos).

### Chat

- `add_assistant` - Inject a KNOWN assistant reply (no generation): prefill the pending user turn and `text` into the KV cache, then close the turn — like `respond` but with a supplied reply.
- `add_user` - Queue a user message for the next `respond`.
- `add_user_audio` - Queue audio (16 kHz mono f32 PCM) for the next `respond` — encoded to soft tokens immediately and spliced at the head of the turn before any `add_user` text.
- `add_user_image` - Queue an image for the next `respond` — geometry, letterbox and the embedder run NOW, spliced at the head of the next user turn.
- `add_user_image_rows` - Queue PRE-ENCODED image soft-token rows (what `encode_image` emits — deepstack models: `(1+n)·dim`-wide, length-checked) with the family's mrope `grid` ((0,0) = sequential) for the next `respond`.
- `create_chat` - Start a conversation over `model`: resolves the chat template (GGUF-embedded, falling back to the arch registry) and creates the session.
- `create_chat_renderer` - `create_chat`'s RENDER-ONLY twin: resolves the template/stop ids/turn close but creates NO KV session — a queued request can render its whole prompt holding tokens only, no cache memory.
- `render_assistant` - `add_assistant`'s render half: appends the exact token stream a known reply prefills to `out` WITHOUT running the model, advancing the transcript like `add_assistant`.
- `render_close` - The tokens that TERMINATE an assistant turn (what `respond` evals after the reply) — for schedulers that close a finished stream's turn themselves.
- `render_turn` - Render the next turn's prefill token ids — BOS + system on the first turn, then the user turn and the generation prompt — WITHOUT running the model.
- `render_turn_audio` - `render_turn`'s AUDIO twin: the same two-span contract around the audio soft-token splice (the template's audio span markers).
- `render_turn_image` - `render_turn`'s IMAGE twin: the turn's prefill as the two token spans that bracket the image soft-token splice — `head` before the rows, `tail` after.
- `respond` - Generate the assistant's reply to the queued user message, streaming pieces through the trailing block (return `false` to stop early).
- `set_thinking` - Toggle reasoning for a hybrid thinking model (Qwen3 family): `false` appends the template's empty think block so the model answers directly.

### Tool calling

- `add_tool_results` - Queue tool results as the next pending turn — the reply to an assistant turn that called tools.
- `parse_calls` - Parse a complete reply per the model family's wire format into a ToolReply — the reasoning span, the content, and the calls with arguments normalized to JSON object text.
- `render_assistant_calls` - `render_assistant`'s tool-calling twin: replay an assistant turn that emitted tool calls (verbatim `\{"name":…,"arguments":…}` objects) plus any `text` alongside.
- `set_tools` - Declare the conversation's tools (verbatim OpenAI `tools[]` JSON objects, moved in) BEFORE the first turn renders — the system turn carries the family's tool block.

### Reasoning (thinking models)

- `effective_stop_ids` - The stop ids in force for the NEXT generation: the template's stops plus its thinking-off extras while thinking is off.
- `make_think_stream` - The incremental reasoning/content splitter for `chat`'s next turn — feed streamed pieces through `think_feed`, flush with `think_finish`.
- `split_reasoning` - Split a complete reply at its reasoning boundary per the model family's reply format (`<think>` pair, Harmony channels, gemma-4's thought channel).
- `think_drain` - Drain a COMPLETE reply through the splitter in one call: feed + finish + the strip rule (both halves strip when a reasoning span was consumed).
- `think_feed` - Feed one streamed piece through the splitter; the out-strings are OVERWRITTEN with this piece's reasoning/content deltas (either may be empty while a partial marker is held).
- `think_finish` - Flush the splitter at end-of-generation (OVERWRITES the out-strings with the final deltas): an unclosed reasoning span classifies as reasoning — the truncated-tail rule.

### Operations: prepared images and dispatch

- `dlim_clean` - Garbage-collect `STALE` and `OTHER` images beside `gguf_path` (`FOREIGN`, another flavor's, are always left alone): `apply = false` only reports, `true` removes; `keep_other` spares `OTHER`.
- `dlim_inventory` - List the prepared images (`.dlim`) minted beside `gguf_path` — per image: file, bytes, image version, identity, and a verdict (`CURRENT` loads; `STALE vN` is an older image version; `OTHER` a different bake configuration or box; `FOREIGN` a different flavor).
- `get_dispatch_worker_limit` - The dispatch worker cap in force (0 = no limit) — `set_dispatch_worker_limit`'s read half.
- `get_jobque_spin_us` - The spin window in force — `set_jobque_spin_us`'s read half.
- `get_single_thread` - Whether every kernel runs on the calling thread — `set_single_thread`'s read half.
- `kernel_backend_available` - True when kernel backend `name` is registered AND its availability witness passes on this box — the detection probe behind defaults-first backend selection (the vulkan witness enumerates devices once and caches its verdict).
- `select_matmul_backend_for_load` - Select the best matmul backend for the NEXT model load (honors a pin).
- `set_dispatch_worker_limit` - Cap the kernel-dispatch worker count (0 = no limit, all job-que workers).
- `set_jobque_spin_us` - The workers' spin-before-park window in microseconds (0 = park at once).
- `set_single_thread` - Run every kernel on the calling thread: no dispatch to the job queue at all, whatever queue exists.

### Operations: GPU tier and model slots

- `gpu_model_marks_init` - A fresh no-model marks holder — one per model slot; the marks save/restore pair swaps tier state across switches.
- `gpu_slot_capture` - Capture the INSTALLED model's tier state into `st` right after its `load_model` and classify it — "gpu:resident" | "gpu:rails" | "cpu" — from the CAPTURED marks, never the process-global VRAM counter (it still holds the previous load's bytes on a later CPU load).
- `gpu_slot_rearm` - Re-arm the INSTALLED model onto the GPU tier — want, arm, resident upload (bake-slice path for a mapped vulkan-flavor `t`).
- `gpu_tier_status` - Snapshot of the GPU tier in force: VRAM budget, resident/streamed layer counts, dense planes, engage/decline state — the server's GPU badge reads this.
- `moe_gpu_drop_model` - Drop the INSTALLED model's whole device state (resident stacks, mirrors, VRAM) — the evict half of a slot switch.
- `moe_gpu_hydrate_session` - Pull `s`'s host KV back from the device mirror when it owns live mirror rows (no-op otherwise).
- `moe_gpu_model_marks_restore` - Install `st` as the per-model GPU tier state — `moe_gpu_model_marks_save`'s inverse; `st` reads as no-model after.
- `moe_gpu_model_marks_save` - Save the INSTALLED model's GPU tier state into `st` and disarm it — one half of the multi-model slot switch (the engine's tier state is per-process, not per-model).
- `moe_gpu_tier_arm` - Arm the recorded tier want so a GPU backend can install its hooks — call between `set_gpu_tier_want` and `load_model`.
- `moe_gpu_weight_budget` - The armed backend's resident-weight VRAM budget in bytes (0 = no GPU backend armed).
- `set_gpu_tier_want` - Record the GPU tier request the NEXT `moe_gpu_tier_arm`/`load_model` honors — the programmatic form of the core `DASLLAMA_GPU_*` knobs (an env var present overrides its field; the classifier/dense-arm knobs are env-only).
- `set_resident_prefill_allowed` - Allow or pin out the resident-decode prefill arm.

## dasllama_tts

Text to speech in pure daslang: load a converted StyleTTS2-lineage GGUF (KittenTTS nano and mini, Kokoro-82M), run text through the das-native front end (normalizer, part-of-speech tagger, grapheme-to-phoneme), and synthesize mono f32 PCM per sentence chunk, timed per model stage. Run with `-jit`; `utils/dasllama-server/txt2wav.das` is the canonical program shape, and the server's `/v1/audio/speech` route serves the same facade.


### Constants

- `TTS_CHUNK_CHARS`

### Enumerations

- `TtsKind`

### Structures

- `TtsModel` - A loaded TTS model: the shared assembly, the family's driver data, and the front-end packs read beside the GGUF.
- `TtsCaps` - What a loaded TTS model can do: its voices (canonical names; aliases resolve in the family file), the PCM rate it emits, the languages it speaks, whether it clones a voice from audio, and whether a speed means anything to it.
- `TtsTimings` - Where a synthesis spent its time, in microseconds of wall clock, model loading excluded: the text front end, then each model stage.
- `TtsAudio` - Synthesized speech: mono f32 PCM at `sample_rate`, with the time it took.
- `TtsNoise` - The source noise a synthesis consumed - captured from the oracle for a parity run, or drawn from the session's own generator into a carrier every synthesis reuses.
- `KittenFamily`
- `KokoroFamily`

### Loading and capabilities

- `caps` - What the loaded model offers: the canonical voice names the front end can drive, each with its language (a voice whose language it does not phonemize is left out; aliases resolve at synthesis), the PCM rate, the languages, whether it clones a voice, whether a speed applies.
- `finalize` - Free the model's planes, the carrier and both packs; `delete m` runs it.
- `g2p_pack_path`
- `load_tts_model` - Load a TTS model: a converted GGUF (KittenTTS, Kokoro or Pocket TTS, picked from its architecture) or its prepared `.dlim` image; the phoneme families read `tts_g2p.bin` and `tts_postag.bin` from the same directory (Pocket needs neither).
- `tts_has_phonemes` - Whether `tts_phonemize` has an answer for this model: the phoneme families yes, a Pocket model no - it reads text, and its chunks carry no phoneme string.
- `tts_needs_packs` - Whether the model at `path` reads the front-end packs (`tts_g2p.bin`, `tts_postag.bin`) from its directory: the phoneme families do, a Pocket TTS GGUF reads text and needs neither - decided before any load, by the architecture test `load_tts_model` makes (a `.dlim` is a phoneme family).
- `tts_voice_lang` - The language the front end reads `voice` in - a canonical name from `caps` or a family alias, resolved the way `synthesize` resolves it.

### Voices

- `tts_register_voice` - Clone a voice: `pcm` (mono, at the model's own sample rate - `caps().sample_rate`) joins the roster under `name` and speaks from the next synthesis on.

### Text front end

- `tts_chunks` - The chunks a synthesis on this model takes, by the family's own driver rule: Kitten's appends a comma to a chunk the split left bare, Kokoro's sends the text as it is (its voices render an added mark as a breath), Pocket's splits on its tokenizer's sentence marks under a token budget.
- `tts_normalize` - The spoken form of `text`: the normalization pass every synthesis runs first (rules alone - it reads no pack), where "Dr." reads "Doctor", "3.5" reads "three point five" and "$12" reads "twelve dollars".
- `tts_phonemize` - The phonemes one already-normalized sentence becomes, in the front end's own American inventory (the tagger of `tts_postag.bin` labels the tokens, the grapheme-to-phoneme pass of `tts_g2p.bin` reads them); a family wanting other symbols rewrites them.

### Synthesis

- `synthesize` - Text -> speech: normalize, chunk by sentence, phonemize, map into the family's symbols, synthesize; the chunks concatenate, the timings sum.
- `synthesize_stream` - Text -> speech, one sentence-sized chunk at a time: the block receives each chunk's audio as soon as it exists, in order.

### The served lane

- `reset_tts_q8` - Drop the `set_tts_q8` pin: the next load follows the policy default again.
- `set_tts_q8` - Pin the GEMM weights' format for subsequent TTS loads of every family: Q8_0 quants (their own prepared image beside the GGUF) or the file's f32 planes; `reset_tts_q8` returns to the policy default.
- `tts_serves_q8` - Would the next TTS load serve its rows GEMMs as q8 - the pin when set, the policy otherwise.

### Timings

- `rtf` - Real-time factor: generation seconds per second of audio produced.
- `timings_line` - One line for the log: seconds of audio, wall time, RTF and the per-stage split in ms - the stages a family ran.

## debugapi

The DEBUGAPI module provides the debug agent infrastructure — creating, installing, and communicating with persistent debug agents that live in their own forked contexts. It supports cross-context function invocation, agent method calls, log interception, data and stack walking, instrumentation, and breakpoint management.


### Handled structures

- `Prologue` - Annotation for inspecting function call prologues.
- `DataWalker` - Annotation for declaring a data walker class.
- `StackWalker` - Annotation for declaring a stack walker class.
- `DebugAgent` - Annotation for declaring a debug agent class.

### Agent lifecycle

- `delete_debug_agent_context` - Removes the debug agent with the given category name.
- `fork_debug_agent_context` - Clones the current context and calls the setup function inside the clone.
- `get_debug_agent_context` - Returns a reference to the `Context` of the named debug agent.
- `has_debug_agent_context` - Returns `true` if a debug agent with the given category name is currently installed.
- `install_debug_agent` - Installs a low-level `smart_ptr<DebugAgent>` under the given category name.
- `install_debug_agent_thread_local` - Installs a low-level `smart_ptr<DebugAgent>` as the thread-local debug agent.
- `is_in_debug_agent_creation` - Returns `true` if the current thread is inside a `fork_debug_agent_context` call.
- `lock_debug_agent` - Executes the block while holding the global debug agent mutex.

### Cross-context invocation

- `invoke_debug_agent_function` - Calls an `[export, pinvoke]` function in the named agent's context.
- `invoke_debug_agent_method` - Calls a method on the debug agent's class instance by name.
- `invoke_in_context` - Calls a function in another context.
- `try_invoke_in_context` - The non-blocking `invoke_in_context`: tries the target context's lock instead of waiting on it.

### Agent construction

- `make_data_walker` - Wraps a `DapiDataWalker` class pointer into a `DataWalker?` for use with `walk_data`.
- `make_debug_agent` - Low-level constructor that wraps a `DapiDebugAgent` class pointer into a `smart_ptr<DebugAgent>`.
- `make_stack_walker` - Wraps a `DapiStackWalker` class pointer into a `smart_ptr<StackWalker>` for use with `walk_stack`.

### Agent tick and state collection

- `collect_debug_agent_state` - Triggers `onCollect` on all installed debug agents, passing the calling context and line info.
- `debug_agent_command` - Sends a user-defined command string to all debug agents.
- `debugger_stop_requested` - Returns `true` if any debug agent has requested the program to stop (e.g.
- `debugger_thread_context_ready` - Internal debugger hook that releases a waiting worker.
- `on_breakpoints_reset` - Notifies all debug agents that breakpoints for the given file have been reset.
- `report_context_state` - Reports a named variable from inside `onCollect` back to the debug system.
- `tick_debug_agent` - Calls `onTick` on all installed debug agents (no-arg variant), or on a specific named agent (string variant).

### Instrumentation

- `clear_instruments` - Removes all instrumentation from the given context, restoring original execution.
- `instrument_all_functions` - Enables or disables instrumentation for all functions in the given context.
- `instrument_all_functions_thread_local` - Thread-local variant of `instrument_all_functions`.
- `instrument_context_allocations` - Enables or disables allocation tracking on the given context.
- `instrument_function` - Enables or disables instrumentation for a specific function in the given context.
- `instrument_node` - Enables or disables per-node instrumentation on the given context.
- `set_single_step` - Enables or disables single-step execution on the given context.

### Data and stack walking

- `get_stackwalk` - Returns a human-readable stack trace of the given context as a string — the same report `stackwalk` prints, captured instead of logged.
- `stack_depth` - Returns the current call stack depth of the given context.
- `stackwalk` - Prints a human-readable stack trace of the given context to the log output.
- `walk_data` - Walks a daslang data structure using the provided `DataWalker`.
- `walk_stack` - Walks the call stack of the given context using the provided `StackWalker`.

### Context inspection

- `get_context_global_variable` - Returns a pointer to a global variable in the given context, looked up by name (string variant) or by index (int variant).
- `get_heap_stats` - Writes the heap allocation statistics of the given context into the provided `uint64` pointer.
- `has_function` - Returns `true` if the given context contains an exported function with the specified name.

### Breakpoints

- `clear_hw_breakpoint` - Clears the hardware breakpoint with the given index.
- `set_hw_breakpoint` - Sets a hardware breakpoint at the given memory address.

### Memory

- `break_on_free` - Triggers a debug break when the specified memory region is freed.
- `free_temp_string` - Frees all temporary string allocations in the given context.
- `temp_string_size` - Returns the total size in bytes of temporary string allocations in the given context.
- `track_insane_pointer` - Begins tracking the specified pointer for dangling-reference detection.

## rtti

The RTTI module exposes runtime type information and program introspection facilities. It allows querying module structure, type declarations, function signatures, annotations, and other compile-time metadata at runtime. Used primarily by macro libraries and code generation tools.


### Type aliases

- `ProgramFlags` - Flags which represent state of the `Program` object, both during and after compilation.
- `context_category_flags` - Flags which specify type of the `Context`.
- `TypeInfoFlags` - Flags which specify properties of the `TypeInfo` object (any rtti type).
- `StructInfoFlags` - Flags which represent properties of the `StructInfo` object (rtti object which represents structure type).
- `ModuleFlags` - Flags which represent the module's state.
- `AnnotationDeclarationFlags` - Flags which represent properties of the `AnnotationDeclaration` object.
- `SimFunctionFlags` - properties of the `SimFunction` object.
- `LocalVariableInfoFlags` - properties of the `LocalVariableInfo` object.
- `RttiValue` - Variant type which represents value of any annotation arguments and variable annotations.
- `FileAccessPtr` - Type alias for `smart_ptr<FileAccess>` — a reference-counted pointer to a `FileAccess` object, used as the standard way to pass file access to the compiler.

### Constants

- `FUNCINFO_INIT` - Bit flag constant on `FuncInfo.flags` indicating that the function runs during `Context` initialization (`[init]` attribute).
- `FUNCINFO_BUILTIN` - Bit flag constant on `FuncInfo.flags` indicating that the function is a built-in (C++-bound) function rather than a daslang-defined one.
- `FUNCINFO_PRIVATE` - Bit flag constant on `FuncInfo.flags` indicating that the function has `[private]` visibility and cannot be called from other modules.
- `FUNCINFO_SHUTDOWN` - Bit flag constant on `FuncInfo.flags` indicating that the function runs during `Context` shutdown (`[finalize]` attribute).
- `FUNCINFO_LATE_INIT` - Bit flag constant on `FuncInfo.flags` indicating the function uses late initialization with a custom init order (`[init(order)]` attribute).

### Enumerations

- `CompilationError` - Enumeration which represents error type for each of the errors which compiler returns at various stages.
- `ConstMatters` - Yes or no flag which indicates if constant flag of the type matters (during comparison).
- `RefMatters` - Yes or no flag which indicates if reference flag of the type matters (during comparison).
- `TemporaryMatters` - Yes or no flag which indicates if temporary flag of the type matters (during comparison).
- `Type` - One of the fundamental (base) types of any type object.

### Handled structures

- `Program.getThisModule` - Property-like accessor that returns the `Module` pointer for the module currently being inferred in the given `Program`.
- `Program.getDebugger` - Property-like accessor that returns `true` if the debugger is attached and enabled for the given `Program`.
- `Program.getOptimize` - Property-like accessor that returns `true` when the optimizer runs for the given `Program`: false under the host's `no_optimizations` policy or any of the program's `options optimize = false`, `options no_optimization`, `options no_optimizations`.
- `Program` - Object representing full information about Daslang program during and after compilation (but not the simulated result of the program).
- `AnnotationArgumentInfo` - One argument of an annotation, deep-copied into the context debug heap (never points into the AST).
- `CodeOfPolicies` - Object which holds compilation and simulation settings and restrictions.
- `LocalVariableInfo` - Object which represents local variable declaration.
- `AstSerializer` - Dummy annotation to strengthen type system.
- `Context.totalFunctions` - Property-like accessor that returns the total number of registered `SimFunction` entries in the given `Context`.
- `Context.totalVariables` - Property-like accessor that returns the total number of global variables registered in the given `Context`.
- `Context.getCodeAllocatorId` - Property-like accessor that returns a non-persistent unique integer ID of the code (node) allocator associated with the given `Context`.
- `Context` - Object which holds single Daslang Context.
- `VarInfo` - Object which represents variable declaration.
- `TypeAnnotation.is_any_vector` - Property-like accessor that returns `true` if the given `TypeAnnotation` wraps any C++ vector-like container (e.g., `std::vector`).
- `TypeAnnotation.canMove` - Property-like accessor that returns `true` if the given `TypeAnnotation` supports move semantics.
- `TypeAnnotation.canCopy` - Property-like accessor that returns `true` if the given `TypeAnnotation` supports copy semantics.
- `TypeAnnotation.canClone` - Property-like accessor that returns `true` if the given `TypeAnnotation` supports the clone operation.
- `TypeAnnotation.isPod` - Property-like accessor that returns `true` if the given `TypeAnnotation` is a POD (plain old data) type — no constructor, destructor, or special semantics.
- `TypeAnnotation.isRawPod` - Property-like accessor that returns `true` if the given `TypeAnnotation` is a raw POD type — a basic value type excluding pointers and strings.
- `TypeAnnotation.isRefType` - Property-like accessor that returns `true` if the given `TypeAnnotation` is always passed by reference, or is itself a reference type.
- `TypeAnnotation.hasNonTrivialCtor` - Property-like accessor that returns `true` if the given `TypeAnnotation` has a non-trivial constructor (requires explicit initialization).
- `TypeAnnotation.hasNonTrivialDtor` - Property-like accessor that returns `true` if the given `TypeAnnotation` has a non-trivial destructor (requires explicit finalization).
- `TypeAnnotation.hasNonTrivialCopy` - Property-like accessor that returns `true` if the given `TypeAnnotation` has non-trivial copy semantics (i.e., a custom copy constructor).
- `TypeAnnotation.canBePlacedInContainer` - Property-like accessor that returns `true` if values of the given `TypeAnnotation` can be stored inside arrays, tables, or other containers.
- `TypeAnnotation.isLocal` - Property-like accessor that returns `true` if the given `TypeAnnotation` can be used as a local variable type within a function.
- `TypeAnnotation.canNew` - Property-like accessor that returns `true` if the given `TypeAnnotation` supports heap allocation via `new`.
- `TypeAnnotation.canDelete` - Property-like accessor that returns `true` if values of the given `TypeAnnotation` can be explicitly deleted.
- `TypeAnnotation.needDelete` - Property-like accessor that returns `true` if values of the given `TypeAnnotation` require explicit `delete` to free resources.
- `TypeAnnotation.canDeletePtr` - Property-like accessor that returns `true` if a pointer to the given `TypeAnnotation` type can be explicitly deleted.
- `TypeAnnotation.isIterable` - Property-like accessor that returns `true` if the given `TypeAnnotation` supports iteration via `for`.
- `TypeAnnotation.isShareable` - Property-like accessor that returns `true` if the given `TypeAnnotation` can be shared across multiple `Context` objects.
- `TypeAnnotation.isSmart` - Property-like accessor that returns `true` if the given `TypeAnnotation` represents a `smart_ptr` managed type.
- `TypeAnnotation.avoidNullPtr` - Property-like accessor that returns `true` if the given `TypeAnnotation` requires pointers to its type to be non-null (i.e., must be initialized on creation).
- `TypeAnnotation.sizeOf` - Property-like accessor that returns the size in bytes of the type described by the given `TypeAnnotation`.
- `TypeAnnotation.alignOf` - Property-like accessor that returns the memory alignment requirement (in bytes) of the type described by the given `TypeAnnotation`.
- `TypeAnnotation` - Handled type.
- `FileInfo` - Information about a single file stored in the `FileAccess` object.
- `EnumInfo` - Type object which represents enumeration.
- `Module` - Collection of types, aliases, functions, classes, macros etc under a single namespace.
- `Error` - Object which holds information about compilation error or exception.
- `LineInfo` - Information about a section of the file stored in the `FileAccess` object.
- `TypeInfo.enumType` - Property-like accessor that returns the `EnumInfo` pointer describing the underlying enumeration for the given enum `TypeAnnotation`.
- `TypeInfo.isRef` - Property-like accessor that returns `true` if the given `TypeInfo` describes a reference (`&`) type.
- `TypeInfo.isRefType` - Property-like accessor that returns `true` if the given `TypeAnnotation` is always passed by reference, or is itself a reference type.
- `TypeInfo.isRefValue` - Property-like accessor that returns `true` if the given `TypeInfo` describes a ref-value type (boxed value accessed by reference).
- `TypeInfo.canCopy` - Property-like accessor that returns `true` if the given `TypeAnnotation` supports copy semantics.
- `TypeInfo.isPod` - Property-like accessor that returns `true` if the given `TypeAnnotation` is a POD (plain old data) type — no constructor, destructor, or special semantics.
- `TypeInfo.isRawPod` - Property-like accessor that returns `true` if the given `TypeAnnotation` is a raw POD type — a basic value type excluding pointers and strings.
- `TypeInfo.isConst` - Property-like accessor that returns `true` if the given `TypeInfo` describes a `const`-qualified type.
- `TypeInfo.isTemp` - Property-like accessor that returns `true` if the given `TypeInfo` describes a temporary (`#`) type that cannot be captured or stored.
- `TypeInfo.isImplicit` - Property-like accessor that returns `true` if the given `TypeInfo` describes an implicit (compiler-inferred) type.
- `TypeInfo.annotation` - Property-like accessor that returns the `Annotation` pointer associated with the given `TypeInfo`.
- `TypeInfo.annotation_or_name` - Property-like accessor that returns the annotation name if one exists, otherwise returns the raw type name from the given `TypeInfo`.
- `TypeInfo.structType` - Property-like accessor that returns the `StructInfo` pointer for the struct described by the given `TypeInfo`, or null if not a struct type.
- `TypeInfo` - Object which represents any Daslang type.
- `FuncInfo` - Object which represents function declaration.
- `SimFunction.lineInfo` - Property-like accessor that returns the `LineInfo` (source location) associated with the given function's `FuncInfo`.
- `SimFunction` - Object which represents simulated function in the `Context`.
- `AnnotationArgument` - Single argument of the annotation, typically part of the `AnnotationArgumentList`.
- `DebugInfoHelper` - Helper object which holds debug information about the simulated program.
- `AnnotationDeclaration` - Annotation declaration, its location, and arguments.
- `AnnotationInfo` - One annotation attached to a structure, function, or enumeration - name, declaring module, and arguments, deep-copied into the context debug heap so it stays valid after the Program is released.
- `BasicStructureAnnotation.fieldCount` - Property-like accessor that returns the number of fields declared in the given `BasicStructureAnnotation`.
- `BasicStructureAnnotation` - Handled type which represents a structure-like annotation for exposing C++ types to daslang.
- `FileAccess` - Object which holds collection of files as well as means to access them (Project).
- `Annotation.isTypeAnnotation` - Property-like accessor that returns `true` if the given `Annotation` is a `TypeAnnotation` (defines a handled type).
- `Annotation.isBasicStructureAnnotation` - Property-like accessor that returns `true` if the given `Annotation` is a `BasicStructureAnnotation`, which exposes C++ struct fields to daslang.
- `Annotation.isStructureAnnotation` - Property-like accessor that returns `true` if the given `Annotation` is a structure annotation (applied to struct declarations).
- `Annotation.isStructureTypeAnnotation` - Property-like accessor that returns `true` if the given `Annotation` is a `StructureTypeAnnotation`, which binds a C++ class as a daslang handled struct.
- `Annotation.isFunctionAnnotation` - Property-like accessor that returns `true` if the given `Annotation` is a `FunctionAnnotation` (applied to functions).
- `Annotation.isEnumerationAnnotation` - Property-like accessor that returns `true` if the given `Annotation` is an `EnumerationAnnotation`.
- `Annotation` - Handled type or macro.
- `EnumValueInfo` - Single element of enumeration, its name and value.
- `ModuleGroup` - Collection of modules.
- `StructInfo` - Type object which represents structure or class.

### Typeinfo macros

- `rtti_typeinfo` - Typeinfo macro that provides compile-time access to RTTI type information structures.

### Handled types

- `recursive_mutex` - Handled type wrapping a system `std::recursive_mutex`, used with `lock_mutex` for thread-safe access to shared data across contexts.
- `AnnotationList` - Handled type representing all annotations attached to a single object (function, structure, or variable), iterable via `each`.
- `AnnotationArgumentList` - Handled type representing an ordered list of annotation arguments and properties, providing indexed and named access to argument entries.
- `NameLookup` - Handled type wrapping the runtime's sealed function or global name lookup (`das::NameLookup`): a perfect-hash table over mangled-name hashes and plain names.
- `AnnotationArguments` - Handled type representing a collection of annotation arguments, typically the raw argument list parsed from an annotation declaration.

### Initialization and finalization

- `RttiValue_nothing` - Constructs an `RttiValue` variant set to the `nothing` alternative, representing an absent or void value.
- `using` - Creates a temporary RTTI helper object (e.g., `Program`, `DebugInfoHelper`) scoped to the given block, automatically finalized on block exit.

### Type access

- `arg_names` - Iterates through the argument names of an RTTI type, yielding each name as a `string` — used for inspecting function or call-site parameter names.
- `arg_types` - Iterates through the argument types of an RTTI type, yielding each element as a `TypeInfo` pointer — used for inspecting function or call-site parameter types.
- `builtin_is_same_type` - Returns `true` if two `TypeInfo` pointers describe the same type, with flags controlling whether ref, const, temp, and other qualifiers are included in the comparison.
- `each_dim` - Iterates through the dimension sizes of a fixed-size array `TypeInfo`, yielding each `int` dimension value (e.g., `int[3][4]` yields 3 then 4).
- `get_das_type_name` - Returns the canonical `string` name of the given `Type` enumeration value (e.g., `tInt` → `"int"`).
- `get_dim` - Returns the dimension size (`int`) at the specified index for a fixed-size array type described by `TypeInfo`.
- `get_type_align` - Returns the memory alignment (`int`, in bytes) of the type described by the given `TypeInfo`.
- `get_type_size` - Returns the size (`int`, in bytes) of the type described by the given `TypeInfo`.
- `is_compatible_cast` - Returns `true` if an object of type `from` (`StructInfo`) can be safely cast to type `to` (`StructInfo`), following the class hierarchy.
- `is_same_type` - Returns `true` if two `TypeInfo` objects describe the same type, with flags controlling comparison of qualifiers (ref, const, temp, etc.).

### Rtti context access

- `class_info` - Returns a `StructInfo` pointer for the given class instance, enabling runtime introspection of its fields and annotations via RTTI.
- `context_for_each_function` - Iterates through all functions in the given `Context`, yielding a `FuncInfo` pointer for each registered function.
- `context_for_each_variable` - Iterates through all global variables in the given `Context`, yielding a `VarInfo` pointer for each registered variable.
- `for_each_init_function` - Iterates through the `[init]` functions of the given `Context` in the order the runtime executes them (late init last), yielding each function's mangled name hash.
- `get_function_by_mnh` - Returns a `SimFunction` pointer looked up by mangled name hash — an alternative form of `get_function_address`.
- `get_function_info` - Returns the `FuncInfo` pointer for a function at the given index in the `Context`, providing access to its name, arguments, and return type.
- `get_line_info` - Returns a `LineInfo` structure representing the source location (file, line, column) of the call site where `get_line_info` is invoked.
- `get_total_functions` - Returns the total number of registered functions (`int`) in the given `Context`.
- `get_total_variables` - Returns the total number of global variables (`int`) in the given `Context`.
- `get_variable_info` - Returns the `VarInfo` pointer for a global variable at the given index in the `Context`, providing access to its name, type, and offset.
- `get_variable_value` - Returns an `RttiValue` variant representing the current value of a global variable, looked up by `VarInfo` in the given `Context`.
- `this_context` - Returns a pointer to the current `Context` in which the calling code is executing.
- `type_info` - Returns the `TypeInfo` object for the specified local variable or expression, resolved at compile time via the `[typeinfo(...)]` macro.

### Program access

- `get_module` - Returns a `Module` pointer looked up by module name `string`, or null if no such module is registered.
- `get_this_module` - Returns the `Module` pointer for the module currently being compiled or inferred, retrieved from the `Program`.
- `has_module` - Returns `true` if a module with the given name is registered, or waits in a `.das_module.manifest` row for the first `require` that names it, `false` otherwise.
- `module_group_for_each_member` - Calls `block` once per module registered under `group` - the require paths a `require [group]` expands to, sorted by member path - and not at all for a group nothing joined.
- `program_for_each_module` - Iterates through all modules referenced by the given `Program` (including transitive dependencies), yielding a `Module` pointer for each.
- `program_for_each_registered_module` - Iterates through all modules registered in the daslang runtime (globally, not per-program), yielding a `Module` pointer for each.

### Module access

- `module_for_each_annotation` - Iterates through each annotation (handled type) in the given `Module`, yielding an `Annotation` pointer for each registered annotation.
- `module_for_each_dependency` - Iterates through each module dependency of the given `Module`, yielding the dependent `Module` pointer for each required module.
- `module_for_each_enumeration` - Iterates through each enumeration declared in the given `Module`, yielding an `EnumInfo` pointer for each enum.
- `module_for_each_function` - Iterates through each function declared in the given `Module`, yielding a `FuncInfo` pointer for each function.
- `module_for_each_generic` - Iterates through each generic (template) function declared in the given `Module`, yielding a `FuncInfo` pointer for each generic.
- `module_for_each_global` - Iterates through each global variable declared in the given `Module`, yielding a `VarInfo` pointer for each variable.
- `module_for_each_structure` - Iterates through each structure declaration in the given `Module`, yielding a `StructInfo` pointer for each struct.

### Annotation access

- `add_annotation_argument` - Appends an annotation argument (name-value pair) to the given `AnnotationArgumentList`, used when constructing annotations programmatically.
- `each_annotation`
- `each_annotation_argument`
- `get_annotation` - Returns the AnnotationInfo at the given index for a `StructInfo`, `FuncInfo`, or `EnumInfo`.
- `get_annotation_argument` - Returns the AnnotationArgumentInfo at the given index of an `AnnotationInfo` (or the field/global annotation arguments of a `VarInfo`).
- `get_annotation_argument_value` - Returns an `RttiValue` variant representing the value of a specific named argument from an `AnnotationArgumentList`.
- `resolve_annotation` - Resolves an `AnnotationInfo` to the live `Annotation` object by looking up its module and name among the registered modules.

### Compilation and simulation

- `ast_serializer_finalize_usec` - Microseconds the reads through `deserialize_program` on this deserializer spent inside finalizeModule - the annotations, the macro-module re-simulation and the gc_collect of every program it restored; the decode is the read minus this and `ast_serializer_setup_usec`.
- `ast_serializer_get_data` - Returns content of serializer.
- `ast_serializer_setup_usec` - Microseconds the reads through `deserialize_program` on this deserializer spent in the program setup that follows the stream - symbol marking and stack allocation - over every program it restored.
- `create_ast_deserializer` - Creates deserializer.
- `create_ast_serializer` - Creates serializer object.
- `delete_ast_serializer` - Frees memory for ast_serializer.
- `deserialize_program` - Deserializes the next program from the stream and calls `block` with it, the way the two-argument form does, and gives the restored program `access` as its own: a stream carries no file access, and a `require_module_now` issued from the program's macros or `[init]` walks through its program's access, so a reader that restores programs whose code requires modules late passes the access it would have compiled them with.
- `for_each_expected_error` - Iterates through each expected compilation error declared in the `Program` (via `expect`), yielding the error code for each.
- `for_each_require_declaration` - Iterates through each `require` declaration of the compiled `Program`, yielding the module name, public/private flag, and source `LineInfo`.
- `serialize_program` - Serializes program to serializer object.
- `simulate` - Simulates (links and initializes) a compiled `Program`, returning a `Context` pointer ready for function execution, or null on failure.

### File access

- `add_extra_module` - Adds extra module to `FileAccess`.
- `add_file_access_root` - Adds an extra root directory (search path) to the given `FileAccess` object, expanding where `require` resolves files from.
- `set_file_source` - Registers a source code `string` for the given file name inside the `FileAccess` object, allowing in-memory compilation without disk files.

### Structure access

- `basic_struct_for_each_field` - Iterates through each field of a `BasicStructureAnnotation`, yielding the field name, C++ name, `TypeInfo`, and byte offset for each field.
- `basic_struct_for_each_parent` - Iterates through each parent (base class) of a `BasicStructureAnnotation`, yielding the parent `TypeInfo` for each ancestor.
- `rtti_builtin_structure_for_each_annotation` - Iterates through each annotation attached to a `StructInfo`, yielding the annotation name and its `AnnotationArgumentList` for each.
- `structure_for_each_annotation` - Iterates through each annotation attached to a `StructInfo`, yielding the annotation name and `AnnotationArgumentList` — an alias of `rtti_builtin_structure_for_each_annotation`.

### Data walking and printing

- `describe` - Returns a human-readable `string` description of an RTTI object (`TypeInfo`, `VarInfo`, `FuncInfo`, etc.), useful for logging and debug output.
- `get_mangled_name` - Returns the full mangled name `string` for the given `FuncInfo`, encoding its module, name, and argument types.
- `sprint_data` - Returns a `string` representation of a value given its data pointer and `TypeInfo`, similar to `debug` or `print` but capturing output as a string.
- `sprint_json_at` - Returns the JSON representation of a value given its raw `addr` and the `TypeInfo` of its type.
- `sscan_json_at` - Parses a JSON `string` and writes the result into the memory at `addr` using the supplied `TypeInfo`.

### Function and mangled name hash

- `get_function_address` - Returns a `SimFunction` pointer looked up by mangled name hash in the given `Context`, or null if not found.
- `get_function_by_mangled_name_hash` - Returns a `function<>` lambda value looked up by its mangled name hash in the given `Context`.
- `get_function_mangled_name_hash` - Returns the `uint64` mangled name hash for the given `function<>` value, which uniquely identifies the function in its `Context`.

### Context and mutex locking

- `lock_context` - Acquires a recursive lock on the given `Context` and executes a block, ensuring thread-safe access to context data within the scope.
- `lock_mutex` - Acquires a recursive lock on the given `recursive_mutex` and executes a block, releasing the lock when the block exits.
- `lock_this_context` - Acquires a recursive lock on the current `Context` and executes a block, ensuring thread-safe access within the scope.

### Name lookup builder

- `name_lookup_count` - Number of entries a sealed lookup holds.
- `name_lookup_create` - Creates an empty `NameLookup` builder and returns a raw pointer the caller owns; release it with `name_lookup_destroy`.
- `name_lookup_destroy` - Deletes a `NameLookup` created by `name_lookup_create`.
- `name_lookup_entry_index` - Index stored in one entries slot - the position in the context's function or global array; `0xffffffff` in an empty slot.
- `name_lookup_entry_mnh` - Mangled-name hash stored in one entries slot, for `slot` below `name_lookup_mnh_slots`; zero in an empty slot.
- `name_lookup_entry_next` - Entries slot of the next entry carrying the same plain name, `-1` at the end of the chain.
- `name_lookup_entry_value` - Value stored in one entries slot - the function index or global byte offset a mangled-name probe answers; `0xffffffff` in an empty slot.
- `name_lookup_insert` - Stages one entry in an unsealed lookup: `mnh` is the mangled-name hash the runtime probes with, `name` the plain name (copied, so the string need not outlive the call), `index` the position in the context's function or global array, and `value` what a mangled-name probe answers - a function index, or a global's byte offset.
- `name_lookup_mnh_buckets` - Bucket count of a sealed lookup's mangled-name hash - the length of the displacement array `name_lookup_mnh_disp` reads.
- `name_lookup_mnh_disp` - Displacement of one bucket of the mangled-name hash, for `bucket` below `name_lookup_mnh_buckets`.
- `name_lookup_mnh_slots` - Slot count of a sealed lookup's mangled-name hash - the length of the entries array the `name_lookup_entry_*` functions read.
- `name_lookup_name_buckets` - Bucket count of a sealed lookup's plain-name hash - the length of the displacement array `name_lookup_name_disp` reads.
- `name_lookup_name_disp` - Displacement of one bucket of the plain-name hash, for `bucket` below `name_lookup_name_buckets`.
- `name_lookup_name_hash` - Plain-name hash stored in one name slot, for `slot` below `name_lookup_name_slots`; zero in an empty slot.
- `name_lookup_name_head` - Entries slot of the first entry whose plain name hashes into this name slot, `-1` when the slot is empty.
- `name_lookup_name_slots` - Slot count of a sealed lookup's plain-name hash - the length of the name-slot array `name_lookup_name_hash` and `name_lookup_name_head` read.
- `name_lookup_seal` - Builds the two perfect hashes over every staged entry and freezes the lookup; panics when two entries share a mangled-name hash or two different names share a name hash, naming both entries.

### Runtime data access

- `get_table_key_index` - Returns the internal slot index (`int`) for the given key within a `table` value, or `-1` if the key is not present.

### Tuple and variant access

- `get_tuple_field_offset` - Returns the byte offset (`int`) of a field at the given index within a tuple type described by `TypeInfo`.
- `get_variant_field_offset` - Returns the byte offset (`int`) of a field at the given index within a variant type described by `TypeInfo`.

### Lint suppression

- `extract_lint_code`
- `is_lint_suppressed`
- `rtti_get_source_line` - Returns the source text of a single line (1-based) from a FileInfo as a freshly-allocated string.
- `rtti_is_nolint_suppressed` - Returns true if the source line at the given FileInfo+line carries a `// nolint:CODE` directive listing `code`.
- `was_nolint_consumed`

### Iteration

- `each` - Iterates through each element of an RTTI container (e.g., `AnnotationArguments`, `AnnotationArgumentList`, `AnnotationList`), yielding individual entries.

## ast

The AST module provides access to the abstract syntax tree representation of daslang programs. It defines node types for all language constructs (expressions, statements, types, functions, structures, enumerations, etc.), visitors for tree traversal, and utilities for AST construction and manipulation. This module is the foundation for writing macros, code generators, and source-level program transformations.


### Type aliases

- `TypeDeclFlags` - properties of the `TypeDecl` object.
- `FieldDeclarationFlags` - properties of the `FieldDeclaration` object.
- `StructureFlags` - properties of the `Structure` object.
- `ExprGenFlags` - generation (genFlags) properties of the `Expression` object.
- `ExprLetFlags` - properties of the `ExprLet` object.
- `ExprFlags` - properties of the `Expression` object.
- `ExprPrintFlags` - printing properties of the `Expression` object.
- `FunctionFlags` - properties of the `Function` object.
- `MoreFunctionFlags` - additional properties of the `Function` object.
- `MoreFunctionFlags2` - Overflow word for function flags (moreFlags is full at 32 bits).
- `FunctionSideEffectFlags` - side-effect properties of the `Function` object.
- `VariableFlags` - properties of the `Variable` object.
- `VariableAccessFlags` - access properties of the `Variable` object.
- `VariableAccessInfoFlags` - informational access-detail flags of the `Variable` object.
- `ExprBlockFlags` - properties of the `ExprBlock` object.
- `ExprAtFlags` - properties of the `ExprAt` object.
- `ExprMakeLocalFlags` - properties of the `ExprMakeLocal` object (`ExprMakeArray`, `ExprMakeStruct`, 'ExprMakeTuple', 'ExprMakeVariant').
- `ExprAscendFlags` - properties of the `ExprAscend` object.
- `ExprCastFlags` - properties of the `ExprCast` object.
- `ExprVarFlags` - properties of the `ExprVar` object.
- `ExprMakeStructFlags` - properties of the `ExprMakeStruct` object.
- `MakeFieldDeclFlags` - Properties of the `MakeFieldDecl` object.
- `ExprFieldDerefFlags` - dereferencing properties of the `ExprField` object.
- `ExprFieldFieldFlags` - field properties of the `ExprField` object.
- `ExprSwizzleFieldFlags` - properties of the `ExprSwizzle` object.
- `ExprYieldFlags` - properties of the `ExprYield` object.
- `ExprReturnFlags` - properties of the `ExprReturn` object.
- `ExprMakeBlockFlags` - properties of the `ExprMakeBlock` object.
- `CopyFlags` - properties of the `ExprCopy` object.
- `MoveFlags` - Properties of the `ExprMove` object.
- `IfFlags` - properties of the `ExprIf` object.
- `StringBuilderFlags` - properties of the `ExprStringBuilder` object.
- `ExpressionPtr` - Smart pointer to an `Expression` object.
- `ProgramPtr` - Smart pointer to a `Program` object.
- `TypeDeclPtr` - Smart pointer to a `TypeDecl` object.
- `VectorTypeDeclPtr` - Smart pointer to a `das::vector<ExpressionPtr>`.
- `EnumerationPtr` - Smart pointer to an `Enumeration` object.
- `StructurePtr` - Smart pointer to a `Structure` object.
- `FunctionPtr` - Smart pointer to a `Function` object.
- `VariablePtr` - Smart pointer to a `Variable` object.
- `MakeFieldDeclPtr` - Smart pointer to a `MakeFieldDecl` object.
- `ExprMakeBlockPtr` - Smart pointer to an `ExprMakeBlock` expression.
- `FunctionAnnotationPtr` - Smart pointer to a `FunctionAnnotation` object.
- `StructureAnnotationPtr` - Smart pointer to a `StructureAnnotation` object.
- `EnumerationAnnotationPtr` - Smart pointer to an `EnumerationAnnotation` object.
- `PassMacroPtr` - Pointer to a `PassMacro` object.
- `VariantMacroPtr` - Pointer to a `VariantMacro` object.
- `ReaderMacroPtr` - Pointer to a `ReaderMacro` object.
- `CommentReaderPtr` - Pointer to a `CommentReader` object.
- `CallMacroPtr` - Pointer to a `CallMacro` object.
- `TypeInfoMacroPtr` - Pointer to a `TypeInfoMacro` object.
- `ForLoopMacroPtr` - Pointer to a `ForLoopMacro` object.
- `CaptureMacroPtr` - Pointer to a `CaptureMacro` object.
- `TypeMacroPtr` - Pointer to a `TypeMacro` object.
- `SimulateMacroPtr` - Pointer to a `SimulateMacro` object.

### Enumerations

- `CaptureMode` - Enumeration with lambda variables capture modes.
- `SideEffects` - Enumeration with all possible side effects of expression or function.

### Handled structures

- `ExprNamedCall` - Named call (`call([argname1=expr1, argname2=expr2])`).
- `Variable.isAccessUnused` - Returns whether the given variable is never accessed in the code.
- `Variable.getMangledNameHash` - Returns the mangled name hash of the given function.
- `Variable` - Variable declaration.
- `ExprConstFloat3.getValue` - Returns the constant value stored in this expression node.
- `ExprConstFloat3` - Holds float3 constant.
- `ExprRef2Ptr` - Addr expresion (`addr(expr)`)
- `FunctionAnnotation` - Adapter for the `AstFunctionAnnotation`.
- `ExprWhile` - While loop (`while expr {your; block; here;}`)
- `ExprLooksLikeCall` - Anything which looks like call (`call(expr1,expr2)`).
- `ExprConstUInt3.getValue` - Returns the constant value stored in this expression node.
- `ExprConstUInt3` - Holds uint3 constant.
- `ExprConstInt2.getValue` - Returns the constant value stored in this expression node.
- `ExprConstInt2` - Holds int2 constant.
- `ExprConstFloat4.getValue` - Returns the constant value stored in this expression node.
- `ExprConstFloat4` - Holds float4 constant.
- `ExprContinue` - Continue expression (`continue`)
- `ExprConst` - Compilation time constant expression base class
- `InferHistory` - Generic function infer history.
- `ExprGoto` - Goto expression (`goto label 13`, `goto x`)
- `Function.origin` - Returns the origin function, indicating which generic function this was instantiated from, if any.
- `Function.getMangledNameHash` - Returns the mangled name hash of the given function.
- `Function.isGeneric` - Returns whether the given function is a generic function.
- `Function` - Function declaration.
- `ModuleLibrary` - Object which holds list of `Module` and provides access to them.
- `ExprCopy` - Copy operator (`expr1 = expr2`)
- `ExprConstRange64.getValue` - Returns the constant value stored in this expression node.
- `ExprConstRange64` - Holds range64 constant.
- `Expression` - Any expression (base class).
- `AstContext` - Lexical context for the particular expression.
- `Structure.sizeOf` - Returns the size of the given type in bytes.
- `Structure` - Structure declaration.
- `ExprUnsafe` - Unsafe expression (`unsafe(addr(x))`)
- `ExprReturn` - Return expression (`return` or `return foo`, or `return <- foo`)
- `ExprCallFunc` - Actual function call (`func(expr1,...)`).
- `ExprConstFloat2.getValue` - Returns the constant value stored in this expression node.
- `ExprConstFloat2` - Holds float2 constant.
- `ExprConstBool.getValue` - Returns the constant value stored in this expression node.
- `ExprConstBool` - Holds bool constant.
- `ExprIs` - Is expression for variants and such (`expr is Foo`).
- `ExprConstUInt64.getValue` - Returns the constant value stored in this expression node.
- `ExprConstUInt64` - Holds uint64 constant.
- `ExprIsVariant` - Is expression (`foo is bar`)
- `ExprConstInt3.getValue` - Returns the constant value stored in this expression node.
- `ExprConstInt3` - Holds int3 constant.
- `ExprStringBuilder` - String builder expression ("blah{blah1}blah2").
- `ExprNullCoalescing` - Null coalescing (`expr1 ?? default_value`).
- `ExprMakeStruct` - Make structure expression (`[[YourStruct v1=expr1elem1, v2=expr2elem1, ...; v1=expr1elem2, ...
- `ExprOp1` - Single operator expression (`+a` or `-a` or `!a` or `~a`)
- `ReaderMacro` - Adapter for the `AstReaderMacro`.
- `ExprSafeAsVariant` - Safe as expression (`foo? as bar`)
- `ExprErase` - Erase expression (`erase(tab,key)`)
- `ExprMakeTuple` - Make tuple expression (`[[auto f1,f2,f3]]`)
- `VisitorAdapter` - Adapter for the `AstVisitor` interface.
- `ExprSafeAt` - Safe index lookup (`expr?[expr1]`).
- `CommentReader` - Adapter for the `AstCommentReader`.
- `ExprAssert` - Assert expression (`assert(x<13)`, or `assert(x<13, "x is too big")`, or `verify(foo()!=0)`)
- `ExprMemZero` - Memzero (`memzero(expr)`)
- `ExprFor` - For loop (`for expr1 in expr2 {your; block; here;}`)
- `ExprAsVariant` - As expression (`foo as bar`)
- `ExprTypeInfo` - typeinfo() expression (`typeinfo dim(a)`, `typeinfois_ref_type<int&>()`)
- `ExprInvoke.isCopyOrMove` - Returns whether the given invoke expression requires a copy or move of a reference type.
- `ExprInvoke` - Invoke expression (`invoke(fn)` or `invoke(lamb, arg1, arg2, ...)`)
- `ExprConstDouble.getValue` - Returns the constant value stored in this expression node.
- `ExprConstDouble` - Holds double constant.
- `ExprConstFloat16.getValue` - Returns the constant value stored in this expression node.
- `ExprConstFloat16` - Holds float16 (fp16) constant.
- `ExprQuote` - Compilation time expression which holds its subexpressions but does not infer them (`quote() <| x+5`)
- `CaptureEntry` - Single entry in lambda capture.
- `ExprMakeBlock` - Any closure.
- `ExprSafeField` - Safe field lookup (`foo?.bar`)
- `ExprFakeLineInfo.getValue` - Returns the constant value stored in this expression node.
- `ExprFakeLineInfo` - Compilation time only fake lineinfo expression.
- `CallMacro` - Adapter for the `AstCallMacro`.
- `ExprConstInt4.getValue` - Returns the constant value stored in this expression node.
- `ExprConstInt4` - Holds int4 constant.
- `SimulateMacro` - Adapter for the `AstSimulateMacro`.
- `ExprRef2Value` - Compilation time only structure which holds reference to value conversion for the value types, i.e.
- `TypeInfoMacro` - Compilation time only structure which holds live information about typeinfo expression for the specific macro.
- `ExprAddr` - Function address (`@@foobarfunc` or `@@foobarfunc<(int;int):bool>`)
- `ExprLabel` - Label (`label 13:`)
- `EnumerationAnnotation` - Adapter for the `AstEnumerationAnnotation`.
- `ExprConstEnumeration` - Holds enumeration constant, both type and entry (`Foo bar`).
- `ExprSetInsert` - Set insert expression, i.e.
- `ExprCast` - Any cast expression (`cast<int>(a)`, `upcast<Foo>(b)` or `reinterpret<Bar?>(c)`)
- `ExprTag` - Compilation time only tag expression, used for reification.
- `ExprStaticAssert` - Static assert expression (`static_assert(x<13)` or `static_assert(x<13, "x is too big")`)
- `TypeDecl.canAot` - Returns whether the given type can be ahead-of-time compiled.
- `TypeDecl.isExprType` - Returns whether the type hierarchy contains an expression type.
- `TypeDecl.isSimpleType` - Returns whether the given type is a simple non-void type that does not require resolution at inference time.
- `TypeDecl.isArray` - Returns whether the given type is an array type.
- `TypeDecl.isGoodIteratorType` - Returns whether the given type is an iterator type.
- `TypeDecl.isGoodArrayType` - Returns whether the given type is a dynamic array type.
- `TypeDecl.isGoodTableType` - Returns whether the given type is a table type.
- `TypeDecl.isGoodBlockType` - Returns whether the given type is a block type.
- `TypeDecl.isGoodFunctionType` - Returns whether the given type is a function type.
- `TypeDecl.isGoodLambdaType` - Returns whether the given type is a lambda type.
- `TypeDecl.isGoodTupleType` - Returns whether the given type is a tuple type.
- `TypeDecl.isGoodVariantType` - Returns whether the given type is a variant type.
- `TypeDecl.isVoid` - Returns whether the given type is the void type.
- `TypeDecl.isAnyType` - Returns whether the given type is the any type, passed as vec4f via standard C++ interop.
- `TypeDecl.isRef` - Returns whether the given type is a reference value.
- `TypeDecl.isRefType` - Returns whether the given type is a reference type.
- `TypeDecl.canWrite` - Returns whether the given type can be written to.
- `TypeDecl.isAotAlias` - Returns whether the type definition contains an AOT alias type.
- `TypeDecl.isShareable` - Returns whether the given type is shareable across contexts.
- `TypeDecl.isIndex` - Returns whether the given type is an index type.
- `TypeDecl.isBool` - Returns whether the given type is a boolean type.
- `TypeDecl.isInteger` - Returns whether the given type is an integer type.
- `TypeDecl.isSignedInteger` - Returns whether the given type is a signed integer type.
- `TypeDecl.isUnsignedInteger` - Returns whether the given type is an unsigned integer type.
- `TypeDecl.isSignedIntegerOrIntVec` - Returns whether the given type is a signed integer or signed integer vector type.
- `TypeDecl.isUnsignedIntegerOrIntVec` - Returns whether the given type is an unsigned integer or unsigned integer vector type.
- `TypeDecl.isFloatOrDouble` - Returns whether the given type is a float or double type.
- `TypeDecl.isNumeric` - Returns whether the given type is a numeric type.
- `TypeDecl.isNumericComparable` - Returns whether the given type supports numeric comparison.
- `TypeDecl.isPointer` - Returns whether the given type is a pointer type.
- `TypeDecl.isSmartPointer` - Returns whether the given type is a smart pointer type.
- `TypeDecl.isVoidPointer` - Returns whether the given type is a void pointer type.
- `TypeDecl.isIterator` - Returns whether the given type is an iterator type.
- `TypeDecl.isEnum` - Returns whether the given type is an enumeration type.
- `TypeDecl.isEnumT` - Returns whether the base type of the given type is an enumeration type.
- `TypeDecl.isHandle` - Returns whether the given type is a handle type, representing a C++ type exposed to daslang via TypeAnnotation.
- `TypeDecl.isStructure` - Returns whether the given type is a structure type.
- `TypeDecl.isClass` - Returns whether the given type is a class type.
- `TypeDecl.isFunction` - Returns whether the given type is a function type.
- `TypeDecl.isTuple` - Returns whether the given type is a tuple type.
- `TypeDecl.isVariant` - Returns whether the given type is a variant type.
- `TypeDecl.sizeOf` - Returns the size of the given type in bytes.
- `TypeDecl.countOf` - Returns the number of elements if the given type is a fixed array, otherwise returns 1.
- `TypeDecl.alignOf` - Returns the memory alignment requirement of the type in bytes.
- `TypeDecl.baseSizeOf` - Returns the size of the given type in bytes, excluding fixed array dimensions.
- `TypeDecl.stride` - Returns the stride size in bytes of an element in a fixed array type.
- `TypeDecl.tupleSize` - Returns the size of the given tuple type in bytes.
- `TypeDecl.tupleAlign` - Returns the alignment of the given tuple type in bytes.
- `TypeDecl.variantSize` - Returns the size of the given variant type in bytes.
- `TypeDecl.variantAlign` - Returns the alignment of the given variant type in bytes.
- `TypeDecl.canCopy` - Returns whether the given type can be copied.
- `TypeDecl.canMove` - Returns whether the given type can be moved.
- `TypeDecl.canClone` - Returns whether the given type can be cloned.
- `TypeDecl.canCloneFromConst` - Returns whether the given type can be cloned from a const instance.
- `TypeDecl.canNew` - Returns whether the given type can be heap-allocated via the new operator.
- `TypeDecl.canDeletePtr` - Returns whether the pointer to the given type can be deleted.
- `TypeDecl.canDelete` - Returns whether the given type can be deleted.
- `TypeDecl.needDelete` - Returns whether the given type requires explicit deletion.
- `TypeDecl.isPod` - Returns whether the given type is a plain old data (POD) type.
- `TypeDecl.isRawPod` - Returns whether the given type is a raw POD type containing no pointers or strings.
- `TypeDecl.isNoHeapType` - Returns whether the given type can be used without heap allocation.
- `TypeDecl.isWorkhorseType` - Returns whether the given type is a workhorse type, which is a built-in non-reference type.
- `TypeDecl.isPolicyType` - Returns whether the given type is a policy type with SimNode implementations available for it.
- `TypeDecl.isVecPolicyType` - Returns whether the given type is a vector policy type, which is any policy type other than string.
- `TypeDecl.isReturnType` - Returns whether the given type can be used as a return type, which includes anything except block.
- `TypeDecl.isCtorType` - Returns whether the given basic type is a constructor type that can be constructed via its type name, such as int(3.4).
- `TypeDecl.isRange` - Returns whether the given type is a range type.
- `TypeDecl.isString` - Returns whether the given type is a string type.
- `TypeDecl.isConst` - Returns whether the given type is const-qualified.
- `TypeDecl.isFoldable` - Returns whether the given type is foldable, such as integer or float, as opposed to pointer or array.
- `TypeDecl.isAlias` - Returns whether the type definition contains an alias type.
- `TypeDecl.isAutoArrayResolved` - Returns whether all fixed array dimensions are fully resolved with no auto or expression dimensions remaining.
- `TypeDecl.isAuto` - Returns whether the type definition contains an auto type.
- `TypeDecl.isAutoOrAlias` - Returns whether the type definition contains an auto or alias type.
- `TypeDecl.isVectorType` - Returns whether the given type is a vector type such as int2, float3, or range64.
- `TypeDecl.isBitfield` - Returns whether the given type is a bitfield type.
- `TypeDecl.isLocal` - Returns whether the given type is a local type that can be allocated on the stack.
- `TypeDecl.hasClasses` - Returns whether the type definition contains any class types.
- `TypeDecl.hasNonTrivialCtor` - Returns whether the type definition contains any non-trivial constructors.
- `TypeDecl.hasNonTrivialDtor` - Returns whether the type definition contains any non-trivial destructors.
- `TypeDecl.hasNonTrivialCopy` - Returns whether the type definition contains any non-trivial copy operations.
- `TypeDecl.canBePlacedInContainer` - Returns whether the given type can be placed in a container.
- `TypeDecl.vectorBaseType` - Returns the scalar base type of a vector type, for example float for float4.
- `TypeDecl.vectorDim` - Returns the number of components in a vector type, for example 4 for float4.
- `TypeDecl.canInitWithZero` - Returns whether the given type can be initialized by zeroing its memory.
- `TypeDecl.rangeBaseType` - Returns the base type of a range type, for example int64 for range64.
- `TypeDecl.unsafeInit` - Returns whether the given type requires initialization and skipping it would be unsafe.
- `TypeDecl.get_mnh` - Returns the mangled name hash of the given type.
- `TypeDecl` - Any type declaration.
- `ExprLet` - Local variable declaration (`let v = expr;`).
- `Enumeration` - Enumeration declaration.
- `ExprConstBitfield.getValue` - Returns the constant value stored in this expression node.
- `ExprConstBitfield` - Holds bitfield constant (`Foo bar`).
- `ExprMakeArray` - Make array expression (`[[auto 1;2;3]]` or `[{auto "foo";"bar"}]` for static and dynamic arrays accordingly).
- `MakeFieldDecl` - Part of `ExprMakeStruct`, declares single field (`a = expr` or `a <- expr` etc)
- `ExprArrayComprehension` - Array comprehension (`[for (x in 0..3); x]`, `[iterator for (y in range(100)); x*2; where (x!=13)]]` for arrays or generators accordingly).
- `ExprField.field` - Returns a pointer to the named field of a structure, or null if the field does not exist or the type is not a structure.
- `ExprField` - Field lookup (`foo.bar`)
- `ExprConstInt8.getValue` - Returns the constant value stored in this expression node.
- `ExprConstInt8` - Holds int8 constant.
- `ExprKeyExists` - Key exists expression (`key_exists(tab,key)`)
- `ExprConstUInt8.getValue` - Returns the constant value stored in this expression node.
- `ExprConstUInt8` - Holds uint8 constant.
- `ExprVar` - Variable access (`foo`)
- `ExprConstUInt.getValue` - Returns the constant value stored in this expression node.
- `ExprConstUInt` - Holds uint constant.
- `ExprConstUInt2.getValue` - Returns the constant value stored in this expression node.
- `ExprConstUInt2` - Holds uint2 constant.
- `ExprFakeContext` - Compilation time only fake context expression.
- `ExprConstUInt4.getValue` - Returns the constant value stored in this expression node.
- `ExprConstUInt4` - Holds uint4 constant.
- `BuiltInFunction` - Bindings for the 'BuiltInFunction', which is used for the builtin (bound) functions in Daslang.
- `PassMacro` - Adapter for the `AstPassMacro`.
- `ExprDebug` - Debug expression (`debug(x)` or `debug(x,"x=")`)
- `VariantMacro` - Adapter for the `AstVariantMacro`.
- `ExprBlock` - Any block expression, including regular blocks and all types of closures.
- `ExprCall` - Anything which looks like call (`call(expr1,expr2)`).
- `EnumEntry` - Entry in the enumeration.
- `ExprConstString` - Holds string constant.
- `ExprAscend` - New expression for ExprMakeLocal (`new [[Foo fld=val,...]]` or `new [[Foo() fld=...]]`, but **NOT** `new Foo()`)
- `ExprConstInt64.getValue` - Returns the constant value stored in this expression node.
- `ExprConstInt64` - Holds int64 constant.
- `ExprConstFloat.getValue` - Returns the constant value stored in this expression node.
- `ExprConstFloat` - Holds float constant.
- `ExprConstURange.getValue` - Returns the constant value stored in this expression node.
- `ExprConstURange` - Holds urange constant.
- `TypeMacro` - Compilation time only structure which holds live information about type macro.
- `StructureAnnotation` - Adapter for the `AstStructureAnnotation`.
- `ExprReader` - Compilation time only expression which holds temporary information for the `AstReaderMacro`.
- `ExprAssume` - Assume expression (`assume name = expr`) or (`typedef name = type`).
- `ExprYield` - Yield expression (`yield foo` or `yield <- bar`)
- `ExprIfThenElse` - If-then-else expression (`if expr1 {your; block; here;} else {your; block; here;}`) including `static_if`'s.
- `FieldDeclaration` - Structure field declaration.
- `ExprConstRange.getValue` - Returns the constant value stored in this expression node.
- `ExprConstRange` - Holds range constant.
- `ExprCallMacro` - Compilation time only expression which holds temporary information for the `AstCallMacro`.
- `ExprConstURange64.getValue` - Returns the constant value stored in this expression node.
- `ExprConstURange64` - Holds urange64 constant.
- `ExprTypeDecl` - typedecl() expression (`typedecl(1+2)`)
- `ExprOp` - Compilation time only base class for any operator.
- `ExprOp2` - Two operand operator (`expr1 + expr2`)
- `ExternalFnBase` - Base class for external function bindings.
- `ExprClone` - Clone operator (`expr1 := expr2`)
- `ExprPtr2Ref` - Pointer dereference (`*expr` or `deref(expr)`).
- `ExprSwizzle` - Vector swizzle operation (`vec.xxy` or `vec.y`)
- `ExprMove` - Move operator (`expr1 <- expr2`)
- `ExprTryCatch` - Try-recover expression (`try {your; block; here;} recover {your; recover; here;}`)
- `ExprAt` - Index lookup (`expr[expr1]`).
- `ExprWith` - With section (`with expr {your; block; here}`).
- `ExprFind` - Find expression (`find(tab,key) <| { your; block; here; }`)
- `ExprBreak` - Break expression (`break`)
- `ExprMakeGenerator` - Generator closure (`generator<int>` or `generator<Foo&>`)
- `ExprConstInt16.getValue` - Returns the constant value stored in this expression node.
- `ExprConstInt16` - Holds int16 constant.
- `ExprConstPtr.getValue` - Returns the constant value stored in this expression node.
- `ExprConstPtr` - Null (`null`).
- `ExprMakeLocal` - Any make expression (`ExprMakeBlock`, `ExprMakeTuple`, `ExprMakeVariant`, `ExprMakeStruct`)
- `ExprConstInt.getValue` - Returns the constant value stored in this expression node.
- `ExprConstInt` - Holds int constant.
- `ExprMakeVariant` - Make variant expression (`[YourVariant variantName=expr1]`)
- `ExprOp3` - Three operand operator (`cond ? expr1 : expr2`)
- `CaptureMacro` - Adapter for the `AstCaptureMacro`.
- `ExprDelete` - Delete expression (`delete blah`)
- `ForLoopMacro` - Adapter for the 'AstForLoopMacro'.
- `ExprNew` - New expression (`new Foo`, `new Bar(expr1..)`, but **NOT** `new [[Foo ...]]`)
- `ExprConstUInt16.getValue` - Returns the constant value stored in this expression node.
- `ExprConstUInt16` - Holds uint16 constant.

### Call macros

- `quote` - Returns the AST expression tree of the provided code without evaluating or type-inferring it.

### Typeinfo macros

- `ast_typedecl` - Returns a `TypeDeclPtr` for the type specified via `type<>` or subexpression type, for example `typeinfo ast_typedecl(type<int?>)`.
- `ast_function` - Returns a `FunctionPtr` to the function specified by the subexpression, for example `typeinfo ast_function(@@foo)`.

### Handled types

- `MakeStruct` - Annotation representing a vector of `MakeFieldDecl` used to initialize fields in `ExprMakeStruct` expressions.

### Classes

- `AstFunctionAnnotation` - Annotation macro that attaches to `Function` declarations.
- `AstBlockAnnotation` - Annotation macro that attaches to `ExprBlock` nodes.
- `AstStructureAnnotation` - Annotation macro that attaches to `Structure` declarations.
- `AstPassMacro` - Macro that executes as an additional inference pass during compilation.
- `AstVariantMacro` - Macro for implementing custom `is`, `as`, and `?as` expressions.
- `AstForLoopMacro` - Macro for implementing custom for-loop iteration patterns.
- `AstCaptureMacro` - Macro for implementing custom lambda capture behavior.
- `AstTypeMacro` - Macro that participates in type declarations, enabling syntax like `$macro_name<type_args...>(args)` for custom type construction and transformation.
- `AstSimulateMacro` - Macro that hooks into the context simulation phase — the final compilation step where the AST is translated into executable simulation nodes.
- `AstReaderMacro` - Macro for implementing custom parsing syntax using the `%MacroName~` notation.
- `AstCommentReader` - Macro for implementing custom comment parsing, such as extracting doxygen-style documentation or other structured metadata from source comments during compilation.
- `AstCallMacro` - Macro for implementing custom call-like expressions (e.g.
- `AstTypeInfoMacro` - Macro for implementing custom `typeinfo` traits, enabling expressions like `typeinfo YourTraitHere(...)` that extract compile-time type information.
- `AstEnumerationAnnotation` - Annotation macro that attaches to `Enumeration` declarations.
- `AstVisitor` - Implements the `Visitor` interface for traversing and transforming the AST tree.

### Compilation and file access

- `compile` - Compiles a daslang program from a source code string using the provided `FileAccess` and `ModuleGroup`, returning a `ProgramPtr` (null on failure).
- `compile_file` - Compiles a daslang program from a file registered in the given `FileAccess` object, returning a `ProgramPtr` (null on failure).
- `make_file_access` - Creates and returns a new `FileAccessPtr` (`smart_ptr<FileAccess>`) initialized as a default file-system-backed project.
- `parse_file` - Parses a daslang file and stops there — no type inference, no optimization, no simulation.
- `parse_file_no_prerequisites` - Parses one file alone and stops there: no prerequisite walk, no type inference, no macro run, and the host's module-cache stream is hidden for the duration.
- `require_module_now` - Compiles `module_name` - a `shared` module, named by its file - and its prerequisites into the process at the point of the call, or answers the module already there, under `codeOfPolicies`; a null `fileAccess` means the compiling program's own.

### Call generation

- `make_call` - Creates the appropriate call expression for a given function name in the program.

### Visitor pattern

- `visit` - Invokes an AST visitor on the given object.
- `visit_enumeration` - Applies the given visitor adapter to the specified enumeration within the context of the program, triggering the appropriate visitor callbacks.
- `visit_finally` - Invokes the visitor on the finally section of a block.
- `visit_module` - Invokes an AST visitor on the given module.
- `visit_modules` - Invokes an AST visitor on all modules in the specified program.
- `visit_structure` - Applies the given visitor adapter to the specified structure within the context of the program, triggering the appropriate visitor callbacks.
- `visit_with_generics` - Visits the program AST including generic function instantiations.

### Expression generation

- `force_generated` - Sets the generated flag on an expression and its subexpressions.
- `get_expression_annotation` - Returns the Annotation associated with an Expression or its inherited types.
- `make_type_info_structure` - Returns a new TypeInfo corresponding to the specified type.
- `stamp_missing_at` - Fills the source location of every node in the expression subtree that has none, leaving the ones that already carry a location untouched.

### Adapter generation

- `make_block_annotation` - Creates an adapter for the AstBlockAnnotation interface.
- `make_block_type` - Generates a TypeDeclPtr for a specified block or lambda type.
- `make_call_macro` - Creates an adapter for the AstCallMacro interface.
- `make_capture_macro` - Creates an adapter for the AstCaptureMacro interface.
- `make_clone_structure` - Generates a clone function for the given structure.
- `make_comment_reader` - Creates an adapter for the AstCommentReader interface.
- `make_enum_debug_info` - Generates an EnumInfo for the specified enumeration using the given DebugInfoHelper.
- `make_enumeration_annotation` - Creates an adapter for the AstEnumerationAnnotation interface.
- `make_for_loop_macro` - Creates an adapter for the AstForLoopMacro interface.
- `make_function_annotation` - Creates an adapter for the AstFunctionAnnotation interface.
- `make_function_debug_info` - Generates a FuncInfo for the specified function using the given DebugInfoHelper.
- `make_invokable_type_debug_info` - Generates a FuncInfo for an invokable type such as a lambda or block using the given DebugInfoHelper.
- `make_pass_macro` - Creates an adapter for the AstPassMacro interface.
- `make_reader_macro` - Creates an adapter for the AstReaderMacro interface.
- `make_simulate_macro` - Creates an adapter for the AstSimulateMacro interface.
- `make_struct_debug_info` - Generates a StructInfo for the specified structure using the given DebugInfoHelper.
- `make_struct_variable_debug_info` - Generates a VariableInfo for a structure field using the given DebugInfoHelper.
- `make_structure_annotation` - Creates an adapter for the AstStructureAnnotation interface.
- `make_type_info` - Generates a TypeInfo for the specified type using the given DebugInfoHelper.
- `make_type_macro` - Creates an adapter for the AstTypeMacro interface.
- `make_typeinfo_macro` - Creates an adapter for the AstTypeInfoMacro interface.
- `make_variable_debug_info` - Generates a VariableInfo for the specified variable using the given DebugInfoHelper.
- `make_variant_macro` - Creates an adapter for the AstVariantMacro interface.
- `make_visitor` - Creates an adapter for the AstVisitor interface.

### Adapter application

- `add_block_annotation` - Attaches an annotation to an expression block and calls the annotation's `apply` method.
- `add_call_macro` - Adds an AstCallMacro adapter to the specified module.
- `add_capture_macro` - Adds an AstCaptureMacro to the specified module.
- `add_comment_reader` - Adds an AstCommentReader adapter to the specified module.
- `add_dirty_infer_macro` - Adds an AstPassMacro adapter to the dirty inference pass.
- `add_enumeration_annotation` - Registers an enumeration annotation with the given module.
- `add_for_loop_macro` - Adds an AstForLoopMacro to the specified module.
- `add_function_annotation` - Attaches a function annotation to a function and calls the annotation's `apply` method.
- `add_global_lint_macro` - Adds an AstPassMacro adapter to the global lint pass.
- `add_infer_macro` - Adds an AstPassMacro adapter to the type inference pass.
- `add_lint_macro` - Adds an AstPassMacro adapter to the lint pass of the current module.
- `add_module_option` - Adds a module-specific option accessible via the `options` keyword.
- `add_new_block_annotation` - Creates an AstBlockAnnotation adapter and adds it to the current module.
- `add_new_call_macro` - Creates an AstCallMacro adapter and adds it to the current module.
- `add_new_capture_macro` - Creates an AstCaptureMacro adapter and adds it to the current module.
- `add_new_comment_reader` - Creates an AstCommentReader adapter and adds it to the current module.
- `add_new_contract_annotation` - Creates an AstContractAnnotation adapter and adds it to the current module.
- `add_new_dirty_infer_macro` - Creates an AstPassMacro adapter and adds it to the current module's dirty infer pass.
- `add_new_enumeration_annotation` - Creates an AstEnumerationAnnotation adapter and adds it to the current module.
- `add_new_for_loop_macro` - Creates an AstForLoopMacro adapter and adds it to the current module.
- `add_new_function_annotation` - Creates an AstFunctionAnnotation adapter and adds it to the current module.
- `add_new_global_lint_macro` - Creates an AstPassMacro adapter and adds it to the current module's global lint pass.
- `add_new_infer_macro` - Creates an AstPassMacro adapter and adds it to the current module's infer pass.
- `add_new_lint_macro` - Creates an AstPassMacro adapter and adds it to the current module's lint pass.
- `add_new_optimization_macro` - Creates an AstPassMacro adapter and adds it to the current module's optimization pass.
- `add_new_post_compile_macro`
- `add_new_post_rewrite_macro`
- `add_new_reader_macro` - Creates an AstReaderMacro adapter and adds it to the current module.
- `add_new_simulate_macro` - Creates an AstSimulateMacro adapter and adds it to the current module.
- `add_new_structure_annotation` - Creates an AstStructureAnnotation adapter and adds it to the current module.
- `add_new_type_macro` - Creates an AstTypeMacro adapter and adds it to the current module.
- `add_new_typeinfo_macro` - Creates an AstTypeInfoMacro adapter and adds it to the current module.
- `add_new_variant_macro` - Creates an AstVariantMacro adapter and adds it to the current module.
- `add_optimization_macro` - Adds an AstPassMacro adapter to the optimization pass of a specific module.
- `add_post_compile_macro` - Registers a pass macro to run once the module is compiled and its gc root collected.
- `add_post_rewrite_macro` - Adds an AstPassMacro adapter to the post-rewrite pass, which runs right after any user pass macro reports that it rewrote the tree.
- `add_reader_macro` - Adds an AstReaderMacro adapter to the specified module.
- `add_simulate_macro` - Adds an AstSimulateMacro adapter to the specified module.
- `add_structure_annotation` - Attaches a structure annotation to the specified module.
- `add_type_macro` - Adds an AstTypeMacro adapter to the specified module.
- `add_typeinfo_macro` - Adds an AstTypeInfoMacro adapter to the specified module.
- `add_variant_macro` - Adds an AstVariantMacro adapter to the specified module.

### Adding objects to objects

- `add_alias` - Adds a type alias to the specified module.
- `add_enumeration_entry` - Adds a new entry with the given name to the specified enumeration.
- `add_function` - Adds a function to the specified module.
- `add_generic` - Adds a generic (template) function to the specified module.
- `add_keyword` - Registers a new keyword in the specified module, making it available to the parser.
- `add_module_cache_dependency` - Registers the file at `path` as a compile-time dependency of the module being compiled, so the module cache re-parses the module when that file's size or content hash changes.
- `add_module_require` - Adds module dependencies, similar to the `require` keyword.
- `add_ptr_ref` - Wraps an expression in a pointer-to-reference (r2v) conversion node if needed.
- `add_structure` - Adds a structure definition to the specified module.
- `add_structure_alias` - Adds a typedef alias to a structure type in the AST, equivalent to a typedef in the structure body.
- `add_type_function` - Adds a type function keyword, allowing function calls to accept type arguments before regular arguments via the `some_call<type_args>(regular_args)` syntax.
- `add_variable` - Adds a global variable to the specified module.

### Program and module access

- `clear_jit_selection` - Empties `program`'s JIT selection - the state `mark_jit_selection` rebuilds before it marks the functions a DLL holds.
- `compiling_module` - Returns the module currently being compiled.
- `compiling_program` - Returns the program currently being compiled.
- `find_macro_context` - Returns the macro context of `module` - every macro context carries the mutex `invoke_in_context` locks from its creation.
- `function_index` - The slot `function` holds in `program`'s context: `-1` when the program never allocated it, `-2` when the allocation found it unused, its function-table index otherwise.
- `has_macro_context` - Returns true when `module` has a macro context - it declares a macro (a `[_macro]` function is the smallest) and was compiled with one.
- `is_jit_selected` - True when `program`'s JIT selected `function` - the DLL the program runs holds it, so a call to it from jitted code goes into the DLL rather than through an interpreter slot.
- `is_used` - True when `program`'s symbol-use pass reached `function`.
- `set_jit_selected` - Marks `function` as selected by `program`'s JIT (`selected` true) or drops it from the selection (false).
- `this_module` - Returns the main module attached to the current context, throwing an error if RTTI is disabled.
- `this_program` - Returns the program attached to the current context, or null if RTTI is disabled.
- `variable_index` - The slot the global `variable` holds in `program`'s context: `-1` when the program never allocated it, `-2` when the allocation found it unused, its index in the context's global table otherwise.

### Textual descriptions of the objects

- `das_to_string` - Returns the name of the corresponding daslang base type as a string.
- `describe`
- `describe_cpp`
- `describe_expression` - Returns a string description of the Expression matching the corresponding daslang source code.
- `describe_function` - Returns a human-readable string description of the specified function, including its name, arguments, and return type.
- `describe_program` - Returns a textual representation of the entire compiled program, equivalent to the output produced by `options log`.
- `describe_typedecl` - Returns a string description of the TypeDecl matching the corresponding daslang type declaration.
- `describe_typedecl_cpp` - Returns a string description of the TypeDecl matching the corresponding C++ type declaration.

### Searching

- `find_bitfield_name` - Finds the name of a bitfield value in the specified type.
- `find_call_macro` - Finds a CallMacro by name in the specified module.
- `find_compiling_function_by_mangled_name_hash` - Returns a Function from the currently compiling program given its mangled name hash.
- `find_compiling_module` - Finds a module by name in the currently compiling program.
- `find_enum_name` - Finds the name corresponding to an enumeration value in the specified type.
- `find_enum_value` - Finds the integer value corresponding to an enumeration name in the specified type.
- `find_matching_variable` - Searches the program for global variables matching the given name that are visible from the specified function's scope.
- `find_method_function` - Returns the function a class method field points at - the override the class declares, else the inherited one - or null for a data field, an unknown name, or a null structure.
- `find_module` - Finds a module by name in the specified program.
- `find_module_function_via_rtti` - Finds a function by name in the specified module using RTTI.
- `find_module_via_rtti` - Finds a module by name in the specified program using RTTI.
- `find_struct_field_parent` - Finds and returns the parent structure in the inheritance chain that originally declares the field with the given name.
- `find_structure_field` - Returns the FieldDeclaration for a specific field of a structure type, or null if not found or the structure is null.
- `find_unique_structure` - Finds a uniquely named structure in the program, returning it if unique or null if ambiguous.
- `find_variable` - Finds a variable by name in the specified module.

### Iterating

- `any_array_foreach` - Iterates through any `array<>` type in a typeless fashion using `void?` pointers.
- `any_table_foreach` - Iterates through any `table<>` type in a typeless fashion using `void?` pointers.
- `for_each_annotation_ordered` - Iterates through each annotation in the given module in the order they were added.
- `for_each_call_macro` - Iterates through every CallMacro adapter in the specified module.
- `for_each_capture_macro` - Iterates over all capture macros registered in the given module, invoking the provided block for each one.
- `for_each_enumeration` - Iterates over all enumerations defined in the given module, calling the provided block for each one.
- `for_each_field` - Iterates through every field in a BuiltinStructure handled type.
- `for_each_for_loop_macro` - Iterates through every for-loop macro in the specified module.
- `for_each_function` - Iterates over all functions with the specified name in the given module, calling the provided block for each matching function.
- `for_each_function_annotation` - Iterates over the function annotations (including tag/function macros) registered in the given module, invoking the provided block with each annotation name.
- `for_each_gc_expression`
- `for_each_gc_typedecl`
- `for_each_generic` - Iterates through each generic function in the given module.
- `for_each_global` - Iterates over all global variables defined in the given module, calling the provided block for each one.
- `for_each_module` - Iterates through each module in the program in dependency order.
- `for_each_module_function` - Iterates over all functions defined in the given module, calling the provided block for each function regardless of name.
- `for_each_module_no_order` - Iterates through each module in the program in no particular order, as they appear in the library group.
- `for_each_pass_macro` - Iterates over all pass macros (infer, dirty-infer, optimization, lint, and global-lint) registered in the given module, invoking the provided block with each macro name.
- `for_each_reader_macro` - Iterates through each reader macro in the given module.
- `for_each_simulate_macro` - Iterates over all simulate macros registered in the given module, invoking the provided block for each one.
- `for_each_structure` - Iterates over all structures defined in the given module, calling the provided block for each one.
- `for_each_structure_alias` - Iterates over all structure aliases defined in the given structure type, invoking the provided block for each alias.
- `for_each_typedef` - Iterates through every typedef in the specified module.
- `for_each_typeinfo_macro` - Iterates through each typeinfo macro in the given module.
- `for_each_typemacro` - Iterates over all type macros registered in the given module, invoking the provided block for each one.
- `for_each_variant_macro` - Iterates through each variant macro in the given module.

### Cloning

- `clone_expression` - Clones an Expression along with all its subexpressions and corresponding type information.
- `clone_file_info` - Clones a FileInfo structure.
- `clone_function` - Creates and returns a deep copy of the specified function, including its body, arguments, and annotations.
- `clone_structure` - Returns a deep clone of the specified Structure.
- `clone_type` - Clones a TypeDecl along with all its subtypes.
- `clone_variable` - Creates and returns a deep copy of the specified variable, including its type declaration and initializer expression.

### Mangled name

- `get_mangled_name` - Returns the mangled name of the given function, which uniquely identifies it by encoding the function name along with its argument and return types.
- `parse_mangled_name` - Parses a mangled name string and creates the corresponding TypeDecl.

### Size and offset

- `any_array_long_size` - Returns the 64-bit size of an array from a pointer to an `array<>` object.
- `any_array_size` - Returns the size of an array from a pointer to an `array<>` object.
- `any_table_long_size` - Returns the 64-bit size of a table from a pointer to a `table<>` object.
- `any_table_size` - Returns the size of a table from a pointer to a `table<>` object.
- `get_handled_type_field_offset` - Returns the byte offset of a field within a handled (C++-bound) type annotation.
- `get_handled_type_size` - Returns the size in bytes (sizeof) of a handled (C++) type from its annotation.
- `get_tuple_field_offset` - Returns the byte offset of a tuple field.
- `get_variant_field_offset` - Returns the byte offset of a variant field.

### Evaluations

- `eval_single_expression` - Simulate and evaluate a single AST expression against the supplied runtime context, returning the result as a `float4` and setting `ok` to indicate success.

### Error reporting

- `macro_error` - Reports an error to the currently compiling program during the active compilation pass.
- `macro_performance_warning` - Reports a performance warning (error code 40217) at the given source location during compilation.
- `macro_sticky_error` - Reports a sticky error to the currently compiling program: unlike macro_error, the report survives subsequent inference passes, so the compile still fails even after the macro repairs the tree in place.
- `macro_style_warning` - Reports a style warning (error code 40218) at the given source location during compilation.

### Location and context

- `collect_dependencies` - Collects all functions and global variables that the specified function depends on, and passes them to the provided block as two arrays.
- `force_at` - Replaces line info in an expression, its subexpressions, and their types.
- `get_ast_context` - Returns the AstContext for a given expression, including the current function, loops, blocks, scopes, and with sections.

### Use queries

- `get_use_functions` - Iterates over all functions that the given function calls or references, invoking the provided block for each one.
- `get_use_global_variables` - Iterates over all global variables that the given function accesses, invoking the provided block for each one.

### Log

- `to_compilation_log` - Writes a message to the compilation log from a macro during compilation.

### Removal

- `remove_structure` - Removes the specified structure from the given module.

### Properties

- `can_access_global_variable` - Checks whether the given global variable is accessible from the specified module context, taking into account visibility rules and the module where the access originates.
- `get_aot_arg_prefix` - Returns the AOT argument prefix string for the specified function.
- `get_aot_arg_suffix` - Returns the AOT argument suffix string for the specified function.
- `get_aot_hash_comment` - Returns a diagnostic string containing the function's own semantic hash and all non-builtin dependency hashes with their mangled names.
- `get_aot_name` - Returns the AOT-generated name for the specified function.
- `get_builtin_function_address` - Takes as argument a BuiltInFunction, and returns its address.
- `get_const_expr` - Returns what the compiler makes of `expression` as a constant -- folding arithmetic and constant constructor calls even where the compilation policy leaves folding off -- or null when it is not a constant.
- `get_current_search_module` - Returns the module currently being searched for a function by name, correctly resolving special names like `""`, `"_"`, `"*"`, and `"__"`.
- `get_field_type` - Returns the type of a field if the target is a structure, variant, tuple, handled type, or pointer to any of those, or null otherwise.
- `get_file_source_line` - Reads a single source line from a FileInfo and invokes the block with the line text as a temporary string.
- `get_func_aot_prefix` - Returns the AOT function prefix string for the specified function.
- `get_function_aot_hash` - Returns the hash of a function used for AOT matching.
- `get_function_hash_by_id` - Returns the hash of a function given its unique identifier.
- `get_handled_type_field_type` - Returns the runtime type information for a field within a handled (C++-bound) type annotation.
- `get_handled_type_field_type_declaration` - Returns the type declaration for a field within a handled (C++-bound) type annotation, optionally as const.
- `get_handled_type_index_type_declaration` - Returns the type declaration of the index operator for a handled type.
- `get_struct_aot_prefix` - Returns the AOT prefix string for the specified structure.
- `get_structure_alias` - Finds and returns a structure alias type by its alias name.
- `get_underlying_value_type` - Returns the daslang type aliased by a ManagedValue handled type.
- `get_vector_length` - Returns the length of a vector given a pointer to the vector object and its type declaration.
- `get_vector_ptr_at_index` - Returns a pointer to the vector element at the specified index given a pointer to the vector object and its TypeDeclPtr.
- `has_field` - Returns true if a structure, variant, tuple, handled type, or pointer to any of those has the specified field.
- `is_cpp_keyword` - Returns true if the string is a reserved C++ keyword (including contextual keywords like override and final).
- `is_das_keyword` - Returns true if the string is a built-in daScript language keyword.
- `is_expr_const` - Returns true if the expression is or inherits from ExprConst.
- `is_expr_like_call` - Returns true if the expression is or inherits from ExprLooksLikeCall.
- `is_same_type` - Compares two types using the given comparison parameters and returns true if they match.
- `is_temp_type` - Returns true if the specified type can be temporary.
- `is_visible_directly` - Returns true if one module is directly visible from another module.

### Infer

- `infer_generic_type` - Infers a concrete type from a generic type declaration and a pass type.
- `update_alias_map` - Updates the alias map for the specified type during inference.

### Module queries

- `module_find_annotation` - Finds an annotation by name in the specified module.
- `module_find_enumeration` - Finds an enumeration by name in the given module.
- `module_find_structure` - Finds a structure by name in the specified module.
- `module_find_type_annotation` - Finds a type annotation by name in the specified module.
- `module_has_comment_reader` - Returns true if the given module installs a comment reader (a [comment_reader] / AstCommentReader that processes //! doc-comments at compile time).
- `not_inferred` - Marks a function as modified by a macro so that it will be inferred again.

### Debug info helpers

- `debug_helper_find_struct_cppname` - Finds a structure in the DebugInfoHelper and returns its C++ name.
- `debug_helper_find_type_cppname` - Finds a type in the DebugInfoHelper and returns its C++ name.
- `debug_helper_iter_enums` - Iterates through all enumerations in the DebugInfoHelper, invoking the provided block for each one.
- `debug_helper_iter_funcs` - Iterates through all functions in the DebugInfoHelper, invoking the provided block for each one.
- `debug_helper_iter_structs` - Iterates through all structures in the DebugInfoHelper, invoking the provided block for each one.
- `debug_helper_iter_types` - Iterates through all types in the DebugInfoHelper, invoking the provided block for each one.
- `debug_helper_iter_vars` - Iterates through all variables in the DebugInfoHelper, invoking the provided block for each one.

### AOT support

- `aot_need_type_info` - Returns true if a `TypeInfo?` is needed for the specified type in a typeinfo expression.
- `aot_previsit_get_field` - Performs the pre-visit step for field access during AOT code generation.
- `aot_previsit_get_field_ptr` - Performs the pre-visit step for field pointer access during AOT code generation.
- `aot_require` - Writes data to the require section of an AOT module.
- `aot_type_ann_get_field_ptr` - Returns the access symbol string for a field, such as `->` for pointer types or `.` for value types.
- `aot_visit_get_field` - Performs the visit step for field access during AOT code generation.
- `getInitSemanticHashWithDep` - Returns the initialization semantic hash including dependencies for the entire program.
- `macro_aot_infix` - Returns true if the macro requires an AOT infix operator for the specified handled type.
- `set_aot_main_module_name_cpp` - Sets the C++ AOT emitter's namespace for the unnamed (promoted) main module's types; "" restores the anonymous-namespace default.
- `write_aot_body` - Writes the AOT (ahead-of-time compilation) body section for the given structure annotation, outputting generated C++ code to the string builder writer.
- `write_aot_macro_prefix` - Writes the AOT macro prefix code for the specified TypeInfoMacro.
- `write_aot_macro_suffix` - Writes the AOT macro suffix code for the specified TypeInfoMacro.
- `write_aot_suffix` - Writes the AOT (ahead-of-time compilation) suffix section for the given structure annotation, outputting generated C++ code to the string builder writer.

### String builder writer

- `string_builder_clear` - Clears a StringBuilder object given a pointer to it.
- `string_builder_str` - Returns the accumulated string from a StringBuilder object given a pointer to it.

### GC

- `ast_gc_collect_scope` - Runs `block` — which is expected to build the body of `function` — on a temporary gc-node scope, then keeps the nodes reachable from the function's body and sweeps the rest.
- `ast_gc_guard` - Executes a block and then collects all unreachable GC nodes (TypeDecl instances) created during execution.
- `delete_expression` - Frees a single orphaned AST expression node mid-compile instead of waiting for the enclosing ast_gc_guard to sweep it; only the node itself is freed (its children remain on the gc root, so a caller-side walker must delete them too).
- `delete_type` - Frees a single orphaned AST TypeDecl node mid-compile (sibling of delete_expression); only the node itself is freed, not its sub-types or the shared structType/enumType it points at.
- `verify_expression_gc` - Asserts that the given Expression gc_node is alive (not swept, not deleted), fatal-erroring with the node's gc_id when called on a stale pointer.
- `verify_typedecl_gc` - Asserts that the given TypeDecl gc_node is alive (not swept, not deleted), fatal-erroring with the node's gc_id when called on a stale pointer.

## strings

The STRINGS module implements string formatting, conversion, searching, and modification routines. It provides functions for building strings (`build_string`), parsing (`to_int`, `to_float`), character classification (`is_alpha`, `is_number`), and low-level string manipulation.


### Enumerations

- `ConversionResult` - Result of conversion from string to number.

### Handled structures

- `StringBuilderWriter` - Object representing a string builder.

### Character set

- `is_char_in_set` - Returns true if the character given by its integer code is present in the 256-bit character set represented as a uint[8] array.
- `set_element` - Returns the character code at the given element index within the 256-bit character set represented as a uint[8] array.
- `set_total` - Returns the total number of characters present (bits set) in the 256-bit character set represented as a uint[8] array.

### Character groups

- `is_alnum` - Returns true if the integer character code represents an alphanumeric ASCII character [A-Za-z0-9].
- `is_alpha` - Returns true if the integer character code represents an alphabetic ASCII character [A-Za-z].
- `is_hex` - Returns true if the integer character code represents a hexadecimal digit [0-9A-Fa-f].
- `is_new_line` - Returns true if the integer character code is a newline character (\\n or \\r).
- `is_number` - Returns true if the integer character code represents a decimal digit [0-9].
- `is_tab_or_space` - Returns true if the integer character code is a tab or space character.
- `is_white_space` - Returns true if the integer character code is one of the six C `isspace` characters: space, tab, CR, LF, FF and VT.

### Character by index

- `character_at` - Returns the integer character code of string `str` at the given index `idx`, with bounds checking.
- `character_uat` - Returns the integer character code of string `str` at the given index `idx` without performing bounds checking (unsafe).
- `first_character` - Returns the first character of the string as an integer.

### String properties

- `ends_with` - Returns true if the string `str` ends with the substring `cmp`, false otherwise.
- `skip_white_space` - Returns the offset of the first byte at or after `from` that is not whitespace, or the length when everything from `from` onwards is whitespace.
- `starts_with` - Returns true if the beginning of string `str` matches the string `cmp`, with optional `offset` and `cmpLen` parameters to control the comparison start position and length.

### String builder

- `build_hash` - Computes a uint64 hash by streaming writes through a StringBuilderWriter passed to `block`, without allocating the full concatenated string.
- `build_string` - Creates a StringBuilderWriter, passes it to `block` for writing, and returns the accumulated output as a string.
- `format` - Formats a numeric value of type T using a C printf-style format string, either appending to a StringBuilderWriter and returning a reference to it, or returning the formatted result as a new string.
- `write` - Writes the textual representation of any value into the StringBuilderWriter and returns a reference to the writer for chaining.
- `write_char` - Writes a single character specified by its integer code `ch` into the StringBuilderWriter and returns a reference to the writer.
- `write_chars` - Writes the character specified by integer code `ch` repeated `count` times into the StringBuilderWriter and returns a reference to the writer.
- `write_escape_string` - Writes the escaped form of string `str` (with special characters converted to escape sequences) into the StringBuilderWriter and returns a reference to the writer.
- `write_string` - Writes the bytes of a byte view (`array<uint8>`, as handed to a `peek_data` block) into the StringBuilderWriter and returns a reference to the writer.

### das::string manipulation

- `append` - Appends a single character specified by its integer code `ch` to the mutable das_string `str`.
- `resize` - Resizes the mutable das_string `str` in place to `new_length` characters.
- `with_das_string` - Creates a temporary empty das_string and passes it to the block.

### String modifications

- `chop` - Returns a substring of `str` beginning at index `start` with the specified `length`.
- `escape` - Returns a new string with special characters replaced by their printable escape sequences (e.g.
- `ltrim` - Returns a new string with leading whitespace characters removed from `str`.
- `repeat` - Returns a new string formed by concatenating `str` repeated `count` times.
- `replace` - Returns a new string with all occurrences of substring `toSearch` in `str` replaced by the substring `replace`.
- `reverse` - Returns a new string with the characters of `str` in reverse order.
- `rtrim` - Returns a new string with trailing whitespace removed from `str`, or with trailing characters from the specified `chars` set removed.
- `safe_unescape` - Unescapes a string by converting printable escape sequences back to their original characters (e.g.
- `slice` - Returns a substring of `str` from index `start` to optional `end` (exclusive), where negative indices count from the end of the string.
- `strip` - Returns a new string with all leading and trailing whitespace characters removed from `str`.
- `strip_left` - Returns a new string with all leading whitespace characters removed from `str`.
- `strip_right` - Returns a new string with all trailing whitespace characters removed from `str`.
- `to_lower` - Returns a new string with all characters of `str` converted to lower case.
- `to_upper` - Returns a new string with all characters of `str` converted to upper case.
- `trim` - Returns a new string with both leading and trailing whitespace characters removed from `str`.
- `unescape` - Returns a new string with printable escape sequences converted back to their original characters (e.g.

### Search substrings

- `find` - Returns the first index at which `substr` (string or character code) occurs in `str`, optionally searching from `start`, or -1 if not found.
- `rfind` - Returns the index of the last occurrence of `substr` in `str`, or -1 if not found.

### String comparison

- `compare_ignore_case` - Performs case-insensitive string comparison.

### String conversion routines

- `double` - Converts a string to a double value, panicking on failure; an overload accepts `result` and `offset` output parameters to report the ConversionResult status and parsed position instead of panicking.
- `float` - Converts a string to a float value, panicking on failure; an overload accepts `result` and `offset` output parameters to report the ConversionResult status and parsed position instead of panicking.
- `fmt` - Formats a numeric value of type T into the StringBuilderWriter using a libfmt/C++20 std::format format string and returns a reference to the writer.
- `int` - Converts a string to an int, panicking on failure; an overload accepts `result`, `offset`, and optional `hex` flag to report the ConversionResult status and parsed position instead of panicking.
- `int16` - Converts a string to an int16, panicking on failure; an overload accepts `result`, `offset`, and optional `hex` flag to report the ConversionResult status and parsed position instead of panicking.
- `int64` - Converts a string to an int64, panicking on failure; an overload accepts `result`, `offset`, and optional `hex` flag to report the ConversionResult status and parsed position instead of panicking.
- `int8` - Converts a string to an int8, panicking on failure; an overload accepts `result`, `offset`, and optional `hex` flag to report the ConversionResult status and parsed position instead of panicking.
- `string` - Constructs and returns a new string from the contents of a uint8 byte array.
- `to_char` - Converts an integer character code to a single-character string.
- `to_cpp_float` - Converts a float value to its string representation using C++ fmt::format_to, correctly handling special constants like FLT_MIN and FLT_MAX.
- `to_double` - Converts a string to a double value, returning 0.0lf if the conversion fails.
- `to_float` - Converts a string to a float value, returning 0.0 if the conversion fails.
- `to_int` - Converts a string to an int value with optional hexadecimal parsing when `hex` is true, returning 0 if the conversion fails.
- `to_int16` - Converts a string to an int16 value with optional hexadecimal parsing when `hex` is true, returning 0 if the conversion fails.
- `to_int64` - Converts a string to an int64 value with optional hexadecimal parsing when `hex` is true, returning 0l if the conversion fails.
- `to_int8` - Converts a string to an int8 value with optional hexadecimal parsing when `hex` is true, returning 0 if the conversion fails.
- `to_uint` - Converts a string to a uint value with optional hexadecimal parsing when `hex` is true, returning 0u if the conversion fails.
- `to_uint16` - Converts a string to a uint16 value.
- `to_uint64` - Converts a string to a uint64 value with optional hexadecimal parsing when `hex` is true, returning 0ul if the conversion fails.
- `to_uint8` - Converts a string to a uint8 value with optional hexadecimal parsing when `hex` is true, returning 0u if the conversion fails.
- `uint` - Converts a string to a uint, panicking on failure; an overload accepts `result`, `offset`, and optional `hex` flag to report the ConversionResult status and parsed position instead of panicking.
- `uint16` - Converts a string to a uint16, panicking on failure; an overload accepts `result`, `offset`, and optional `hex` flag to report the ConversionResult status and parsed position instead of panicking.
- `uint64` - Converts a string to a uint64, panicking on failure; an overload accepts `result`, `offset`, and optional `hex` flag to report the ConversionResult status and parsed position instead of panicking.
- `uint8` - Converts a string to a uint8, panicking on failure; an overload accepts `result`, `offset`, and optional `hex` flag to report the ConversionResult status and parsed position instead of panicking.

### String as array

- `modify_data` - Maps the raw bytes of string `str` into a temporary uint8 array, passes it to `block` for in-place reading and writing, and returns the modified string.
- `peek_data` - Maps the raw bytes of string `str` into a temporary read-only uint8 array and passes it to `block` for inspection.
- `to_bytes` - Returns a new uint8 byte array holding a copy of the bytes of string `str`, with no terminator appended.

### Low level memory allocation

- `delete_string` - Frees the string `str` from the heap and clears the reference, returning true on success; unsafe because existing aliases become dangling pointers.
- `reserve_string_buffer` - Allocates a copy of the string data on the heap with at least `length` bytes reserved and returns the new string.

## jobque

The JOBQUE module provides low-level job queue and threading primitives. It includes thread-safe `Channel` and `Stream` types for inter-thread communication, `LockBox` for shared data access, `Atomic32` / `Atomic64` counters, `JobStatus` tracking, and fine-grained thread management. For higher-level job abstractions, see `jobque_boost`.


### Constants

- `SEQ_BOX_PAYLOAD` - Maximum payload size in bytes that a `SeqBox` snapshot can hold.

### Handled structures

- `LockBox` - Lockbox.
- `Stream.isEmpty` - Whether the channel or pipe contains no remaining elements.
- `Stream.total` - Total number of elements that have been added to the pipe.
- `Stream` - FIFO pipe of opaque byte buffers shared between contexts and threads.
- `SeqBox.hasValue` - Returns true when a snapshot has been published and not since cleared.
- `SeqBox` - Wait-free snapshot box.
- `Atomic64` - Atomic 64 bit integer.
- `Atomic32` - Atomic 32 bit integer.
- `JobStatus.isReady` - Whether the job has completed execution.
- `JobStatus.isValid` - Whether the job status object refers to a valid, active job.
- `JobStatus.size` - Returns the current entry count of the JobStatus or Channel.
- `JobStatus` - Job status indicator (ready or not, as well as entry count).
- `Channel.isEmpty` - Whether the channel or pipe contains no remaining elements.
- `Channel.total` - Total number of elements that have been added to the pipe.
- `Channel` - Channel provides a way to communicate between multiple contexts, including threads and jobs.

### Channel, JobStatus, Lockbox, SeqBox, Stream

- `add_ref` - Increases the reference count of a `JobStatus` or `Channel`, preventing premature deletion.
- `append` - Increases the entry count of the channel, signaling that new work has been added.
- `channel_create` - Creates a new `Channel` for inter-thread communication and synchronization.
- `channel_remove` - Destroys a `Channel` and releases its resources.
- `job_status_create` - Creates a new `JobStatus` object for tracking the completion state of asynchronous jobs.
- `job_status_remove` - Destroys a `JobStatus` object and releases its resources.
- `join` - Blocks the current thread until the job or channel's entry count reaches zero, indicating all work is complete.
- `lock_box_create` - Creates a new `LockBox` for thread-safe shared access to a single value.
- `lock_box_remove` - Destroys a `LockBox` and releases its resources.
- `notify` - Decreases the channel's entry count, signaling that one unit of work has completed.
- `notify_and_release` - Decreases the entry count **and** the reference count of a `Channel` or `JobStatus` in a single operation.
- `release` - Decreases the reference count of a `JobStatus` or `Channel`; the object is deleted when the count reaches zero.
- `seq_box_create` - Creates a new `SeqBox` for wait-free snapshot exchange between threads.
- `seq_box_release` - Releases one reference to the box and clears the caller pointer; whoever drops the last reference deletes it.
- `stream_create` - Creates a new empty `Stream`, a FIFO of opaque byte buffers owned by the runtime.
- `stream_remove` - Releases the caller's reference to a `Stream` and sets the local pointer to null.

### Queries

- `count_jobque_leaks` - Returns the number of JobStatus, Channel, LockBox, Stream and Feature objects currently alive (globally tracked).
- `get_total_hw_cores` - Returns the number of physical CPU cores on the system, with SMT/hyperthreading siblings collapsed (compare get_total_hw_threads, which counts logical processors).
- `get_total_hw_jobs` - Returns the total number of hardware threads allocated to the job system.
- `get_total_hw_threads` - Returns the total number of hardware threads available on the system.
- `get_total_perf_cores` - Returns the fast-tier physical core count on a heterogeneous box (Apple perflevel0); 0 means the box is homogeneous or the topology is unknown.
- `is_job_que_available` - Returns true when a job queue exists for the current process — inside a with_job_que block or after create_job_que — i.e.
- `is_job_que_shutting_down` - Returns `true` if the job queue infrastructure is shutting down or has not been initialized.
- `is_slow_tier_compute` - Returns true only when the second core tier's reported kind is on the compute allowlist ("Performance", "Super") - a tier worth extending a batch pool over.

### Event tracing

- `jobque_trace_category` - Registers a named category for a trace op tag (the id stamped via `jobque_trace_tag`), with a display color packed as 0xRRGGBB.
- `jobque_trace_marker` - Stamps an instant unit-boundary event (kind id from `jobque_trace_marker_name`, plus a free-form `arg` such as the token index) on the caller lane.
- `jobque_trace_marker_name` - Registers a marker kind (a "unit" name such as "token", "frame", "layer") and returns its id for `jobque_trace_marker`.
- `jobque_trace_save` - Writes the recorded per-lane trace events to path as a Perfetto/Chrome trace-event JSON file (one track per lane), returning false when there is no recorded trace or the file cannot be written.
- `jobque_trace_start` - Arms the per-lane JobQue event tracer: every lane (each worker plus the dispatching caller) records publish, chunk, stage-wait, wake and fifo-job events into a preallocated buffer of max(events_per_lane, 1024) entries; a lane stops recording when its buffer fills.
- `jobque_trace_stop` - Stops the per-lane JobQue event tracer armed by jobque_trace_start.
- `jobque_trace_tag` - Sets the application-defined operation tag stamped into subsequently recorded trace events (0 = untagged).

### Internal invocations

- `flush_jobque_batch` - Publish this thread's pending batched fork jobs to the queue NOW instead of at join (a no-op when nothing is pending or batching is off — see set_jobque_batch_dispatch).
- `get_current_processor` - Returns the index of the logical processor the calling thread is running on right now - on Windows, within the thread's processor group - or -1 where the platform exposes none (macOS, Haiku, platforms without a processor index) and when the query itself fails (Linux `sched_getcpu` returning -1).
- `get_jobque_affinity` - Returns the worker-thread affinity mode previously set with set_jobque_affinity, or -1 when unset.
- `get_jobque_fork_pool` - Report whether the current context pools its per-job fork contexts, which `set_jobque_fork_pool` turns on.
- `get_jobque_team_mode` - Returns true when team dispatch mode is currently enabled on the job queue.
- `get_jobque_team_prof` - Returns average per-operation team dispatch times in nanoseconds as float4: x = publish, y = serve, z = join tail, w = total.
- `get_jobque_team_prof_counts` - Returns team dispatch profiling counters as int4: x = operations, y = chunks published, z = caller-served chunks, w = operations served solo by the caller.
- `get_jobque_team_prof_react` - Returns worker claim reaction times as float2: x = average delay from publish to the first chunk claim, y = to the last, in nanoseconds.
- `get_jobque_team_rank_gate` - Returns whether the per-op team rank gate is currently enabled.
- `get_jobque_thread_team_mode` - Reads the calling OS thread's team-mode override (see `set_jobque_thread_team_mode`).
- `get_jobque_threads` - Returns the standing thread request set by set_jobque_threads, or 0 when none is set.
- `get_jobque_worker_limit` - Returns the current worker limit set by set_jobque_worker_limit.
- `jobque_try_run_one` - Pop one queued job off the fifo and run it on the calling thread (the same cloned-closure job a worker would run — full capture semantics), returning false if the fifo is empty; any batch pending on this thread (see set_jobque_batch_dispatch) is flushed first.
- `new_debugger_thread` - Creates a new debugger tick thread for servicing debug connections.
- `new_job_invoke` - Clones the current context, moves the attached lambda into it, and submits it to the job queue.
- `new_thread_invoke` - Clones the current context, moves the attached lambda into it, and runs it on a new dedicated thread.
- `reset_jobque_team_prof` - Resets the accumulated team dispatch profiling counters to zero.
- `set_current_thread_affinity` - Binds the calling thread to logical processor `cpu`.
- `set_current_thread_priority` - Sets the OS priority of the calling thread on the jobque `JobPriority` scale, -2 (Minimum) ..
- `set_jobque_affinity` - Sets the worker-thread affinity mode applied when a job queue is created: 0 off, 1 ideal-processor hint, 2 hard mask.
- `set_jobque_batch_dispatch` - Enable batched fork-job dispatch on the calling thread: new_job keeps the prepared job closures locally, and join publishes the whole batch to the queue under one lock with a single wake, instead of one push (and one worker wake) per job.
- `set_jobque_fork_pool` - Opt into pooling of per-job fork contexts on the current context: when keep is true, the contexts cloned for new_job dispatches are reused across jobs instead of being cloned and destroyed every time, removing the per-dispatch clone cost.
- `set_jobque_fork_skip_heap_reset` - Skip the heap reset when a pooled fork context is reused for a new_job dispatch: when skip is true, acquiring a fork from the pool no longer resets its linear and string heaps, removing that work from the per-dispatch path.
- `set_jobque_join_spin` - Set the fork/join join-poll level: JobStatus join polls the remaining-counter for level×1024×128 relax-rounds before parking on the condvar (0 = park immediately, the default).
- `set_jobque_team_mode` - Enables or disables team dispatch mode on the current job queue (sticky, opt-in).
- `set_jobque_team_prof` - Enables or disables team dispatch profiling, which accumulates per-operation publish, serve, and join timing counters for team_parallel_for dispatches.
- `set_jobque_team_rank_gate` - Enables or disables the per-op team rank gate: when enabled, a worker serves a published team operation only if its rank fits within that operation's chunk count, so small operations do not wake the whole pool.
- `set_jobque_thread_team_mode` - Per-caller team-mode override: with team mode off for this OS thread, `team_parallel_*` calls run inline on the caller while other threads keep publishing to the shared worker team.
- `set_jobque_threads` - Requests the TOTAL compute lane count of job queues created afterwards - N-1 workers plus the computing main thread.
- `set_jobque_threads_cap` - Caps the default worker count of job queues created afterwards: the effective default becomes the minimum of the stock rule and the cap.
- `set_jobque_worker_limit` - Limits how many job queue workers may serve work: workers with an index at or above the limit park dormant and wake only when the limit is raised or the queue shuts down.
- `set_jobque_worker_spin` - Set the idle-worker spin window in microseconds (0 = park on the condvar immediately, the default): a worker that runs out of jobs spin-polls the queue for this long before blocking, so a fork/join burst of back-to-back parallel_for dispatches never pays the OS thread-wake per job — a wake of a parked worker costs the dispatching thread several microseconds, serially, per job.
- `team_parallel_for_indexed_invoke` - Internal implementation of team_parallel_for_indexed.
- `team_parallel_for_invoke` - Internal implementation of team_parallel_for.
- `team_parallel_stages_invoke` - Internal implementation of team_parallel_stages.

### Construction

- `create_job_que` - Creates a persistent job queue (worker thread pool) and pins it so a nested with_job_que block cannot tear it down.
- `destroy_job_que` - Releases the persistent job queue created by create_job_que.
- `with_channel` - Creates a `Channel` scoped to the given block and automatically destroys it afterward.
- `with_job_que` - Ensures job queue infrastructure is initialized for the duration of the block.
- `with_job_status` - Creates a `JobStatus` scoped to the given block and automatically destroys it afterward.
- `with_lock_box` - Creates a `LockBox` scoped to the given block and automatically destroys it afterward.
- `with_seq_box` - Creates a `SeqBox` scoped to the given block and releases it afterward.
- `with_stream` - Creates an unbounded `Stream`, invokes the given block with a stable pointer to it, and automatically releases the reference when the block returns.

### SeqBox operations

- `clear` - Drops the stored snapshot, so the box reads back empty until the next publish.

### Atomic

- `atomic32_create` - Creates an `Atomic32` — a thread-safe 32-bit integer for lock-free concurrent access.
- `atomic32_remove` - Destroys an `Atomic32` and releases its resources.
- `atomic64_create` - Creates an `Atomic64` — a thread-safe 64-bit integer for lock-free concurrent access.
- `atomic64_remove` - Destroys an `Atomic64` and releases its resources.
- `dec` - Atomically decrements the integer value and returns the result.
- `get` - Returns the current value of the atomic integer.
- `inc` - Atomically increments the integer value and returns the result.
- `set` - Sets the atomic integer to the specified value.
- `with_atomic32` - Creates an `Atomic32` scoped to the given block and automatically destroys it afterward.
- `with_atomic64` - Creates an `Atomic64` scoped to the given block and automatically destroys it afterward.

## algorithm

The ALGORITHM module provides array and collection manipulation algorithms including sorting, searching, set operations, element removal, and more.


### Search

- `binary_search` - Returns true if val appears within the range [f, last).
- `equal_range` - Returns a pair of indices [lower, upper) bounding the range of elements equal to val within [f, l).
- `lower_bound` - Returns the index of the first element in the range [f, l) that is not less than val, or l if no such element is found.
- `upper_bound` - Returns the index of the first element in the range [f, l) that is greater than val, or l if no such element is found.

### Array manipulation

- `combine` - Returns a new array containing elements from a followed by b.
- `erase_all` - Erases all elements equal to value from arr in O(n) time.
- `fill` - Sets all elements of the array to the given value using clone.
- `is_sorted` - Returns true if the array is sorted in non-descending order.
- `max_element` - Returns the index of the maximum element in the array, or -1 if the array is empty.
- `min_element` - Returns the index of the minimum element in the array, or -1 if the array is empty.
- `reverse` - Reverses the elements of a in place.
- `rotate` - Rotates the array so that the element at index mid becomes the first element.
- `sort_unique` - Returns an array of elements from a, sorted and with duplicates removed.
- `topological_sort` - Topological sort of a graph.
- `unique` - Returns an array with adjacent duplicate elements removed.

### Table manipulation

- `difference` - Returns the difference of two sets.
- `identical` - Returns true if the two sets are identical.
- `intersection` - Returns the intersection of two sets.
- `is_subset` - Returns true if all elements of a are contained in b.
- `symmetric_difference` - Returns the symmetric difference of two sets (elements in either set but not both).
- `union` - Returns the union of two sets.

## ansi_colors

The ANSI_COLORS module provides helpers for wrapping strings with ANSI escape codes for colored and styled terminal output.


### Configuration

- `init_ansi_colors` - Auto-detect color support from command-line arguments.

### Colors

- `blue_str` - Wraps `str` with ANSI blue (`\\x1B[34m`) if colors are enabled.
- `cyan_str` - Wraps `str` with ANSI cyan (`\\x1B[36m`) if colors are enabled.
- `green_str` - Wraps `str` with ANSI green (`\\x1B[32m`) if colors are enabled.
- `magenta_str` - Wraps `str` with ANSI magenta (`\\x1B[35m`) if colors are enabled.
- `red_str` - Wraps `str` with ANSI red (`\\x1B[31m`) if colors are enabled.
- `yellow_str` - Wraps `str` with ANSI yellow (`\\x1B[33m`) if colors are enabled.

### Styles

- `bold_str` - Wraps `str` with ANSI bold (`\\x1B[1m`) if colors are enabled.
- `dim_str` - Wraps `str` with ANSI dim (`\\x1B[2m`) if colors are enabled.
- `reset_str` - Returns the ANSI reset sequence if colors are enabled, empty string otherwise.
- `underline_str` - Wraps `str` with ANSI underline (`\\x1B[4m`) if colors are enabled.

## tty

The TTY module answers whether a stream is attached to a real terminal, and how wide that terminal is.


### Constants

- `TTY_STDIN`
- `TTY_STDOUT`
- `TTY_STDERR`
- `TTY_DEFAULT_WIDTH`

### Terminal detection

- `is_stderr_terminal` - True when standard error is attached to a terminal.
- `is_stdin_terminal` - True when standard input is attached to a terminal.
- `is_stdout_terminal` - True when standard output is attached to a terminal.
- `is_stream_terminal` - True when the standard stream `fd` (`TTY_STDIN` / `TTY_STDOUT` / `TTY_STDERR`) is attached to a terminal.

### Terminal geometry

- `terminal_columns`

## apply_in_context

The APPLY_IN_CONTEXT module extends apply operations to work across different execution contexts, enabling cross-context function invocation with packed arguments.


### Function annotations

- `apply_in_context` - [apply_in_context] function annotation.

## cross_context

The CROSS_CONTEXT module requires a `shared` module after the prerequisite walk - from a macro, a simulate macro or an `[init]`, never from a running script, which has no compile for the module to join - and calls into its macro context by function name, so a module that is expensive to bring up and only sometimes needed comes in at the point that decides it needs it.


### Late require

- `macro_context_of` - The macro context of the shared module `name`, required now when the process lacks it; null, logged, when the module has no macros.
- `require_module_now` - Compiles `name` - a `shared` module - and its prerequisites now, under the compiling program's policies and file access, or answers the one already in the process; a panic outside a compile (a macro, a simulate macro, an `[init]`).

## module_group

The MODULE_GROUP module calls into every member of a module group. `call_module_group("group", "entry", args...)` expands at compile time to one `member::entry(args...)` call per module registered under the group, sorted by member path, so the module that wrote `require [group]` reaches its members without naming them. A member without the entry is a compile error naming the call; a group nothing joined expands to nothing.


### Call macros

- `call_module_group` - `call_module_group("group", "entry", args...)` expands to one `member::entry(args...)` call per member, sorted by member path, an empty group to nothing; an argument that is not a constant or a plain variable is bound to a local ahead of the calls, so it is evaluated once.

### Group members

- `module_group_module_name` - The module a group member's require path names: its last segment, less the `.das` a relative spelling (`./foo.das`) may carry.

## apply

The APPLY module provides the `apply` macro for iterating over struct, tuple, and variant fields at compile time. Each field is visited with its name and a reference to its value, enabling generic per-field operations like serialization, printing, and validation.


### Call macros

- `apply_imm` - Struct-only, faster sibling of `apply` for a hot field walk (e.g.
- `apply` - Visits every field of a struct, tuple, or variant with a per-field block.

## archive

The ARCHIVE module implements general-purpose serialization infrastructure. It provides the `Archive` type and `serialize` functions for reading and writing binary data. Custom types are supported by implementing `serialize` for each type.


### Structures

- `Archive` - Archive is a combination of serialization stream, and state (version, and reading status).

### Classes

- `Serializer` - Base class for serializers.
- `MemSerializer` - This serializer stores data in memory (in the array<uint8>) internal data buffer current reading offset last error code
- `MemSerializer.write` - Appends bytes at the end of the data.
- `MemSerializer.read` - Reads bytes from data, advances the reading position.
- `MemSerializer.error` - Sets the last error code.
- `MemSerializer.OK` - Implements 'OK' method, which returns true if the serializer is in a valid state.
- `MemSerializer.extractData` - Extract the data from the serializer.
- `MemSerializer.getCopyOfData` - Returns copy of the data from the serializer.
- `MemSerializer.getLastError` - Returns last serialization error.

### Serialization

- `read_raw` - Read raw data (straight up bytes for raw pod)
- `serialize` - Serializes array.
- `serialize_raw` - Serialize raw data (straight up bytes for raw pod)
- `write_raw` - Write raw data (straight up bytes for raw pod)

### Memory archive

- `mem_archive_load` - Loads the object from a memory archive, reading from a borrowed view without cloning.
- `mem_archive_save` - Saves the object to a memory archive.

## array_boost

The ARRAY_BOOST module extends array operations with temporary array views over fixed-size arrays and C++ handled vectors, emptiness checks, sub-array views, and arithmetic operators on fixed-size arrays.


### Temporary arrays

- `temp_array` - Creates temporary array from the given object.

### Empty check

- `empty` - returns true if 'v' has 0 elements.

### Sub-array view

- `array_view` - creates a subview of an existing borrowed array, valid only within the block
- `pod_view` - reads one raw POD value from borrowed bytes at an arbitrary, possibly unaligned, byte offset; bounds-checked byte copy — the alignment-free complement of the typed `array_view`

## assert_once

The ASSERT_ONCE module provides the `assert_once` macro — an assertion that triggers only on its first failure. Subsequent failures at the same location are silently ignored, preventing assertion storms in loops or frequently called code.


### Function annotations

- `AssertOnceMacro` - This macro converts `assert_once(expr, message)` to equivalent inline code.

### Assertion

- `assert_once` - Same as assert, only the check will be not be repeated after the assertion failed the first time.

## only_nttp

The ONLY_NTTP module provides the `[only_nttp]` function annotation — a guard for benchmark kernels. Compilation fails if the annotated function calls a call-based extern that is not bound through `addExternInline` (the NTTP interpreter node flavor), naming the offending call. This keeps a measured fastpath honest: a silently downgraded binding cannot skew the benchmark.


### Function annotations

- `only_nttp` - Fails the compile when the annotated function calls an extern not bound with `addExternInline`.

### Classes

- `OnlyNttpAnnotation`
- `OnlyNttpAnnotation.patch`

## ast_block_to_loop

The AST_BLOCK_TO_LOOP module provides an AST transformation macro that converts block-based iteration patterns into explicit loop constructs. Used internally by other macro libraries for optimization.


### Block to loop conversion

- `convert_block_to_loop` - Converts closure block to loop.

## ast_boost

The AST_BOOST module provides high-level utilities for working with the AST. It includes helpers for creating expressions, types, and declarations, quote-based AST construction, and common AST query and transformation patterns used by macro authors.


### Type aliases

- `AnnotationDeclarationPtr` - Type alias for `AnnotationDeclaration?`, used when constructing or attaching annotation declarations to functions, blocks, or structures.
- `DebugExpressionFlags` - Bitfield controlling `debug_expression` output formatting.

### Function annotations

- `tag_function` - The `[tag_function]` function annotation — attaches named tags to a function so that `[tag_function_macro]`-based annotations can discover and process it.
- `macro` - The `[macro]` function annotation — marks a function to run only during macro module compilation, gating its body behind `is_compiling_macros`.

### Variant macros

- `better_rtti_in_expr` - Variant macro that enables improved RTTI type matching in `is` and `as` expressions.

### Structure macros

- `dirty_infer_macro` - The `[dirty_infer_macro]` structure annotation — registers an `AstPassMacro` subclass that is invoked during the dirty infer compilation pass.
- `reader_macro` - The `[reader_macro]` structure annotation — registers an `AstReaderMacro` subclass as a named reader macro invoked by the `%name~...~~` syntax during parsing.
- `tag_function_macro` - The `[tag_function_macro]` structure annotation — registers an `AstFunctionAnnotation` that is automatically applied to every function carrying a matching `[tag_function(tag)]` tag.
- `contract` - The `[contract]` structure annotation — registers an `AstFunctionAnnotation` subclass as a named function contract that validates arguments or return values.
- `post_compile_macro` - The `[post_compile_macro]` structure annotation — registers an `AstPassMacro` subclass that is invoked once the module is compiled and its gc root collected, so every node the module still owns is a live one.
- `structure_macro` - The `[structure_macro]` structure annotation — registers an `AstStructureAnnotation` subclass as a named annotation applicable to structures and classes.
- `type_macro` - The `[type_macro]` structure annotation — registers an `AstTypeMacro` subclass as a named macro that can intercept and transform type expressions.
- `lint_macro` - The `[lint_macro]` structure annotation — registers an `AstPassMacro` subclass that is invoked during the lint compilation pass.
- `comment_reader` - The `[comment_reader]` structure annotation — registers an `AstCommentReader` subclass as a named comment reader invoked during parsing.
- `global_lint_macro` - The `[global_lint_macro]` structure annotation — registers an `AstPassMacro` subclass that is invoked during the global lint compilation pass.
- `tag_structure` - The `[tag_structure]` structure annotation — attaches named boolean tags to a structure, allowing macro code to discover and process tagged structures.
- `for_loop_macro` - The `[for_loop_macro]` structure annotation — registers an `AstForLoopMacro` subclass as a named macro that can transform `for` loop expressions.
- `enumeration_macro` - The `[enumeration_macro]` structure annotation — registers an `AstEnumerationAnnotation` subclass as a named annotation applicable to enumerations.
- `post_rewrite_macro` - The `[post_rewrite_macro]` structure annotation — registers an `AstPassMacro` subclass that is invoked right after any user macro reports that it rewrote the tree: the inference macro loops and the optimizer's macro loop.
- `variant_macro` - The `[variant_macro]` structure annotation — registers an `AstVariantMacro` subclass as a named macro that can customize `is`, `as`, and `?as` variant operations.
- `infer_macro` - The `[infer_macro]` structure annotation — registers an `AstPassMacro` subclass that is invoked during the type inference compilation pass.
- `function_macro` - The `[function_macro]` structure annotation — registers an `AstFunctionAnnotation` subclass as a named function annotation available to the compiler.
- `simulate_macro` - The `[simulate_macro]` structure annotation — registers an `AstSimulateMacro` subclass as a named macro invoked during the simulation (code generation) phase.
- `block_macro` - The `[block_macro]` structure annotation — registers an `AstBlockAnnotation` subclass as a named block-level annotation available to the compiler.
- `typeinfo_macro` - The `[typeinfo_macro]` structure annotation — registers an `AstTypeInfoMacro` subclass as a named macro that extends the `typeinfo name(...)` built-in.
- `capture_macro` - The `[capture_macro]` structure annotation — registers an `AstCaptureMacro` subclass as a named capture macro that can customize lambda capture behavior.
- `call_macro` - The `[call_macro]` structure annotation — registers an `AstCallMacro` subclass as a named call-expression macro available during compilation.
- `optimization_macro` - The `[optimization_macro]` structure annotation — registers an `AstPassMacro` subclass that is invoked during the optimization compilation pass.

### Classes

- `MacroMacro` - Implements the `[macro]` function annotation, which wraps the function body so it only executes during macro module compilation.
- `MacroMacro.apply` - Wraps the annotated function body in an `is_compiling_macros` guard and sets `macroInit` flag so it only runs during macro module compilation.
- `TagFunctionAnnotation` - Implements the `[tag_function]` function annotation, which attaches named boolean tags to functions so they can be discovered and processed by `[tag_function_macro]`.
- `TagFunctionAnnotation.apply` - Validates that all `[tag_function(...)]` annotation arguments are tag names (boolean flags) and rejects any non-boolean arguments with an error.
- `TagStructureAnnotation` - Implements the `[tag_structure]` structure annotation, which attaches named boolean tags to structures for later discovery by macro code.
- `TagStructureAnnotation.apply` - Validates that all `[tag_structure(...)]` annotation arguments are tag names (boolean flags) and rejects any non-boolean arguments with an error.
- `SetupAnyAnnotation` - This is base class for any annotation or macro setup.
- `SetupAnyAnnotation.apply` - Generates a macro-init function that constructs an instance of the annotated class and registers it with the compiler under the specified `name`.
- `SetupAnyAnnotation.setup_call` - Populates the registration call arguments — by default adds the annotation `name` as a string constant; overridden in subclasses to add extra parameters.
- `SetupFunctionAnnotation` - Base class for creating function annotations via the `[function_macro]` structure annotation; registers an `AstFunctionAnnotation` with the compiler.
- `SetupBlockAnnotation` - Base class for creating block annotations via the `[block_macro]` structure annotation; registers an `AstBlockAnnotation` with the compiler.
- `SetupStructureAnnotation` - Base class for creating structure annotations via the `[structure_macro]` structure annotation; registers an `AstStructureAnnotation` with the compiler.
- `SetupEnumerationAnnotation` - Base class for creating enumeration annotations via the `[enumeration_macro]` structure annotation; registers an `AstEnumerationAnnotation` with the compiler.
- `SetupContractAnnotation` - Base class for creating function contract annotations via the `[contract]` structure annotation; registers an `AstFunctionAnnotation` as a contract.
- `SetupReaderMacro` - Base class for creating reader macros via the `[reader_macro]` structure annotation; registers an `AstReaderMacro` with the compiler.
- `SetupCommentReader` - Base class for creating comment readers via the `[comment_reader]` structure annotation; registers an `AstCommentReader` with the compiler.
- `SetupVariantMacro` - Base class for creating variant macros via the `[variant_macro]` structure annotation; registers an `AstVariantMacro` with the compiler.
- `SetupForLoopMacro` - Base class for creating for-loop macros via the `[for_loop_macro]` structure annotation; registers an `AstForLoopMacro` with the compiler.
- `SetupCaptureMacro` - Base class for creating capture macros via the `[capture_macro]` structure annotation; registers an `AstCaptureMacro` with the compiler.
- `SetupTypeMacro` - Base class for creating type macros via the `[type_macro]` structure annotation; registers an `AstTypeMacro` with the compiler.
- `SetupSimulateMacro` - Base class for creating simulate macros via the `[simulate_macro]` structure annotation; registers an `AstSimulateMacro` with the compiler.
- `SetupCallMacro` - Base class for creating call macros via the `[call_macro]` structure annotation; registers an `AstCallMacro` with the compiler.
- `SetupTypeInfoMacro` - Base class for creating typeinfo macros via the `[typeinfo_macro]` structure annotation; registers an `AstTypeInfoMacro` with the compiler.
- `SetupInferMacro` - Base class for creating infer pass macros via the `[infer_macro]` structure annotation; registers an `AstPassMacro` that runs during the type inference pass.
- `SetupDirtyInferMacro` - Base class for creating dirty-infer pass macros via the `[dirty_infer_macro]` structure annotation; registers an `AstPassMacro` that runs during the dirty infer pass.
- `SetupPostRewriteMacro` - Base class for creating post-rewrite pass macros via the `[post_rewrite_macro]` structure annotation; registers an `AstPassMacro` that runs right after any user pass macro reports that it rewrote the tree.
- `SetupPostCompileMacro` - Base class for creating post-compile pass macros via the `[post_compile_macro]` structure annotation; registers an `AstPassMacro` that runs once the module is compiled and its gc root collected.
- `SetupLintMacro` - Base class for creating lint pass macros via the `[lint_macro]` structure annotation; registers an `AstPassMacro` that runs during the lint pass.
- `SetupGlobalLintMacro` - Base class for creating global lint pass macros via the `[global_lint_macro]` structure annotation; registers an `AstPassMacro` that runs during the global lint pass.
- `SetupOptimizationMacro` - Base class for creating optimization pass macros via the `[optimization_macro]` structure annotation; registers an `AstPassMacro` that runs during the optimization pass.
- `TagFunctionMacro` - [tag_function_macro] implementation.
- `TagFunctionMacro.apply` - Extends `SetupAnyAnnotation` apply to extract the required `tag` argument and register a `setup_tag_annotation` call that links the annotation to tagged functions.
- `TagFunctionMacro.setup_call` - Overrides the default `setup_call` to pass both the annotation `name` and the `tag` string as arguments to `setup_tag_annotation`.
- `BetterRttiVisitor` - An `AstVariantMacro` that replaces `is`, `as`, and `?as` variant operations on `Expression` subclasses with runtime `__rtti` string checks and casts.
- `BetterRttiVisitor.visitExprIsVariant` - Visitor override that replaces `expr is Type` on `Expression` subclasses with an `__rtti` string comparison, returning `true` if the runtime type matches.
- `BetterRttiVisitor.visitExprAsVariant` - Visitor override that replaces `expr as Type` on `Expression` subclasses with an RTTI-checked cast via `__rtti`, panicking on mismatch.
- `BetterRttiVisitor.visitExprSafeAsVariant` - Visitor override that replaces `expr ?as Type` on `Expression` subclasses with an RTTI-checked cast via `__rtti`, returning `null` on mismatch instead of panicking.

### Containers

- `emplace_new` - Moves a newly created pointer (Expression, TypeDecl, Variable, or MakeFieldDecl) into a vector container.

### Textual descriptions of the objects

- `debug_expression` - Returns a hierarchical, Lisp-like textual dump of an `ExpressionPtr` tree showing RTTI types, field values, and nested sub-expressions for debugging.
- `describe` - Returns a human-readable textual representation of an AST object (`AnnotationArgumentList`, `AnnotationDeclaration`, `AnnotationList`, `Variable`, or `Expression`).
- `describe_bitfield` - Returns a textual representation of the set bits in a bitfield value, listing the names of all active flags joined by the specified `merger` string.
- `describe_function_short` - Returns a short human-readable description of the given function, including its name and signature.

### Queries

- `find_annotation` - Looks up an `Annotation` by name within the specified module during compilation and returns a pointer to it, or `null` if not found.
- `find_arg` - Searches an `AnnotationArgumentList` for an argument by name and returns its `RttiValue`; returns `nothing` if the argument is not present.
- `find_argument_index` - Searches the `argNames` of a `TypeDeclPtr` (tuple or variant) for the given name and returns its zero-based index, or `-1` if not found.
- `find_unique_function` - Searches the compiling program for exactly one non-generic function with the given name; returns it or `null` if zero or multiple matches exist.
- `find_unique_generic` - Searches the compiling program for exactly one generic function with the given name; returns it or `null` if zero or multiple matches exist.
- `getVectorElementCount` - Returns the number of scalar elements in a vector `Type` (e.g., 2 for `float2`/`range`, 3 for `float3`, 4 for `float4`), or 0 for non-vector types.
- `getVectorElementSize` - Returns the byte size of a single scalar element in a vector `Type` — 8 for `range64`/`urange64`, 4 for all other vector types.
- `getVectorElementType` - Returns the scalar `Type` of each element in a vector type (e.g., `tFloat` for `float2`, `tInt` for `int3` and `range`, `tInt64` for `range64`).
- `getVectorOffset` - Returns the zero-based element index for a named swizzle component (`x`/`y`/`z`/`w` or `r`/`g`/`b`/`a`) within a vector `Type`, or `-1` if out of bounds.
- `get_for_source_index` - Returns the zero-based index of a given iterator variable or source expression within a `for` loop's source list, or `-1` if not found.
- `get_workhorse_types` - Returns a fixed array of all commonly used `Type` values — booleans, strings, pointers, numeric scalars, enumerations, bitfields, vectors, and ranges.
- `isCMRES` - Returns true if the given function uses copy-on-return elimination (CMRES), meaning its return value is constructed directly in the caller's memory rather than being copied.
- `isCMRESType` - Returns `true` if a `TypeDeclPtr` represents a reference type without an explicit `ref` flag, meaning it will use copy-or-move-on-stack semantics.
- `isExprCallFunc` - Returns `true` if the expression's RTTI tag is `ExprCallFunc`, `ExprOp`, `ExprNew`, or `ExprCall` — i.e., any function-call-like expression.
- `isExpression` - Returns `true` if the given `TypeDeclPtr` refers to an `ast` module handled type whose name starts with `Expr`, including pointer-to-expression types.
- `isMakeLocal` - Returns `true` if the expression is any `ExprMakeLocal` subclass: `ExprMakeStruct`, `ExprMakeArray`, `ExprMakeTuple`, or `ExprMakeVariant`.
- `isVectorType` - Returns `true` if the given `Type` is a vector, range, or urange type (`int2`..`int4`, `uint2`..`uint4`, `float2`..`float4`, `range`, `urange`, `range64`, `urange64`).
- `is_class_method` - Returns `true` if a `TypeDeclPtr` field represents a class method — a non-dim `tFunction` whose first argument is the class structure (or a parent of it).
- `is_same_or_inherited` - Returns `true` if `child` is the same `Structure` as `parent` or is transitively inherited from `parent` by walking the parent chain.

### Annotations

- `add_annotation_argument` - Adds a typed annotation argument (`bool`, `int`, `float`, `string`, or `AnnotationArgument`) to an `AnnotationArgumentList` and returns the new argument index.
- `append_annotation` - Creates an `AnnotationDeclaration` for the named annotation (with optional typed arguments) and attaches it to a `Function`, `ExprBlock`, or `Structure`.
- `append_loop_flag` - Adds a name-only per-loop hint flag such as vectorize, unroll, or unroll_full to a for-loop expression.
- `append_loop_hint` - Adds an integer-valued per-loop hint such as unroll_count, vectorize_width, or interleave_count to a for-loop expression.
- `append_unroll` - Adds the plain unroll hint when count is zero or negative, otherwise adds unroll_count with the given count.
- `append_vectorize` - Marks the loop with the vectorize hint.

### Expression generation

- `convert_to_expression` - Converts a runtime value of any supported type to an equivalent AST `ExpressionPtr` that would produce that value when compiled, using `typeinfo` for reflection.
- `make_static_assert_false` - Creates an `ExprStaticAssert` expression node that always fails at compile time with the given error message text.
- `override_method` - Replaces the initializer of a field named `name` in the given structure with an `@@funcName` function address cast, effectively overriding that class method.
- `panic_expr_as` - Helper function that panics with `"invalid 'as' expression or null pointer dereference"` and returns `null` — used as the failure branch in `as` variant casts.

### Type generation

- `function_to_type` - Constructs a `TypeDeclPtr` of `tFunction` base type from a `FunctionPtr`, capturing its argument types and names plus the return type.
- `make_fixed_array_type` - Wraps `element` into a fixed-array type node (`Type tFixedArray`) of size `total`.

### Type casts

- `operator as BuiltInFunction` - Casts a `Function?` to `BuiltInFunction?` via `reinterpret`, verifying the target is a built-in function first (panics otherwise).
- `operator as ExternalFnBase` - Casts a `Function?` to `ExternalFnBase?` via `reinterpret`, verifying it is a property-flagged built-in function first (panics otherwise).
- `operator as FunctionAnnotation` - Casts an `Annotation?` to `FunctionAnnotation?` via `reinterpret`, verifying the annotation kind first (panics otherwise).
- `operator as StructureAnnotation` - Casts an `Annotation?` to `StructureAnnotation?` via `reinterpret`, verifying the annotation kind first (panics otherwise).
- `operator is BuiltInFunction` - Returns `true` if the given `Function?` has the `builtIn` flag set, indicating it is a `BuiltInFunction`; returns `false` for any other type.
- `operator is ExternalFnBase` - Returns `true` if the given `Function?` is both `builtIn` and has the `propertyFunction` flag, indicating it is an `ExternalFnBase`; returns `false` otherwise.
- `operator is FunctionAnnotation` - Returns `true` if the given `Annotation?` is a `FunctionAnnotation` according to its `isFunctionAnnotation` property.
- `operator is StructureAnnotation` - Returns `true` if the given `Annotation?` is a `StructureAnnotation` according to its `isStructureAnnotation` property.
- `walk_and_convert` - Recursively walks raw data bytes using a `TypeDeclPtr` schema and builds an equivalent AST expression tree that would reproduce that data when compiled.

### Setup

- `setup_call_list` - Creates or locates a compilation-phase setup function (`__setup_macros`) and returns its body `ExprBlock` so callers can append registration calls to it.
- `setup_macro` - Creates or locates a macro initialization function (`__setup_macros`) guarded by `is_compiling_macros` and returns its body block for appending macro registration code.
- `setup_tag_annotation` - Creates an `AstFunctionAnnotation` instance and automatically applies it to every function that carries a matching `[tag_function(tag)]` annotation in the module.

## ast_cursor

Position-based AST queries. Given a file, line, and column, finds all expression nodes at that cursor position with innermost-first ordering and enclosing function context. Used for implementing IDE features like goto-definition, type-of, and find-references.


### Enumerations

- `DeclarationKind` - What `find_declaration_at_cursor` or `declaration_of` found: which of `DeclarationHit`'s pointers is set.

### Structures

- `CursorHit` - A single hit at the cursor position: an expression node, or when asked for a variable declaration or a type name.
- `DeclarationHit` - A declaration - the one whose name the cursor is on, or the one a type denotes: at most one of the pointers is set, by `kind`.

### Classes

- `CursorVisitor`
- `CursorVisitor.preVisitTypeDecl`
- `CursorVisitor.preVisitStructureField`
- `CursorVisitor.visitStructureField`
- `CursorVisitor.preVisitFunction`
- `CursorVisitor.visitFunction`
- `CursorVisitor.preVisitFunctionArgument`
- `CursorVisitor.preVisitExpression`
- `CursorVisitor.preVisitExprBlockArgument`
- `CursorVisitor.preVisitExprLetVariable`
- `CursorVisitor.preVisitExprForVariable`
- `CursorVisitor.in_target_file`
- `CursorVisitor.cursor_on_variable`
- `CursorVisitor.add_variable_at_cursor`
- `CursorVisitor.generated_variable`
- `CursorVisitor.on_field_name`
- `CursorVisitor.cursor_on_token`

### Cursor queries

- `cursor_inside` - Check whether a point (line, col) is inside the span described by `at`.
- `declaration_of` - The declaration a type denotes - its typedef, structure, enumeration or C++-bound annotation; `none` for a bare type.
- `find_at_cursor` - Find all expression nodes at the given cursor position, innermost first; `file` is a substring of `FileInfo.name` ("" matches any); nodes inside a synthesized member and compiler-made variables are never hits.
- `find_at_cursor_in_function` - Find all expression nodes at the given cursor position within a single function body.
- `find_declaration_at_cursor` - The declaration whose name is at the cursor, the parent a `class` line names included; `line_text` "" makes it a line-only lookup, `file` is a substring of `FileInfo.name`.
- `typedef_named` - The type declared under the typedef `name` - in `mod` when given, else in the program's own module, else in any module it requires; null when none declares it.

### Source text

- `bare_name` - "Animal`speak" -> "speak": the token the source spells at a method's declaration and call sites.
- `declared_name_column` - Where the line a declaration starts on spells its `name`; `at.column` when it does not.
- `name_column` - Where `line_text` spells `name` as a whole word (0-based byte column), else `hint`.
- `source_name` - The name the source spells for a compiler-renamed variable (a generator's loop variable); any other name unchanged.
- `spells_name` - True when `line_text` spells `name` as a whole word starting exactly at `col`.
- `word_at` - The identifier the 0-based byte column `col` is on in `line_text`, with its start column; a caret right after a word is on it, as in an editor, and the word is empty when `col` is on none.

### Class members

- `field_at` - Where the field `name` of `st` is declared - the parent's line for an inherited field; a zero position when there is no such field.
- `field_name_at` - Where a field access spells its field name - a method call's name sits past the `->` its node spans; a zero position for any other node.
- `is_own_field` - True for a field the source declares in its struct's or class's own body - an override included - not one the compiler made (`__rtti`, `__finalize`) or a parent's copy.
- `value_struct` - The struct a field access reads its field from - through a pointer when `value` is one; null for any other value.

### Result inspection

- `describe` - Pretty-print a CursorHit for debugging.
- `hit_at` - The position of the node the hit holds: its expression, variable or type.
- `is_synthesized` - True for a member the compiler made on the `class` line (`Foo'__finalize`), never for a lambda or generator body or a generic instance.

## ast_used

The AST_USED module implements analysis passes that determine which AST nodes are actually used in the program. This information is used for dead code elimination, tree shaking, and optimizing generated output.


### Structures

- `OnlyUsedTypes` - Collection of all structure and enumeration types that are used in the AST.

### Collecting type information

- `collect_used_types` - Goes through list of functions `vfun` and variables `vvar` and collects list of which enumeration and structure types are used in them.

## ast_match

AST pattern matching via reverse reification. Matches expressions and blocks against structural patterns using the same tag system as `qmacro` but in reverse: `$e` clones a matched expression, `$v` extracts a constant value, `$i` extracts an identifier name, `$c` extracts a call name, `$f` extracts a field name, `$b` captures a statement range, `$t` captures a type, and `$a` captures remaining arguments.


### Enumerations

- `QMatchError`

### Structures

- `QMatchResult`

### Call macros

- `qmatch`
- `qmatch_block`
- `qmatch_function` - `qmatch_function(func, block_pattern)` — match function body against block pattern.

### Match execution

- `qm_run` - Invoke a matching block on the expression.
- `qm_run_function` - Run matching block on a function, passing both body and Function pointer.

### Call matching

- `match_call_in_linq` - Match an `ExprCall` to `name` in the `linq` module.
- `match_call_in_module` - Match an `ExprCall` to `name` in module `modName`, transparent to generic instantiation.

### Result builders

- `qm_fail_const` - Return a failure — constant type does not match $v extraction target.
- `qm_fail_field` - Return a failure — a named field differs between pattern and expression.
- `qm_fail_list` - Return a failure — block statement count mismatch.
- `qm_fail_null` - Return a failure — null expression encountered.
- `qm_fail_rtti` - Return a failure — expression RTTI type does not match the pattern.
- `qm_fail_type` - Return a failure — TypeDecl comparison failed.
- `qm_fail_wildcard` - Return a failure — wildcard could not find a matching statement.
- `qm_ok` - Return a successful match result.

### Capture staging

- `qm_tmp_args` - Empty argument-list capture temporary (see `qm_tmp_expr`).
- `qm_tmp_expr` - Null-initialized capture temporary — matchers stage captures in these and commit to the user's variables only after the whole match succeeds.
- `qm_tmp_stmts` - Empty statement-list capture temporary (see `qm_tmp_expr`).

### Value extraction

- `extract_const_string` - For `ExprConstString` constants return `(true, value)`; otherwise `(false, "")`.
- `qm_extract` - Extract bool from ExprConstBool.

### Name and type extraction

- `qm_call_name_matches` - Check if a call's name matches, accounting for generic instantiation and module qualification.
- `qm_extract_call_name` - Extract call name from ExprCall.
- `qm_extract_field_name` - Extract field name from ExprField, ExprSafeField, or variant expressions.
- `qm_extract_name` - Extract the "name" field from common expression types.
- `qm_extract_type` - Extract TypeDeclPtr from ExprTypeDecl.
- `qm_rtti` - Return the RTTI type name of an expression.

### Argument filtering

- `qm_count_real_args` - Count arguments excluding fakeContext and fakeLineInfo.
- `qm_count_real_call_args` - Count call arguments excluding fakeContext and fakeLineInfo expressions.
- `qm_extract_remaining_args` - Collect remaining real arguments (skipping fakeContext/fakeLineInfo) starting from logical index.
- `qm_is_fake_arg` - Returns true if the expression is a compiler-injected fakeContext or fakeLineInfo.
- `qm_real_arg_index` - Map logical argument index (skipping fakeContext/fakeLineInfo) to physical index.
- `qm_real_call_arg_index` - Map logical call argument index (skipping fake args) to physical index.

### Block scanning

- `qm_extract_stmts` - Extract statements [from_pos, to_pos) from a block into an array (cloned).
- `qm_scan` - Scan block statements from pos to len, trying matcher at each position.

### Constant construction

- `qm_make_zero_const` - Create a zero-value ExprConst for the range first component, reporting at the call it stands in for.

### AST reconstruction

- `qm_convert_comprehension` - Reverse generateComprehension: ExprInvoke(invoke, ExprMakeBlock(closure)) → ExprArrayComprehension.
- `qm_convert_local_function` - Reverse generateLocalFunction: ExprAddr → ExprMakeBlock(isLocalFunction=true).
- `qm_resolve_comprehension` - If expr is ExprArrayComprehension, shallow-clone it.
- `qm_resolve_local_function` - If expr is ExprMakeBlock, clone it.

### AST normalization

- `peel_lambda_rename_2vars` - 2-arg peel variant for shapes like aggregate's `block<(acc, x) : AGG>`.
- `peel_lambda_rename_var` - Peel a single-arg, single-return lambda by renaming its bound variable to `argName` in the cloned body.
- `peel_lambda_replace_var` - Variant of `peel_lambda_rename_var` — substitutes the bound variable with an arbitrary expression in the cloned body.
- `peel_lambda_single_return` - For a single-argument, single-return lambda `@(x : T) => expr`, return the body `expr`.
- `peel_tuple_field_read` - `true` when `expr` matches `<bindName>._<fieldIndex>` — a tuple-slot read on a named bind.
- `qm_peel_ref2value` - Strip `ExprRef2Value` wrappers from `e` in place.

### Hygienic names

- `qn` - Build a backtick-quoted qname unique to `at` for the given `prefix`.

## async_boost

The ASYNC_BOOST module implements an async/await pattern for daslang using generator-based cooperative multitasking. It provides the `[async]` function annotation, `await` for waiting on results, and `await_next_frame` for suspending until the next step. Under the hood every `[async]` function is transformed into a state-machine generator — no threads, channels, or job queues are involved.


### Function annotations

- `AwaitMacro` - Function annotation that implements coroutine await semantics.
- `AwaitCoroutineMacro` - This macro converts an `await(<coroutine>)` expression into equivalent inline code.
- `async` - This macro converts function into generator.

### Awaiting

- `await` - This function is used to wait for the result of the async function.
- `await_next_frame` - This function is used to suspend coroutine until next frame.

### Running async tasks

- `async_race` - This function runs two async functions concurrently and returns the index (0 or 1) of whichever finishes first.
- `async_run` - This function runs async function until it is finished.
- `async_run_all` - This function runs all async function until they are finished (in parallel, starting from the last one).
- `async_timeout` - This function runs an async function for at most `max_frames` frames.

## base64

The BASE64 module implements Base64 encoding and decoding. It provides `base64_encode` and `base64_decode` for converting between binary data (strings or `array<uint8>`) and Base64 text representation.


### Encoding

- `BASE64_ENCODE_OUT_SIZE` - Returns the encoded output size for binary data of length `s`.
- `base64_encode` - Encodes a string to its Base64 text representation.

### Decoding

- `BASE64_DECODE_OUT_SIZE` - Returns the maximum decoded output size for a Base64 string of length `s`.
- `base64_decode` - Decodes a Base64-encoded string.

## bitfield_boost

The BITFIELD_BOOST module provides utility macros for working with bitfield types including conversion between bitfield values and strings, and iteration over set bits.


### Bitfield element access

- `auto(TT)&[]&&=` - && assignment for bitfield bit at index i
- `auto(TT)&[]=` - set bitfield bit at index i to v
- `auto(TT)&[]^^=` - toggle bitfield bit at index i if v is true
- `auto(TT)&[]||=` - || assignment for bitfield bit at index i
- `auto[]` - get bitfield bit at index i

### Iteration

- `each_bit` - Iterates over each bit of a bitfield value, yielding true or false for each bit.

## bitfield_trait

The BITFIELD_TRAIT module implements reflection utilities for bitfield types: converting bitfield values to and from human-readable strings, iterating over individual set bits, and constructing bitfield values from string names.


### Function annotations

- `EachBitfieldMacro` - This macro converts `each(bitfield)` to equivalent inline code.
- `EachBitNameBitfieldMacro` - This macro converts `each(bitfield)` to equivalent inline code.

### Iteration

- `each` - Iterates over the names of a bitfield type, yielding each bit as a bitfield value (1ul << bitIndex).
- `each_bit_name` - Iterates over the names of a bitfield type, yielding each bit name as a string.

## bool_array

The BOOL_ARRAY module provides a compact boolean array implementation using bit-packing. Each boolean value uses a single bit instead of a byte, providing an 8x memory reduction compared to `array<bool>`.


### Structures

- `BoolArray` - A dynamic array of booleans, stored as bits.
- `BoolArray.finalize` - Releases the memory used by the BoolArray.
- `BoolArray.[]` - Get the boolean value at the given index.
- `BoolArray.[]=` - Set the boolean value at the given index.
- `BoolArray.[]^^=` - Perform XOR operation on the boolean value at the given index.
- `BoolArray.[]&&=` - Perform AND operation on the boolean value at the given index.
- `BoolArray.[]||=` - Perform OR operation on the boolean value at the given index.
- `BoolArray.clear` - Clear the BoolArray.
- `BoolArray.reserve` - Reserve capacity for the BoolArray.
- `BoolArray.resize` - Resize the BoolArray to the new size.
- `BoolArray.push` - Push a new boolean value to the end of the BoolArray.
- `BoolArray.pop` - Pop the last boolean value from the BoolArray and return it.
- `BoolArray.length` - Get the length of the BoolArray.
- `BoolArray.erase` - Erase the boolean value at the given index from the BoolArray.
- `BoolArray.insert` - Insert a boolean value at the given index in the BoolArray.
- `BoolArray.to_string` - Convert the BoolArray to a string representation.
- `BoolArray.data_pointer` - Get the data pointer of the BoolArray.

### Iteration

- `each` - Returns an iterator over all boolean values in the BoolArray.

## build_const

The BUILD_CONST module provides compile-time constants from the build environment. Each `build_*` accessor is a call macro that reads the daslang process's command line or environment variables while the requiring module compiles, and splices the answer into the code as a literal — feed a global `let` and gate code with `static_if` to erase the disabled branch entirely. Interpreted and JIT runs resolve the constants per launch; an `-exe` build bakes them at build time.


### Call macros

- `build_arg_int` - `build_arg_int(flag, dflt)` — integer form of `build_arg_value`; garbage is a compile error.
- `build_env_flag` - `build_env_flag(name)` — boolean env rule: unset/""/0/false/off/no are false, anything else true.
- `build_flag` - `build_flag(flag, env)` — both-carrier boolean: true when `flag` is on the command line, else the env rule on `env`.
- `build_arg_value` - `build_arg_value(flag, dflt)` — the `--name value` / `--name=value` argument, or `dflt`.
- `build_int` - `build_int(flag, env, dflt)` — both-carrier integer; a garbage value on either carrier is a compile error.
- `build_has_arg` - `build_has_arg(flag)` — true when `flag` appears verbatim on the command line.
- `build_env_int` - `build_env_int(name, dflt)` — integer env value; set-but-empty is `dflt`, garbage is a compile error.
- `build_env_value` - `build_env_value(name, dflt)` — the environment value, or `dflt` when unset or empty.
- `build_value` - `build_value(flag, env, dflt)` — both-carrier string: command line beats environment beats `dflt`.

## class_boost

The CLASS_BOOST module provides macros for extending class functionality, including the `[serialize_as_class]` annotation for automatic serialization and common class patterns like abstract method enforcement.


### Function annotations

- `class_method` - Turns a static method into a class method by adding a `self` argument of the class type as the first argument, and wrapping the function body in `with (self) { ...
- `explicit_const_class_method` - Same as `[class_method]` but marks the `self` parameter with `explicitConst`, allowing overloading of const and non-const class methods.

## command_line

Command-line utilities for daslang scripts and standalone executables. Provides functions for locating the daslang interpreter, which is essential when a script or standalone executable needs to spawn daslang subprocesses.


### Executable path

- `get_das_exe`

## constant_expression

The CONSTANT_EXPRESSION module provides the `[constant_expression]` function annotation. Functions marked with this annotation are evaluated at compile time when all arguments are constants, replacing the call with the computed result.


### Function annotations

- `constexpr` - Checks selected function arguments and fails when any receives a non-constant expression.
- `constant_expression` - This function annotation implements constant expression folding for the given arguments.

### Macro helpers

- `isConstantExpression` - This macro function returns true if the expression is a constant expression

## consume

The CONSUME module implements the `consume` pattern, which moves ownership of containers and other moveable values while leaving the source in a default-constructed state. This enables efficient ownership transfer.


### Function annotations

- `consume` - This annotation ensures that all arguments to the function are passed as moved values.

## contracts

The CONTRACTS module provides compile-time type constraints for generic function arguments. Annotations like `[expect_any_array]`, `[expect_any_enum]`, `[expect_any_numeric]`, and `[expect_any_struct]` restrict which types can instantiate a generic parameter, producing clear error messages on mismatch.


### Function annotations

- `expect_any_variant` - [expect_any_variant(argname)] contract, which only accepts variants
- `expect_any_workhorse` - [expect_any_workhorse(argname)] contract, which only accepts workhorse types (int, float, etc) Workhorse types are: bool,int*,uint*,float*,double,range and urange, range64 and urange64, string,enumeration,and non-smart pointers
- `expect_any_function` - [expect_any_function(argname)] contract, which only accepts functions
- `expect_any_lambda` - [expect_any_lambda(argname)] contract, which only accepts lambdas
- `expect_ref` - [expect_ref(argname)] contract, which only accepts references
- `expect_any_bitfield` - [expect_any_bitfield(argname)] contract, which only accepts bitfields
- `expect_value_handle` - [expect_value_handle(argname)] contract, which only accepts value handles
- `expect_any_struct` - [expect_any_struct(argname)] contract, which only accepts structs (but not classes)
- `expect_any_enum` - [expect_any_enum(argname)] contract, which only accepts enumerations
- `expect_any_tuple` - [expect_any_tuple(argname)] contract, which only accepts tuples
- `expect_any_numeric` - [expect_any_numeric(argname)] contract, which only accepts numeric types (int, float, etc)
- `expect_any_array` - [expect_any_array(argname)] contract, which only accepts array<T>, T[], or das`vector<T>
- `expect_any_vector_type` - [expect_any_vector_type(argname)] contract, which only accepts vector types, i.e.
- `expect_pointer` - [expect_pointer(argname)] contract, which only accepts pointers
- `expect_class` - [expect_class(argname)] contract, which only accepts class instances
- `expect_any_workhorse_raw` - [expect_any_workhorse_raw(argname)] contract, which only accepts workhorse types which are raw (not pointer or bool)

### Type queries

- `isYetAnotherVectorTemplate` - returns true if the given type declaration is a das::vector template bound on C++ side

## coroutines

The COROUTINES module provides coroutine infrastructure including the `[coroutine]` function annotation, `yield_from` for delegating to sub-coroutines, and `co_await` for composing asynchronous generators. Coroutines produce values lazily via `yield` and can be iterated with `for`.


### Type aliases

- `Coroutine` - A coroutine is a generator that yields bool to indicate if it is still running.
- `Coroutines` - An array of coroutines.

### Function annotations

- `coroutine` - This macro converts coroutine function into generator, adds return false.

### Call macros

- `co_continue` - This macro converts co_continue to yield true.
- `co_await` - This macro converts `co_await(sub_coroutine)` into equivalent inline code.
- `yeild_from` - This macro converts a `yeild_from(THAT)` expression into equivalent inline code.

### Top level coroutine evaluation

- `cr_run` - This function runs coroutine until it is finished.
- `cr_run_all` - This function runs all coroutines until they are finished.

## cpp_bind

The CPP_BIND module provides utilities for generating daslang bindings to C++ code. It helps generate module registration code, type annotations, and function wrappers for exposing C++ APIs to daslang programs.


### Generation of bindings

- `log_cpp_class_adapter` - Generates C++ class adapter for the Daslang class.

## cuckoo_hash_table

The CUCKOO_HASH_TABLE module implements a cuckoo hash table data structure. Cuckoo hashing provides worst-case O(1) lookup time by using multiple hash functions and displacing existing entries on collision.


### Hash functions

- `hash0` - this hash function converts and workhorse key to a 64 bit hash
- `hash_extra` - Returns a secondary hash derived from the upper 32 bits of the primary hash, used for cuckoo hashing.

## dap

The DAP module implements the Debug Adapter Protocol (DAP) for integrating daslang with external debuggers. It provides the message types, serialization, and communication infrastructure needed for IDE debugging support.


### Structures

- `InitializeRequestArguments` - Arguments for the DAP initialize request.
- `DisconnectArguments` - Arguments for the DAP disconnect request.
- `Capabilities` - Debugger capabilities reported in the initialize response.
- `DataBreakpoint` - A data breakpoint that triggers on memory access.
- `SetDataBreakpointsArguments` - Arguments for the setDataBreakpoints request.
- `DataBreakpointInfoArguments` - Arguments for the dataBreakpointInfo request.
- `DataBreakpointInfoResponse` - Response body for the dataBreakpointInfo request.
- `SourceBreakpoint` - A breakpoint specified by source location line number.
- `Source` - A source file descriptor with name and path.
- `SetBreakpointsArguments` - Arguments for the setBreakpoints request.
- `Breakpoint` - A breakpoint with verification status and location.
- `SetBreakpointsResponse` - Response body for the setBreakpoints request.
- `Thread` - A thread with an identifier and name.
- `ThreadsResponseBody` - Response body for the threads request.
- `StackTraceArguments` - Arguments for the stackTrace request.
- `StackFrame` - A stack frame with source location and identifier.
- `StackTraceResponseBody` - Response body for the stackTrace request.
- `ScopesArguments` - Arguments for the scopes request.
- `Scope` - A named variable scope with a variables reference.
- `ScopesResponseBody` - Response body for the scopes request.
- `VariablesArguments` - Arguments for the variables request.
- `Variable` - A variable with name, value, and type information.
- `VariablesResponseBody` - Response body for the variables request.
- `OutputEventBody` - Body of the output event for debugger console messages.
- `ContinueArguments` - Arguments for the continue request.
- `PauseArguments` - Arguments for the pause request.
- `StepInArguments` - Arguments for the stepIn request.
- `NextArguments` - Arguments for the next (step over) request.
- `StepOutArguments` - Arguments for the stepOut request.
- `EvaluateArguments` - Arguments for the evaluate request.
- `EvaluateResponse` - Response body for the evaluate request.
- `BreakpointEvent` - Event body indicating a breakpoint status change.
- `ThreadEvent` - Event body indicating a thread started or exited.

### JSON serialization

- `JV` - Converts an EvaluateResponse struct to its DAP JSON representation.

### JSON field accessors

- `j_s` - Returns the string value of a JSON value, or `defVal` if not a string.
- `job` - Returns a boolean JSON field by name, or `defVal` if not found.
- `joj` - Returns a nested JSON object field by name, or null if not found.
- `jon` - Returns a numeric JSON field by name, or `defVal` if not found.
- `jos` - Returns a string JSON field by name, or `defVal` if not found.

## das_source_formatter

The DAS_SOURCE_FORMATTER module implements source code formatting for daslang. It can parse and re-emit daslang source code with consistent indentation, spacing, and line breaking rules. Used by editor integrations and code quality tools.


### Formatting

- `format_source` - Formats daslang source code given as a byte array and returns the formatted result.
- `format_source_string` - Formats a daslang source code string and returns the formatted result.

## das_source_formatter_fio

The DAS_SOURCE_FORMATTER_FIO module extends the source formatter with file I/O capabilities, enabling formatting of daslang source files on disk. It reads, formats, and writes back source files in place or to new locations.


### File formatting

- `format_file` - Reads a daslang source file, formats it, and writes the result back if changed.
- `format_files` - Formats multiple daslang source files in place.

## dashv_boost

The DASHV_BOOST module provides high-level daScript wrapper classes for the low-level `dashv` C++ bindings.


### Classes

- `HvWebSocketClient` - WebSocket client with event-driven callbacks.
- `HvWebSocketClient.init` - Connects to the given WebSocket URL.
- `HvWebSocketClient.cleanup` - Releases the underlying WebSocket client handle.
- `HvWebSocketClient.onMessageFrame` - Called for every complete text or binary frame.
- `HvWebSocketClient.is_connected` - Returns `true` if the WebSocket connection is currently open.
- `HvWebSocketClient.process_event_que` - Processes pending WebSocket events; must be called periodically.
- `HvWebSocketClient.send` - Sends a text message to the server.
- `HvWebSocketClient.send_result` - Sends a text message and returns the native transport result.
- `HvWebSocketClient.close` - Closes the WebSocket connection.
- `HvWebServer` - HTTP and WebSocket server with route registration and event callbacks.
- `HvWebServer.init` - Initializes an HTTP/WebSocket server on the specified port.
- `HvWebServer.init_wss` - Initializes an HTTPS/WSS server with TLS.
- `HvWebServer.cleanup` - Releases the underlying WebSocket server handle.
- `HvWebServer.start` - Starts the server.
- `HvWebServer.set_bind_host` - Selects the interface to bind before `start` (for example, `127.0.0.1`).
- `HvWebServer.stop` - Stops the server.
- `HvWebServer.tick` - Processes pending HTTP and WebSocket events; must be called periodically.
- `HvWebServer.onInit`
- `HvWebServer.GET` - Registers a handler for HTTP GET requests matching `uri`.
- `HvWebServer.POST` - Registers a handler for HTTP POST requests matching `uri`.
- `HvWebServer.PUT` - Registers a handler for HTTP PUT requests matching `uri`.
- `HvWebServer.DELETE` - Registers a handler for HTTP DELETE requests matching `uri`.
- `HvWebServer.PATCH` - Registers a handler for HTTP PATCH requests matching `uri`.
- `HvWebServer.HEAD` - Registers a handler for HTTP HEAD requests matching `uri`.
- `HvWebServer.ANY` - Registers a handler for any HTTP method matching `uri`.
- `HvWebServer.STATIC` - Serves static files from `dir` under the URL prefix `path`.
- `HvWebServer.allow_cors` - Enables cross-origin resource sharing (CORS) on the server.
- `HvWebServer.set_document_root` - Sets the document root directory for static file serving.
- `HvWebServer.set_home_page` - Sets the default home page file (e.g., `index.html`).
- `HvWebServer.set_index_of` - Enables directory listing for the specified directory.
- `HvWebServer.set_error_page` - Sets a custom error page file.
- `HvWebServer.SSE` - Registers an SSE (Server-Sent Events) handler for `uri`.
- `HvWebServer.STREAM` - Registers a streaming (incremental) handler for `uri` (any HTTP method, like `ANY`).

### HTTP request helpers

- `get_body_bytes` - Extracts the response body as an `array<uint8>`.
- `url_encode` - Percent-encodes a query-string component (RFC 3986: the unreserved set stays verbatim, everything else becomes `%XX`).
- `with_http_request` - Creates a temporary `HttpRequest`, invokes the block, then cleans up.

## debug_eval

The DEBUG_EVAL module provides runtime expression evaluation for debugging purposes. It can evaluate daslang expressions in the context of a running program, supporting variable inspection and interactive debugging.


### Structures

- `Result` - Result of evaluating a debug expression.

### Evaluation

- `debug_eval` - Evaluates a debug expression string with the given variable context and returns the result.

## decs

The DECS module implements a Data-oriented Entity Component System. Entities are identified by integer IDs and store components as typed data. Systems query and process entities by their component signatures, enabling cache-friendly batch processing of game objects.


### Type aliases

- `ComponentHash` - daslang Entity Component System (DECS).
- `TypeHash` - Hash value of the individual type
- `DeferEval` - Lambda which holds deferred action.
- `ComponentMap` - Table of component values for individual entity.
- `PassFunction` - One of the callbacks which form individual pass.

### Constants

- `INVALID_ENTITY_ID` - Entity ID which represents invalid entity.

### Structures

- `CTypeInfo` - Type information for the individual component subtype.
- `Component` - Single ECS component.
- `EntityId`
- `Archetype` - ECS archetype.
- `ComponentValue` - Value of the component during creation or transformation.
- `EcsRequestPos` - Location of the ECS request in the code (source file and line number).
- `EcsRequest` - Individual ECS requests.
- `DecsState` - Entire state of the ECS system.
- `DecsPass` - Individual pass of the update of the ECS system.

### Comparison and access

- `ComponentMap.` - Accesses a component value by name.
- `EntityId!=` - Inequality operator for entity IDs.
- `EntityId==` - Equality operator for entity IDs.

### Access (get/set/clone)

- `clone` - Sets individual component value.
- `find_component_index` - Returns the index of a named component within the archetype's component array.
- `get` - Creates temporary array of component given specific name and type of component.
- `get_component` - Returns a copy of the named component for the given entity.
- `has` - Returns true if object has specified subobjec.
- `remove` - Removes specified value from the component map.
- `set` - Set component value specified by name and type.
- `set_direct` - Writes a component value directly into archetype storage at the given entity index.
- `set_direct_at` - Writes a component value directly into archetype storage by pre-resolved component index.

### Entity status

- `entity_count` - Returns the total number of alive entities across all archetypes.
- `is_alive` - Returns true if the entity is alive (exists and has not been deleted).
- `long_entity_count` - int64-safe variant of `entity_count`.

### Debug and serialization

- `debug_dump` - Prints out state of the ECS system to LOG_DEBUG.
- `debug_dump_string` - Returns state of the ECS system as a string.
- `debug_dump_writer` - Writes state of the ECS system to a StringBuilderWriter.
- `describe` - Returns textual description of the type.
- `finalize` - Deletes component.
- `serialize` - Serializes component value.

### Stages

- `commit` - Finishes all deferred actions.
- `decs_stage` - Invokes specific ECS pass.
- `register_decs_stage_call` - Registration of a single pass callback.

### Deferred actions

- `create_entities` - Creates `count` entities in bulk.
- `create_entities_from_cmp` - Creates `count` entities in bulk with maximum performance.
- `create_entity` - Creates deferred action to create entity.
- `delete_entity` - Creates deferred action to delete entity specified by id.
- `update_entity` - Creates deferred action to update entity specified by id.

### GC and reset

- `after_gc` - Low level callback to be called after the garbage collection.
- `before_gc` - Low level callback to be called before the garbage collection.
- `restart` - Restarts ECS by erasing all deferred actions and entire state.

### Iteration

- `decs_array` - Low level function returns temporary array of component given specific type of component.
- `for_each_archetype` - Invokes block for each entity of each archetype that can be processed by the request.
- `for_each_archetype_find` - Invokes block for each entity of each archetype that can be processed by the request.
- `for_eid_archetype` - Invokes block for the specific entity id, given request.
- `get_default_ro` - Returns const iterator of component given specific name and type of component.
- `get_optional` - Returns const iterator of component given specific name and type of component.
- `get_ro` - Returns const temporary array of component given specific name and type of component for regular components.

### Request

- `compile_request` - Compiles ECS request, by creating request hash.
- `lookup_request` - Looks up ECS request in the request cache.
- `verify_request` - Verifies ECS request.

## decs_boost

The DECS_BOOST module provides convenience macros and syntactic sugar for the DECS entity component system, including simplified component registration, entity creation, and system definition patterns.


### Function annotations

- `REQUIRE` - This annotation provides list of required components for entity.
- `decs` - This macro converts a function into a DECS pass stage query.
- `REQUIRE_NOT` - This annotation provides list of components, which are required to not be part of the entity.

### Call macros

- `find_query` - This macro implements `find_query` functionality.
- `from_decs_template` - This macro converts a DECS query over a `[decs_template]` struct into an `iterator<tuple<...>>`.
- `query` - This macro implements `query` functionality.
- `from_decs` - This macro converts a DECS query into an `iterator<tuple<...>>`.

### Structure macros

- `decs_template` - This macro creates a template for the given structure.

## decs_state

The DECS_STATE module extends DECS with state machine support for entities. It provides state transition management, allowing entities to change behavior based on their current state.


## defer

The DEFER module implements the `defer` pattern — the ability to schedule cleanup code to run at scope exit, similar to Go's `defer`. The deferred block is moved to the `finally` section of the enclosing scope at compile time.


### Function annotations

- `DeferMacro` - This macro converts defer() <| block expression into {}, and move block to the finally section of the current block

### Call macros

- `defer_delete` - This macro converts defer_delete() expression into {}, and add delete expression to the finally section of the current block

### Defer

- `defer` - Defers a block of code until scope exit.

### Stub

- `nada` - helper function which does nothing and will be optimized out

## with_boost

The WITH_BOOST module provides the `with_` call macro: bind one or more array / table element references inside a block, with an automatic container lock around the body so push / erase / resize / clear inside the body panic at runtime instead of silently dangling. The macro emits the lock / invoke / unlock sequence fully inline, so any arity and any mix of array / table args work uniformly with a single `require`.


### Call macros

- `with_` - Implements the `with_` call macro.

## delegate

C#-like multicast delegate (multicast callback) via type macro. Generates a struct that holds a list of handlers (lambdas or function pointers) which are all invoked when the delegate is called.


### Structures

- `DelegateReturn`
- `DelegateReturn.clone`
- `DelegateReturn.`
- `DelegateReturn.each`
- `DelegateVoid`
- `DelegateVoid.clone`
- `DelegateVoid.`
- `DelegateVoid.each`

### Delegate type macro

- `delegate` - Type macro that generates a delegate struct from a function or lambda type.

## dynamic_cast_rtti

The DYNAMIC_CAST_RTTI module implements runtime dynamic casting between class types using RTTI information. It provides safe downcasting with null results on type mismatch, similar to C++ `dynamic_cast`.


### Variant macros

- `ClassAsIs` - Variant macro that implements class dynamic casting via `is` and `as`.

### Dynamic casts

- `dynamic_type_cast` - Casts a class instance to the target type using RTTI, returns null if the cast fails.
- `force_dynamic_type_cast` - Casts a class instance to the target type using RTTI, panics if the cast fails.
- `is_instance_of` - Returns true if the class instance is an instance of the specified class using RTTI.

## enum_trait

The ENUM_TRAIT module provides reflection utilities for enumerations: iterating over all values, converting between enum values and strings, and building lookup tables. The `[string_to_enum]` annotation generates a string constructor for the annotated enum type.


### Typeinfo macros

- `enum_length` - Implements typeinfo enum_length(EnumOrEnumType) which returns total number of elements in enumeration.
- `enum_names` - Implements typeinfo enum_names(EnumOrEnumType) which returns array of strings with enumValue names.

### Handled enumerations

- `string_to_enum` - Enumeration annotation which implements string constructor for enumeration.

### Enumeration iteration

- `each` - Returns an iterator over all values of the given enumeration type.

### Enumeration conversion

- `auto!` - True when the enum value's underlying integer is zero.
- `bool` - True when the enum value's underlying integer is non-zero.
- `enum_to_table` - converts enum type to a table of name => value pairs usage: let t = enum_to_table(type<EnumType>)
- `string` - converts enum value to string usage: let s = string(EnumValue)
- `to_enum` - converts string to enum value, panics if not found usage: let e = to_enum(type<EnumType>,"EnumValueName")

## faker

Random test-data generator.


### Structures

- `Faker` - Instance of the faker with all the settings inside.

### Random values

- `random_double` - Generates random double.
- `random_float` - Generates random float.
- `random_float2` - Generates random float2.
- `random_float3` - Generates random float3.
- `random_float3x3` - Generates random float3x3.
- `random_float3x4` - Generates random float3x4.
- `random_float4` - Generates random float4.
- `random_float4x4` - Generates random float4x4.
- `random_int` - Generates random integer.
- `random_int16` - Generates random int16.
- `random_int2` - Generates random int2.
- `random_int3` - Generates random int3.
- `random_int4` - Generates random int4.
- `random_int64` - Generates random int64
- `random_int8` - Generates random int8.
- `random_range` - Generates random range.
- `random_range64` - Generates random range64.
- `random_uint` - Generates random unsigned integer.
- `random_uint16` - Generates random uint16.
- `random_uint2` - Generates random uint2.
- `random_uint3` - Generates random uint3.
- `random_uint4` - Generates random uint4.
- `random_uint64` - Generates random uint64
- `random_uint8` - Generates random uint8.
- `random_urange` - Generates random urange.
- `random_urange64` - Generates random urange64.

### Random strings

- `any_char` - Generates random char.
- `any_enum` - Generates random enumeration value.
- `any_file_name` - Generates random file name.
- `any_float` - Generates random float string.
- `any_hex` - Generates random integer hex string.
- `any_int` - Generates random integer string.
- `any_set` - Generates random set (uint[8])
- `any_string` - Generates a string of random characters.
- `any_uint` - Generates random unsigned integer string.
- `long_string` - Generates a long string of random characters.
- `number` - Generates random number string.
- `positive_int` - Generates random positive integer string.

### Date and time

- `date` - Generates random date string.
- `day` - Generates random day string.
- `is_leap_year` - Returns true if year is leap year.
- `month` - Generates random month string.
- `week_day` - Returns week day for given date.

## flat_hash_table

The FLAT_HASH_TABLE module implements a flat hash table — open addressing with linear probing. `TFlatHashTable` keeps keys, hashes, and values in parallel arrays, which is cache-friendly and performs well for small to medium tables.


## functional

The FUNCTIONAL module implements lazy iterator adapters and higher-order function utilities including `filter`, `map`, `reduce`, `fold`, `scan`, `flatten`, `flat_map`, `enumerate`, `chain`, `pairwise`, `iterate`, `islice`, `cycle`, `repeat`, `sorted`, `sum`, `any`, `all`, `tap`, `for_each`, `find`, `find_index`, and `partition`.


### Transformations

- `filter` - iterates over `src` and yields only those elements for which `blk` returns true
- `flat_map` - maps each element to an iterator, then flattens the results one level
- `flatten` - iterates over `it`, then iterates over each element of each element of `it` and yields it
- `map` - iterates over `src` and yields the result of `blk` for each element
- `scan` - yields every intermediate accumulator value, starting from `seed`
- `sorted` - iterates over input and returns it sorted version

### Aggregation

- `all` - iterates over `it` and yields true if all elements are true
- `any` - iterates over `it` and yields true if any element is true
- `fold` - combines elements left-to-right starting from `seed`
- `reduce` - iterates over `it` and yields the reduced (combined) result of `blk` for each element and previous reduction result
- `reduce_or_default` - like reduce, but returns `default_value` on empty input
- `sum` - iterates over `it` and yields the sum of all elements same as reduce(it, @(a,b) => a + b)

### Search and split

- `find` - returns the first element for which `blk` returns true, or `default_value`
- `find_index` - returns the index of the first element for which `blk` returns true, or -1
- `partition` - splits elements into `(matching, non_matching)` arrays

### Iteration

- `echo` - prints `x` to the output with `extra` appended, then returns `x` unchanged.
- `enumerate` - yields tuples of `(index, element)` for each element in `src`
- `for_each` - invokes `blk` on every element of `src`
- `long_enumerate` - yields tuples of `(int64 index, element)` for each element in `src`
- `tap` - yields every element unchanged, calling `blk` on each as a side-effect

### Generators

- `chain` - yields all elements of `a`, then all elements of `b`
- `cycle` - endlessly iterates over `src`
- `islice` - iterates over `src` and yields only the elements in the range [start,stop)
- `iterate` - yields `seed`, `f(seed)`, `f(f(seed))`, ...
- `pairwise` - yields consecutive pairs: `(a,b)`, `(b,c)`, `(c,d)`, ...
- `repeat` - yields `value` `count` times.
- `repeat_ref` - yields `value` by reference `count` times

### Predicates

- `is_equal` - yields true if `a` and `b` are equal
- `is_not_equal` - yields true if `a` and `b` are not equal
- `not` - yields !x

## fuzzer

The FUZZER module implements fuzz testing infrastructure for daslang programs. It generates random inputs for functions and verifies they do not crash or produce unexpected errors, helping discover edge cases and robustness issues.


### Fuzzer tests

- `fuzz` - run block however many times ignore panic, so that we can see that runtime crashes
- `fuzz_all_ints_op1` - fuzzes generic function that takes single numeric or vector argument.
- `fuzz_all_unsigned_ints_op1` - fuzzes generic function that takes single numeric or vector argument.
- `fuzz_compareable_op2` - fuzzes generic function that takes two numeric or vector arguments.
- `fuzz_debug` - run block however many times do not ignore panic, so that we can see where the runtime fails this is here so that `fuzz` can be easily replaced with `fuzz_debug` for the purpose of debugging
- `fuzz_eq_neq_op2` - fuzzes generic function that takes two numeric or vector arguments.
- `fuzz_float_double_or_float_vec_op1` - fuzzes generic function that takes single numeric or vector argument.
- `fuzz_float_double_or_float_vec_op2` - fuzzes generic function that takes two numeric or vector arguments.
- `fuzz_float_double_or_float_vec_op3` - fuzzes generic function that takes three numeric or vector arguments.
- `fuzz_float_or_float_vec_op1` - fuzzes generic function that takes single numeric or vector argument.
- `fuzz_float_or_float_vec_op2` - fuzzes generic function that takes two numeric or vector arguments.
- `fuzz_int_vector_op2` - fuzzes generic function that takes two numeric or vector arguments.
- `fuzz_numeric_and_storage_op1` - fuzzes generic function that takes single numeric or vector argument.
- `fuzz_numeric_and_vector_op1` - fuzzes generic function that takes single numeric or vector argument.
- `fuzz_numeric_and_vector_op2` - fuzzes generic function that takes two numeric or vector arguments.
- `fuzz_numeric_and_vector_op2_no_unint_vec` - fuzzes generic function that takes two numeric or vector arguments.
- `fuzz_numeric_and_vector_signed_op1` - fuzzes generic function that takes single numeric or vector argument.
- `fuzz_numeric_op1` - fuzzes generic function that takes single numeric or vector argument.
- `fuzz_numeric_op2` - fuzzes generic function that takes two numeric or vector arguments.
- `fuzz_numeric_op3` - fuzzes generic function that takes three numeric or vector arguments.
- `fuzz_numeric_op4` - fuzzes generic function that takes four numeric or vector arguments.
- `fuzz_numeric_scal_vec_op2` - fuzzes generic function that takes vector and matching scalar on the left arguments pairs are: int2,int; int3,int; uint2,uint; uint3,uint; uint4,uint; int4,int; float2,float; float3,float; float4,float
- `fuzz_numeric_vec_scal_op2` - fuzzes generic function that takes vector and matching scalar on the right arguments pairs are: int2,int; int3,int; uint2,uint; uint3,uint; uint4,uint; int4,int; float2,float; float3,float; float4,float
- `fuzz_rotate_op2` - fuzzes generic function that takes numeric or vector argument, with matching rotate type on the right.
- `fuzz_shift_op2` - fuzzes generic function that takes numeric or vector argument, with matching shift type on the right.
- `fuzz_vec_mad_op3` - fuzzes generic function that takes three numeric or vector arguments.
- `fuzz_vec_op3` - fuzzes generic function that takes three numeric or vector arguments.

## generic_return

The GENERIC_RETURN module provides the `[generic_return]` annotation that allows generic functions to automatically deduce their return type from the body. This simplifies writing generic utility functions by eliminating explicit return type specifications.


### Call macros

- `generic_return` - Replaces generic_return(expr) with a block that calls expr and returns its result, handling void, copyable, and movable return types.

## if_not_null

The IF_NOT_NULL module provides a null-safe call macro. The expression `ptr |> if_not_null <| call(args)` expands to a null check followed by a dereferenced call: `if (ptr != null) { call(*ptr, args) }`.


### Call macros

- `if_not_null` - This macro transforms guarded pointer access into null-safe inline code.

## instance_function

The INSTANCE_FUNCTION module provides the `[instance_function]` annotation for creating bound method-like functions. It captures the `self` reference at call time, enabling object-oriented dispatch patterns in daslang.


### Function annotations

- `instance_function` - [instance_function(generic_name,type1=type1r,type2=type2r,...)] macro creates instance of the generic function with a particular set of types.

## interfaces

The INTERFACES module implements interface-based polymorphism for daslang. It provides the `[interface]` annotation for defining abstract interfaces with virtual method tables, supporting multiple implementations and dynamic dispatch without class inheritance.


### Variant macros

- `InterfaceAsIs` - Variant macro that enables `is`, `as`, and `?as` operators for interface types declared with `[interface]` / `[implements]`.

### Structure macros

- `interface` - Verifies that the annotated class is a valid interface — it may only contain function-typed fields (no data members).
- `implements` - Generates interface bindings for a struct.

## is_local

The IS_LOCAL module provides compile-time checks for whether a variable is locally allocated (on the stack) versus heap-allocated. This enables writing generic code that optimizes differently based on allocation strategy.


### Scope checks

- `is_local_expr` - Returns true if the expression is local to the current scope.
- `is_local_or_global_expr` - Returns true if expression is local to the current scope or global scope.
- `is_scope_expr` - Returns true if the expression is a scoped expression, i.e.
- `is_shared_expr` - Returns true if the expression refers to a global shared variable.
- `is_temp_safe` - Returns true if the expression had no calls, [] or table [] operators of any kind.

## jobque_boost

The JOBQUE_BOOST module provides high-level job queue abstractions built on the low-level `jobque` primitives: `new_job` / `new_thread` (which capture a lambda and clone the context for the worker), `with_wait_group` / `done`, `parallel_for` and the `team_parallel_*` family, and typed `push` / `pop` / `gather` over a `Stream`. It requires `jobque` publicly, so the builtins — `with_job_que`, `with_job_status`, `Channel`, `LockBox` — are visible through it as well.


### Function annotations

- `ParallelForJobMacro` - Base macro for parallel_for, parallel_for_each, and parallel_map.
- `TeamParallelStagesMacro` - Rewrites `team_parallel_stages` into `team_parallel_stages_invoke`, generating the same lambda cloning infrastructure as `team_parallel_for` (per-worker cloning on the C++ side).
- `TeamParallelForMacro` - Rewrites `team_parallel_for` into `team_parallel_for_invoke`, generating the same lambda cloning infrastructure as `new_job` (the per-worker cloning itself happens on the C++ side).
- `ParallelMapJobMacro` - This macro handles `parallel_map`.
- `TeamParallelForIndexedMacro` - Same lowering as `TeamParallelForMacro`; the C++ entry point invokes the lambda with the extra leading worker-slot argument.
- `ParallelForEachJobMacro` - This macro handles `parallel_for_each`.
- `NewJobMacro` - this macro handles `new_job` and `new_thread` calls.

### Invocations

- `new_job` - Create a new job.
- `new_thread` - Creates a new thread with a cloned context and invokes the cloned lambda on that thread.

### Iteration

- `each` - this iterator is used to iterate over the channel in order it was pushed.
- `each_clone` - this iterator is used to iterate over the channel in order it was pushed.
- `for_each` - reads input from the channel (in order it was pushed) and invokes the block on each input.
- `for_each_clone` - reads input from the channel (in order it was pushed) and invokes the block on each input.

### Passing data

- `push` - pushes value to the channel (at the end)
- `push_archive` - Serialize `value` via `daslib/archive` and push the resulting bytes onto the stream.
- `push_batch` - pushes values to the channel (at the end)
- `push_batch_clone` - clones data and pushes values to the channel (at the end)
- `push_clone` - clones data and pushes value to the channel (at the end)

### Receiving data

- `gather` - reads input from the channel (in order it was pushed) and invokes the block on each input.
- `gather_and_forward` - reads input from the channel (in order it was pushed) and invokes the block on each input.
- `gather_archive` - Drain the stream, deserializing each blob via `daslib/archive` and invoking the block on each.
- `gather_ex` - reads input from the channel (in order it was pushed) and invokes the block on each input.
- `peek` - reads input from the channel (in order it was pushed) and invokes the block on each input.
- `pop` - Blocking pop from the stream.
- `pop_and_clone_one` - reads one command from channel
- `pop_archive` - Blocking pop + deserialize via `daslib/archive`.
- `pop_one` - reads one command from channel
- `pop_with_timeout` - Pop from channel with timeout in milliseconds.
- `pop_with_timeout_clone` - Pop from channel with timeout and clone.
- `try_pop` - Non-blocking pop from channel.
- `try_pop_archive` - Non-blocking pop + deserialize.
- `try_pop_clone` - Non-blocking pop with clone from channel.

### Synchronization

- `done` - Mark one unit of work as done in a wait group.
- `with_wait_group` - Creates a wait group starting at count 0 with auto-join.

### Parallel execution

- `_parallel_for` - Partitions `[range_begin..range_end)` into `num_jobs` chunks and invokes `blk` once per chunk on the calling thread with `(chunk_begin, chunk_end, wg)`.
- `_parallel_for_each` - Runtime implementation for `parallel_for_each`.
- `_parallel_map` - Runtime implementation for `parallel_map`.
- `parallel_for` - this one is stub for _parallel_for
- `parallel_for_each` - Convenience wrapper around `parallel_for` for arrays.
- `parallel_map` - Partitions array indices `[0..length(arr))` into `num_jobs` chunks.
- `team_parallel_for` - Team-mode `parallel_for` (see `set_jobque_team_mode`): partitions `[range_begin..range_end)` into `num_jobs` chunks with the same split as `parallel_for`, but instead of one fifo job per chunk the lambda is cloned once per WORKER and the workers self-serve chunk indices off a single atomic — no fifo, no wait group; the caller participates and spin-joins.
- `team_parallel_for_indexed` - `team_parallel_for` with the claiming worker's slot exposed: the lambda receives `(slot, job_begin, job_end)`.
- `team_parallel_stages` - Multi-stage team dispatch: one team rendezvous runs the `stages` in order, with a worker-side barrier between consecutive stages — stage `s+1` may read anything stage `s` wrote.

### LockBox operations

- `clear` - clear value from the lock box
- `fill` - stores pointer to data in the lock box and marks it as full (isReady becomes false).
- `get` - reads value from the lock box and invokes the block on it
- `grab` - grabs data from lock box, marks it as empty (isReady becomes true), invokes block.
- `set` - clones data and sets value to the lock box
- `update` - update value in the lock box and invokes the block on it

### SeqBox operations

- `publish` - Publishes a snapshot of `value`, replacing whatever was there; false when another writer holds the box, or `TT` (raw POD — no array/table/string/pointer) exceeds `SEQ_BOX_PAYLOAD` bytes.
- `read` - Copies the latest snapshot out and invokes the block on the copy; consumes nothing and blocks no one; false when nothing was published or the size differs — size is the box's only identity, so an equal-size type reads back reinterpreted.

### Internal capture details

- `capture_jobque_channel` - this function is used to capture a channel that is used by the jobque.
- `capture_jobque_job_status` - this function is used to capture a job status that is used by the jobque.
- `capture_jobque_lock_box` - this function is used to capture a lock box that is used by the jobque.
- `capture_jobque_seq_box` - this function is used to capture a seq box that is used by the jobque.
- `capture_jobque_stream` - this function is used to capture a stream that is used by the jobque.
- `release_capture_jobque_channel` - this function is used to release a channel that is used by the jobque.
- `release_capture_jobque_job_status` - this function is used to release a job status that is used by the jobque.
- `release_capture_jobque_lock_box` - this function is used to release a lock box that is used by the jobque.
- `release_capture_jobque_seq_box` - this function is used to release a seq box that is used by the jobque.
- `release_capture_jobque_stream` - this function is used to release a stream that is used by the jobque.

## jobque_profile

The JOBQUE_PROFILE module wraps the low-level `jobque_trace_*` builtins into a self-describing profiling API: named **categories** with colors for the op tags stamped via `profile_tag`, and **markers** — instant "unit" events such as `token` or `frame` — so saved traces carry their own legend and are navigable unit-to-unit. Files remain perfetto-compatible; the category and marker tables ride in a `jobqueProfile` sibling key that Perfetto ignores.


### Constants

- `JOBQUE_PROFILING_ENABLED`
- `PROFILE_COLOR_BLUE`
- `PROFILE_COLOR_GREEN`
- `PROFILE_COLOR_PURPLE`
- `PROFILE_COLOR_AMBER`
- `PROFILE_COLOR_CORAL`
- `PROFILE_COLOR_TEAL`
- `PROFILE_COLOR_MAGENTA`
- `PROFILE_COLOR_INDIGO`

### Trace sessions

- `profile_save` - Write the recorded trace as perfetto-compatible JSON, including the category and marker registries.
- `profile_start` - Arm the per-lane trace rings (`events_per_lane` events each; full lanes stop recording).
- `profile_stop` - Disarm tracing.

### Categories

- `profile_category` - Register a named category for an existing tag id (`color` is 0xRRGGBB; 0 picks from the default palette by name hash).
- `profile_tag` - Stamp the current op tag; subsequent chain publishes carry it (viewer color channel).

### Markers

- `profile_marker` - Stamp an instant unit boundary on the caller lane (no-op when tracing is off).
- `profile_marker_id` - Register/look up a marker kind (e.g.

## json_boost

The JSON_BOOST module extends JSON support with operator overloads for convenient field access (`?.` / `?[]`), null-coalescing (`??`), and generic conversions in both directions: `JV(value)` builds a `JsonValue?` tree from a struct, tuple, array, table, or vector, and `from_JV(js, default)` reads one back into a typed value. Field annotations on the struct steer both directions (see below), and the builtin `sprint_json` honours the same annotations.


### Structures

- `JsonFieldState` - Per-field serialization options for JSON struct conversion.

### Reader macros

- `json` - This macro embeds a JSON object into the program.

### Variant macros

- `better_json` - This macro is used to implement `is json_value` and `as json_value` runtime checks.

### Value conversion

- `JV` - Creates `JsonValue` out of value.
- `from_JV` - Parse a JSON value and return the corresponding value of any type.

### LINQ source builders

- `from_json` - Lazily walks the elements of a JSON array *jv*, reading each element's top-level fields **by name** into a *TT* — the JSON counterpart of `from_xml_node`, for `for` loops and LINQ chains:
- `from_json_row` - Materialize one JSON object into *TT* **by name** — the per-element worker behind `from_json`.
- `read_json_field` - Reads one same-named key of the JSON object *jv* into a *TT* — the field-pruned counterpart of a full by-name materialize, emitted by linq_fold's fused JSON lane.

### Element access operators

- `JsonValue const? ==const?.` - Returns the value of the key in the JSON object, if it exists.
- `JsonValue const? ==const?[]` - Returns the value of the index in the JSON array, if it exists.
- `JsonValue? ==const?.` - Returns the value of the key in the JSON object, if it exists.
- `JsonValue? ==const?[]` - Returns the value of the index in the JSON array, if it exists.
- `key_exists` - True when `a` is a JSON object carrying `key`; false for null, a non-object, or a missing key - the `?[]` read alone cannot tell a missing key from a null value.

### Null coalescing operators

- `JsonValue const???` - Returns the value of the JSON object, if it exists, otherwise returns the default value.

### Value extraction

- `JsonValue const? ==const?.value` - Returns the value of the JSON object, if it exists.
- `JsonValue? ==const?.value` - Returns the value of the JSON object, if it exists.

### Annotation parsing

- `parse_json_annotation` - Parse JSON field annotations and return the corresponding JsonFieldState.

## json

The JSON module implements JSON parsing and serialization. It provides `read_json` for parsing JSON text into a `JsonValue` tree, `write_json` for serializing back to text, and `JV` helpers for constructing JSON values from daslang types. `write_json` pretty-prints (one element per line, tab-indented); `write_json_compact` emits the same tree on a single line.


### Type aliases

- `JsValue` - Single JSON element.
- `Token` - JSON input stream token.

### Structures

- `JsonValue` - JSON value, wraps any JSON element.
- `TokenAt` - JSON parsing token.

### Value conversion

- `JV` - Creates `JsonValue` out of bitfield value.
- `JVNull` - Creates `JsonValue` representing `null`.

### Read and write

- `read_json` - reads JSON from the `text` array of uint8.
- `write_json` - Overload accepting temporary type
- `write_json_compact` - `write_json` on one line: no newlines or indentation, `", "` / `": "` separators — the python `json.dumps` / jinja `tojson` shape chat templates and wire protocols carry.

### Memory management

- `delete_json` - Free a JsonValue tree.
- `json_null_sentinel` - Shared, per-context `null` JsonValue returned by `operator ?[]` / `?.` when a key/index is absent — so missing-key navigation allocates nothing (a fresh `JVNull()` per miss was a steady GC-pressure source).
- `update` - Set object field `key` to `jsv`, first freeing (via `delete_json`) whatever JsonValue was stored at `key`.

### JSON properties

- `set_allow_duplicate_keys` - if `value` is true, then duplicate keys are allowed in objects.
- `set_no_empty_arrays` - if `value` is true, then empty arrays are not written at all
- `set_no_trailing_zeros` - if `value` is true, then numbers are written without trailing zeros.

### Broken JSON

- `try_fixing_broken_json` - fixes broken json.

## jsonrpc

The JSON-RPC module is a transport-agnostic JSON-RPC 2.0 implementation (https://www.jsonrpc.org/specification). It provides envelope builders, request/response parsers, optional §6 batch handling, and a high-level `dispatch_line` convenience for stdio-style servers.


### Constants

- `PARSE_ERROR` - < Invalid JSON received.
- `INVALID_REQUEST` - < Not a valid request object.
- `METHOD_NOT_FOUND` - < Method does not exist or is not available.
- `INVALID_PARAMS` - < Invalid method params.
- `INTERNAL_ERROR` - < Internal JSON-RPC error.

### Structures

- `ParsedRequest`
- `ParsedBatch`
- `ParsedResponse`
- `ParsedResponseBatch`

### Envelope builders

- `error` - Wrap `code` + `message` in a JSON-RPC 2.0 error response envelope.
- `error_with_data` - Like `error`, but also includes the optional `data` field (spec §5.1).
- `quoted` - `s` as a JSON string literal, quotes and escapes included.
- `response` - Wrap `result_json` in a JSON-RPC 2.0 success response envelope.
- `serialize_id` - Serialize a JSON-RPC id (string/number/null) to its wire form.

### Server-side parsers

- `parse_batch` - Parse a JSON-RPC 2.0 wire body that may be a single request or a §6 batch array.
- `parse_request` - Parse a single JSON-RPC 2.0 request line.

### Request scope

- `free_batch` - End a `parse_batch` scope: free the owned document and release the entry array.
- `free_request` - End a `parse_request` scope: free the owned document and clear the `params` view.

### Outgoing request builders

- `make_batch` - Wrap pre-built request/notification strings as a JSON-RPC 2.0 §6 batch array.
- `make_notification` - Build a JSON-RPC 2.0 notification (no id — server MUST NOT respond).
- `make_request` - Build a JSON-RPC 2.0 request with an integer id.

### Response parsers

- `parse_response` - Parse a single JSON-RPC 2.0 response line.
- `parse_response_batch` - Parse a JSON-RPC 2.0 response wire body that may be a single response or a §6 batch array.

### Framing helper

- `compact_json_whitespace` - Strip JSON pretty-printer whitespace (spaces, tabs, newlines, carriage returns) that appears outside string literals.

### High-level dispatch

- `dispatch_line` - Dispatch a JSON-RPC 2.0 wire line (single or §6 batch) through `dispatcher`, returning the response string ready for the wire (or `""` for all-notifications).

## linq

The LINQ module provides query-style operations on sequences: filtering (`where_`), projection (`select`), sorting (`order`, `order_by`), deduplication (`distinct`), pagination (`skip`, `take`), aggregation (`sum`, `average`, `aggregate`), and element access (`first`, `last`).


### Sorting data

- `order` - Sorts an array
- `order_by` - Sorts an array
- `order_by_descending` - Sorts an array in descending order
- `order_by_descending_inplace` - Sorts an array in descending order in place
- `order_by_descending_to_array` - Sorts an iterator in descending order and returns an array
- `order_by_inplace` - Sorts an array in place
- `order_by_keys` - Sorts an array by a composite (tuple) key with per-key direction (`mask`).
- `order_by_keys_inplace` - Sorts an array in place by a composite (tuple) key; bit `i` of `mask` → key `i` descending.
- `order_by_keys_to_array` - Sorts an iterator by a composite (tuple) key with per-key direction (`mask`); returns an array.
- `order_by_to_array` - Sorts an iterator and returns an array
- `order_descending` - Sorts an array in descending order
- `order_descending_inplace` - Sorts an array in descending order in place
- `order_descending_to_array` - Sorts an iterator in descending order and returns an array
- `order_inplace` - Sorts an array in place
- `order_to_array` - Sorts an iterator and returns an array
- `order_unique_folded` - sort and remove duplicate elements from an array
- `order_unique_folded_inplace` - sort and remove duplicate elements from an array in place
- `reverse` - Reverses an array
- `reverse_inplace` - Reverses an array in place
- `reverse_to_array` - Reverses an iterator and returns an array

### Top-N selection

- `spliced_pop_heap` - Thin re-export of sort_boost::pop_heap for the bounded-heap splice (see spliced_push_heap).
- `spliced_push_heap` - Thin re-export of sort_boost::push_heap so plan_decs_order_family's bounded-heap splice can call it via _:: from any user module without requiring sort_boost directly.
- `top_n` - Returns the `n` smallest elements of `arr` (by `<`), sorted ascending.
- `top_n_by` - Returns the `n` smallest elements of `arr` by `key(element)`, sorted ascending.
- `top_n_by_descending` - Returns the `n` largest elements of `arr` by `key(element)`, sorted descending.
- `top_n_by_with_cmp` - Returns the `n` smallest-per-`cmp` elements of `arr`.
- `top_n_descending` - Returns the `n` largest elements of `arr` (by `<`), sorted descending.

### Set operations

- `distinct` - Returns distinct elements from an array
- `distinct_by` - Returns distinct elements from an array based on a key
- `distinct_by_inplace` - Returns distinct elements from an array based on a key in place
- `distinct_by_to_array` - Returns distinct elements from an iterator based on a key and returns an array
- `distinct_inplace` - Returns distinct elements from an array in place
- `distinct_to_array` - Returns distinct elements from an iterator and returns an array
- `except` - Returns elements from the array that are not in the iterator
- `except_by` - Returns elements from the array that are not in the iterator by key
- `except_by_to_array` - Elements from an array (left) not present in an iterator (right) by key, returning an array
- `except_to_array` - Elements from an array (left) not present in an iterator (right), returning an array
- `intersect` - Returns elements that are present in both an array and an iterator
- `intersect_by` - Returns elements present in both an array and an iterator by key
- `intersect_by_to_array` - Elements present in both an array and an iterator by key, returning an array
- `intersect_to_array` - Elements present in both an array and an iterator, returning an array
- `union` - Returns distinct elements from the concatenation of an array and an iterator
- `union_by` - Returns distinct elements from the concatenation of an array and an iterator by key
- `union_by_to_array` - Distinct elements from the concatenation of an array and an iterator by key, returning an array
- `union_to_array` - Distinct elements from the concatenation of an array and an iterator, returning an array
- `unique` - remove adjacent duplicate elements from an array (sort first to make it global)
- `unique_by` - remove adjacent duplicate elements from an array based on a key (sort first to make it global)
- `unique_by_inplace` - remove duplicate elements from a sorted array based on a key in place
- `unique_by_to_array` - remove adjacent duplicate elements from an iterator based on a key and return an array (sort first to make it global)
- `unique_inplace` - remove duplicate elements from sorted array in place
- `unique_key` - generates unique key of workhorse type for the value
- `unique_to_array` - remove adjacent duplicate elements from an iterator and return an array (sort first to make it global)

### Concatenation operations

- `append` - Appends a value to the end of an array
- `append_inplace` - Appends a value to the end of an array in place
- `append_to_array` - Appends a value to the end of an iterator and returns an array
- `concat` - Concatenates an array and an iterator.
- `concat_inplace` - Concatenates two arrays in place
- `concat_to_array` - Concatenates an array and an iterator, returning an array
- `prepend` - Prepends a value to the beginning of an array
- `prepend_inplace` - Prepends a value to the beginning of an array in place
- `prepend_to_array` - Prepends a value to the beginning of an iterator and returns an array

### Generation operations

- `default_empty` - Returns the elements of the iterator, or a default value if the iterator is empty
- `empty` - Returns an empty iterator of the specified type
- `range_sequence` - Generates a sequence of integers within a specified range
- `repeat` - Generates a sequence that contains one repeated value

### Aggregation operations

- `aggregate` - Aggregates elements in an array using a seed and a function
- `average` - Averages elements in an array.
- `count` - Counts elements in an array
- `long_count` - Counts elements in an array, using a long integer
- `max` - Finds the maximum element in an array
- `max_by` - Finds the maximum element in an array by key
- `min` - Finds the minimum element in an array
- `min_by` - Finds the minimum element in an array by key
- `min_max` - Finds the minimum and maximum elements in an array
- `min_max_average` - Finds the minimum, maximum, and average elements in an array
- `min_max_average_by` - Finds the minimum, maximum, and average elements in an array by key
- `min_max_by` - Finds the minimum and maximum elements in an array by key
- `sum` - Sums elements in an array

### Filtering data

- `where_` - Filters elements in an array based on a predicate
- `where_to_array` - Filters elements in an iterator based on a predicate and returns an array

### Partitioning data

- `chunk` - Splits an array into chunks of a specified size
- `chunk_to_array` - Splits an iterator into chunks of a specified size and returns an array
- `skip` - Yields all but the first `total` elements
- `skip_inplace` - Removes the first `total` elements from an array in place
- `skip_last` - Yields all but the last `total` elements from an array
- `skip_last_inplace` - Removes the last `total` elements from an array in place
- `skip_last_to_array` - Yields all but the last `total` elements from an iterator and returns an array
- `skip_to_array` - Yields all but the first `total` elements and returns an array
- `skip_while` - Skips all elements of an array while the predicate is true
- `skip_while_to_array` - Skips all elements of an iterator while the predicate is true and returns an array
- `take` - Yields only the first `total` elements
- `take_inplace` - Keeps only a range of elements in an array in place
- `take_last` - Yields only the last `total` elements from an array
- `take_last_inplace` - Keeps only the last `total` elements in an array in place
- `take_last_to_array` - Yields only the last `total` elements from an iterator and returns an array
- `take_to_array` - Yields a range of elements from an iterator and returns an array
- `take_while` - Yields only the elements of an array while the predicate is true
- `take_while_to_array` - Yields only the elements of an iterator while the predicate is true and returns an array

### Joining

- `cross_join` - Cross join — every (TA, TB) pair
- `cross_join_to_array` - Cross join of an array (left) and an iterator (right), returning an array.
- `full_outer_join` - Full outer join — both sides surface; unmatched rows pair with `none` on the other side
- `full_outer_join_to_array` - Full outer join of an array (left) and an iterator (right), returning an array
- `group_join` - C# GroupJoin (outer): `result` receives every TA paired with the array of matching TB — empty when none match.
- `group_join_to_array` - Group join of an array (left) and an iterator (right), returning an array
- `join` - Joins two arrays based on matching keys (inner join)
- `join_to_array` - Inner join of an array (left) and an iterator (right), returning an array
- `left_join` - Left outer join — every TA emits at least one row; right side is Option<TB> (none when no match)
- `left_join_to_array` - Left outer join of an array (left) and an iterator (right), returning an array
- `right_join` - Right outer join — every TB emits at least one row; left side is Option<TA> (none when no left match)
- `right_join_to_array` - Right outer join of an array (left) and an iterator (right), returning an array

### Grouping

- `group_by` - Groups the elements of an array according to a specified key selector function
- `group_by_lazy` - IGrouping shape over an array — yields `(key, array<TT>)` per group.
- `group_by_lazy_to_array` - IGrouping shape returning an array — see `group_by_lazy`.
- `group_by_to_array` - Groups the elements of an iterator according to a specified key selector function and returns an array
- `having_` - Post-aggregate filter on an array — semantics match `where_`; preserved as a distinct call so SQL translation can route to `HAVING`.
- `having_to_array` - Post-aggregate filter returning an array — see `having_`.

### Querying data

- `all` - Returns true if all elements in the array satisfy the predicate
- `any` - Returns true if the array has at least one element
- `contains` - Returns true if the element is present in the array
- `none` - Returns true if the array has no elements

### Element operations

- `element_at` - Returns the element at the specified index
- `element_at_or_default` - Returns the element at the specified index, or a default value if the index is out of range
- `first` - Returns the first element of an array
- `first_or_default` - Returns the first element of an array, or a default value if the array is empty
- `last` - Returns the last element of an array
- `last_or_default` - Returns the last element of an array, or a default value if the array is empty
- `single` - Returns the only element of an array, and throws if there is not exactly one element
- `single_or_default` - Returns the only element of an array, or a default value if there is not exactly one element

### Transform operations

- `select` - Projects each element of an array into a new form
- `select_many` - Projects each element of an array to an iterator and flattens the resulting iterators into one array
- `select_many_pair` - SelectMany whose result selector receives both the outer element and each flattened inner element — C# `from o … from x in o.coll select f(o, x)`.
- `select_many_pair_to_array` - `select_many_pair` returning an array (the iterator source is materialized).
- `select_many_to_array` - Projects each element of an iterator to an iterator and flattens the resulting iterators into one array
- `select_to_array` - Projects each element of an iterator into a new form and returns an array
- `zip` - Merges two arrays into an array of tuples
- `zip_to_array` - Merges an array and an iterator into an array of tuples.

### Conversion operations

- `to_sequence` - Converts an array to an iterator
- `to_sequence_move` - Converts an array to an iterator, captures input.
- `to_table` - Converts an array to a table

### Comparators and keys

- `less` - Compares two values, returns true if first is less than second
- `sequence_equal` - Checks if an array and an iterator hold equal elements in the same order
- `sequence_equal_by` - Checks if an array and an iterator are equal by key

## linq_boost

The LINQ_BOOST module extends LINQ with pipe-friendly macros using underscore syntax for inline predicates and selectors. Expressions like `arr |> _where(_ > 3) |> _select(_ * 2)` provide concise functional pipelines.


### Call macros

- `_distinct_by` - implements _distinct_by(iterator, expression) shorthand notation that expands into distinct_by(iterator, $(_) => expression).
- `_none` - implements _none(iterator, expression) shorthand notation that expands into none(iterator, $(_) => expression).
- `_right_join` - implements _right_join(srca, srcb, on, result) shorthand notation that expands into right_join(srca, srcb, keya, keyb, result).
- `_select_many_2` - Stage 2 of `_select_many` (see above).
- `_union_by_to_array` - implements _union_by_to_array(iterator1, iterator2, expression) shorthand notation that expands into union_by_to_array(iterator1, iterator2, $(_) => expression).
- `_min_max_average_by` - implements _min_max_average_by(iterator, expression) shorthand notation that expands into min_max_average_by(iterator, $(_) => expression).
- `_order_by_descending_to_array` - implements _order_by_descending_to_array(iterator, expression) shorthand notation that expands into order_by_descending_to_array(iterator, $(_) => expression).
- `_union_by` - implements _union_by(iterator1, iterator2, expression) shorthand notation that expands into union_by(iterator1, iterator2, $(_) => expression).
- `_group_join` - implements _group_join(srca, srcb, on, result) shorthand — C# GroupJoin (outer).
- `_left_join` - implements _left_join(srca, srcb, on, result) shorthand notation that expands into left_join(srca, srcb, keya, keyb, result).
- `_except_by` - implements _except_by(iterator1, iterator2, expression) shorthand notation that expands into except_by(iterator1, iterator2, $(_) => expression).
- `_group_by_lazy_to_array` - implements _group_by_lazy_to_array(iterator, key) shorthand notation that expands into group_by_lazy_to_array(iterator, $(_) => key).
- `_order_by` - implements _order_by(iterator, expression) shorthand notation that expands into order_by(iterator, $(_) => expression).
- `_select_to_array` - implements _select_to_array(iterator, expression) shorthand notation that expands into select_to_array(iterator, $(_) => expression).
- `_where` - implements _where(iterator, expression) shorthand notation that expands into where_(iterator, $(_) => expression).
- `_distinct_by_to_array` - implements _distinct_by_to_array(iterator, expression) shorthand notation that expands into distinct_by_to_array(iterator, $(_) => expression).
- `_group_by_to_array` - implements _group_by_to_array(iterator, key) shorthand notation that expands into group_by_lazy_to_array(iterator, $(_) => key).
- `_cross_join` - implements _cross_join(srca, srcb, result) shorthand notation that expands into cross_join(srca, srcb, result).
- `_take_while` - implements _take_while(iterator, expression) shorthand notation that expands into take_while(iterator, $(_) => expression).
- `_max_by` - implements _max_by(iterator, expression) shorthand notation that expands into max_by(iterator, $(_) => expression).
- `_skip_while` - implements _skip_while(iterator, expression) shorthand notation that expands into skip_while(iterator, $(_) => expression).
- `_order_by_to_array` - implements _order_by_to_array(iterator, expression) shorthand notation that expands into order_by_to_array(iterator, $(_) => expression).
- `_unique_by_to_array` - implements _unique_by_to_array(iterator, expression) shorthand notation that expands into unique_by_to_array(iterator, $(_) => expression).
- `_intersect_by` - implements _intersect_by(iterator1, iterator2, expression) shorthand notation that expands into intersect_by(iterator1, iterator2, $(_) => expression).
- `_select_many` - Stage 1 of the correlated SelectMany sugar — `_select_many(src, $(o) => coll, $(o, x) => result)`.
- `_select` - implements _select(iterator, expression) shorthand notation that expands into select(iterator, $(_) => expression).
- `_sequence_equal_by` - implements _sequence_equal_by(iterator1, iterator2, expression) shorthand notation that expands into sequence_equal_by(iterator1, iterator2, $(_) => expression).
- `_not_in` - implements _not_in(element, subquery) shorthand notation that expands into !contains(subquery, element).
- `_having` - implements _having(iterator, expression) shorthand notation that expands into having_(iterator, $(_) => expression).
- `_all` - implements _all(iterator, expression) shorthand notation that expands into all(iterator, $(_) => expression).
- `_min_max_by` - implements _min_max_by(iterator, expression) shorthand notation that expands into min_max_by(iterator, $(_) => expression).
- `_full_outer_join` - implements _full_outer_join(srca, srcb, on, result) shorthand notation that expands into full_outer_join(srca, srcb, keya, keyb, result).
- `_long_count` - implements _long_count(iterator, expression) shorthand notation that expands into long_count(iterator, $(_) => expression).
- `_min_by` - implements _min_by(iterator, expression) shorthand notation that expands into min_by(iterator, $(_) => expression).
- `_order_by_keys` - implements _order_by_keys(iterator, $(x) => (k1, k2, …), descMask) — multi-key order with per-key direction.
- `_any` - implements _any(iterator, expression) shorthand notation that expands into any(iterator, $(_) => expression).
- `_in` - implements _in(element, subquery) shorthand notation that expands into contains(subquery, element).
- `_where_to_array` - implements _where_to_array(iterator, expression) shorthand notation that expands into where_to_array(iterator, $(_) => expression).
- `_having_to_array` - implements _having_to_array(iterator, expression) shorthand notation that expands into having_to_array(iterator, $(_) => expression).
- `_count` - implements _count(iterator, expression) shorthand notation that expands into count(iterator, $(_) => expression).
- `_join` - implements _join(srca, srcb, on, result) shorthand notation that expands into join(srca, srcb, keya, keyb, result).
- `_order_by_keys_to_array` - implements _order_by_keys_to_array(iterator, $(x) => (k1, k2, …), descMask) shorthand notation that expands into order_by_keys_to_array(iterator, key-lambda, descMask).
- `_group_by` - implements _group_by(iterator, key) shorthand notation that expands into group_by_lazy(iterator, $(_) => key) — the same macro as `_group_by_lazy` under two names
- `_except_by_to_array` - implements _except_by_to_array(iterator1, iterator2, expression) shorthand notation that expands into except_by_to_array(iterator1, iterator2, $(_) => expression).
- `_unique_by` - implements _unique_by(iterator, expression) shorthand notation that expands into unique_by(iterator, $(_) => expression).
- `_group_by_lazy` - implements _group_by_lazy(iterator, key) shorthand notation that expands into group_by_lazy(iterator, $(_) => key).
- `_order_by_descending` - implements _order_by_descending(iterator, expression) shorthand notation that expands into order_by_descending(iterator, $(_) => expression).
- `_intersect_by_to_array` - implements _intersect_by_to_array(iterator1, iterator2, expression) shorthand notation that expands into intersect_by_to_array(iterator1, iterator2, $(_) => expression).

## linq_fold

The LINQ_FOLD module provides the `_fold` and `_old_fold` call macros that rewrite LINQ pipelines into optimized loop forms — `_fold` is the active fusion macro, `_old_fold` is a frozen pre-rewrite baseline kept for benchmark comparison as `_fold` evolves toward splice-mode fusion.


### Call macros

- `_fold`

## lint

The LINT module implements static analysis checks for daslang code. It provides customizable lint rules that detect common mistakes, style violations, and potential bugs at compile time.


### Lint operations

- `paranoid` - Runs the paranoid lint visitor on the program to check for common coding issues.
- `paranoid_collect` - Runs the paranoid lint visitor and collects errors as strings.
- `paranoid_collect_issues` - Structured twin of `paranoid_collect` — same filters, but emits `LintIssue` records (rule code + position) instead of display strings.
- `stale_nolint_collect`

## lint_config

The lint_config module loads `{get_das_root()}/.lint_config` (a TOML file with a `[rules]` table of booleans) and folds it into a `disabled_codes` set consumed by the three lint pass-macros (`daslib/lint`, `daslib/perf_lint`, `daslib/style_lint`), by the standalone runner `utils/lint/main.das`, and by the MCP `lint` tool. `seed_default_disabled` seeds the canonical default-off rule set (currently STYLE005) before the file is read.


### Constants

- `LINT_SKIP_HEADER_LINES`

### Structures

- `FormatPolicy`
- `FormatJob`
- `LintIssue`

### Configuration

- `add_module_rule_overrides`
- `build_lint_macro_disabled`
- `lint_config_forces_on`
- `load_env_disabled`
- `load_lint_config`
- `load_lint_config_from_path`
- `rule_docs_only_at`
- `seed_default_disabled`

### Path excludes

- `is_lint_path_excluded`
- `load_path_excludes_from_path`
- `matches_path_excludes`

### Path-based rule defaults

- `is_core_library_source`
- `is_daslib_source`
- `is_shipped_library_source`
- `lint029_source_exempt`

### Lint-surface predicates

- `is_lint_fixture_name`
- `is_user_authored_body`
- `lint_file_skip_reason`

### Structured findings

- `make_lint_issue`

### Format policy

- `format_policy_for`
- `partition_by_format_policy`

## toml

The TOML module parses `TOML 1.0`_ into the same `JsonValue?` tree shape produced by `daslib/json`, so existing `json_boost` accessors (`v ?? def`, `from_JV`, etc.) work on TOML inputs as-is. Date-time tokens are preserved as raw RFC-3339 strings since JSON has no native date type.


### Parsing

- `read_toml` - Reads TOML 1.0 from `text`.

### Writing

- `write_toml` - `write_toml` without the error out-param — returns `""` on unrepresentable input.

## logger

The logger module provides a structured, file-backed logging facility for daslang tools (MCP server, daslang-live, dastest, etc). Records are written as one JSON object per line ("JSON Lines" / `ndjson`) to `{get_das_root()}/logs/<name>.log`, with an ISO 8601 UTC timestamp (millisecond precision), level name, dotted category, message, and optional `fields` object.


### Setup

- `logger_get_name` - Returns the logical name passed to `logger_set_name` (empty if the path was set directly via `logger_set_path`).
- `logger_get_path` - Returns the currently configured log file path (empty if unset).
- `logger_init` - Convenience: `logger_set_name(name)` + `logger_install_hook()`.
- `logger_init_tee` - `logger_init` in tee mode: everything still prints to the console AND lands, timestamped, in `logs/<name>.log` — the one-liner for interactive servers.
- `logger_set_name` - Configure the log file as `{get_das_root()}/logs/<name>.log`.
- `logger_set_path` - Override the log file path explicitly.

### Level filters

- `logger_clear_category_levels` - Remove all per-category overrides.
- `logger_get_min_level` - Returns the currently configured global minimum level.
- `logger_set_category_level` - Override the level filter for one category.
- `logger_set_min_level` - Global minimum level.

### Output control

- `logger_capture_diagnostics` - Send the runtime's own diagnostics — a leak dump, a fatal, what it reports about itself — as raw text to `<log>.diag.log`, or to stderr when that cannot open.
- `logger_close` - Close the log file.
- `logger_flush` - Explicit flush.
- `logger_set_stderr_fallback` - Enable / disable writing to stderr when the log file can't be opened.
- `logger_set_tee` - Tee mode: hooked `print`/`to_log` lines go to the log file AND still reach stdout/stderr (the default hook behavior diverts them to the file only — right for stdio-transport tools, wrong for interactive servers watched on a console).

### Log calls

- `logger_critical` - Critical / fatal record.
- `logger_debug` - Debug-level record.
- `logger_error` - Error record.
- `logger_info` - Informational record.
- `logger_log` - Generic log entry.
- `logger_trace` - Verbose trace-level record.
- `logger_warning` - Warning record.

### Stdout hook

- `logger_install_hook` - Install a global debug agent that diverts `print` and `to_log` (from any Context, current or future) into the configured log file.

## sha_256

FIPS 180-4 SHA-256 in pure daslang: one-shot hashing of strings and byte arrays to lowercase hex, plus a streaming init/update/final state for data that arrives in pieces. Content addressing and integrity checks need a collision-resistant hash; the builtin 64-bit hash is not one.


### Structures

- `Sha256State` - Streaming hash state.

### One-shot hashing

- `sha256_file_hex` - SHA-256 of a file's bytes, lowercase hex; "" when the file is unreadable or empty.
- `sha256_hex` - SHA-256 of a byte array, lowercase hex (accepts temporary views).

### Streaming

- `sha256_final` - Apply FIPS 180-4 padding and return the digest as eight big-endian words.
- `sha256_hex_digest` - Digest words → 64-char lowercase hex.
- `sha256_init` - Fresh streaming state (FIPS 180-4 initial hash values).
- `sha256_update` - Absorb one byte.

## lpipe

The LPIPE module provides the `lpipe` macro for passing multiple block arguments to a single function call. While `<|` handles the first block argument, `lpipe` adds subsequent blocks on following lines.


### Call macros

- `lpipe` - Implements `lpipe`, allowing blocks to pipe the previous line's call.

## macro_boost

The MACRO_BOOST module provides utility macros for macro authors, including pattern matching on AST nodes, code generation helpers, and common transformation patterns used when writing compile-time code.


### Structures

- `CapturedVariable` - Stored captured variable together with the `ExprVar` which uses it

### Function annotations

- `MacroVerifyMacro` - Converts `macro_verify(expr, message, prog, at)` to equivalent inline code.

### Implementation details

- `macro_verify` - Same as verify, only the check will produce macro error, followed by return [[ExpressionPtr]]

### Block analysis

- `capture_block` - Collect all captured variables in the expression.
- `collect_finally` - Collect all finally blocks in the expression.
- `collect_labels` - Collect all labels in the expression.

### Expression analysis

- `has_sideeffects` - Conservative side-effect detection.

## md_boost

The MD_BOOST module provides Markdown generation helpers: GitHub-flavored table rendering with aligned columns and per-column alignment, plus small text utilities (bold, cell-separator escaping). Numeric cell formatting is left to the caller (the `"{x:.2f}"` interpolation-format form); this module handles layout and alignment.


### Enumerations

- `ColAlign`

### Tables

- `render_md_table`

### Text helpers

- `md_bold`
- `md_escape`

## match

The MATCH module implements pattern matching on variants, structs, tuples, arrays, and scalar values. Supports variable capture (`$v(name)`), wildcards (`_`), guard expressions (`&&`), and alternation (`||`).


### Call macros

- `static_multi_match` - Implements `static_multi_match` macro.
- `multi_match` - Implements `multi_match` macro.
- `match` - Implements `match` macro.
- `static_match` - Implements `static_match` macro.

### Structure macros

- `match_copy` - Implements `match_copy` annotation.
- `match_as_is` - Implements `match_as_is` annotation.

## math_bits

The MATH_BITS module provides bit-level reinterpretation between integer and floating point representations — `int_bits_to_float`, `uint_bits_to_float`, `float_bits_to_int`, `float_bits_to_uint` (plus the 64-bit `double` forms and 2/3/4-lane vector overloads) — as well as the `cast_to_*` helpers that pack and unpack values through a `float4` payload.


### float in int,uint

- `int_bits_to_float` - bit representation of x is interpreted as a float
- `uint_bits_to_float` - bit representation of x is interpreted as a float

### int,uint in float

- `float_bits_to_int` - bit representation of x is interpreted as a int
- `float_bits_to_uint` - bit representation of x is interpreted as a uint

### int64,uint64 in double

- `double_bits_to_int64` - bit representation of x is interpreted as a int64
- `double_bits_to_uint64` - bit representation of x is interpreted as a uint64
- `int64_bits_to_double` - bit representation of x is interpreted as a double
- `uint64_bits_to_double` - bit representation of x is interpreted as a double

### bit-cast vec4f

- `cast_to_int16` - return an int16 which was bit-cast from x
- `cast_to_int32` - return an int32 which was bit-cast from x
- `cast_to_int64` - return an int64 which was bit-cast from x
- `cast_to_int8` - return an int8 which was bit-cast from x
- `cast_to_pointer` - return a pointer which was bit-cast from x
- `cast_to_string` - return a string which pointer was bit-cast from x
- `cast_to_vec4f` - return a float4 which stores bit-cast version of x

## math_boost

The MATH_BOOST module adds geometric types (`AABB`, `AABR`, `Ray`), intersection tests (`is_intersecting`), plane helpers (`plane_dot`, `plane_normalize`, `plane_from_point_normal`, `planar_shadow`), color space conversion (`linear_to_SRGB`, `RGBA_TO_UCOLOR`, `UCOLOR_TO_RGBA`), and view/projection matrix construction (`look_at_lh`, `look_at_rh`, `perspective_rh`, `ortho_rh`). It requires `math` publicly, so requiring `math_boost` also brings in the whole scalar/vector math surface (`degrees`, `radians`, `sin`, `length`, ...).


### Structures

- `AABR` - axis aligned bounding rectangle
- `AABB` - axis aligned bounding box
- `Ray` - ray (direction and origin)

### Intersections

- `is_intersecting` - returns true if inputs intersect

### Matrices

- `look_at_lh` - left-handed (z forward) look at matrix with origin at `Eye` and target at `At`, and up vector `Up`.
- `look_at_rh` - right-handed (z towards viewer) look at matrix with origin at `Eye` and target at `At`, and up vector `Up`.
- `ortho_rh` - right handed (z towards viewer) orthographic (parallel) projection matrix
- `perspective_lh` - left-handed (z forward) perspective matrix
- `perspective_rh` - right-handed (z toward viewer) perspective matrix
- `perspective_rh_0_to_1` - right-handed (z toward viewer) perspective matrix, clip-space z in [0..1] (Vulkan / D3D).
- `perspective_rh_minus1_to_1` - right-handed (z toward viewer) perspective matrix, clip-space z in [-1..1] (OpenGL).
- `perspective_rh_opengl` - right-handed (z toward viewer) opengl (z in [-1..1]) perspective matrix
- `planar_shadow` - planar shadow projection matrix, i.e.

### Plane

- `plane_dot` - dot product of `Plane` and 'Vec'
- `plane_from_point_normal` - construct plane from point `p` and normal `n`
- `plane_normalize` - normalize `Plane`, length xyz will be 1.0 (or 0.0 for no plane)

### Color conversions

- `linear_to_SRGB` - convert value from linear space to sRGB curve space

### Color packing and unpacking

- `RGBA_TO_UCOLOR` - conversion from RGBA to ucolor.
- `UCOLOR_TO_RGB` - conversion from ucolor to RGB.
- `UCOLOR_TO_RGBA` - conversion from ucolor to RGBA.

## option

Monadic `Option<T>` — represents a value that may or may not be present. Functional API for modelling "absence" in ordinary value code, where nullable pointers are inapplicable and sentinel values (`-1`, `""`) are fragile.


### Structures

- `Option`

### Constructors

- `move_some`
- `none`
- `some`

### Queries

- `is_none`
- `is_some`

### Transforming

- `and_then`
- `filter`
- `map`
- `or_else`
- `or_value`

### Extraction

- `expect_value`
- `move_unwrap`
- `unwrap`
- `unwrap_or`
- `unwrap_or_default`
- `unwrap_or_else`

### Side effects

- `if_none`
- `if_some`

### Pairing

- `zip`

### Operators

- `$Option(type<auto(TT)>)!=`
- `$Option(type<auto(TT)>)==`
- `$Option(type<auto(TT)>)??`
- `auto(TT)!=`
- `auto(TT)==`

## profiler

The PROFILER module provides CPU profiling infrastructure for measuring function execution times. It includes instrumentation-based profiling with hierarchical call tracking and timing statistics.


### Profiler control

- `set_enable_profiler` - Enables or disables the profiler for the given context ID.
- `set_enable_profiler_log` - Enables or disables the profiler log for the given context ID.

## profiler_boost

The PROFILER_BOOST module extends profiling with high-level macros for scoped timing (`profile_block`), function-level profiling annotations, and formatted output of profiling results.


### Context profiler control

- `disable_profiler` - Disables the profiler for the given context.
- `disable_profiler_log` - Disables the profiler log for the given context.
- `enable_profiler` - Enables the profiler for the given context.
- `enable_profiler_log` - Enables the profiler log for the given context.

## PUGIXML_boost

The PUGIXML_BOOST module provides high-level daScript helpers on top of the low-level `pugixml` C++ bindings.


### Block-based iteration

- `for_each` - Iterates over all `xpath_node` entries in an `xpath_node_set`.
- `for_each_attribute` - Iterates over all attributes of *node*.
- `for_each_child` - Iterates over all child elements of *node*.
- `for_each_child_reverse` - Iterates over all child nodes of *node* (text and comments included, not only elements) in **reverse** document order (`last_child` / `previous_sibling`, both O(1) in pugixml).

### Iterator-based iteration

- `each` - Returns a lazy iterator over all `xpath_node` entries in an `xpath_node_set`.
- `each_attribute` - Returns a lazy iterator over all attributes of *node*: `for (a in each_attribute(node)) { ...
- `each_child` - Returns a lazy iterator over all child elements of *node*: `for (ch in each_child(node)) { ...
- `each_child_reverse` - Returns a lazy iterator over all child nodes of *node* — text and comments included, not only elements — in **reverse** document order: `for (ch in each_child_reverse(node)) { ...

### LINQ source

- `build_xml_row` - Fills *row*'s scalar fields from same-named attributes of *node*.
- `from_xml_node` - As `from_xml_node` above, but only walks child elements whose tag matches *name* (`child`/`next_sibling` by name already match elements only).
- `peek_xml_field` - Like `read_xml_field` (linq_fold's materialize-under-guard path emits it for where-gate fields), but a `string` attribute comes back as a borrowed temp `string#` — a non-owning view into the document, whose `#` rejects storing it, so it cannot dangle past the document's RAII scope.
- `read_xml_field` - Reads one same-named attribute of *node* into a *TT*-typed value — the field-pruned counterpart to `build_xml_row` (same supported types), emitted by linq_fold's fused XML lane.

### RAII document handling

- `open_xml` - Opens an XML file, invokes *blk* with the parsed document and a success flag, then automatically frees the document.
- `parse_xml` - Parses an XML string, invokes *blk* with the document and a success flag, then automatically frees the document.
- `with_doc` - Creates an empty `xml_document`, invokes *blk*, then frees it.

### Quick accessors

- `node_attr` - Returns the string value of attribute *attr_name*, or *default_value* if no such attribute exists.
- `node_attr_bool` - Returns the boolean value of attribute *attr_name*.
- `node_attr_float` - Returns the float value of attribute *attr_name*.
- `node_attr_int` - Returns the integer value of attribute *attr_name*.
- `node_text` - Returns the text content of the child element named *child_name*, or *default_value* if the child does not exist or has no text.

### Builder helpers

- `add_attr` - Appends an attribute with *name* and *value* (bool).
- `add_child` - Appends a child element with *name* and sets its text content.
- `add_child_ex` - Appends a child element with *name* and a single attribute.

### Builder EDSL

- `attr` - Appends a bool attribute, returns parent for chaining.
- `tag` - Appends a bare child element to a document.

### String conversion

- `to_string` - Serializes the entire document to a pretty-printed XML string with two-space indentation and default formatting.

### XPath helpers

- `for_each_select` - Selects all nodes matching *xpath_query* and invokes *blk* for each result.
- `select_text` - Runs an XPath query and returns the text content of the first matching node, or *default_value* if nothing matches.
- `select_value` - Runs an XPath query and returns the value of the first matching node.
- `with_xpath` - Compiles an XPath expression, invokes *blk* with the compiled query handle, then frees it.

### Serialization

- `XML` - Serializes a native value into XML children of *node*.
- `XML_table`
- `from_XML` - Deserializes an XML node tree into a native value.
- `to_XML` - Serializes a struct to a complete XML string, wrapping it in an element named *root_name* (default `"root"`).

### Type conversion operators

- `operator as bool` - Converts an `xml_attribute` value to `bool` (default `false`).
- `operator as double` - Converts an `xml_attribute` value to `double` (default `0.0lf`).
- `operator as float` - Converts an `xml_attribute` value to `float` (default `0.0`).
- `operator as int` - Converts an `xml_attribute` value to `int` (default `0`).
- `operator as string` - Converts an `xml_attribute` value to `string` (default `""`).
- `operator as uint` - Converts an `xml_attribute` value to `uint` (default `0u`).
- `operator as xml_node` - Returns the document as an `xml_node` handle for mutation APIs that require a node (e.g.
- `operator is bool` - Returns `true` if the attribute exists.
- `operator is double` - Returns `true` if the attribute exists.
- `operator is float` - Returns `true` if the attribute exists.
- `operator is int` - Returns `true` if the attribute exists (has a value convertible to int).
- `operator is string` - Returns `true` if the attribute exists.
- `operator is uint` - Returns `true` if the attribute exists.
- `xml_node[]` - Returns the attribute named *attr_name*.

## quote

The QUOTE module provides quasiquotation support for AST construction. It allows building AST nodes using daslang syntax with `$`-prefixed splice points for inserting computed values, making macro writing more readable and less error-prone than manual AST construction.


### Structures

- `CaptureEntryInitData` - Initialization data for a captured variable entry.
- `LineInfoInitData` - Initialization data for source line info reconstruction.
- `EnumEntryInitData` - Initialization data for a quoted enum entry.
- `AnnotationArgumentInitData` - Initialization data for a quoted annotation argument.
- `FileInfoInitData` - Initialization data for reconstructing file info.

### Clone operations

- `clone` - Clones an array of AnnotationArgumentInitData into an AnnotationArgumentList.
- `clone_file_info` - Creates a FileInfo from a FileInfoInitData struct.
- `clone_line_info` - Creates a LineInfo from a LineInfoInitData struct.
- `resolve_file_info` - Stable FileInfo for reconstructed LineInfos: one interned dummy per (file name, tab size) (per context), instead of a fresh allocation per evaluation.

### Conversion

- `cvt_to_mks` - Converts an array of arguments into a MakeStruct pointer.
- `make_alias_type_decl` - Reconstruction helper: a by-name alias TypeDecl, resolved by re-infer at the splice site.

## random

The RANDOM module implements pseudo-random number generation using a linear congruential generator with vectorized state (`int4`). It provides integer, float, and vector random values, as well as geometric sampling (unit vectors, points in spheres and disks).


### Constants

- `LCG_RAND_MAX` - maximum possible output of random number generator
- `LCG_RAND_MAX_BIG` - maximum possible output of random_big_int

### Seed and basic generators

- `random_big_int` - random integer 0..32768*32768-1 (LCG_RAND_MAX_BIG)
- `random_float` - random float 0..1
- `random_float4` - random float4, each component is 0..1
- `random_int` - random integer 0..32767 (LCG_RAND_MAX)
- `random_int4` - random int4, each component is 0..32767 (LCG_RAND_MAX)
- `random_seed` - constructs seed vector out of single integer seed
- `random_seed2D` - constructs seed vector out of 2d screen coordinates and frame counter `cf`
- `random_uint` - random unsigned integer using 3-component LCG, covering full uint range

### Random iterators

- `each_random_uint` - infinite generator of random uints initialized with `rnd_seed`

### Specific distributions

- `random_in_unit_disk` - Returns a random float3 point uniformly distributed inside the unit disk (length <= 1, z=0).
- `random_in_unit_sphere` - Returns a random float3 point uniformly distributed inside the unit sphere (length <= 1).
- `random_unit_vector` - random float3 unit vector (length=1.)

## refactor

The REFACTOR module implements automated code refactoring transformations. It provides tools for renaming symbols, extracting functions, and other structural code changes that preserve program semantics.


### Function annotations

- `ExtractMethodMacro` - Function annotation implementing extract-method refactoring.
- `ExtractVariableFunction` - Function annotation for extract-variable target functions.

### Call macros

- `extract_variable` - Call macro implementing extract-variable refactoring.

### Refactoring operations

- `extract_expression` - Marks an expression for expression extraction refactoring.
- `extract_method` - Marks a block of code for method extraction refactoring.
- `extract_variable_nonref` - Marks an expression for variable extraction by value.
- `extract_variable_ref` - Marks an expression for variable extraction by reference.

## regex_boost

The REGEX_BOOST module extends regular expressions with the `%regex~` reader macro for compile-time regex construction. Inside the reader macro, backslashes are literal — no double-escaping is needed (e.g. `%regex~\d{3}%%` instead of `"\\d\{3}"`).


### Reader macros

- `regex` - Reader macro that converts `%regex~` literals into precompiled `regex::Regex` objects at compilation time.

## regex

The REGEX module implements regular expression matching and searching. It provides `regex_compile` for building patterns, `regex_match` for full-string matching, `regex_search` for finding the first match anywhere, `regex_foreach` for iterating all matches, `regex_replace` for substitution (both block-based and template-string forms), `regex_split` for splitting strings, `regex_match_all` for collecting all match ranges, `regex_group` for capturing groups by index, and `regex_group_by_name` for named group lookup.


### Type aliases

- `CharSet` - Bitfield character set used internally by the regex engine.
- `ReGenRandom` - Random number generator callback used by `re_gen` for regex-based string generation.
- `MaybeReNode` - Regex node or nothing.

### Enumerations

- `ReOp` - Type of regular expression operation.

### Structures

- `ReNode` - Regular expression node.
- `Regex` - Regular expression structure.

### Compilation and validation

- `debug_set` - Prints all characters contained in a `CharSet` for debugging purposes.
- `is_valid` - Returns `true` if the compiled regex is valid and ready for matching.
- `regex_compile` - Compiles a regular expression pattern string into a `Regex` object.
- `regex_debug` - Prints the internal structure of a compiled regex for debugging purposes.
- `visit_top_down` - Visits all nodes of a compiled regex tree in top-down order, invoking a callback for each node.

### Access

- `Regex[]` - Returns the match range for the given group index.
- `regex_foreach` - Iterates over all non-overlapping matches of a regex in a string, invoking a block for each match.
- `regex_group` - Returns the substring captured by the specified group index after a successful match.
- `regex_group_by_name` - Returns the matched substring for the named capturing group `(?P<name>...)`.

### Match & replace

- `regex_match` - Matches a compiled regex against a string and returns the end position of the match, or `-1` on failure.
- `regex_match_all` - Returns an array of all non-overlapping match ranges for the regular expression in `str`.
- `regex_replace` - Replaces each substring matched by the regex with the result returned by the provided block.
- `regex_search` - Searches for the first occurrence of the regular expression anywhere in `str`, starting from `offset`.
- `regex_split` - Splits `str` by all non-overlapping matches of the regular expression.

### Generation

- `re_gen` - Generates a random string that matches the given compiled regex.
- `re_gen_get_rep_limit` - Returns the maximum repetition limit used by regex quantifiers during string generation.

## remove_call_args

The REMOVE_CALL_ARGS module provides AST transformation macros that remove specific arguments from function calls at compile time. Used for implementing optional parameter patterns and compile-time argument stripping.


### Function annotations

- `remove_call_args` - This macro removes all arguments by given indices [remove_call_args(arg=(1,2,3))]

## result

Monadic `Result<T, E>` — a value (`ok`) or an error (`err`). Functional API for modelling fallible computations where an error carries meaning (unlike a bare `Option<T>` that only says "missing").


### Structures

- `Result`

### Constructors

- `err`
- `move_err`
- `move_ok`
- `ok`

### Queries

- `is_err`
- `is_ok`

### Transforming

- `and_then`
- `map`
- `map_err`
- `or_else`

### Extraction

- `expect_err`
- `expect_value`
- `move_unwrap`
- `move_unwrap_err`
- `unwrap`
- `unwrap_err`
- `unwrap_or`
- `unwrap_or_default`
- `unwrap_or_else`

### Side effects

- `if_err`
- `if_ok`

### Bridges to Option

- `err_to_option`
- `to_option`

### Operators

- `$Result(type<auto(TT)>,type<auto(EE)>)!=`
- `$Result(type<auto(TT)>,type<auto(EE)>)==`
- `$Result(type<auto(TT)>,type<auto(EE)>)??`
- `auto(TT)!=`
- `auto(TT)==`

## rst

The RST module implements the documentation generation pipeline for daslang. It uses RTTI to introspect modules, types, and functions, then produces reStructuredText output suitable for Sphinx documentation builds.


### Enumerations

- `DocumentBlock` - The section kinds a generated module page can carry; `DocGenConfig.doc_block_order` lists them in the order the page renders.

### Structures

- `DocGroup` - Group of documentation items.
- `DocsHook` - Hook for RST documentation generation.

### Document writers

- `document` - Generates RST documentation for a single module and writes it to a file.
- `document_enumeration` - Generates RST documentation for a single enumeration type.
- `document_enumerations` - Generates RST documentation for all enumerations in the given modules.
- `documents` - Generates RST documentation for multiple modules and writes them to files.
- `write_digest` - Writes the markdown module digest to `digest_path`: a sorted module index, then one section per documented module with its intro paragraph and every public symbol on one line.

### Descriptions

- `describe_short` - Returns a concise one-line description of an expression or type.

### Label makers

- `function_label_file` - Creates a unique, file-name-safe label string for a function.

### RST section makers

- `make_group` - Creates a named documentation group with a decorative RST section header.

### Group operations

- `append_to_group_by_regex` - Appends functions whose names match a regex to an existing documentation group.
- `group_by_regex` - Groups module items whose names match the provided regular expression under a documentation section.
- `hide_group` - Marks the specified documentation group as hidden so it is excluded from output.

### Naming helpers

- `safe_function_name` - Escapes special characters in a function name to produce a safe identifier for RST output.

### Reference policy

- `mark_module_pageless` - Declares that a module (typically a raw native binding such as `imgui`) gets no generated documentation pages.

## safe_addr

The SAFE_ADDR module provides compile-time checked pointer operations. `safe_addr` returns a temporary pointer to a variable only if the compiler can verify the pointer will not outlive its target. This prevents dangling pointer bugs without runtime overhead.


### Function annotations

- `SafeAddrMacro` - This macro reports an error if safe_addr is attempted on the object, which is not local to the scope.
- `SharedAddrMacro` - This macro reports an error if shared_addr is attempted on anything other that shared global variables.
- `TempValueMacro` - This macro reports an error if temp_value is attempted outside of function arguments.

### Safe temporary address

- `safe_addr` - returns temporary pointer to the given expression
- `shared_addr` - returns address of the given shared variable.

### Temporary pointers

- `temp_ptr` - returns temporary pointer from a given pointer

### Temporary values

- `temp_value` - returns temporary reference to the given expression

## soa

The SOA (Structure of Arrays) module transforms array-of-structures data layouts into structure-of-arrays layouts for better cache performance. It provides macros that generate parallel arrays for each field of a structure, enabling SIMD-friendly data access patterns.


### Structures

- `SOA_INDEX` - Proxy type returned by the `[]` operator on an SOA structure.

### Function annotations

- `SoaCallMacro` - Rewrites `soa[index].field` into `soa.field[index]` at compile time.

### Structure macros

- `soa` - Generates a Structure-of-Arrays layout from a regular struct.

### SOA field access

- `SOA_INDEX.` - Field access operator for SOA_INDEX; rewritten by SoaCallMacro to convert soa[index].field into soa.field[index].

## sort_boost

The SORT_BOOST module provides the `qsort` macro that uniformly sorts built-in arrays, dynamic arrays, and C++ handled vectors using the same syntax. It automatically wraps handled types in `temp_array` as needed.


### Call macros

- `qmake_heap` - Implements `qmake_heap(value, block)` for arrays and handled vectors.
- `qnth_element` - Implements `qnth_element(value, n, block)` for arrays and handled vectors.
- `qpush_heap` - Implements `qpush_heap(value, block)` for arrays and handled vectors.
- `qsort` - Implements `qsort` macro.
- `qpartial_sort` - Implements `qpartial_sort(value, n, block)` — dispatches to `partial_sort` for array sources or `partial_sort(temp_array(value), …)` for handled vector types.
- `qpop_heap` - Implements `qpop_heap(value, block)` for arrays and handled vectors.

### Partial sorting

- `nth_element`
- `partial_sort`

### Heap operations

- `make_heap`
- `pop_heap`
- `push_heap`

## static_let

The STATIC_LET module implements the `static_let` pattern — local variables that persist across function calls, similar to C `static` variables. The declaration is promoted to module scope under a mangled name, so it is initialized once when the context starts (not lazily on first call) and retains its value across calls. `static_let_finalize` additionally deletes the variable on context shutdown.


### Function annotations

- `StaticLetMacro` - This macro implements the `static_let` and `static_let_finalize` functions.

### Static variable declarations

- `static_let` - Given a scope with the variable declarations, this function will make those variables global.
- `static_let_finalize` - This is very similar to regular static_let, but additionally the variable will be deleted on the context shutdown.

## stbimage_boost

High-level image API built on stbimage and raster. Provides the Image struct with load, save, resize, flip, crop, blit, and format conversion.


### Structures

- `Image` - Image container with raw pixel data in original format.
- `Image.valid`
- `Image.stride`
- `Image.has_alpha`
- `Image.pixel_size`
- `Image.is_hdr`
- `Image.data_size`
- `Image.load`
- `Image.load_hdr`
- `Image.load_16`
- `Image.load_from_memory`
- `Image.load_hdr_from_memory`
- `Image.load_16_from_memory`
- `Image.save`
- `Image.encode`
- `Image.resize`
- `Image.flip_vertical`
- `Image.flip_horizontal`
- `Image.crop`
- `Image.blit`
- `Image.fill_rect`
- `Image.blit_alpha`
- `Image.to_channels`
- `Image.to_bpc`

### Image construction

- `make_image` - Create a blank (zeroed) image with the given dimensions.

### Image loading

- `image_info` - Get image dimensions and channel count without loading pixel data.
- `is_16_bit` - Returns true if the file contains 16-bit data.
- `is_16_bit_from_memory` - Returns true if the memory buffer contains 16-bit data.
- `is_hdr` - Returns true if the file is in HDR format.
- `is_hdr_from_memory` - Returns true if the memory buffer contains HDR format data.
- `load_image` - Load an image from file.

### Pixel access

- `with_pixels` - Access pixel data as `array<float>`.
- `with_pixels_16` - Explicit-name alias for `with_pixels` with uint16 block.
- `with_pixels_f` - Explicit-name alias for `with_pixels` with float block.
- `with_row` - Access a single row as `array<float>`.
- `with_row_16` - Explicit-name alias for `with_row` with uint16 block.
- `with_row_f` - Explicit-name alias for `with_row` with float block.

## stbimage_ttf

High-level TrueType font loading and text rendering API. Loads fonts via stb_truetype packing API, stores atlas as `Image`, and provides software text rendering with alpha blending. Built on `stbimage_boost` and `stbtruetype`.


### Structures

- `Font` - Font loaded from a TTF file.

### Low-level initialization

- `stbtt_init` - Initialize a `stbtt_fontinfo` from font data.
- `stbtt_num_fonts` - Returns the number of fonts in a TrueType collection (.ttc), or 1 for a single .ttf.

### Low-level scale

- `stbtt_scale_for_em` - Compute scale factor for mapping em to pixels.
- `stbtt_scale_for_pixel_height` - Compute scale factor for a given pixel height.

### Low-level metrics

- `stbtt_codepoint_box` - Returns glyph bounding box `int4(x0, y0, x1, y1)` in font units.
- `stbtt_codepoint_hmetrics` - Returns horizontal metrics in font units for a codepoint.
- `stbtt_codepoint_kern` - Returns kerning advance between two codepoints in font units.
- `stbtt_find_glyph` - Returns glyph index for a codepoint.
- `stbtt_font_bbox` - Returns font bounding box `int4(x0, y0, x1, y1)` in font units.
- `stbtt_font_vmetrics` - Returns vertical metrics in font units: ascent, descent, line gap.

### Low-level glyph shape

- `stbtt_codepoint_shape` - Iterate glyph shape vertices for a codepoint via callback.
- `stbtt_codepoint_shape_count` - Returns the number of shape vertices for a codepoint.

### Low-level bitmap rendering

- `stbtt_codepoint_bitmap` - Renders a codepoint glyph bitmap at the given scale.
- `stbtt_codepoint_bitmap_box` - Returns pixel-space bounding box `int4(ix0, iy0, ix1, iy1)` for a codepoint bitmap at the given scale.
- `stbtt_codepoint_bitmap_box_subpixel` - Returns pixel-space bounding box with subpixel shift.
- `stbtt_codepoint_bitmap_subpixel` - Renders a codepoint glyph bitmap with subpixel shift.
- `stbtt_make_codepoint_bitmap` - Renders a codepoint glyph into a caller-provided buffer.
- `stbtt_make_codepoint_bitmap_subpixel` - Renders a codepoint glyph with subpixel shift into a caller-provided buffer.

### Low-level packing

- `stbtt_pack` - Pack a character range into an atlas bitmap.
- `stbtt_packed_quad` - Returns the packed quad for a character, advancing `xpos`.

### Font loading

- `is_valid` - Returns `true` if the font has been successfully loaded.
- `load_font` - Load a TrueType font from file for metrics and shape queries (no atlas).
- `load_ttf` - Load a TrueType font from file and pack a character range into an atlas.

### Font scale

- `scale_for_em` - Compute scale factor for mapping em to pixels.
- `scale_for_pixel_height` - Compute scale factor for a given pixel height.

### Font metrics

- `codepoint_box` - Returns glyph bounding box `int4(x0, y0, x1, y1)` in font units.
- `codepoint_hmetrics` - Returns horizontal metrics in font units for a codepoint.
- `codepoint_kern` - Returns kerning advance between two codepoints in font units.
- `find_glyph` - Returns glyph index for a codepoint.
- `font_bounding_box` - Returns font bounding box `int4(x0, y0, x1, y1)` in font units.
- `font_metrics` - Returns font metrics in pixels using the font's `pixel_height`.
- `font_vmetrics` - Returns vertical metrics in font units: ascent, descent, line gap.

### Font glyph shape

- `codepoint_shape` - Iterate glyph shape vertices for a codepoint via callback.
- `codepoint_shape_count` - Returns the number of shape vertices for a codepoint.

### Font bitmap rendering

- `codepoint_bitmap` - Renders a codepoint glyph bitmap at the given scale.
- `codepoint_bitmap_box` - Returns pixel-space bounding box `int4(ix0, iy0, ix1, iy1)` for a codepoint bitmap.
- `codepoint_bitmap_box_subpixel` - Returns pixel-space bounding box with subpixel shift.
- `codepoint_bitmap_subpixel` - Renders a codepoint glyph bitmap with subpixel shift.

### Text measurement and rendering

- `measure_text` - Measure the width of a string in pixels at the given pixel height.
- `render_text` - Render text onto a 4-channel RGBA image using pixel-to-pixel alpha blit.

## stringify

The STRINGIFY module provides the `%stringify~` reader macro for embedding multi-line string literals verbatim. Text between `%stringify~` and `%%` is captured as-is without requiring escape sequences for quotes, braces, or other special characters.


### Reader macros

- `stringify` - This macro embeds a long string into the source code.

## strings_boost

The STRINGS_BOOST module extends string handling with splitting (`split`, `split_by_chars`), joining (`join`), padding (`wide`), multi-substring replacement (`replace_multiple`), and edit distance (`levenshtein_distance`). It re-exports `strings`, so the built-in string surface is available too.


### Split and join

- `join` - Joins the elements of an iterable into a single string using the specified separator.
- `split` - Splits a string by the specified delimiter string and returns an array of substrings.
- `split_by_chars` - Splits a string by the specified delimiter characters and returns an array of substrings.

### Formatting

- `capitalize` - Returns a copy of the string with the first character converted to uppercase.
- `pad_left` - Pads the string with the character `ch` on the left to reach the specified minimum `width`.
- `pad_right` - Pads the string with the character `ch` on the right to reach the specified minimum `width`.
- `wide` - Pads the string with trailing spaces to reach the specified minimum width.

### Queries and comparisons

- `contains` - Returns true if `sub` is found anywhere within `str`.
- `count` - Counts non-overlapping occurrences of `sub` in `str`.
- `eq` - Compares a `string` with a `das_string` for equality, returning `true` if they match.
- `is_character_at` - Returns `true` if the byte at the specified index in the array equals the given character code.
- `is_null_or_whitespace` - Returns true if the string is null, empty, or contains only whitespace characters (space, tab, CR, LF, FF, VT).

### Search and match

- `glob_match` - Matches `text` against a shell-style **filename** glob `pattern`.
- `last_index_of` - Returns the index of the last occurrence of `sub` in `str` searching only up to position `start` (exclusive), or -1 if not found.

### Replace

- `replace_multiple` - Applies multiple find-and-replace substitutions to a string in a single pass.

### Prefix and suffix

- `trim_prefix` - Removes `prefix` from the beginning of `str` if present.
- `trim_suffix` - Removes `suffix` from the end of `str` if present.

### String similarity

- `jaccard` - Jaccard similarity over two string-sets, returning `|intersection| / |union|` in 0..1.
- `levenshtein_distance` - Computes the Levenshtein edit distance between two strings.
- `levenshtein_distance_fast` - Computes the Levenshtein edit distance between two strings using an optimized algorithm.

## strings_convert

The STRINGS_CONVERT module provides soft-failing string-to-numeric conversions that return `Result<T; ConversionError>` instead of panicking or silently returning zero. Use these when parsing untrusted input where you need to distinguish between not-a-number, overflow, and trailing-garbage.


### Enumerations

- `ConversionError`

### Soft-failing conversions

- `try_to_double`
- `try_to_float`
- `try_to_int`
- `try_to_int16`
- `try_to_int64`
- `try_to_int8`
- `try_to_uint`
- `try_to_uint16`
- `try_to_uint64`
- `try_to_uint8`

## temp_strings

The TEMP_STRINGS module provides temporary string construction that avoids heap allocations. Temporary strings are allocated on the stack or in scratch memory and are valid only within the current scope, offering fast string building for formatting and output.


### Function annotations

- `TempStringMacro` - Function annotation that enables temporary string optimization.

### Temporary string builders

- `build_temp_string` - Same as build_string, but delete the string after the callback is called.

### Temporary string conversion

- `temp_string` - Construct string from array of bytes and pass it to the callback.

## templates_boost

The TEMPLATES_BOOST module extends template utilities with high-level macros for common code generation patterns, including template function generation, type-parameterized struct creation, and compile-time code expansion.


### Structures

- `Template` - This structure contains collection of substitution rules for a template.

### Call macros

- `qmacro_template_function` - Call macro for quoting named template functions.
- `qmacro_variable` - This macro implements expression reification for variables.
- `qmacro_type` - This macro implements type declaration reification 'qmacro_type'
- `qmacro_function` - This macro implements expression reification for functions.
- `qmacro` - This macro implements expression reification 'qmacro'
- `qmacro_template_class` - Call macro for quoting named template class methods.
- `qmacro_method` - This macro implements expression reification for class methods.
- `qmacro_block` - This macro implements expression block reification 'qmacro_block'
- `qmacro_expr` - This macro implements first line of the expression block reification 'qmacro_expr'
- `qmacro_block_to_array` - This macro implements expression block to array reification 'qmacro_block_to_array'

### Template rules

- `kaboomVarField` - Adds a rule to to the template to replace a variable field access with a prefix and suffix.
- `renameCall` - Adds a rule to the template to rename a call.
- `renameField` - Adds a rule to the template to rename any field lookup (., ?., as, is, etc)
- `renameVariable` - Adds a rule to the template to rename a variable.
- `replaceAnnotationArgument` - Adds a rule to the template to replace an annotation argument with the result of a callback.
- `replaceArgumentWithList` - Adds a rule to the template to replace a block argument with a list of variables.
- `replaceBlockArgument` - Adds a rule to the template to rename a block argument.
- `replaceStructWithTypeDecl` - Adds a rule to the template to replace a type alias with another type alias, specified by type declaration.
- `replaceType` - Adds a rule to the template to replace a type alias with another type alias, specified by name.
- `replaceTypeWithTypeDecl` - Adds a rule to the template to replace a type alias with another type alias, specified by type declaration.
- `replaceVarTag` - Adds a rule to the template to replace a variable tag with an expression.
- `replaceVariable` - Adds a rule to the template to replace a variable with an expression.
- `replaceVariableWithList` - Adds a rule to the template to replace a variable with an expression list.

### Template application

- `apply_template` - Applies the template to the given expression.
- `clone_and_rename_var` - Clones `src` and renames every `ExprVar` named `from_name` to `to_name`.

### Expression helpers

- `expression_at` - Force expression location, then return it.
- `remove_deref` - Removes dereferences of the variable `varname` from the expression.
- `visit_expression` - Visits the expression with the given visitor adapter.

### Expression generation

- `make_expression_block` - Create ExprBlock and move all expressions from expr to the list of the block.

### Block helpers

- `move_unquote_block` - Moves the corresponding block subexpression expression from the ExprMakeBlock.
- `push_block_list` - Splice every statement from a `qmacro_block(...) { ...
- `unquote_block` - Returns the corresponding block subexpression expression from the ExprMakeBlock.

### Global variable helpers

- `add_global_let` - Add global variable to the module, given name and initial value.
- `add_global_private_let` - Add global variable to the module, given name and initial value.
- `add_global_private_var` - Add global variable to the module, given name and initial value.
- `add_global_var` - Add global variable to the module, given name and type.

### Hygienic names

- `make_unique_private_name` - Generates unique private name for the variable, given prefix and line info.

### Quoting macros

- `apply_qblock` - Implementation details for the expression reification.
- `apply_qblock_expr` - Implementation details for the expression reification.
- `apply_qblock_to_array` - Implementation details for the expression reification.
- `apply_qmacro` - Implementation details for the expression reification.
- `apply_qmacro_function` - Implementation details for reification.
- `apply_qmacro_method` - Implementation details for reification.
- `apply_qmacro_template_class` - Implementation details for the expression reification.
- `apply_qmacro_template_function` - Applies template rules to a function, cloning it with substituted types.
- `apply_qmacro_variable` - Implementation details for reification.
- `apply_qtype` - Implementation details for the expression reification.

### Type pointer helpers

- `add_array_ptr_ref` - Implementation details for the reification.
- `add_type_ptr_ref` - Implementation details for the reification.

### Structure helpers

- `add_structure_field` - Adds a field to the structure.

### Class generation

- `enum_class_type` - return underlying type for the enumeration
- `make_class` - Creates a class structure.
- `make_class_constructor` - Adds a class constructor from a constructor function.
- `modify_to_class_member` - Modifies function to be a member of a particular class.

## templates

The TEMPLATES module implements template instantiation utilities for daslang code generation. It supports stamping out parameterized code patterns with type and value substitution.


### Function annotations

- `template` - This macro is used to remove unused (template) arguments from the instantiation of the generic function.

### Call macros

- `decltype` - This macro returns `ast::TypeDecl` for the corresponding expression.
- `decltype_noref` - This macro returns TypeDecl for the corresponding expression, minus the ref (&) portion.

## type_traits

The TYPE_TRAITS module provides compile-time type introspection and manipulation. It includes type queries (`is_numeric`, `is_string`, `is_pointer`), type transformations, and generic programming utilities for writing type-aware macros and functions.


### Call macros

- `is_subclass_of` - Converts to 'true' if the first type is a subclass of the second type.

### Typeinfo macros

- `fields_count` - this macro implements "fields_count" type trait, which returns total number of fields in the structure
- `safe_has_property` - this macro implements "has_property" type trait, which returns true when structure has a property

## typemacro_boost

The TYPEMACRO_BOOST module provides infrastructure for defining type macros — custom compile-time type transformations. Type macros allow introducing new type syntax that expands into standard daslang types during compilation.


### Structures

- `TypeMacroTemplateArgument` - Holds a type macro template argument with its name and inferred type.

### Function annotations

- `typemacro_function` - This macro converts function into a type macro.
- `typemacro_template_function` - This one converts function into a type macro that uses template arguments.

### Structure macros

- `template_tuple` - Like `[template_structure]`, but produces a typemacro that returns a named tuple instead of cloning a structure.
- `template_structure` - This macro creates typemacro function and associates it with the structure.
- `typemacro_documentation` - Structure annotation that stores type macro documentation metadata.
- `typemacro_template` - Structure annotation that marks a struct as a type macro template instance.
- `tsi_reified` - Marker the reifier appends to a finished instance: a template CHAIN pipes one reifier copy per ancestor, and the surplus copies no-op on sight of this.
- `template_struct_instance` - Reifies a `struct template` / `class template` parent into the annotated instance at parse time: instance `typedef`s bind the template's type parameters, `override` inits bind its `@template_constant` fields (erased from the result), `@template_gate` fields exist only in stamps where the named bool `@template_constant` is true (`@template_gate = AXIS`, or the string form `"!AXIS"` to invert; a gated-off field is erased as if never written, and the argument stays inert on kept fields — reference a gated field only from `static_if` arms on its axis: a dead `static_if` arm may name it, a dead ternary arm may not, since infer sees both ternary arms), `@template_call` fields rebind free-function callees by name (`override sdot = @@ssdot` — bare-spelled calls AND `@@` addresses; erased too, while `_::sdot` / `__::sdot` keep their normal resolution rules — the pinned escape to the real function), template methods are cloned onto the instance (instance-authored methods win), and the template parent is cut — every later structure annotation sees a finished concrete struct or class.

### Enum helpers

- `int64_to_enum` - Converts an int64 value to the specified enum type via reinterpret cast.

### Template structure instantiation

- `is_typemacro_template_instance` - template instance is determined by having parent == template.parent
- `make_typemacro_template_instance` - Annotates a structure as a typemacro template instance of the given template type.
- `template_structure_name` - Builds a mangled template structure name from its base name and argument types.

### Type inference helpers

- `add_structure_aliases` - Adds all template argument type aliases to a structure.
- `infer_struct_aliases` - Infers structure alias types for all template arguments from a structure definition.
- `infer_template_types` - Infers and validates template argument types against a pass argument, returning the resolved type.
- `verify_arguments` - Verifies that all template arguments have been fully inferred (no remaining auto or alias types).

### String constant access

- `get_string_const` - Extracts a string constant value or function address name from an expression.

### Work tracking

- `is_custom_work_done` - Returns true if custom work has already been performed on the template structure.
- `mark_custom_work_done` - Marks the template structure's custom work as complete in its annotation.

### Type macro arguments

- `typemacro_argument` - Extracts a string constant or function address argument at the given index from a type macro's dimension expressions.

## unroll

The UNROLL module implements compile-time loop unrolling. The `unroll` macro replaces a `for` loop with a constant `range` bound by stamping out each iteration as separate inlined code, eliminating loop overhead.


### Function annotations

- `UnrollMacro` - This macro implements loop unrolling in the form of `unroll` function.

### Unrolling

- `unroll` - Unrolls the for loop (with fixed range)

## uriparser_boost

The URIPARSER_BOOST module extends URI handling with convenience functions for common operations like building URIs from components, extracting query parameters, and resolving relative paths.


### Split and compose

- `uri_compose` - Compose a URI from its components.
- `uri_compose_query` - Compose a query string from a table of key-value pairs.
- `uri_compose_query_in_order` - Compose a query string from a table of key-value pairs, in the sorted order.
- `uri_split_full_path` - Split the full path of a URI into its components.

### Component accessors

- `fragment` - Return the fragment of a URI.
- `host` - Return the host of a URI.
- `path` - Return the path of a URI.
- `port` - Return the port of a URI.
- `query` - Return the query of a URI.
- `scheme` - Returns the scheme of a URI.
- `user_info` - Return the user info of a URI.

## utf8_utils

The UTF8_UTILS module provides Unicode UTF-8 string utilities including character iteration, codepoint extraction, byte length calculation, and validation of UTF-8 encoded text.


### Constants

- `s_utf8d` - Byte-class and state-transition table for the UTF-8 DFA decoder.
- `UTF8_ACCEPT` - DFA accept state indicating a valid UTF-8 sequence.
- `_UTF32_NON_WORD_RANGES`
- `_UTF32_LATIN1_LOWER`

### Encoding and decoding

- `decode_unicode_escape` - Decodes Unicode escape sequences (backslash followed by hex digits) in a string to UTF-8.
- `utf16_to_utf32` - Converts a UTF-16 surrogate pair to a single UTF-32 codepoint.
- `utf8_decode` - Converts UTF-8 string to UTF-32 and returns it as an array of codepoints (UTF-32 string)
- `utf8_encode` - Converts a codepoint (UTF-32 symbol) to the UTF-8 byte array

### Iteration

- `each_word` - Tokenizes UTF-8 into lower-cased word tokens (uses `utf32_is_word_char` for boundaries, `utf32_to_lower` for case fold).

### Length and measurement

- `utf8_length` - Returns the number of characters in the UTF-8 string

### Validation

- `contains_utf8_bom` - Returns true if the byte array starts with a UTF-8 BOM (byte order mark).
- `is_first_byte_of_utf8_char` - Returns true if the given byte is the first byte of a UTF-8 character.
- `is_utf8_string_valid` - Returns true if the byte array contains a valid UTF-8 encoded string.

### Unicode word/case primitives

- `utf32_is_word_char` - ASCII `[A-Za-z0-9_]` fast path; otherwise consults a non-word range table.
- `utf32_to_lower` - Lowercases a codepoint.

## validate_code

The VALIDATE_CODE module implements AST validation passes that check for common code quality issues, unreachable code, missing return statements, and other semantic errors beyond what the type checker verifies.


### Function annotations

- `verify_completion` - Verify that the function completes without infinite loops or recursion This annotation checks for potential infinite loops and recursive calls within the annotated function.
