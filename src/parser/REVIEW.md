# Parser Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`skills/internal/cpp_codebase_notes.md` (repo root).

**New surface syntax in `ds2_parser.ypp` ships the matching `tree-sitter-daslang/grammar.js`
rule (repo root; regenerated), its `tree-sitter-daslang/test/corpus` case, and a section in
`modules/dasImgui/tests/test_grammar_canary.das`.** The editor and the MCP search tools parse
with the tree-sitter grammar, not with bison — syntax the grammar lacks disappears from folds,
outline and `grep_usage` the moment it is used.
