# Code Formatting (REQUIRED)

After creating or modifying any `.das` file that is part of the project (daslib modules, tutorials, tests, etc.), run the source formatter on it. Do NOT format temporary/scratch files that will be deleted.

**Formatter:** MCP `format_file` tool (calls `daslib/das_source_formatter` directly). By default it ALSO strips comments file-wide — kept set: leading header block, `//!` docs, `//fmt:` directives, `nolint` suppressions; fail-closed (a strip must pass compile_check or the file is restored). For tutorials and examples, where prose carries the lesson, pass `keep_comments='true'`.

**CLI fallback (when the MCP server is unavailable):** the formatter and linter also run as in-tree daslang scripts:

- Format: `bin/daslang utils/das-fmt/dasfmt.das -- --path <dirOrFile>` — formats in place; add `--verify` for a dry-run check (changes nothing, fails on any unformatted file — same as CI).
- Lint: `bin/daslang utils/lint/main.das -- <dirOrFile>` — STYLE/PERF/LINT rules; `0 issue(s), 0 error(s)` is clean.

For a module under `modules/` whose files `require` sibling modules (e.g. `require openai/openai_chat`), pass `-load_module <moduleDir>` before `--` so cross-module requires resolve. The formatter only parses, so it works regardless; lint reports `SKIP … missing prerequisite` for files it can't fully resolve (e.g. examples/tests before the module is registered/installed).

> **Not the converter:** the installed `bin/gen1_to_gen2` binary (CMake target from `utils/gen1-to-gen2/main.cpp`) is the **v1→v2 syntax converter** (flags `-i` / `--tests` / `--semicolon`), not a code formatter. The formatter is the daslang script `utils/das-fmt/dasfmt.das` (flags `--path` / `--verify`) — invoke it through `bin/daslang utils/das-fmt/dasfmt.das -- ...` or the MCP `format_file` tool.

**What the formatter does and does not do.** It normalizes spacing inside expressions (`if( a>0 )` → `if (a > 0)`, `print( x )` → `print(x)`). It does **not** re-indent: a misindented file is rewritten with its indentation untouched and still passes `--verify`, so it also passes the CI format gate. Indent width is taken from the file itself — inferred from the first indented line, or pinned by a file-level `options indenting = N` (clamped 1..8). Getting indentation right is on you; neither the formatter nor CI will catch it.

**CI check (repo-only):** The `extended_checks` job builds `das-fmt` from the in-tree `utils/das-fmt/dasfmt.das` and runs the formatter over the whole tree twice — interpreted (`daslang utils/das-fmt/dasfmt.das -- --path ./ --verify`) and compiled (`das-fmt.exe --path ./ --verify`). Both call `daslib/das_source_formatter` — the same engine as the MCP `format_file` tool. If CI reports `[E] Unformatted file`, the file was not formatted.

**Procedure:**

1. **Back up** the file before formatting: copy it to `<filename>.das.bak`
2. **Run the formatter:** use the MCP `format_file` tool with the file path
3. **Verify** the formatted file still compiles: use the MCP `compile_check` tool — it handles test files and module files (no `main` required) directly. CLI equivalents: `bin/daslang -compile-only path/to/file.das`, or `bin/daslang dastest/dastest.das -- --compile-only --test path/to/test.das` for a test file (`--compile-only` compiles without executing).
4. **Remove the backup** if formatting succeeded: delete `<filename>.das.bak`
5. **Restore from backup** if formatting broke the file: copy `.das.bak` back over the `.das` file, delete the backup, and report the issue

**When to format:**
- New `.das` files: tutorials, tests, daslib modules, utilities
- Modified `.das` files: after any edits to existing project files

**When NOT to format:**
- Temporary/scratch files that will be deleted immediately
- Files you are only reading, not modifying
- C++ source files, RST docs, Python scripts, etc. (only `.das` files)
