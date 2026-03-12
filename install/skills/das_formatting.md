# Code Formatting (REQUIRED)

After creating or modifying any `.das` file, run the source formatter on it. Do NOT format temporary/scratch files that will be deleted.

**Formatter:** MCP `format_file` tool (calls `daslib/das_source_formatter` directly). If MCP is not available, use [profelis/das-fmt](https://github.com/profelis/das-fmt) CLI.

**Procedure:**

1. **Back up** the file before formatting: copy it to `<filename>.das.bak`
2. **Run the formatter:** use the MCP `format_file` tool with the file path
3. **Verify** the formatted file still compiles: `bin/daslang path/to/file.das`
   - For module files (no `main`), verify by running a file that requires them
4. **Remove the backup** if formatting succeeded: delete `<filename>.das.bak`
5. **Restore from backup** if formatting broke the file: copy `.das.bak` back over the `.das` file, delete the backup, and report the issue

**When to format:**
- New `.das` files you create
- Modified `.das` files after edits

**When NOT to format:**
- Temporary/scratch files that will be deleted immediately
- Files you are only reading, not modifying
- Non-`.das` files (C++, RST, Python, etc.)
