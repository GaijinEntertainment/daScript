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
- **JIT execution:** `bin/daslang -jit path/to/script.das` — compile to native via LLVM JIT before running. Available only in builds compiled with the JIT module. `bin/daslang -exe path/to/script.das -output out.exe` JITs to a standalone executable (implies `-dry-run`).
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

- **Always check the exit code** after running `daslang` — a crash may produce no output at all, looking like a silent success
- On Windows, check `$LASTEXITCODE` in PowerShell after every run. Exit code `0` = success, non-zero = error
- Exit code `-1073741819` (`0xC0000005`) = **Access Violation** — indicates a native crash (segfault)
- If the program crashes with no error message, the bug is in native code (C++ bindings or smart pointer misuse) — check exit code first
- **Don't truncate output** with `head`/`tail` — daslang stack traces are easily clipped. Capture full output, then `grep` if needed
- **`options log`** — append at the end of a `.das` file to dump the final post-compilation program text. Useful for confirming what the compiler actually produces (constant folding, generic reification, macro expansion).
- **`options log_infer_passes`** — append at the end of a failing `.das` file for a per-pass infer-pipeline dump (which generics got reified, when finalize ran, where lookups missed). Smaller and more targeted than `options log` for template/generic reification bugs.

## Skill Files (REQUIRED)

Task-specific instructions are in skill files under `skills/`. Read the relevant skill file(s) before performing the corresponding task.

| Skill file | Read BEFORE... |
|---|---|
| `skills/das_formatting.md` | Creating or modifying any `.das` file |
| `skills/cpp_integration.md` | Embedding daslang in C++ host applications, binding types/functions/enums |
| `skills/daslib_modules.md` | Using `daslib/` modules (linq, json, regex, functional, match, etc.), channels |
| `skills/das_macros.md` | Writing compile-time macros, AST manipulation, qmacro/quote code generation, gc_node AST-pointer patterns |
| `skills/daspkg.md` | Creating `.das_package` manifests, package structure, daspkg commands |
| `skills/clargs_usage.md` | Writing your own daslang command-line tools — declarative argv parsing via `daslib/clargs` |
| `skills/dynamic_modules.md` | Understanding `.das_module` descriptors, module resolution, `register_native_path`, `register_dynamic_module` |
| `skills/daslang_live.md` | Working with `daslang-live`, live-reload lifecycle, REST API, `[live_command]`, persistent state |
| `skills/json.md` | Reading or writing JSON in `.das` code — choosing between `sprint_json`/`sscan_json`, `JV`/`from_JV` from `daslib/json_boost`, custom converters, or manual `JsonValue?` |
| `skills/xml.md` | Reading, building, querying, or serializing XML via `dasPUGIXML` (`PUGIXML_boost`) — RAII parsing, iteration, `tag`/`attr` builder, XPath, struct↔XML round-trip |
| `skills/filesystem.md` | Any `.das` code that builds, splits, or normalizes a file path, or touches the filesystem (existence, listing, copy/rename/remove, temp files). **Rule:** path & filename operations MUST use `fio` helpers (`base_name`/`dir_name`/`extension`/`stem`/`path_join`/`normalize`/`is_absolute`/`relative`) — never hand-rolled `rfind`/`slice`/string-interp. Filesystem ops use `fio` / `daslib/fio` (`stat`, `dir_rec`, RAII `fopen`, `_result` variants) |
| `skills/writing_tests.md` | Writing tests for your daslang code with the bundled `dastest` framework (`bin/daslang dastest/dastest.das -- --test ...`) |
| `skills/memory_leak_detection.md` | Diagnosing leaks — `--das-profiler-leaks`, `-track-allocations -heap-report`, `GC APP LEAK` reports, `--track-smart-ptr`, `HandleRegistry` dump |
| `skills/jobque_debugging.md` | Debugging Channel/LockBox/JobStatus/Stream/Feature leaks using `DumpJobQueLeaks` and `--track-job-status <id>` refcount tracing |
| `skills/detect_dupe.md` | Detecting duplicate / near-duplicate functions across a daslang codebase — building a corpus, asking "did I just write something that already exists?", or wiring a CI gate. Covers both the MCP tools (`export_corpus`, `detect_duplicates`) and the underlying CLI (`utils/detect-dupe/main.das`) |
| `skills/linq.md` | Any `.das` code that filters, maps, sorts, groups, aggregates, or otherwise transforms a sequence into another sequence, array, or table. Preference order: comprehension `[for (x in src); expr; where cond]` when one expression covers it → LINQ (`daslib/linq_boost` shorthand `_select` / `_where` / `_to_array`, or pipe-form `arr \|> where_(...) \|> ...`) for chains, lazy iterators, set ops, joins, aggregations → plain `for` loop for side-effecting iteration. Avoid `daslib/functional` (`map` / `filter` / `each` / `to_array`) for new code |

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

