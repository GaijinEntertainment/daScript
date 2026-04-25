# Documentation Conventions

## Standard Library Documentation

The stdlib docs live in `doc/source/stdlib/` and are generated from `//!` doc-comments in source files.

### Documentation pipeline

1. Source files contain `//!` doc-comments for each module, struct, function:
   - `daslib/*.das` — standard library modules
   - `modules/<ModuleName>/<subdir>/*.das` — external module daScript code (e.g. `modules/dasStbImage/stbimage/stbimage_boost.das`)
   - C++ modules get their docs from handmade files only (no `//!` comments)
2. `doc/reflections/rst_comment.das` extracts `//!` comments into `doc/source/stdlib/detail/*.rst`
3. `doc/reflections/das2rst.das` + `doc/reflections/rst.das` combine detail RST with handmade content
4. **`doc/source/stdlib/handmade/`** — manually written descriptions (fallback/supplement for functions, types, enums)
5. **`doc/source/stdlib/detail/`** — auto-generated from `//!` comments (do NOT edit manually)
6. Final RST output goes to **`doc/source/stdlib/generated/`** (NOT `doc/source/stdlib/` root)

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

#### `regex_match` is start-anchored — use `.*` prefix

`group_by_regex` uses `regex_match` (from `daslib/regex`), which matches **only from the start of the string** (offset 0). It does NOT scan/search the full string like `regex_search`. This means:

- A pattern like `(ok|parent)$` will match `"ok"` but **NOT** `".`ok"` (dot-backtick property name)
- C++ modules that register property-style functions (`.`name`) need `.*` prefix: `%regex~.*(ok|parent)$%%`
- **Always prefix patterns with `.*`** unless you are certain all function names in the module start directly with the matched text

#### Backtick function names need `regex_compile`

The `%regex~...%%` reader macro cannot contain literal backtick characters (`` ` ``). For function names that contain backticks (e.g. `` `is`int ``, `` `as`float ``), use `regex_compile` with a string pattern:

```das
var r_type_conv : Regex
regex_compile(r_type_conv, ".*(`is`|`as`|\\\\[\\\\])")
group_by_regex("Type conversion operators", mod, r_type_conv)
```

### Adding a module example

1. Add `example="""..."""` to the `reg()` call in `gen_module_examples.py`
2. Test it: add to `test_new_examples.py` and run `python doc/reflections/test_new_examples.py`
3. Regenerate: `python doc/reflections/gen_module_examples.py`
4. Validate: `bin/Release/daslang.exe doc/reflections/das2rst.das`

### Checking for `// stub` files (REQUIRED after regeneration)

When `das2rst.das` encounters a function, typedef, structure, or class that has no handmade doc file, it creates a stub file in `doc/source/stdlib/handmade/` starting with `// stub` followed by the signature. These stubs produce raw signature text in the generated docs instead of descriptions.

**After every doc regeneration**, check for remaining stubs:
```
Select-String -Path "doc\source\stdlib\handmade\*.rst" -Pattern "// stub" -SimpleMatch
```

To fix a stub:
1. Open the stub file (e.g., `function-strings_boost-capitalize-0x1747f4e995e14ba9.rst`)
2. The second line contains the function signature — use it to locate the source
3. Replace the **entire file content** with a plain-text description (1–2 sentences, no RST directives). For bitfield typedefs, use one line per flag (first line = type description, subsequent = per-flag descriptions)
4. Regenerate: `bin/Release/daslang.exe doc/reflections/das2rst.das`
5. Verify: `Select-String -Path "doc\source\stdlib\*.rst" -Pattern "// stub"` should return 0 matches

### Handmade files are for C++ builtin modules ONLY — daslang modules use `//!`

`doc/source/stdlib/handmade/*.rst` per-symbol files are **only** for
C++ builtin modules (e.g. `audio`, `strings`, `math`, `stbimage`,
`raster` — anything declared on the C++ side with no daslang source to
read). For pure-daslang modules (anything under `daslib/*.das` or
`modules/*/*.das`), per-symbol documentation comes from `//!` comments
in the `.das` source.

**If `das2rst` emits a `// stub` for a symbol in a daslang module, the
fix is to add a `//!` comment in the `.das` source** — not to fill the
handmade stub. Then regenerate; the stub disappears.

The only handmade artifact allowed for a daslang module is a trivial
one-line `module-<name>.rst` header (e.g. `module-strudel_midi.rst`
contains just `Module strudel_midi`). The real module description goes
in the `//!` comment at the top of the `.das` file.

