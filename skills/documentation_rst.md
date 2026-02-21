# Documentation Conventions

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

## Tutorial RST conventions

Tutorial RST files live in `doc/source/reference/tutorials/` with companion `.das` files in `tutorials/language/`.

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

- Tutorial labels for cross-references: `tutorial_hello_world`, `tutorial_variables`, `tutorial_operators`, `tutorial_control_flow`, `tutorial_functions`, `tutorial_arrays`, `tutorial_strings`, `tutorial_structs`, `tutorial_enumerations`, `tutorial_tables`, `tutorial_tuples_and_variants`, `tutorial_function_pointers`, `tutorial_blocks`, `tutorial_lambdas`, `tutorial_iterators_and_generators`, `tutorial_modules`, `tutorial_move_copy_clone`, `tutorial_classes`, `tutorial_generics`, `tutorial_lifetime`, `tutorial_error_handling`, `tutorial_unsafe`, `tutorial_string_format`, `tutorial_pattern_matching`, `tutorial_annotations`, `tutorial_contracts`, `tutorial_testing`, `tutorial_linq`, `tutorial_functional`, `tutorial_json`, `tutorial_regex`, `tutorial_operator_overloading`, `tutorial_pointers`, `tutorial_utility_patterns`, `tutorial_random`, `tutorial_dynamic_type_checking`, `tutorial_coroutines`, `tutorial_serialization`, `tutorial_testing_tools`, `tutorial_interfaces`, `tutorial_compile_and_run`
- C++ integration tutorial labels: `tutorial_integration_cpp_hello_world`, `tutorial_integration_cpp_calling_functions`, `tutorial_integration_cpp_binding_functions`, `tutorial_integration_cpp_binding_types`, `tutorial_integration_cpp_binding_enums`, `tutorial_integration_cpp_interop`, `tutorial_integration_cpp_callbacks`, `tutorial_integration_cpp_methods`, `tutorial_integration_cpp_operators_and_properties`
- C++ integration tutorial plan (remaining): 10 Custom Modules, 11 Context Variables, 12 Smart Pointers & GC, 13 AOT, 14 Serialization, 15 Custom Annotations, 16 Sandbox
- Macro tutorial labels: `tutorial_macro_call_macro`, `tutorial_macro_when_expression`, `tutorial_macro_tag_function_macro`, `tutorial_macro_reader_macro`, `tutorial_macro_variant_macro`, `tutorial_macro_structure_macro`