- `strict_smart_pointers` is ON — but the only types that are still `smart_ptr` are `Program` (`ProgramPtr`), `Context` (`ContextPtr`), `FileAccess` (`FileAccessPtr`), and a few internals (`DebugAgentPtr`, `VisitorAdapterPtr` from `make_visitor`). Only those need `var inscope`. **All AST types** (TypeDecl, Expression, Function, Structure, Enumeration, Variable, MakeFieldDecl, MakeStruct, every `Annotation` subclass) are now plain raw pointers (gc_node) — see "AST nodes (gc_node)" below
- No implicit type promotion: `int + float` is a compile error — both sides must match
- No `bool(int)` cast — use `x != 0`; no `string(bool)` — use `"{flag}"`
- `int("123")` does NOT work — use `to_int` from `require strings`
- Hex literals are `uint` by default — use `int(0x3F)` for int
- **`default<T>`** — the default (zero) value of type `T`: `default<int>` is `0`, `default<string>` is `""`, `default<float>` is `0.0f`. The body of the called function CAN use the value freely.
- **`type<T>`** vs **`default<T>`** as a witness argument — they are **not** interchangeable:
  - `type<T>` is a no-stack, no-construction compile-time type tag. The function body must NOT use the parameter (compile error if it does). Annotate with `[unused_argument(t)]`.
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
- **The few remaining `smart_ptr<T>` types** (`ProgramPtr`, `ContextPtr`, `FileAccessPtr`, `DebugAgentPtr`, `VisitorAdapterPtr`) **still use refcount semantics**. Variables holding them require `var inscope` for cleanup. This is the narrow remaining surface where the smart_ptr rules in older docs still apply
- **`var s : string`** — writable local copy, changes do NOT propagate back to the caller
- **`var s : string&`** — pass by reference, changes propagate back. Use `&` for string out-parameters
- **`clone_string(s)`** — clones a string into the current context's heap. Required for cross-context calls where the source context may be destroyed
- **`:=`** on strings performs a clone (allocates in current context). Plain `=` copies the pointer

### AST nodes (gc_node) — unique ownership, clone to duplicate

Every AST node lives at exactly one location. Multiple `ExpressionPtr` or `TypeDeclPtr` values pointing at the same node are **shared references to one object**, not independent copies. The garbage collector tracks the node by its address; sticking the same pointer in two places does not create a second node.

- **Don't insert the same pointer into two parent expressions** — that creates aliasing. Both parents think they own the child, gc_collect walks it twice, mutations on one parent show up in the other.
- **Use the matching `clone_*` to duplicate:**
  - `clone_type(t)` for `TypeDeclPtr`
  - `clone_expression(e)` for `ExpressionPtr` (recursive deep clone)
  - `clone_function(f)` for `FunctionPtr` — note: still returns via move (`var x <- clone_function(f)`)
  - `clone_variable(v)` for `VariablePtr`
  - `clone_structure(s)` for `StructurePtr`
- **Don't use `var inscope`** for AST pointer types — the gc_node owns its own lifetime via `gc_root`. `var inscope` is for the residual smart_ptr types only (`ProgramPtr`, `ContextPtr`, `FileAccessPtr`).
- **Don't use `<-`** when assigning AST pointers — plain `=` is correct (`fn.body = newBlock`, `td.firstType = elemType`). Keep `<-` only where the API still demands it (`clone_function`, `qmacro_function` returns).
- **Tools/utilities that build AST at runtime** (outside the normal compile pipeline) must wrap the scope in `ast_gc_guard() { ... }` from `daslib/ast`, or the leak detector reports `GC APP LEAK` at exit.
- **Don't use `clone_to_move`** on AST pointers as a substitute for `clone_*` — `clone_to_move` is the generic copy-then-move helper for non-copyable values like `array<T>`. For AST nodes the right call is the type-specific `clone_type` / `clone_expression` / etc.

