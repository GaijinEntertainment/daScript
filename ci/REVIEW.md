# CI Scripts Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md` (repo root).

**A diff that shrinks what `smoke_test_bundle.sh` - the script that checks a built release
bundle - rejects is a defect**: every bundle it failed before the diff still fails. A new
exemption may name only a file no existing check matched.
