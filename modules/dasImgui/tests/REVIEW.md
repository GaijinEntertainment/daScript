# dasImgui Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`. A grammar-drift fixture — a test whose text the vendored tree-sitter grammar must
parse (`test_grammar_canary.das`) — applies `tree-sitter-daslang/REVIEW.md` (repo root) too.

**Tests are kept multiplatform** — no hardcoded platform-specific paths, path separators, or
OS-only assumptions; locations resolve via `get_das_root()`, never absolute or machine-local
paths.
