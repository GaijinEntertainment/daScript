# das-fmt Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**A diff that removes `das-fmt` from `DAS_UTILS_SHIPPED_EXES` in `utils/CMakeLists.txt` is a
defect.** The removal drops the prebuilt das-fmt exe from the release bundle, and the gate
cannot see a removed entry.
