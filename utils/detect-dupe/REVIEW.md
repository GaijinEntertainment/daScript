# detect-dupe Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`.

**detect-dupe ships as a prebuilt exe: it stays in `DAS_UTILS` and `DAS_UTILS_SHIPPED_EXES`
(`utils/CMakeLists.txt`)** — dropping either entry breaks the release bundle.
