# daslang Project Instructions

> **Keep in sync:** This file and `CLAUDE.md` (repo root) share identical content. Both reference skill files in the `skills/` directory at repo root — skill files are shared, not duplicated.

## Project Overview

This is the [daslang](https://dascript.org/) programming language repository (GaijinEntertainment/daScript). daslang (formerly daScript) is a high-performance statically-typed scripting language designed for games and real-time applications. The language has been renamed to **daslang**, but the repository and many C++ API names still use the old "daScript" spelling.

## Build & Run

- **Build system:** CMake + MSVC (Visual Studio 2022)
- **Generate:** `generate_msvc_2022.bat` → creates `build/DAS.sln`
- **Build:** `cmake --build build --config Release`
- **Compiler binary:** `bin/Release/daslang.exe`
- **Run a script:** `bin/Release/daslang.exe path/to/script.das`
- **Run tests:** `bin/Release/daslang.exe dastest/dastest.das -- --test path/to/test.das`

### Debugging

- **Always check the exit code** after running `daslang.exe` — a crash may produce no output at all, looking like a silent success
- On Windows, check `$LASTEXITCODE` in PowerShell after every run. Exit code `0` = success, non-zero = error
- Exit code `-1073741819` (`0xC0000005`) = **Access Violation** — indicates a native crash (segfault), usually a dangling pointer or double-free in macro AST manipulation
- If the program crashes with no error message, the bug is in native code (C++ bindings or smart pointer misuse), not in daScript logic — check exit code first before investigating script errors

## Skill Files (REQUIRED)

Task-specific instructions are split into skill files under `skills/`. You MUST read the relevant skill file(s) before performing the corresponding task.

| Skill file | Read BEFORE... |
|---|---|
| `skills/das_formatting.md` | Creating or modifying any `.das` file (tutorials, tests, daslib modules, utilities) |
| `skills/writing_tests.md` | Writing or editing test files under `tests/` |
| `skills/documentation_rst.md` | Editing RST files in `doc/source/`, editing `//!` doc-comments in `daslib/*.das`, writing tutorial RST pages, editing `doc/source/stdlib/handmade/` files, running `das2rst.das`, or regenerating stdlib module documentation |
| `skills/cpp_integration.md` | Writing or editing C++ files in `src/`, `modules/`, or `tutorials/integration/cpp/` |
| `skills/daslib_modules.md` | Working with `daslib/` modules (linq, json, regex, functional, match, etc.) or extending the standard library |

Multiple skill files may apply to a single task. For example, creating a new daslib module requires reading `skills/das_formatting.md`, `skills/daslib_modules.md`, and possibly `skills/documentation_rst.md`.

## daslang Language — Gen2 Syntax (REQUIRED)

All code examples and documentation MUST use gen2 syntax (add `options gen2` at the top of every file). Key rules:

- **Parentheses** on control flow: `if (x > 0)`, `for (i in range(10))`, `while (running)`
- **Braces** on all blocks — no indentation-based blocks: `def foo() { ... }`, `if (x) { ... }`
- **Construction:** `new Type(field=val)` — NOT `new [[Type() field=val]]`
- **Enum access:** `EnumName.EnumValue` with dot — NOT `EnumName EnumValue` without
- **Array literals:** `[1, 2, 3]` (commas, square brackets) — NOT `[[int 1; 2; 3]]`. Note: this creates a **dynamic** `array<int>` — use `fixed_array(1, 2, 3)` for fixed-size arrays
- **Struct init:** `Foo(a=1, b=2)` — NOT `[[Foo() a=1, b=2]]`
- **No `[[ ]]` for `new`** — `new` always uses parentheses: `new Foo(x=1)`
- **Table literals:** `{ "k" => v, "k2" => v2 }` (single braces, commas) — NOT `{{ "k" => v; "k2" => v2 }}`
- **Named arguments:** `foo([name = value])` with square brackets — NOT `foo(name=value)`
- **Block arguments:** a block or lambda after `func()` is automatically piped as the last argument — no `<|` needed. Parameterless blocks need no `$`: `defer() { ... }`. Blocks with parameters use `$`: `build_string() $(var writer) { ... }`. Lambdas use `@`: `emplace() @(x : int) { ... }`
- **Lambda:** `@(args) { body }` or `@@(args) { body }` (no-capture)
- **Generator:** `$() { yield value; }` or `$ { yield value; }` — both are valid gen2 syntax
- **Tuple `=>` operator:** `a => b` creates a `tuple<auto;auto>` — useful in LINQ, table construction, and ad-hoc pairs
- **Bitfield variables** need explicit type for `.field` access and printing: `var f : MyBitfield`
- **Bitfield dot access:** read with `f.flag` (returns bool), write with `f.flag = true/false`
- **`typeinfo`** gen2 syntax: trait name goes **outside** parentheses — `typeinfo trait_name(type<T>)`, NOT `typeinfo(trait_name type<T>)`. With subtrait: `typeinfo has_method<name>(type<T>)`. With two traits: `typeinfo trait<sub;extra>(type<T>)`
- **`static_if`:** `static_if (condition) { ... }` — parentheses required in gen2

### Important defaults

- `strict_smart_pointers` is ON by default — smart_ptr variables require `var inscope` declaration
- `smart_ptr<T>` only works with C++ handled types (not user structs)
- No implicit type promotion: `int + float` is a compile error — both sides must match
- No `bool(int)` cast — use `x != 0`
- `string(x)` works for numeric types; `string(bool)` does NOT work — use string interpolation `"{flag}"`
- `int("123")` does NOT work — use `to_int` from `require strings`
- Hex literals like `0x3F800000` are `uint` by default — use `int(0x3F)` for int
- Float printing is compact: `3.14` not `3.140000`; uint prints as hex: `0xff` not `255`

### Memory management

- daslang has garbage collection — `delete` is not required in most code
- `delete` is available for explicit cleanup but requires `unsafe` for heap pointers
- `var inscope` declares automatic cleanup in a `finally` block
- Prefer omitting `delete` in tutorials and examples unless the topic is memory management
- Struct fields without initializers require defaults or `@safe_when_uninitialized`

### Move semantics (`<-` vs `move`)

- **`<-` operator**: ALWAYS `memcpy(dest, src) + memset(src, 0)` — it is a raw memory operation, NOT smart_ptr-aware. It zeros the source regardless of type.
- **`move` function**: Bound via C++ `builtin_smart_ptr_move*` family in `module_builtin_runtime.cpp` — proper smart pointer move with reference counting. Use `move(dest, src)` or `move(dest) src` for `smart_ptr<T>` transfers.
- **`return <- expr`**: Moves value to return slot and zeroes `expr`. If `expr` is a `&` ref parameter, this zeroes the *caller's* variable since they share memory.
- **Key subtlety**: When a function takes `var x : smart_ptr<T>&` (by reference) and internally calls a function that takes `var x : smart_ptr<T>` (by value/move), the `<-` inside `return` zeroes the `&` ref. Callers MUST capture the return value: `unsafe { expr <- apply_template(expr) $() { ... } }`
- **Safe pattern**: `unsafe { variable <- function_returning_smart_ptr(variable) $() { ... } }` — captures return value back into the same variable

### AST smart pointer ownership (macros)

- Many AST functions take `smart_ptr<T>` **by value** — this **moves** the pointer from the caller. After the call, the caller's variable is null/zeroed.
- **`var inscope` + move = double-free crash**: If you declare `var inscope p = make_something()` and pass `p` to a function that moves it, `p` is zeroed by the move. Then `inscope` destroys `p` at scope exit → double-free → Access Violation.
- **`clone_type(typeExpr)`** — deep-copies a `TypeDeclPtr`. Use when passing a type to a consuming function while keeping the original, or when the source is a `var inscope` variable.
- **`clone_expression(expr)`** — deep-copies an `ExpressionPtr`. Same ownership rules as `clone_type`.
- **Safe pattern for `add_structure_field`**: pass temporaries directly — `st |> add_structure_field("name", clone_type(qmacro_type(type<int>)), qmacro($v(val)))` — the `clone_type` result is a temporary safely consumed by the move.
- **`move_new`**: `field |> move_new <| expr` — idiomatic way to assign a newly created `smart_ptr<T>` into a field. Equivalent to `field <- expr` but does not require `unsafe`. Preferred over `field := null; unsafe { field <- expr }` when setting AST node fields (e.g. `cm.init |> move_new <| qmacro(...)`).
- **`add_ptr_ref(raw_ptr)`** — wraps a raw pointer (`T?`) into a `smart_ptr<T>` by adding a reference. AST node fields are often raw pointers (e.g. `typeDecl.structType` is `Structure?`, `typeDecl.enumType` is `Enumeration?`), but many API functions expect `smart_ptr<T>`. Use `add_ptr_ref` to bridge: `var inscope st <- add_ptr_ref(pair_type.structType)` gives a `StructurePtr` from a `Structure?` field. Also accepts `smart_ptr<T>` input (adds a ref and returns a new smart_ptr). Always use `var inscope` for the result to manage lifetime.
- **Rule of thumb**: if a function signature takes `var x : smart_ptr<T>` (not `&`), it **consumes** `x`. Pass a temporary, a clone, or accept that your variable will be null after the call.

### `qmacro` vs `quote` (code generation)

- **`qmacro(expr)`** — quasi-quote with reification splices (`$v()`, `$e()`, `$c()`, `$t()`, `$i()`, `$f()`, `$a()` etc.). Use when the generated code contains interpolated values.
- **`quote(expr)`** — plain quote with NO reification. Use when the expression is a simple literal or constant with no splices — e.g. `quote(true)`, `quote(false)`, `quote(0)`.
- **Rule**: if the expression contains no `$…()` reification operators, prefer `quote()` over `qmacro()` — it is simpler, clearer, and avoids unnecessary reification overhead.

### Error handling

- `try/recover` — NOT `try/catch` (`recover` is the keyword)
- `panic("message")` — abort with message
- `assert(condition)` / `assert(condition, "message")` — debug assertion
- `verify(condition)` — assertion that remains in release builds

### Class method modifiers

- **`def const`** — const method: `self` is `const`, cannot modify fields. Use for read-only methods.
- **`def abstract const`** / **`def override const`** — abstract/override const methods in class hierarchies
- **`def static`** — static method: no implicit `self`; called as `ClassName.method()` or `ClassName\`method()`
- **`def static const`** — static method with const `self` parameter (used in template structures like `flat_hash_table.das`). The first explicit argument becomes const `self`.
- **`[class_method]`** annotation (from `daslib/class_boost`) — transforms a `def static` into a proper class method by injecting `self` and wrapping body in `with (self) { ... }`. Const-ness follows `def static const`.
- Method call syntax: `obj.method()` for value types, `obj->method()` for pointer types, `obj |> method()` pipe syntax

### Generic function dispatch (`_::`, `__::`, unqualified)

- Generic functions are instanced as private functions in the **calling** module
- **Unqualified call** `foo(x)`: resolves in the module where the generic is **defined** — caller's overloads are NOT visible
- **`_::foo(x)`**: resolves as if called implicitly in the **current** (calling) module — caller's overloads ARE visible
- **`__::foo(x)`**: resolves strictly in the **defining** module only — nothing imported
- This is why `:=` and `delete` emit `_::clone` / `_::finalize` — so user overloads are picked up in generics
- When writing a library generic that should dispatch to user-provided overloads, use `_::` prefix
- `mem_archive_save` does NOT use `_::serialize` — so user serialize overloads are invisible; use manual `Archive` + `MemSerializer` pattern instead

### Table operations

- `table[key]` **inserts** a new default entry if `key` is missing — use only when you want insert-on-access
- `table[key]` requires `unsafe` by default; add `options unsafe_table_lookup = false` to allow safe `[]` access
- `table?[key] ?? default_value` — safe lookup with fallback, does NOT insert missing keys
- `key_exists(table, key)` — check if a key is present without inserting
- `table |> insert(key, value)` — insert into `table<K;V>`; `table |> insert(key)` — insert into set `table<K>`
- `table |> erase(key)` — remove a key
- **Never use two `[]` lookups on the same table in one expression** (e.g. `tab[k1] = tab[k2]`) — tables are unboxed containers and re-hashing on insert can invalidate the first reference
- `get(table, key) $(pval) { ... }` — block-based lookup; block receives value reference if found

### Common gotchas

- Lambda params can shadow function params — use distinct names (e.g., `$(lhs, rhs)` not `$(a, b)` when `a` is already in scope)
- `struct Foo { ... }` requires braces in gen2 — empty struct is `struct Foo {}`
- String builder: `build_string() $(var writer) { write(writer, "text") }` — requires `unsafe` or `options persistent_heap` if returned
- `options persistent_heap` — needed when returning strings built with `build_string`
- Tuple field access uses underscore prefix: `t._0`, `t._1`, `t._2`
- Annotations: `[export]` for entry points, `[private]` for private functions, `[test]` for test functions
- `options no_aot` — disable ahead-of-time compilation (common in test files)
- `options rtti` — enable runtime type information (needed for some daslib features)
- `require` uses forward slash paths: `require daslib/linq` — NOT `require daslib\linq`
- `require foo public` — re-exports `foo` so that any module requiring the current module also sees `foo`'s symbols transitively. Example: `regex.das` has `require strings public`, so `require daslib/regex` automatically makes `slice`, `starts_with`, etc. visible without a separate `require strings`
- `<-` is memcpy+memset(0), NOT a smart_ptr-aware move — see "Move semantics" section above
- When calling `apply_template`, always capture the return value: `unsafe { expr <- apply_template(expr) $() { ... } }` — discarding the return loses the expression data
- Iterator comprehension: `[iterator for(x in src); expression]` — semicolon separates generator from body
- `to_array` (from `daslib/builtin`) converts any iterator to an array
- Lambdas CAN be stored in arrays: `var fns : array<lambda<(x:int):int>>` + `fns |> emplace() @(x : int) : int { return x * 2; }` — move semantics, not copy
- Blocks CANNOT be stored in containers, returned from functions, or captured — use lambdas or function pointers for those use cases
- `match`, `multi_match`, `static_match` macros (from `daslib/match.das`) handle side effects automatically — do NOT add `[sideeffects]` annotations to functions that only use match
- `[export] def main()` returns `void` — do NOT `return true` or return other values from main

### Channels and cross-context communication

- `with_channel(N) $(ch) { ... }` — creates a channel expecting `N` notify calls before `for_each_clone` unblocks
- **`notify` vs `notify_and_release`**: when a lambda captures a channel, the reference count is incremented; `notify_and_release` decrements the entry count AND releases that extra reference, setting the variable to `null`. When passing a channel as a plain argument (e.g. via `invoke_in_context`), no lambda capture occurs — no extra reference is added — so use plain `notify`
- `notify_and_release` sets the channel/status variable to `null` after release
- `push_clone(ch, value)` — push a clone of `value` into the channel
- `for_each_clone(ch) $(val : T#) { ... }` — drain channel; data arrives as temporary type `T#`
- `invoke_in_context(context, "func", ch)` — can pass `Channel?` directly to a child context
- Child scripts that use channel operations need `require daslib/jobque_boost`; compile with `compile_file` + `make_file_access("")` so the child can resolve daslib modules from disk

## Key Directories

- `src/` — C++ compiler/runtime source
- `include/daScript/` — C++ headers
- `daslib/` — Standard library modules (86 .das files)
- `dastest/` — Test framework
- `tests/` — Test suite (with per-module subfolders: `tests/decs/`, `tests/match/`, `tests/json/`, etc.)
- `doc/source/reference/language/` — RST language documentation (36 files)
- `doc/source/stdlib/` — RST standard library documentation (auto-generated + handmade)
- `doc/reflections/` — Documentation generation tools (das2rst.das, rst.das, gen_module_examples.py)
- `tutorials/language/` — Language tutorial `.das` files (42 progressive tutorials)
- `tutorials/integration/cpp/` — C++ integration tutorials (embedding daslang in C++ host applications)
- `tutorials/macros/` — Macro tutorials (call macros, reader macros, etc.)
- `doc/source/reference/tutorials/` — RST companion pages for each tutorial
- `tests/linq/` — LINQ module tests (15 test files, ~500 tests)
- `tests/functional/` — Functional module tests
- `tests/json/` — JSON module tests (4 test files, ~148 tests)
- `tests/regex/` — Regex module tests (8 test files, 278 tests)
- `tests/interfaces/` — Interface module tests (4 test files, 67 tests)
- `modules/` — External plugin modules

## Keywords Reference

`aka` — variable name alias (`var a aka alpha = 42`, `for (x aka element in arr)`)
`inscope` — declares variable owns smart_ptr lifetime (`var inscope p = ...`)
`is type<T>` — compile-time type check (`a is type<int>` returns bool)
`expect` — suppress specific compilation errors in test files (`expect 30507`)
`template` — generic type constraint in function signatures
