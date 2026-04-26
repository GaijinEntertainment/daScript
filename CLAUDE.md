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

## Build & Run

- **Build system:** CMake + MSVC (Visual Studio 2022)
- **Generate:** `generate_msvc_2022.bat` → creates `build/DAS.sln`
- **Build:** `cmake --build build --config Release -j 64 -- /nodeReuse:false`
- **Compiler binary:** `bin/Release/daslang.exe`
- **Live-reload host:** `bin/Release/daslang-live.exe` — same script runs in both; see `utils/daslang-live/main.cpp`
- **Run a script:** `bin/Release/daslang.exe path/to/script.das`
- **Compile-only check:** `bin/Release/daslang.exe -compile-only path/to/script.das` — compiles without simulation or execution, useful for syntax/type checking without needing a window or GL context. Use `-dry-run` to also simulate (but not execute).
- **Run tests:** `bin/Release/daslang.exe dastest/dastest.das -- --test path/to/test.das`
- **AOT tests:** `cmake --build build --config Release --target test_aot -- /nodeReuse:false` then `bin/Release/test_aot.exe -use-aot dastest/dastest.das -- --use-aot --test tests`
- **IMPORTANT:** When adding a new test directory under `tests/`, register it in `tests/aot/CMakeLists.txt` for AOT compilation. See `skills/aot_testing.md` for the step-by-step pattern. CI runs ALL tests with AOT enabled — unregistered test directories cause `error[50101]: AOT link failed`

### Build Timing

- **Builds are slow** — clean builds take **15-25 minutes**, incremental builds take **2-10 minutes** depending on what changed
- **Always use `timeout: 0`** (no timeout) when running `cmake --build` commands in the terminal. Never set a short timeout on build commands — a build that hasn't finished is not stuck or broken, it's just compiling
- **Do not assume build failure** from lack of output — MSVC is silent during compilation and only prints when there are warnings/errors or when it finishes
- **Wait for the build to complete** before drawing any conclusions. If a terminal command times out, check the output — it likely just needed more time
- For incremental builds after editing a single `.cpp` file, expect ~2-5 minutes. For changes touching headers, expect longer

### Debugging

- **Always check the exit code** after running `daslang.exe` — a crash may produce no output at all, looking like a silent success
- On Windows, check `$LASTEXITCODE` in PowerShell after every run. Exit code `0` = success, non-zero = error
- Exit code `-1073741819` (`0xC0000005`) = **Access Violation** — indicates a native crash (segfault)
- If the program crashes with no error message, the bug is in native code (C++ bindings or smart pointer misuse) — check exit code first
- **Don't truncate output** with `head`/`tail` — daslang stack traces and `DAS_GC_BREAK_ON_ID` traces are easily clipped. Capture full output, then `grep` if needed
- **`options log_infer_passes`** — append at the end of a failing `.das` file to dump per-pass infer activity (which generics got reified, when finalize ran, where lookups missed). Smaller and more targeted than `options log` for template/generic reification bugs; `options log` stays the right tool when you need the final program text

### Build Configurations (Module Flags)

Optional modules are controlled by CMake flags (`DAS_*_DISABLED`). The active configuration lives in `.vscode/settings.json` under `cmake.configureSettings` (the "WIP" block is the active one; others are commented-out presets).

Key flags (all default to `ON` = disabled in CMakeLists.txt):
- `DAS_HV_DISABLED` — dasHV (HTTP/WebSocket via libhv)
- `DAS_PUGIXML_DISABLED` — dasPUGIXML (XML parsing)
- `DAS_GLFW_DISABLED` — GLFW (OpenGL windowing)
- `DAS_IMGUI_DISABLED` — ImGui
- `DAS_LLVM_DISABLED` — LLVM JIT
- `DAS_CLANG_BIND_DISABLED` — Clang bindings
- `DAS_AUDIO_DISABLED`, `DAS_STBIMAGE_DISABLED`, `DAS_STBTRUETYPE_DISABLED`, `DAS_STDDLG_DISABLED`, `DAS_SQLITE_DISABLED`

