# daslang SDK Instructions

## Overview

[daslang](https://daslang.io/) (formerly daScript) is a high-performance statically-typed programming language designed for games and real-time applications, standalone or embedded. Many C++ API names still use the old "daScript" spelling.

## What and Why

daslang was created at Gaijin Entertainment to solve a concrete problem: **interop overhead** between scripting languages (Lua/LuaJIT, Squirrel) and C++ was killing frame budgets in their ECS game engine. The key insight is that daslang's data layout matches C++ — no marshaling, no boxing — making script↔C++ calls near-zero cost.

**Core design principles:**
- **Iteration speed is king** — full production game recompiles in ~5 sec; hot reload built in
- **Explicit, not implicit** — no hidden conversions, no silent allocations; `options log` shows exactly what the compiler produces
- **99% safe, not 100%** — eliminate real-world C++ bugs pragmatically, not at Rust-level cost; `unsafe` for the remaining 1%
- **No data marshaling** — C++-compatible data layout; this is what makes ECS-scale scripting viable
- **If it gets slow, you can fix it** — manual `delete` to reduce GC pressure, AOT compilation for native speed
- **Language reflects the domain** — macros (FORTRAN+LISP inspiration) let libraries reshape syntax to match the problem

**Three execution tiers** (all planned from day one): fast tree-based interpreter → AOT to C++ (required for consoles) → JIT via LLVM. Hybrid mode uses semantic hashing: unchanged functions stay AOT, changed ones fall back to the interpreter.

**Audience:** game scripters (largest group — hot reload, fast compile, never rewrite to C++), engine/tools programmers (zero-cost interop, macros), and a growing standalone/ecosystem community (LLVM executables, package manager in development).

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

`tutorials/language/01_hello_world.das` through `tutorials/language/53_clargs.das` are an ordered tour of the language — start there when learning a new feature. Each tutorial is a runnable `.das` file with comments explaining the construct. Module-specific tutorials live alongside (`tutorials/dasPUGIXML/`, `tutorials/dasHV/`, `tutorials/dasAudio/`, `tutorials/sql/`, `tutorials/macros/`, `tutorials/integration/`).

### Debugging

- **Always check the exit code** after running `daslang` — a crash may produce no output at all, looking like a silent success. PowerShell: `$LASTEXITCODE`. Bash/zsh: `$?`. Exit code `0` = success.
- Exit code `-1073741819` / `0xC0000005` (Windows) or `139` / `134` (Linux/macOS) = native crash — Access Violation / SIGSEGV / SIGABRT
- If the program crashes with no error message, the bug is in native code (C++ bindings or smart pointer misuse) — check exit code first
- **Don't truncate output** with `head`/`tail` — daslang stack traces are easily clipped. Capture full output, then `grep` if needed
- **`options log`** — append at the end of a `.das` file to dump the final post-compilation program text. Useful for confirming what the compiler actually produces (constant folding, generic reification, macro expansion).
- **`options log_infer_passes`** — append at the end of a failing `.das` file for a per-pass infer-pipeline dump (which generics got reified, when finalize ran, where lookups missed). Smaller and more targeted than `options log` for template/generic reification bugs.

## Skill Files (REQUIRED)

Task-specific instructions are in skill files under `skills/`. Read the relevant skill file(s) before performing the corresponding task.

| Skill file | Read BEFORE... |
|---|---|
| `skills/mcp_tools.md` | Full MCP tool table + live-API reference |
| `skills/das_formatting.md` | Creating or modifying any `.das` file |
| `skills/cpp_integration.md` | Embedding daslang in C++; binding types/functions/enums |
| `skills/daslib_modules.md` | Using `daslib/` modules (linq, json, regex, etc.) |
| `skills/das_macros.md` | Compile-time macros, AST manipulation, qmacro/quote, gc_node patterns |
| `skills/daspkg.md` | Creating `.das_package` manifests, daspkg commands |
| `skills/clargs_usage.md` | Writing daslang CLI tools — declarative argv parsing via `daslib/clargs` |
| `skills/dynamic_modules.md` | `.das_module` descriptors, module resolution, `register_native_path` |
| `skills/daslang_live.md` | `daslang-live` lifecycle, REST API, `[live_command]`, persistent state |
| `skills/json.md` | Reading/writing JSON (`sprint_json`/`sscan_json`, `JV`, manual `JsonValue?`) |
| `skills/xml.md` | XML via `dasPUGIXML`/`PUGIXML_boost` (RAII, builder, XPath, struct round-trip) |
| `skills/filesystem.md` | Any `.das` path/filename/filesystem op — must use `fio` helpers, never `rfind`/`slice` |
| `skills/writing_tests.md` | Writing tests with the bundled `dastest` framework |
| `skills/memory_leak_detection.md` | Diagnosing leaks (`--das-profiler-leaks`, `--track-smart-ptr`, `GC APP LEAK`, `HandleRegistry`) |
| `skills/jobque_debugging.md` | Channel/LockBox/JobStatus/Stream/Feature leaks |
| `skills/detect_dupe.md` | Duplicate-function detection (corpus, MCP tools `export_corpus`/`detect_duplicates`, CLI under `utils/detect-dupe/`) |
| `skills/linq.md` | Filter/map/sort/group/aggregate transforms — prefer comprehension → linq_boost → plain `for`; avoid `daslib/functional` for new code |
| `skills/regex.md` | Writing regular expressions in `.das` code |

Multiple skill files may apply to a single task. For example, embedding daslang and calling its standard library requires reading both `skills/cpp_integration.md` and `skills/daslib_modules.md`.

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
- **Inline literals over temp-var-and-push** — for short arrays consumed in one expression, write `stack([a, b, c])` rather than `var xs : array<T>; xs |> emplace(a); xs |> emplace(b); stack(xs)`. Faster in interpreted mode and easier to read; same applies to table literals

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

For migrating older code that uses `var inscope`/`<-` on AST types, see `skills/das_macros.md`.

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

- **`_::foo(x)`**: resolves in the **calling** module — caller's overloads visible. Use in library generics.
- **Unqualified** `foo(x)`: resolves in the **defining** module — caller's overloads NOT visible.
- This is why `:=` and `delete` emit `_::clone` / `_::finalize`

### Dot as pseudo-pipe

`a.foo(b)` is sugar for `foo(a, b)` — but **only when `a` is a struct/class value** (chains: `a.foo().bar(x)` ≡ `bar(foo(a), x)`).

- **Works on:** struct / class values (incl. by-ref).
- **Does NOT work on:** primitives (`let n = 5; n.double()` → "can't get field 'double' of int const&"), tuples/arrays, and **lambda typedefs**. For lambda-typed values you must use `|>`.
- **When telling someone "use pipe here":** check the receiver type — for structs `.method()` is idiomatic, for lambdas only `|>` works.

### Table operations

- `table[key]` **inserts** a default entry if missing — use `table?[key] ?? default` for safe lookup
- `key_exists(table, key)` — check without inserting
- `table |> insert(key, value)` / `table |> erase(key)`
- **Never use two `[]` lookups on the same table in one expression** — re-hashing can invalidate references
- **Move-assign table literal:** `tab <- { "k" => v }` works for both `var tab <- { ... }` declarations and `tab <- { ... }` reassignment to existing variables
- **Table comprehension move-assign:** `tab <- { for(x in range(5)); x => x*x }` — same move-assign rules apply
- `table[key]` (read or assign) is **safe** — do NOT wrap in `unsafe(...)`. Some legacy daslib code has `unsafe(tab[k])`; do not propagate that pattern

### Iterators and `each`

- `[unsafe_outside_of_for] def each(x) : iterator<T>` makes a type iterable in `for` loops
- When the iterator is named `each`, the call can be omitted: `for (v in each(x))` is identical to `for (v in x)`
- Other iterator names (e.g. `filter`, `map`) cannot be omitted

### String access functions

- **`peek_data(str) $(arr) { ... }`** — safe O(1) per-element read access to string as `array<uint8> const#`. One `strlen` call total. Preferred over `character_at` for iteration.
- **`modify_data(str) $(var arr) { ... }`** — returns a modified copy; allocates new string, opens as mutable `array<uint8>`. Use for character-level transformations.
- **`character_at(s, i)`** — O(n) per call (`strlen` + bounds check). Fine for isolated checks, but use `peek_data` in loops or hot paths.

### Common gotchas

- Lambda params can shadow function params — use distinct names
- String builder requires `unsafe` or `options persistent_heap` if returned
- Tuple field access: `t._0`, `t._1`, `t._2`
- Annotations: `[export]`, `[test]`; `options no_aot`, `options rtti`
- **Visibility is a prefix keyword, not an annotation:** `def private foo()`, `struct private Foo { ... }`, `enum private E { ... }`, `variable private x = 0`, `alias private X = Y`. There is **no** `[private]` annotation — it's a grammar error
- **Field/variable annotations use `@name` only:** `@safe_when_uninitialized at : LineInfo`, `@sql_primary_key id : int64`. The `[name]` form is reserved for struct/function/global-level annotations and does NOT parse on a struct field
- `require` uses forward slash: `require daslib/linq` — NOT backslash
- `require foo public` — re-exports `foo` transitively
- `[export] def main()` defaults to returning `void`, but you can declare it as `def main() : int { ... return rc }` when you need to surface a non-zero process exit code (e.g. CLI tools whose callers branch on exit). Don't reach for `panic` just to force a non-zero exit; declare `: int` and `return rc` instead.
- `push` copies (fails for non-copyable types), `emplace` moves (zeros source), `push_clone` clones (preserves source)
- Non-copyable types (`array<T>`, `table<K;V>`, lambdas): use `:=`, `push_clone`, or `<-`
- Blocks cannot be stored/returned/captured — use lambdas or function pointers
- Class methods: `def const`, `def abstract const`, `def static`; call syntax `obj.method()`, `obj->method()`, `obj |> method()`
- **`is`/`as` on handled types checks EXACT type**, not C++ inheritance — `expr is ExprField` is `false` when `expr` is `ExprSafeField`. `as` on wrong type crashes. Must handle each concrete type explicitly.
- `#pragma optimize` in AOT-generated code must be wrapped in `#ifdef _MSC_VER` — Clang warns on unknown pragmas
- **Macro-generated struct variables** need `default<$t(st)>` initialization (not `var x : $t(st)`) — avoids "uninitialized variable" errors for structs without field defaults

### Code style — prefer idiomatic forms

| Don't write | Write instead | Why |
|---|---|---|
| `string(x.__rtti) == "ExprFoo"` | `x is ExprFoo` | `is` works directly on AST pointers |
| `get_ptr(x) == null` / `get_ptr(x).field` | `x == null` / `x.field` | AST pointers auto-dereference; `get_ptr` is smart_ptr-era residue |
| `string(das_str) == "lit"`, `!empty(string(das_str))` | drop the `string(...)` cast | `das_string` compares with `string` directly; `empty()` works on it |
| `let v = string(x.name); $i(v)` / `var copy = val; $v(copy)` | `$i(x.name)` / `$v(val)` | qmacro tags accept `das_string`, `let` vars, loop vars directly |
| `if (true) { ... }` | `{ ... }` | bare blocks create lexical scope in gen2 |
| `var inscope r <- expr; return <- r` | `return <- expr` | direct return avoids intermediate |
| `unsafe { (reinterpret<ExprBlock?> blk).list }` / `unsafe(reinterpret<T?> x)` | make param `var` + plain `x.list` | `var` param gives non-const field access without reinterpret |

For path/filename ops use `fio` helpers (`base_name`/`dir_name`/`path_join`/etc.) — see `skills/filesystem.md`. Never hand-roll `rfind("/")` / slice — misses Windows separators.

## SDK Directory Layout

- `bin/` — Compiler binaries (`daslang`, `daslang-live`, `das-fmt`, `clang-cl`, `sqlite_shell`)
- `lib/` — Static and shared libraries for C++ embedding
- `include/daScript/` — C++ headers for embedding
- `daslib/` — Standard library modules (.das files)
- `modules/` — Optional plugin modules (dasHV, dasGlfw, dasImgui, dasPUGIXML, etc.)
- `examples/` — Example scripts
- `tutorials/` — Language, integration, and module tutorials
- `dastest/` — Test framework (usable for testing your own code)
- `utils/mcp/` — MCP server for AI coding assistants (stdio transport, no extra deps)
- `utils/detect-dupe/` — Cross-file duplicate-function detector (also exposed via the `export_corpus` and `detect_duplicates` MCP tools)
- `utils/daspkg/` — Package manager
- `utils/dascov/` — Code coverage tool
- `tree-sitter-daslang/` — Tree-sitter grammar, shared library, and highlighting queries

## Package Manager (daspkg)

daspkg manages daslang packages — install from GitHub or local paths, resolve dependencies, build native modules.

```bash
bin/daslang utils/daspkg/main.das -- <command> [args]
```

Commands: `install <source>`, `update [name]`, `remove <name>`, `list`, `search <query>`, `check`, `doctor`, `build <name>`.

Packages install to `modules/<RepoName>/`. Lock file: `daspkg.lock`.

See `skills/daspkg.md` for `.das_package` manifest format and package structure.

## MCP Server (AI Tool Integration)

`utils/mcp/` contains a [Model Context Protocol](https://modelcontextprotocol.io/) server that exposes compiler diagnostics and program introspection to AI coding assistants. Stdio transport — no extra build dependencies. **Prefer MCP tools** over manual compilation and grep — `grep_usage` is parse-aware (tree-sitter), `find_references` resolves cross-module symbols, and `live_*` tools talk to `daslang-live` directly instead of curl.

Full tool table (including `detect_duplicates`/`judge_duplicates`/`find_dupe`), live-API caveats, and `.mcp.json` configuration: **`skills/mcp_tools.md`**.
