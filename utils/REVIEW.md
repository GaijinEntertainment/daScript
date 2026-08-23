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

**A new or changed test under a `utils/` tool whose load-bearing assertions CI can run - the
assertions that prove the change, not a skip-path assertion - ships in the same change with
the CI row that runs them**: a row in `.github/workflows/extended_checks.yml` (repo root), a
`DAS_UTILS_TO_TEST` entry in `CMakeLists.txt` (beside this file) that the `run_utils_tests`
row builds, or the tool's own workflow for a browser suite. A row that only compile-checks
the test (`dastest --compile-only`) is not that row. A test whose assertions no row runs
passes review once and never runs again.

**A test whose load-bearing assertions CI cannot run - they need an artifact the lane lacks:
a model file, a GPU, a network service - has a compile-only row in
`.github/workflows/extended_checks.yml` (repo root) or the tool's own workflow.**

**A new or changed test that takes a compile-only row ships with its executed run recorded
in the PR description**: the machine the assertions ran on, the gate it satisfied (the model
file, GPU, or service present), and the pass count.
