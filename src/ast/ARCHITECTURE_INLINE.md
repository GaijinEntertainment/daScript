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

## An `[inline]` call the pass leaves behind {#inline-unspliced-call}

The patch pass runs in rounds until a round changes nothing. A splice, a lowering and a reported
failure each end a site; a site none of them ended is collected again by the next round. So when
a round changes nothing and reports nothing, every `[inline]` call still in a body is one the
pass skipped without a reason. `reportUnsplicedCalls` turns each into a compile error, which is
the contract of the annotation: a call splices, or the compile says why not.

Three things are left alone, because their calls stay calls by rule: a caller or a callee in an
`options never_inline` module, a build with `options disable_inline`, and an initializer of a
global variable or a structure field, which the lint pass reports on its own.
