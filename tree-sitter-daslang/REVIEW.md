# tree-sitter-daslang Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`.

**A diff that changes what `grammar.js` or `src/scanner.c` accepts adds a `test/corpus/` case
for it, in the same change.**

**A diff that adds syntax `grammar.js` or `src/scanner.c` accepts adds a `CanarySection` entry
to `CANARY_SECTIONS` in `modules/dasImgui/tests/test_grammar_canary.das` (repo root), in the
same change.**