If you find yourself reading older guidance about `var inscope`, `<-`, or `clone_to_move` for AST types, the source is pre-migration — see `skills/das_macros.md` for the current rules.

### Memory and move semantics

- daslang has garbage collection — `delete` is not required in most code
- **No C++/Rust-style scope RAII for plain `var`** — a local `var arr : array<T>` declared in any scope (function body, if-block, loop body) does NOT finalize on scope exit; the heap allocation stays alive until the context tears down. To get cleanup you must either (a) declare with `var inscope` (smart_ptrs only — Program/Context/FileAccess), (b) call `delete arr` explicitly before scope exit, or (c) move ownership out via `<-`. Per-frame leaks in hot paths usually trace back to a local `var arr : array<...>` that was never deleted
- `var inscope` declares automatic cleanup; struct fields need defaults or `@safe_when_uninitialized`
- `var inscope` is legal inside `for` / `while` loop bodies — the loop's `finally` runs per iteration (on fall-through, `continue`, `break`, `return`), so each iteration finalizes its own scoped variables
- `<-` is memcpy+memset(0), NOT smart_ptr-aware. For the residual smart_ptrs (`ProgramPtr`, `ContextPtr`, `FileAccessPtr`) it bypasses refcount handling. For AST raw pointers it just shuffles a pointer slot, harmless but stylistically wrong (use `=`)

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
| `string(x.__rtti) == "ExprFoo"` | `x is ExprFoo` | `is` works directly on AST pointers |
| `get_ptr(x) == null` | `x == null` | AST pointers compare to `null` directly (also still works for residual smart_ptrs) |
| `get_ptr(x).field` | `x.field` | AST pointers auto-dereference for field access; `get_ptr` is leftover from the smart_ptr era |
| `string(das_str) == "lit"` | `das_str == "lit"` | `das_string` compares directly with `string` |
| `!empty(string(das_str))` | `!empty(das_str)` | `empty()` works on `das_string` |
| `let v = string(x.name); $i(v)` | `$i(x.name)` | qmacro `$i`/`$f` accept `das_string` directly |
| `var copy = val; $v(copy)` | `$v(val)` | qmacro `$v` works with `let` vars and loop vars |
| `if (true) { ... }` | `{ ... }` | bare blocks create lexical scope in gen2 |
| `var inscope r <- expr; return <- r` | `return <- expr` | direct return avoids intermediate |
| `unsafe { (reinterpret<ExprBlock?> blk).list }` | `blk.list` | AST pointers auto-dereference |
| `unsafe(reinterpret<T?> x)` | make param `var` + plain `x` | `var` param gives non-const access, no reinterpret needed |
| `let i = max(rfind(p, "/"), rfind(p, "\\")); slice(p, i+1)` | `base_name(p)` | `fio.base_name` is cross-platform; manual `rfind` misses Windows separators or trailing slashes |
| `slice(p, 0, max(rfind(p, "/"), rfind(p, "\\")))` | `dir_name(p)` | same — use `fio.dir_name`/`parent`, never hand-roll directory splitting |
| `"{a}/{b}"` for file paths | `path_join(a, b)` | handles separator collisions and platform separators |

**Minimize `unsafe`:** Most `unsafe(reinterpret<T?>)` in macro code exists to strip `const` from raw-pointer field access. Fix the root cause: make the function parameter `var` so field access returns non-const pointers. Reserve `unsafe` for genuinely unsafe operations (pointer arithmetic, `reinterpret` across unrelated types).

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
| `lint` | Running lint/perf_lint/style_lint manually or requiring the modules for code quality, performance, and style checks |
| `export_corpus` | Running `utils/detect-dupe/main.das --export-functions` from a shell to build a duplicate-detection corpus |
| `detect_duplicates` | Running `utils/detect-dupe/main.das --against` from a shell to ask "did I just write something that already exists?". Wraps B2 mode end-to-end |
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
`inscope` — declares variable owns a smart_ptr lifetime; only relevant for the residual smart_ptr types (`ProgramPtr`, `ContextPtr`, `FileAccessPtr`, `DebugAgentPtr`, `VisitorAdapterPtr`). AST pointers (gc_node) do NOT use `inscope`
`is type<T>` — compile-time type check
`expect` — suppress specific compilation errors
`template` — generic type constraint in function signatures
