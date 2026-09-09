# tests-cpp/small Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `skills/internal/writing_cpp_tests.md` (repo root).

**A diff that drops a file from a pin test's watched list, drops one of its arms, or loosens
its comparison is a defect unless the same PR removes what the pin protected.** A pin test
(`*_pin.cpp`) asserts that a compiled-in layout, offset, or watched file set stays put; a stale
stamp turns it red on its own, so re-stamping needs no rule.

**A diff that weakens `test_float2string.cpp`'s byte-for-byte compare against fmt's `"{}"` -
fewer patterns, looser comparison, or a dropped arm - is a defect.** That test holds
`float2string`/`double2string` to the spelling the `tests/msl/` and `tests/glsl/` goldens pin.
