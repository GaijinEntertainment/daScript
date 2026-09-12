# Utils Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md` (repo root).

A tool is a directory holding a program and the files only that program uses. A directory is
a tool when `utils/CMakeLists.txt` (beside this file) or the repo root's `CMakeLists.txt`
builds or ships a program in it, wherever that directory sits; a directory under `utils/` is
also a tool when its `.das_package` declares a program with `release_main` (its own) or
`release_program` (a companion). A change under `common/` (beside this file) is a change to
every tool that requires it. An arm is one `t |> run(...)` case of a `[test]` function. An
arm's load-bearing assertions are the ones that prove the change, never a skip-path assertion.
A CI row is a workflow step whose command runs the arm, directly or through a process it
spawns. An assertion no CI row can run is one no CI row would execute: either no CI row runs
the arm, or the arm returns or skips before the assertion. An arm that skips unless a host
tool is present is runnable when the pull-request lane's runner image carries that tool, and
the change names that lane. One arm can hold both kinds.

**A changed file that belongs to a tool, wherever the tool sits, is reviewed with that tool's
own `REVIEW.md`, where one exists, as well as with this checklist.**

**A diff that changes how a tool builds a `.dlim` from a gguf, how one loads it, or what
identifies one - the fields that decide whether two `.dlim`s are the same image - answers to
`modules/dasLLAMA/REVIEW.md` (repo root) too.**

**A file that belongs to a tool and carries `options _dasllama_internal`, wherever the diff
puts it, applies `modules/dasLLAMA/REVIEW.md` (repo root) too.**

**Weakening `REVIEW.das` (beside this file) is a defect: dropping a check, narrowing what a check
walks, or rewriting a finding text so it no longer names what failed.**

**A diff that drops a tool from `DAS_UTILS_SHIPPED_EXES` (`CMakeLists.txt`, beside this file)
while keeping that tool's directory records the decision to stop shipping it in that tool's
own `README.md`, wherever that directory is, in the same change** - `REVIEW.das` cannot see a
removed entry, and it reads each shipped tool's `README.md` for the standing decision.

**A diff that deletes a tool named in `DAS_UTILS_SHIPPED_EXES` outright removes its entry and
leaves a comment line beside the list naming the removed exe, in the same change.**

**An arm the diff adds or changes that covers a change to a tool, whose load-bearing
assertions a CI row can run against the change, ships with a CI row that executes those
assertions on every pull request, wherever the diff puts the arm, added in the same change if
no existing row already executes them on every pull request.** A row that only compile-checks
the arm (`dastest --compile-only`) does not execute them, and a nightly-only row runs them
after the merge.

**An arm the diff adds or changes that covers a change to a tool, whose load-bearing
assertions no CI row can run, ships with a row that compile-checks it -
`dastest --compile-only` - added in the same change if no row already compile-checks that
file.**

**An arm the diff adds or changes that covers a change to a tool, whose load-bearing
assertions no CI row can run, records in the PR description an executed run against the build
the diff produces - the machine the assertions ran on, why no CI row can run them, and the pass
count - or cites the log of a workflow run on this diff's branch showing those assertions ran.**
A run against an already-deployed artifact proves nothing about the diff under review.

**A diff that adds or renames a key in a `watchdog.json` that belongs to a tool, wherever the
tool sits, names a `WatchdogConfig` field in `watchdog/watchdog.das`, in the same change** -
the supervisor refuses to start on an unknown key.
