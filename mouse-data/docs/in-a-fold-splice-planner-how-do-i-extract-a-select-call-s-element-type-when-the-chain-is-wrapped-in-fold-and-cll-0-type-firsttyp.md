---
slug: in-a-fold-splice-planner-how-do-i-extract-a-select-call-s-element-type-when-the-chain-is-wrapped-in-fold-and-cll-0-type-firsttyp
title: In a fold splice planner, how do I extract a SELECT call's element type when the chain is wrapped in `_fold(...)` and `cll._0._type.firstType` returns an unresolved `typedecl(result_selector(...))` formula?
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

**Problem:** plan_loop_or_count does `elementType = clone_type(cll._0._type.firstType)` and it works. When I copied this pattern into plan_zip (Phase 2B Z3), it broke with `error[30316]: local variable type can't be inferred` on the emitted `var $i(bufName) : array<$t(elementType)>` — because `firstType` came back as `typedecl(result_selector(result_selector, type<tuple<int;int>>))`, a generic-formula expression NOT a resolved type.

**Why it differs:** plan_loop_or_count's typical input chain is `each(arr)._select($(x) => x*2)` where `arr : array<int>` and the lambda body `x*2` infers cleanly to `int` — the select's return type resolves all the way down to `array<int -const-&>`. plan_zip's input is `zip(a, b) |> select($(t : tuple<int;int>) => t._0 + t._1)` — `zip(a, b)`'s output is `array<tuple<int;int>>`, but when the chain is wrapped in `_fold(...)`, the call-macro context defers full generic-typedecl evaluation on select. The select call's `_type` is reported as `array<typedecl(result_selector(...)) -const-&>` with the inner typedecl still as a formula. Calling `firstType` on that returns the unresolved formula, not `int`.

**Fix (PR Phase 2B / #2739):** pull elementType from the projection block's return-expression `_type` directly:

```das
var projElemType : TypeDeclPtr
var projBody = cll._0.arguments[1]
if (projBody is ExprMakeBlock) {
    var projMblk = projBody as ExprMakeBlock
    var projBlk = projMblk._block as ExprBlock
    if (projBlk.list |> length == 1 && projBlk.list[0] is ExprReturn) {
        var projRet = projBlk.list[0] as ExprReturn
        if (projRet.subexpr != null && projRet.subexpr._type != null) {
            projElemType = clone_type(projRet.subexpr._type)
        }
    }
}
if (projElemType == null) return null
projElemType.flags.constant = false   // strip const — `array<T const>` is unwritable
elementType = projElemType
```

The projection block (`$(t : tuple<int;int>) => t._0 + t._1`) was typed before the macro fired — `ret.subexpr._type` is concrete (`int const` here). Cloning and stripping const gives a usable element type for the buffer decl.

**Two non-obvious constraints:**

1. **Always strip const** via `flags.constant = false` after clone — projection return types are typically `T const` (since `t._0 + t._1` is a const r-value), and `array<T const>` fails at `push_clone` with `can't write to a constant value`.

2. **Block-shape guard required.** The lookup only works for the simple `{ return <expr> }` block shape. For complex projections (multiple stmts, non-simple-return), bail to tier-2 by returning null when projElemType is null. plan_loop_or_count's `firstType` path quietly accepts these by falling back through the generic typedecl chain — but in plan_zip's _fold-wrapped context, that chain doesn't resolve.

**Why plan_loop_or_count escapes:** its element binds via single-arg lambda over a SINGLE source (not a zip tuple). The single-source case lets daslang's typer pre-resolve the typedecl chain inside the _fold's argument before the macro fires; the multi-source zip-with-tuple-projection case doesn't resolve until after splice. This is a typer subtlety, not a planner bug — the workaround above is the right shape for plan_zip.

**Related:** [[how-do-i-pass-a-tuple-of-iter-vars-into-a-peeled-lambda-predicate-in-a-fold-splice]] — chain-op substitution in plan_zip uses `fold_linq_cond` (rename-based) + `let it = (itA, itB)` body-bind, NOT `fold_linq_cond_peel` with raw tuple-expr substitution.

## Questions
- In a fold splice planner, how do I extract a SELECT call's element type when the chain is wrapped in `_fold(...)` and `cll._0._type.firstType` returns an unresolved `typedecl(result_selector(...))` formula?

## Questions
- In a fold splice planner, how do I extract a SELECT call's element type when the chain is wrapped in `_fold(...)` and `cll._0._type.firstType` returns an unresolved `typedecl(result_selector(...))` formula?
