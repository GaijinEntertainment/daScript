# Declared side effects and the optimizer

This document follows `ARCHITECTURE_COMMON.md`.

## The observable-effects mask {#observable-effects}

`OBSERVABLE_EFFECTS` (`ast.cpp`) is the set of `SideEffects` bits that make a call do something
besides compute its result: `userScenario`, `modifyExternal` (which is also `accessExternal`),
`modifyArgument`, `accessGlobal`, `invoke` and `captureString`. The `unsafe` bit is not in it:
`setSideEffects` moves it to `unsafeOperation`, and an unsafe call writes nothing by being unsafe.
A pass reads it through `Function::hasObservableEffects` (or `observableEffects` when it needs the
bits), never `sideEffectFlags` itself, so a bit added to `SideEffects` that is not an observable
effect does not silently turn every pure check off.

Every consumer below needs the whole mask; none of them is safe with any one of its bits set:

- **Dropping an evaluation** - `noSideEffects` on `ExprOp1`, `ExprOp2` and `ExprCall`, and `isNop`
  (`ast_const_folding.cpp`). Each bit is an effect the dropped call would have had. `accessGlobal`
  does not split reads from writes, so a call that only reads a global is kept too. The one
  exception is local: a call whose only effect is `modifyArgument` and whose every argument is a
  make-local temporary writes only memory the expression itself builds, so `ExprCall` marks it
  free of side effects at that site.
- **Running a call at compile time** - `isConstExprFunc`, the constant arms of the `ExprOp1`,
  `ExprOp2` and `ExprCall` folds, and the folding of a das function with constant arguments. An
  external write would happen during compilation and never at run time; an external or global read
  sees the compiler's state, not the program's; an `invoke` runs code the fold cannot see; a
  captured string points into the folding context's heap, which dies with the fold; and with
  constant arguments the only memory a `modifyArgument` builtin can write is a temporary or null.
- **Rewriting an operator** - the operator lookup that builds a new node (`findBuiltinOperator`),
  the involution and comparison folds, the splat fold and reassociation. These create, merge or
  reorder evaluations of the operator, so any effect it has would run a different number of times.
- **Escape analysis** - `isArgEscapeNeutral` (`ast_escape_analysis.cpp`). A builtin with any of the
  bits can store the argument's address: through external state, through another argument, through
  a global, through an invoked block, or by capturing the string.
- **Lint and registration** - the "top level no side effect operation" error (`ast_lint.cpp`), the
  `addFunction` checks for a builtin that returns nothing and does nothing and for a
  `modifyArgument` builtin with no writable argument (`ast_module.cpp`), and the `[nosideeffects]`
  marker of the AST printer (`ast_print.cpp`).

The inliner (`isReorderSafe`, `calleeWriteFree`) reads the whole mask too. CSE reads it minus
`CSE_TRANSPARENT_CALL_EFFECTS`: an argument write is stamped at the call, and a captured string
writes no memory CSE tracks.

`SideEffects::mayRaise` is outside the mask: a call that can raise is still pure for folding,
rewriting, escape analysis and the lint, and `10 / 2` still folds at compile time while `10 / 0` is
left to raise at run time. A builtin declares the bit in its binding (integer `/` and `%`, `panic`,
the string conversions and character reads). A das function gets it the way it gets every other
inferred bit: `getSideEffects` (`ast_unused.cpp`) sets it from the function's own nodes and ORs it
in from every callee. A callee still being analyzed - a call cycle - counts as raising, since an
unreported raise would be dropped.

`NoSideEffectVisitor` (`ast_const_folding.cpp`) sets `Expression::mayRaise` beside `noSideEffects`,
for the whole subtree: `a[10] + 1` raises although its root does not, and a call or an operator
raises when its function does. A node built after the walk has `mayRaise` clear, but also
`noSideEffects`, so it is never dropped before the next walk computes it. An `ExprMakeBlock` does not raise: making a block runs
none of its body, and whatever the body raises surfaces where the block is invoked.

Both visitors take the node rule from `nodeRaises` and the call rule from `callRaises`
(`ast_effects.cpp`). `ExprAt` raises unless its bound check is gone (`noBoundCheck`); `ExprPtr2Ref`
and an `ExprField` through a pointer raise unless the dereference is unchecked (`unsafeDeref`, where
a null pointer is undefined behavior, not a raise); `ExprAsVariant`, `ExprAssert` and an `ExprInvoke`
(its target is unknown) always raise. Only a pass that deletes
an evaluation asks, and only through `droppable`, which wants `noSideEffects` and no `mayRaise` together. CSE does not: it reuses the first
evaluation, which still raises.
