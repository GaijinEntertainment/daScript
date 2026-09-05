# Utils Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md` (repo root).

A tool is a directory that owns one program - its entry point and the files only that program
uses - under `utils/`, or outside `utils/` when `CMakeLists.txt` (beside this file) builds or
ships it. A test's load-bearing assertions are the ones that prove the change, never a
skip-path assertion. A CI row is a workflow step whose command reaches the test.

**A file under `utils/` that belongs to a tool other than the one owning the directory it
sits in is reviewed with that tool's own `REVIEW.md`, where one exists, as well as with this
checklist - not with the checklist of the directory it sits in.**

**A diff under `utils/` that changes how a `.dlim` is built from a gguf, how one is loaded, or
what identifies one - the fields that decide whether two `.dlim`s are the same image - answers
to `modules/dasLLAMA/REVIEW.md` (repo root) too.** A `utils/` diff never opens that checklist
on its own.

**Weakening `REVIEW.das` (beside this file) is a defect.** What the gate checks is read from the
script itself, and each check's finding text states its rule.

**A diff that drops a tool from `DAS_UTILS_SHIPPED_EXES` (`CMakeLists.txt`, beside this file)
while keeping that tool's directory records the decision to stop shipping it in that tool's
own `REVIEW.md`, wherever that directory is, in the same change** - `REVIEW.das` cannot see a
removed entry.

**A diff that deletes a tool named in `DAS_UTILS_SHIPPED_EXES` outright removes its entry and
leaves a comment line beside the list naming the removed exe, in the same change.** A tool the
list never carried leaves no record.

**A test the diff adds or changes that covers a change under `utils/`, whose load-bearing
assertions a CI row can run against the change, ships with a CI row that executes those
assertions, wherever the diff puts the test, added in the same change if no row already covers
it.** A row that only compile-checks the test (`dastest --compile-only`) does
not execute them. A test whose assertions no row executes never runs again.

**A test the diff adds or changes that covers a change under `utils/`, whose load-bearing
assertions no CI row can run for want of hardware or data, ships with a row that compile-checks it - `dastest
--compile-only` for a `.das` test - in the same change.**

**A test the diff adds or changes that covers a change under `utils/`, whose load-bearing
assertions no CI row can run for want of hardware or data, records in the PR description an
executed run against the build the diff produces**: the machine the assertions ran on, what
that machine had that CI lacks, and the pass count. A run against an already-deployed artifact
proves nothing about the diff under review.
