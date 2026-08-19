# tree-sitter-daslang Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`CLAUDE.md`.

**`grammar.js` and the tracked artifacts `src/parser.c`, `src/grammar.json`, `src/node-types.json`
change together: `tree-sitter generate` on the tree writes nothing new.** Weakening `REVIEW.das`
(beside this file), which checks exactly that and runs the corpus, is a defect.

**A `grammar.js` change ships a `test/corpus` case for the syntax it adds or changes, and
`tree-sitter test` is green.**

**A `grammar.js` change ships a green `modules/dasImgui/tests/test_grammar_canary.das` (repo root)
with a section for the syntax it adds** — the canary only protects syntax it pins.

**A red canary is repaired in `grammar.js` and regenerated — never by deleting or loosening a
canary section.**
