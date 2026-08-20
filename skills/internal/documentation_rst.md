# Documentation Conventions

## Standard Library Documentation

`//!` comments in `daslib/*.das` and `modules/<Mod>/<subdir>/*.das` (e.g.
`modules/dasStbImage/stbimage/stbimage_boost.das`) → `daslib/rst_comment.das` →
`doc/source/stdlib/generated/detail/*.rst` (never edit by hand). `doc/reflections/das2rst.das` +
`daslib/rst.das` merge those with handmade `doc/source/stdlib/handmade/` into
**`doc/source/stdlib/generated/`** (NOT the `doc/source/stdlib/` root). C++ modules carry no `//!`
— handmade only.

**Regenerate** — exit code 0 = success; "regenerate" below means exactly this:

```
bin/Release/daslang.exe -documentation doc/reflections/das2rst.das
```

### Adding public functions to a module

The generated RST groups functions by `group_by_regex("Group Name", mod, %regex~(func1|func2)$%%)`
calls in the module's `document_module_*` function in `das2rst.das`. A public function no group
regex matches lands in an **"Uncategorized"** section.

1. Add the name to the right `group_by_regex` in `das2rst.das`
2. Regenerate — this also writes `// stub` files for undocumented symbols
3. Document each stub (below), regenerate again
4. `grep -c Uncategorized doc/source/stdlib/generated/*.rst | grep -v ':0$'` — must print nothing
5. Sphinx build

`group_by_regex` uses `regex_match` (`daslib/regex`), which matches **only from offset 0** — it does
not scan like `regex_search`. `(ok|parent)$` matches `"ok"` but **NOT** `` ".`ok" `` (dot-backtick
property name, as C++ modules register). **Always prefix with `.*`**: `%regex~.*(ok|parent)$%%`.

The `%regex~...%%` reader macro cannot hold a literal backtick, so names like `` `is`int `` need
`regex_compile` with a string pattern:

```das
var r_type_conv : Regex
regex_compile(r_type_conv, ".*(`is`|`as`|\\\\[\\\\])")
group_by_regex("Type conversion operators", mod, r_type_conv)
```

### Adding a module example

`doc/reflections/gen_module_examples.py` generates/updates the `module-*.rst` files with
descriptions and compilable examples. Add `example="""..."""` to the `reg()` call there; run the
snippet through `bin/Release/daslang` to confirm it compiles and runs;
`python doc/reflections/gen_module_examples.py`; regenerate.

### `// stub` files (check after EVERY regeneration)

`das2rst.das` writes a `// stub` file into `doc/source/stdlib/handmade/` for every function,
typedef, structure or class with no handmade doc; the page then shows raw signature text instead of
a description.

```
Select-String -Path "doc\source\stdlib\handmade\*.rst" -Pattern "// stub" -SimpleMatch
```

To fix one (e.g. `function-strings_boost-capitalize-0x1747f4e995e14ba9.rst`): its **second line** is
the signature — use it to locate the source; replace the **entire file content** with a plain-text
description, 1–2 sentences, no RST directives. Bitfield typedefs are positional: first line
describes the type, line N+1 describes the Nth flag. Regenerate, then
`Select-String -Path "doc\source\stdlib\*.rst" -Pattern "// stub"` must return 0 matches.

### Handmade files are for C++ builtin modules ONLY — daslang modules use `//!`

Per-symbol `handmade/*.rst` files serve only C++ builtin modules (`audio`, `strings`, `math`,
`stbimage`, `raster` — declared C++-side with no daslang source). For `daslib/*.das` and
`modules/*/*.das`, a `// stub` means **add a `//!` in the `.das` source**, not fill the stub; then
regenerate and it disappears. Verify: `grep -rl "// stub" doc/source/stdlib/handmade/ | grep <module>`
must be empty. The one handmade artifact allowed there is a trivial one-line `module-<name>.rst`
header (`module-strudel_midi.rst` is just `Module strudel_midi`); the real module description is the
`//!` at the top of the `.das`. Do not convert existing C++-module handmade content (dasAudio's ~200
`function-audio-*.rst`) to the daslang flow.

**`//!` placement is INSIDE the body, not above the `def`.** `daslib/rst_comment.das` attaches a
`//!` to a function only when it sits inside the body (first lines after `{`); `beforeFunction`
unconditionally discards a pending block above the `def` (verified against the parser, 2026-07-02).
An above-def `//!` extracts NOTHING and says nothing — bare signature on the page, no detail file.
Structs: `//!` inside the struct body, `//!<` per field. A `//!` above the `module` decl does not
reach the page either; the module header text comes from handmade `module-<name>.rst`.

**STYLE014 on an intentionally long `//!` block:** put `//!@nolint` on its first line —
`rst_comment.das` drops only that marker line, so the rest still reaches `generated/detail/`. For
`//` blocks (no doc-comment) use `// nolint:STYLE014` / `// nolint:STYLE015` on the first line —
those bypass lint without involving the doc generator.

