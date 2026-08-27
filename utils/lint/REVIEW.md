# lint Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**A diff that removes `lint` from `DAS_UTILS` or `DAS_UTILS_SHIPPED_EXES`
(`utils/CMakeLists.txt`) is a defect** - dropping either entry breaks the release bundle.

**A diff that drops a rule id from the set `REVIEW.das` (beside this file) finds - by
editing the gate, or by removing the id's only spelling the gate can find in the source -
is a defect.** The gate checks that every rule id a lint module emits has a fixture and a
section in `doc/source/reference/language/lint.rst` (repo root), so a dropped id retires
that check with no warning.
