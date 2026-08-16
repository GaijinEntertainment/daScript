# dasImgui Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`CLAUDE.md`.

**Tests go under `modules/dasImgui/tests`.** Every dasImgui test file in the diff is under it;
`tests/dasImgui` (repo root) is not a home. A dasImgui test file, wherever the diff puts it,
answers to the `tests/` subfolder's checklist.

**A change that can affect runtime behavior — any `.das`, `bind/`, or `src/` file — runs the
suite before the PR on the author's host OS: `preflight --only imgui`** (raw command and
exclude list: module `CLAUDE.md` § Tests).
`.github/workflows/nightly_imgui.yml` is the suite's only CI lane, so the local run is the
only pre-merge gate.
