# Parser Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`skills/internal/cpp_codebase_notes.md` (repo root).

**A diff that lets `ds2_parser.ypp` or `ds2_lexer.lpp` accept new syntax also adds the matching
rule to `tree-sitter-daslang/grammar.js` (repo root), in the same change.** The editor and the
MCP search tools parse with the tree-sitter grammar, not with bison, so syntax missing from
`grammar.js` does not appear in code folding, outline or `grep_usage`.
