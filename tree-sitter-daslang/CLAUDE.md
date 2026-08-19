# tree-sitter-daslang — the daslang grammar for tree-sitter

`grammar.js` is the source; `tree-sitter generate` writes the tracked artifacts `src/parser.c`,
`src/grammar.json`, `src/node-types.json` (`src/scanner.c` is hand-written). Three consumers
compile `src/` in: the `tree_sitter_daslang` shared library (`daslang.dylib` / `.dll` / `.so`
beside this file, gitignored — what ast-grep and the MCP `grep_usage` / `outline` tools load
through `sgconfig.yml`), the `dasTreeSitter` module (`modules/dasTreeSitter`, which embeds the
grammar for the editor: folds, outline, highlights from `queries/`), and through it `daslang`
and `daslang-live`. CMake rebuilds all three from `src/` — a change to `grammar.js` that is not
regenerated ships a stale parser to every consumer.

Two oracles guard the grammar. `test/corpus/*.txt` is the `tree-sitter test` corpus — one case
per construct with its expected S-expression (`tree-sitter parse <file>` prints the tree to
copy from). `modules/dasImgui/tests/test_grammar_canary.das` is the drift canary: one tricky
construct per section, each capped by a marker function; a tree-sitter error region swallows
every construct after it, so a missing marker fold names the section that broke. New syntax
lands in both — the corpus pins the tree, the canary pins that the editor still sees the file
after it. `REVIEW.das` beside this file regenerates the grammar in a scratch copy, diffs the
three artifacts, and runs the corpus (needs `tree-sitter-cli` on PATH; CI installs it).

`zed-daslang/` is the Zed extension packaging of the same grammar; `outline_rules.yml` /
`cpp_outline_rules.yml` are the ast-grep outline rules the MCP tools read.
