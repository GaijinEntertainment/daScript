# daslang Project Instructions

## Project Overview

This is the [daslang](https://daslang.io/) programming language repository (GaijinEntertainment/daScript). daslang (formerly daScript) is a high-performance statically-typed scripting language for games and real-time applications. The repository and many C++ API names still use the old "daScript" spelling.

For the **why** — design principles, three-tier execution model, the macros-as-design-lens rule — read `skills/project_overview.md`. The full long-form rationale lives in `doc/source/reference/design_philosophy.rst`.

## Build & Run

CMake-based build, supported on Windows / Linux / macOS / iOS / Android / WASM (CI runs the full matrix). Quick start: `cmake --build build --config Release -j 64`, then run `bin/Release/daslang path/to/script.das` (Windows MSVC layout) or `build/daslang ...` (single-config Make/Ninja). Builds are slow (15-25 min clean, 2-10 min incremental) — always pass `timeout: 0` to long `cmake --build` commands, do NOT assume "no output" means failure.

Full reference (per-platform generator commands, build flags, AOT debugging, exit code meanings, runtime crash diagnostics): **`skills/build_and_debug.md`**.

## Workspace Hygiene

Clean up files created for diagnostics, staging, and one-off tests before handing work back. This
includes temporary release trees, formatter backups, extracted audio, generated `__pycache__`
directories, and ad-hoc stdout/stderr logs. Keep intentional application logs, databases, configs,
rollback bundles, and user-owned artifacts. In deployed application directories, do not leave
unrelated diagnostic files beside the program.

## GitHub Operations

- **Use GitHub MCP tools** (`mcp__github__*`) for all GitHub operations (creating PRs, listing issues, reading PRs, etc.) — they avoid shell escaping issues entirely
- **Fallback:** If MCP tools are unavailable, use `gh` CLI with `--body-file` for any text containing backticks (they're shell escape characters in every supported shell)

## MCP-first search

Before reaching for `Bash`/`Grep`/`Read` to find a symbol or trace usages in this repo, ToolSearch the daslang MCP tool that answers the question and call it. The tools are deferred (schemas not in turn-start context) so the workflow is two calls: `ToolSearch select:mcp__daslang__<tool>` → invoke. (`defer_loading: false` in `.mcp.json` is the documented knob to flip this, but [it's currently broken upstream](https://github.com/anthropics/claude-code/issues/26844) — set it anyway, then operate as if deferred.)

| Question | Tool |
|---|---|
| Where is symbol X defined? (`.das`) | `find_symbol` — pass `with_cpp_source=true` for builtins / handled types to bridge daslang→C++ in one call |
| Where is symbol X defined? (`.cpp`) | `cpp_find_symbol` |
| Where is X used? (`.das`) | `grep_usage` |
| Where is X used? (`.cpp`) | `cpp_grep_usage` |
| What's in this file? | `outline` (`.das`) / `cpp_outline` (`.cpp`) |
| Goto definition at cursor | `goto_definition` (`.das`) / `cpp_goto_definition` (`.cpp`) |
| All references to X (`.das`) | `find_references` |

Same applies to lint/format: `mcp__daslang__lint` / `format_file`, not shell `bin/daslang` invocations. Same applies to GitHub operations (see above): `mcp__github__*`, not `gh` shell.

Fall back to `Bash`/`Grep`/`Read` only when the MCP tool reports an error or the question is genuinely outside MCP coverage (RST prose, CMake, Python tooling).

**Cross-tree guard (#3392):** when you work in a git worktree, MCP tool results and LSP diagnostics prepend a `CROSS-TREE WARNING` if you query a file **outside the server's own tree** — that answer used the *wrong* tree's module sources AND that binary's compiled-in C++ bindings, so it's likely stale (missing symbols, outdated signatures). Heed it, don't trust cross-tree output: run the session *inside* the worktree (its checked-in `.claude/skills/daslang-lsp` gives correct diagnostics; `daslang utils/mcp/setup.das -- --root <worktree>` bootstraps its MCP tools). A main-tree-rooted session can never give correct diagnostics for a worktree file.

## Skill Files (REQUIRED)

Task-specific instructions are split into skill files under `skills/`. You MUST read the relevant skill file(s) before performing the corresponding task.

| Skill file | Read BEFORE... |
|---|---|
| `skills/project_overview.md` | First significant task — design philosophy, three execution tiers, macros-as-design-lens |
| `skills/build_and_debug.md` | Build flags, AOT build commands, exit-code/crash diagnosis, `options log_infer_passes` |
| `skills/mcp_tools.md` | Full MCP tool table + live-API reference |
| `skills/das_formatting.md` | Creating or modifying any `.das` file |
| `skills/writing_tests.md` | Writing or editing test files under `tests/` |
| `skills/writing_cpp_tests.md` | Writing or editing C++ tests under `tests-cpp/` (doctest, leak guards, ctest wiring) |
| `skills/documentation_rst.md` | Editing RST in `doc/source/`, `//!` doc-comments in `daslib/*.das`, tutorial RST pages |
| `skills/tutorials.md` | Anything that looks like a tutorial — they live under `/tutorials/<area>/`, NEVER `modules/<X>/tutorial/` |
| `skills/cpp_integration.md` | Writing/editing C++ in `src/`, `modules/`, `tutorials/integration/cpp/` |
| `skills/clang_bind_build.md` | Enabling the `dasClangBind` module / installing or bumping the libclang SDK / running any `bind_*.das` self-binder (`bind_clangbind`, `bind_imgui`, …) to regenerate native bindings |
| `skills/daslib_modules.md` | Working with `daslib/` modules or extending the stdlib |
| `skills/das_macros.md` | Compile-time macros, AST manipulation, qmacro/quote, gc_node patterns |
| `skills/writing_benchmarks.md` | Writing/running `benchmarks/` files |
| `skills/daspkg.md` | Running daspkg commands, `.das_package` manifests |
| `skills/dynamic_modules.md` | `.das_module` descriptors, adding modules under `modules/` |
| `skills/external_module_debugging.md` | Working on an external daslang module (dasImgui, dasPUGIXML, dasSQLITE, etc.) locally — need to run/lint/test from a standalone daslang.exe or via MCP before push-to-CI. Covers the `<DummyRoot>/modules/<your-module>` junction pattern + `project_root` MCP arg |
| `skills/install_instructions.md` | Updating `install/CLAUDE.md` or `install/skills/` for the shipped SDK |
| `skills/aot_testing.md` | AOT test files, `test_aot` binary, `Module::aotRequire()`, AOT hash mismatches |
| `skills/llvm_tune.md` | The `[tune]` kernel-tuning framework — `[tune_perm]`/`[tune_scope]`/`[tune_policy]`/`--tune`, per-box manifests, the runtime-tune-and-re-exec model, the AOT/-exe gates, adding a kernel family (`modules/dasLLVM/daslib/llvm_tune.das`) |
| `skills/visitor_gen_bind.md` | Adding `Visitor` virtual methods / `canVisit*` gates / `gen_bind.das` regen |
| `skills/daslang_live.md` | `daslang-live`, live-reload lifecycle, `[live_command]`, `[before_reload]`/`[after_reload]` |
| `skills/daslang_lsp.md` | Working on `utils/lsp/` (the LSP server for Claude Code / stdio clients) — locked architecture, coordinate conventions, CC wire facts, headless dev rig, protocol tests |
| `skills/imgui_ui_debugging.md` | **CRITICAL UI SKILL** — diagnosing/fixing ANY dasImgui UI or interaction bug. The discipline: reproduce + screenshot → make it observable in `imgui_snapshot` (fix the inspection if it isn't) → fix → prove via snapshot + test → 'after' screenshot. UI is hard; **never claim a UI fix works from logic or a screenshot — only from structured snapshot state.** |
| `skills/perf_lint.md` | Adding rules to `daslib/perf_lint.das` |
| `skills/style_lint.md` | Adding rules to `daslib/style_lint.das` |
| `skills/strings.md` | Any `.das` string operation — `find`/`replace`/`split`/parsing/`build_string`/`peek_data` (covers `strings`, `daslib/strings_boost`, `daslib/strings_convert`) |
| `skills/regex.md` | Writing regular expressions in `.das` code |
| `skills/glob.md` | Writing or reviewing any glob/wildcard pattern handling — file selection, include/exclude masks, pattern-match-on-paths (`*` / `?` / `**` / `[abc]`) |
| `skills/gc_migration.md` | Migrating external/archived code from `smart_ptr<T>` AST patterns to gc_node (in-tree migration is complete) |
| `skills/version_update.md` | Bumping the daslang version number |
| `skills/jobque_debugging.md` | Channel/LockBox/JobStatus/Feature leaks (`--track-job-status`, `DumpJobQueLeaks`) |
| `skills/memory_leak_detection.md` | Any leak report at exit — master index of all six leak-detection mechanisms (gc_node, `--das-profiler-leaks`, `-track-allocations`, smart_ptr tracking, jobque, HandleRegistry) and which to reach for |
| `skills/make_pr.md` | Creating a pull request (lint, test, AOT, format checklist) |
| `skills/preflight.md` | Pushing a non-trivial branch or reproducing a red CI lane — maps every PR-triggered CI lane to its exact local mirror command (or an honest "not mirrorable") |
| `skills/abi_break_sweep.md` | Changing public C++ API, AST node layout, or daslib generic signatures that external module repos compile against — both-worlds spellings, externals-merge-first ordering, daspkg-index scope |
| `skills/wsl_ci_repro.md` | Reproducing a Linux-only CI failure (sanitizers, POSIX divergence, headless timing) in the WSL CI-mirror distro — verbatim-CI recipe and its traps |
| `skills/babysit.md` | Babysitting an open PR through CI failures and Copilot/human review feedback after the PR is created (the post-open loop) |
| `skills/strudel_port.md` | Porting strudel.cc patterns into daslang |
| `skills/clargs_usage.md` | Writing or editing any tool that parses command-line flags — declarative argv parsing via `daslib/clargs`, plus migration discipline for legacy `get_command_line_arguments()` callers |
| `skills/json.md` | Reading/writing JSON in `.das` code (`sprint_json`/`sscan_json`, `JV`, manual `JsonValue?`) |
| `skills/xml.md` | XML via `dasPUGIXML`/`PUGIXML_boost` (RAII parsing, builder, XPath, struct round-trip) |
| `skills/sql.md` | SQL via `daslib/sql_linq` + providers (`dasSQLITE` / `dasDuckDB` / `dasPostgreSQL`) — `[sql_table]` / `[sql_view]` / `[sql_fts5]` / `[sql_function]`, the `_sql(...)` LINQ-to-SQL flagship + `_each_sql` / `_sql_update` / `_sql_delete` / `_sql_upsert` / `_create_view` / `_distinct_by`, per-provider capability gates, custom-type adapter rail, `@sql_json` / `@sql_blob` columns, transactions, migrations (`[sql_migration]` + `with_latest_sqlite`, SQLite-only) |
| `skills/dashv.md` | Any `.das` that serves HTTP, streams SSE, makes outbound HTTP requests, or tests a server — `HvWebServer` (routes `GET`/`POST`/`STREAM`/…), buffered vs streaming responses, handle-first SSE writer ops, the client API (`GET`/`POST`/`request`/`with_http_request`/`get_body_bytes`), the `with_test_server` harness, and the per-context-module-global / `options stack` gotchas. Worked example: `utils/dasllama-server/` |
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

**Syntax and factual corrections are fix-in-place, always.** If a compiler error, probe, or user correction shows that a claim in CLAUDE.md or `skills/*.md` is wrong, incomplete, or stale, fix it in the same session and flag the edit in the end-of-turn summary — never defer it to a proposal. Verify the corrected claim before writing it (grammar truth is `src/parser/ds2_parser.ypp`; behavior truth is a probe-compile with the current binary).

**Doc improvements at stopping points.** Propose-first applies only to what's left: restructuring, removing existing guidance, **or proposing a new skill file when you see a recurring pattern that no existing skill covers**. Doc edits direct future Claude behavior, so structural diffs still get review — but factual drift must be self-healing, not queued behind it.

## daslang Language — Gen2 Syntax (REQUIRED)

**gen2 is the DEFAULT parser** — every `.das` file parses as gen2 unless it explicitly opts out with `options gen2 = false` (the only gen1 discriminator; the `options gen2` markers around the tree are historical no-ops — NEVER infer gen1 from their absence). All code MUST use gen2 syntax; house style still adds `options gen2` at the top of new files. Key rules:

- **Parentheses** on control flow: `if (x > 0)`, `for (i in range(10))`, `while (running)`
- **Braces** on all blocks: `def foo() { ... }`, `if (x) { ... }`
- **Construction:** `new Type(field=val)` — NOT `new [[Type() field=val]]`
- **Enum access:** `EnumName.EnumValue` with dot — NOT `EnumName EnumValue`
- **Array literals:** `[1, 2, 3]` — NOT `[[int 1; 2; 3]]`. Creates `array<int>`; use `fixed_array(1, 2, 3)` for fixed-size
- **Struct init:** `Foo(a=1, b=2)` — NOT `[[Foo() a=1, b=2]]`. Move-init: `Foo(a=1, b <- expr)` for non-copyable fields
- **Table literals:** `{ "k" => v, "k2" => v2 }` — NOT `{{ "k" => v; "k2" => v2 }}`
- **Bare blocks:** `{ var x = 1; ... }` at statement level creates a lexical scope (NOT a table literal). Supports `finally`: `{ ... } finally { ... }`
- **`with (module foo/bar) { ... }`:** compile-time resolution scope — names inside resolve as if written in module foo/bar (incl. its PRIVATES + its require graph; the enclosing module's own symbols are NOT visible inside); `_::name` escapes back to the enclosing module; locals stay lexical and win; nested forms don't combine — innermost wins outright; erased after infer, zero runtime. Policy `with_module_is_unsafe` (or `.das_project` `with_module_unsafe(mod, file)`) makes user-written ones require `unsafe`
- **Named arguments:** `foo([name = value])` with square brackets. Multiple named arguments share
  one bracket group: `foo(positional, [first = a, second = b])`; separate groups such as
  `foo([first = a], [second = b])` are a syntax error
- **Block arguments:** block/lambda after `func()` pipes as last arg. No `$` for parameterless blocks: `defer() { ... }`. With params: `build_string() $(var writer) { ... }`. Lambdas: `emplace() @(x : int) { ... }`. **Arrow shorthand for single-expression blocks:** `arr |> sort() $(a, b) => a < b`. Defaulted parameters sitting between the explicit args and a trailing block are padded automatically — don't spell them out
- **Lambda:** `@(args) { body }` or `@@(args) { body }` (no-capture). **Inline arrow form:** `@(x) => expr` (capture lambda) and `@@(x) => expr` (no-capture function pointer) — preferred for short transforms passed as arguments: `sometimes(pat, @@(x) => fast(x, 2.0lf))`
- **Function/method arrow body:** `def add(a, b : int) : int => a + b` — single-expression body, return type optional (`def add(a, b : int) => a + b` infers). Works on class methods too: `def get() : int => count + 2`. The body must START on the `=>` line — `def f() : int64 =>` followed by a newline is `error[30151]` (probe-verified 2026-07-12); wrap a long body by opening `(` on the `=>` line and breaking inside the parens
- **Generator:** `$() { yield value; }` or `$ { yield value; }`
- **Tuple `=>`:** `a => b` creates `tuple<auto;auto>`
- **`typeinfo`:** `typeinfo trait_name(type<T>)` — trait name outside parens
- **`static_if`:** `static_if (condition) { ... }` — parentheses required
- **Type function call:** `take(type<int>, 1, 2)` — NOT `take < int > (1, 2)`
- **Casts require call-style parens:** `cast<T>(x)` / `upcast<T>(x)` / `reinterpret<T>(x)` — the parenthesized operand is mandatory and self-delimiting, so `reinterpret<uint8?>(p) + 1` means `(reinterpret<uint8?>(p)) + 1`, like the call it looks like. The old juxtaposition form `cast<T> x` is a syntax error (it swallowed trailing `<< >> + - * / % ??` into the operand, silently running the arithmetic on `x`'s original type — stride 0 for `void?`, i.e. dropping the add). Related: pointer arithmetic on `void?` is a compile error (30950); do byte math via `intptr` and reinterpret once
- **Newlines inside `(...)`, `[...]`, `{...}` are free** — long pipe chains, multi-arg calls, array/table literals can wrap freely. Statement-level (no surrounding bracket) still requires one statement per line, so wrap the RHS in `(...)` if a `let x = a |> b |> c` needs to break across lines. **DANGER — silent, no error:** without the parens, a continuation line starting with a *unary-capable* operator (`+`, `-`) parses as a separate statement — `+ b` is unary plus, pure, so the optimizer **silently deletes it**. `let x = a` ⏎ `+ b` ⏎ `+ c` becomes just `let x = a` (the `+ b`/`+ c` lines vanish) — wrong result, no diagnostic (verified: `x` is `a`, not `a+b+c`). A non-unary operator like `|> f()` can't begin a statement, so it errors loudly instead — it's `+`/`-` that bite silently. Always wrap a multi-line arithmetic RHS in `(...)`
- **Inline literals over temp-var-and-push** — for short arrays consumed in one expression, write `stack([a, b, c])` rather than `var xs : array<T>; xs |> emplace(a); xs |> emplace(b); stack(xs)`. Faster in interpreted mode and easier to read; same applies to table literals and other bracketed constructors. Threshold: while it stays readable

### The const model — `let`/`var`, dereference, and why const-stripping miscompiles

`const` in das is **purely a type qualifier** — mutability lives entirely in the type, fixed at declaration. There is no separate "const variable" concept, and there is one declaration node (`let`); the `let`/`var` keyword only flags whether `const` is appended to the type.

- **`let` (and a parameter with no `var`) appends `const` to the declared type — whatever you spelled or left to inference.** So `let a = x` ≡ `let a : auto const = x`; `var a = x` ≡ `var a = x` (nothing appended). `def f(a : T)` ≡ `def f(a : T const)`; `def f(var a : T)` ≡ `def f(a : T)`. `var` means *only* "do not append the trailing `const`." Uniform for every type — `int`, structs, pointers alike. So `let`/`var` on a local and the presence/absence of `var` on a parameter are the **same one mechanism**.
- **Dereference / index / field access appends the handle's const to the result.** Unlike C++ (where `*p` / `p[i]` yields the pointee type as-is, so a `T* const` still derefs to a mutable `T&`), das flows the const of *the thing you dereference* onto the result. `a : float? const` ⟹ `a[2]` is `float const` (not `float`), `*a` is `float const`, `a.field` is `<field> const`. A `let`/non-`var` handle therefore gives **const access to everything reachable through it**; only a `var` handle yields mutable access. (This is the same rule as "take the param `var` for mutable field access.")
- **A pointer type has two independent `const` positions:** `float const?` = the **pointee** is const; `float? const` = the **pointer** is const (this is the trailing one that `let`/no-`var` adds). To write *through* a pointer parameter you need **both** non-const → `var x : float?`. Anything less makes `x[i]` a `float const&` and the write fails `error[30952] can't write to a constant value`.
- **The optimizer trusts the type, so stripping const to write is a lie that miscompiles — not a style nit.** A `const` pointee is a promise to the compiler that *that memory is never written*; the JIT/AOT may then mark the parameter `readonly`, treat a `noalias` write as dead, or DCE the whole call. `var p = reinterpret<float?>(somethingConst); p[i] = …` retypes the `const` away, but the **original** const type already licensed those optimizations — the write silently vanishes, in interp / JIT / AOT alike, with no diagnostic. This is the exact root of the `noalias` write-elision and the forwarded-`const?` elision (issue #3311). **Never take a writable pointer as `T const?` and `reinterpret`-strip it — declare the parameter `var T?`.**
- **Idiom for a writable pointer kernel (including fork-callable pointer cores):** the output parameter is `var T?` (write `o[i]` directly, no strip); genuinely read-only inputs are `T const?`. Callers pass `addr(arr[0])` straight in — it binds to `var T?` (output) and to `T const?` (input, add-const is implicit), with **no laundering `reinterpret`**. At a local, `let p = addr(x)` is a const handle (`p[i]` is const — can't write); `var p = addr(x)` is writable. A hoisted `var p` captured into a `parallel_for`/`maybe_parallel_for` worker stays writable — capture freezes the *binding*, not the pointee, and `p + off` is still a mutable `T?`.

### Type modifiers

- **`==const`** on a parameter type — propagates the caller's constness (NOT "always non-const"): `def foo(self : MyStruct ==const)` accepts either `MyStruct` or `MyStruct const`, and inside the body `self`'s constness matches what the caller passed. Use plain `Foo?` for non-const-only, `Foo const?` for const-only, `Foo? ==const` when you want the callee to accept either and inherit the caller's view
- **`-const`** removes a `const` from a type expression (with `reinterpret`): `unsafe(reinterpret<MyStruct? -const>(addr(self)))`. **Caveat (see The const model above):** this is the genuine-interior-mutability escape hatch for a const handle you *cannot* re-declare `var` (e.g. a `==const`-propagated `self`), and it carries the same readonly/`noalias`-elision risk the optimizer is licensed to take. For a parameter you control that you simply need to write through, the fix is **`var T?`, not a `-const`/reinterpret strip** — don't reach for `-const` to paper over a missing `var`
- **Function pointer with explicit type:** `@@<(var self : T) : RetT> funcName` — specifies the exact parameter/return types of a function pointer literal
- **OR types in params** (`T1 | T2 | …`) — a parameter may list alternative accepted types: `int | float | double`, or heterogeneous forms like `array<int> | table<auto> | auto(NT)`. This is a **generic "OR" type, NOT a runtime tagged variant** — the function is monomorphized per the concrete argument type that matches one alternative, so each instantiation sees that concrete type with no per-call dispatch or unpacking cost. Don't "hoist the union cast out of the loop" — there is no union value; a cast like `float(n)` inside the body is just a trivial concrete cast in each instantiation. Use it to widen an overload set in one signature (e.g. `def fast(n : int | float | double)` accepting bare ints, floats, and doubles at the same call site)

### Fixed arrays (structural since 0.6.3)

`int[10]` is a **structural type** (`Type::tFixedArray`), not a qualifier vector on the element: one node per dimension, element in `firstType`, size in `fixedDim`, outermost first (`int[3][4]` = FA(3, FA(4, int))); ref/const/temporary live on the chain head. Operations act on the **outermost level only** (the one-peel rule).

- **Generic binding:** `auto(TT)` binds the WHOLE array (`TT = int[3][4]`); `auto(TT)[]` peels one level (`TT = int[4]`, parameter constness inherited); `TT - []` in a return/alias removes ONE level — pre-0.6.3 docs saying "removes all dims" describe the deleted flattened world
- **`safe_addr(arr)` on a fixed array returns a pointer to the FIRST ELEMENT** (C-style decay; multi-dim peels one level: `int[2][3]` → `int[3]?#`) — this is what makes `glGetBooleanv(what, safe_addr(flags4))`-style C interop work
- **Typedefs compose:** `typedef M4 = float[4]` then `M4[10]` is `float[10][4]` — array-ness survives aliasing and generic substitution (the pre-0.6.3 flattening bugs are gone)
- **Runtime `TypeInfo` is still flattened** (`dim[]`/`dimSize`) — only the AST is structural. C++ `TypeDecl::isArray()` means "is a fixed array" in both pre- and post-rework daslang (useful for external modules spanning versions)
- Macro authors: das-side `TypeDecl` fields are `fixedDim`/`fixedDimExpr` (NOT `dim`/`dimExpr` — deleted); typemacro payloads live in `typeMacroExpr`; build chains with `make_fixed_array_type(total, element)` from `daslib/ast_boost` — details in `skills/das_macros.md`

### Important defaults

- No implicit type promotion: `int + float` is a compile error — both sides must match
- No `bool(int)` cast — use `x != 0`; no `string(bool)` — use `"{flag}"`
- `int("123")` does NOT work — use `to_int` from `require strings`. **`to_int` silently returns `0` on garbage** (`to_int("foo")` → `0`, `to_int("12abc")` → `12`). When you need to validate user/external input — including any string that flows into a shell command, file path, or system call — use `try_to_int` / `try_to_float` from `daslib/strings_convert` instead. Those return `Result<T; ConversionError>` distinguishing `invalid_argument` / `out_of_range` / `trailing_garbage`, so `";rm -rf;"` rejects cleanly instead of becoming `0`. Same for `to_float` → `try_to_float`
- Hex literals are `uint` by default — use `int(0x3F)` for int
- **16/8-bit type lattice**: `float16` scalar (`half` = builtin alias; literal suffix `1.5h`) + packed vectors `half2/3/4/8`, `short2/3/4/8`, `ushort2/3/4/8`, `byte2/3/4/8/16` (**byte = SIGNED int8**), `ubyte2/3/4/8/16`. Tightly packed (half3 = 6B), element-aligned, pass by value. fp16 family has **closed arithmetic** (per-op promote-to-float-round-back — bit-identical to native fp16); int families are **storage + converts only** (`byte4 + byte4` is a compile error; widen via `int4(b4)`, narrow via ctor = C-truncate or `short4_sat(i4)`-style saturating forms). `.s`-hex swizzles on every vector (`v.s3210`, `b16.sf`), `.lo`/`.hi` on 8/16-lane forms; `xyzw` stays ≤4 lanes. Conformance harness: `tests/type_lattice/` (GENERATED — regen via `daslang utils/dasgen/gen_type_conformance.das`). Lattice vectors have NO ExprConst nodes by design (`isFoldable` false; `Program::makeConst` returns null — callers must handle)
- **`default<T>`** — the zero value of `T`. Body of the called function CAN use it.
- **`type<T>`** vs **`default<T>`** as a witness argument: `type<T>` is a no-stack tag (compile error if body reads it; annotate `[unused_argument(t)]`); `default<T>` is a real zero value (body can read/pass it). Pick by whether the body needs to touch the param. If you want to read a `type<T>` param, switch the caller to `default<T>` — don't rewrite the function.
- **`typedecl(expr)`** — compile-time type-of, usable inside `default<>`: `default<typedecl(field)>`.
- **Bitfields**: `bitfield Name : uint8 { ... }` (also `uint16`/`uint64`; default `uint`, always unsigned). From an int: `bitfield64(1ul << 13ul)` (also `bitfield8`/`bitfield16`).
- **Distinct types (nominal newtype, PR #3424)**: `typedef distinct Foo = int` (privacy: `typedef private distinct Foo = int`) — a unique nominal type over any workhorse underlying, ABI-identical and fully erased at runtime (print/RTTI/debugger show the underlying value). No interconversion in either direction; two distincts over the same underlying are unrelated; overloads on `Foo` vs `int` coexist. The only way IN is `Foo(expr)` (exactly one arg of exactly the underlying type, zero-cost relabel); the only way OUT is `*a` (peels one distinct level, yields underlying as ref — const flows from the handle; for distinct-over-pointer it peels the distinct, not the pointer). Only `==`/`!=` are borrowed (same distinct both sides); all other operators are user-defined (`def operator + (a, b : Foo)`). NOT a table key by design. `default<Foo>` / `var a : Foo` zero-init. `distinct` is NOT a keyword (validated in the grammar action — identifiers like linq's `distinct()` are unaffected). C++ side: `DistinctTypeAnnotation` + `MAKE_DISTINCT_TYPE_FACTORY(TYPE,CTYPE)` + `makeType`-registered externs (worked example: `NativeId` in dasUnitTest). Reference: `doc/source/reference/language/distinct.rst`.

### Pass-by-value vs pass-by-reference

- Structs/arrays/tables always pass by reference — no `&` needed.
- Only **workhorse types** (`int`, `float`, `bool`, `string`, …, `isWorkhorseType` on the C++ side) pass by value.
- **AST pointers (gc_node) pass by value** — copying the pointer, no refcount, no allocation. `def foo(p : ExpressionPtr)` shares the node; `var p` lets you reassign locally; `var p : ExpressionPtr&` propagates reassignment back. For mutable field access, take the param as `var`.
- **Lambdas are copyable.** A `lambda<…>` is a fat pointer to a heap-allocated capture frame; `=` and pass-by-value copy the pointer (creates an alias), and `push`/array storage works without `push_clone`. **`delete lam` requires `unsafe`** since other aliases may still be live — same rule as raw pointer / class `delete`. The unsafe-delete rule cascades: `array<lambda<…>>`, structs with a lambda field, tuple/variant containing a lambda — all inherit the unsafe-delete requirement.
- **`delete` on `array<T?>` (any container of raw pointers) FREES THE POINTEES** — it finalizes and heap-frees every element, then the buffer. On borrowed pointers this is heap corruption: interp reports `deleting <ptr>, which is not a chunk pointer`; Release+JIT corrupts SILENTLY and crashes at some later unrelated alloc/free (probe-verified 2026-07-09). For non-owning pointer containers: struct fields take `@do_not_delete` (canonical: `daslib/aot_cpp.das` `@do_not_delete stack : array<ExprBlock?>` — pointees survive, buffer still freed); locals do `arr |> clear()` before `delete arr` (`clear` does NOT finalize elements — probe-verified).
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

- **`unsafe(expr)`** — narrow-scope unsafe, preferred over `unsafe { block }`. Limits unsafe to the exact expression that needs it. Lint backs this: STYLE024 flags `unsafe` wraps with no descendant needing unsafe; STYLE025 flags blocks where exactly one statement needs unsafe (narrow to expression form); STYLE026 flags nested `unsafe { ... }`
  - **The expression form does NOT propagate into nested call arguments.** `unsafe(f(addr(x)))` still fails with `error[31000] address of reference requires unsafe` at the inner `addr` — the wrap only authorizes the unsafe op at the *top* of `expr`, not a sub-expression buried in an argument. Wrap the exact unsafe op instead: `f(unsafe(addr(x)))`. The block form `unsafe { f(addr(x)) }` *does* cover the nested op (whole scope), so it's the fallback when several nested ops need it. The one sanctioned exception is `addr<T?>(x)` below — its desugared reinterpret propagates the wrap down to its own generated addr.
- **`addr<T?>(x)`** — sugar for `reinterpret<T?>(addr(x))` with ONE unsafe gate: `unsafe(addr<T?>(x))` covers both halves (the desugared reinterpret carries `castFlags.fromAddrSugar`; infer preVisit pushes `alwaysSafe` down onto the generated inner addr). Target must be a pointer type — `addr<int>(x)` is `error[30133]`, always a typo for `addr<int?>`. The exact-type form (`addr<int?>(i)` where `addr(i)` is already `int?`) folds to plain `addr` but still demands unsafe via the addr gate. Prefer over the double-wrapped `unsafe(reinterpret<T?>(unsafe(addr(x))))` — STYLE034 flags the long form (pointer targets only; the sugar's own output is exempt)
- **Local reference binding is unsafe:** `let blk & = expr` requires `unsafe` whenever it creates a local reference to a non-local expression — `let blk & = unsafe(expr)`
- **Variant `as` read access is safe:** `(v as _field).member` works without `unsafe` after an `is` check
- **Variant field assignment is always unsafe:** `v._field = value` and `set_variant_index(v, N)` require `unsafe`
- **`reinterpret<T>(expr)`** requires `unsafe` — used for const-stripping on regular pointers: `unsafe(reinterpret<Foo?>(const_ptr))`
- **`reinterpret` is a PREFIX operator that swallows a following additive expression — silent wrong pointer math.** `reinterpret<float?>(p) + off` parses as `reinterpret<float?>((p) + off)`: the parens around `p` do NOT delimit the operand, so `+ off` executes on `p`'s ORIGINAL type first (byte-scaled for `uint8?`), then the result is cast — no diagnostic (probe-verified 2026-07-09: values landed at byte offsets, not element offsets). Fixes (both verified): bind the cast to a local first (`var d = reinterpret<float?>(p); d + off`), or parenthesize the whole cast (`(reinterpret<float?>(p)) + off`)
- **`typeinfo is_unsafe_when_uninitialized(type<T>)`** — the trait that gates unsafe-ness per type in generic code. Pairs with field-level `@safe_when_uninitialized` and struct-level `[safe_when_uninitialized]` annotations. Canonical use in `daslib/builtin.das`: declare `var x : TT` inside `static_if (typeinfo is_unsafe_when_uninitialized(type<TT>)) { unsafe { ... } } else { ... }` — the unsafe block needs `// nolint:STYLE025` on the `unsafe {` line because STYLE025 sees only one statement needing unsafe at any instantiation and can't reason across the static_if branches

### Error handling

- `try/recover` — NOT `try/catch` (`recover` is the keyword)
- `panic("message")`, `assert(condition)`, `verify(condition)` (stays in release)
- **`assert`/`verify` message must be a string CONSTANT** — `assert(cond, "msg")` rejects an interpolated `"...{x}..."` with `error[30117]` "assert comment must be string constant". Use a constant message; when you need the runtime value in the diagnostic, guard instead: `if (!cond) panic("...{x}...")` (`panic` takes an interpolated string).
- **Postfix conditional:** `return expr if (cond)`, `break if (cond)`, `continue if (cond)` — early-exit guard on one line
- **Braceless early-exit:** prefer `if (cond) return X` (or postfix `return X if (cond)`) over `if (cond) { return X }` — STYLE005 flags the braced single-terminator form as noise
- **Panic is fatal, not an exception.** daslang has no C++/JS-style exception model. A `panic` (or failed `assert` / `verify`) means the program is broken — the only correct response is to print diagnostics and exit. `try/recover` exists to capture the message before exit so you can log it nicely, NOT to recover-and-continue. Do not write code that relies on continuing after `recover`; do not design APIs around panic-as-control-flow. Corollary: `{ body } finally { cleanup }` deliberately skips `cleanup` on panic (the cleanup can't run safely on a broken program); this is not a bug. Don't try to "fix" it; don't use `finally` for cleanup that needs to run on panic. If you need post-statements that run after a block in the normal path, just put them after the block — panic skips everything, and that's the design.

### Generic function dispatch

- **`_::foo(x)`**: resolves in the **calling** module — caller’s overloads visible. Use in library generics.
- **Unqualified** `foo(x)`: resolves in the **defining** module — caller’s overloads NOT visible.
- This is why `:=` and `delete` emit `_::clone` / `_::finalize`

### Recursive flatten / variadic generics (untyped param)

A generic that should accept `array<T>`, `array<array<T>>`, … (any nesting) — e.g. a flattening `push_from(dest, src)` — uses an **untyped** source param (`def f(var dest : array<auto(T)>; src)`, NOT `array<auto(TT)>`) plus `static_if (typeinfo is_array(src)) { for (x in src) f(dest, x) } else { <base> }`. **ADD it alongside** the concrete single-level overload (`array<numT>`); the concrete one out-specializes a bare untyped param, so flat sources take the fast/bulk path and the recursive form fires only for deeper nesting — zero change/risk to the existing overload. (Live example: variadic `push_from`/`push_clone_from` in `daslib/builtin.das`.)

- **`array<numT>` and `array<auto(TT)>` are EQUALLY specialized** → two such same-name overloads are *ambiguous* (`error[30341] too many matching functions`). A bare untyped `src` is *less* specialized than `array<numT>`, so the concrete one wins — that's why the recursive param is untyped, not `array<auto(TT)>`. (Whether an alias should out-specialize a non-alias is an open compiler question.)
- **Compile-time type equality:** there is **no** `typeinfo is_same_type(A, B)` (typeinfo traits take one arg). Use `typeinfo stripped_typename(x) == typeinfo stripped_typename(y)` (pattern from `daslib/algorithm.das`).
- **`-#` / `==const` on ≥2 same-`numT` array params breaks matching** (rejects a plain `array<int>&`) — use plain `array<numT>` for fixed-arity multi-source params (`f(dest; a : array<numT>; b : array<numT>)`).

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
- **`table<T>` (one type param) is the set type** — value type elided. `var s : table<int>; s |> insert(5); key_exists(s, 5)`. Distinct from `table<K; V>` (the map form); both shapes coexist. Set-literal init: `let STOP_WORDS : table<string> <- { "a", "an", "the" }` — value-less braces, comma-separated. Use this instead of declaring `var X : table<T>` and populating in an `[init]` function.

### Iterators and `each`

- `[unsafe_outside_of_for] def each(x) : iterator<T>` makes a type iterable in `for` loops
- When the iterator is named `each`, the call can be omitted: `for (v in each(x))` is identical to `for (v in x)`
- Other iterator names (e.g. `filter`, `map`) cannot be omitted
- **Iterator element-const variance is pointer-like:** `iterator<T -&>` flows into `iterator<T const -&>` (mut → const), not the reverse. So a generic param declared as `iterator<auto(TT) const>` takes both `each(array<T>)` (yields `iterator<T -&>`) and iterator-comprehension (yields `iterator<T const -&>`) sources via a single instantiation. The `const` qualifier alone is enough — do NOT add `&` (that's a separate ref-form modifier, not what you want for variance)
- **Generic-mangling pitfall:** instantiations of the same generic that differ only in inner element-const (`iterator<int -&>` vs `iterator<int const -&>`) currently hash-collide in the instance registry, producing `error[50609]: multiple instances of …` when both arise in one module. Workaround at the library level: declare the iterator overload as `iterator<auto(TT) const>` instead of `iterator<auto(TT)>` — both source flavors then converge on a single instance per the variance rule above. Caveat: the constify makes `it` inside the body const, so the body must not move from / mutate / call non-const operators on `it`. linq.das only constifies `all` and `contains` for this reason; the rest stay vulnerable until the mangler is fixed upstream

### String access functions

- **`peek_data(str) $(arr) { ... }`** — safe O(1) per-element read access to string as `array<uint8> const#`. One `strlen` call total. Preferred over `character_at` for iteration.
- **`modify_data(str) $(var arr) { ... }`** — returns a modified copy; allocates new string, opens as mutable `array<uint8>`. Use for character-level transformations.
- **`character_at(s, i)`** — O(n) per call (`strlen` + bounds check). Fine for isolated checks, but use `peek_data` in loops or hot paths.
- Pointer-based string access (`reinterpret<uint8?>`) is for core library implementations only — user code should use `peek_data`/`modify_data` for safety.

### Common gotchas

- Lambda params can shadow function params — use distinct names
- **`label`, `expect`, and `pass` are reserved words** (lexer keywords `DAS_LABEL`/`DAS_EXPECT`/`DAS_PASS` — `pass` is the no-op statement) — using any as a parameter/variable name is a syntax error; rename (`tag`, `want`, `cpass`)
- **Literal `{`/`}` in string literals must be escaped `\{`/`\}`** — unescaped `{...}` is interpolation. Bites when embedding shader/C source as inline strings. String literals may span multiple lines (raw newlines are legal); probe-verified 2026-07-11
- String builder requires `unsafe` or `options persistent_heap` if returned
- **DANGER — silent JIT miscompile:** a comprehension used **inline** as a `<-` move-init argument inside a struct constructor (`Foo(a <- [for (x in src); expr], b <- local)`) yields an **empty array under `-jit`** (interp is fine; a plain-local `<-` field beside it is unaffected). No diagnostic. **Hoist the comprehension to a `var` local first**, then `Foo(a <- local_comp, b <- local)`. (This is exactly the fix applied in `modules/dasLLAMA/performance/gen_asr_profile.das`.)
- Tuple field access: `t._0`, `t._1`, `t._2`
- Annotations: `[export]`, `[test]`; `options no_aot`, `options rtti`
- **`options` are MODULE-LOCAL for pass-macros** (`[lint_macro]` / `AstPassMacro`). The macro fires once per module in the require chain, reading `prog._options` from THAT module's options table — not the program-root's. So `options _my_lint_off = true` in `foo.das` suppresses YOUR lint in `foo`, but `require foo` from `bar.das` does not inherit the flag — `bar` gets linted unless it sets its own. Don't confuse with runtime options (`gc`, `multiple_contexts`, `persistent_heap`, `rtti`) which DO unify across the program codegen and effectively cascade up to consumers
- **`options stack` is MAIN-MODULE-ONLY — it does NOT unify up from required modules** (probe-verified 2026-07-09: a 1MB `options stack` in a required shared module left the program's context at the 16K default and it still overflowed). A deep library (dasLLAMA's forward chain, llvm_jit) cannot declare its own stack need — every PROGRAM (test file, example, tool) that drives it must carry `options stack = 65536` (or whatever it needs) itself; `llvm_jit.das`'s 4MB works only because those files run as the program root
- **Visibility is a prefix keyword, not an annotation:** `def private foo()`, `struct private Foo { ... }`, `enum private E { ... }`, `variable private x = 0`, `alias private X = Y`. There is **no** `[private]` annotation — it's a grammar error
- **Field/variable annotations use `@name` only:** `@safe_when_uninitialized at : LineInfo`, `@sql_primary_key id : int64`, `@do_not_delete ctx : Context?`. The `[name]` form is reserved for struct/function/global-level annotations and does NOT parse on a struct field
- `require` uses forward slash: `require daslib/linq` — NOT backslash
- `require foo public` — re-exports `foo` transitively
- **`require` path resolution:** bare `require name` resolves a module-root mount (`daslib/x`, a registered module) **or** a *same-directory* `name.das` (this is how `require tutorial_server` picks up `tutorials/dasHV/tutorial_server.das`). To require a file *elsewhere in the tree* by path, use a **file-relative path with the explicit `.das` extension** — `require ../../foo/bar.das`. Dropping the `.das`, or using a non-mounted root-relative path like `require tutorials/x/y`, fails with `error[20605] missing prerequisite … file not found`. **A require path is `NAME` tokens joined by `/` `.` `%` `..` (grammar `require_module_name`) — so it CANNOT contain a hyphen:** a dir like `utils/dasllama-server/` is unreachable by path (the parser stops at the `-`, e.g. `require ../../utils/dasllama-server/x.das` resolves `../../utils/dasllama` and fails). This is why hyphenated-dir tools (`detect-dupe`, `find-dupe`, `dasllama-server`) keep their `[test]` files *inside the dir* and `require` the sibling module by **bare name**. Upshot: a test needing a shared fixture should keep the fixture in its *own* directory (bare same-dir require) rather than reaching across the tree
- `[export] def main()` defaults to returning `void`, but you can declare it as `def main() : int { ... return rc }` when you need to surface a non-zero process exit code (e.g. CLI tools where callers — MCP wrappers, CI, parent shells — branch on exit). See `dastest/dastest.das` for the canonical pattern. Don't reach for `panic` just to force a non-zero exit; declare `: int` and `return rc` instead.
- `push` copies (fails for non-copyable types), `emplace` moves (zeros source), `push_clone` clones (preserves source)
- Non-copyable types (`array<T>`, `table<K;V>`): use `:=`, `push_clone`, or `<-`. (Lambdas are copyable — see above.)
- Blocks cannot be stored/returned/captured — use lambdas or function pointers
- Class methods: `def const`, `def abstract const`, `def static`; call syntax `obj.method()`, `obj->method()`, `obj |> method()`
- **`is`/`as` on handled types checks EXACT type**, not C++ inheritance — `expr is ExprField` is `false` when `expr` is `ExprSafeField`. `as` on wrong type crashes. Must handle each concrete type explicitly.
- `#pragma optimize` in AOT-generated code must be wrapped in `#ifdef _MSC_VER` — Clang warns on unknown pragmas
- **Macro-generated `var x : $t(st)`** (no init) trips `error[31016]` "uninitialized variable is unsafe" for **any** struct result/local type — field defaults do **not** exempt it. Fixes: `= default<$t(st)>` when the type is default-constructible; but for handled/backend types `default<>` is `error[50503] unsupported variable type` — then set `td.flags.safeWhenUninitialized = true` on the (cloned) decl type when the uninitialized read is intentional and discarded (canonical: the `[flatten]` return accumulator in `daslib/flatten.das`)
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
| `unsafe { (reinterpret<ExprBlock?>(blk)).list }` / `unsafe(reinterpret<T?>(x))` | make param `var` + plain `x.list` | `var` param gives non-const field access without reinterpret |
| `if (cond) { return X }` (or `{ break }` / `{ continue }`) | `if (cond) return X` or postfix `return X if (cond)` | STYLE005: braces around a single-statement early-exit are noise |
| `for (i in range(length(arr))) { ... arr[i] ... }` where `i` is used only as `arr[i]` | `for (c in arr) { ... c ... }` | PERF018: direct iteration drops the index variable |
| `from_JV(v, type<int>, 13)` | `v ?? 13` | STYLE020: json_boost provides `operator ??` for every scalar `from_JV` overload |
| `var args : table<string; JsonValue?>; args \|> insert("k1", JV(v1)); args \|> insert("k2", JV(v2))` | `var args = JV((k1=v1, k2=v2))` | STYLE021: named-tuple JV form (json_boost.das:638) is one line instead of N |
| `int(BfT.a) \| int(BfT.b)` (same bitfield, or enum with `operator \|`) | `int(BfT.a \| BfT.b)` | PERF019: collapse two int casts to one. Const-foldable forms only surface under lint policies |
| `int64(a)` where `a : int64` (or any of the 15 workhorse casts: `int*`/`uint*`/`float`/`double`/`string`/`bitfield*`) | `a` | PERF020: same-type workhorse cast is a no-op `ExprCall`. Match is `baseType`-strict, so widening/narrowing/signedness/float↔int still fire as genuine work. User-named bitfield/enum ctors (`MyBitfield(x)`) and vector ctors (`int2(x,y)`) are out of scope |
| `foo \|= BfT.m` / `foo &= ~BfT.m` (bitfield `foo`, single named bit) | `foo.m = true` / `foo.m = false` | STYLE022: bitfield-as-field assignment reads bit-name-first, drops the `~` for clears |
| `uint(bf & BfT.m) != 0u` / `int(bf & BfT.m) == 0` (bitfield `bf`, single named bit) | `bf.m` / `!bf.m` | STYLE023: bitfield-as-field read; drop the int cast + `!= 0` / `== 0` compare |
| `unsafe(x + y)` / `unsafe { let d = x + y }` where nothing inside requires unsafe | drop the wrap | STYLE024: redundant `unsafe` — flagged when no descendant matches a known inherently-unsafe shape (reinterpret/upcast cast, `delete`, `addr`, table-index, variant-write, ExprCallFunc with `unsafeOperation`). Macro-generated subtrees (`genFlags.generated == true`) skipped per design |
| `unsafe { stmt1; stmt2; stmt_needing_unsafe }` (only ONE stmt actually needs unsafe) | `stmt1; stmt2; unsafe(<sub-expr>)` | STYLE025: narrow block-form unsafe to expression-form on the single unsafe-needing statement. Silent when ≥2 statements need unsafe (block is justified) |
| `unsafe { ...; unsafe { ... }; ... }` (nested `unsafe { }` block) | drop the inner wrap | STYLE026: outer `unsafe` already covers the whole inner scope, so the inner block is pure noise. Closure / lambda / generator bodies are NOT nested for this rule — they execute in a separate context where the outer wrap does not propagate |
| `for (s in A) { B \|> push(s) }` / `push_clone(s)` (iter-var only) | `B \|> push_from(A)` / `push_clone_from(A)` | PERF022: the bulk overload in builtin.das reserves combined capacity up front. Single name `push`/`push_clone` is overloaded between single-element and bulk (ambiguous when destination is `array<T[]>`); the `_from` suffix names the bulk intent. Source must be `array<T>` or C-array — range/iterator sources are not flagged. `emplace` is out of scope (const iter-var can't be moved) |
| `var a : array<T>; for (x in SRC) { if (COND) { a \|> push(EXPR) } }` (or `table<K;V>` + `insert`/`a[k]=v`) | `var a <- [for (x in SRC); EXPR; where COND]` (or `\{for (...); k => v; where ...\}`) | STYLE027: var with empty default-init followed by a for-loop that only push/insert into it. Accepts depth ≤ 2 nested fors and if-filters at any depth. `emplace` excluded — move-source-zeroing differs from comprehension element-construction. Iterator-comprehension form (`[$f ...]`) NOT suggested |
| `var t : table<K;V>; t \|> insert(k1, v1); t \|> insert(k2, v2)` (or `t[k] = v` runs, or 2-arg set inserts) | `var t <- { k1 => v1, k2 => v2 }` (set: `var s <- { k1, k2 }`) | STYLE031: ≥ 2 contiguous inserts/`[]=` after an empty table decl collapse to a literal move-assign. Computed keys fine; runs with a duplicate CONST key stay silent (literal duplicates are `error[30706]`, inserts overwrite). `table<string; JsonValue?>` const-key runs get STYLE021's `JV((k1=...))` form instead |
| `var w : array<T>; w \|> push_from(SRC)` (empty decl + single bulk copy from an `array<T>`) | `var w := SRC` (clone-assign); if then `return <- w`, `return clone_to_move(SRC)` | STYLE032 (the init half) + PERF009-clone (the return half) — they compose. A bulk `push_from`/`push_clone_from` into a fresh-empty array IS a clone-init. Array source only; a C-array source stays silent (`var w := cArray` ≠ `array<T>`). Only the statement immediately after the decl is inspected — a `reserve`/guard between keeps it quiet. PERF009 clone variant: `var x := src; return <- x` → `return clone_to_move(src)`, NOT `return <- src` (would move/destroy the source) |
| `var X = clone_expression(E); ... $e(X) ...` (only-uses-are-qmacro-splice) | drop the pre-clone, inline `$e(E)` at each splice site | PERF023: `qmacro`/`qmacro_block`/`qmacro_expr`/`qmacro_block_to_array` go through `apply_template` (templates_boost.das:418), whose substitution visitor calls `clone_expression` on every substitution input (templates_boost.das:252). Pre-cloning is wasted work. Detection: post-expansion `$e(X)` becomes `add_ptr_ref(X)` inside an `ExprMakeBlock`; visitor tracks splice-wrapper depth via preVisitExprCall/visitExprCall counter on `add_ptr_ref`, classifies each candidate `ExprVar` reference as "safe" when depth>0. Fires only when ALL uses are safe AND ≥1 is observed. Multi-clone-of-same-source flagged too — apply_template clones each substitution independently |
| hand-rolled `is X` / `as X` / null-guard / `ExprRef2Value`-peel ladders in macro code | `qmatch(e, $e(a) + $e(b))` for source-syntax shapes; `match (e) { if (ExprField(name = "key", value = ExprVar(...))) { ... } }` for node-class shapes | both matchers peel `ExprRef2Value` automatically; `\|\|` alternation, `&&` guards, and `match_expr(local)` cover most ladders. Limits + the qmatch↔match division of labor: `skills/das_macros.md` "`match` (daslib/match)" |
| `unsafe(reinterpret<T?>(unsafe(addr(x))))` (reinterpret-of-addr, pointer target) | `unsafe(addr<T?>(x))` | STYLE034: same AST, one unsafe gate instead of two. Non-pointer puns (`reinterpret<uint64>(addr(x))`) have no `addr<T?>` spelling and stay silent; the sugar's own desugared output is exempt via `castFlags.fromAddrSugar` |
| `b == T('(')` where `b : T` and T is a non-`int` built-in numeric scalar (also `!=`, ranges, and Yoda forms) | make `b : int`, then write `b == '('` | STYLE035: character literals are `int`; changing the plain variable once removes every repeated numeric cast. The warning is deduplicated at the declaration. |

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

- `daslib/aot_cpp.das` — AOT C++ emitter lives here, NOT in C++ (`src/ast/ast_aot_cpp.cpp` was deleted; only the header remains)
- `tests/aot/CMakeLists.txt` — register new test directories here for AOT compilation. Two AOT binaries: `test_aot_subset` (tests/language only, in ALL — the per-PR CI compile gate) and full `test_aot` (`EXCLUDE_FROM_ALL`, ~1080 AOT TUs — nightly CI + `preflight --full` only, via `--target test_aot`/`run_tests_aot`)
- `dastest/` — test framework (used by both `tests/` and external repos)
- `utils/detect-dupe/` (in-repo dupe finder) and `utils/find-dupe/` (Claude-based judge that needs `daspkg install --root utils/find-dupe` + `ANTHROPIC_API_KEY`) — both also exposed as MCP tools
- `utils/mcp/`, `utils/daslang-live/`, `utils/daspkg/` — in-tree tools (most also have skills under `skills/`)
- `tutorials/language/` (language tour) vs `tutorials/<area>/` (per-area) — never put tutorials in `modules/<X>/tutorial/`
- **Array/dim/vector indexing lives across 5 tiers** — bug fixes (bounds checks, neg-index detection, width-aware bounds) usually need parallel edits in all of them: AOT C++ ([include/daScript/simulate/aot.h](include/daScript/simulate/aot.h)), interpreter non-fused ([include/daScript/simulate/runtime_array.h](include/daScript/simulate/runtime_array.h) + [include/daScript/simulate/simulate_nodes.h](include/daScript/simulate/simulate_nodes.h)), interpreter fused ([src/simulate/simulate_fusion_at_array.cpp](src/simulate/simulate_fusion_at_array.cpp) + [src/simulate/simulate_fusion_at.cpp](src/simulate/simulate_fusion_at.cpp)), JIT ([modules/dasLLVM/daslib/llvm_jit.das](modules/dasLLVM/daslib/llvm_jit.das)), and AST const-fold ([src/ast/ast_simulate.cpp](src/ast/ast_simulate.cpp)). Debug builds bypass the fused permutations — a fix that lands only in the fused path passes Release CI and trips Debug-ARM. Bump `LLVM_JIT_CODEGEN_VERSION` in `modules/dasLLVM/daslib/llvm_jit_run.das` after JIT emitter/ABI changes to invalidate cached `.dll`s (the cache hash also folds per-function AOT hashes, so AST-level changes self-invalidate without a bump)

## MCP Server (AI Tool Integration)

The daslang MCP server (`utils/mcp/main.das`) exposes compiler diagnostics, program introspection, and live-reload control. **Prefer MCP tools** over manual compilation and grep — `grep_usage` is parse-aware (tree-sitter), `find_references` resolves cross-module symbols, and `live_*` tools talk to `daslang-live` directly instead of curl.

**Fresh worktree/clone with no daslang MCP tools?** `.mcp.json`, `sgconfig.yml`, `bin/`, and the tree-sitter grammar lib are all gitignored, so a `git worktree add` (or clone) starts with zero daslang tools. Bootstrap it from any existing daslang binary: `daslang utils/mcp/setup.das -- --root <worktree>` builds a worktree-local binary (+ grammar), copies the platform `sgconfig.yml`, and merges a `daslang` entry into `.mcp.json` (writes no secrets, preserves `github` etc.). `--no-build` wires config to an already-built binary. Restart the session in the worktree to pick it up. Details: **`skills/mcp_tools.md`**.

Full tool table (including `detect_duplicates`/`judge_duplicates`/`find_dupe`), live-API caveats, and `.mcp.json` configuration: **`skills/mcp_tools.md`**.
