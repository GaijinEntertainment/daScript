# Parser Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `src/ast/ARCHITECTURE.md` (the require forms, sec.2), `skills/internal/cpp_codebase_notes.md`.

**A diff that lets a grammar or lexer source in this folder accept new syntax - the gen2 pair
`ds2_parser.ypp` / `ds2_lexer.lpp`, or the gen1 pair `ds_parser.ypp` / `ds_lexer.lpp` - also
adds the matching rule to `tree-sitter-daslang/grammar.js` (repo root), in the same change.**
The editor and the MCP search tools parse with the tree-sitter grammar, not with bison, so
syntax missing from `grammar.js` does not appear in code folding, outline or `grep_usage`.

**A diff that lets a grammar or lexer source in this folder accept new syntax also adds a
section exercising it to `modules/dasImgui/tests/test_grammar_canary.das` (repo root), in the
same change.** The canary reds only for syntax it already carries, so syntax it lacks can drift
from the tree-sitter grammar with no test to say so.
