# Utils Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `CLAUDE.md` (repo root).

**Weakening `REVIEW.das` (beside this file), or its bundle-side twin
`ci/smoke_test_bundle.sh`, is a defect.** The gate machine-checks this folder's
mechanical rules — what it enforces is read from it, each check's finding text stating its
rule.

**A tool REMOVED from `DAS_UTILS_SHIPPED_EXES` (`utils/CMakeLists.txt`) records the decision
in that tool's own `REVIEW.md` (for `dastest`, `dastest/REVIEW.md` at the repo root; beside
the list in `utils/CMakeLists.txt` when the tool is deleted outright) in the
same change** — a removed entry's decision record is the one thing the gate cannot see.

**A new or changed test under a `utils/` tool ships in the same change with the row that
runs it** — `.github/workflows/extended_checks.yml` for a `.das` or `.py` test, the tool's
own workflow for a browser suite. A test no workflow names is a defect: it passes review
once and never runs again.
