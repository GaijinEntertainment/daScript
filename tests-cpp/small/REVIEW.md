# tests-cpp/small Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `skills/internal/writing_cpp_tests.md` (repo root).

**A diff that widens or removes a pin assertion also changes, in the same PR, what that
assertion watches - the struct, offset, or file named in the pin's own assertion text.** A
pin test (`*_pin.cpp`) asserts that a compiled-in layout, offset, or watched file set stays
put.
