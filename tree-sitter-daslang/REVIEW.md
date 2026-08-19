# tree-sitter-daslang Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`CLAUDE.md`.

**Weakening `REVIEW.das` (beside this file) is a defect** — it byte-diffs `src/parser.c`,
`src/grammar.json`, `src/node-types.json` and `src/tree_sitter/parser.h` against a fresh
`tree-sitter generate` and runs the `test/corpus` suite.

**A `grammar.js` change ships a `test/corpus` case for the syntax it adds or changes.**

**A `grammar.js` change ships a green `modules/dasImgui/tests/test_grammar_canary.das` (repo root)
with a section for the syntax it adds** — the canary only protects syntax it pins.

**A red canary is repaired in `grammar.js` and regenerated — never by deleting or loosening a
canary section.**
