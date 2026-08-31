# tests-cpp/small Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `skills/internal/writing_cpp_tests.md` (repo root).

**A diff that widens or removes a pin assertion also changes, in the same PR, what that
assertion watches - the struct, offset, or file named in the pin's own assertion text.** A
pin test (`*_pin.cpp`) asserts that a compiled-in layout, offset, or watched file set stays
put.

**A diff that weakens `test_float2string.cpp`'s byte-for-byte compare against fmt's `"{}"` -
fewer patterns, looser comparison, or a dropped arm - is a defect.** That test holds
`float2string`/`double2string` to the spelling the `tests/msl/` and `tests/glsl/` goldens pin.
