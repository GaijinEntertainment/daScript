# Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**A test that runs a class carrying a `[metal_kernel]` method or creates any Metal object,
wherever under this folder the diff puts it, answers to `metal/REVIEW.md`.**

**An MSL emitter fixture (a test that compiles a kernel and checks the emitted MSL), wherever
under this folder the diff puts it, or a change to `msl/test_msl_census.das` answers to
`msl/REVIEW.md`.**

**A SPIR-V emitter fixture (a test holding a `*_words` function that compiles a shader and
returns its emitted words), wherever under this folder the diff puts it, or a change to
`spirv/test_census.das` answers to `spirv/REVIEW.md`.**

**A test that runs a class carrying a `[cuda_kernel]` method or calls `with_cuda_context`, or a
CUDA C emitter fixture (a test that compiles a kernel and checks the emitted CUDA C), wherever
under this folder the diff puts it, or a change to `cuda/test_cuda_census.das` answers to
`cuda/REVIEW.md`.**

**A test that calls `tick_debug_agent`, wherever under this folder the diff puts it, answers to
`debug_agent/REVIEW.md`.**

**A test that asserts on `require` path lookup, module compile order, or files under the AST
module cache folder (`.cache/daslang/module_cache/`), wherever under this folder the diff puts
it, answers to `module_cache/REVIEW.md`.**

**A diff that adds, moves, or changes a test that asserts on `require` path lookup, module
compile order, or files under the AST module cache folder (`.cache/daslang/module_cache/`) leaves
it under `module_cache/`.**

**A diff that adds or changes a test that writes a file or directory - itself, through any
function it calls (daslang or C++-bound), or through a program or server it starts, including
under a working directory or project root it passes that program - writes it under
`test_temp_dir()` (`dastest/testing_boost`, repo root) or a directory from
`create_temp_directory` (`daslib/fio`, repo root); a write anywhere else, the system temp folder
`temp_directory()` returns included, is a defect.** Two runs of one test file can share the tree
at once, and only those two directories are one run's own. The reviewer checks every write
`REVIEW.das` does not flag; weakening that check is a defect.

**A diff that widens the `dasbind` skip in `.das_test` or drops one of the `[extern]` probe
functions or any of its assertions from `dasbind/test_extern_abi.das` is a defect.** The suite is
the only check of which register or stack slot an interpreted `[extern]` call puts each argument
in - the JIT never takes that path - so coverage lost there reports green on every lane.

**A diff that changes a `.das` file under this folder not marked `options no_aot` - one an
`AOT_*_FILES` variable of `tests/aot/CMakeLists.txt` holds after its filters, or one in a
`tests/<name>/` folder `DAS_AOT_SUITES` lists - names in the PR body the run that showed
`test_aot` pass: a local `daslang utils/internal/preflight/main.das -- --full` (repo root), a
`--target run_tests_aot` build, or a CI job link.** Per-PR CI builds only the AOT subset binary
(the test_aot_subset target), so the PR's own checks do not prove `test_aot`.

**A diff that marks a `.das` file under `tests/` `options no_aot`, or filters one out of an
`AOT_*_FILES` variable of `tests/aot/CMakeLists.txt`, states in a comment beside the option or
the filter what stops `test_aot` compiling or running the file, why an AOT build of it tests
nothing, or which `AOT_*_FILES` variable compiles it instead.** Without a stated reason no later
diff can tell whether the opt-out is still needed.
