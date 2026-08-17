# das-fmt Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`.

**das-fmt ships as a prebuilt exe: it stays in `DAS_UTILS_SHIPPED_EXES` (`utils/CMakeLists.txt`)
and `dasfmt.das` stays `daslang -exe`-compilable** — a change that breaks the exe build breaks
the release bundle.
