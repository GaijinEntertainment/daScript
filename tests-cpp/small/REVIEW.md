# tests-cpp/small Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `skills/internal/writing_cpp_tests.md` (repo root).

**A diff that widens or removes a pin assertion also changes, in the same PR, what that
assertion watches - the struct, offset, or file named in the pin's own assertion text.** A
pin test (`*_pin.cpp`) asserts that a compiled-in layout, offset, or watched file set stays
put.

**A diff that narrows what `test_float2string.cpp` compares - fewer bit patterns, a
tolerance in place of the byte compare, or a dropped `double2string` arm - is a defect.**
That test is what holds `float2string` and `double2string` to fmt's `"{}"` spelling, which
the msl/glsl goldens and the string corpus compare byte for byte.
