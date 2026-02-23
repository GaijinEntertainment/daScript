# Code Formatting (REQUIRED)

After creating or modifying any `.das` file that is part of the project (daslib modules, tutorials, tests, etc.), run the source formatter on it. Do NOT format temporary/scratch files that will be deleted.

**Formatter tool:** `utils/dasCodeFormatter/main.das`

**Procedure:**

1. **Back up** the file before formatting: copy it to `<filename>.das.bak`
2. **Run the formatter:** `bin/Release/daslang.exe utils/dasCodeFormatter/main.das -- path/to/file.das`
3. **Verify** the formatted file still compiles: `bin/Release/daslang.exe path/to/file.das`
   - For test files (no `main`), compile-check with: `bin/Release/daslang.exe dastest/dastest.das -- --test path/to/test.das`
   - For module files (no `main`), verify by running a file that requires them
4. **Remove the backup** if formatting succeeded: delete `<filename>.das.bak`
5. **Restore from backup** if formatting broke the file: copy `.das.bak` back over the `.das` file, delete the backup, and report the issue

**Batch formatting (multiple files):**

When formatting many files at once, use the `--files:` option to avoid paying the ~450ms process startup cost per file:

1. Write a file list (one path per line, no BOM): `file_list.txt`
2. Run: `bin/Release/daslang.exe utils/dasCodeFormatter/main.das -- --files:file_list.txt`

This formats all files in a single process invocation. Example: 419 files in ~1.7s (batch) vs ~189s (one-by-one).

**Important:** the file list must be UTF-8 without BOM. In PowerShell, use `[System.IO.File]::WriteAllLines("file_list.txt", $paths)` instead of `Set-Content -Encoding UTF8` (which adds a BOM).

**When to format:**
- New `.das` files: tutorials, tests, daslib modules, utilities
- Modified `.das` files: after any edits to existing project files

**When NOT to format:**
- Temporary/scratch files that will be deleted immediately
- Files you are only reading, not modifying
- C++ source files, RST docs, Python scripts, etc. (only `.das` files)
