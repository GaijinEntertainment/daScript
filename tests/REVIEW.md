# Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**A test answers to its kind's checklist wherever the diff puts it:** a test whose subject is
module resolution, module loading or the module cache to `module_cache/REVIEW.md`; a test that
runs a dasMetal kernel class or creates any Metal object to `metal/REVIEW.md`; an MSL emitter
fixture or a change to `msl/test_msl_census.das` to `msl/REVIEW.md`; a SPIR-V emitter `*_words`
fixture (a file of expected SPIR-V words) or a change to `spirv/test_census.das` to
`spirv/REVIEW.md`; a test that calls `tick_debug_agent` to `debug_agent/REVIEW.md`.

**A diff that widens the `dasbind` skip in `.das_test` or drops one of the `[extern]` probe
functions or its assertion from `dasbind/test_extern_abi.das` is a defect.** The suite is the only
check of which register or stack slot an interpreted `[extern]` call puts each argument in - the
JIT never takes that path - so coverage lost there reports green on every lane.
