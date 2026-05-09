# Pre-PR Checklist

Before creating a pull request, complete ALL of the following steps in order. Do not skip steps. If any step fails, fix the issue before proceeding.

## 0. Sync with origin/master and rebase the branch

**Always do this first.** If you skip it, a stale local `master` will cause your squashed commit to absorb other already-merged PRs as if they were branch-original work — the PR ends up touching files it has no business touching.

```bash
git fetch origin master
git diff --name-only master..origin/master | head -5    # quick sanity: is local behind?
git rebase origin/master                                # bring branch on top of latest origin/master
git diff --name-only origin/master..HEAD | head         # confirm only your files remain
```

After the rebase, every file in `git diff --name-only origin/master..HEAD` should be one you actually edited. If you see `daslib/option.das`, `daslib/result.das`, or anything else from a recently merged PR, your local `master` was behind — re-rebase or `git reset --hard` the branch and start over.

**Why this matters:** `git reset --soft master` (used to squash N commits into one) collapses everything HEAD-back-to-master into the index. If local `master` is behind origin's by even one merge, that merge's content gets baked into your "single squashed commit" — invisibly. The PR diff against origin/master then shows the leak. Always rebase onto **origin/master** (not local `master`) before squashing.

If a rebase produces conflicts on files that were independently changed on origin/master, resolve them by keeping origin/master's version (your branch's "modification" was an outdated copy of the same change) — verify with `git show origin/master:<path>` that the merged version subsumes yours.

## 1. Lint all changed `.das` files

Run the unified lint utility on changed directories:
```bash
bin/Release/daslang.exe utils/lint/main.das -- daslib/ modules/ tutorials/
```

Or lint only the files changed relative to `master`:
```bash
git diff --name-only master -- '*.das' | xargs bin/Release/daslang.exe utils/lint/main.das --
```

Fix significant issues (unused variables that indicate bugs, performance warnings in hot paths). Use `// nolint:CODE` comments to suppress false positives (LINT001-004, PERF001-011, STYLE001-010). Minor lint warnings in unchanged code can be ignored unless trivial to fix.

## 1.5. Check for duplicates against the corpus

Run `detect-dupe` against the **changed `.das` files only**, with a per-PR corpus scoped to the **directories the PR actually touches** (plus their sibling test/example/tutorial dirs). This is the "did I just write something that already exists?" check — catches structural copy-paste before review.

**Don't run a full-tree corpus.** Exporting all of `daslib`, `tests`, `utils`, `modules` is slow and the noise floor swamps real signal — the dupes that matter are the ones near the code you changed (sibling functions in the same module, parallel tests, copy-pasted tutorial scaffolds). Build a narrow corpus.

**How to scope:**

```bash
# 1. List the unique top-level dirs touched by the PR
git diff --name-only origin/master..HEAD -- '*.das' | awk -F/ 'NF>=2 {print $1"/"$2}' | sort -u

# Example output for a strudel-only PR:
#   examples/daStrudel
#   modules/dasAudio
#   tests/strudel
#   tutorials/daStrudel
```

Use those dirs (or trim further to a single module subfolder, e.g. `modules/dasAudio/strudel`) as the corpus scope.

```bash
# Per-PR: corpus over the touched scope only
bin/Release/daslang.exe utils/detect-dupe/main.das -- \
  -p modules/dasAudio/strudel -p tests/strudel -p tutorials/daStrudel -p examples/daStrudel \
  --export-functions /tmp/corpus.json

# Compare the diff against it
git diff --name-only origin/master..HEAD -- '*.das' | \
  bin/Release/daslang.exe utils/detect-dupe/main.das -- \
    --import-functions /tmp/corpus.json --against-from-stdin
```

Or via MCP (preferred when available):

```
mcp__daslang__export_corpus(paths="modules/dasAudio/strudel,tests/strudel,tutorials/daStrudel,examples/daStrudel", out="/tmp/corpus.json")
mcp__daslang__detect_duplicates(paths="<git-diff list>", corpus="/tmp/corpus.json")
```

The candidate files are auto-stripped from the corpus before comparison, so it's fine for the corpus scope to overlap the changed-files list.

**When to widen the scope** — if the PR introduces a generic helper (`pattern_*`, `time_*`, etc.), add the obvious adjacent directory (`daslib`) to catch dupes against the standard library. But still avoid a tree-wide sweep.

**Triage policy:**
- **Exact match (similarity 1.0)** with an existing function — almost always a real concern. Either (a) reuse the existing one, (b) document why a sibling is needed, or (c) extract a shared helper.
- **Fuzzy ≥0.85** — eyeball both. Often a refactor target (siblings that should share a private helper).
- **Fuzzy 0.7–0.85** — usually noise (tutorial-shape similarity, "open DB → query → print" patterns). Scan but don't treat as blocking.
- **Pattern-skipped** (`visitor`, `dispatch`, `emit`) — already filtered; no action.

Skip this step for PRs that only touch tests, fixtures, or generated files.

See `skills/detect_dupe.md` for the full workflow including B1 baseline / CI gate modes.

## 2. Run all tests

