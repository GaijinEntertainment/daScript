# MSL emitter fixtures Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`modules/dasMetal/MASTERPLAN.md`.

**Weakening `test_msl_census.das`'s two-direction check - every declared kind emitted by some
fixture, every emitted kind declared - is a defect.** Text has no disassembler; the census is
the only coverage proxy.
