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
- **Table literals:** `{ "k" => v, "k2" => v2 }` (single braces, commas) — NOT `{{ "k" => v; "k2" => v2 }}`
- **Named arguments:** `foo([name = value])` with square brackets — NOT `foo(name=value)`
- **Block arguments with `<|`:** use `$()` or `@()` prefix: `defer() <| $() { ... }` — NOT `defer <| { ... }` (bare `{ }` creates a table literal)
- **Lambda:** `@(args) { body }` or `@@(args) { body }` (no-capture)
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

- daScript has garbage collection — `delete` is not required in most code
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

## Key Directories

- `src/` — C++ compiler/runtime source
- `include/daScript/` — C++ headers
- `daslib/` — Standard library modules (86 .das files)
- `dastest/` — Test framework
- `tests/` — Test suite (with per-module subfolders: `tests/decs/`, `tests/match/`, `tests/json/`, etc.)
- `doc/source/reference/language/` — RST language documentation (36 files)
- `doc/source/stdlib/` — RST standard library documentation (auto-generated + handmade)
- `doc/reflections/` — Documentation generation tools (das2rst.das, rst.das, gen_module_examples.py)
- `tutorials/language/` — Language tutorial `.das` files (29 progressive tutorials)
- `doc/source/reference/tutorials/` — RST companion pages for each tutorial
- `tests/linq/` — LINQ module tests (15 test files, ~500 tests)
- `tests/functional/` — Functional module tests
- `tests/json/` — JSON module tests (4 test files, ~148 tests)
- `tests/regex/` — Regex module tests (5 test files, 162 tests)
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
- Use `:ref:` cross-references to link between pages (labels: `_structs`, `_classes`, `_functions`, `_statements`, `_expressions`, `_arrays`, `_tables`, `_iterators`, `_generators`, `_lambdas`, `_blocks`, `_tuples`, `_variants`, `_bitfields`, `_aliases`, `_modules`, `_options`, `_unsafe`, `_enumerations`, `_generic_programming`, `_pattern-matching`, `_comprehensions`, `_string_builder`, `_macros`, `_reification`, `_finalizers`, `_clone`, `_temporary`, `_move_copy_clone`, `_annotations`, `_program_structure`, `_type_conversions`, `_contexts`, `_locks`, `_datatypes_and_values`)
- Verify examples compile: `bin/Release/daslang.exe example.das`

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
- Tutorial labels for cross-references: `tutorial_hello_world`, `tutorial_variables`, `tutorial_operators`, `tutorial_control_flow`, `tutorial_functions`, `tutorial_arrays`, `tutorial_strings`, `tutorial_structs`, `tutorial_enumerations`, `tutorial_tables`, `tutorial_tuples_and_variants`, `tutorial_function_pointers`, `tutorial_blocks`, `tutorial_lambdas`, `tutorial_iterators_and_generators`, `tutorial_modules`, `tutorial_move_copy_clone`, `tutorial_classes`, `tutorial_generics`, `tutorial_lifetime`, `tutorial_error_handling`, `tutorial_unsafe`, `tutorial_string_format`, `tutorial_pattern_matching`, `tutorial_annotations`, `tutorial_contracts`, `tutorial_testing`, `tutorial_linq`, `tutorial_functional`, `tutorial_json`, `tutorial_regex`

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
- `daslib/regex.das` / `daslib/regex_boost.das` — regular expressions. Re-exports `strings` publicly (`require strings public`), so `require daslib/regex` makes `slice`, `starts_with`, etc. available. Core: recursive-descent parser building `ReNode` AST, function-pointer-driven backtracking matcher. `Regex` struct, `regex_compile(pattern)`, `regex_match(re, str, offset=0)` → end position or -1, `regex_search(re, str, offset=0)` → `int2(start, end)` or `int2(-1,-1)` (finds first match anywhere), `regex_group(re, group_num, str)` → captured substring, `regex_group_by_name(re, name, str)` → named group substring, `re[index]` → `range` for group by int index (1-based), `re["name"]` → `range` for named group (returns `range(0,0)` if not found), `regex_foreach(re, str, block)` iterates all matches passing `range` values, `regex_replace(re, str, block)` replaces matches, `regex_split(re, str)` → `array<string>` of substrings between matches, `regex_match_all(re, str)` → `array<range>` of all match ranges, `is_valid(re)` checks compilation. Supports: `.` (any), `^` (BOL), `$` (EOL), `+` `*` `?` quantifiers (greedy), `+?` `*?` `??` quantifiers (lazy), `{n}` `{n,}` `{n,m}` counted quantifiers (greedy), `{n}?` `{n,}?` `{n,m}?` counted quantifiers (lazy), `(...)` capturing groups, `(?:...)` non-capturing groups, `(?P<name>...)` named capturing groups, `|` alternation, `[abc]` `[a-z]` `[^...]` character sets, `\w` `\W` `\d` `\D` `\s` `\S` classes, `\b` `\B` word boundaries, `\t` `\n` `\r` `\f` `\v` escapes, `\xHH` hex escapes. ASCII only (256-bit CharSet). Boost: `%regex~pattern%%` reader macro (compile-time, no double-escaping). Key gotchas: `{` must be escaped as `\{` in daScript strings for counted quantifiers (`"\\d\{3}"`), but reader macro takes literal text (`%regex~\d{3}%%`). `regex_match` always matches from position 0 (or offset) — it does NOT search for the pattern; use `regex_search` for first occurrence or `regex_foreach`/`regex_match_all` to find all occurrences. Nested groups have limited support — prefer sequential groups. `-` is only special inside `[...]` character sets.
- `daslib/builtin.das` — core builtins like `to_array`, `to_table`

## Keywords Reference

`aka` — variable name alias (`var a aka alpha = 42`, `for (x aka element in arr)`)
`inscope` — declares variable owns smart_ptr lifetime (`var inscope p = ...`)
`is type<T>` — compile-time type check (`a is type<int>` returns bool)
`expect` — suppress specific compilation errors in test files (`expect 30507`)
`template` — generic type constraint in function signatures
