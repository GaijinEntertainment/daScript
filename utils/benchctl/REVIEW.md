# benchctl Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**A diff that drops `benchctl` from `DAS_UTILS` or from `DAS_UTILS_SHIPPED_EXES`
(`utils/CMakeLists.txt`) is a defect** - those two entries are what puts the prebuilt
`benchctl` exe in the release bundle when sqlite is enabled.
