# Declared side effects and the optimizer

This document follows `ARCHITECTURE_COMMON.md`.

## The observable-effects mask {#observable-effects}

`OBSERVABLE_EFFECTS` (`ast.cpp`) is the set of `SideEffects` bits that make a call do something
besides compute its result: `userScenario`, `modifyExternal` (which is also `accessExternal`),
`modifyArgument`, `accessGlobal`, `invoke` and `captureString`. The `unsafe` bit is not in it:
`setSideEffects` moves it to `unsafeOperation`, and an unsafe call writes nothing by being unsafe.
A pass reads it through `Function::hasObservableEffects` (or `observableEffects` when it needs the
bits), never `sideEffectFlags` itself, so a bit added to `SideEffects` that is not an observable
effect does not silently turn every pure check off. `[no_capture_string]` on a function clears
its inferred `captureString` bit (`ast_unused.cpp`), so every consumer below then reads a function
that carries it wrongly as one that keeps no string.

`accessGlobal` also marks a function whose value depends on the process it runs in: one that
takes a function's address (`@@fn`), or `reinterpret`s a pointer, function, lambda, block or
string to a type that is none of those. An address is the compiler's while folding and the
program's at run time, so a folded one would put the wrong value, and a different AOT hash per
generation, into the program.

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
The walk reads a callee still being analyzed with only the bits it has so far, so after it
`closeCallCycles` ORs every callee's bits (minus `modifyArgument`) into its callers until nothing
changes: a function in a call cycle carries every bit any member of the cycle has, and a global
read two calls around a cycle still keeps a constant-argument call from folding.

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

## Sinking a pure local into the branch that reads it {#sink-pure-let}

`optimizationSink` (`ast_sink.cpp`) moves a `let` of one variable into the `if` right after it, when
that `if` reads the variable in one arm only and nothing after the `if` reads it. The local is then
computed only on the path that uses it. The common case is an inlined check: the inliner binds the
message argument to a local ahead of the body's `if`, so a check that passes still builds its
message; once sunk, it builds it only when it fails.

Moving an evaluation later, and onto one path, is the same question as deleting it on the others, so
the pass asks `droppable` of the initializer: no side effect, and no raise, since a raise that
happens today on every path must not vanish on the paths that no longer evaluate it. The `if`
condition must be `droppable` too: the initializer now runs after it, and a condition that writes
what the initializer reads would change its value. A reference local, an `inscope` or collected
local (its cleanup sits in the block's final list) and a move out of storage (the move zeroes the
source at a different time) stay where they are. No arm can declare the moved local's name: infer
refuses a local that shadows another, and the optimizer's own locals carry unique names. `options
disable_sink` turns the pass off.

The pass is one visitor. A block's pre-visit finds its let-if pairs and registers each `if`'s arms,
which are blocks themselves; the walk counts every use of a candidate's variable, and the uses inside
each of its arms; and the block's post-visit moves a let whose uses all sit in one arm - none in the
condition, the other arm or after the `if`. A local lives in its own block, so by the post-visit
every use of it has been counted. It runs no side-effect walk of its own: it
reads the flags that walk left at the start of dead-store elimination, the pass right before it.
A node built since then is never `droppable`, so a stale flag can only keep a let in place.

## Forwarding a pure local read once {#forward-read-once-let}

The sink visitor also replaces the single read of a local with the local's initializer and drops
the `let`, when the read is in the statement right after it: `let t = a * 3; g += t` becomes
`g += a * 3`. The store into the local and its stack slot are gone. A spliced body leaves many
such locals: an argument bound to a local, a result read once.

`forwardableLet` asks what the sink asks of the `let` - one variable, no reference, no cleanup,
a `droppable` initializer - and also that the initializer is neither a move nor a clone and is a
value of the local's own type. The variable is read once in the whole function, as a value.

The initializer moves from in front of the statement to the place of the read, so nothing with
an effect may run between the two. `evaluatesAfterPureSiblings` checks each step of the path
from the statement down to the read. The step passes under these parents, and under no other:

- a `return`, the condition of an `if`, the sources of a `for`, the initializer of a
  one-variable `let`;
- a read of a reference, a cast, a pointer dereference, a field, a one-operand operator;
- a two-operand operator other than `&&` and `||`, an index, a call's arguments, the elements
  of a string builder - when every other operand of that parent is `droppable`.

Every other operand, not only the ones in front: the order a call evaluates its arguments in is
not defined. `&&`, `||`, `??` and a safe index evaluate an operand on some paths only, and
moving an evaluation onto one path is the sink's rewrite, with the conditions that section
gives.

The pass reads the flags the sink reads and builds none. A function whose shape a macro owns
keeps its locals (`ARCHITECTURE_EFFECTS.md#macro-owned-shape`). `options disable_forward` turns
the forward off and leaves the sink on; `options disable_sink` turns the whole pass off.

## A function whose shape a macro owns {#macro-owned-shape}

`hasMacroOwnedShape` is true of a function that carries an annotation other than `export` and
`unused_argument`, and of a generated function that is neither a lambda nor a local function. A
macro may read such a body after the optimizer ran: a shader backend lowers an entry point,
`[flatten]` matches the `let`s of the twin it generated. Forwarding a local and unrolling a loop
(`ARCHITECTURE_INLINE.md#loop-unroll`) keep the values and change the statements, which is what
such a macro reads, so both leave these functions as written.

The test is per function. A function a macro reaches only through a call - a helper a kernel
calls, a method beside the annotated one - carries no mark. A file of such functions opts out
by option: the GPU kernel sources of dasLLAMA set `options loop_unroll_cost = 0` and
`options disable_forward`, and the shader emitter fixtures under `tests` set the first. They
keep the sink on: it shaped those kernels before either rewrite existed.
