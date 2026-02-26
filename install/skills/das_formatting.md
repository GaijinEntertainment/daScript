# Code Formatting (REQUIRED)

After creating or modifying any `.das` file, run the source formatter on it. Do NOT format temporary/scratch files that will be deleted.

**Formatter tool:** `utils/dasCodeFormatter/main.das` (included in the SDK)

**Procedure:**

1. **Back up** the file before formatting: copy it to `<filename>.das.bak`
2. **Run the formatter:** `bin/daslang utils/dasCodeFormatter/main.das -- path/to/file.das`
3. **Verify** the formatted file still compiles: `bin/daslang path/to/file.das`
   - For module files (no `main`), verify by running a file that requires them
4. **Remove the backup** if formatting succeeded: delete `<filename>.das.bak`
5. **Restore from backup** if formatting broke the file: copy `.das.bak` back over the `.das` file, delete the backup, and report the issue

**Batch formatting (multiple files):**

When formatting many files at once, use the `--files:` option to avoid paying the ~450ms process startup cost per file:

1. Write a file list (one path per line, no BOM): `file_list.txt`
2. Run: `bin/daslang utils/dasCodeFormatter/main.das -- --files:file_list.txt`

This formats all files in a single process invocation.

**Important:** the file list must be UTF-8 without BOM. In PowerShell, use `[System.IO.File]::WriteAllLines("file_list.txt", $paths)` instead of `Set-Content -Encoding UTF8` (which adds a BOM).

**When to format:**
- New `.das` files you create
- Modified `.das` files after edits

**When NOT to format:**
- Temporary/scratch files that will be deleted immediately
- Files you are only reading, not modifying
- Non-`.das` files (C++, RST, Python, etc.)
