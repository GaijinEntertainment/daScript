# tree-sitter-daslang — the daslang grammar for tree-sitter

`grammar.js` is the source; `tree-sitter generate` writes the tracked artifacts `src/parser.c`,
`src/grammar.json`, `src/node-types.json` and `src/tree_sitter/parser.h` (the ABI header — it
changes only on a CLI version bump, and stays tracked because `parser.c` must pair with the
header it was generated against); `src/scanner.c` is hand-written. Every consumer gets its parser
from `src/`: the `tree_sitter_daslang` shared library (`daslang.dylib` / `.dll` / `.so` beside
this file, gitignored — what ast-grep and the MCP `grep_usage` / `outline` tools load through
`sgconfig.yml`), and the `dasTreeSitter` module (`modules/dasTreeSitter`, which embeds the
grammar for the editor: folds and outline from the parse tree, highlights from
`queries/highlights.scm`), which `daslang` and `daslang-live` load. A `grammar.js` change that is
not regenerated ships a stale parser to all of them.

The oracles: `test/corpus/*.txt` is the `tree-sitter test` corpus — one case per construct with
its expected S-expression (`tree-sitter parse <file>` prints the tree to copy from);
`modules/dasImgui/tests/test_grammar_canary.das` is the drift canary — one tricky construct per
section, each capped by a marker function; a tree-sitter error region swallows every construct
after it, so a missing marker fold names the section that broke; `test_batch.sh` parses every
`.das` under `tutorials/ tests/ examples/ benchmarks/` and reports files with error nodes. The
corpus pins the tree; the canary pins that the editor still sees the file after it. `REVIEW.das`
beside this file regenerates the grammar in a scratch copy, diffs the four artifacts, and runs
the corpus (needs the tree-sitter CLI at the version that generated `src/` — a different
generator rewrites `parser.c` and `parser.h`, so the byte-diff goes red on version skew; the
pinned version is the CI workflow's install step).

`zed-daslang/` is the Zed extension: it pins this grammar by git `rev` in `extension.toml` and
Zed builds it from GitHub at that rev, so a grammar change reaches Zed only when the rev is
bumped. `outline_rules.yml` / `cpp_outline_rules.yml` are the ast-grep outline rules the MCP
tools read.
