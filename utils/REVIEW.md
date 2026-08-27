# Utils Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md` (repo root).

**A file that belongs to a tool is reviewed with that tool's own `REVIEW.md`, where one exists,
as well as with this checklist - the tool it belongs to decides, not the directory it is in.**

**Weakening `REVIEW.das` (beside this file) is a defect.** What the gate checks is read from the
script itself, and each check's finding text states its rule.

**A tool removed from `DAS_UTILS_SHIPPED_EXES` (`CMakeLists.txt`, beside this file) records
the decision in that tool's own `REVIEW.md`, wherever that tool's directory is, in the same
change** - the gate cannot see a removed entry. A tool deleted outright records the decision
beside the list in `CMakeLists.txt` instead.

**A new or changed test for a `utils/` tool whose load-bearing assertions a CI lane can run -
the assertions that prove the change, not a skip-path assertion - ships with a CI row that
executes those assertions, wherever the diff puts the test, added in the same change if no
row already covers it.** A row that only compile-checks the test (`dastest --compile-only`)
does not execute them. A test whose assertions no row executes never runs again.

**A test for a `utils/` tool whose load-bearing assertions no CI lane can run - they need
something no CI machine has - ships with a CI row that compile-checks it.**

**A new or changed test for a `utils/` tool that gets a compile-only row records its executed
run in the PR description**: the machine the assertions ran on, what that machine had that CI
lacks, and the pass count.
