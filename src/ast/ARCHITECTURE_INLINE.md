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

`classifyArguments` therefore records, for each argument, whether a later one is unsafe to
reorder (`effectsAfter`). With such an argument behind it, a variable that is written anywhere
in the caller, and an expression that reads one, bind a local of their own, set at the
argument's place in call order.
