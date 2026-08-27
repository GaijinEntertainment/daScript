# das-fmt Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**A diff that drops `das-fmt` from `DAS_UTILS` or `DAS_UTILS_SHIPPED_EXES` in
`utils/CMakeLists.txt` (repo root) is a defect** - das-fmt ships as a prebuilt exe, and dropping
either entry breaks the release bundle.
