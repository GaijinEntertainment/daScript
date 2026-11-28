# Inliner conditional lowering

This document follows `ARCHITECTURE_COMMON.md`.

## Conditional call lowering in the inliner {#inline-conditional-lowering}

When a call inside a conditional expression needs statements for inlining,
`InlinePatch::tryLowerCallPosition` splits the condition into arm stores in the
current pass. For a user declaration it first hoists the expression into a generated
temporary, then splits that temporary immediately: the split declaration keeps the
temporary's `Variable`, so every read the hoist bound stays bound, and no later
round has to rediscover the call.

## A call under a node the walk does not list {#inline-unlisted-position}

The inliner finds a call inside its statement with `childrenInEvalOrder`, which lists the
children of each expression kind it knows, in the order they are evaluated. That order is what
lets a splice move the call's temporaries in front of the statement without passing an earlier
side effect. A kind the walk does not list hides everything under it.

A site whose statement is still in place, but whose call the walk cannot reach, is therefore
not a statement an earlier splice rewrote. `unlistedHolderOf` finds the call with a full visit
and returns the node that holds it out of the walk's reach; the site then fails by name - a
compile error for an `[inline]` callee, a logged decline for a best-effort site. Without it the
call stays a plain call and nothing is reported.

## A by-value `var` parameter the body only reads {#inline-read-only-var-parameter}

A `var` parameter passed by value is the callee's own copy, so a splice binds the argument to
a local of its own. That copy is needed only when the body can tell the two apart: it assigns
the parameter, takes its address, or hands it on by reference. `ParamReadScan` already counts
those uses as place reads and reference-argument reads, from the shape of the body it is about
to splice. With none of them, `classifyArguments` handles the parameter like a non-`var` one: a
variable or a constant substitutes, a pure expression read once substitutes, anything else binds
a local.

A pointer parameter read to reach its pointee - the base of a field, an index or a dereference -
is a value read, whatever is then done to the pointee. The write flags the side-effect pass
leaves on a variable do not make this split: a write through a pointer marks the pointer
variable written, so that the write reaches the storage the pointer aliases. That is why the
scan reads the shape and not `Variable::access_ref`. Most pointer parameters are read this way:
`var` on a pointer says the pointee is mutable, and the body writes through the pointer and
never assigns the pointer itself.

A substituted variable has the constness of the parameter it replaces: the body's nodes are
typed against the parameter, and no inference pass runs after a splice to retype them. A `var`
variable given to a non-`var` parameter is substituted and each read of it is typed const. A
const variable given to a `var` parameter always binds a local, since a read cannot drop its
constness.

## An `unsafe` body grafted as an expression {#inline-unsafe-expression-graft}

A callee body that is one `return <expr>` grafts into the caller as that expression;
`soleReturn` finds the `return` through an `unsafe { }` wrapper and the plain blocks around it.
When the callee uses `unsafe`, the grafted expression sits under a generated `ExprUnsafe`, the
same wrapper a statement splice puts around its block (`underCalleeUnsafe` builds both).

The grafted expression arrives with no wrapper of its own, for one of three reasons: the graft
lifts it out of the `unsafe { }` that held the `return`; an `unsafe(expr)` is a flag on one node
and no wrapper at all; a callee from a module compiled earlier has its wrappers folded away.
Without the generated wrapper, a caller that is inferred again refuses the operations the
callee's `unsafe` covered. The wrapper carries its body's type and builds no node of its own in
the interpreter, so it costs nothing at run time.

## An `[inline]` call the pass leaves behind {#inline-unspliced-call}

The patch pass runs in rounds until a round changes nothing. A splice, a lowering and a reported
failure each end a site; a site none of them ended is collected again by the next round. So when
a round changes nothing and reports nothing, every `[inline]` call still in a body is one the
pass skipped without a reason. `reportUnsplicedCalls` turns each into a compile error, which is
the contract of the annotation: a call splices, or the compile says why not.

Three things are left alone, because their calls stay calls by rule: a caller or a callee in an
`options never_inline` module, a build with `options disable_inline`, and an initializer of a
global variable or a structure field, which the lint pass reports on its own.

## An argument read after a later argument's effect {#inline-argument-after-sibling}

A substituted argument is evaluated where the body reads the parameter, which is after every
argument that binds a local: those locals are set in call order in front of the statement. So a
substituted argument that reads a variable sees a write made by an argument that follows it in
the call, where the call itself read the variable first: `f(n, bump(n))` with `bump` writing `n`
by reference.

`classifyArguments` therefore records, for each argument, whether a later one has an effect
(`effectsAfter`): it calls something with observable effects, or is a shape the walk does not
know. With such an argument behind it, a variable that is written anywhere in the caller, and
an expression that reads one, bind a local of their own, set at the argument's place in call
order.

