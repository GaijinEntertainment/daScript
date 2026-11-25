# Utils Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

Terms. A tool is a directory holding a program - its entry file sits in the directory, or in a child
folder of it when the directory has a `README.md` - and the files only that program uses, when
`CMakeLists.txt` (beside this file) or the repo root `CMakeLists.txt` builds or ships the program, when
`CMakeLists.txt` (beside this file) runs a `DAS_UTILS_TO_TEST` suite against it, or, for a directory
under this folder, when its `.das_package` declares the program with `release_main` or
`release_program` or a CMake target or `.github/workflows/` step (repo root) starts it as its command's
program (`ARCHITECTURE.md#tools`). A change to a tool is a change to one of its sources: a file under its
directory, a `common/` module it requires, or a module named in the `RULE_MODULES` list of its
directory's `REVIEW.das` (`ARCHITECTURE.md#tool-sources`). An arm is one test case a runner reports pass
or fail for on its own - a `dastest` `[test]` and each `t |> run(...)` case in it, a compile-fail
fixture with an `expect` line, a JavaScript runner's `test(...)` case; its load-bearing assertions are
the ones that prove the change, never one that runs only when the arm skips (`ARCHITECTURE.md#arms`). A
CI row is a workflow step that runs the arm, directly or through a process it spawns; a pull-request
lane is a CI job run on every pull request. An assertion no CI row can run needs one of these and
nothing else: a GPU, a host tool no pull-request image carries, a network service, a credential, or a
model or data file neither the checkout nor the build provides. One arm can hold assertions of both
kinds, and each rule below binds the assertions of its own kind (`ARCHITECTURE.md#ci-rows`).

**A changed file that belongs to a tool, wherever the tool sits, is reviewed with that tool's
own `REVIEW.md`, where one exists, as well as with this checklist.**

**A diff that adds a `[test]` covering a change to a tool puts it under the tool's directory, in
the tool's existing category file for the topic or in that directory's `uncategorized.das`,
never in a new file; a test that calls a `common/` module directly goes under `common/`, even
when it also runs a tool, one that tests a `common/` change by running a tool that requires it
goes under that tool, and a test covering two tools goes under either one.** The shape rules -
three or more tests per category file, one `uncategorized.das` per directory, a description
on every test, `timeout = 1200` on a test that builds, spawns or waits - are `tests/REVIEW.md`
(repo root) and `tests/ARCHITECTURE.md`; `REVIEW.das` (beside this file) applies them to every
suite under this folder in tests-only mode, where a `.das` with neither `[test]` nor `expect`
is a tool's source or a fixture its suite feeds it.

**A diff that changes how a tool builds a `.dlim` from a `.gguf` model file, how one loads it,
or what identifies one - the fields that decide whether two `.dlim`s are the same image -
answers to `modules/dasLLAMA/REVIEW.md` (repo root) too.**

**A diff touching a file under this folder that carries `options _dasllama_internal` applies
`modules/dasLLAMA/REVIEW.md` (repo root) too.**

**A `[test]` file under this folder with a `require dasllama/...` line of its own answers to
`modules/dasLLAMA/REVIEW.md` (repo root) too.**

**Weakening `REVIEW.das` (beside this file) is a defect: dropping a check, narrowing what a check
walks, or rewriting a finding text so it no longer names what failed.**

**A diff that drops a tool from `DAS_UTILS_SHIPPED_EXES` (`CMakeLists.txt`, beside this file)
while keeping that tool's directory records the decision to drop its named exe in that tool's
own `README.md`, wherever that directory is, in the same change** - `REVIEW.das` cannot see a
removed entry, and it reads the `README.md` of each `DAS_UTILS_SHIPPED_EXES` entry for the
standing decision.

**A diff that deletes a tool named in `DAS_UTILS_SHIPPED_EXES` leaves a comment line beside
the list naming the removed exe, in the same change.**

**A `daslang -exe` build of a tool that exists for the tool's tests - `bin/das-lint.exe`, which
`lint/tests/_lint_cli.das` spawns when a fresh one is there - never joins `DAS_UTILS_SHIPPED_EXES`,
an install rule, or a release bundle.** A standalone exe carries the modules it was built with,
and a user's tree loads its own set dynamically; the shipped form of a tool stays a copy of
`daslang` that compiles the tool at launch.

**An arm the diff adds or changes that covers a change to a tool, whose load-bearing
assertions a CI row can run against the change, ships with a CI row that executes those
assertions on every pull request, wherever the diff puts the arm, added in the same change
when no row does.** A row that only compile-checks the arm (`dastest --compile-only`) does not
execute them.

**For an arm the diff adds or changes that covers a change to a tool, whose load-bearing
assertions a CI row can run, and that skips unless a host tool is present, the PR description
names the pull-request lane whose runner image carries that tool.**

**An arm the diff adds or changes that covers a change to a tool, whose load-bearing
assertions no CI row can run, ships with a row that compile-checks the file the arm sits in -
`dastest --compile-only` - added in the same change when no row already compile-checks that
file.**

**For an arm the diff adds or changes that covers a change to a tool, whose load-bearing
assertions no CI row can run, the PR description records an executed run against the build the
diff produces (the machine the assertions ran on, why no CI row can run them, the pass count) or
cites the log of a workflow run on this diff's branch showing those assertions ran.** A run
against an already-deployed artifact proves nothing about the diff under review.

**A key a diff adds or renames in a `watchdog.json` that belongs to a tool is a
`WatchdogConfig` field in `watchdog/watchdog.das`, added in the same change when none exists.**
The supervisor refuses to start on an unknown key.

**A diff that adds a front to a tool whose program an `install(...)` rule in `CMakeLists.txt`
(beside this file or at the repo root) puts in the bundle adds a `run_check` row starting that
front to `ci/smoke_test_bundle.sh` (repo root), in the same change.** A front is a command-line
mode that makes a program serve a different role or protocol. `ci/smoke_test_bundle.sh` proves
only the fronts it starts.

**A test the diff adds or changes in a `DAS_UTILS_TO_TEST` suite (`CMakeLists.txt`, beside this
file) writes no file under a folder another line of that list names - a line naming a single
`.das` file names no folder.** Preflight's `utils-tests` lane runs every suite at once, so a file
written there lands in a sweep that is running.
