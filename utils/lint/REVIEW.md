# lint Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`.

**`lint` ships as a prebuilt exe (`DAS_UTILS_SHIPPED_EXES` in `utils/CMakeLists.txt`): a change
that un-ships it records the decision here in the same change.**

**Every rule id a lint module emits (`daslib/lint.das`, `daslib/perf_lint.das`,
`daslib/style_lint.das`) has a fixture `tests/<id-lowercase>_*.das` and a section in
`doc/source/reference/language/lint.rst` (repo root).** Weakening `REVIEW.das` (beside this
file), which checks that triple, is a defect.
