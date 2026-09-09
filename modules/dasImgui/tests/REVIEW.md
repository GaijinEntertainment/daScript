# dasImgui Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`. A grammar-drift fixture - a test that asserts the vendored tree-sitter grammar
parses a pinned set of constructs (`test_grammar_canary.das`) - applies
`tree-sitter-daslang/REVIEW.md` (repo root) too.

**Never hardcode a platform-specific path, a path separator, or an OS-only assumption in a
test** - resolve every location through `get_das_root()`, never an absolute or machine-local
path.
