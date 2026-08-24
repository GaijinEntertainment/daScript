# tests-cpp Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `skills/internal/writing_cpp_tests.md` (repo root).

**A `*_pin.cpp` file, wherever the diff puts it, answers to the `small/` subfolder's
checklist.**

**A diff that touches a `big/<name>/` test states its local run result.** CI runs only the
small suite, so a big test proves itself only on the author's machine. The PR says that
`ctest -L big` (or the affected test binary) ran and passed; without that line the reviewer
treats the test as never executed.
