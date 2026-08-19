# dasImgui Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`CLAUDE.md`.

**Tests go under `modules/dasImgui/tests`.** Every dasImgui test file in the diff is under it.
A dasImgui test file, wherever the diff puts it, also answers to the `tests/` subfolder's
checklist (`modules/dasImgui/tests/REVIEW.md`).

**A diff whose hunks change executable content in any `.das` file under this folder, or any
file under `bind/` or `src/`, runs the suite before the PR on the author's host OS:
`preflight --only imgui`** (the per-OS exclude split: module `CLAUDE.md` § Tests). No per-PR CI
lane runs the suite (`.github/workflows/nightly_imgui.yml` is nightly-only), so the local run is
the only pre-merge gate.
