# lint Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`. `lint` ships as a prebuilt exe — the `DAS_UTILS_SHIPPED_EXES` entry
(`utils/CMakeLists.txt`) records this decision; the rules for changing it live in
`utils/REVIEW.md`.

**Weakening `REVIEW.das` (beside this file) is a defect** — it checks that every rule id
`daslib/lint.das`, `daslib/perf_lint.das`, and `daslib/style_lint.das` emit has a fixture and a
section in `doc/source/reference/language/lint.rst` (repo root).
