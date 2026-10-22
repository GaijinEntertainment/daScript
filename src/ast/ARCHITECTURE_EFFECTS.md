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
