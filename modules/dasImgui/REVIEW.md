# dasImgui Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`CLAUDE.md`.

**Tests go under `modules/dasImgui/tests`.** Every dasImgui test file in the diff is under it;
`tests/dasImgui` is not a home.

**A change that can affect runtime behavior — any `.das`, `bind/`, or `src/` file — runs the
suite before the PR: `preflight --only imgui`** (raw command and exclude lists: module
`CLAUDE.md` § Tests; local Windows adds the high-POST excludes listed there).
`nightly_imgui.yml` is the suite's only CI lane, so the local run is the only pre-merge gate.

**Tests are kept multiplatform** — no hardcoded platform-specific paths, separators, or
OS-only assumptions; locations resolve via `get_das_root()` / module-relative helpers, never
absolute or machine-local paths.

**A diff touching both the grammar (`tree-sitter-daslang/`) and
`modules/dasImgui/tests/test_grammar_canary.das` applies `tree-sitter-daslang/REVIEW.md` too.**
