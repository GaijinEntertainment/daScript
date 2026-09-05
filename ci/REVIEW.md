# CI Scripts Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `CLAUDE.md` (repo root), `skills/internal/preflight.md`.

**A diff that shrinks what the bundle gate rejects - `smoke_test_bundle.sh` and the checkers
it runs (`check_shipped_skills.py`) - is a defect**: every bundle it failed before the diff
still fails. A new `--exclude` or skip may name only a file no check flagged before the diff.

**Weakening `ci/test_ci_matrix.py` - dropping or loosening any assertion it makes - is a
defect.** One assertion carries the role split: every `matrix.role` condition in
`extended_checks.yml` is spelled `!=`, because the nightly job sets `role: all` and an `==`
condition would skip its step in every nightly job.

**Weakening `REVIEW.das` (beside this file) is a defect: dropping a check, narrowing what a check
walks, or rewriting a finding text so it no longer names what failed.**
