# dastest Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`.

**dastest ships as a prebuilt exe: it stays in the `dastest_exe` install set
(`DAS_UTILS_SHIPPED_EXES`, `utils/CMakeLists.txt`) and `dastest.das` stays
`daslang -exe`-compilable** — a change that breaks the exe build breaks the release bundle.