**To change modules:** Edit the active `cmake.configureSettings` in `.vscode/settings.json`, then reconfigure:
```
cmake --no-warn-unused-cli -B./build -G "Visual Studio 17 2022" -A x64 -DFLAG=VALUE ...
```
Or let VSCode CMake Tools pick up the settings change automatically.

**Documentation generation** (`doc/reflections/das2rst.das`) requires `DAS_HV_DISABLED=OFF` and `DAS_PUGIXML_DISABLED=OFF` because it documents all modules. Temporarily enable them, rebuild `daslang`, run das2rst, then revert settings.

### AOT Hash Debugging

When AOT fails with `error[50101]: AOT link failed`, the issue is a **semantic hash mismatch** between the generated C++ stubs and runtime. Each generated `.cpp` file has hash comments showing function hashes and dependency hashes. The runtime error also prints the same breakdown. Compare them to find the diverging function or dependency. See `skills/aot_testing.md` for the full debugging guide (hash architecture, debug macros, common causes).

The AOT C++ emitter lives in **`daslib/aot_cpp.das`** (the old `src/ast/ast_aot_cpp.cpp` was emptied by commit `581363ebc`). When codegen output diverges, edit `daslib/aot_cpp.das` — not the C++ stub in `src/ast/`.

## GitHub Operations

- **Use GitHub MCP tools** (`mcp__github__*`) for all GitHub operations (creating PRs, listing issues, reading PRs, etc.) — they avoid shell escaping issues entirely
- **Fallback:** If MCP tools are unavailable, use `gh` CLI with `--body-file` for any text containing backticks (backticks are shell escape characters in both PowerShell and bash heredocs on MSYS2)

## Skill Files (REQUIRED)

Task-specific instructions are split into skill files under `skills/`. You MUST read the relevant skill file(s) before performing the corresponding task.

