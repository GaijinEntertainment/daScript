# Utils Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `CLAUDE.md` (repo root).

**A tool's out-of-folder companions — its CI workflow, its docs page, its `daspkg` manifest
entry — answer to that tool's own `REVIEW.md` too when it carries one**, on top of the
path-membership routing the discovery walk already does for files under the tool directory
(a direct child of `utils/` or `utils/internal/`).

**Weakening `REVIEW.das` (beside this file) is a defect.** It machine-checks this folder's
mechanical rules — what it enforces is read from it, each check's finding text stating its
rule; the bundle-side twin of its internal-reference check is `ci/smoke_test_bundle.sh`.

**A tool REMOVED from `DAS_UTILS_SHIPPED_EXES` (`utils/CMakeLists.txt`) records the decision
in that tool's own `REVIEW.md` (for `dastest`, `dastest/REVIEW.md` at the repo root) in the
same change** — a removed entry's decision record is the one thing the gate cannot see.
