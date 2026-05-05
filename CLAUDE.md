# daslang Project Instructions

## Project Overview

This is the [daslang](https://dascript.org/) programming language repository (GaijinEntertainment/daScript). daslang (formerly daScript) is a high-performance statically-typed scripting language designed for games and real-time applications. The language has been renamed to **daslang**, but the repository and many C++ API names still use the old "daScript" spelling.

## What and Why

daslang was created at Gaijin Entertainment to solve a concrete problem: **interop overhead** between scripting languages (Lua/LuaJIT, Squirrel) and C++ was killing frame budgets in their ECS game engine. The key insight is that daslang’s data layout matches C++ — no marshaling, no boxing — making script↔C++ calls near-zero cost.

**Core design principles:**
- **Iteration speed is king** — full production game recompiles in ~5 sec; hot reload built in
- **Explicit, not implicit** — no hidden conversions, no silent allocations; `options log` shows exactly what the compiler produces
- **99% safe, not 100%** — eliminate real-world C++ bugs pragmatically, not at Rust-level cost; `unsafe` for the remaining 1%
- **No data marshaling** — C++-compatible data layout; this is what makes ECS-scale scripting viable
- **If it gets slow, you can fix it** — manual `delete` to reduce GC pressure, AOT compilation for native speed
- **Language reflects the domain** — macros (FORTRAN+LISP inspiration) let libraries reshape syntax to match the problem

**Three execution tiers** (all planned from day one): fast tree-based interpreter → AOT to C++ (required for consoles) → JIT via LLVM. Hybrid mode uses semantic hashing: unchanged functions stay AOT, changed ones fall back to the interpreter.

**Audience:** game scripters (largest group — hot reload, fast compile, never rewrite to C++), engine/tools programmers (zero-cost interop, macros), and a growing standalone/ecosystem community (LLVM executables, package manager in development).

See `doc/source/reference/design_philosophy.rst` for the full design philosophy document.

## Designing with macros — the quantum advantage

Other languages have macros (Rust, C++, Lisp). What daslang combines that they don't is full AST + type access at compile time, gen2 syntax that already reads like the target domain, qmacro/quote sugar for AST construction, and zero-runtime-cost expansion. The mix makes a different class of EDSL viable. Every flagship win in this codebase falls out of that — `[sql_table]`, `_sql`, `linq_boost`, `text_match`, `[sql_index]`, the AOT codegen itself — each one collapses 20+ lines of user-side ceremony into a single declarative form *and* keeps the library implementation smaller than the hand-rolled equivalent. Macros are not a "fancy template" — they are how daslang competes.

**Use macros as a design lens.** Before writing an API or its implementation, ask: *would a macro collapse this?* If the user's call site is mechanical — derivable from a smaller declarative form — the macro is probably the answer. The win compounds: a typed surface saves boilerplate at the call site (`add_column(type<T>, .Field)` vs hand-typed `ALTER TABLE … ADD COLUMN …` strings), and the macro generates the codegen at the library layer. Two boilerplates collapsed at once.

**Don't stop at one meta level.** When a macro itself has mechanical patterns — qmatch templates, repeated AST shape walks, parallel arm-per-form blocks — write a sub-macro. `linq_boost`'s pattern-matchers, dasSQLITE's shared walker, qmacro-built ExprMakeStruct helpers — all macro-on-macro. If macro-on-macro buys an order-of-magnitude reduction in either user code or library code, do it. The compile-time cost is a few ms per expansion; the maintenance cost saved is ongoing.

Read `skills/das_macros.md` before reaching for a hand-rolled solution that a macro would have collapsed.

## Build & Run

CMake-based build, supported on Windows / Linux / macOS / iOS / Android / WASM (CI runs the full matrix). Quick start: `cmake --build build --config Release -j 64`, then run `bin/Release/daslang path/to/script.das` (Windows MSVC layout) or `build/daslang ...` (single-config Make/Ninja). Builds are slow (15-25 min clean, 2-10 min incremental) — always pass `timeout: 0` to long `cmake --build` commands, do NOT assume "no output" means failure.

Full reference (per-platform generator commands, build flags, AOT debugging, exit code meanings, runtime crash diagnostics): **`skills/build_and_debug.md`**.

## GitHub Operations

- **Use GitHub MCP tools** (`mcp__github__*`) for all GitHub operations (creating PRs, listing issues, reading PRs, etc.) — they avoid shell escaping issues entirely
- **Fallback:** If MCP tools are unavailable, use `gh` CLI with `--body-file` for any text containing backticks (they're shell escape characters in every supported shell)

## Skill Files (REQUIRED)

Task-specific instructions are split into skill files under `skills/`. You MUST read the relevant skill file(s) before performing the corresponding task.

| Skill file | Read BEFORE... |
|---|---|
| `skills/build_and_debug.md` | Build flags, AOT build commands, exit-code/crash diagnosis, `options log_infer_passes` |
| `skills/mcp_tools.md` | Full MCP tool table + live-API reference |
| `skills/das_formatting.md` | Creating or modifying any `.das` file |
| `skills/writing_tests.md` | Writing or editing test files under `tests/` |
| `skills/writing_cpp_tests.md` | Writing or editing C++ tests under `tests-cpp/` (doctest, leak guards, ctest wiring) |
| `skills/documentation_rst.md` | Editing RST in `doc/source/`, `//!` doc-comments in `daslib/*.das`, tutorial RST pages |
| `skills/tutorials.md` | Anything that looks like a tutorial — they live under `/tutorials/<area>/`, NEVER `modules/<X>/tutorial/` |
| `skills/cpp_integration.md` | Writing/editing C++ in `src/`, `modules/`, `tutorials/integration/cpp/` |
| `skills/daslib_modules.md` | Working with `daslib/` modules or extending the stdlib |
| `skills/das_macros.md` | Compile-time macros, AST manipulation, qmacro/quote, gc_node patterns |
| `skills/writing_benchmarks.md` | Writing/running `benchmarks/` files |
| `skills/daspkg.md` | Running daspkg commands, `.das_package` manifests |
| `skills/dynamic_modules.md` | `.das_module` descriptors, adding modules under `modules/` |
| `skills/install_instructions.md` | Updating `install/CLAUDE.md` or `install/skills/` for the shipped SDK |
| `skills/aot_testing.md` | AOT test files, `test_aot` binary, `Module::aotRequire()`, AOT hash mismatches |
| `skills/visitor_gen_bind.md` | Adding `Visitor` virtual methods / `canVisit*` gates / `gen_bind.das` regen |
| `skills/daslang_live.md` | `daslang-live`, live-reload lifecycle, `[live_command]`, `[before_reload]`/`[after_reload]` |
| `skills/perf_lint.md` | Adding rules to `daslib/perf_lint.das` |
| `skills/style_lint.md` | Adding rules to `daslib/style_lint.das` |
| `skills/strings.md` | Any `.das` string operation — `find`/`replace`/`split`/parsing/`build_string`/`peek_data` (covers `strings`, `daslib/strings_boost`, `daslib/strings_convert`) |
| `skills/regex.md` | Writing regular expressions in `.das` code |
| `skills/glob.md` | Writing or reviewing any glob/wildcard pattern handling — file selection, include/exclude masks, pattern-match-on-paths (`*` / `?` / `**` / `[abc]`) |
| `skills/gc_migration.md` | Migrating external/archived code from `smart_ptr<T>` AST patterns to gc_node (in-tree migration is complete) |
| `skills/version_update.md` | Bumping the daslang version number |
| `skills/jobque_debugging.md` | Channel/LockBox/JobStatus/Feature leaks (`--track-job-status`, `DumpJobQueLeaks`) |
| `skills/make_pr.md` | Creating a pull request (lint, test, AOT, format checklist) |
| `skills/pr_review_iteration.md` | Working an open PR through CI failures and Copilot/human review feedback after the PR is created |
| `skills/strudel_port.md` | Porting strudel.cc patterns into daslang |
| `skills/gc_use_after_sweep.md` | Debugging `bad_alloc` / `length_error` in TypeDecl/Expression copy-ctors (use-after-sweep) |
| `skills/clargs_usage.md` | Writing or editing any tool that parses command-line flags — declarative argv parsing via `daslib/clargs` |
| `skills/clargs_migration.md` | Editing any tool that still parses `get_command_line_arguments()` directly — migrate to `daslib/clargs` in the same PR |
| `skills/json.md` | Reading/writing JSON in `.das` code (`sprint_json`/`sscan_json`, `JV`, manual `JsonValue?`) |
| `skills/xml.md` | XML via `dasPUGIXML`/`PUGIXML_boost` (RAII parsing, builder, XPath, struct round-trip) |
| `skills/filesystem.md` | Any `.das` path/filename/filesystem op — must use `fio` helpers, never `rfind`/`slice` |
| `skills/detect_dupe.md` | Duplicate-function detection (corpus build, MCP tools `export_corpus`/`detect_duplicates`, CLI under `utils/detect-dupe/`) |
| `skills/find_dupe.md` | AI-judging a detect-dupe report via Claude (MCP tools `judge_duplicates`/`find_dupe`, CLI under `utils/find-dupe/`); cost guardrails (`--dry-run`, `--max-clusters`, `--positives-only`) |
| `skills/linq.md` | Filter/map/sort/group/aggregate transforms — preference order: comprehension → linq_boost → plain `for`. Avoid `daslib/functional` for new code |
| `skills/decs.md` | Programming with `daslib/decs` / `decs_boost` — entities, components, queries, `[decs_template]`, stages, bulk creation, `from_decs` linq bridge |
| `skills/aot_hash_desync_debugging.md` | `error[50101]: AOT link failed` — semantic-hash desync diagnostics |

Multiple skill files may apply to a single task. For example, creating a new daslib module requires reading `skills/das_formatting.md`, `skills/daslib_modules.md`, and possibly `skills/documentation_rst.md`.

**Formatter reminder:** Use the MCP `format_file` tool to format `.das` files. It calls `daslib/das_source_formatter` directly. Do NOT use `utils/dasFormatter/` (that is the v1→v2 syntax converter, not a code formatter).

### Updating Instructions with New Knowledge

When you discover something new about daslang syntax, semantics, or conventions — whether through compiler errors, user corrections, or experimentation — **update this file** with the new knowledge. If it relates to a specific skill area, update the relevant `skills/*.md` file instead.

**Doc improvements at stopping points.** When a task wraps and you spot a typo or factual error in CLAUDE.md or `skills/*.md` — fix it in-place and flag the edit in the end-of-turn summary. Anything more — clarifications, additions, restructuring, removing existing guidance, **or proposing a new skill file when you see a recurring pattern that no existing skill covers** — propose first. Default toward propose-first; doc edits direct future Claude behavior and silent diffs are not OK.

## daslang Language — Gen2 Syntax (REQUIRED)

All code MUST use gen2 syntax (add `options gen2` at the top of every file). Key rules:

- **Parentheses** on control flow: `if (x > 0)`, `for (i in range(10))`, `while (running)`
- **Braces** on all blocks: `def foo() { ... }`, `if (x) { ... }`
- **Construction:** `new Type(field=val)` — NOT `new [[Type() field=val]]`
- **Enum access:** `EnumName.EnumValue` with dot — NOT `EnumName EnumValue`
- **Array literals:** `[1, 2, 3]` — NOT `[[int 1; 2; 3]]`. Creates `array<int>`; use `fixed_array(1, 2, 3)` for fixed-size
- **Struct init:** `Foo(a=1, b=2)` — NOT `[[Foo() a=1, b=2]]`. Move-init: `Foo(a=1, b <- expr)` for non-copyable fields
- **Table literals:** `{ "k" => v, "k2" => v2 }` — NOT `{{ "k" => v; "k2" => v2 }}`
- **Bare blocks:** `{ var x = 1; ... }` at statement level creates a lexical scope (NOT a table literal). Supports `finally`: `{ ... } finally { ... }`
- **Named arguments:** `foo([name = value])` with square brackets
- **Block arguments:** block/lambda after `func()` pipes as last arg. No `$` for parameterless blocks: `defer() { ... }`. With params: `build_string() $(var writer) { ... }`. Lambdas: `emplace() @(x : int) { ... }`
- **Lambda:** `@(args) { body }` or `@@(args) { body }` (no-capture). **Inline arrow form:** `@(x) => expr` (capture lambda) and `@@(x) => expr` (no-capture function pointer) — preferred for short transforms passed as arguments: `sometimes(pat, @@(x) => fast(x, 2.0lf))`
- **Generator:** `$() { yield value; }` or `$ { yield value; }`
- **Tuple `=>`:** `a => b` creates `tuple<auto;auto>`
- **`typeinfo`:** `typeinfo trait_name(type<T>)` — trait name outside parens
- **`static_if`:** `static_if (condition) { ... }` — parentheses required
- **Type function call:** `take(type<int>, 1, 2)` — NOT `take < int > (1, 2)`
- **Newlines inside `(...)`, `[...]`, `{...}` are free** — long pipe chains, multi-arg calls, array/table literals can wrap freely. Statement-level (no surrounding bracket) still requires one statement per line, so wrap the RHS in `(...)` if a `let x = a |> b |> c` needs to break across lines
- **Inline literals over temp-var-and-push** — for short arrays consumed in one expression, write `stack([a, b, c])` rather than `var xs : array<T>; xs |> emplace(a); xs |> emplace(b); stack(xs)`. Faster in interpreted mode and easier to read; same applies to table literals and other bracketed constructors. Threshold: while it stays readable

### Type modifiers

- **`==const`** on a parameter type — propagates the caller's constness (NOT "always non-const"): `def foo(self : MyStruct ==const)` accepts either `MyStruct` or `MyStruct const`, and inside the body `self`'s constness matches what the caller passed. Use plain `Foo?` for non-const-only, `Foo const?` for const-only, `Foo? ==const` when you want the callee to accept either and inherit the caller's view
- **`-const`** strips constness in type expressions — used with `reinterpret` for interior mutability: `unsafe(reinterpret<MyStruct? -const>(addr(self)))`
- **Function pointer with explicit type:** `@@<(var self : T) : RetT> funcName` — specifies the exact parameter/return types of a function pointer literal

### Important defaults

- No implicit type promotion: `int + float` is a compile error — both sides must match
- No `bool(int)` cast — use `x != 0`; no `string(bool)` — use `"{flag}"`
- `int("123")` does NOT work — use `to_int` from `require strings`. **`to_int` silently returns `0` on garbage** (`to_int("foo")` → `0`, `to_int("12abc")` → `12`). When you need to validate user/external input — including any string that flows into a shell command, file path, or system call — use `try_to_int` / `try_to_float` from `daslib/strings_convert` instead. Those return `Result<T; ConversionError>` distinguishing `invalid_argument` / `out_of_range` / `trailing_garbage`, so `";rm -rf;"` rejects cleanly instead of becoming `0`. Same for `to_float` → `try_to_float`
- Hex literals are `uint` by default — use `int(0x3F)` for int
- **`default<T>`** — the zero value of `T`. Body of the called function CAN use it.
- **`type<T>`** vs **`default<T>`** as a witness argument: `type<T>` is a no-stack tag (compile error if body reads it; annotate `[unused_argument(t)]`); `default<T>` is a real zero value (body can read/pass it). Pick by whether the body needs to touch the param. If you want to read a `type<T>` param, switch the caller to `default<T>` — don't rewrite the function.
- **`typedecl(expr)`** — compile-time type-of, usable inside `default<>`: `default<typedecl(field)>`.
- **Bitfields**: `bitfield Name : uint8 { ... }` (also `uint16`/`uint64`; default `uint`, always unsigned). From an int: `bitfield64(1ul << 13ul)` (also `bitfield8`/`bitfield16`).

### Pass-by-value vs pass-by-reference

- Structs/arrays/tables always pass by reference — no `&` needed.
- Only **workhorse types** (`int`, `float`, `bool`, `string`, …, `isWorkhorseType` on the C++ side) pass by value.
- **AST pointers (gc_node) pass by value** — copying the pointer, no refcount, no allocation. `def foo(p : ExpressionPtr)` shares the node; `var p` lets you reassign locally; `var p : ExpressionPtr&` propagates reassignment back. For mutable field access, take the param as `var`.
- **Strings:** `var s : string` is a writable local copy (no propagation). `var s : string&` propagates. `:=` clones into current context's heap (required across contexts); plain `=` copies the pointer.
- **Residual `smart_ptr` types** (`ProgramPtr`, `ContextPtr`, `FileAccessPtr`, `DebugAgentPtr`, `VisitorAdapterPtr`) still use refcount semantics — variables holding them need `var inscope`. AST types do NOT — see below.

### AST nodes (gc_node) and memory

AST types (TypeDecl, Expression, Function, Structure, Enumeration, Variable, MakeFieldDecl, MakeStruct, every `Annotation` subclass) are plain raw pointers tracked by gc_node. The only types still using `smart_ptr` are `Program`, `Context`, `FileAccess`, plus a couple of internal helpers (`DebugAgentPtr`, `VisitorAdapterPtr`).

Quick rules:
- AST nodes have **unique ownership** — don't insert the same pointer into two parents; use `clone_type` / `clone_expression` / `clone_function` / `clone_variable` / `clone_structure` to duplicate.
- AST pointers use plain `=` and pass-by-value. **No `var inscope`, no `<-`** for them. `var inscope` is only for the residual smart_ptr types.
- Tools that build AST at runtime (outside the compile pipeline) must wrap their scope in `ast_gc_guard() { ... }` from `daslib/ast`, or the leak detector reports `GC APP LEAK` at exit.
- daslang has garbage collection, but plain `var arr : array<T>` does NOT finalize on scope exit. Either declare with `var inscope` (smart_ptr only), call `delete` explicitly, or move out via `<-`. Per-frame leaks in hot paths usually trace to a local `var arr` never deleted.

Full migration table (when reading older docs that say `var inscope` or `<-` for AST types): **`skills/gc_migration.md`**.

### Context heaps and threading

- **`new Foo()` allocates on the current context's heap** — each context has its own heap
- **Contexts cannot retain data from other contexts** — only copy. A pointer into context A's heap is invalid in context B
- **Threads run in separate contexts** — `new_thread() <| @{ ... }` creates a new context. Data must be copied/cloned when crossing thread boundaries
- `clone_string(s)` clones a string into the current context's heap — required when passing strings across contexts
- `:=` on strings does a clone (new allocation in current context); `=` copies the pointer (unsafe across contexts)
- **Channel data**: when sending data through channels, the receiving context gets a temporary reference (`#`) — clone/copy what you need before the callback returns
- **Implication for threaded audio**: parsed data (arrays, structs) created on the main thread cannot be referenced by pointer from the audio thread. Either clone into the audio thread's context, or use C++-side shared memory that lives outside any daScript context

### Unsafe

- **`unsafe(expr)`** — narrow-scope unsafe, preferred over `unsafe { block }`. Limits unsafe to the exact expression that needs it
- **Local reference binding is unsafe:** `let blk & = expr` requires `unsafe` whenever it creates a local reference to a non-local expression — `let blk & = unsafe(expr)`
- **Variant `as` read access is safe:** `(v as _field).member` works without `unsafe` after an `is` check
- **Variant field assignment is always unsafe:** `v._field = value` and `set_variant_index(v, N)` require `unsafe`
- **`reinterpret<T>(expr)`** requires `unsafe` — used for const-stripping on regular pointers: `unsafe(reinterpret<Foo?>(const_ptr))`

### Error handling

- `try/recover` — NOT `try/catch` (`recover` is the keyword)
- `panic("message")`, `assert(condition)`, `verify(condition)` (stays in release)
- **Postfix conditional:** `return expr if (cond)`, `break if (cond)`, `continue if (cond)` — early-exit guard on one line

### Generic function dispatch

- **`_::foo(x)`**: resolves in the **calling** module — caller’s overloads visible. Use in library generics.
- **Unqualified** `foo(x)`: resolves in the **defining** module — caller’s overloads NOT visible.
- This is why `:=` and `delete` emit `_::clone` / `_::finalize`

### Dot as pseudo-pipe

`a.foo(b)` is sugar for `foo(a, b)` — but **only when `a` is a struct/class value** (chains: `a.foo().bar(x)` ≡ `bar(foo(a), x)`).

- **Works on:** struct / class values (incl. by-ref).
- **Does NOT work on:** primitives (`let n = 5; n.double()` → "can't get field 'double' of int const&"), tuples/arrays, and **lambda typedefs** — most importantly strudel's `Pattern` type (`typedef Pattern = lambda<...>`); `s("bd").fast(2.0lf)` fails. Pattern chains must use `|>` (or direct call).
- **When telling someone "use pipe here":** check the receiver type — for structs `.method()` is idiomatic, for lambdas only `|>` works. Don't say "daslang uses pipes instead of method chains" without qualification.

### Table operations

- `table[key]` **inserts** a default entry if missing — use `table?[key] ?? default` for safe lookup
- `key_exists(table, key)` — check without inserting
- `table |> insert(key, value)` / `table |> erase(key)`
- **Never use two `[]` lookups on the same table in one expression** — re-hashing can invalidate references
- `table[key]` (read or assign) is **safe** — do NOT wrap in `unsafe(...)`. Some legacy daslib code has `unsafe(tab[k])`; do not propagate that pattern
- **Move-assign table literal:** `tab <- { "k" => v }` works for both `var tab <- { ... }` declarations and `tab <- { ... }` reassignment to existing variables
- **Table comprehension move-assign:** `tab <- { for(x in range(5)); x => x*x }` — same move-assign rules apply

### Iterators and `each`

- `[unsafe_outside_of_for] def each(x) : iterator<T>` makes a type iterable in `for` loops
- When the iterator is named `each`, the call can be omitted: `for (v in each(x))` is identical to `for (v in x)`
- Other iterator names (e.g. `filter`, `map`) cannot be omitted

### String access functions

- **`peek_data(str) $(arr) { ... }`** — safe O(1) per-element read access to string as `array<uint8> const#`. One `strlen` call total. Preferred over `character_at` for iteration.
- **`modify_data(str) $(var arr) { ... }`** — returns a modified copy; allocates new string, opens as mutable `array<uint8>`. Use for character-level transformations.
- **`character_at(s, i)`** — O(n) per call (`strlen` + bounds check). Fine for isolated checks, but use `peek_data` in loops or hot paths.
- Pointer-based string access (`reinterpret<uint8?>`) is for core library implementations only — user code should use `peek_data`/`modify_data` for safety.

### Common gotchas

- Lambda params can shadow function params — use distinct names
- String builder requires `unsafe` or `options persistent_heap` if returned
- Tuple field access: `t._0`, `t._1`, `t._2`
- Annotations: `[export]`, `[test]`; `options no_aot`, `options rtti`
- **Visibility is a prefix keyword, not an annotation:** `def private foo()`, `struct private Foo { ... }`, `enum private E { ... }`, `variable private x = 0`, `alias private X = Y`. There is **no** `[private]` annotation — it's a grammar error
- **Field/variable annotations use `@name` only:** `@safe_when_uninitialized at : LineInfo`, `@sql_primary_key id : int64`, `@do_not_delete ctx : Context?`. The `[name]` form is reserved for struct/function/global-level annotations and does NOT parse on a struct field
- `require` uses forward slash: `require daslib/linq` — NOT backslash
- `require foo public` — re-exports `foo` transitively
- `[export] def main()` defaults to returning `void`, but you can declare it as `def main() : int { ... return rc }` when you need to surface a non-zero process exit code (e.g. CLI tools where callers — MCP wrappers, CI, parent shells — branch on exit). See `dastest/dastest.das` for the canonical pattern. Don't reach for `panic` just to force a non-zero exit; declare `: int` and `return rc` instead.
- `push` copies (fails for non-copyable types), `emplace` moves (zeros source), `push_clone` clones (preserves source)
- Non-copyable types (`array<T>`, `table<K;V>`, lambdas): use `:=`, `push_clone`, or `<-`
- Blocks cannot be stored/returned/captured — use lambdas or function pointers
- Class methods: `def const`, `def abstract const`, `def static`; call syntax `obj.method()`, `obj->method()`, `obj |> method()`
- **`is`/`as` on handled types checks EXACT type**, not C++ inheritance — `expr is ExprField` is `false` when `expr` is `ExprSafeField`. `as` on wrong type crashes. Must handle each concrete type explicitly.
- `#pragma optimize` in AOT-generated code must be wrapped in `#ifdef _MSC_VER` — Clang warns on unknown pragmas
- **Macro-generated struct variables** need `default<$t(st)>` initialization (not `var x : $t(st)`) — avoids "uninitialized variable" errors for structs without field defaults
- `print` is for user-facing scripts only. In `tests/`, `daslib/`, `utils/`: use `to_log(LOG_INFO|LOG_WARNING|LOG_ERROR)` — same stdout, but level-tagged and filterable. Canonical example: `utils/detect-dupe/main.das`

### Code style — prefer idiomatic forms

| Don't write | Write instead | Why |
|---|---|---|
| `string(x.__rtti) == "ExprFoo"` | `x is ExprFoo` | `is` works directly on AST pointers |
| `get_ptr(x) == null` / `get_ptr(x).field` | `x == null` / `x.field` | AST pointers auto-dereference; `get_ptr` is smart_ptr-era residue |
| `string(das_str) == "lit"`, `!empty(string(das_str))` | drop the `string(...)` cast | `das_string` compares with `string` directly; `empty()` works on it |
| `let v = string(x.name); $i(v)` / `var copy = val; $v(copy)` | `$i(x.name)` / `$v(val)` | qmacro tags accept `das_string`, `let` vars, loop vars directly |
| 6 qmacro arms differing only in the call target (`if isTry { qmacro(_::try_run_select(…)) } elif … { … }`) | `let fname = (isTry ? "try_run_select" : "run_select") + suffix; qmacro($c(fname)(…))` | `$c(stringVar)` splices a function name; resolution at splice site uses user's `require` chain. Note: `_::$c(…)` is a parse error — drop `_::` |
| `if (true) { ... }` | `{ ... }` | bare blocks create lexical scope in gen2 |
| `var inscope r <- expr; return <- r` | `return <- expr` | direct return avoids intermediate |
| `unsafe { (reinterpret<ExprBlock?> blk).list }` / `unsafe(reinterpret<T?> x)` | make param `var` + plain `x.list` | `var` param gives non-const field access without reinterpret |

For path/filename ops use `fio` helpers (`base_name`/`dir_name`/`path_join`/etc.) — see `skills/filesystem.md`. Never hand-roll `rfind("/")` / slice — misses Windows separators.

**Minimize `unsafe`:** Most `unsafe(reinterpret<T?>)` in macro code exists to strip `const` from raw-pointer field access. Fix the root cause: make the function parameter `var` so field access returns non-const pointers. Reserve `unsafe` for genuinely unsafe operations (pointer arithmetic, `reinterpret` across unrelated types).

**Comment hygiene.** Comments are 1–2 lines max. Strict rules:

1. **No banner comments above a documented function.** When a function carries `//!` inside its body, drop the `// ===== name — desc =====` block above. The banner duplicates the doc.
2. **No multi-paragraph architectural prose at the head of a section.** Don't write 10–30 line preambles explaining design decisions, surface examples, NULL handling, panic semantics, etc. above `// Section name`. Code reads well; design docs (plans, `API_REWORK.md`, RST tutorials) carry the WHY. If a reader genuinely needs that context, it goes in those docs, not the source.
3. **Private functions and types don't get public-style docs.** `//!` / `//!<` is for tooling-visible public API. On `def private`, `struct private`, `enum private`, `variant private`, drop the docstring entirely — the symbol isn't exported, so no doc generator ever sees it, and the docstring just restates the function name / field name to a reader who already has them. If a function or field genuinely needs a 1-line WHY (non-obvious invariant, surprising behavior), write a plain `// ...` line, not `//!`. The bar for keeping any comment on a private symbol is "a maintainer reading the symbol alone would be surprised."
4. **What stays:** terse 2-line section dividers (`// ===== Section name =====`), `//!` docstrings on PUBLIC functions/types (visible to tooling), and inline `//` comments that flag a non-obvious WHY at the *exact* line — a workaround for a specific bug, a subtle invariant, behavior that would surprise a reader. Don't restate what the code says.
5. **When in doubt:** delete. If reading the code + the relevant docstring(s) doesn't make the WHY clear, the comment was load-bearing. Otherwise it was noise.

## Key Directories

Most layout is obvious from `ls`. Non-obvious ones worth knowing:

- `daslib/aot_cpp.das` — AOT C++ emitter lives here, NOT in the (emptied) `src/ast/ast_aot_cpp.cpp`
- `tests/aot/CMakeLists.txt` — register new test directories here for AOT compilation
- `dastest/` — test framework (used by both `tests/` and external repos)
- `utils/detect-dupe/` (in-repo dupe finder) and `utils/find-dupe/` (Claude-based judge that needs `daspkg install --root utils/find-dupe` + `ANTHROPIC_API_KEY`) — both also exposed as MCP tools
- `utils/mcp/`, `utils/daslang-live/`, `utils/daspkg/` — in-tree tools (most also have skills under `skills/`)
- `tutorials/language/` (language tour) vs `tutorials/<area>/` (per-area) — never put tutorials in `modules/<X>/tutorial/`

## MCP Server (AI Tool Integration)

The daslang MCP server (`utils/mcp/main.das`) exposes compiler diagnostics, program introspection, and live-reload control. **Prefer MCP tools** over manual compilation and grep — `grep_usage` is parse-aware (tree-sitter), `find_references` resolves cross-module symbols, and `live_*` tools talk to `daslang-live` directly instead of curl.

Full tool table (including `detect_duplicates`/`judge_duplicates`/`find_dupe`), live-API caveats, and `.mcp.json` configuration: **`skills/mcp_tools.md`**.