| Skill file | Read BEFORE... |
|---|---|
| `skills/das_formatting.md` | Creating or modifying any `.das` file (tutorials, tests, daslib modules, utilities) |
| `skills/writing_tests.md` | Writing or editing test files under `tests/` |
| `skills/documentation_rst.md` | Editing RST files in `doc/source/`, editing `//!` doc-comments in `daslib/*.das`, writing tutorial RST pages |
| `skills/tutorials.md` | Creating, moving, or restructuring tutorial `.das` files. **Always check before editing anything that looks like a tutorial** — tutorials live under `/tutorials/<area>/`, NEVER inside `modules/<X>/tutorial/` (which holds inherited examples) |
| `skills/cpp_integration.md` | Writing or editing C++ files in `src/`, `modules/`, or `tutorials/integration/cpp/` |
| `skills/daslib_modules.md` | Working with `daslib/` modules (linq, json, regex, functional, match, etc.), channels, or extending the standard library |
| `skills/das_macros.md` | Writing compile-time macros, AST manipulation, qmacro/quote code generation, gc_node AST-pointer patterns |
| `skills/writing_benchmarks.md` | Writing or running benchmark files under `benchmarks/` |
| `skills/daspkg.md` | Running daspkg commands, creating `.das_package` manifests, package structure |
| `skills/dynamic_modules.md` | Creating or editing `.das_module` descriptors, adding new modules under `modules/` |
| `skills/install_instructions.md` | Creating or updating AI instruction files (`install/CLAUDE.md`, `install/skills/`) for the installed SDK |
| `skills/aot_testing.md` | Adding AOT test files, working with the `test_aot` binary, `Module::aotRequire()`, CMake AOT macros, **debugging AOT hash mismatches** |
| `skills/visitor_gen_bind.md` | Adding or modifying `Visitor` virtual methods, `canVisit*` gates, running `gen_bind.das`, updating adapter bindings in `ast_gen.inc` |
| `skills/daslang_live.md` | Working with `daslang-live.exe`, live-reload lifecycle, REST API, `[live_command]`, `[before_reload]`/`[after_reload]`, persistent store, `live/glfw_live`, `live/live_api` |
| `skills/perf_lint.md` | Adding new performance lint rules to `daslib/perf_lint.das` |
| `skills/style_lint.md` | Adding new style lint rules to `daslib/style_lint.das` |
| `skills/gc_migration.md` | Migrating older `.das` or C++ code (external repos, archived branches) from `smart_ptr<T>` AST patterns to the current gc_node form. The in-tree migration is **complete** — only TypeDecl, Expression, Function, Structure, Enumeration, Variable, MakeFieldDecl, MakeStruct, every Annotation subclass; only `Program`, `Context`, `FileAccess` remain `smart_ptr` |
| `skills/version_update.md` | Bumping the daslang version number (all files that need updating) |
| `skills/jobque_debugging.md` | Debugging Channel/LockBox/JobStatus/Feature leaks using the tracking system (`--track-job-status`, `DumpJobQueLeaks`, refcount tracing) |
| `skills/make_pr.md` | Creating a pull request (lint, test, AOT build+test, format checklist) |
| `skills/strudel_port.md` | Copy-pasting a strudel.cc pattern (user-level live-coding expression) into daslang |
| `skills/gc_use_after_sweep.md` | Debugging crashes in `TypeDecl`/`Expression` copy-ctors (`bad_alloc`, `length_error`, `basic_string::_M_create`) — gc_node use-after-sweep, `DAS_GC_DEBUG`, `DAS_GC_BREAK_ON_ID`, copy-on-mutate fix pattern |
| `skills/clargs_migration.md` | Editing any in-tree tool that still calls `get_command_line_arguments()` directly — migrate its argv parsing to `daslib/clargs` in the same PR (`utils/lint`, `utils/aot`, `utils/dasFormatter`, `utils/benchctl`, `utils/mcp`, `utils/daslang-live`, `daslib/debug`, `daslib/ansi_colors`, etc.) |

Multiple skill files may apply to a single task. For example, creating a new daslib module requires reading `skills/das_formatting.md`, `skills/daslib_modules.md`, and possibly `skills/documentation_rst.md`.

**Formatter reminder:** Use the MCP `format_file` tool to format `.das` files. It calls `daslib/das_source_formatter` directly. Do NOT use `utils/dasFormatter/` (that is the v1→v2 syntax converter, not a code formatter).

### Updating Instructions with New Knowledge

When you discover something new about daslang syntax, semantics, or conventions — whether through compiler errors, user corrections, or experimentation — **update this file** with the new knowledge. If it relates to a specific skill area, update the relevant `skills/*.md` file instead.

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

- `strict_smart_pointers` is ON — but the only types that are still `smart_ptr` are `Program` (`ProgramPtr`), `Context` (`ContextPtr`), `FileAccess` (`FileAccessPtr`), and a handful of internal helpers (`DebugAgentPtr`, `VisitorAdapterPtr` from `make_visitor`). Only those need `var inscope`. **All AST types** (TypeDecl, Expression, Function, Structure, Enumeration, Variable, MakeFieldDecl, MakeStruct, every `Annotation` subclass) are now plain raw pointers (gc_node), and `var inscope` does NOT apply to them — see "AST nodes (gc_node)" below
- No implicit type promotion: `int + float` is a compile error — both sides must match
- No `bool(int)` cast — use `x != 0`; no `string(bool)` — use `"{flag}"`
- `int("123")` does NOT work — use `to_int` from `require strings`
- Hex literals are `uint` by default — use `int(0x3F)` for int
- **`default<T>`** — the default (zero) value of type `T`: `default<int>` is `0`, `default<string>` is `""`, `default<float>` is `0.0f`. The body of the called function CAN use the value freely.
- **`type<T>`** vs **`default<T>`** as a witness argument — they are **not** interchangeable:
  - `type<T>` is a no-stack, no-construction compile-time type tag. The function body must NOT use the parameter (it will fail to compile if it does). Annotate with `[unused_argument(t)]`.
  - `default<T>` is a real zero-value of `T`. The body can read/pass it. Use `default<T>` when the called function's body might touch the witness; use `type<T>` only when the parameter exists purely for overload discrimination.
  - Smell: if you find yourself wanting to read a `type<T>` parameter inside the body, switch the call site to `default<T>` — don't rewrite the function.
