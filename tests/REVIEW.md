# Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**A test answers to its kind's checklist wherever under this folder the diff puts it:**
a test that runs a class carrying a `[metal_kernel]` method or creates any Metal object -
`metal/REVIEW.md`;
an MSL emitter fixture (a test that compiles a kernel and checks the emitted MSL) or a change to
`msl/test_msl_census.das` - `msl/REVIEW.md`;
a SPIR-V emitter fixture (a test holding a `*_words` function that compiles a shader and returns
its emitted words) or a change to `spirv/test_census.das` - `spirv/REVIEW.md`;
a test that calls `tick_debug_agent` - `debug_agent/REVIEW.md`;
a test that asserts on `require` path lookup, module compile order, or files under the AST module
cache folder (`.cache/daslang/module_cache/`) - `module_cache/REVIEW.md`.

**A diff that adds, moves, or changes a test that asserts on `require` path lookup, module
compile order, or files under the AST module cache folder
(`.cache/daslang/module_cache/`) keeps it in `tests/module_cache/` (repo root),
or in a `module_cache/` directory within the tool it tests.**

**Every file or directory a test writes - itself, through a C++-bound function it calls, or
through a program or server it starts, including what that program writes under a working
directory or project root the test passes it - goes under `test_temp_dir()` (`dastest/testing_boost`,
repo root) or a directory from `create_temp_directory` (`daslib/fio`, repo root), and nowhere
else - not `temp_directory` itself, however unique the name.** Two runs of one test file can
share the tree at once, and only those two directories are one run's own. The reviewer checks
every write `REVIEW.das` does not flag; weakening that check is a defect.

**A diff that widens the `dasbind` skip in `.das_test` or drops one of the `[extern]` probe
functions or any of its assertions from `dasbind/test_extern_abi.das` is a defect.** The suite is the only
check of which register or stack slot an interpreted `[extern]` call puts each argument in - the
JIT never takes that path - so coverage lost there reports green on every lane.

**A diff that changes a `.das` file under this folder that `test_aot` compiles names in the PR
body the run that showed `test_aot` pass: a local `preflight --full`, a `--target run_tests_aot`
build, or a CI job link.** `test_aot` compiles every file not marked `options no_aot` that an
`AOT_*_FILES` variable of `tests/aot/CMakeLists.txt` holds after its filters; a folder
`tests/<name>/` its `DAS_AOT_SUITES` lists is globbed whole only where no variable already covers
it. Per-PR CI builds only the AOT subset binary (the test_aot_subset target), so the PR's own
checks do not prove `test_aot`.

**A diff that marks a `.das` file under `tests/` `options no_aot`, or filters one out of an
`AOT_*_FILES` variable of `tests/aot/CMakeLists.txt`, states in a comment beside the option or
the filter what stops the file compiling or running on its suite's `test_aot` lane, or why an
AOT build of it tests nothing.** Without a stated reason no later diff can tell whether the
opt-out is still needed.
