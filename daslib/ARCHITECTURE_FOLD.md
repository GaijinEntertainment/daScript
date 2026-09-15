# daslib architecture notes - the declarative fold engine

A companion of `ARCHITECTURE.md`; section numbers are unique across the set. This file carries
`fold_rules` and `fold_rules_std`, the peephole optimizer whose rules are daslang source patterns.

## 39. fold_rules

- **The engine is an `[optimization_macro]`, not an `[infer_macro]`, because a fold rule must see
  the tree the C++ passes already reduced - constants propagated into call sites, calls inlined,
  `ExprRef2Value` turned into `r2v` flags - and at that phase there is no re-inference
  (`src/ast/ast_optimize.cpp`).** `type_rewrite` is the consequence: it resolves the operator or
  builtin for every node a rule built, mirroring `ConstFolding::findBuiltinOperator`
  (`src/ast/ast_const_folding.cpp`), and refuses anything outside constants / operators /
  pure-builtin calls rather than splice an untyped node. Nothing reports drift between the two
  resolvers: an operator the C++ side accepts and the das side rejects surfaces only as a rule that
  stops firing.
- **`FoldRules.apply` stamps `Function.optimizationRound` on every function it changed, which keeps
  the C++ passes visiting that function next round** (`PassVisitor::funcIsDirty`,
  `include/daScript/ast/ast_visitor.h`; the field is bound for this in
  `src/builtin/module_builtin_ast_annotations_1.cpp`). Without the stamp a das rewrite that leaves
  an identity shape behind is never reduced, because round N+1 skips the function as clean.
- **`options disable_algebraic_folding` gates only the identity rewrites of `ConstFolding`, never
  its constant evaluation** - the policy field in `include/daScript/simulate/code_of_policies.h` and
  the four `noAlgebraic` guards in `src/ast/ast_const_folding.cpp` are a pair. It is the parity
  switch `tests/fold_rules/test_fold_parity.das` rides, which is only meaningful while the flag
  splits exactly those two sets.
- **Every condition a rule depends on is a predicate pattern, never a test in the rule body.** A
  `_name(...)` call in a pattern is a guard on the matched node (`generate_predicate_match` in
  `daslib/ast_match.das`), so a rule whose guard fails does not match and the ladder tries the next
  one - which is what keeps one arm per rewrite. A predicate whose argument names a capture is
  DEFERRED to the end of the pattern: the generic field walk visits a node's children in annotation
  order, so the right operand can be reached before the left is staged, and `_same(x)` would
  otherwise read an unbound variable.
- **Purity splits by what the rule claims: `_droppable` licenses evaluating an expression zero
  times, `_repeatable` licenses evaluating it more than once.** They are not the same test - a pure
  but large operand is droppable and NOT repeatable, and duplicating it is a pessimization no
  correctness check would catch. `_repeatable` carries the node budget for that.
- **A rules module is self-contained: one pass-macro class, its rules inside it, no registry.**
  Macro modules compile into their own contexts, so a rule module could not push into a registry the
  engine holds; ordering across rule modules is `Module::foreach` order.
- **Every node class a pattern can root at needs its own `visitExprXxx` override in `FoldVisitor`.**
  The generic `visitExpression` hook is called for observation only and its result discarded
  (`VISIT_EXPR` in `include/daScript/simulate/aot_builtin_ast.h`), so a node class missing from that
  list is one rules silently cannot rewrite.
- **`clone` carries the read flags (`r2v`, `r2cr`, `write`) so a post-optimize clone stays faithful**
  (`ExprVar`, `ExprAt`, `ExprSafeAt`, `ExprField`, `ExprSwizzle` in `src/ast/ast.cpp`). Those flags
  are set only by RefFolding and CSE, both optimize-phase, so copying them is inert before the
  optimizer and load-bearing after it: without them a `$e` capture spliced back by a fold rule reads
  a reference instead of a value.
