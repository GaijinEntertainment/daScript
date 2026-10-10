# CI Scripts Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `packaging/README.md`.

**A diff that shrinks what `smoke_test_bundle.sh`, or any checker it runs, rejects is a defect -
fix the flagged file instead**: every bundle it failed before the diff still fails.

**A diff that adds a skip to `smoke_test_bundle.sh` or a checker it runs - an `--exclude`
pattern, or a `COMPILE_TESTS` or `SHIPPED_EXE_TESTS` row that names a module a bundle may lack
(that bundle skips the row) - for a file the same diff adds states in the PR body why that file
cannot be fixed.**

**A diff after which `tree_sitter_parse_sources.sh` skips a gen2 file that it parsed before and
that `daslang -dry-run` does not reject with an `error[1xxxx]` code or a `syntax error` message
is a defect** - fix `tree-sitter-daslang/grammar.js` (repo root) instead.

**Weakening `test_ci_matrix.py` - dropping or loosening any assertion it makes - is a
defect.** Those assertions are what turns a job or step that stopped running per PR into a red
test.

**Weakening `REVIEW.das` (beside this file) is a defect: dropping a check, narrowing what a check
walks, or rewriting a finding text so it no longer names what failed.**