- **`typedecl(expr)`** — compile-time type-of expression, usable inside `default<>`: `default<typedecl(field)>` gives the zero value of `field`'s type. Useful in generic code with `static_if` to compare against defaults.
- **Bitfield sizes**: `bitfield Name : uint8 { ... }`, `: uint16`, `: uint64`; default is `uint` (32-bit). Always unsigned.
- **Bitfield from expression**: `bitfield64(1ul << 13ul)` — use the constructor to create a bitfield value from an integer expression. Similarly `bitfield8()`, `bitfield16()`.

### Pass-by-value vs pass-by-reference

- Most types (structs, arrays, tables) always pass by reference — `&` is unnecessary on them
- Only **workhorse types** (`int`, `float`, `bool`, `string`, etc. — `isWorkhorseType` on the C++ side) pass by value
- **AST pointers (gc_node) pass by value** — `ExpressionPtr`, `TypeDeclPtr`, `FunctionPtr`, `StructurePtr`, `EnumerationPtr`, `VariablePtr`, `MakeFieldDeclPtr`, `MakeStructPtr`, `AnnotationPtr` and friends are all plain raw pointers. Passing them by value just copies the pointer — no refcount bump, no allocation. The underlying gc_node is owned by its `gc_root` (typically the Module), not by the caller.
  - `def foo(p : ExpressionPtr)` — caller passes a pointer; both sides reference the same node
  - `def foo(var p : ExpressionPtr)` — `var` lets you reassign `p` locally
  - `def foo(var p : ExpressionPtr&)` — pass by reference, so `p = newExpr` propagates back
  - Use `TypeDecl?` (never `TypeDecl const?`) — for mutable field access through the pointer, take the parameter as `var`
