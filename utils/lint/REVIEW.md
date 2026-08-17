# lint Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`doc/source/reference/utils/lint.rst` (repo root).

**lint ships as a prebuilt exe: it stays in `DAS_UTILS_SHIPPED_EXES` (`utils/CMakeLists.txt`) and
`main.das` stays `daslang -exe`-compilable** — a change that breaks the exe build breaks the
release bundle.
