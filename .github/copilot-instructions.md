# daScript Project Instructions

## Project Overview

This is the [daScript](https://dascript.org/) programming language repository (GaijinEntertainment/daScript). daScript is a high-performance statically-typed scripting language designed for games and real-time applications.

## Build & Run

- **Build system:** CMake + MSVC (Visual Studio 2022)
- **Generate:** `generate_msvc_2022.bat` → creates `build/DAS.sln`
- **Build:** `cmake --build build --config Release`
- **Compiler binary:** `bin/Release/daslang.exe`
- **Run a script:** `bin/Release/daslang.exe path/to/script.das`
- **Run tests:** `bin/Release/daslang.exe dastest/dastest.das -- --test path/to/test.das`

## daScript Language — Gen2 Syntax (REQUIRED)

All code examples and documentation MUST use gen2 syntax (add `options gen2` at the top of every file). Key rules:

- **Parentheses** on control flow: `if (x > 0)`, `for (i in range(10))`, `while (running)`
- **Braces** on all blocks — no indentation-based blocks: `def foo() { ... }`, `if (x) { ... }`
- **Construction:** `new Type(field=val)` — NOT `new [[Type() field=val]]`
- **Enum access:** `EnumName.EnumValue` with dot — NOT `EnumName EnumValue` without
- **Array literals:** `[1, 2, 3]` (commas, square brackets) — NOT `[[int 1; 2; 3]]`
- **Struct init:** `Foo(a=1, b=2)` — NOT `[[Foo() a=1, b=2]]`
- **No `[[ ]]` for `new`** — `new` always uses parentheses: `new Foo(x=1)`
- **Block arguments with `<|`:** use `$()` or `@()` prefix: `defer() <| $() { ... }` — NOT `defer <| { ... }` (bare `{ }` creates a table literal)
- **Lambda:** `@(args) { body }` or `@@(args) { body }` (no-capture)
- **`static_if`:** `static_if (condition) { ... }` — parentheses required in gen2

### Important defaults

- `strict_smart_pointers` is ON by default — smart_ptr variables require `var inscope` declaration
- `smart_ptr<T>` only works with C++ handled types (not user structs)
- No implicit type promotion: `int + float` is a compile error — both sides must match
- No `bool(int)` cast — use `x != 0`
- `string(x)` works for any type; `int("123")` does NOT work — use `to_int` from `require strings`
- Hex literals like `0x3F800000` are `uint` by default — use `int(0x3F)` for int

## Key Directories

- `src/` — C++ compiler/runtime source
- `include/daScript/` — C++ headers
- `daslib/` — Standard library modules (86 .das files)
- `dastest/` — Test framework
- `tests/` — Test suite (with per-module subfolders: `tests/decs/`, `tests/match/`, `tests/json/`, etc.)
- `doc/source/reference/language/` — RST language documentation (36 files)
- `doc/source/stdlib/` — RST standard library documentation (auto-generated + handmade)
- `doc/reflections/` — Documentation generation tools (das2rst.das, rst.das, gen_module_examples.py)
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
- Use `:ref:` cross-references to link between pages (labels: `_structs`, `_classes`, `_functions`, `_statements`, `_expressions`, `_arrays`, `_tables`, `_iterators`, `_generators`, `_lambdas`, `_blocks`, `_tuples`, `_variants`, `_bitfields`, `_aliases`, `_modules`, `_options`, `_unsafe`, `_enumerations`, `_generic_programming`, `_pattern-matching`, `_comprehensions`, `_string_builder`, `_macros`, `_reification`, `_finalizers`, `_clone`, `_temporary`, `_move_copy_clone`, `_annotations`, `_program_structure`, `_type_conversions`, `_contexts`, `_locks`, `_datatypes_and_values`)
- Verify examples compile: `bin/Release/daslang.exe example.das`

## C++ Codebase Notes

- Main type inference: `src/ast/ast_infer_type.cpp` (very large file)
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

## Keywords Reference

`aka` — variable name alias (`var a aka alpha = 42`, `for (x aka element in arr)`)
`inscope` — declares variable owns smart_ptr lifetime (`var inscope p = ...`)
`is type<T>` — compile-time type check (`a is type<int>` returns bool)
`expect` — suppress specific compilation errors in test files (`expect 30507`)
`template` — generic type constraint in function signatures
