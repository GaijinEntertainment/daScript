# Code Formatting (REQUIRED)

After creating or modifying any `.das` file that is part of the project (daslib modules, tutorials, tests, etc.), run the source formatter on it. Do NOT format temporary/scratch files that will be deleted.

**Formatter:** MCP `format_file` tool (calls `daslib/das_source_formatter` directly)

**CLI fallback (when the MCP server is unavailable):** the formatter and linter also run as in-tree daslang scripts:

- Format: `bin/Release/daslang.exe utils/das-fmt/dasfmt.das -- --path <dirOrFile>` — formats in place; add `--verify` for a dry-run check (changes nothing, fails on any unformatted file — same as CI).
- Lint: `bin/Release/daslang.exe utils/lint/main.das -- <dirOrFile>` — STYLE/PERF/LINT rules; `0 issue(s), 0 error(s)` is clean.

For a module under `modules/` whose files `require` sibling modules (e.g. `require openai/openai_chat`), pass `-load_module <moduleDir>` before `--` so cross-module requires resolve. The formatter only parses, so it works regardless; lint reports `SKIP … missing prerequisite` for files it can't fully resolve (e.g. examples/tests before the module is registered/installed).

> **WARNING:** `utils/das-fmt/` (script `dasfmt.das`) is the formatter. Do NOT confuse it with `utils/dasFormatter/` — a *different* directory holding the **v1→v2 syntax converter**, which is not a code formatter.

**CI check:** The `extended_checks` job builds `das-fmt` from the in-tree `utils/das-fmt/dasfmt.das` and runs the formatter over the whole tree twice — interpreted (`daslang utils/das-fmt/dasfmt.das -- --path ./ --verify`) and compiled (`das-fmt.exe --path ./ --verify`). Both call `daslib/das_source_formatter` — the same engine as the MCP `format_file` tool. If CI reports `[E] Unformatted file`, the file was not formatted.

**Procedure:**

1. **Back up** the file before formatting: copy it to `<filename>.das.bak`
2. **Run the formatter:** use the MCP `format_file` tool with the file path
3. **Verify** the formatted file still compiles: use the MCP `compile_check` tool
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
