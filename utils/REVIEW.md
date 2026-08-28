# Utils Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md` (repo root).

**A file under `utils/` that belongs to a tool other than the one owning the directory it
sits in is reviewed with that tool's own `REVIEW.md`, where one exists, as well as with this
checklist - not with the checklist of the directory it sits in. A file in a `utils/` library
directory (`common/`) is reviewed with this checklist and with the checklist of every tool
that requires it.** A tool's file OUTSIDE `utils/` answers to the checklist of the folder that
contains it.

**A diff that changes the consent wording in `watchdog/watchdog.py` answers to
`modules/dasLLAMA/performance/REVIEW.md` (repo root) too.**

**Weakening `REVIEW.das` (beside this file) is a defect.** What the gate checks is read from the
script itself, and each check's finding text states its rule.

**A diff that drops a tool from `DAS_UTILS_SHIPPED_EXES` (`CMakeLists.txt`, beside this file)
while keeping that tool's directory records the decision to stop shipping it in that tool's
own `REVIEW.md`, wherever that directory is, in the same change** - the gate cannot see a
removed entry.

**A diff that deletes a tool outright records the decision beside `DAS_UTILS_SHIPPED_EXES` in
`CMakeLists.txt` (beside this file), in the same change.**

**A new or changed test for a `utils/` tool whose load-bearing assertions a CI lane can run -
the assertions that prove the change, not a skip-path assertion - ships with a CI row that
executes those assertions, wherever the diff puts the test, added in the same change if no
row already covers it.** A row that only compile-checks the test (`dastest --compile-only`)
does not execute them. A test whose assertions no row executes never runs again.

**A new or changed test for a `utils/` tool whose load-bearing assertions no CI lane can run
ships with a CI row that compile-checks it.**

**A new or changed test for a `utils/` tool that gets a compile-only row records its executed
run in the PR description**: the machine the assertions ran on, what that machine had that CI
lacks, and the pass count.
