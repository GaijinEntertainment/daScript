# tree-sitter-daslang Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`skills/mcp_tools.md`.

## Grammar canary — drift contract

**Any change to `grammar.js` regenerates every tracked artifact `tree-sitter generate` writes
(`src/parser.c`, `src/grammar.json`, `src/node-types.json`), rebuilds all three consumers** (the
tree_sitter_daslang shared module/DLL, `daslang`, `daslang-live`) **and shows a green
`modules/dasImgui/tests/test_grammar_canary.das` — in the same change.** "It still parses" is not
the gate; the canary is.

**New syntax (`src/parser/ds2_parser.ypp` or `grammar.js`) lands with a new canary section in
`modules/dasImgui/tests/test_grammar_canary.das` in the same PR** — the canary only protects
syntax it pins.

**A red canary names the section that broke. Never ship around it by deleting or loosening
sections.**
