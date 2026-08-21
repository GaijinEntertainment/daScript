# CI Scripts Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `CLAUDE.md` (repo root).

**Weakening `smoke_test_bundle.sh` — the release-bundle smoke gate — is a defect.** What it
enforces is read from the script: dropping a check, loosening an assertion, or shortening a
list it walks each weaken it.
