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
- **Pass arguments to the script:** everything after `--` is forwarded to the script. `bin/daslang script.das -- arg1 arg2`. Use `daslib/clargs` to parse them — see `skills/clargs_usage.md`.
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
| `skills/cpp_integration.md` | Embedding daslang in C++; binding types/functions/enums |
| `skills/daslib_modules.md` | Using `daslib/` modules (linq, json, regex, etc.) |
| `skills/das_macros.md` | Compile-time macros, AST manipulation, qmacro/quote, gc_node patterns |
| `skills/daspkg.md` | Creating `.das_package` manifests, daspkg commands |
| `skills/clargs_usage.md` | Writing daslang CLI tools — declarative argv parsing via `daslib/clargs` |
| `skills/dynamic_modules.md` | `.das_module` descriptors, module resolution, `register_native_path` |
| `skills/external_module_debugging.md` | Iterating on a daslang module outside the SDK tree (dasImguiNodeEditor, dasCards, or your own daspkg package) — run/lint/test from a standalone `daslang` or via MCP without a full `daspkg install` (junction pattern + `project_root` MCP arg) |
| `skills/daslang_live.md` | `daslang-live` lifecycle, REST API, `[live_command]`, persistent state |
| `skills/imgui_ui_debugging.md` | Diagnosing or fixing any dasImgui UI / interaction bug — the discipline: reproduce → make it observable in `imgui_snapshot` → fix → prove via snapshot + test. Never claim a UI fix works from logic or a screenshot alone |
| `skills/json.md` | Reading/writing JSON (`sprint_json`/`sscan_json`, `JV`, manual `JsonValue?`) |
| `skills/xml.md` | XML via `dasPUGIXML`/`PUGIXML_boost` (RAII, builder, XPath, struct round-trip) |
| `skills/filesystem.md` | Any `.das` path/filename/filesystem op — must use `fio` helpers, never `rfind`/`slice` |
| `skills/writing_tests.md` | Writing tests with the bundled `dastest` framework |
| `skills/tdd_audit.md` | Auditing any diff for test coverage — does every new/changed branch have a test that fails without it? The negative-control procedure, pin and expectation discipline, the reporting shape |
| `skills/memory_leak_detection.md` | Diagnosing leaks (`--das-profiler-leaks`, `--track-smart-ptr`, `GC APP LEAK`, `HandleRegistry`) |
| `skills/jobque_debugging.md` | Channel/LockBox/JobStatus/Stream/Feature leaks |
| `skills/detect_dupe.md` | Duplicate-function detection (corpus, MCP tools `export_corpus`/`detect_duplicates`, CLI under `utils/detect-dupe/`) |
| `skills/detect_dupe_reference.md` | The long-form detect-dupe reference `detect_dupe.md` points at — every flag, in tables |
| `skills/find_dupe.md` | AI-judging a detect-dupe report via Claude (MCP tools `judge_duplicates`/`find_dupe`, CLI under `utils/find-dupe/`, needs `ANTHROPIC_API_KEY`); cost guardrails (`--dry-run`, `--max-clusters`, `--positives-only`) |
| `skills/linq.md` | Filter/map/sort/group/aggregate transforms — prefer comprehension → linq_boost → plain `for`; avoid `daslib/functional` for new code |
| `skills/decs.md` | Programming with `daslib/decs` / `decs_boost` — entities, components, queries, `[decs_template]`, stages, bulk creation, `from_decs` linq bridge |
| `skills/regex.md` | Writing regular expressions in `.das` code |
| `skills/strings.md` | Any `.das` string operation — `find`/`replace`/`split`/parsing/`build_string`/`peek_data` (covers `strings`, `daslib/strings_boost`, `daslib/strings_convert`) |
| `skills/glob.md` | Writing or reviewing any glob/wildcard pattern handling — file selection, include/exclude masks, pattern-match-on-paths (`*` / `?` / `**` / `[abc]`) |
| `skills/sql.md` | SQL via `daslib/sql_linq` + providers (`dasSQLITE` / `dasDuckDB` / `dasPostgreSQL`) — `[sql_table]` / `[sql_view]` / `[sql_fts5]` / `[sql_function]`, the `_sql(...)` LINQ-to-SQL flagship, per-provider capability gates, custom-type adapters, `@sql_json` / `@sql_blob` columns, transactions, migrations |
| `skills/dashv.md` | Any `.das` that serves HTTP, streams SSE, makes outbound HTTP requests, or tests a server — `HvWebServer` routes (`GET`/`POST`/`STREAM`/…), buffered vs streaming responses, the SSE writer ops, the client API (`GET`/`POST`/`request`/`with_http_request`/`get_body_bytes`), the `with_test_server` harness, and the per-context / `options stack` gotchas |
| `skills/gc_migration.md` | Migrating older code (external repos, archived projects) from `smart_ptr<T>` AST patterns to gc_node |
| `skills/strudel_port.md` | Porting strudel.cc patterns into `dasStrudel` |
| `skills/llvm_tune.md` | The `[tune]` kernel-tuning framework — `[tune_perm]`/`[tune_scope]`/`--tune`, per-box manifests, the runtime-tune-and-re-exec model, the AOT/`-exe` gates |
| `skills/tune.md` | The long-form `[tune]` reference that `llvm_tune.md` points at — the full permutation/scope/policy surface |
| `skills/profiler.md` | Runtime profiling and `--das-profiler-leaks` — the full guide `memory_leak_detection.md` points at |
| `skills/linq_fold_patterns.md` | Which chain shapes `_fold(...)` recognizes — the full pattern reference `linq.md` points at |
| `skills/perf_lint.md` | Adding or interpreting `PERF*` rules in `daslib/perf_lint.das` — **and before declaring any hot path off-limits to allocation**: `[hot_path]` / `[no_alloc]` / `[no_env]` / `[no_io]` contracts (PERF026-028), `[cold_path]` to prune, `@scratch` to declare a reused buffer |
| `skills/style_lint.md` | Adding or interpreting `STYLE*` rules in `daslib/style_lint.das` |

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
| `var X = clone_expression(E); ... $e(X) ...` (only-uses-are-qmacro-splice) | drop the pre-clone, inline `$e(E)` at each splice site | PERF023: `qmacro`/`qmacro_block`/`qmacro_expr`/`qmacro_block_to_array` go through `apply_template` in templates_boost.das, whose substitution visitor calls `clone_expression` on every substitution input. Pre-cloning is wasted work. Detection: post-expansion `$e(X)` becomes `add_ptr_ref(X)` inside an `ExprMakeBlock`; visitor tracks splice-wrapper depth via preVisitExprCall/visitExprCall counter on `add_ptr_ref`, classifies each candidate `ExprVar` reference as "safe" when depth>0. Fires only when ALL uses are safe AND ≥1 is observed. Multi-clone-of-same-source flagged too — apply_template clones each substitution independently |
| `var t : table<K;V>; t \|> insert(k1, v1); t \|> insert(k2, v2)` (or `t[k] = v` runs, or 2-arg set inserts) | `var t <- { k1 => v1, k2 => v2 }` (set: `var s <- { k1, k2 }`) | STYLE031: ≥ 2 contiguous inserts/`[]=` after an empty table decl collapse to a literal move-assign. Computed keys fine; runs with a duplicate CONST key stay silent (literal duplicates are `error[30706]`, inserts overwrite). `table<string; JsonValue?>` const-key runs get STYLE021's `JV((k1=...))` form instead |
| `var w : array<T>; w \|> push_from(SRC)` (empty decl + single bulk copy from an `array<T>`) | `var w := SRC` (clone-assign); if then `return <- w`, `return clone_to_move(SRC)` | STYLE032 (the init half) + PERF009-clone (the return half) — they compose. A bulk `push_from`/`push_clone_from` into a fresh-empty array IS a clone-init. Array source only; a C-array source stays silent (`var w := cArray` ≠ `array<T>`). Only the statement immediately after the decl is inspected — a `reserve`/guard between keeps it quiet. PERF009 clone variant: `var x := src; return <- x` → `return clone_to_move(src)`, NOT `return <- src` (would move/destroy the source) |
| hand-rolled `is X` / `as X` / null-guard / `ExprRef2Value`-peel ladders in macro code | `qmatch(e, $e(a) + $e(b))` for source-syntax shapes; `match (e) { if (ExprField(name = "key", value = ExprVar(...))) { ... } }` for node-class shapes | both matchers peel `ExprRef2Value` automatically; `\|\|` alternation, `&&` guards, and `match_expr(local)` cover most ladders. Limits + the qmatch↔match division of labor: `skills/das_macros.md` "`match` (daslib/match)" |
| `unsafe(reinterpret<T?>(unsafe(addr(x))))` (reinterpret-of-addr, pointer target) | `unsafe(addr<T?>(x))` | STYLE034: same AST, one unsafe gate instead of two. Non-pointer puns (`reinterpret<uint64>(addr(x))`) have no `addr<T?>` spelling and stay silent; the sugar's own desugared output is exempt |
| `b == T('(')` where `b : T` and T is a non-`int` built-in numeric scalar (also `!=`, ranges, and Yoda forms) | make `b : int`, then write `b == '('` | STYLE035: character literals are `int`; changing the plain variable once removes every repeated numeric cast. The warning is deduplicated at the declaration. |
| `int64(length(x))` / `uint64(capacity(x))` (also `count`, `find_index`, `fread`, `fwrite`) | `long_length(x)` / `long_capacity(x)` / … | LINT017: the inner call returns `int`, so the 2^31 limit is hit *before* the widening cast — as a wrap, or as the panic guard on array/table/string length — and the cast buys nothing. Gated on receiver type, so same-named user overloads and the fixed-array `length` generic (no `long_` twin) stay silent |
| `memcpy(d, s, int(n))` / `memcmp(d, s, int(n))` where `n` is `int64`/`uint64`/`uint` | `memcpy(d, s, n)` | LINT018: both carry `uint`/`int64`/`uint64` size overloads, so the `int(...)` narrowing is pure loss — above 2^31 it copies the wrong byte count |
| `cast<T -const>(x)` / `reinterpret<T -#>(x)` / `addr<void? -const>(x)` — any of `-const` `-&` `-[]` `-#` `==const` `==&` on a **concrete** cast target | drop the contract | STYLE036: these are *substitution* contracts — they act only while a generic binds, and infer clears them once consumed, so a flag still set at lint time proves it did nothing. `auto`/unresolved-alias targets are excluded (substitution hasn't run there); a concrete typedef is NOT (`reinterpret<CI? -const>` with `typedef CI = int const` keeps the const) |

For path/filename ops use `fio` helpers (`base_name`/`dir_name`/`path_join`/etc.) — see `skills/filesystem.md`. Never hand-roll `rfind("/")` / slice — misses Windows separators.

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

- `bin/` — Compiler and tool binaries: `daslang`, `daslang-live`, `gen1_to_gen2` (plus shared-module and tree-sitter libraries on Windows). **Name trap:** `bin/gen1_to_gen2` is the gen1→gen2 syntax *converter*, not a code formatter. The formatter is `das-fmt`, and it ships as a script, not a binary — format with the MCP `format_file` tool or `bin/daslang utils/das-fmt/dasfmt.das`
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
- `utils/aot/`, `utils/jit/` — AOT-generation and JIT helper scripts
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
