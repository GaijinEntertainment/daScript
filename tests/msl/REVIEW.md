# MSL emitter fixtures Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`modules/dasMetal/MASTERPLAN.md`.

**Weakening the census check is a defect.** The check runs both directions: every kind in
`declared_msl_census` is emitted by some fixture, and every kind any fixture emits is declared
there. A kind is the construct label the emitter records at emit time. Dropping either
direction's assertion in `test_msl_census.das` weakens the check, and so does dropping a kind
from `declared_msl_census` or a fixture's census from `all_msl_censuses` (both in
`_msl_common.das`). Emitted text cannot be read back into constructs, so the census is the
only measure of coverage.
