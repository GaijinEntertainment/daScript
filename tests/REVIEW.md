# Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md` (this folder).

**A test answers to its kind's checklist wherever the diff puts it:**
a test that asserts on `require` path lookup, module compile order, or files under the module
cache folder - `module_cache/REVIEW.md`;
a test that runs a dasMetal kernel class or creates any Metal object - `metal/REVIEW.md`;
an MSL emitter fixture (a test that compiles a kernel and checks the emitted MSL) or a change to
`msl/test_msl_census.das` - `msl/REVIEW.md`;
a SPIR-V emitter fixture (a test holding a `*_words` function that compiles a shader and returns
its emitted words) or a change to `spirv/test_census.das` - `spirv/REVIEW.md`;
a test that calls `tick_debug_agent` - `debug_agent/REVIEW.md`.

**A diff that widens the `dasbind` skip in `.das_test` or drops one of the `[extern]` probe
functions or any of its assertions from `dasbind/test_extern_abi.das` is a defect.** The suite is the only
check of which register or stack slot an interpreted `[extern]` call puts each argument in - the
JIT never takes that path - so coverage lost there reports green on every lane.

**A change to a `.das` file that it leaves in any `AOT_*_FILES` variable of
`tests/aot/CMakeLists.txt`, `_MODULE_FILES` ones included - by glob or by name - is green on
both the `dastest` run and that suite's `test_aot` lane.** The `tests` suite is the folder
`tests/aot/`, not `tests/`, and the per-PR CI compiles only the language subset.

**A new `.das` file under `tests/` that a glob in `tests/aot/CMakeLists.txt` puts in an
`AOT_*_FILES` variable and that does not compile on its suite's `test_aot` lane is filtered out
of that variable in the same change, with the reason in a comment beside the filter.** The glob
picks a new file up silently.
