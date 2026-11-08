# Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE.md`.

**A test of lint suppression or lint configuration, wherever under this folder the diff puts
it, answers to `lint/REVIEW.md`.**

**A test that runs a class carrying a `[metal_kernel]` method or creates any Metal object,
wherever under this folder the diff puts it, answers to `metal/REVIEW.md`.**

**An MSL emitter fixture (a test that compiles a kernel and checks the emitted MSL), wherever
under this folder the diff puts it, answers to `msl/REVIEW.md`.**

**A SPIR-V emitter fixture (a test holding a `*_words` function that compiles a shader and
returns its emitted words), wherever under this folder the diff puts it, answers to
`spirv/REVIEW.md`.**

**A test that runs a class carrying a `[cuda_kernel]` method or calls `with_cuda_context`, or a
CUDA C emitter fixture (a test that compiles a kernel and checks the emitted CUDA C), wherever
under this folder the diff puts it, answers to `cuda/REVIEW.md`.**

**A test that calls `tick_debug_agent`, wherever under this folder the diff puts it, answers to
`debug_agent/REVIEW.md`.**

**A test that asserts on `require` path lookup, module compile order, or files under the AST
module cache folder (`.cache/daslang/module_cache/`) lives in `module_cache/`, or in a
`module_cache/` directory within the tool it tests, and answers to `module_cache/REVIEW.md`
wherever under this folder the diff puts it.**

**A `[test]` a diff adds goes into an existing file of its topic and fence signature (the
file's program-changing `options`) when the folder holds one; a file the diff creates for it
is a category file of three or more tests on a topic no file in the folder holds, the
`uncategorized_<signature>.das` of a fence signature the folder has no file for
(`ARCHITECTURE.md#fence-signature`), the next `uncategorized..._<N>.das` after a file at the
cap, one of the files of a category file split at the cap with three or more tests each, or a
file whose tests carry `standalone` - any other new file is a defect
(`ARCHITECTURE.md#file-shape`).**

**A diff that adds a `[test]` to one of a folder's uncategorized files, making three or more
tests there on one topic, moves those tests into a category file, new or existing, in the same
change.**

**A `[test]` description that says the function's name in other words, or names what is under
test without saying what holds, is a defect - write the claim the test proves
(`ARCHITECTURE.md#test-description`).**

**A `[test(standalone = "...")]` whose reason names neither a declaration of this test that
changes or breaks the other tests compiled into its program, nor code in another test that
would hide what this test checks, nor a test or tool that consumes the file itself as a unit,
is a defect - the test goes to its topic's file or to the uncategorized file of its fence
signature (`ARCHITECTURE.md#standalone`).** A test that differs from its neighbours only by
its `options` goes to the `uncategorized_<signature>.das` of its fence signature.

**A diff that adds or changes a `[test]` that builds, spawns a child or waits on a peer gives it
`timeout = 1200`, the suite default, never a number measured on the author's box
(`ARCHITECTURE.md#test-timeout`).** CI runs several times slower than a workstation, so an
estimate is a flake; the budget's job is to lift the file's caps, and the suite's own `--timeout`
still ends a hang.

**A diff that weakens a check of `REVIEW.das`, or the `check_tests_index` target, is a
defect.** The checks hold the tree to `ARCHITECTURE.md#run-file`, `ARCHITECTURE.md#file-classes`
and `ARCHITECTURE.md#file-shape`.

**A diff that adds or changes a test that writes a file or directory - itself, through any
function it calls (daslang or C++-bound), or through a program or server it starts, including
under a working directory or project root it passes that program - writes it under
`test_temp_dir()` (`dastest/testing_boost`, repo root) or a directory from
`create_temp_directory` (`daslib/fio`, repo root); a write anywhere else, the system temp folder
`temp_directory()` returns included, is a defect (`ARCHITECTURE.md#test-writes`).** Two runs of
one test file can share the tree at once, and only those two directories are one run's own.
The reviewer checks every write `REVIEW.das` does not flag.

**A diff that adds or changes a test that starts a daslang child passes it `-no-module-cache`
or `-module-cache <dir>/...`, and for a `-jit` child also `-jit-no-cache` or `-output <dir>/...`,
or starts it in a working directory inside `<dir>`, where `<dir>` is `test_temp_dir()` or a
`create_temp_directory` directory.** A child writes its front-end module cache and its JIT DLL
cache relative to its working directory otherwise, which is the tree two concurrent runs share.

**A diff that widens the `dasbind` skip in `.das_test` or drops one of the `[extern]` probe
functions or one of their assertions from
`dasbind/uncategorized_no_aot_rtti_strict_smart_pointers_false.das` is a defect.** This file is the
only check of which register or stack slot an interpreted `[extern]` call puts each argument
in - the JIT never takes that path - so losing that coverage fails no CI job.

**A diff that changes a `.das` file under `tests/` (repo root) that an `AOT_*_FILES` variable of
`tests/aot/CMakeLists.txt` lists, or that sits directly in a `tests/<name>/` folder
`DAS_AOT_SUITES` names, and that is not marked `options no_aot`, names in the PR body the run
that showed `test_aot` pass: a local
`daslang utils/internal/preflight/main.das -- --full` (repo root), a `--target run_tests_aot`
build, or a CI job link (`ARCHITECTURE.md#test-aot`).** Per-PR CI builds only the AOT subset
binary (the test_aot_subset target), so the PR's own checks do not prove `test_aot`.

**A diff that takes a `.das` file under `tests/` (repo root) out of what `test_aot` compiles or
runs - `options no_aot` in the file, a filter on an `AOT_*_FILES` variable of
`tests/aot/CMakeLists.txt`, a `--use-aot` skip of its folder in `tests/.das_test`, a move of
the file into a subfolder of a `DAS_AOT_SUITES` folder, or a new `tests/<name>/` folder whose
files neither `DAS_AOT_SUITES` nor any `AOT_*_FILES` variable compiles - states beside the
opt-out (for a folder, beside the `DAS_AOT_SUITES` list) what stops
`test_aot` compiling or running the file, why an AOT build of it tests nothing, or which other
`AOT_*_FILES` variable compiles it instead.** Without a stated reason no later diff can tell
whether the opt-out is still needed.
