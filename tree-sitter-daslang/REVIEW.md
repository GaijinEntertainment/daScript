# tree-sitter-daslang Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`CLAUDE.md`.

**Weakening `REVIEW.das` (beside this file) is a defect** - it compares `src/parser.c`,
`src/grammar.json`, `src/node-types.json` and `src/tree_sitter/parser.h` byte for byte against a
fresh `tree-sitter generate`, and runs the tests in `test/corpus`.

**A `grammar.js` change ships a `test/corpus` case for the syntax it adds or changes.**

**A `grammar.js` change ships `modules/dasImgui/tests/test_grammar_canary.das` (repo root) passing,
with a section for the syntax it adds** - that file only protects syntax it pins.

**A failing `modules/dasImgui/tests/test_grammar_canary.das` (repo root) is fixed by changing
`grammar.js` and re-running `tree-sitter generate` - never by deleting or loosening a section of
that file.**
