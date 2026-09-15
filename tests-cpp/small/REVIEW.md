# tests-cpp/small Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `skills/internal/writing_cpp_tests.md`.

**A diff that widens or removes a pin assertion also changes, in the same change, what that
assertion watches.** A pin test (`*_pin.cpp`) asserts that a compiled-in layout, offset, or
watched file set stays put.

**A diff that weakens `test_float2string.cpp`'s byte-for-byte compare against fmt's `"{}"` -
fewer patterns, looser comparison, or a dropped arm - is a defect.** That test holds
`float2string`/`double2string` to the spelling the `tests/msl/` and `tests/glsl/` goldens pin.

**Weakening `test_env_serializer.cpp`'s position census - the cold-to-warm comparison of every
source position it records, field for field - is a defect.** The record codes a `LineInfo` as
a delta against the previous one, so a compression change rounds a position off with nothing
else noticing.