**Verification for daslang modules:** after regenerate, both must hold:

```
grep -rl "// stub" doc/source/stdlib/handmade/ | grep <module>   # must be empty
```

When planning doc work for a daslang module, do **not** include "fill
handmade stubs" as a work step — fill the `//!` source comments
instead.

C++ modules (e.g. dasAudio's ~200 `function-audio-*.rst` files) are
the normal case for handmade content; do not try to convert those to
the daslang flow.

## RST Editing Conventions

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
   cd d:/Work/daScript/doc
   rm -rf sphinx-build ../site/doc
   d:/Work/daScript/.venv/Scripts/sphinx-build.exe -b html -d sphinx-build source ../site/doc 2>&1 | sed 's/\x1b\[[0-9;]*m//g' | tee /tmp/sphinx_out.txt
   ```
   Sphinx is installed in the project venv (`d:/Work/daScript/.venv/Scripts/sphinx-build.exe`).

3. **Verify no new errors or warnings**: Strip ANSI codes and check the build output:
   ```
   grep -iE "warning:|error:" /tmp/sphinx_out.txt
   ```
   The build must introduce **no new** Sphinx errors or warnings. The final summary line should say `build succeeded.` with no warning count.

   Common issues:
   - **Duplicate label**: Two RST files define the same `.. _label:` — rename one
   - **Unknown target**: `:ref:\`label\`` points to a nonexistent label — check spelling
   - **Malformed table**: Grid/simple table column widths don't align — see RST table rules below
   - **Unexpected indentation**: Content after a directive must be indented consistently

**When to run the workflow:**
- New or modified RST files (language docs, tutorials, stdlib docs)
- New or modified `//!` doc-comments in `daslib/*.das` files
- Changes to `doc/reflections/das2rst.das` or `doc/reflections/rst.das`
- New public functions added to any module (C++ builtins or `daslib/*.das`) — also update `group_by_regex` in `das2rst.das`

**Full checklist when adding new public functions:**
1. Add the function name to the correct `group_by_regex` in `das2rst.das` (prevents "Uncategorized")
2. `bin/Release/daslang.exe doc/reflections/das2rst.das` — generates RST and creates stub doc files
3. Check for stubs: `grep -rl "// stub" doc/source/stdlib/handmade/` — fill in descriptions
4. Regenerate: `bin/Release/daslang.exe doc/reflections/das2rst.das` (picks up filled stubs)
5. Verify no "Uncategorized": `grep -c Uncategorized doc/source/stdlib/generated/*.rst | grep -v ':0$'`
6. Clean Sphinx build (step 2 above) — verify `build succeeded.` with no new warnings

**Documenting external modules (under `modules/`):**
- C++ modules (e.g. `stbimage`, `raster`): use `get_module("name")` in `das2rst.das` — documented via handmade files only
- daScript modules (e.g. `stbimage_boost`): use `find_module("name")` in `das2rst.das` — documented via `//!` comments + handmade files
- Add `require` for the module in `das2rst.das` (e.g. `require stbimage/stbimage_boost`) to load it
- The `.das_module` descriptor transitively loads C++ modules, so one `require` may suffice for multiple modules

## Tutorial RST conventions

Tutorial RST files live in `doc/source/reference/tutorials/` with companion `.das` files in `tutorials/language/` (language tutorials), `tutorials/dasStbImage/` (image tutorials), `tutorials/dasHV/` (HTTP tutorials), `tutorials/dasPUGIXML/` (XML tutorials), `tutorials/macros/` (macro tutorials), or `tutorials/integration/cpp/` (C++ integration tutorials).

- Each RST starts with a label: `.. _tutorial_name:` (e.g., `.. _tutorial_linq:`)
- Include `.. index::` directive with relevant `single: Tutorial; Topic` entries
- Code blocks use `.. code-block:: das` with gen2 syntax
- End each RST with a `.. seealso::` block containing:
  - Full source as `:download:` link: `Full source: :download:\`tutorials/language/XX_name.das <../../../../tutorials/language/XX_name.das>\``
  - Next tutorial link (except last): `Next tutorial: :ref:\`tutorial_next_name\``
  - Related language reference links via `:ref:`
- Toctree is in `doc/source/reference/tutorials.rst` — add new tutorials there

## C++ integration tutorial RST conventions

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

## Macro tutorial RST conventions

Macro tutorial RST files live in `doc/source/reference/tutorials/macros/` with `.das` files in `tutorials/macros/`.

- Each macro tutorial has **two** source files: a module file (`<topic>_mod.das`) with definitions and a usage file (`NN_<topic>.das`) that requires the module
- Module files do NOT have numeric prefixes (so `require` resolution works); only usage files are numbered
- Label pattern: `.. _tutorial_macro_<topic>:` (e.g., `.. _tutorial_macro_call_macro:`)
- Index entries: `single: Tutorial; Macros; <Topic>`
- Code blocks: `.. code-block:: das` with gen2 syntax
- End with `.. seealso::` containing:
  - `:download:` links for **both** module and usage source files
  - Next tutorial link (except last)
- Toctree is the "Macro Tutorials" section (label `tutorials_macros`) in `doc/source/reference/tutorials.rst`

## Tutorial development workflow

When creating or modifying tutorials:

1. **Write a throwaway test file** (e.g., `test_<topic>.das`) during development to validate the tutorial code works correctly
2. **Run the test file** with `dastest` to confirm all behavior — always check `$LASTEXITCODE` after running `daslang.exe` (a crash may produce no output)
3. **Do NOT stage or commit the test file** — tutorials are self-demonstrating (run them, read the output); they don't need a permanent test suite
4. **Delete the test file** once the tutorial is confirmed working
5. **Stage only** the tutorial source files (`.das`), RST documentation, and toctree/seealso updates

Test files for tutorials are development scaffolding, not deliverables.

## Cross-reference labels

- Tutorial labels for cross-references: `tutorial_hello_world`, `tutorial_variables`, `tutorial_operators`, `tutorial_control_flow`, `tutorial_functions`, `tutorial_arrays`, `tutorial_strings`, `tutorial_structs`, `tutorial_enumerations`, `tutorial_tables`, `tutorial_tuples_and_variants`, `tutorial_function_pointers`, `tutorial_blocks`, `tutorial_lambdas`, `tutorial_iterators_and_generators`, `tutorial_modules`, `tutorial_move_copy_clone`, `tutorial_classes`, `tutorial_generics`, `tutorial_lifetime`, `tutorial_error_handling`, `tutorial_unsafe`, `tutorial_string_format`, `tutorial_pattern_matching`, `tutorial_annotations`, `tutorial_contracts`, `tutorial_testing`, `tutorial_linq`, `tutorial_functional`, `tutorial_json`, `tutorial_regex`, `tutorial_operator_overloading`, `tutorial_pointers`, `tutorial_utility_patterns`, `tutorial_random`, `tutorial_dynamic_type_checking`, `tutorial_coroutines`, `tutorial_serialization`, `tutorial_testing_tools`, `tutorial_interfaces`, `tutorial_compile_and_run`, `tutorial_debug_agents`, `tutorial_apply_in_context`, `tutorial_async`, `tutorial_soa`
- C++ integration tutorial labels: `tutorial_integration_cpp_hello_world`, `tutorial_integration_cpp_calling_functions`, `tutorial_integration_cpp_binding_functions`, `tutorial_integration_cpp_binding_types`, `tutorial_integration_cpp_binding_enums`, `tutorial_integration_cpp_interop`, `tutorial_integration_cpp_callbacks`, `tutorial_integration_cpp_methods`, `tutorial_integration_cpp_operators_and_properties`, `tutorial_integration_cpp_threading`
- C integration tutorial labels: `tutorial_integration_c_hello_world`, `tutorial_integration_c_calling_functions`, `tutorial_integration_c_binding_types`, `tutorial_integration_c_callbacks`, `tutorial_integration_c_unaligned_advanced`, `tutorial_integration_c_sandbox`, `tutorial_integration_c_context_variables`, `tutorial_integration_c_serialization`, `tutorial_integration_c_aot`, `tutorial_integration_c_threading`, `tutorial_integration_c_type_introspection`, `tutorial_integration_c_ecs`, `tutorial_integration_c_shared_module`
- C++ integration tutorial plan (remaining): 10 Custom Modules, 11 Context Variables, 12 Smart Pointers & GC, 13 AOT, 14 Serialization, 15 Custom Annotations, 16 Sandbox
- Macro tutorial labels: `tutorial_macro_call_macro`, `tutorial_macro_when_expression`, `tutorial_macro_tag_function_macro`, `tutorial_macro_reader_macro`, `tutorial_macro_variant_macro`, `tutorial_macro_structure_macro`
