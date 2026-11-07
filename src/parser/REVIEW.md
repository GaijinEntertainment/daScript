# Parser Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`skills/internal/cpp_codebase_notes.md` (repo root). A diff that adds, removes or renames a token
in `ds2_lexer.lpp` or an assignment rule in `ds2_parser.ypp` applies `daslib/REVIEW.md` (repo
root) too - the formatter and the AST printer carry their pairs (`daslib/ARCHITECTURE_SYNTAX.md`).

**A diff that lets `ds2_parser.ypp` or `ds2_lexer.lpp` accept new syntax also makes
`tree-sitter-daslang` (repo root) parse it, in the same change - in
`tree-sitter-daslang/src/scanner.c` for a token listed under `externals` in
`tree-sitter-daslang/grammar.js`, else in `grammar.js`.** The editor and the MCP search tools
parse with tree-sitter, not bison, so syntax it lacks is missing from folding, outline and
`grep_usage`.
