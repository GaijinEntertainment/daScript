# dasImgui Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`CLAUDE.md`.

## Tests

**Tests go under `modules/dasImgui/tests`.** Every dasImgui test file in the diff is under it;
`tests/dasImgui` is not a home.

**A change that can affect runtime behavior — any `.das`, `bind/`, or `src/` file — runs the
suite before the PR:**
`daslang dastest/dastest.das -- --test modules/dasImgui/tests --headless --isolated-mode --isolated-mode-threads 4 --exclude glfw_synth --exclude key_hud --exclude embedded_terminal`
(also available as `preflight --only imgui`). `nightly_imgui.yml` is the suite's only CI lane,
so the local run is the only pre-merge gate.

**On local Windows runs, additionally pass the high-POST excludes** listed in
`modules/dasImgui/CLAUDE.md` § Tests.

**Tests are kept multiplatform** — no hardcoded platform-specific paths, separators, or
OS-only assumptions; locations resolve via `get_das_root()` / module-relative helpers, never
absolute or machine-local paths.

**`modules/dasImgui/tests/test_grammar_canary.das` is governed by
`tree-sitter-daslang/REVIEW.md`** — a grammar change touching it applies that list too.
