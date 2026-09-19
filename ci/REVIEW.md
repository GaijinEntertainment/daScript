# CI Scripts Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `CLAUDE.md` (repo root), `skills/internal/preflight.md` (repo root), `packaging/README.md`.

**A diff that shrinks what the bundle gate rejects - `smoke_test_bundle.sh` and the checkers
it runs (`check_shipped_skills.py`) - is a defect**: every bundle it failed before the diff
still fails. A new `--exclude` or skip may name only a file no check flagged before the diff,
and a skip on a file the same diff adds states in the PR body why the file cannot be fixed
instead.

**Weakening `test_ci_matrix.py` - dropping or loosening any assertion it makes - is a
defect.** Those assertions are what turns a job or step that stopped running per PR into a red
test.

**Weakening `REVIEW.das` (beside this file) is a defect: dropping a check, narrowing what a check
walks, or rewriting a finding text so it no longer names what failed.**
