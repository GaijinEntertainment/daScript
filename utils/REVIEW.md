# Utils Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.**

A tool is a directory holding a program and the files only that program uses. A directory holds
a program when the entry file of the program sits in it, or in a child folder of it when the
directory has a `README.md`. A directory anywhere in the repo is a tool when `CMakeLists.txt`
(beside this file) or the `CMakeLists.txt` at the repo root builds or ships a program it holds,
or when `CMakeLists.txt` (beside this file) runs a `DAS_UTILS_TO_TEST` suite against that
program. A directory under this folder is also a tool when its `.das_package` declares a program
with `release_main` or `release_program`, or when a CMake target or a `.github/workflows/` step
(repo root) runs a program it holds as the program the command starts. A change under `common/`
(beside this file) is a change to every tool that requires it.

An arm is one test case that a test runner executes and reports pass or fail for on its own,
in any language: a `dastest` `[test]` function and each `t |> run(...)` case inside that
function are arms, and so is each `test(...)` case of a JavaScript runner (`node:test`,
Playwright). An arm's load-bearing assertions are the ones that prove the change, never an
assertion that runs only when the arm skips.

A CI row is a workflow step whose command runs the arm, directly or through a process it
spawns. A pull-request lane is a CI job that runs on every pull request. An assertion no CI row
can run is one that needs something no pull-request lane's runner image provides - a GPU, a host
tool the image does not carry, a network service, a credential - that list is closed; whether a
row already runs the arm does not decide it. An arm that skips unless something the build can
produce is present - a target, an exe, a generated file - has assertions a CI row can run. An
arm that skips unless a host tool is present has assertions a CI row can run when a pull-request
lane's runner image carries that tool. One arm can hold assertions of both kinds.

**A changed file that belongs to a tool, wherever the tool sits, is reviewed with that tool's
own `REVIEW.md`, where one exists, as well as with this checklist.**

**A diff that adds a `[test]` file covering a change to a tool puts that file under the tool's
directory; a test that calls a `common/` module directly goes under `common/`, even when it also
runs a tool, one that tests a `common/` change by running a tool that requires it goes under that
tool, and a test covering two tools goes under either one.**

**A diff that changes how a tool builds a `.dlim` from a `.gguf` model file, how one loads it,
or what identifies one - the fields that decide whether two `.dlim`s are the same image -
answers to `modules/dasLLAMA/REVIEW.md` (repo root) too.**

**A diff touching a file under this folder that carries `options _dasllama_internal` applies
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
