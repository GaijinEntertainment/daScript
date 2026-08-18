# Utils Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `CLAUDE.md` (repo root).

A tool directory is any direct child directory of `utils/` (or `utils/internal/`) that is
not a C++ binary source dir (one carrying `main.cpp` — its binary is documented under
`bin/`) and not the shared library `common`. A file belonging to a tool whose directory
carries its own `REVIEW.md`, wherever the diff puts it, answers to that tool's checklist too.

**Weakening `REVIEW.das` (beside this file) is a defect.** It machine-checks this folder's
mechanical rules — what it enforces is read from it, each check's finding text stating its
rule; the bundle-side twin of its internal-reference check is `ci/smoke_test_bundle.sh`.

**A tool REMOVED from `DAS_UTILS_SHIPPED_EXES` (`utils/CMakeLists.txt`) records the decision
in that tool's own `REVIEW.md` (for `dastest`, `dastest/REVIEW.md` at the repo root) in the
same change** — a removed entry's decision record is the one thing the gate cannot see.