```bash
bin/Release/daslang.exe dastest/dastest.das -- --test tests/
```

Or use the MCP `run_test` tool with the `tests/` directory.

**If tests fail or error:**
- Both **failures** (assertion errors) and **errors** (compilation errors) count — check both numbers in the summary
- If the failure/error is in code you changed — fix it
- If the failure/error is pre-existing (exists on master too) and the fix is obvious — fix it
- If the failure/error is pre-existing and non-obvious — **ask the user**, do not ignore it
- **Do not assume errors are pre-existing.** If you changed files under `modules/X/`, verify that `tests/dasX/` (or equivalent) produces the same error count as master. A "pre-existing" error in the same test file might have a different root cause after your changes.
- Tests MUST pass for the PR to merge

**Module-specific testing:** CI enables ALL modules via `ci/release_modules.txt` (PUGIXML, LLVM, Audio, SQLite, GLFW, HV). If you changed files under `modules/X/daslib/`, explicitly run that module's tests even if your local build has the module disabled. Handled types (C++ interop like `xml_node`, `sqlite3_stmt`) often require `var` — lint's `var`→`let` suggestion is wrong for them because the C++ binding expects non-const.

## 3. Build and run AOT tests

**IMPORTANT:** Kill the MCP server and any running daslang processes first — they lock build output files.

