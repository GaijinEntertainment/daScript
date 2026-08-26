# Utils Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md` (repo root).

**A tool's own files answer to that tool's `REVIEW.md` where one exists, keyed by what the file
IS, not where it sits: a file belonging to a tool is reviewed with that tool's checklist as well
as this one.**

**Weakening `REVIEW.das` (beside this file) is a defect.** The gate machine-checks this
folder's mechanical rules - what it enforces is read from it, each check's finding text
stating its rule.

**A tool REMOVED from `DAS_UTILS_SHIPPED_EXES` (`CMakeLists.txt`, beside this file) records
the decision in that tool's own `REVIEW.md`, wherever the tool's directory sits, in the same
change** - a removed entry's decision record is the one thing the gate cannot see. A tool
deleted outright records it beside the list in `CMakeLists.txt` instead.

**A new or changed test for a `utils/` tool whose load-bearing assertions a CI lane can run -
the assertions that prove the change, not a skip-path assertion - is covered, wherever the
diff puts it, by a CI row that executes them, added in the same change if none already
covers it.** A row that only compile-checks the test (`dastest --compile-only`) does not
execute them, and a test whose assertions no row runs passes review once and never runs
again.

**A test for a `utils/` tool whose load-bearing assertions no CI lane can run - they need
something no CI machine has - ships with a CI row that compile-checks it.**

**A new or changed test for a `utils/` tool that takes a compile-only row ships with its
executed run recorded in the PR description**: the machine the assertions ran on, what that
machine had that CI lacks, and the pass count.
