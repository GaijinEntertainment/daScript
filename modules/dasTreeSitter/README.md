# dasTreeSitter

`dasTreeSitter` provides view-independent UTF-8 syntax tokens for daslang, C,
C++, and Markdown. One embedded Tree-sitter C runtime hosts every grammar.

```das
require tree_sitter/tree_sitter

var tokens : array<SyntaxToken>
let status = syntax_highlight("cpp", "int answer() { return 42; }", tokens)
```

Source ranges are half-open UTF-8 byte ranges. Query capture names remain available
for theme-specific refinement while `SyntaxRole` provides a stable common
vocabulary. The native layer resolves overlapping query captures before they
cross into daslang.

Language aliases include `das`, `daslang`, `c`, `cpp`, `c++`, `hpp`, `md`, and
`markdown`.
