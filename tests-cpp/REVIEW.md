# tests-cpp Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `skills/internal/writing_cpp_tests.md` (repo root).

**A `*_pin.cpp` file, wherever the diff puts it, answers to the `small/` subfolder's
checklist as well as this one.**

**A test that owns its own `CMakeLists.txt`, wherever the diff puts it, answers to the
`big/` subfolder's checklist as well as this one.**

**A diff adding or changing a test that skips or reports pass without running, under any
condition not every lane meets - a missing file or binary, an unset environment variable -
says in the PR which lane runs it for real, naming the command.**
