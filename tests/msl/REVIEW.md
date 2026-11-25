# MSL emitter fixtures Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`modules/dasMetal/ARCHITECTURE.md`. Shared emitter rules: `modules/REVIEW_SHADER_EMITTERS.md` -
apply that list with this one.

**Weakening the census check - the two-way assertion in `uncategorized.das` that the kinds the
fixtures emit are the kinds `declared_msl_census` (`_msl_common.das`) declares - is a defect; so
is dropping from `all_msl_censuses` (`_msl_common.das`) the census of a fixture that still
exists.**
Emitted text cannot be read back into constructs, so the census is the only measure of coverage.

**A diff that adds a fixture outside `_fail_closed/` - a `[metal_kernel]` kernel this folder
declares, whose emit records a `<name>_census` global - adds that census to `all_msl_censuses`
(`_msl_common.das`) in the same change.** A fixture left out of that list emits kinds the
check never sees, so both directions pass with the fixture uncounted.

**Weakening `uncategorized_no_aot.das`'s `check_rejects` is a defect.** It asserts each
`_fail_closed/` fixture fails to compile and that the compile error contains the fixture's
needle - the substring naming the rejected construct.
