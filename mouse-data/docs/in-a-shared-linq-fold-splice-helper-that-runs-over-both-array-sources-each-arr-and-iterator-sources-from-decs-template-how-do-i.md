---
slug: in-a-shared-linq-fold-splice-helper-that-runs-over-both-array-sources-each-arr-and-iterator-sources-from-decs-template-how-do-i
title: In a shared linq_fold splice helper that runs over both array sources (`each(arr)`) and iterator sources (`from_decs_template(...)`), how do I dispatch the explicit return type so the typer doesn't choke on `error[30343] iterator<T> = array<T>`?
created: 2026-05-21
last_verified: 2026-05-21
links: []
---

The decs bridge (`from_decs_template(type<T>)`) types as `iterator<T>` because the surface emits `to_sequence(res)` internally. Adding `.to_array()` peels it back to `array<T>`. **A splice that supports both source families must check the chain's expression type and emit the return-type annotation + final-return shape to match.**

Pattern (drop-in for any planner that buffers tuples then returns):

```das
// captured once when the planner runs
let exprIsIterator = expr._type.isIterator

// dispatched in two places:

// 1) the explicit return-type annotation on the inner invoke wrapper
let retType : TypeDeclPtr = exprIsIterator
    ? clone_type(make_iterator_type(elemType))
    : clone_type(make_array_type(elemType))

// 2) the final-return statement
def buffer_return(bufName : string; isIterator : bool) : ExpressionPtr {
    return isIterator
        ? qmacro_block(return <- $i(bufName).to_sequence_move())
        : qmacro_block(return <- $i(bufName))
}
```

**Why both sides need it.** If you build the buffer as `array<tuple<...>>` then return it bare, the typer infers the wrapping invoke's return type as `array<...>`. If the caller chain types as `iterator<...>` (because `from_decs_template` didn't have `.to_array()` peeled), `error[30343]: iterator<...> = array<...>` fires at the call site. Adding `.to_sequence_move()` converts; the explicit `retType` annotation on the invoke wrapper anchors the typer.

**Decs-side gotcha.** The decs bridge ALWAYS wraps the emit in `qmacro(invoke($() : retType { ...stmts; return ... }))` (you can't splice the body directly into the call site because of the outer `for_each_archetype` lambda scope). The explicit `: retType` annotation is non-negotiable on the decs side — without it the typer fails to infer back to iterator vs array correctly when both shapes share the helper.

Reference fix: PR #2775 commit `a7b0de4d3` (Slice 5e, Copilot review #1). When `plan_decs_group_by` was first added it hardcoded `return <- $i(bufName)` (array assumption), which broke the test `target_unroll5e_groupby_iter_typed_fold` where the chain stayed iterator-typed (no `.to_array()` peel). Fix: capture `exprIsIterator` early, branch on it for both `retType` and `buffer_return`. Same dispatch is used by `plan_group_by_core` post-refactor (commit `cb72db296`), so the array-side path automatically inherits the fix and the iterator-typed case is now exercised by both source families' tests.

## Questions

- In a shared linq_fold splice helper that runs over both array sources (`each(arr)`) and iterator sources (`from_decs_template(...)`), how do I dispatch the explicit return type so the typer doesn't choke on `error[30343] iterator<T> = array<T>`?
- Why does my decs splice planner fail to compile with `error[30343] iterator<X> = array<X>` only when `.to_array()` is absent from the chain?
- What's the `buffer_return(bufName, isIterator)` pattern and where does it dispatch?
- When does `from_decs_template(type<T>)` type as `iterator<T>` vs `array<T>` in the caller chain?

## See also

- [[when-adding-a-decs-specific-splice-planner-e-g-plan-decs-group-by-to-the-linq-fold-cascade-where-does-it-go-relative-to-the-arra]] — cascade ordering trap (sibling lesson)
- [[when-does-daslib-s-order-by-return-iterator-vs-array-and-why-is-to-sequence-move-dangerous-to-wrap-blindly-around-the-result]] — `to_sequence_move` semantics on the source side
- [[in-a-fold-splice-planner-how-do-i-extract-a-select-call-s-element-type-when-the-chain-is-wrapped-in-fold-and-cll-0-type-firsttyp]] — getting the element type out of the chain in the first place

## Questions
- In a shared linq_fold splice helper that runs over both array sources (`each(arr)`) and iterator sources (`from_decs_template(...)`), how do I dispatch the explicit return type so the typer doesn't choke on `error[30343] iterator<T> = array<T>`?
