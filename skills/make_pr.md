# Pre-PR Checklist

Before creating a pull request, complete ALL of the following steps in order. Do not skip steps. If any step fails, fix the issue before proceeding.

## 1. Lint all changed `.das` files

Find all `.das` files changed relative to `master`:
```bash
git diff --name-only master -- '*.das'
```

Run the MCP `lint` tool on each changed file (or batch them comma-separated). Fix significant issues (unused variables that indicate bugs, performance warnings in hot paths). Minor lint warnings in unchanged code can be ignored unless trivial to fix.

## 2. Run all tests

```bash
bin/Release/daslang.exe dastest/dastest.das -- --test tests/
```

Or use the MCP `run_test` tool with the `tests/` directory.

**If tests fail:**
- If the failure is in code you changed — fix it
- If the failure is pre-existing (exists on master too) and the fix is obvious — fix it
- If the failure is pre-existing and non-obvious — **ask the user**, do not ignore it
- Tests MUST pass for the PR to merge

## 3. Build and run AOT tests

**IMPORTANT:** Kill the MCP server and any running daslang processes first — they lock build output files.

```bash
# Kill processes that lock build files
taskkill /F /IM daslang.exe 2>/dev/null
taskkill /F /IM mcp.exe 2>/dev/null

# Build test_aot
cmake --build build --config Release --target test_aot

# Run AOT tests
bin/Release/test_aot.exe dastest/dastest.das -- --test tests/aot -use-aot
```

Use `timeout: 0` (no timeout) for the cmake build — it can take 2-25 minutes.

**If test_aot doesn't build:**
- Check if new test directories need registering in `tests/aot/CMakeLists.txt` (see `skills/aot_testing.md`)
- Check for AOT hash mismatches (`error[50101]`) — compare hash comments in generated `.cpp` with runtime output
- Fix the build before proceeding

**If AOT tests fail:**
- Same rules as regular tests — fix your changes, fix obvious pre-existing, ask about non-obvious

## 4. Update documentation (if applicable)

**Skip this step** if the PR only changes examples, tests, or non-public code.

**Run this step if ANY of the following changed:**
- Public functions in `daslib/*.das` (added, removed, renamed, or signature changed)
- `//!` doc-comments in `daslib/*.das` files
- C++ bindings in `modules/*/src/*.cpp` that add new public functions or types
- RST files in `doc/source/`
- `doc/reflections/das2rst.das` or `doc/reflections/rst.das`

**Steps:**

1. **Add new functions to groups** in `das2rst.das` (prevents "Uncategorized" section):
   - Find the module's `document_module_*` function in `doc/reflections/das2rst.das`
   - Add the function name to the appropriate `group_by_regex` call

2. **Regenerate stdlib docs:**
   ```bash
   bin/Release/daslang.exe doc/reflections/das2rst.das
   ```

3. **Check for stubs** — fill in descriptions for any new stub files:
   ```bash
   grep -rl "// stub" doc/source/stdlib/handmade/
   ```
   Replace stub content with 1-2 sentence descriptions.

4. **Regenerate again** (picks up filled stubs):
   ```bash
   bin/Release/daslang.exe doc/reflections/das2rst.das
   ```

5. **Verify no "Uncategorized":**
   ```bash
   grep -c Uncategorized doc/source/stdlib/generated/*.rst | grep -v ':0$'
   ```

6. **Clean Sphinx build** — MUST delete cache (cached builds hide errors):
   ```bash
   cd doc
   rm -rf sphinx-build ../site/doc
   sphinx-build -b html -d sphinx-build source ../site/doc 2>&1 | tee /tmp/sphinx_out.txt
   grep -iE "warning:|error:" /tmp/sphinx_out.txt
   ```
   Must introduce **no new** Sphinx warnings or errors.

See `skills/documentation_rst.md` for full details on doc conventions, tutorial RST, and cross-references.

## 5. Format all changed `.das` files

Run the MCP `format_file` tool on every changed `.das` file. Verify each still compiles after formatting.

Do NOT format files you didn't change — only format files that are part of the PR.

## 6. Create the PR

Stage, commit, push, and create the PR using GitHub MCP tools or `gh` CLI. Follow the commit message conventions from the repository (see recent `git log` for style).

**Squashed branches:** If the user asked to squash the branch (single commit), keep it squashed. Any subsequent fixes (lint, test failures, formatting, CI issues) must be amended into the existing commit (`git commit --amend --no-edit`) and force-pushed — do NOT create new commits on a squashed branch.

## Quick reference

| Step | Tool/Command | Fix policy |
|---|---|---|
| Lint | MCP `lint` | Fix significant issues in changed files |
| Tests | `dastest -- --test tests/` | Must pass. Fix own, fix obvious pre-existing, ask about unclear |
| AOT build | `cmake --build build --config Release --target test_aot` | Kill daslang first. Register new test dirs |
| AOT tests | `test_aot.exe dastest -- --test tests/aot -use-aot` | Same as regular tests |
| Docs | `das2rst.das` + stubs + Sphinx | Only if daslib/C++ bindings/RST changed |
| Format | MCP `format_file` on each changed `.das` | Only changed files |
| PR | GitHub MCP `create_pull_request` or `gh pr create` | — |
