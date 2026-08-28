# tests-cpp/big Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `skills/internal/writing_cpp_tests.md` (repo root).

**A diff that touches a test whose ctest labels include `big` says in the PR that the test
ran and passed on the author's machine, naming the command - `ctest -L big` or the test's
own binary.** CI runs `ctest -L small` only, so no other run shows that a big-labelled test
passes.