## RST Editing Conventions

In `doc/source/reference/language/`: code blocks are `.. code-block:: das` with gen2 syntax, carry
`// output:` comments on runnable examples and any `require` the example needs; cross-page links use
`:ref:`. Verify examples compile with `bin/Release/daslang.exe example.das`.

### RST table rules

- **Grid tables** (`+---+---+`): every row line exactly as wide as every separator line; off-by-one
  spaces are Sphinx errors.
- **Simple tables** (`===  ===`): the `=` runs define column widths. Non-last-column content must
  NOT extend past its column's `=` boundary; a header must start at or after its column's start (not
  in the gap); gaps are at least 2 spaces.

### Documentation workflow (REQUIRED)

After creating or modifying RST, stdlib docs, `//!` comments in `daslib/*.das`, or
`doc/reflections/das2rst.das` / `doc/reflections/rst.das`:

1. Regenerate — if `daslib/*.das` or `das2rst.das` changed.
2. Sphinx build (`-d` keeps the doctree cache across iterations):
   ```
   sphinx-build -b html -d doc/sphinx-build doc/source site/doc 2>&1 | sed 's/\x1b\[[0-9;]*m//g' | tee /tmp/sphinx_out.txt
   ```
   Delete `doc/sphinx-build` before both builders; preflight's docs gate does this unconditionally so
   stale doctrees cannot hide warnings. `sphinx-build` comes from PATH — `pip install sphinx` if
   missing; the repo `.venv/` is not maintained.
3. `grep -iE "warning:|error:" /tmp/sphinx_out.txt` — **no new** errors or warnings; the summary
   line must read `build succeeded.` with no warning count.

Message → cause: `duplicate label` = two files define the same `.. _label:` (rename one);
`unknown target` = `:ref:` spelling; malformed table = column misalignment; unexpected indentation =
inconsistent indent under a directive.

**External modules (under `modules/`)** in `das2rst.das`: C++ modules (`stbimage`, `raster`) use
`get_module("name")` and are handmade-only; daScript modules (`stbimage_boost`) use
`find_module("name")` and combine `//!` with handmade. Add a `require` to load the module (e.g.
`require stbimage/stbimage_boost`); the `.das_module` descriptor transitively loads C++ modules, so
one `require` may suffice for several.

## Tutorial RST conventions

Source-file conventions (location, naming, header shape, install hooks): **read
`skills/internal/tutorials.md` first** — the recurring mistake it prevents is overwriting the
inherited examples under `modules/<X>/tutorial/`.

Areas: `language`, `macros`, `integration/c`, `integration/cpp`, `sql`, `dasAudio`, `dasHV`,
`dasMinfft`, `dasOPENAI`, `dasPEG`, `dasPUGIXML`, `dasStbImage`, `daStrudel`.

| | RST lives in | sources | label | `.. index:: single:` | `:download:` links |
|---|---|---|---|---|---|
| general | `doc/source/reference/tutorials/` | `tutorials/<area>/` | `.. _tutorial_<name>:` | `Tutorial; <Topic>` | the `.das` |
| C++ integration | `doc/source/reference/tutorials/` | `tutorials/integration/cpp/` | `.. _tutorial_integration_cpp_<topic>:` | `Tutorial; C++ Integration; <Topic>` | `.cpp` + `.das` |
| macros | `doc/source/reference/tutorials/macros/` | `tutorials/macros/` | `.. _tutorial_macro_<topic>:` | `Tutorial; Macros; <Topic>` | module + usage `.das` |

- Every RST ends with `.. seealso::` carrying those downloads —
  ``Full source: :download:`tutorials/language/XX_name.das <../../../../tutorials/language/XX_name.das>` `` —
  plus ``Next tutorial: :ref:`tutorial_next_name` `` (except the last) and related `:ref:` links
- Toctree: `doc/source/reference/tutorials.rst`; macro tutorials go under its "Macro Tutorials"
  section (label `tutorials_macros`)
- Macro tutorials are two files: `<topic>_mod.das` (definitions, unnumbered so `require` resolves)
  plus `NN_<topic>.das` (usage)
- C++ integration tutorials are one self-contained `.cpp` with embedded `main()` — no build
  infrastructure beyond the CMake targets `integration_cpp_01`…`integration_cpp_NN` in
  `tutorials/integration/cpp/CMakeLists.txt`; the RST adds a Build & run section with the
  `cmake --build` command and expected output. C++ code blocks are `.. code-block:: cpp`

### Tutorial development workflow

Validate with a throwaway `test_<topic>.das` run under `dastest` — always check `$LASTEXITCODE`, a
crash may produce no output — then delete it. Stage only tutorial `.das`, RST, and toctree/seealso
edits: tutorials are self-demonstrating and get no permanent test.
