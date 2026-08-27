# dascov Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**A diff that removes `dascov` from `DAS_UTILS` or `DAS_UTILS_SHIPPED_EXES` in
`utils/CMakeLists.txt` is a defect.** Either entry missing drops the prebuilt dascov exe from
the release bundle.
