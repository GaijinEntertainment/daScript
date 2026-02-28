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

Multiple skill files may apply to a single task. For example, embedding daslang and calling its standard library requires reading both `skills/cpp_integration.md` and `skills/daslib_modules.md`.

## daslang Language — Gen2 Syntax (REQUIRED)

All code MUST use gen2 syntax (add `options gen2` at the top of every file). Key rules:

- **Parentheses** on control flow: `if (x > 0)`, `for (i in range(10))`, `while (running)`
- **Braces** on all blocks: `def foo() { ... }`, `if (x) { ... }`
- **Construction:** `new Type(field=val)` — NOT `new [[Type() field=val]]`
- **Enum access:** `EnumName.EnumValue` with dot — NOT `EnumName EnumValue`
- **Array literals:** `[1, 2, 3]` — NOT `[[int 1; 2; 3]]`. Creates `array<int>`; use `fixed_array(1, 2, 3)` for fixed-size
- **Struct init:** `Foo(a=1, b=2)` — NOT `[[Foo() a=1, b=2]]`
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

### Memory and move semantics

- daslang has garbage collection — `delete` is not required in most code
- `var inscope` declares automatic cleanup; struct fields need defaults or `@safe_when_uninitialized`
- `<-` is memcpy+memset(0), NOT smart_ptr-aware — see `skills/das_macros.md` for smart_ptr patterns

### Error handling

- `try/recover` — NOT `try/catch` (`recover` is the keyword)
- `panic("message")`, `assert(condition)`, `verify(condition)` (stays in release)

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

## SDK Directory Layout

- `bin/` — Compiler binary (`daslang`)
- `lib/` — Static libraries for C++ embedding
- `include/daScript/` — C++ headers for embedding
- `daslib/` — Standard library modules (.das files)
- `examples/` — Example scripts
- `dastest/` — Test framework (usable for testing your own code)

## Keywords Reference

`aka` — variable name alias (`var a aka alpha = 42`)
`inscope` — declares variable owns smart_ptr lifetime
`is type<T>` — compile-time type check
`expect` — suppress specific compilation errors
`template` — generic type constraint in function signatures