- **The few remaining `smart_ptr<T>` types** (`ProgramPtr`, `ContextPtr`, `FileAccessPtr`, `DebugAgentPtr`, `VisitorAdapterPtr`) **still use refcount semantics** — pass by value copies the smart_ptr (no refcount bump in daslang, since it's a raw pointer copy at the daslang level), but variables holding them require `var inscope` for cleanup. This is the narrow remaining surface where the smart_ptr rules in older docs still apply
- **`var s : string`** — writable local copy, changes do NOT propagate back to the caller
- **`var s : string&`** — pass by reference, changes propagate back. Use `&` for string out-parameters
- **`clone_string(s)`** — clones a string into the current context's heap. Required for cross-context calls where the source context may be destroyed
- **`:=`** on strings performs a clone (allocates in current context). Plain `=` copies the pointer

### AST nodes (gc_node) — unique ownership, clone to duplicate

Every AST node lives at exactly one location. Multiple `ExpressionPtr` or `TypeDeclPtr` values pointing at the same node are **shared references to one object**, not independent copies. The garbage collector tracks the node by its address; sticking the same pointer in two places does not create a second node.

- **Don't copy by value.** Inserting the same `ExpressionPtr` into two different parent expressions creates aliasing — both parents think they own the child, gc_collect walks it twice, mutations on one parent show up in the other, and the AST validator flags it.
- **Use the matching `clone_*` to duplicate:**
  - `clone_type(t)` for `TypeDeclPtr`
  - `clone_expression(e)` for `ExpressionPtr` (recursive deep clone)
  - `clone_function(f)` for `FunctionPtr` — note: still returns via move (`var x <- clone_function(f)`)
  - `clone_variable(v)` for `VariablePtr`
  - `clone_structure(s)` for `StructurePtr`
- **Don't use `var inscope`** for AST pointer types — the gc_node owns its own lifetime via `gc_root`. `var inscope` is for the residual smart_ptr types only (`ProgramPtr`, `ContextPtr`, `FileAccessPtr`).
- **Don't use `<-`** when assigning AST pointers — plain `=` is correct (`fn.body = newBlock`, `td.firstType = elemType`). `<-` was needed when these were smart_ptr; now it just memcpy+memset(0) a raw pointer slot, which is harmless but stylistically wrong. Keep `<-` only where the API still demands it (`clone_function`, `qmacro_function` returns).
- **Tools/utilities that build AST at runtime** (outside the normal compile pipeline) must wrap the scope in `ast_gc_guard() { ... }` from `daslib/ast`, or the leak detector reports `GC APP LEAK` at exit.
- **Don't use `clone_to_move`** on AST pointers as a substitute for `clone_*` — `clone_to_move` is the generic copy-then-move helper for non-copyable values like `array<T>`. For AST nodes the right call is the type-specific `clone_type` / `clone_expression` / etc.

This is the post-migration state. If you find yourself reading older guidance about `var inscope`, `<-`, or `clone_to_move` for AST types, the source is pre-migration — see `skills/gc_migration.md` for the conversion table.

### Memory and move semantics

- daslang has garbage collection — `delete` is not required in most code
- **No C++/Rust-style scope RAII for plain `var`** — a local `var arr : array<T>` declared in any scope (function body, if-block, loop body) does NOT finalize on scope exit; the heap allocation stays alive until the context tears down. To get cleanup you must either (a) declare with `var inscope` (smart_ptrs), (b) call `delete arr` explicitly before scope exit, or (c) move ownership out via `<-`. Per-frame leaks in hot paths usually trace back to a local `var arr : array<...>` that was never deleted (e.g. the strudel_visualizer 384 KB/frame leak fixed by adding `delete verts`)
- `var inscope` declares automatic cleanup; struct fields need defaults or `@safe_when_uninitialized`
- `var inscope` is legal inside `for` / `while` loop bodies — the loop's `finally` runs per iteration (on fall-through, `continue`, `break`, `return`), so each iteration finalizes its own scoped variables
- `<-` is memcpy+memset(0), NOT smart_ptr-aware. For the residual smart_ptrs (`ProgramPtr`, `ContextPtr`, `FileAccessPtr`) it bypasses refcount handling — see `skills/das_macros.md` for the patterns. For AST raw pointers it just shuffles a pointer slot, harmless but stylistically wrong (use `=`)

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
- `[export] def main()` returns `void` — do NOT return values from main
- `push` copies (fails for non-copyable types), `emplace` moves (zeros source), `push_clone` clones (preserves source)
- Non-copyable types (`array<T>`, `table<K;V>`, lambdas): use `:=`, `push_clone`, or `<-`
- Blocks cannot be stored/returned/captured — use lambdas or function pointers
- Class methods: `def const`, `def abstract const`, `def static`; call syntax `obj.method()`, `obj->method()`, `obj |> method()`
- **`is`/`as` on handled types checks EXACT type**, not C++ inheritance — `expr is ExprField` is `false` when `expr` is `ExprSafeField`. `as` on wrong type crashes. Must handle each concrete type explicitly.
- `#pragma optimize` in AOT-generated code must be wrapped in `#ifdef _MSC_VER` — Clang warns on unknown pragmas
- **Macro-generated struct variables** need `default<$t(st)>` initialization (not `var x : $t(st)`) — avoids "uninitialized variable" errors for structs without field defaults
- `print` should not be used in `tests` and in `daslib` folders. `to_log(LOG_INFO)` (or
other level) should be used instead.


### Code style — prefer idiomatic forms

| Don't write | Write instead | Why |
|---|---|---|
| `string(x.__rtti) == "ExprFoo"` | `x is ExprFoo` | `is` works directly on AST pointers |
| `get_ptr(x) == null` | `x == null` | AST pointers compare to `null` directly (also still works for the residual smart_ptrs) |
| `get_ptr(x).field` | `x.field` | AST pointers auto-dereference for field access; `get_ptr` is leftover from the smart_ptr era |
| `string(das_str) == "lit"` | `das_str == "lit"` | `das_string` compares directly with `string` |
| `!empty(string(das_str))` | `!empty(das_str)` | `empty()` works on `das_string` |
| `let v = string(x.name); $i(v)` | `$i(x.name)` | qmacro `$i`/`$f` accept `das_string` directly |
| `var copy = val; $v(copy)` | `$v(val)` | qmacro `$v` works with `let` vars and loop vars |
| `if (true) { ... }` | `{ ... }` | bare blocks create lexical scope in gen2 |
| `var inscope r <- expr; return <- r` | `return <- expr` | direct return avoids intermediate |
| `unsafe { (reinterpret<ExprBlock?> blk).list }` | `blk.list` | AST pointers auto-dereference |
| `unsafe(reinterpret<T?> x)` | make param `var` + plain `x` | `var` param gives non-const access, no reinterpret needed |

**Minimize `unsafe`:** Most `unsafe(reinterpret<T?>)` in macro code exists to strip `const` from raw-pointer field access. Fix the root cause: make the function parameter `var` so field access returns non-const pointers. Reserve `unsafe` for genuinely unsafe operations (pointer arithmetic, `reinterpret` across unrelated types).

## Key Directories

- `src/` — C++ compiler/runtime source
- `include/daScript/` — C++ headers
- `daslib/` — Standard library modules (86 .das files)
- `dastest/` — Test framework
- `tests/` — Test suite. See `tests/README.md` for full index
- `tests/aot/` — AOT compilation tests (built into `test_aot` binary)
- `doc/source/reference/language/` — RST language documentation
- `tutorials/language/` — Language tutorial `.das` files
- `tutorials/integration/cpp/` — C++ integration tutorials
- `modules/` — External plugin modules
- `modules/dasLiveHost/` — C++ module for live-reload host lifecycle (dynamic module)
- `utils/daslang-live/` — Live-reloading application host (`daslang-live.exe`)
- `utils/mcp/` — MCP server for AI coding assistants (30 tools, stdio transport, no extra deps)
- `utils/daspkg/` — Package manager (install, update, build, search packages)
- `examples/daslive/` — Live-reload examples (hello, triangle, tank_game, etc.)
- `examples/games/` — Full game examples (arcanoid, sequence) — run under daslang-live or daslang
- `examples/daspkg/` — Package manager example projects
- `examples/crash/` — Crash handler example (native + daslang stack traces)
- `tests/live_host/` — Unit tests for dasLiveHost module (lifecycle, commands, store)
- `include/daScript/misc/crash_handler.h` — Crash handler with daslang stack walk support

## MCP Server (AI Tool Integration)

The daslang MCP server (`utils/mcp/main.das`) exposes compiler diagnostics, program introspection, and live-reload control to AI coding assistants via the [Model Context Protocol](https://modelcontextprotocol.io/). Uses stdio transport — no extra build dependencies.

**When MCP tools are available**, prefer them over manual compilation and grep-based exploration. **For searching `.das` files, prefer MCP tools over built-in Grep/Glob** — `grep_usage` is parse-aware (tree-sitter), `find_references` resolves cross-module symbols, `find_symbol` searches all loaded modules. **For interacting with `daslang-live`, always use MCP live tools** — not curl.

| Tool | Use instead of... |
|---|---|
| `compile_check` | Running `daslang.exe` manually and parsing errors |
| `list_functions` | Grepping for `def ` in `.das` files |
| `list_types` | Grepping for `struct`/`class`/`enum` definitions |
| `find_symbol` | Searching across modules for function/type names |
| `list_module_api` | Reading daslib source to find available functions |
| `list_modules` | Guessing module names or scanning `daslib/` directory |
| `ast_dump` | Manually inspecting AST or post-macro output (supports `lineinfo` for source locations) |
| `program_log` | Running with `options log` to see full post-compilation program text |
| `run_script` | Running scripts via shell and capturing output |
| `run_test` | Running dastest via shell and parsing results |
| `format_file` | Running the formatter script manually |
| `convert_to_gen2` | Running `das-fmt` manually to convert gen1→gen2 syntax |
| `goto_definition` | Manually tracing symbol definitions across files |
| `type_of` | Manually inspecting expression types |
| `list_requires` | Grepping for `require` statements and guessing transitive deps |
| `find_references` | Manually searching for all usages of a symbol across files |
| `eval_expression` | Evaluating expressions by writing throwaway scripts |
| `describe_type` | Reading source to understand type fields, methods, and values |
| `grep_usage` | Using built-in Grep tool to search for symbol names in `.das` files (parse-aware via ast-grep + tree-sitter — no false positives from comments/strings) |
| `outline` | Manually scanning files for function/struct/enum declarations |
| `aot` | Manually running AOT generation and extracting function C++ |
| `lint` | Running lint/perf_lint/style_lint manually or requiring the modules for code quality, performance, and style checks |
| `live_launch` | Manually starting `daslang-live.exe` from shell |
| `live_status` | `curl http://localhost:9090/status` |
| `live_error` | `curl http://localhost:9090/error` |
| `live_reload` | `curl -X POST http://localhost:9090/reload` |
| `live_pause` | `curl -X POST http://localhost:9090/pause` or `/unpause` |
| `live_command` | `curl -X POST http://localhost:9090/command -d '{"name":"..."}` |
| `live_shutdown` | `curl -X POST http://localhost:9090/shutdown` |
| `shutdown` | Manually restarting the MCP server process |

Cursor-based tools (`goto_definition`, `type_of`, `find_references`) support a `no_opt` parameter that disables compiler optimizations to preserve the full AST — useful when globals, enum values, or bitfield constants get constant-folded away.

**Live tools:** The `live_*` tools interact with a running `daslang-live.exe` instance via its REST API. `live_launch` starts one if not already running (sets working directory to the script's folder). All live tools accept an optional `port` parameter (default 9090). When a compilation error is active, `live_command`, `live_pause` return HTTP 503 with the error. Use `live_reload` to fix. Hitting any unknown endpoint on the live API returns JSON help with all endpoints and curl examples.

**`shutdown` tool:** Shuts down the MCP server process. Claude Code auto-restarts it, picking up code changes to `.das` tool files. Tool registration changes (adding/removing tools) still require a manual MCP restart.

**Configuration:** Configure `.mcp.json` with `"command": "bin/Release/daslang.exe", "args": ["utils/mcp/main.das"]`. See `utils/mcp/README.md` for details and Claude Code permissions.

**Tests:** `bin/Release/daslang.exe dastest/dastest.das -- --test utils/mcp/test_tools.das`

## Keywords Reference

`aka` — variable name alias (`var a aka alpha = 42`)
`inscope` — declares variable owns a smart_ptr lifetime; only relevant for the residual smart_ptr types (`ProgramPtr`, `ContextPtr`, `FileAccessPtr`, `DebugAgentPtr`, `VisitorAdapterPtr`). AST pointers (gc_node) do NOT use `inscope`
`is type<T>` — compile-time type check
`expect` — suppress specific compilation errors in test files
`template` — generic type constraint in function signatures
