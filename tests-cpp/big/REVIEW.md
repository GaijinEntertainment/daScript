# tests-cpp/big Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `skills/internal/writing_cpp_tests.md` (repo root).

**A diff that touches a test under this folder says in the PR that the test ran and passed
on the author's machine, naming the command - `ctest -L big` or the test's own binary.** CI
runs only the small suite, so nothing else proves a big test.
