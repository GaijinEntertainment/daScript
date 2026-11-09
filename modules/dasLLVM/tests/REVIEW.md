# dasLLVM Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE.md` (beside this file). The module's own checklist, `../REVIEW.md`, binds too:
its gate names every test file a CI lane must run.

**A diff that adds a `[test]` here puts it in the existing category file for its lane's topic or
in `uncategorized.das`, never in a new file, with a `description`, and with `timeout = 1200` when
the test builds, spawns a child daslang or runs the tuner** - the shape rules are
`tests/REVIEW.md` (repo root) and `tests/ARCHITECTURE.md`; `REVIEW.das` (beside this file) applies
them, and `TESTS.md` (beside this file) is the generated index (`daslang
utils/internal/test-audit/main.das -- --root modules/dasLLVM/tests --index
modules/dasLLVM/tests/TESTS.md --readme`; the `check_tests_index` target verifies it). `README.md`
beside this file is prose, not the index.

**A diff that adds a `.das` file here that is not itself a test - a program root a test builds or
runs through a child daslang, a client, a module a fixture requires - starts its name with `_`** -
dastest skips a `_` name when it scans the directory, so such a file never runs as a test of its
own, and the lane rows in `../CMakeLists.txt` never have to name it.
