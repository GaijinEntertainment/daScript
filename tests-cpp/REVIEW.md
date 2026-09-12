# tests-cpp Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `skills/internal/writing_cpp_tests.md`.

**A `*_pin.cpp` file, wherever the diff puts it, answers to the `small/` subfolder's
checklist as well as this one.**

**A test that owns its own `CMakeLists.txt`, wherever the diff puts it, answers to the
`big/` subfolder's checklist as well as this one.**

**A C++ test a diff adds or changes that some lane running the suite cannot fail on - the test
skips there, or its subject sits behind a `#if` that lane leaves undefined - names in the PR
the lanes that do run it, with the command.**
