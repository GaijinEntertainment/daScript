# Tutorial Source Files (REQUIRED)

The companion skill is `skills/documentation_rst.md` for the RST side. **This file owns the `.das` source side**: where it lives, how it's named, what its header looks like, and the install hook.

## Where tutorials live

**Root `/tutorials/<area>/` only.** Never inside `modules/<X>/tutorial/`.

| Path | Purpose |
|---|---|
| `tutorials/<area>/*.das` | The canonical, project-wide tutorial home. New tutorials go here. |
| `modules/<X>/tutorial/*.das` | **Inherited / upstream examples** (e.g. `modules/dasSQLITE/tutorial/` is the zetcode C-API port). Reference material — leave them alone. |
| `modules/<X>/tutorial-mockup/*.mockup` | Pre-implementation design artifacts (un-compilable). Reference material — leave them alone. |

If you find yourself editing or replacing a file under `modules/<X>/tutorial/`, **stop**. Create the new tutorial under `tutorials/<area>/` instead and leave the inherited file as-is — those files are the cross-reference for the API rewrite, and overwriting them loses provenance.

Existing areas (one per "topic", roughly one per non-language module):

```
tutorials/language/        — language tutorials (NN_name.das)
tutorials/macros/          — macro tutorials (NN_name.das + name_mod.das)
tutorials/integration/c/   — C integration tutorials
tutorials/integration/cpp/ — C++ integration tutorials
tutorials/sql/             — dasSQLITE (NN-name.das, hyphenated)
tutorials/dasAudio/        — dasAudio
tutorials/dasHV/           — dasHV (HTTP/WebSocket)
tutorials/dasPEG/          — dasPEG (parser)
tutorials/dasPUGIXML/      — dasPUGIXML (XML)
tutorials/dasStbImage/     — dasStbImage
tutorials/daStrudel/       — daStrudel (live-coding)
```

When adding tutorials for a **new** module, create a new `tutorials/<module>/` directory rather than dumping into an existing one.

## Naming convention

Match the area you're adding to. The conventions in use:

| Area | Pattern | Example |
|---|---|---|
| `language/`, `macros/`, `dasAudio/`, `dasHV/`, `dasPEG/`, `dasPUGIXML/`, `dasStbImage/` | `NN_topic.das` (underscore) | `01_hello_sound.das` |
| `sql/` | `NN-topic.das` (hyphen) | `01-version.das` |
| `daStrudel/` | `daStrudel_NN_topic.das` (full prefix) | `daStrudel_01_hello_pattern.das` |

Use **two-digit zero-padded** numbers (`01`, not `1`). Numbering is contiguous within an area and follows teaching order (each tutorial assumes the prior tutorials).

## File header (REQUIRED shape)

Open with `options gen2` and a header comment block:

```das
options gen2
options persistent_heap   // only if needed (audio, async, GC-pressure paths)

require <module>/<boost>
require daslib/<helper>   // as needed

// Tutorial AREA-NN: One-line title
//
// This tutorial covers:
//   - Bullet 1
//   - Bullet 2
//
// Prerequisites: ... (optional, when needed)
//
// Run: daslang.exe tutorials/<area>/NN_name.das
```

**`require` rules:**

- For dasSQLITE the canonical user-facing pair is `require daslib/sql` + `require sqlite/sqlite_boost`. Add `require sqlite/sqlite_linq` only when the tutorial uses the `_sql(...)` macro. `daslib/sql` is the abstract layer; never re-export sqlite-specific symbols through it.
- For other modules, follow the require shape used by the area's existing tutorials.

**`options`:** keep to the minimum the tutorial actually needs. `gen2` is mandatory. `persistent_heap` for audio / strudel / async / heavy-allocation paths. `indenting = 4` is fine but not required (the formatter normalizes).

## File structure

Each tutorial is **one self-contained `[export] def main()` script** that runs end-to-end with `daslang.exe path/to/tutorial.das`. Inside `main()`:

- Use section-banner comments to chunk the body:
  ```das
  // ──────────────────────────────────────────────────────────────────────────
  // Section 1 — One-line section title
  // ──────────────────────────────────────────────────────────────────────────
  //
  // 2-3 lines of prose explaining what this section demonstrates.
  ```
- Print to `print(...)` or `to_log(LOG_INFO, ...)` so the reader can follow what's happening when they run it. Tutorials in `tests/`-adjacent areas (`daslib`) prefer `to_log`; tutorials proper can use `print` freely.
- Keep examples runnable in isolation. Avoid global state; use local `let`/`var` bindings.
- Inline comments explain the *why* (which API surface this exercises, why the choice matters), not the *what* (which the code already shows).

## Macro tutorials are split

Anything under `tutorials/macros/` follows a special two-file pattern: numbered usage file (`NN_topic.das`) requires a non-numbered module file (`topic_mod.das`). Module files have no number prefix so `require` resolution works. See `skills/documentation_rst.md` § "Macro tutorial RST conventions" for the doc side.

## RST companion (REQUIRED)

Every shipped tutorial has a paired RST page under `doc/source/reference/tutorials/`. The full conventions (label pattern, `.. seealso::` block, toctree placement) live in `skills/documentation_rst.md` § "Tutorial RST conventions". The short version:

1. Create `doc/source/reference/tutorials/<area>_NN_<topic>.rst` (use a sibling tutorial's RST as the template — same pattern as `sql_03_last_row_id.rst` or `01_hello_world.rst`).
2. Add the new entry to the matching toctree section in `doc/source/reference/tutorials.rst`.
3. Run the Sphinx workflow from `skills/documentation_rst.md` to verify zero new warnings.

## CMake install hook

Tutorials are installed to `${DAS_INSTALL_TUTORIALSDIR}/<area>/` via per-area `file(GLOB ...) + install(FILES ...)` blocks in the root `CMakeLists.txt` (search for `_TUTORIAL_SOURCES`). When adding a tutorial to an **existing** registered area (`language`, `macros`, `dasHV`, `dasPUGIXML`, `dasStbImage`, `dasAudio`), the glob picks it up automatically — no CMake edit needed.

When adding a tutorial to an **unregistered** area, add the install rule. Currently missing as of writing: `tutorials/sql/`, `tutorials/dasPEG/`, `tutorials/daStrudel/`. Pattern:

```cmake
file(GLOB SQL_TUTORIAL_SOURCES
    ${PROJECT_SOURCE_DIR}/tutorials/sql/*.das
)
install(FILES ${SQL_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/sql
)
```

If a tutorial pulls in non-`.das` data files (like `tutorials/dasPUGIXML/books.xml`), add the matching glob pattern (`*.xml`, `*.json`, etc.).

## Development workflow

From `skills/documentation_rst.md` § "Tutorial development workflow" — repeated here because it's load-bearing:

1. **Throwaway test file** during development to validate behavior — `dastest --test test_<topic>.das`. Always check `$LASTEXITCODE`.
2. **Do NOT stage or commit the test file.** Tutorials are self-demonstrating — running the tutorial file itself is the test.
3. **Format** every `.das` tutorial with the MCP `format_file` tool before staging (`skills/das_formatting.md`).
4. **Run the tutorial end-to-end** as the final check: `bin/Release/daslang.exe tutorials/<area>/NN_name.das` and inspect output. Crashes can produce no output — always check exit code.
5. Stage only: tutorial `.das`, RST page, toctree update, CMake install rule (if first tutorial in a new area).
