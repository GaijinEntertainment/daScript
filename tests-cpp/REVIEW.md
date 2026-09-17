# tests-cpp Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `skills/internal/writing_cpp_tests.md`.

**A `*_pin.cpp` file, wherever the diff puts it, answers to the `small/` subfolder's
checklist as well as this one.**

**A test whose ctest labels include `big`, wherever the diff puts it, answers to the `big/`
subfolder's checklist as well as this one.**

**A diff that adds or changes a C++ test, or adds or changes a gate that decides whether one
builds, registers, or runs - a doctest skip, a `#if` a configuration leaves undefined, a CMake
gate on a build option - names in the PR one configuration that does run that test: its
configure flags and its ctest command.**
