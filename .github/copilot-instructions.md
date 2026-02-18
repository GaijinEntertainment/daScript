# daslang Project Instructions

> **Keep in sync:** This file and `CLAUDE.md` (repo root) share identical content. When updating one, copy the changes to the other so both GitHub Copilot and Claude Code see the same project instructions.

## Project Overview

This is the [daslang](https://dascript.org/) programming language repository (GaijinEntertainment/daScript). daslang (formerly daScript) is a high-performance statically-typed scripting language designed for games and real-time applications. The language has been renamed to **daslang**, but the repository and many C++ API names still use the old "daScript" spelling.

## Build & Run

- **Build system:** CMake + MSVC (Visual Studio 2022)
- **Generate:** `generate_msvc_2022.bat` → creates `build/DAS.sln`
- **Build:** `cmake --build build --config Release`
- **Compiler binary:** `bin/Release/daslang.exe`
- **Run a script:** `bin/Release/daslang.exe path/to/script.das`
- **Run tests:** `bin/Release/daslang.exe dastest/dastest.das -- --test path/to/test.das`

## Code Formatting (REQUIRED)

After creating or modifying any `.das` file that is part of the project (daslib modules, tutorials, tests, etc.), run the source formatter on it. Do NOT format temporary/scratch files that will be deleted.

**Formatter tool:** `utils/dasCodeFormatter/main.das`

**Procedure:**

1. **Back up** the file before formatting: copy it to `<filename>.das.bak`
2. **Run the formatter:** `bin/Release/daslang.exe utils/dasCodeFormatter/main.das -- path/to/file.das`
3. **Verify** the formatted file still compiles: `bin/Release/daslang.exe path/to/file.das`
   - For test files (no `main`), compile-check with: `bin/Release/daslang.exe dastest/dastest.das -- --test path/to/test.das`
   - For module files (no `main`), verify by running a file that requires them
4. **Remove the backup** if formatting succeeded: delete `<filename>.das.bak`
5. **Restore from backup** if formatting broke the file: copy `.das.bak` back over the `.das` file, delete the backup, and report the issue

**When to format:**
- New `.das` files: tutorials, tests, daslib modules, utilities
- Modified `.das` files: after any edits to existing project files

**When NOT to format:**
- Temporary/scratch files that will be deleted immediately
- Files you are only reading, not modifying
- C++ source files, RST docs, Python scripts, etc. (only `.das` files)

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
- **Block arguments with `<|`:** use `$()` or `@()` prefix: `defer() <| $() { ... }` — NOT `defer <| { ... }` (bare `{ }` creates a table literal)
- **Lambda:** `@(args) { body }` or `@@(args) { body }` (no-capture)
- **Generator:** `$() { yield value; }` or `$ { yield value; }` — both are valid gen2 syntax
- **Tuple `=>` operator:** `a => b` creates a `tuple<auto;auto>` — useful in LINQ, table construction, and ad-hoc pairs
- **Bitfield variables** need explicit type for `.field` access and printing: `var f : MyBitfield`
- **Bitfield dot access:** read with `f.flag` (returns bool), write with `f.flag = true/false`
- **`typeinfo`** special syntax: `typeinfo enum_length(type<MyEnum>)` — NOT `typeinfo(enum_length type<MyEnum>)`
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
- **`move` function**: Bound via C++ `builtin_smart_ptr_move*` family in `module_builtin_runtime.cpp` — proper smart pointer move with reference counting. Use `move(dest, src)` or `move(dest) <| src` for `smart_ptr<T>` transfers.
- **`return <- expr`**: Moves value to return slot and zeroes `expr`. If `expr` is a `&` ref parameter, this zeroes the *caller's* variable since they share memory.
- **Key subtlety**: When a function takes `var x : smart_ptr<T>&` (by reference) and internally calls a function that takes `var x : smart_ptr<T>` (by value/move), the `<-` inside `return` zeroes the `&` ref. Callers MUST capture the return value: `unsafe { expr <- apply_template(expr) <| ... }`
- **Safe pattern**: `unsafe { variable <- function_returning_smart_ptr(variable) <| ... }` — captures return value back into the same variable

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

### Common gotchas

- Lambda params can shadow function params — use distinct names (e.g., `$(lhs, rhs)` not `$(a, b)` when `a` is already in scope)
- `struct Foo { ... }` requires braces in gen2 — empty struct is `struct Foo {}`
- String builder: `build_string() <| $(var writer) { write(writer, "text") }` — requires `unsafe` or `options persistent_heap` if returned
- `options persistent_heap` — needed when returning strings built with `build_string`
- Tuple field access uses underscore prefix: `t._0`, `t._1`, `t._2`
- Annotations: `[export]` for entry points, `[private]` for private functions, `[test]` for test functions
- `options no_aot` — disable ahead-of-time compilation (common in test files)
- `options rtti` — enable runtime type information (needed for some daslib features)
- `require` uses forward slash paths: `require daslib/linq` — NOT `require daslib\linq`
- `require foo public` — re-exports `foo` so that any module requiring the current module also sees `foo`'s symbols transitively. Example: `regex.das` has `require strings public`, so `require daslib/regex` automatically makes `slice`, `starts_with`, etc. visible without a separate `require strings`
- `<-` is memcpy+memset(0), NOT a smart_ptr-aware move — see "Move semantics" section above
- When calling `apply_template`, always capture the return value: `unsafe { expr <- apply_template(expr) <| ... }` — discarding the return loses the expression data
- Iterator comprehension: `[iterator for(x in src); expression]` — semicolon separates generator from body
- `to_array` (from `daslib/builtin`) converts any iterator to an array
- Lambdas CAN be stored in arrays: `var fns : array<lambda<(x:int):int>>` + `fns |> emplace() <| @(x : int) : int { return x * 2; }` — move semantics, not copy
- Blocks CANNOT be stored in containers, returned from functions, or captured — use lambdas or function pointers for those use cases
- `match`, `multi_match`, `static_match` macros (from `daslib/match.das`) handle side effects automatically — do NOT add `[sideeffects]` annotations to functions that only use match
- `[export] def main()` returns `void` — do NOT `return true` or return other values from main

## Key Directories

- `src/` — C++ compiler/runtime source
- `include/daScript/` — C++ headers
- `daslib/` — Standard library modules (86 .das files)
- `dastest/` — Test framework
- `tests/` — Test suite (with per-module subfolders: `tests/decs/`, `tests/match/`, `tests/json/`, etc.)
- `doc/source/reference/language/` — RST language documentation (36 files)
- `doc/source/stdlib/` — RST standard library documentation (auto-generated + handmade)
- `doc/reflections/` — Documentation generation tools (das2rst.das, rst.das, gen_module_examples.py)
- `tutorials/language/` — Language tutorial `.das` files (32 progressive tutorials)
- `tutorials/integration/cpp/` — C++ integration tutorials (embedding daslang in C++ host applications)
- `doc/source/reference/tutorials/` — RST companion pages for each tutorial
- `tests/linq/` — LINQ module tests (15 test files, ~500 tests)
- `tests/functional/` — Functional module tests
- `tests/json/` — JSON module tests (4 test files, ~148 tests)
- `tests/regex/` — Regex module tests (8 test files, 278 tests)
- `modules/` — External plugin modules

## Standard Library Documentation

The stdlib docs live in `doc/source/stdlib/` and are generated from `//!` doc-comments in `daslib/*.das`.

### Documentation pipeline

1. `daslib/*.das` files contain `//!` comments for each module, struct, function
2. `doc/reflections/rst_comment.das` extracts comments into `doc/source/stdlib/detail/*.rst`
3. `doc/reflections/das2rst.das` + `doc/reflections/rst.das` combine detail RST with handmade content
4. **`doc/source/stdlib/handmade/`** — manually written module descriptions and examples (2,001 files)
5. Final RST output goes to `doc/source/stdlib/*.rst`

### Key documentation tools

- `doc/reflections/gen_module_examples.py` — generates/updates all 86 module-*.rst files with descriptions and compilable examples (31 modules have examples)
- `doc/reflections/test_new_examples.py` — tests all example snippets by running them through `daslang.exe`
- `doc/reflections/fix_short_docs.py` — fixes terse function documentation
- Validate: `bin/Release/daslang.exe doc/reflections/das2rst.das` (exit code 0 = success)

### Function grouping in generated docs

Functions in each module's RST are organized into named groups (e.g. "Compilation and validation", "Access", "Match & replace"). Groups are defined in `doc/reflections/das2rst.das` via `group_by_regex("Group Name", mod, %regex~(func1|func2)$%%)`. Any public function not matched by a group regex ends up in an **"Uncategorized"** section. After adding new public functions to a module:

1. Add the function name to the appropriate `group_by_regex` call in the module's `document_module_*` function in `das2rst.das`
2. Create or update the handmade doc file in `doc/source/stdlib/handmade/` (replace `// stub` with a description)
3. Regenerate: `bin/Release/daslang.exe doc/reflections/das2rst.das`
4. Verify no "Uncategorized" section remains: search for `Uncategorized` in the generated `doc/source/stdlib/*.rst`

### Adding a module example

1. Add `example="""..."""` to the `reg()` call in `gen_module_examples.py`
2. Test it: add to `test_new_examples.py` and run `python doc/reflections/test_new_examples.py`
3. Regenerate: `python doc/reflections/gen_module_examples.py`
4. Validate: `bin/Release/daslang.exe doc/reflections/das2rst.das`

## Documentation Conventions

When editing RST files in `doc/source/reference/language/`:

- All code blocks use `.. code-block:: das` with gen2 syntax
- Include `// output:` comments showing expected output for runnable examples
- Add `require` statements when examples need imports
- Use `:ref:` cross-references to link between pages (labels: `_structs`, `_classes`, `_functions`, `_statements`, `_expressions`, `_arrays`, `_tables`, `_iterators`, `_generators`, `_lambdas`, `_blocks`, `_tuples`, `_variants`, `_bitfields`, `_aliases`, `_modules`, `_options`, `_unsafe`, `_enumerations`, `_generic_programming`, `_pattern-matching`, `_comprehensions`, `_string_builder`, `_macros`, `_reification`, `_finalizers`, `_clone`, `_temporary`, `_move_copy_clone`, `_annotations`, `_program_structure`, `_type_conversions`, `_contexts`, `_locks`, `_datatypes_and_values`, `_pointers`)
- Verify examples compile: `bin/Release/daslang.exe example.das`

### RST table rules

RST uses two table formats — **grid tables** and **simple tables**. Both are fragile:

- **Grid tables** (`+---+---+`): Every row line must be exactly the same width as every separator line. Off-by-one spaces cause Sphinx errors.
- **Simple tables** (`===  ===`): The `=` separator defines column widths. Content in non-last columns must NOT extend past its column's `=` boundary. Headers must start at or after the column's start position (not in the gap). The gap between columns must be at least 2 spaces.
- After creating or editing any RST table, verify the file with a Sphinx build (see below).

### Documentation workflow (REQUIRED)

After creating or modifying any RST files, stdlib documentation, or `daslib/*.das` module doc-comments:

1. **Regenerate stdlib docs** (if `daslib/*.das` files or `doc/reflections/das2rst.das` were changed):
   ```
   bin/Release/daslang.exe doc/reflections/das2rst.das
   ```

2. **Clean Sphinx build** — MUST delete cache; cached builds hide errors:
   ```
   cd doc
   Remove-Item -Recurse -Force sphinx-build    # delete doctree cache
   Remove-Item -Recurse -Force ../site/doc     # delete HTML output
   sphinx-build -b html -d sphinx-build source ../site/doc
   ```
   On Linux/Mac:
   ```
   cd doc
   rm -rf sphinx-build ../site/doc
   sphinx-build -b html -d sphinx-build source ../site/doc
   ```

3. **Verify no new errors or warnings**: Check the build output for `ERROR` and `WARNING`. The build must introduce **no new** Sphinx errors or warnings compared to the baseline.

**When to run the workflow:**
- New or modified RST files (language docs, tutorials, stdlib docs)
- New or modified `//!` doc-comments in `daslib/*.das` files
- Changes to `doc/reflections/das2rst.das` or `doc/reflections/rst.das`
- New public functions added to any `daslib/*.das` module (also update `group_by_regex` in `das2rst.das`)

### Tutorial RST conventions

Tutorial RST files live in `doc/source/reference/tutorials/` with companion `.das` files in `tutorials/language/`.

- Each RST starts with a label: `.. _tutorial_name:` (e.g., `.. _tutorial_linq:`)
- Include `.. index::` directive with relevant `single: Tutorial; Topic` entries
- Code blocks use `.. code-block:: das` with gen2 syntax
- End each RST with a `.. seealso::` block containing:
  - Full source as `:download:` link: `Full source: :download:\`tutorials/language/XX_name.das <../../../../tutorials/language/XX_name.das>\``
  - Next tutorial link (except last): `Next tutorial: :ref:\`tutorial_next_name\``
  - Related language reference links via `:ref:`
- Toctree is in `doc/source/reference/tutorials.rst` — add new tutorials there

### C++ integration tutorial RST conventions

C++ integration tutorial RST files live in `doc/source/reference/tutorials/` with `.cpp` and `.das` files in `tutorials/integration/cpp/`.

- Label pattern: `.. _tutorial_integration_cpp_<topic>:` (e.g., `.. _tutorial_integration_cpp_binding_types:`)
- Index entries: `single: Tutorial; C++ Integration; <Topic>`
- Code blocks: `.. code-block:: cpp` for C++ code, `.. code-block:: das` for daslang code
- Build & run section with `cmake --build` command and expected output
- End with `.. seealso::` containing:
  - `:download:` links for both `.cpp` and `.das` source files
  - Previous/Next tutorial links via `:ref:`
- Each tutorial is one self-contained `.cpp` file with embedded `main()` — no separate build infrastructure needed beyond CMake target
- Tutorial CMake targets: `integration_cpp_01` through `integration_cpp_NN` (defined in `tutorials/integration/cpp/CMakeLists.txt`)

- Tutorial labels for cross-references: `tutorial_hello_world`, `tutorial_variables`, `tutorial_operators`, `tutorial_control_flow`, `tutorial_functions`, `tutorial_arrays`, `tutorial_strings`, `tutorial_structs`, `tutorial_enumerations`, `tutorial_tables`, `tutorial_tuples_and_variants`, `tutorial_function_pointers`, `tutorial_blocks`, `tutorial_lambdas`, `tutorial_iterators_and_generators`, `tutorial_modules`, `tutorial_move_copy_clone`, `tutorial_classes`, `tutorial_generics`, `tutorial_lifetime`, `tutorial_error_handling`, `tutorial_unsafe`, `tutorial_string_format`, `tutorial_pattern_matching`, `tutorial_annotations`, `tutorial_contracts`, `tutorial_testing`, `tutorial_linq`, `tutorial_functional`, `tutorial_json`, `tutorial_regex`, `tutorial_operator_overloading`, `tutorial_pointers`, `tutorial_utility_patterns`, `tutorial_random`, `tutorial_dynamic_type_checking`, `tutorial_coroutines`, `tutorial_serialization`, `tutorial_testing_tools`
- C++ integration tutorial labels: `tutorial_integration_cpp_hello_world`, `tutorial_integration_cpp_calling_functions`, `tutorial_integration_cpp_binding_functions`, `tutorial_integration_cpp_binding_types`, `tutorial_integration_cpp_binding_enums`, `tutorial_integration_cpp_interop`, `tutorial_integration_cpp_callbacks`, `tutorial_integration_cpp_methods`, `tutorial_integration_cpp_operators_and_properties`
- C++ integration tutorial plan (remaining): 10 Custom Modules, 11 Context Variables, 12 Smart Pointers & GC, 13 AOT, 14 Serialization, 15 Custom Annotations, 16 Sandbox

## C++ Integration Patterns

These patterns are used in C++ host applications that embed daslang.

### Host program boilerplate

Every C++ host that runs daslang scripts follows this pattern:

```cpp
#include "daScript/daScript.h"
using namespace das;

void run_script() {
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    auto fAccess = make_smart<FsFileAccess>();
    auto program = compileDaScript(getDasRoot() + "/path/to/script.das",
                                   fAccess, tout, dummyLibGroup);
    if (program->failed()) { /* report errors */ return; }
    Context ctx(program->getContextStackSize());
    if (!program->simulate(ctx, tout)) { /* report errors */ return; }
    auto fn = ctx.findFunction("test");
    ctx.evalWithCatch(fn, nullptr);
}

int main(int, char * []) {
    NEED_ALL_DEFAULT_MODULES;
    NEED_MODULE(Module_MyModule);  // custom modules
    Module::Initialize();
    run_script();
    Module::Shutdown();
    return 0;
}
```

### Creating a module

Derive from `Module`, register types/functions/enums in the constructor, then use `REGISTER_MODULE`:

```cpp
class Module_MyMod : public Module {
public:
    Module_MyMod() : Module("my_module_name") {
        ModuleLibrary lib(this);
        lib.addBuiltInModule();
        // addAnnotation, addExtern, addEnumeration, addConstant ...
    }
};
REGISTER_MODULE(Module_MyMod);
```

The host uses `NEED_MODULE(Module_MyMod)` before `Module::Initialize()`.  Scripts access it via `require my_module_name`.

### Callbacks — `TBlock<>`, `TFunc<>`, `TLambda<>`, `das_invoke*`

Three closure types exist, each with a typed template and an invocation helper:

| Type | Template | Invocation | Lifetime |
|------|----------|------------|----------|
| Block | `TBlock<Ret, Args...>` | `das_invoke<Ret>::invoke(ctx, at, blk, args...)` | Stack-bound — valid only during the call |
| Func | `TFunc<Ret, Args...>` (or untyped `Func`) | `das_invoke_function<Ret>::invoke(ctx, at, fn, args...)` | Context-bound — storable |
| Lambda | `TLambda<Ret, Args...>` (or untyped `Lambda`) | `das_invoke_lambda<Ret>::invoke(ctx, at, lmb, args...)` | Heap-allocated — captures variables |

**Typed vs untyped**: `TBlock<int,int>` maps to `block<(arg:int):int>` in daslang — the compiler checks signatures. Untyped `Lambda` maps to `lambda<>` and will **not** match typed lambdas like `lambda<(x:int):int>`. Prefer typed templates.

**Block callback example**:

```cpp
void with_values(int32_t a, int32_t b,
                 const TBlock<void, int32_t, int32_t> & blk,
                 Context * context, LineInfoArg * at) {
    das_invoke<void>::invoke(context, at, blk, a, b);
}

addExtern<DAS_BIND_FUN(with_values)>(*this, lib, "with_values",
    SideEffects::invoke, "with_values")
        ->args({"a", "b", "blk", "context", "at"});
```

Use `SideEffects::invoke` for any function that invokes script callbacks.

In daslang: blocks use `<|` with `$()` prefix, function pointers use `@@func_name`, lambdas use `@(args) { body }`.

### Calling daslang functions from C++ — `das_invoke_function`

The high-level `das_invoke_function<ReturnType>::invoke(ctx, at, fnPtr, arg1, arg2, ...)` handles argument marshalling automatically.  Preferred over raw `cast<>` + `evalWithCatch`.

### Binding C++ functions — `addExtern` + `DAS_BIND_FUN`

```cpp
addExtern<DAS_BIND_FUN(cpp_function)>(*this, lib, "das_name",
    SideEffects::none, "cpp_function")
        ->args({"param1", "param2"});
```

`SideEffects` flags: `none` (pure), `modifyExternal` (stdout/files), `modifyArgument` (mutates ref params), `accessGlobal` (reads shared state), `invoke` (calls daslang), `worstDefault` (safe fallback).

### Binding C++ types — `MAKE_TYPE_FACTORY` + `ManagedStructureAnnotation`

1. **`MAKE_TYPE_FACTORY(DasName, CppType)`** at file scope — creates `typeFactory<CppType>` + `typeName<CppType>`
2. **`ManagedStructureAnnotation<T, canNew, canDelete>`** — describe fields with `addField<DAS_BIND_MANAGED_FIELD(member)>("name", "name")`
3. **`addAnnotation(make_smart<MyAnnotation>(lib))`** in the module — order matters: if type B contains type A, register A first
4. Functions returning bound types by value require **`SimNode_ExtFuncCallAndCopyOrMove`** template parameter in `addExtern`

**Handled types are reference types** — important consequences for scripts:
- Mutable local variables (`var`) of handled types require `unsafe` blocks
- Immutable locals (`let`) returned from factory functions work without `unsafe`
- **Factory function pattern**: provide `make_xxx()` functions returning by value so scripts can create instances ergonomically with `let x = make_xxx(...)` — no `unsafe` needed
- POD structs (no default member initializers, no virtual functions) work best with `ManagedStructureAnnotation`

### Binding C++ methods — `DAS_CALL_MEMBER` + `DAS_CALL_METHOD`

daslang has no member functions — "methods" are free functions where the first argument is `self`. Pipe syntax (`obj |> method()`) provides method-call ergonomics.

```cpp
// Step 1: Create wrapper aliases
using method_increment = DAS_CALL_MEMBER(Counter::increment);
using method_get       = DAS_CALL_MEMBER(Counter::get);

// Step 2: Register with addExtern
addExtern<DAS_CALL_METHOD(method_increment)>(*this, lib, "increment",
    SideEffects::modifyArgument,
    DAS_CALL_MEMBER_CPP(Counter::increment))
        ->args({"self"});

addExtern<DAS_CALL_METHOD(method_get)>(*this, lib, "get",
    SideEffects::none,
    DAS_CALL_MEMBER_CPP(Counter::get))
        ->args({"self"});
```

- **Non-const methods**: `SideEffects::modifyArgument` (they mutate the object)
- **Const methods**: `SideEffects::none`
- `DAS_CALL_MEMBER_CPP(Class::method)` provides the AOT-compatible name string

### Binding operators and properties

**Operators**: register functions with the operator symbol as the daslang name:

```cpp
addExtern<DAS_BIND_FUN(vec3_add), SimNode_ExtFuncCallAndCopyOrMove>(
    *this, lib, "+", SideEffects::none, "vec3_add")->args({"a", "b"});
// Unary: addExtern<...>(*this, lib, "-", ...)->args({"a"});
```

Available operator names: `+`, `-`, `*`, `/`, `%`, `<<`, `>>`, `<`, `>`, `<=`, `>=`, `&`, `|`, `^`.

**Equality**: `addEquNeq<T>(*this, lib)` binds both `==` and `!=` (requires `operator==` and `operator!=` on T).

**Properties**: method calls disguised as field access in `ManagedStructureAnnotation`:

```cpp
addProperty<DAS_BIND_MANAGED_PROP(length)>("length", "length");
```

In daslang, `v.length` calls `Vec3::length()` — looks like a field, calls a method.

### Binding C++ enums — `DAS_BASE_BIND_ENUM`

```cpp
// MUST come BEFORE `using namespace das` to avoid name collisions
DAS_BASE_BIND_ENUM(CppEnum, DasName, Value1, Value2, Value3)

using namespace das;

// In module constructor:
addEnumeration(make_smart<EnumerationDasName>());
```

- `DAS_BASE_BIND_ENUM` creates class `EnumerationDasName` + `typeFactory<CppEnum>`
- `DAS_BIND_ENUM_CAST(CppEnum)` — explicit `cast<>` specialization (often not needed, SFINAE default suffices)
- `DAS_BASE_BIND_ENUM_98` — for unscoped (C-style) enums
- **Critical**: place enum macros BEFORE `using namespace das` — the macros define names inside `namespace das` that collide with global enum names
- **Name collision pitfall**: `das::LogLevel` is defined internally in `include/daScript/misc/string_writer.h` — do NOT name your enum `LogLevel` when `using namespace das`
- Manual construction alternative: `make_smart<Enumeration>("Name")` + `pEnum->addIEx("Value", "CppEnum::Value", intValue, LineInfo())`

### Low-level interop — `addInterop`

`addInterop` binds a C++ function that receives raw simulation-level arguments (`vec4f *`), the call node (`SimNode_CallBase *`), and the context. Unlike `addExtern`, it supports **"any type" arguments** — when a template parameter is `vec4f`, it means the argument can be any daslang type. The function inspects `call->types[i]` (`TypeInfo *`) at runtime to determine what was actually passed.

**Signature**: the C++ function must match `InteropFunction`:

```cpp
// typedef vec4f (*InteropFunction)(Context &, SimNode_CallBase *, vec4f *);
vec4f my_interop(Context & context, SimNode_CallBase * call, vec4f * args) {
    TypeInfo * ti = call->types[0];   // type info for first argument
    // ... inspect ti->type, ti->structType, etc.
    return v_zero();
}
```

**Registration**:

```cpp
addInterop<my_interop, ReturnType, ArgType1, ArgType2>(
    *this, lib, "das_name", SideEffects::none, "my_interop");
```

Where `vec4f` as an `ArgType` means "any type" — the argument accepts any daslang value. Concrete types (e.g. `int32_t`, `const char *`, `const Block &`) are also valid and work like `addExtern`.

**Key capabilities** (vs `addExtern`):
- Access to `call->types[]` — per-argument `TypeInfo` with full type metadata
- Access to `call->debugInfo` — source location of the call site
- `vec4f` argument type = "any" — accept arguments of any daslang type
- Used internally for `sprint`, `hash`, `write`, `binary_save/load`, `invoke_in_context`

**TypeInfo union warning**: `TypeInfo` has a union — `structType`, `enumType`, and `annotation_or_name` share the same memory. Which member is valid depends on `ti->type`:
- `tStructure` → `ti->structType` (StructInfo *)
- `tEnumeration` / `tEnumeration8` / `tEnumeration16` → `ti->enumType` (EnumInfo *)
- `tHandle` → use `ti->getAnnotation()` (resolves tagged pointer safely)

Accessing the wrong union member is **undefined behavior**. `das_to_string(Type::tHandle)` returns an empty string — use `ti->getAnnotation()->name` for handled type names.

**Example** — `new_and_init` allocates and initializes any struct:

```cpp
vec4f new_and_init(Context & context, SimNode_CallBase * call, vec4f * args) {
    TypeInfo * typeInfo = call->types[0];
    if (typeInfo->type != Type::tStructure)
        context.throw_error_at(call->debugInfo, "expected struct");
    auto size = getTypeSize(typeInfo);
    auto data = context.allocate(size, &call->debugInfo);
    if (typeInfo->structType && typeInfo->structType->init_mnh) {
        auto fn = context.fnByMangledName(typeInfo->structType->init_mnh);
        context.callWithCopyOnReturn(fn, nullptr, data, 0);
    } else {
        memset(data, 0, size);
    }
    return cast<char *>::from(data);
}

addInterop<new_and_init, void *, vec4f>(*this, lib, "new_and_init",
    SideEffects::none, "new_and_init");
```

## C++ Codebase Notes

- Main type inference: `src/ast/ast_infer_type.cpp` (implementation) + `include/daScript/ast/ast_infer_type.h` (class declarations for `CaptureLambda` and `InferTypes`)
- Builtin runtime functions: `src/builtin/module_builtin_runtime.cpp`
- Smart pointer builtins: `move`, `move_new`, `smart_ptr_clone`, `smart_ptr_use_count`
- Compilation errors: `include/daScript/ast/compilation_errors.h` (error codes 10001–40214)
- Lexer: `src/parser/ds2_lexer.lpp`
- Parser: `src/parser/ds2_parser.ypp`

### Key AST function flags

- `func.flags.isClassMethod` — function is a struct/class method (set after inference)
- `func.moreFlags.isStaticClassMethod` — static method (declared with `def static`); name is `StructName\`methodName`, self is explicit first argument
- Non-static methods (`isClassMethod=true`, `isStaticClassMethod=false`) — self is added implicitly during inference; name stays unqualified (e.g. `finalize`, `[]`)
- `func.moreFlags.propertyFunction` — property accessor (name starts with `.\``)
- `func.classParent` — pointer to the struct/class that owns the method

### Table operations

- `table[key]` **inserts** a new default entry if `key` is missing — use only when you want insert-on-access
- `table[key]` requires `unsafe` by default; add `options unsafe_table_lookup = false` to allow safe `[]` access
- `table?[key] ?? default_value` — safe lookup with fallback, does NOT insert missing keys
- `key_exists(table, key)` — check if a key is present without inserting
- `table |> insert(key, value)` — insert into `table<K;V>`; `table |> insert(key)` — insert into set `table<K>`
- `table |> erase(key)` — remove a key
- **Never use two `[]` lookups on the same table in one expression** (e.g. `tab[k1] = tab[k2]`) — tables are unboxed containers and re-hashing on insert can invalidate the first reference
- `find(table, key) <| $(pval) { ... }` — block-based lookup; block receives pointer to value if found
- `get(table, key, blk)` — similar block-based access (see `daslib/builtin.das`)

## Testing Conventions (dastest)

Tests use the `dastest` framework. Test files live in `tests/` with per-module subfolders.

### Test file structure

```das
options gen2
require dastest/testing_boost public
require daslib/module_under_test

[test]
def test_something(t : T?) {
    t |> run("description") <| @(t : T?) {
        t |> equal(actual, expected)
        t |> success()
    }
}
```

### Key test functions

- `t |> equal(actual, expected)` — value equality assertion
- `t |> success()` — mark subtest as passed
- `t |> run("name") <| @(t : T?) { ... }` — named subtest
- `t |> equal(actual, true)` / `t |> equal(actual, false)` — boolean assertions (there is no `expect_true`/`expect_false`)
- `t |> strictEqual(actual, expected)` — strict equality assertion

### Common test options

- `options no_unused_function_arguments = false` — suppress warnings for test params
- `options no_unused_block_arguments = false` — suppress warnings for block params
- Shared test helpers go in `_common.das` module files (e.g., `tests/linq/_common.das`)

### Running tests

- Single file: `bin/Release/daslang.exe dastest/dastest.das -- --test tests/linq/test_linq_aggregation.das`
- Directory: `bin/Release/daslang.exe dastest/dastest.das -- --test tests/linq/`
- All tests: `bin/Release/daslang.exe dastest/dastest.das -- --test tests/`

## Standard Library Module Conventions

### Base + boost pattern

Many modules come in pairs: `daslib/foo.das` (core) + `daslib/foo_boost.das` (macro layer):

- **Base module** (`linq.das`, `json.das`, `regex.das`, etc.): pure functional API, runtime functions, iterator implementations
- **Boost module** (`linq_boost.das`, `json_boost.das`, etc.): macro-based sugar, compile-time optimizations, pipe-syntax rewrites
- All boost modules re-export their base module publicly (`require daslib/foo public`), so only `require daslib/foo_boost` is needed — do NOT add a separate `require daslib/foo`
- `regex.das` also has `require strings public`, so `require daslib/regex` (or `require daslib/regex_boost`) makes `slice`, `starts_with`, etc. available
- Example: `linq.das` provides `where`, `select`, `order_by` functions; `linq_boost.das` adds `_fold` macro that rewrites iterator chains into imperative loops

### Iterator implementation pattern

Many daslib functions follow this convention for iterator-based operations:

- `foo_impl` — internal generator function (yields values)
- `foo` — public function returning `iterator<T>` (calls `_impl`)
- `foo_to_array` — convenience wrapper returning `array<T>` (pipes through `to_array`)
- Inplace `foo` on arrays — overload taking `var arr : array<T>` and modifying in place

### Key daslib modules

- `daslib/linq.das` — LINQ-style queries (where, select, order_by, group_by, zip, etc.)
- `daslib/linq_boost.das` — `_fold` optimization macro, pipe-syntax macros
- `daslib/match.das` — pattern matching on variants and types
- `daslib/templates_boost.das` — template/reification infrastructure for AST macros; `apply_template` rewrites AST nodes
- `daslib/functional.das` — lazy iterator adapters and higher-order function utilities (filter, map, reduce, fold, scan, enumerate, chain, pairwise, iterate, find, find_index, partition, tap, for_each, flat_map, sorted, repeat, cycle, islice, echo, sum, any, all). Uses lambdas/functions for generator-returning functions (blocks cannot be captured into generators). Non-generator functions (reduce, fold, for_each, find, find_index, partition) also accept blocks.
- `daslib/strings_boost.das` — string manipulation helpers
- `daslib/json.das` / `daslib/json_boost.das` — JSON parsing/generation. Core: `JsValue` variant (7 types: `_object`, `_array`, `_string`, `_number`, `_longint`, `_bool`, `_null`), `JsonValue` struct wrapper, `read_json`, `write_json`, `JV()` constructors, `JVNull()`. Boost: safe access (`?.`, `?[]`, `??`), `from_JV`/`JV` generic struct↔JSON conversion, `%json~...%%` reader macro, `BetterJsonMacro` (`is`/`as` on `JsonValue?`). Settings: `set_no_trailing_zeros`, `set_no_empty_arrays`, `set_allow_duplicate_keys`. `try_fixing_broken_json` repairs LLM output. Key gotcha: `js?.value` accesses `JsonValue.value` field (returns `JsValue`), not a JSON key named "value" — use `js?["value"]` for that.
- `daslib/regex.das` / `daslib/regex_boost.das` — regular expressions. Re-exports `strings` publicly (`require strings public`), so `require daslib/regex` makes `slice`, `starts_with`, etc. available. Core: recursive-descent parser building `ReNode` AST, function-pointer-driven backtracking matcher. `Regex` struct, `regex_compile(pattern, case_insensitive=false, dot_all=false)`, `regex_match(re, str, offset=0)` → end position or -1, `regex_search(re, str, offset=0)` → `int2(start, end)` or `int2(-1,-1)` (finds first match anywhere), `regex_group(re, group_num, str)` → captured substring, `regex_group_by_name(re, name, str)` → named group substring, `re[index]` → `range` for group by int index (1-based), `re["name"]` → `range` for named group (returns `range(0,0)` if not found), `regex_foreach(re, str, block)` iterates all matches passing `range` values, `regex_replace(re, str, block)` replaces matches via block, `regex_replace(re, str, replacement)` replaces matches with template string (`$0`/`$&` for whole match, `$1`-`$9` for numbered groups, `${name}` for named groups, `$$` for literal `$`), `regex_split(re, str)` → `array<string>` of substrings between matches, `regex_match_all(re, str)` → `array<range>` of all match ranges, `is_valid(re)` checks compilation. Supports: `.` (any char except newline — use `dot_all=true` to also match `\n`), `^` (BOL), `$` (EOL), `+` `*` `?` quantifiers (greedy), `+?` `*?` `??` quantifiers (lazy), `{n}` `{n,}` `{n,m}` counted quantifiers (greedy), `{n}?` `{n,}?` `{n,m}?` counted quantifiers (lazy), `(...)` capturing groups, `(?:...)` non-capturing groups, `(?P<name>...)` named capturing groups, `(?=...)` positive lookahead, `(?!...)` negative lookahead, `|` alternation, `[abc]` `[a-z]` `[^...]` character sets, `\w` `\W` `\d` `\D` `\s` `\S` classes, `\b` `\B` word boundaries, `\t` `\n` `\r` `\f` `\v` escapes, `\xHH` hex escapes. ASCII only (256-bit CharSet). Flags: `case_insensitive=true` for case-insensitive matching (ASCII only), `dot_all=true` for dot matching newline. Boost: `%regex~pattern%%` reader macro (compile-time, no double-escaping); flags via `%regex~pattern~flags%%` where `i`=case-insensitive, `s`=dotAll. Key gotchas: `{` must be escaped as `\{` in daslang strings for counted quantifiers (`"\\d\{3}"`), but reader macro takes literal text (`%regex~\d{3}%%`). `regex_match` always matches from position 0 (or offset) — it does NOT search for the pattern; use `regex_search` for first occurrence or `regex_foreach`/`regex_match_all` to find all occurrences. Nested groups have limited support — prefer sequential groups. `-` is only special inside `[...]` character sets. Quantifiers on lookaheads are not allowed.
- `daslib/flat_hash_table.das` — template-based open-addressing hash table (`TFlatHashTable`) with methods: `empty`, `length`, `clear`, `grow`, `rehash`, `reserve`, `key_index`, `key_exists`, `get`, `erase`, `foreach`, `keys`, `values`, `operator[]`, `operator?[]`
- `daslib/builtin.das` — core builtins like `to_array`, `to_table`

## Keywords Reference

`aka` — variable name alias (`var a aka alpha = 42`, `for (x aka element in arr)`)
`inscope` — declares variable owns smart_ptr lifetime (`var inscope p = ...`)
`is type<T>` — compile-time type check (`a is type<int>` returns bool)
`expect` — suppress specific compilation errors in test files (`expect 30507`)
`template` — generic type constraint in function signatures
