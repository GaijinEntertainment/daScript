# dasImgui Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`. A grammar-drift fixture — a test that asserts the vendored tree-sitter grammar
parses a pinned set of constructs (`test_grammar_canary.das`) — applies
`tree-sitter-daslang/REVIEW.md` (repo root) too.

**Tests are kept multiplatform** — no hardcoded platform-specific paths, path separators, or
OS-only assumptions; locations resolve via `get_das_root()`, never absolute or machine-local
paths.
