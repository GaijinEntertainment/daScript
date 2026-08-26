# MSL emitter fixtures Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`modules/dasMetal/MASTERPLAN.md`.

**Weakening the census check - every kind in `declared_msl_census` emitted by some fixture,
every kind any fixture emits declared there, where a kind is the construct label the emitter
records at emit time - is a defect**, whether by dropping either direction's assertion in
`test_msl_census.das`, dropping a kind from `declared_msl_census`, or dropping a fixture's
census from `all_msl_censuses` (both in `_msl_common.das`). Text has no disassembler; the
census is the only coverage proxy.
