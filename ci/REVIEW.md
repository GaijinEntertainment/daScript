# CI Scripts Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `CLAUDE.md` (repo root), `skills/internal/preflight.md`.

**A diff that shrinks what the bundle gate rejects - `smoke_test_bundle.sh` and the checkers
it runs (`check_shipped_skills.py`) - is a defect**: every bundle it failed before the diff
still fails. A new `--exclude` or skip may name only a file no check flagged before the diff.

**Weakening `ci/test_ci_matrix.py` - dropping its role-condition assertion or its per-event
matrix-cell assertions - is a defect.** The nightly `extended_checks` job runs with role `all`
and is the only run of the steps too slow for a PR, so a step conditioned on its own role
would run nowhere in full; that test is what keeps every condition `matrix.role != '<other>'`.

**Weakening `REVIEW.das` (beside this file) is a defect.** What the gate checks is read from the
script itself, and each check's finding text states its rule.
