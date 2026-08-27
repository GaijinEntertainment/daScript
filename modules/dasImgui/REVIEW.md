# dasImgui Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`CLAUDE.md`.

**Never add or change a dasImgui test file outside `modules/dasImgui/tests` - put it under that
folder instead.** A dasImgui test file, wherever the diff puts it, also answers to the `tests/`
subfolder's checklist (`modules/dasImgui/tests/REVIEW.md`).

**A diff that changes executable content in any `.das` file under this folder, or in any file
under `bind/` or `src/`, runs the test suite on the author's host OS before the PR:
`preflight --only imgui`** (the per-OS exclude split: module `CLAUDE.md` sec. Tests). No per-PR CI
lane runs that suite (`.github/workflows/nightly_imgui.yml` is nightly-only), so the local run is
the only pre-merge gate.
