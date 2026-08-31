# CI Scripts Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md` (repo root).

**A diff that shrinks what the bundle gate rejects - `smoke_test_bundle.sh` and the checkers
it runs (`check_shipped_skills.py`) - is a defect**: every bundle it failed before the diff
still fails. A new `--exclude` or skip may name only a file no check flagged before the diff.
