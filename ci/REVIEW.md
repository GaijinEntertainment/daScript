# CI Scripts Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md` (repo root).

**A diff that weakens `smoke_test_bundle.sh` - the script that checks a built release bundle -
is a defect.** Weakening means dropping a check, loosening an assertion, or shortening a list
the script walks.
