# dasImgui Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`. A grammar-drift fixture - a test that asserts the vendored tree-sitter grammar
parses a pinned set of constructs (`test_grammar_canary.das`) - applies
`tree-sitter-daslang/REVIEW.md` (repo root) too.

**Tests never hardcode machine-local host paths or host path separators.** Resolve
SDK resources through `get_das_root()` in daslang and use portable path APIs in
external drivers. HTTP URL paths and browser virtual-filesystem paths are not
host filesystem locations.
