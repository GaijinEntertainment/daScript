# daslib architecture notes - style_lint

Companion to `ARCHITECTURE.md` in this folder; section numbers are unique across the family. The rest of the lint family - perf_lint, lint_config, lint - is `ARCHITECTURE_LINT.md`.

## 4. style_lint

- **Lint runs under policies that skip const-folding and leave some types unset.** Take a
  variable's type from `Variable._type`, not an `ExprVar`'s; expect `T('c')` to stay an
  `ExprCall`; expect `-1` as an `ExprOp1` over a constant; when a type is missing, bias
  conservative ("needs unsafe").
- **Rules whose forms are identical in the AST read the source span instead** - `defer <|`,
  generators, STYLE003's `$()`, STYLE028's three invoke spellings. STYLE005 uses LineInfo
  identity: the parser gives braceless and postfix forms a synthetic block sharing the
  terminator's LineInfo, so `blk.at != inner.at` <=> the user wrote braces. Both of these
  tests - the source-span read and the LineInfo identity check - are the rule's only
  evidence for the distinction, and both break silently when lowering or LineInfo
  assignment changes.
- **STYLE024/025 mirror the compiler's unsafe rules with a deliberate asymmetry: in doubt,
  mark "needs unsafe".** A false "needs" costs a missed note; a false "redundant" tells
  the user to delete a wrap the compiler requires. `let v & = unsafe(expr)` is the one
  recorded exception - the wrap is required by the reference binding, not the inner
  expression - and a block whose only unsafe need is such a binding is not redundant
  either (the frame's let-ref mark holds STYLE024 off). STYLE025 narrows only when the
  single statement has an expression form.
- **A require counts as used when its symbols cannot be traced, not only when seen.**
  Generic instances attribute to `fromGeneric._module`; bare names in never-instantiated
  generic bodies keep their providers alive; re-export chains are followed; a module
  providing any compile-time macro or lifecycle hook is used - it leaves no AST reference.
  STYLE029 fires only when exactly one re-exported module is used: two or more is an
  aggregation facade.
- **Metric rules attribute code to the module that can fix it.** STYLE037/038 walk the
  `fromGeneric` chain to its root (`[template]` generics instantiate INTO the caller); generated
  carriers stay unmetered; closures are not length-checked separately - the host trips first.
- **STYLE041 is sound because of a reference count, not the structural scan.** The scan
  classifies writes, plumb reads, and the single consuming `if`, arming the candidate with
  that count; `preVisitExprVar` counts every reference in the function, and the warning
  fires only when the counts match. Anything the scan does not descend into therefore
  rejects the candidate instead of being assumed harmless.
- **STYLE036 keys on a flag the compiler clears.** Infer strips a substitution contract
  when a generic binds; one still set on a resolved cast target was never consumed - but
  an `auto`/unresolved-alias target has simply not been substituted yet, hence the
  `isAutoOrAlias` bail.
- **STYLE039 requires non-ASCII in both the value and its source span** - escape-spelled bytes in
  ASCII source are log-safe, and span-slicing keeps a neighbouring comment's typography from firing.
- **The unsafe-frame walk**: `preVisitExpression` pushes one frame per expression node,
  `visitExpression` pops it - the popped frame is the subtree's summary, stored in
  `unsafeExprs` for `visitExprUnsafe` (STYLE024 vs STYLE025), propagated to the parent
  slot, and fired as STYLE024 when an `unsafe(...)` target's subtree had count 0. The
  balance panic in each entry point is the guard against a `canVisit*` override skipping a
  subtree the frame walk still counts.
- **Two compiler regimes run this visitor.** The `[lint_macro]` path runs inside a normal
  compile (const-folding ON - `T('c')` folds to a typed const); the standalone runner and
  MCP path run under lint policies (folding OFF - `T('c')` stays an `ExprCall`, `-1` stays
  an `ExprOp1`). Shape-sensitive rules carry both arms (`numeric_char_cast_target`'s
  ExprCall branch + source-span re-parse); deleting the "redundant" second arm kills the
  rule in the other regime.
- **`expr.canShadow` on a for-variable exempts it from LINT002/003/004** - the flag marks
  macro-generated loops, and lint does not judge names the user never spelled.
- **The `expect` probe scans the whole file while `lint-skip-file` is header-capped** -
  an `expect` directive anywhere makes the file a compile-error fixture (its position is
  semantic to dastest), whereas `lint-skip-file` deeper than the header would let quoted
  prose unlint a file.
- **The `blk.list` index rescans are O(block^2) by choice** - the per-statement visitor
  callback carries no index, and a shared index stack would have to push/pop with nested
  blocks; block statement lists are short.
- **Threshold provenance**: STYLE038's 80 ~ p97 of this tree's function lengths; STYLE041
  caps report-arg recursion at 8 and fails closed (deeper is unknown shape);
  `LINT_SKIP_HEADER_LINES = 16` exists so prose quoting the directive cannot unlint a
  file; STYLE037's 20 and `note_type`'s depth 24 have no recorded derivation.
- **Collapse checks partition their overlaps**: STYLE021 owns the const-key run over STYLE031,
  STYLE032 owns fresh-empty declarations over STYLE033, and STYLE033's chain-receiver check leaves
  a plain-variable receiver to STYLE033's variable-form check.
- **C++ mirror pairs**: the STYLE024/025 unsafe map <-> infer's `unsafe_*` rules;
  `style036_inert_contract` <-> infer's contract clearing. Nothing fails when one side moves alone.

### 4.1 STYLE029/030 over a require group {#style-lint-require-group}

- **The source line at a declaration's (file, line, column) key, read for the `[...]`
  spelling after the keyword and its guard, is what tells a group apart from a require written
  by hand.** A group's members share the group line's key, but a group with one member has one
  declaration, exactly like a plain require, so the key alone cannot say; both rules judge
  members and then keep, suppress and report per key.
- **A declaration's verdict is settled only after every member sharing its key has been
  judged**, so `st030_report_requires` records verdicts in one walk and reports them in a
  later one. Reporting inside the deciding walk would fire on a member judged ahead of the
  used member that keeps the line.
