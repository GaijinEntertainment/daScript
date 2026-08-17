# lineinfo-audit

Validates AST `LineInfo` against actual source bytes across a corpus of `.das`
files. Exists to keep ranges trustworthy for tooling that splices source text —
lint auto-fix, extract-method/refactoring in the VSCode plugin, LSP features.

```
daslang utils/internal/lineinfo-audit/main.das -- -p tests -p daslib [-s 5] [--max-files N] [-v] [--fails]
```

Compiles each file (`no_optimizations`, full macro pipeline), walks every
non-generated user function, and checks each node against the source:

| check | meaning |
|---|---|
| `anchor` | source text at `at` must equal the token the node names (var/call/field/operator/keyword/literal) |
| `bounds` | range is ordered, non-empty, inside the file |
| `colspill` | a range endpoint points past the end of its source line |
| `envelope` | expression lies inside its function's `atDecl` span |
| `block` | `ExprBlock` starts at `{` and ends at `}` |
| `enclosure` | call `atEnclosure` ends at `)` |

## Coordinate conventions (probed ground truth)

Lines are 1-based, columns 0-based byte offsets, ranges end-exclusive. Most
nodes carry a single-TOKEN anchor (`ExprOp2.at` spans just the operator,
`ExprIfThenElse.at` just the `if`); full spans exist only on `ExprBlock`,
`Function.atDecl` and `ExprLooksLikeCall.atEnclosure`. Tooling that needs a
full expression span must compute the union of subtree anchors.

## Known-legit classes the tool accepts

Macro rewrites and reification mangle names while `at` stays at the source
spelling (segment match, incl. `aka`), literal ctors anchor at `[`/`{`,
dot-sugar calls at `.`, arrow bodies synthesize `return` at `=>`, comprehension
lowering plants `let`/`if`/`return` at `for`, variant `is`/`as` lowers to `==`,
copy/move/clone promote into each other, const folding keeps the folded
expression's range, `def operator X` anchors at `operator`.

## Reading the report

Remaining violations are dominated by synthesized nodes that carry borrowed or
degenerate ranges without `genFlags.generated` — compiler desugar (`delete`
statements, lambda/generator capture machinery) and daslib macro output
(`match`, `apply`, `macro_verify`, decs `query`, `templates_boost` splices).
Those are range-consumer hazards, not parser bugs: a fix/refactor tool must
treat them as untrustworthy until they are flagged generated.
