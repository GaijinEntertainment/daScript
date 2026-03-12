# Code Formatting (REQUIRED)

After creating or modifying any `.das` file that is part of the project (daslib modules, tutorials, tests, etc.), run the source formatter on it. Do NOT format temporary/scratch files that will be deleted.

**Formatter:** MCP `format_file` tool (calls `daslib/das_source_formatter` directly)

> **WARNING:** Do NOT confuse formatting with `utils/dasFormatter/` — that is a **v1→v2 syntax converter** (`das-fmt`), not a code formatter.

**CI check:** The GitHub Actions CI workflow clones [profelis/das-fmt](https://github.com/profelis/das-fmt) and runs `dasfmt.das --path ./ --verify`, which internally calls `daslib/das_source_formatter` — the same formatter engine used by the MCP `format_file` tool. If CI reports `[E] Unformatted file`, it means the file was not formatted.

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
