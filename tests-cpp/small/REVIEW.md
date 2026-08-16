# tests-cpp/small Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `skills/internal/writing_cpp_tests.md` (repo root).

**A pin test is weakened only by the source change that forces it.** A pin test (`*_pin.cpp`)
asserts that a compiled-in layout, offset, or watched file set stays put. A diff that loosens
or deletes a pin assertion carries, in the same PR, a change to what that assertion watches —
the struct, offset, or file named in the pin's own assertion text; a pin relaxed with no such
source change is a defect.
