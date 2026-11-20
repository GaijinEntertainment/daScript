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

**A test that requires a `modules/dasCUDA` module (`require cuda/...`), runs a class carrying a
`[cuda_kernel]` method or calls `with_cuda_context`, or a CUDA C emitter fixture (a test that
compiles a kernel and checks the emitted CUDA C), wherever under this folder the diff puts it,
answers to `cuda/REVIEW.md`.**

**A test that calls `tick_debug_agent`, wherever under this folder the diff puts it, answers to
`debug_agent/REVIEW.md`.**

**A test that asserts on `require` path lookup, module compile order, or files under the AST
module cache folder (`.cache/daslang/module_cache/`) lives in `module_cache/`, or in the
`utils/<tool>/tests/module_cache/` (repo root) of the tool it tests, and answers to
`module_cache/REVIEW.md` wherever under this folder the diff puts it.**

**A `[test]` without `standalone` that a diff adds goes into the folder's file of its topic
and fence signature (the file's program-changing `options`,
`ARCHITECTURE.md#fence-signature`).**

**A run file (a `.das` file whose name and every directory on whose path do not start with
`_`, other than `REVIEW.das`; `ARCHITECTURE.md#run-file`, `ARCHITECTURE.md#file-classes`) a diff
creates for a `[test]` holds a topic or fence signature no other file in the folder holds, or
holds tests that carry `standalone` (`ARCHITECTURE.md#file-shape`).**

**A diff that adds a `[test]` to one of a folder's uncategorized files, making three or more
tests there on one topic, moves those tests into a category file, new or existing, in the same
change.**

**A `[test]` description that says the function's name in other words, or names what is under
test without saying what holds, is a defect - write the claim the test proves
(`ARCHITECTURE.md#test-description`).**

**A `[test(standalone = "...")]` reason names one of three things: a declaration of this test
that changes or breaks the other tests compiled into its program, code in another test that
would hide what this test checks, or a test or tool that consumes the file itself as a unit
(`ARCHITECTURE.md#standalone`).** Any other reason sends the test to its topic's file, or - when
it differs from its neighbours only by its `options` - to the `uncategorized_<signature>.das` of
its fence signature.

**A diff that adds or changes a `[test]` that builds (compiles a daslang program), spawns a
child (starts a process) or waits on a peer (blocks on another process or a socket) gives it
`timeout = 1200`, the suite default, never a number measured on the author's box
(`ARCHITECTURE.md#test-timeout`).** CI runs several times slower than a workstation, so an
estimate is a flake.

**A diff that weakens any check of `REVIEW.das` is a defect.**

**A diff that adds or changes a test that writes a file or directory - itself, through any
function it calls (daslang or C++-bound), or through a program or server it starts, including
under a working directory or project root it passes that program - writes it under
`test_temp_dir()` (`dastest/testing_boost`, repo root) or a directory from
`create_temp_directory` (`daslib/fio`, repo root); a write anywhere else, the system temp folder
`temp_directory()` returns included, is a defect (`ARCHITECTURE.md#test-writes`).** Two runs of
one test file can share the tree at once, and only those two directories are one run's own.
The reviewer checks every write `REVIEW.das` does not flag.

**A diff that adds or changes a test that starts a daslang child either starts it in a working
directory inside `<dir>`, or passes it `-no-module-cache` or `-module-cache <dir>/...` when it
runs with none of `-exe`, `-compile-only`, `-documentation`, `-ser`, `-deser`,
`--das-wait-debugger` and `-use-aot` (such a run compiles through the front-end module cache),
and passes a `-jit` child `-jit-no-cache` or `-output <dir>/...` - where `<dir>` is
`test_temp_dir()` or a `create_temp_directory` directory.** A child writes its front-end module cache and its JIT DLL cache relative to its
working directory otherwise, which is the tree two concurrent runs share.

**A diff that adds or changes a test under `tests/language/` that reads a file by a path built
from `get_das_root()` skips on `get_running_platform_name() == "emscripten"`, or hands
`compile_file` its source with `set_file_source`.** The web lane's das root does not hold the repo
(`ARCHITECTURE.md#web-lane`). `REVIEW.das` reports only a `compile_file` in a file with no
emscripten skip anywhere; the reviewer checks every other read.

**A diff that widens the `dasbind` skip in `.das_test` or drops one of the `[extern]` probe
functions or one of their assertions from
`dasbind/uncategorized_no_aot_rtti_strict_smart_pointers_false.das` is a defect.** This file is the
only check of which register or stack slot an interpreted `[extern]` call puts each argument
in - the JIT never takes that path - so losing that coverage fails no CI job.

**A diff that changes a `.das` file under `tests/` (repo root) that `test_aot` compiles - one an
`AOT_*_FILES` variable of `tests/aot/CMakeLists.txt` holds after its filters, or a top-level
file of a `DAS_AOT_SUITES` folder no variable covers, neither marked `options no_aot` nor named
`failed_*`, `cant_*` or `invalid_*` - names in the PR body the run that showed `test_aot` pass:
a local `daslang utils/internal/preflight/main.das -- --full` (repo root), a
`--target run_tests_aot` build, or a link to a `nightly` workflow run whose Nightly Tests step
passed (`ARCHITECTURE.md#test-aot`).** Per-PR CI builds only the AOT subset binary (the
test_aot_subset target), which compiles `tests/language` alone against a smaller set of `daslib`
modules, so the PR's own checks do not prove `test_aot`.

**A diff that adds or changes a `.das` file under `tests/` (repo root) whose name starts with none
of `_`, `failed_`, `cant_`, `invalid_`, that `test_aot` does not compile, or compiles and does not
run, states what would stop an AOT build compiling or running the file, or, when nothing would, why
an AOT build of it tests nothing: in the file for `options no_aot`; beside the `DAS_AOT_SUITES` list
for a file whose folder that list leaves out or that sits in a subfolder; beside the `AOT_*_FILES`
filter in `aot/CMakeLists.txt` or the `--use-aot` skip in `.das_test` otherwise
(`ARCHITECTURE.md#test-aot`).** Without a stated reason no later diff can tell whether the opt-out
is still needed.
