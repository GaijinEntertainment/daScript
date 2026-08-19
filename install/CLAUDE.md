# daslang SDK Instructions

## Overview

[daslang](https://daslang.io/) (formerly daScript) is a high-performance statically-typed programming language designed for games and real-time applications, standalone or embedded. Many C++ API names still use the old "daScript" spelling.

For the **why** — design principles, three-tier execution model (interpreter / AOT / JIT), the macros-as-design-lens rule — read `skills/project_overview.md`.

## Running Scripts

- **Run a script:** `bin/daslang path/to/script.das`
- **Compile-only check:** `bin/daslang -compile-only path/to/script.das` — compiles without simulation or execution. Use `-dry-run` to also simulate (but not execute).
- **AOT generation:** `bin/daslang -aot input.das output.cpp` — emit C++ stubs ahead-of-time. Add `-aot-macros` to include macros in the output.
- **JIT execution:** `bin/daslang -jit path/to/script.das` — compile to native via LLVM JIT before running. Available only in builds compiled with the JIT module. `bin/daslang -exe path/to/script.das -output out` JITs to a standalone executable (implies `-dry-run`); the platform's executable extension is appended automatically (e.g. `.exe` on Windows).
- **AOT-linked execution:** `bin/daslang -use-aot path/to/script.das` — run with AOT stubs that have been linked into the binary (host-side flag).
- **Pass arguments to the script:** everything after `--` is forwarded to the script. `bin/daslang script.das -- arg1 arg2`. Use `daslib/clargs` to parse them — see `skills/daslang/references/cli-and-config.md`.
- **Example:** `bin/daslang examples/hello_world.das`

### Project files (`.das_project`)

A `.das_project` file is a daslang script the compiler runs at startup to control module resolution, includes, and sandboxing. Pass it with `-project`:

```bash
bin/daslang -project path/to/project.das_project script.das
```

The project script can export callbacks the compiler invokes during resolution:

| Callback | Purpose |
|---|---|
| `module_get(req, from)` | Required. Returns `(module_name, file_path, import_alias)` for a `require` |
| `include_get(inc, from)` | Resolve `#include` directives |
| `module_allowed(mod, filename)` | Whitelist which modules a file may `require` |
| `module_allowed_unsafe(mod, filename)` | Allow `unsafe` inside specific modules |
| `option_allowed(opt, from)` | Whitelist `options` directives |
| `annotation_allowed(ann, from)` | Whitelist annotations |

The compiler sets `DAS_PAK_ROOT` to the project directory before evaluating callbacks. Project files can themselves `include other.das_project` to compose. If you hit "module not found" errors when your modules live outside `daslib/` or the script directory, you almost always need a `.das_project`.

### Tutorials

`tutorials/language/01_hello_world.das` through `tutorials/language/58_logger.das` are an ordered tour of the language — start there when learning a new feature. Each tutorial is a runnable `.das` file with comments explaining the construct. Area- and module-specific tutorials live alongside (`tutorials/macros/`, `tutorials/integration/`, `tutorials/sql/`, `tutorials/dasHV/`, `tutorials/dasAudio/`, `tutorials/dasPUGIXML/`, `tutorials/dasPEG/`, `tutorials/dasStbImage/`, `tutorials/dasMinfft/`, `tutorials/dasOPENAI/`, `tutorials/dasLLAMA/`, `tutorials/daStrudel/`, `tutorials/jsonrpc/`, `tutorials/opengl/`).

### Debugging

- **Always check the exit code** after running `daslang` — a crash may produce no output at all, looking like a silent success. PowerShell: `$LASTEXITCODE`. Bash/zsh: `$?`. Exit code `0` = success.
- Exit code `-1073741819` / `0xC0000005` (Windows) or `139` / `134` (Linux/macOS) = native crash — Access Violation / SIGSEGV / SIGABRT
- If the program crashes with no error message, the bug is in native code (C++ bindings or smart pointer misuse) — check exit code first
- **`error[50101]: AOT link failed on <fn>`** — the runtime computed a different semantic hash for `<fn>` than the AOT generator recorded, so the stub lookup misses. Almost always means the `.das` source (or a daslib it pulls in) changed after the AOT C++ was emitted — regenerate the stubs with `bin/daslang -aot input.das output.cpp` and rebuild
- **Don't truncate output** with `head`/`tail` — daslang stack traces are easily clipped. Capture full output, then `grep` if needed
- **`options log`** — append at the end of a `.das` file to dump the final post-compilation program text. Useful for confirming what the compiler actually produces (constant folding, generic reification, macro expansion).
- **`options log_infer_passes`** — append at the end of a failing `.das` file for a per-pass infer-pipeline dump (which generics got reified, when finalize ran, where lookups missed). Smaller and more targeted than `options log` for template/generic reification bugs.

## Skill Files (REQUIRED)

Task-specific instructions are in skill files under `skills/`. Read the relevant skill file(s) before performing the corresponding task.

| Skill file | Read BEFORE... |
|---|---|
| `skills/project_overview.md` | First significant task — design philosophy, three execution tiers, macros-as-design-lens |
| `skills/design_philosophy.md` | The long-form **why** behind daslang — origins, the deliberate trade-offs, the performance model, and why the language is shaped the way it is |
| `skills/mcp_tools.md` | Full MCP tool table + live-API reference |
| `skills/das_formatting.md` | Creating or modifying any `.das` file |
| `skills/comment_style_hygiene.md` | Writing or reviewing comments, names, or local code shape in ANY language — the house rulebook |
| `skills/cpp_integration.md` | Embedding daslang in C++; binding types/functions/enums |
| `skills/daslib_modules.md` | Using `daslib/` modules (linq, json, regex, etc.) |
| `skills/das_macros.md` | Compile-time macros, AST manipulation, qmacro/quote, gc_node patterns |
| `skills/daspkg.md` | Creating `.das_package` manifests, daspkg commands |
| `skills/daslang/references/cli-and-config.md` | Writing daslang CLI tools — declarative argv parsing via `daslib/clargs` |
| `skills/dynamic_modules.md` | `.das_module` descriptors, module resolution, `register_native_path` |
| `skills/external_module_debugging.md` | Iterating on a daslang module outside the SDK tree — run/lint/test from a standalone `daslang` or MCP; the junction pattern + `project_root` MCP arg |
| `skills/daslang_live.md` | `daslang-live` lifecycle, REST API, `[live_command]`, persistent state |
| `skills/imgui_application.md` | Building any dasImgui application — harness lifecycle (`init`/`update`/`shutdown`), the headless arm, heap/GC ownership contract |
| `skills/imgui_migration.md` | Migrating v1 `imgui_boost` code to the v2 boost layer — the v1→v2 mapping table |
| `skills/imgui_ui_debugging.md` | Any dasImgui UI/interaction bug: reproduce → make it observable in `imgui_snapshot` → fix → prove via snapshot + test. Never claim a UI fix from logic or a screenshot alone |
| `skills/daslang/references/json.md` | Reading/writing JSON (`sprint_json`/`sscan_json`, `JV`, manual `JsonValue?`) |
| `skills/xml.md` | XML via `dasPUGIXML`/`PUGIXML_boost` (RAII, builder, XPath, struct round-trip) |
| `skills/daslang/references/files-and-paths.md` | Any `.das` path/filename/filesystem op — must use `fio` helpers, never `rfind`/`slice` |
| `skills/writing_tests.md` | Writing tests with the bundled `dastest` framework |
| `skills/tdd_audit.md` | Auditing any diff for test coverage — the branch-test rule: every new/changed reachable branch has a test that fails without it |
| `skills/review_md.md` | Creating or editing any folder-scoped `REVIEW.md` checklist — the opening contract, the template; the shared constitution is `REVIEW_COMMON.md` at the SDK root |
| `skills/writing_benchmarks.md` | Writing/running benchmark `.das` files (`--bench`, `run`, `Asserter`) |
| `skills/memory_leak_detection.md` | Diagnosing leaks (`--das-profiler-leaks`, `--track-smart-ptr`, `GC APP LEAK`, `HandleRegistry`) |
| `skills/jobque_debugging.md` | Channel/LockBox/JobStatus/Stream/Feature leaks |
| `skills/daslang/references/queries.md` | Filter/map/sort/group/aggregate transforms — prefer comprehension → linq_boost → plain `for`; avoid `daslib/functional` for new code |
| `skills/decs.md` | Programming with `daslib/decs` / `decs_boost` — entities, components, queries, `[decs_template]`, stages, bulk creation, `from_decs` linq bridge |
| `skills/daslang/references/strings.md` | Writing regular expressions in `.das` code |
| `skills/daslang/references/strings.md` | Any `.das` string operation — `find`/`replace`/`split`/parsing/`build_string`/`peek_data` |
| `skills/daslang/references/files-and-paths.md` | Writing or reviewing any glob/wildcard pattern handling — file selection, include/exclude masks, pattern-match-on-paths (`*` / `?` / `**` / `[abc]`) |
| `skills/sql.md` | Any SQL in `.das` — `daslib/sql_linq` + the dasSQLITE/dasDuckDB/dasPostgreSQL providers, `[sql_table]`, the `_sql(...)` LINQ bridge, migrations |
| `skills/dashv.md` | Any `.das` that serves HTTP, streams SSE, makes outbound requests, or tests a server — `HvWebServer`, the client API, `with_test_server` |
| `skills/strudel_port.md` | Porting strudel.cc patterns into `dasStrudel` |
| `skills/tune.md` | The `[tune]` kernel-tuning framework — `[tune_perm]`/`[tune_scope]`/`[tune_policy]`/`--tune`, sidecars, the tune-and-re-exec model, adding a kernel family |
| `skills/profiler.md` | Runtime profiling and `--das-profiler-leaks` — the full guide `memory_leak_detection.md` points at |
| `skills/linq_fold_patterns.md` | Which chain shapes `_fold(...)` recognizes — the full pattern reference `references/queries.md` points at |
| `skills/perf_lint.md` | Running the lint suite or suppressing a finding — **and before declaring any hot path off-limits to allocation**: `[hot_path]`/`[no_alloc]`/`[no_env]`/`[no_io]`, `[cold_path]`, `@scratch` |
| `skills/style_lint.md` | Suppressing a `STYLE*` finding (the per-module `options` knobs) or applying the STYLE037/038 suppress-vs-split policy |

Multiple skill files may apply to a single task. For example, embedding daslang and calling its standard library requires reading both `skills/cpp_integration.md` and `skills/daslib_modules.md`.

**`repo-only` marks paths that are not in this SDK.** The skills are shared with the daslang repository, so a few of them point at daslang's own sources (`src/…`, `tests/…`) when explaining internals. Any such line — or the heading of a whole section of them — carries the token `repo-only`. Those paths do not exist in this bundle and never will; skip them. Everything not marked that way resolves inside the SDK.

## daslang Language

The language, its runtime, and the standard library are documented in full in
**`skills/daslang/`** — `SKILL.md` plus thirteen references (`types`, `functions`,
`structs-and-classes`, `closures`, `memory`, `generics`, `macros`, `modules-and-stdlib`,
`strings`, `files-and-paths`, `json`, `queries`, `cli-and-config`). It is a standalone skill:
read the reference that covers what you are about to write or review, before you write it.

Two things live here instead, because they are not language facts: the shapes that fail
silently, and this SDK's house style.

### Fails silently

Each of these compiles clean and then gives a wrong answer or corrupts memory, with no
diagnostic in any tier.

- **A continuation line starting `+` or `-` is its own statement**, and unary plus is pure, so
  the optimizer deletes it: `let x = a` ⏎ `+ b` ⏎ `+ c` leaves `x == a`. Always wrap a
  multi-line arithmetic RHS in `(...)`.
- **Stripping `const` in order to write.** The original const type already licensed
  readonly / `noalias` / DCE, so the write vanishes in interp, JIT and AOT alike. Declare the
  parameter `var T?`; never `reinterpret` the const away.
- **`delete` on `array<T?>` frees the POINTEES**, not just the buffer. On borrowed pointers the
  interpreter reports `deleting <ptr>, which is not a chunk pointer`, while Release+JIT corrupts
  the heap silently and crashes later at an unrelated allocation. `clear()` the container first,
  or mark the field `@do_not_delete`.
- **A lambda copy aliases one capture frame** — deleting a container that holds two copies of
  the same lambda is a double free.
- **`:=` on a string copies the pointer** unless the source is a temp (`#`) string or the
  program declares `options multiple_contexts`. Across a context boundary that is a dangling
  read; the reliable copy is `clone_string(s)`.
- **`is` / `as` on handled and AST types is EXACT-type**: `expr is ExprField` is false when
  `expr` is an `ExprSafeField`, and `as` on the wrong type crashes. Enumerate every concrete
  type, or use a matcher.
- **`tab[k]` on a mutable table INSERTS on read.** Read with `tab?[k] ?? d`.
- **`to_int` / `to_float` return `0` on garbage** (`to_int("12abc")` is `12`). Anything from a
  user, a file, or the environment goes through `try_to_int` / `try_to_float`.
- **`<-` into a table slot ZEROES the source.** Registering one value under two keys takes a
  fresh clone per registration, not the same variable passed twice.
- **`new WithCtor(field = v)` skips the user constructor** — it is plain field-init, so
  inherited fields stay zero. Write `new WithCtor(args)` when the constructor must run.

### Code style — prefer idiomatic forms

| Don't write | Write instead | Why |
|---|---|---|
| `string(x.__rtti) == "ExprFoo"` | `x is ExprFoo` | `is` works directly on AST pointers |
| `get_ptr(x) == null` / `get_ptr(x).field` | `x == null` / `x.field` | AST pointers auto-dereference |
| `string(das_str) == "lit"`, `!empty(string(das_str))` | drop the `string(...)` cast | `das_string` compares with `string` directly; `empty()` works on it |
| `let v = string(x.name); $i(v)` / `var copy = val; $v(copy)` | `$i(x.name)` / `$v(val)` | qmacro tags accept `das_string`, `let` vars, loop vars directly |
| N qmacro arms differing only in the call target | `let fname = ...; qmacro($c(fname)(…))` | `$c(stringVar)` splices a function name. `_::$c(…)` is a parse error — drop `_::` |
| `if (true) { ... }` | `{ ... }` | bare blocks create lexical scope in gen2 |
| `var inscope r <- expr; return <- r` | `return <- expr` | |
| `unsafe { (reinterpret<ExprBlock?>(blk)).list }` / `unsafe(reinterpret<T?>(x))` | make param `var` + plain `x.list` | `var` param gives non-const field access without reinterpret |
| `if (cond) { return X }` (or `{ break }` / `{ continue }`) | `if (cond) return X` or postfix `return X if (cond)` | STYLE005 |
| `for (i in range(length(arr))) { ... arr[i] ... }` where `i` is used only as `arr[i]` | `for (c in arr) { ... c ... }` | PERF018 |
| `from_JV(v, type<int>, 13)` | `v ?? 13` | STYLE020 |
| per-key `insert("k1", JV(v1))` into `table<string; JsonValue?>` | `var args = JV((k1=v1, k2=v2))` | STYLE021 |
| `int(BfT.a) \| int(BfT.b)` (same bitfield, or enum with `operator \|`) | `int(BfT.a \| BfT.b)` | PERF019 |
| `int64(a)` where `a : int64` (any same-type workhorse cast) | `a` | PERF020 |
| `foo \|= BfT.m` / `foo &= ~BfT.m` (single named bit) | `foo.m = true` / `foo.m = false` | STYLE022 |
| `uint(bf & BfT.m) != 0u` / `int(bf & BfT.m) == 0` (single named bit) | `bf.m` / `!bf.m` | STYLE023 |
| `unsafe(x + y)` / `unsafe { ... }` where nothing inside requires unsafe | drop the wrap | STYLE024 |
| `unsafe { stmt1; stmt2; stmt_needing_unsafe }` (only ONE needs it) | `stmt1; stmt2; unsafe(<sub-expr>)` | STYLE025 |
| `unsafe { }` nested inside another `unsafe { }` | drop the inner wrap | STYLE026. Closure/lambda/generator bodies DO need their own wrap — the outer one does not propagate into them |
| `for (s in A) { B \|> push(s) }` / `push_clone(s)` (iter-var only) | `B \|> push_from(A)` / `push_clone_from(A)` | PERF022: reserves the combined capacity once |
| a for-loop that only pushes/inserts into a fresh-empty array/table | a comprehension: `var a <- [for (x in SRC); EXPR; where COND]` / `\{for (...); k => v; ...\}` | STYLE027. `emplace` loops stay loops — move semantics differ |
| ≥ 2 `insert`s / `t[k] = v` right after an empty table decl | a literal: `var t <- { k1 => v1, k2 => v2 }` (set: `{ k1, k2 }`) | STYLE031. Duplicate const keys: inserts overwrite, the literal is a compile error — don't rewrite those |
| `var w : array<T>; w \|> push_from(SRC)` (fresh-empty target) | `var w := SRC`; if then returned, `return clone_to_move(SRC)` | STYLE032 + PERF009. NOT `return <- src` — that would move out of the source |
| `var X = clone_expression(E); ... $e(X) ...` (only splice uses) | inline `$e(E)` at each splice site | PERF023: the template machinery clones every substitution already |
| hand-rolled `is X` / `as X` / null-guard ladders in macro code | `qmatch(...)` for source-syntax shapes; `match (e) { if (ExprField(...)) }` for node-class shapes | division of labor: `skills/das_macros.md` |
| `unsafe(reinterpret<T?>(unsafe(addr(x))))` (pointer target) | `unsafe(addr<T?>(x))` | STYLE034 |
| `b == T('(')` where `b` is a non-`int` numeric scalar | make `b : int`, compare `b == '('` | STYLE035: character literals are `int` |
| `dst := src` / `push_clone(src)` on a plain `string` | `dst = src` / `push(src)`; cross-context copy = `clone_string(src)` | LINT016: the clone spelling copies only the pointer |
| `int64(length(x))` / `uint64(capacity(x))` (also `count`, `find_index`, `fread`, `fwrite`) | `long_length(x)` / `long_capacity(x)` / … | LINT017: the inner call returns `int` — 2^31 is hit before the cast |
| `memcpy(d, s, int(n))` / `arr \|> resize(int(n64))` with a 64-bit `n` | `memcpy(d, s, n)` / `resize(n64)` — the 64-bit overloads exist | LINT018 |
| `range(int(n64))` / `urange(uint(u64))` | `range64(n64)` / `urange64(u64)` | LINT020. Vector components and `string` index stay 32-bit |
| a 64-bit local whose every use sits under `int(...)` | narrow once at the declaration, or lift the sinks to 64-bit | LINT021 |
| `def f(var why : string)` written on a path but never read | `var why : string&` — a by-value copy's write never reaches the caller | LINT023 |
| bare `resize(need)` on an input-scaled buffer (frames, pixels, vocab) | declare it `@exact_size`; then `reserve`/`ensure_capacity` before every `resize` (or size it through `reserve_resize`-style helpers) | PERF032 — the annotation is a lint contract; the guard panics only when the big input arrives |
| `-const` `-&` `-[]` `-#` `==const` `==&` on a **concrete** cast target | drop the contract | STYLE036: substitution contracts act only while a generic binds — inert on concrete targets |
| `slice(s, i, j)` / `chop(s, i, n)` in a loop over an outer string | `peek_data(s) $(d)` and slice the view | PERF031: each call re-strlens the whole source — O(n²); every haystack op has a byte-view twin |

For path/filename ops use `fio` helpers (`base_name`/`dir_name`/`path_join`/etc.) — see `skills/daslang/references/files-and-paths.md`. Never hand-roll `rfind("/")` / slice — misses Windows separators.

**Inline literals over temp-var-and-push:** for a short array consumed in one expression, write `stack([a, b, c])` rather than `var xs : array<T>; xs |> emplace(a); xs |> emplace(b); stack(xs)`. Faster interpreted and easier to read; same for table literals and other bracketed constructors. Threshold: while it stays readable.

**`print` is fine for application scripts.** In library and tool code prefer `to_log(LOG_INFO|LOG_WARNING|LOG_ERROR)` — same stdout, but level-tagged and filterable.

**Minimize `unsafe`:** Most `unsafe(reinterpret<T?>)` in macro code exists to strip `const` from raw-pointer field access. Fix the root cause: make the function parameter `var` so field access returns non-const pointers. Reserve `unsafe` for genuinely unsafe operations (pointer arithmetic, `reinterpret` across unrelated types).

**Comment hygiene.** Comments are 1–2 lines max. Strict rules:

1. **No banner comments above a documented function.** When a function carries `//!` inside its body, drop the `// ===== name — desc =====` block above. The banner duplicates the doc.
2. **No multi-paragraph architectural prose at the head of a section.** Don't write 10–30 line preambles explaining design decisions, surface examples, NULL handling, panic semantics, etc. above `// Section name`. Code reads well; design docs carry the WHY. If a reader genuinely needs that context, it goes in those docs, not the source.
3. **Private functions and types don't get public-style docs.** `//!` / `//!<` is for tooling-visible public API. On `def private`, `struct private`, `enum private`, `variant private`, drop the docstring entirely — the symbol isn't exported, so no doc generator ever sees it, and the docstring just restates the function name / field name to a reader who already has them. If a function or field genuinely needs a 1-line WHY (non-obvious invariant, surprising behavior), write a plain `// ...` line, not `//!`. The bar for keeping any comment on a private symbol is "a maintainer reading the symbol alone would be surprised."
4. **What stays:** terse 2-line section dividers (`// ===== Section name =====`), `//!` docstrings on PUBLIC functions/types (visible to tooling), and inline `//` comments that flag a non-obvious WHY at the *exact* line — a workaround for a specific bug, a subtle invariant, behavior that would surprise a reader. Don't restate what the code says.
5. **When in doubt:** delete. If reading the code + the relevant docstring(s) doesn't make the WHY clear, the comment was load-bearing. Otherwise it was noise.

## SDK Directory Layout

- `bin/` — Compiler and tool binaries: `daslang`, `daslang-live`, `gen1_to_gen2`, plus prebuilt tool exes (`lint.exe`, `das-fmt.exe`, `daspkg.exe`, `dascov.exe`, `detect-dupe.exe`, `benchctl.exe`, `dastest.exe` — the `.exe` suffix on every platform) and shared-module / tree-sitter libraries on Windows. Each tool also keeps its source form in the bundle — `bin/daslang utils/<tool>/main.das`, except das-fmt (`utils/das-fmt/dasfmt.das`) and dastest (`dastest/dastest.das`). **Name trap:** `bin/gen1_to_gen2` is the gen1→gen2 syntax *converter*, not a code formatter. The formatter is `das-fmt` — the MCP `format_file` tool, `bin/das-fmt.exe`, or `bin/daslang utils/das-fmt/dasfmt.das`
- `lib/` — Static and shared libraries for C++ embedding
- `include/daScript/` — C++ headers for embedding
- `daslib/` — Standard library modules (.das files)
- `modules/` — Optional plugin modules (dasHV, dasGlfw, dasPUGIXML, dasSQLITE, dasAudio, dasLLVM, dasLLAMA, etc.)
- `examples/` — Example scripts
- `tutorials/` — Language, integration, and module tutorials
- `dastest/` — Test framework (usable for testing your own code)
- `utils/mcp/` — MCP server for AI coding assistants (stdio transport, no extra deps)
- `utils/lsp/` — LSP server for Claude Code (push diagnostics + navigation; auto-loads for sessions started at the SDK root, needs `python3` on PATH)
- `utils/lint/` — Lint runner: `bin/daslang utils/lint/main.das -- <files> --quiet`
- `utils/das-fmt/` — The code formatter script (`dasfmt.das`, wraps `daslib/das_source_formatter`)
- `utils/detect-dupe/` — Cross-file duplicate-function detector (also exposed via the `export_corpus` and `detect_duplicates` MCP tools)
- `utils/find-dupe/` — Claude-based judge for detect-dupe reports (needs `ANTHROPIC_API_KEY`; also the `judge_duplicates`/`find_dupe` MCP tools)
- `utils/daspkg/` — Package manager
- `utils/dascov/` — Code coverage tool
- `utils/aot/` — AOT generation driver (`bin/daslang utils/aot/main.das -- -aot <in.das> <out.cpp>`; `-ctx` emits a standalone context dir — the integration tutorial scaffolds invoke it)
- `utils/fix-lint-errors/` — Auto-fixer for mechanical lint findings (`--dry-run` to preview)
- `utils/benchctl/` — Benchmark result database + statistical comparison (needs the sqlite module)
- `utils/dasllama-server/` — OpenAI-compatible dasLLAMA inference server (JIT-only; `deploy-jit.ps1` builds a standalone bundle)
- `utils/dasllama-convert/` — Offline GGUF → `.dlim` model prep
- `utils/watchdog/` — Python supervisor for long-running daslang programs (`python utils/watchdog/watchdog.py --cwd <program-dir>`)
- `utils/jobque-timeline/` — Per-lane jobque trace viewer (ImGui)
- `utils/vscode-daslang-test/` — VSCode Test Explorer extension for dastest (ships as source; build per its README)
- `tree-sitter-daslang/` — Tree-sitter grammar, shared library, and highlighting queries (`sgconfig.yml` at the SDK root wires ast-grep to it)

## Package Manager (daspkg)

daspkg manages daslang packages — install from GitHub or local paths, resolve dependencies, build native modules.

```bash
bin/daslang utils/daspkg/main.das -- <command> [args]
```

Commands: `install <source>`, `update [name]`, `upgrade`, `remove <name>`, `list`, `search <query>`, `check`, `cleanup`, `doctor`, `build [--global]` (builds all native packages under the root), `release`. Full list and flags: `skills/daspkg.md`.

Packages install to `modules/<RepoName>/`. Lock file: `daspkg.lock`.

See `skills/daspkg.md` for `.das_package` manifest format and package structure.

## MCP Server (AI Tool Integration)

`utils/mcp/` contains a [Model Context Protocol](https://modelcontextprotocol.io/) server that exposes compiler diagnostics and program introspection to AI coding assistants. Stdio transport — no extra build dependencies. **Prefer MCP tools** over manual compilation and grep — `grep_usage` is parse-aware (tree-sitter), `find_references` resolves cross-module symbols, and `live_*` tools talk to `daslang-live` directly instead of curl.

Full tool table (including `detect_duplicates`/`judge_duplicates`/`find_dupe`), live-API caveats, and `.mcp.json` configuration: **`skills/mcp_tools.md`**.

## LSP Server (Push Diagnostics + Navigation)

`utils/lsp/` is a language server for `.das`: the compiler and lint report after **every** edit with no explicit tool call, plus definition / references / hover / document & workspace symbols / call hierarchy / go-to-implementation. Needs `python3` on `PATH`. Claude Code sessions started at the SDK root load it automatically (shipped `.claude/skills/daslang-lsp/` manifest); elsewhere use `claude --plugin-dir <sdk-root>/utils/lsp/plugin`. Configuration (compiler override, `project_root` for native modules, `.das_project`): `utils/lsp/README.md`.
