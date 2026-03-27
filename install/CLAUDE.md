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
- **AOT generation:** `bin/daslang -aot input.das output.cpp`
- **Example:** `bin/daslang examples/hello_world.das`

### Debugging

- **Always check the exit code** after running `daslang` — a crash may produce no output at all, looking like a silent success
- On Windows, check `$LASTEXITCODE` in PowerShell after every run. Exit code `0` = success, non-zero = error
- Exit code `-1073741819` (`0xC0000005`) = **Access Violation** — indicates a native crash (segfault)
- If the program crashes with no error message, the bug is in native code (C++ bindings or smart pointer misuse) — check exit code first

## Skill Files (REQUIRED)

Task-specific instructions are in skill files under `skills/`. Read the relevant skill file(s) before performing the corresponding task.

| Skill file | Read BEFORE... |
|---|---|
| `skills/das_formatting.md` | Creating or modifying any `.das` file |
| `skills/cpp_integration.md` | Embedding daslang in C++ host applications, binding types/functions/enums |
| `skills/daslib_modules.md` | Using `daslib/` modules (linq, json, regex, functional, match, etc.), channels |
| `skills/das_macros.md` | Writing compile-time macros, AST manipulation, qmacro/quote code generation, smart_ptr ownership |
| `skills/daspkg.md` | Creating `.das_package` manifests, package structure, daspkg commands |
| `skills/dynamic_modules.md` | Understanding `.das_module` descriptors, module resolution, `register_native_path`, `register_dynamic_module` |
| `skills/daslang_live.md` | Working with `daslang-live`, live-reload lifecycle, REST API, `[live_command]`, persistent state |

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
- **Lambda:** `@(args) { body }` or `@@(args) { body }` (no-capture)
- **Generator:** `$() { yield value; }` or `$ { yield value; }`
- **Tuple `=>`:** `a => b` creates `tuple<auto;auto>`
- **`typeinfo`:** `typeinfo trait_name(type<T>)` — trait name outside parens
- **`static_if`:** `static_if (condition) { ... }` — parentheses required
- **Type function call:** `take(type<int>, 1, 2)` — NOT `take < int > (1, 2)`

### Type modifiers

- **`==const`** on a parameter type — accepts both const and non-const arguments: `def foo(self : MyStruct ==const)` — callers can pass `MyStruct` or `MyStruct const`
- **`-const`** strips constness in type expressions — used with `reinterpret` for interior mutability: `unsafe(reinterpret<MyStruct? -const>(addr(self)))`
- **Function pointer with explicit type:** `@@<(var self : T) : RetT> funcName` — specifies the exact parameter/return types of a function pointer literal

### Important defaults

- `strict_smart_pointers` is ON — smart_ptr variables require `var inscope`
- No implicit type promotion: `int + float` is a compile error — both sides must match
- No `bool(int)` cast — use `x != 0`; no `string(bool)` — use `"{flag}"`
- `int("123")` does NOT work — use `to_int` from `require strings`
- Hex literals are `uint` by default — use `int(0x3F)` for int
- **`default<T>`** — the default (zero) value of type `T`: `default<int>` is `0`, `default<string>` is `""`, `default<float>` is `0.0f`
- **`typedecl(expr)`** — compile-time type-of expression, usable inside `default<>`: `default<typedecl(field)>` gives the zero value of `field`'s type. Useful in generic code with `static_if` to compare against defaults.
- **Bitfield sizes**: `bitfield Name : uint8 { ... }`, `: uint16`, `: uint64`; default is `uint` (32-bit). Always unsigned.
- **Bitfield from expression**: `bitfield64(1ul << 13ul)` — use the constructor to create a bitfield value from an integer expression. Similarly `bitfield8()`, `bitfield16()`.

### Pass-by-value vs pass-by-reference

- Most types (structs, arrays, tables) always pass by reference — `&` is unnecessary on them
- Only **workhorse types** (`int`, `float`, `bool`, `string`, etc. — `isWorkhorseType` on the C++ side) pass by value
- **`smart_ptr<T>` also passes by value (move)** — like workhorse types, needs explicit `&` for pass-by-reference
  - `def foo(var p : ExpressionPtr)` — **moves** the caller's smart_ptr, zeroing it
  - `def foo(var p : ExpressionPtr&)` — pass by **reference**, caller keeps ownership
  - Without `&`, passing a `var inscope` smart_ptr zeroes it on entry, then `inscope` cleanup double-frees
- **`var s : string`** — writable local copy, changes do NOT propagate back to the caller
- **`var s : string&`** — pass by reference, changes propagate back. Use `&` for string out-parameters
- **`clone_string(s)`** — clones a string into the current context's heap. Required for cross-context calls where the source context may be destroyed
- **`:=`** on strings performs a clone (allocates in current context). Plain `=` copies the pointer

### Memory and move semantics

- daslang has garbage collection — `delete` is not required in most code
- `var inscope` declares automatic cleanup; struct fields need defaults or `@safe_when_uninitialized`
- `<-` is memcpy+memset(0), NOT smart_ptr-aware — see `skills/das_macros.md` for smart_ptr patterns

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

### Table operations