```bash
# Kill processes that lock build files
taskkill /F /IM daslang.exe 2>/dev/null
taskkill /F /IM mcp.exe 2>/dev/null

# Build test_aot
cmake --build build --config Release --target test_aot -j 64 -- /nodeReuse:false

# Run AOT tests (must match CI: -use-aot before --, --use-aot after --)
bin/Release/test_aot.exe -use-aot dastest/dastest.das -- --use-aot --color --failures-only --timeout 900 --test tests
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
- C++ bindings in `modules/*/src/*.cpp` or `src/builtin/*.cpp` that add new public functions, types, or struct fields
- RST files in `doc/source/` (handwritten tutorials, reference pages, TOCs)
- `doc/reflections/das2rst.das` or `doc/reflections/rst.das`

**Which substeps to run** — match what changed, not "all in order":

| Changed | Run |
|---|---|
| Only handwritten RST under `doc/source/` (tutorials, TOCs, reference prose) | **4f, 4g** (skip 4a–4e — das2rst is for `daslib` regen, not handwritten pages) |
| Only `//!` comments / new daslib public functions / C++ bindings | **4a, 4b, 4c, 4d, 4e, 4f, 4g** |
| Both daslib and handwritten RST | **all of 4a–4g** |
| `das2rst.das` / `rst.das` itself | **all of 4a–4g** |

CI runs `sphinx-build -W` (warnings-as-errors) for both HTML and LaTeX. **Any** warning fails CI. Step 4f catches every warning class — title underline/overline length, duplicate labels, broken `:ref:`, malformed tables, missing TOC entries, etc. Skipping 4f because "I only added a tutorial page" misses exactly the warnings handwritten RST tends to produce.

### 4a. Add new functions to groups in `das2rst.das`

Every public function must belong to a named group — otherwise it lands in an "Uncategorized" section which **fails CI**.

1. Find the module's `document_module_*` function in `doc/reflections/das2rst.das`
2. Add the function name to the appropriate `group_by_regex` call
3. For builtin functions: look at groups like "Macro infrastructure", "System infrastructure", etc.

### 4b. First doc generation pass

```bash
bin/Release/daslang.exe doc/reflections/das2rst.das
```

If this **panics** with "has less documentation than values", a handmade doc file needs updating. The error message lists the expected field names — find the missing one and add a description line in the correct position in the handmade file (e.g., `doc/source/stdlib/handmade/structure_annotation-rtti-CodeOfPolicies.rst`). Field descriptions are positional — line 1 is the struct description, line 2 is the first field, etc.

### 4c. Check for stubs and fill them

```bash
grep -rl "// stub" doc/source/stdlib/handmade/
```

Each stub file contains `// stub` on line 1 and the function signature on line 2. Replace the **entire file content** with a 1-2 sentence plain-text description (no RST directives). If no stubs are found, skip to step 4d.

### 4d. Second doc generation pass (picks up filled stubs)

```bash
bin/Release/daslang.exe doc/reflections/das2rst.das
```

### 4e. Verify no "Uncategorized"

```bash
grep -c Uncategorized doc/source/stdlib/generated/*.rst | grep -v ':0$'
```

Must return empty. If not, go back to step 4a and add the missing function to a group.

### 4f. Clean Sphinx build

MUST delete cache — cached builds hide errors:

```bash
rm -rf doc/sphinx-build site/doc
sphinx-build -b html -d doc/sphinx-build doc/source site/doc 2>&1 | sed 's/\x1b\[[0-9;]*m//g' | tee /tmp/sphinx_out.txt
tail -3 /tmp/sphinx_out.txt
grep -iE "warning:|error:" /tmp/sphinx_out.txt
```

Use `sphinx-build` from PATH. If unavailable, install with `pip install sphinx` (or `python -m pip install sphinx`) and re-run. The repo `.venv/` is not maintained — don't rely on it.

Must say `build succeeded.` with **zero** warnings and errors. The `grep` must return empty.

Common Sphinx issues:
- **Title overline/underline too short**: Both `===` lines around a title MUST be at least as long as the title text (Sphinx counts source chars, including backticks and inline-code spans). Easy to miscount when titles include `` ``foo`` ``, em-dashes, or non-ASCII.
- **Document not in any toctree**: Added a new RST page but forgot to wire it into `tutorials.rst` (or wherever the relevant `.. toctree::` lives). Find the matching TOC and add the page.
- **Duplicate label**: Two RST files define the same `.. _label:` — rename one
- **Unknown target**: `:ref:` points to nonexistent label — check spelling
- **Malformed table**: Grid/simple table column widths don't align
- **Unexpected indentation**: Content after directive must be indented consistently

### 4g. Stage doc changes

Add all changed/new files in `doc/` and `doc/reflections/` to the commit. For squashed branches, amend the existing commit.

See `skills/documentation_rst.md` for full details on doc conventions, tutorial RST, and cross-references.

## 5. Format all changed `.das` files

Run the MCP `format_file` tool on all changed `.das` files in a single batched call using a comma-separated list or glob pattern. Verify the changed files still compile after formatting.

**Format every changed `.das` file regardless of syntax era** — the formatter handles both gen2 and gen1 (`options gen2 = false`) cleanly, including `.das_project` files. CI fails on unformatted gen1 files (e.g. spacing around `=` in `options gen2 = false`), so don't skip them.

Do NOT format files you didn't change — only format files that are part of the PR.

### CI `das-fmt` ≠ MCP `format_file` — verify named-arg spacing

CI's `extended_checks` job runs `./bin/Release/daslang ./das-fmt/dasfmt.das -- --path ./ --verify` (plus a compiled `das-fmt.exe` pass). The `das-fmt/dasfmt.das` file is NOT in the repo tree — CI fetches it externally — and it is **stricter** than MCP `format_file` on at least one rule:

| MCP `format_file` accepts | CI `das-fmt` requires |
|---|---|
| `Foo(a=1, b=2)` | `Foo(a = 1, b = 2)` (spaces around `=` in named args) |

`bin/Release/das-fmt.exe` built from `utils/dasFormatter/` is the **v1→v2 syntax converter**, NOT the same formatter — running it locally does not reproduce CI.

**Before pushing:** mentally format named-arg constructor / call sites with spaces around `=`. If CI `extended_checks` fails on a format diff after MCP said "already formatted", fix the spacing and re-push (or amend, on a squashed branch).

## 5.5. Run the task-wrap-up curation pass

Pre-PR is a natural moment to close the blind-mouse loop — work is final, you know what was learned, diff is locked. The full curation steps (review log, surface un-asked, cost calculus) live in `skills/task_wrap_up.md` and apply to *any* major task wrap-up, not just PRs. Run that pass now if you keep a personal Q&A cache.

## 6. Create the PR

Stage, commit, push, and create the PR using GitHub MCP tools or `gh` CLI. Follow the commit message conventions from the repository (see recent `git log` for style).

**Squashed branches:** If the user asked to squash the branch (single commit), keep it squashed. Any subsequent fixes (lint, test failures, formatting, CI issues, review-comment fixes) must be amended into the existing commit (`git commit --amend --no-edit`) and force-pushed — do NOT create new commits on a squashed branch.

**Squash-from-multi-commit pattern (`git reset --soft master`)**: only safe AFTER step 0 has rebased onto `origin/master`. Otherwise the soft reset collapses the gap between local `master` and origin's into your one commit. If you forgot step 0 and already pushed a leaky PR, the fix is `git fetch origin master && git rebase origin/master && git push --force-with-lease` — git's 3-way merge drops files where your "modification" matches the already-merged content on origin/master.

## Quick reference

| Step | Tool/Command | Fix policy |
|---|---|---|
| Sync | `git fetch origin master && git rebase origin/master` | Always run first; verify diff vs origin/master is clean |
| Lint | `utils/lint/main.das` | Fix significant issues in changed files |
| Tests | `dastest -- --test tests/` | Must pass. Fix own, fix obvious pre-existing, ask about unclear |
| AOT build | `cmake --build build --config Release --target test_aot -j 64` | Kill daslang first. Register new test dirs |
| AOT tests | `test_aot.exe -use-aot dastest/dastest.das -- --use-aot --test tests` | Same as regular tests |
| Docs | `das2rst.das` + stubs + Sphinx | Only if daslib/C++ bindings/RST changed |
| Format | MCP `format_file` with comma-separated list or glob of changed `.das` files (single call) | Only changed files |
| Wrap-up curation | `skills/task_wrap_up.md` | Optional. Add answers for cache misses, edit cached answers this PR invalidated |
| `.md` stop | `git diff --name-only origin/master..HEAD \| grep '\.md$'` | If any match: STOP, list changes, ask user to review BEFORE push |
| PR | GitHub MCP `create_pull_request` or `gh pr create` | — |
| Review iter | Follow `skills/pr_review_iteration.md` | One round per Copilot pass; convergence in 1-3 rounds is normal |
