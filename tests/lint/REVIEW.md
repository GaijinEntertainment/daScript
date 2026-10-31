# Lint tests review checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../README.md`.

**Changes to `test_nolint_suppression.das` and `test_stale_nolint.das` retain assertions that
string literals, URLs and mid-comment `nolint:` do not suppress warnings, first-token
directives after `//` or `//!` do, and `lint-skip-file` outside the leading header is prose.**

**Changes to `test_lint_config.das` retain assertions that each `[format]` key resolves
nearest-wins independently up to `.git`; setting one key does not reset another.**