A read of a global is not an effect. It writes nothing, so it does not change what an argument
before it reads. It is time-sensitive instead: its value changes when something writes the
global between the argument's place in the call and the read in the body. Only the callee and
a later argument can do that, so an argument that reads a global substitutes when the callee
has no observable effects and no later argument has an effect, and binds a local otherwise.
`isReorderSafe` is the one walk behind both tests - may this expression move, does this
argument have an effect - and its `stableGlobals` argument says whether a global read counts
as stable.

## Unrolling a loop of known passes {#loop-unroll}

The patch pass replaces a `for` loop with one copy of its body for each pass, when the passes are
known at compile time and few. In the interpreter a short loop pays its setup and one loop node
a pass; the copies pay neither, and a constant in place of the loop variable folds.

`InlineCollect` plans every loop on the walk that plans the call sites, so the pass has no
visitor of its own. `InlinePatch::unrollLoop` runs after the function's call sites are spliced,
an inner loop ahead of the loop that holds it. A loop a splice brings in is planned by the next
round.

The loop has one source and one variable. `loopPassValues` gives the value of each pass:

- a constant range, of any of the four range types - a typed constant a pass;
- a stack array literal whose elements are all constants - the elements;
- a fixed-array variable - `a[i]` with a constant index, a reference to the element.

`loop_unroll_cost` (`CodeOfPolicies`, `options loop_unroll_cost`, 8 by default) is the largest
product of passes and body statements, counted over every block of the body. So a body of one
statement unrolls up to 8 passes and a body of two up to 4. A loop of one pass always unrolls,
and a loop of no passes goes away. `for [unroll]` lifts the limit to 256 passes and
`for [unroll_disable]` keeps the loop. The option at 0 turns the rewrite off, and it runs only
where the best-effort inline tier runs: an optimized build with no debugger.

The loop stays when its body has a `finally` or an annotation, or holds a `break` or `continue`
of this loop, a `goto`, a label, `try`, `yield`, `assume`, a generator, or a lambda, a local
function or a block literal that captures. A `break` of a loop inside the body does not keep
it. With constants for values the loop also stays when the body writes the loop variable, takes
its address or passes it by reference: a constant has no storage. A function whose shape a macro owns (`ARCHITECTURE_EFFECTS.md#macro-owned-shape`) and
an instance of another module's generic keep their loops.

Each pass is a clone of the body with its locals renamed under a prefix of its own and the loop
variable substituted, built as a splice builds a callee body. The passes go into the block in
place of the loop with no scope block around them; the renaming is what lets them share one.

## A literal argument that becomes the loop source {#inline-loop-source-argument}

`f([1, 2, 3, 4])` into `def f(a : array<int>) { for (x in a) ... }` splices as a loop over the
literal itself: a fixed array on the stack, as `for (x in [1, 2, 3, 4])` is
(`ARCHITECTURE_LOOP_SOURCE.md#stack-loop-literal`). No heap array is built, and the loop is one
the unroll reads.

`stackLiteralForLoopSource` takes the argument when the body reads the parameter exactly once,
as the source of a `for` loop that is not inside another loop. `ParamReadScan` records that read
(`readAsLoopSource`) and counts loop depth from the loop's body, so a source is not under its
own loop. The argument is an array literal `stackLoopLiteral` accepts, or a literal that is on
the stack already.

The literal is then evaluated at the loop header, after every statement the body has in front
of the loop. An element that is not a constant must be safe to move (`isReorderSafe`). When the
callee has observable effects, or an argument after this one has an effect, a read of a global
is not safe to move, and the element may read only locals nothing else can reach
(`argReadsOnlyPrivateLocals`). A splice that needs a scope of its own binds the array to a
local as before.

The loop variable is typed against the parameter, so it is constant when the parameter is. The
literal is not, and no inference runs after a splice: `InlineBodyRewriter` gives the variable
the constness of the literal. AOT writes the variable's type out, and a mismatch there does not
compile.

## A body that ends in its `return` {#inline-trailing-return}

A callee body of several statements whose only `return` is its last statement splices flat:
the statements in front of the `return` go in front of the caller's statement, and the returned
expression takes the place of the call. `trailingReturn` finds the shape. Any other body with
statements goes into a scope block and stores its result in a local the caller reads; that
local and block cost nodes, and they hide the returned expression from the folds that follow.

The shape needs two statements or more, no `finally`, a last statement that returns a value
without a move, and no other `return` and no label in front of it. The site must not store its
result in place and the body must need no scope of its own. The callee is a function: an
invoked block literal keeps the block form. So does a callee that uses `unsafe`: its statements
need the generated wrapper (`ARCHITECTURE_INLINE.md#inline-unsafe-expression-graft`), and a
flat splice has no one node to put under it.