- `table[key]` **inserts** a default entry if missing — use `table?[key] ?? default` for safe lookup
- `key_exists(table, key)` — check without inserting
- `table |> insert(key, value)` / `table |> erase(key)`
- **Never use two `[]` lookups on the same table in one expression** — re-hashing can invalidate references
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

### Common gotchas

- Lambda params can shadow function params — use distinct names
- String builder requires `unsafe` or `options persistent_heap` if returned
- Tuple field access: `t._0`, `t._1`, `t._2`
- Annotations: `[export]`, `[private]`, `[test]`; `options no_aot`, `options rtti`
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

### Code style — prefer idiomatic forms

| Don't write | Write instead | Why |
|---|---|---|
| `string(x.__rtti) == "ExprFoo"` | `x is ExprFoo` | `is` works on both smart_ptr and raw ptrs |
| `get_ptr(x) == null` | `x == null` | smart_ptr supports `==`/`!=` null directly |
| `get_ptr(x).field` | `x.field` | smart_ptr auto-dereferences for field access |
| `string(das_str) == "lit"` | `das_str == "lit"` | `das_string` compares directly with `string` |
| `!empty(string(das_str))` | `!empty(das_str)` | `empty()` works on `das_string` |
| `let v = string(x.name); $i(v)` | `$i(x.name)` | qmacro `$i`/`$f` accept `das_string` directly |
| `var copy = val; $v(copy)` | `$v(val)` | qmacro `$v` works with `let` vars and loop vars |
| `if (true) { ... }` | `{ ... }` | bare blocks create lexical scope in gen2 |
| `var inscope r <- expr; return <- r` | `return <- expr` | direct return avoids intermediate |
| `unsafe { (reinterpret<ExprBlock?> blk).list }` | `blk.list` | smart_ptr auto-dereferences |
| `unsafe(reinterpret<T?> get_ptr(x))` | make param `var` + `get_ptr(x)` | `var` param gives non-const access, no reinterpret needed |

**Minimize `unsafe`:** Most `unsafe(reinterpret<T?>)` in macro code exists to strip `const` from smart_ptr field access. Fix the root cause: make the function parameter `var` so field access returns non-const pointers. Reserve `unsafe` for genuinely unsafe operations (pointer arithmetic, `reinterpret` across unrelated types).

## SDK Directory Layout

- `bin/` — Compiler binaries (`daslang`, `daslang-live`, `das-fmt`, `clang-cl`, `sqlite_shell`)
- `lib/` — Static and shared libraries for C++ embedding
- `include/daScript/` — C++ headers for embedding
- `daslib/` — Standard library modules (.das files)
- `modules/` — Optional plugin modules (dasHV, dasGlfw, dasImgui, dasPUGIXML, etc.)
- `examples/` — Example scripts
- `tutorials/` — Language, integration, and module tutorials
- `dastest/` — Test framework (usable for testing your own code)
- `utils/mcp/` — MCP server for AI coding assistants (29 tools, stdio transport, no extra deps)
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

`utils/mcp/` contains a [Model Context Protocol](https://modelcontextprotocol.io/) server that exposes compiler diagnostics and program introspection to AI coding assistants. Uses stdio transport — no extra build dependencies.

**When MCP tools are available**, prefer them over manual compilation and grep-based exploration:

| Tool | Use instead of... |
|---|---|
| `compile_check` | Running `daslang` manually and parsing errors |
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
| `grep_usage` | Grepping for symbol names across files (parse-aware via ast-grep + tree-sitter) |
| `outline` | Manually scanning files for function/struct/enum declarations |
| `aot` | Manually running AOT generation and extracting function C++ |
| `live_launch` | Manually starting `daslang-live` from shell |
| `live_status` | `curl http://localhost:9090/status` |
| `live_error` | `curl http://localhost:9090/error` |
| `live_reload` | `curl -X POST http://localhost:9090/reload` |
| `live_pause` | `curl -X POST http://localhost:9090/pause` or `/unpause` |
| `live_command` | `curl -X POST http://localhost:9090/command -d '{"name":"..."}` |
| `live_shutdown` | `curl -X POST http://localhost:9090/shutdown` |
| `shutdown` | Manually restarting the MCP server process |

Cursor-based tools (`goto_definition`, `type_of`, `find_references`) support a `no_opt` parameter that disables compiler optimizations to preserve the full AST — useful when globals, enum values, or bitfield constants get constant-folded away.

**Live tools:** The `live_*` tools interact with a running `daslang-live` instance via its REST API. `live_launch` starts one if not already running. All live tools accept optional `port` (default 9090). When a compilation error is active, `live_command` and `live_pause` return 503 with the error. Hitting any unknown endpoint returns JSON help.

**Configuration:** Configure `.mcp.json` with `"command": "bin/daslang", "args": ["utils/mcp/main.das"]`. See `utils/mcp/README.md` for details and permissions.

## Keywords Reference

`aka` — variable name alias (`var a aka alpha = 42`)
`inscope` — declares variable owns smart_ptr lifetime
`is type<T>` — compile-time type check
`expect` — suppress specific compilation errors
`template` — generic type constraint in function signatures
