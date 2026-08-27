# detect-dupe Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**A diff removing detect-dupe from `DAS_UTILS_SHIPPED_EXES` records the removal decision in
this file.** detect-dupe ships as a prebuilt exe, and `utils/REVIEW.das` machine-checks the
membership itself.
