---
slug: when-does-peel-each-in-daslib-linq-fold-das-unwrap-each-x-to-x-and-what-s-the-design-rationale
title: When does `peel_each` in daslib/linq_fold.das unwrap `each(<x>)` to `<x>`, and what's the design rationale?
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

**`peel_each` only unwraps when `x` is a true array (`isGoodArrayType` or `isArray`).** For iterator-typed `x` (e.g., `each(range(N))`, `each(generator())`), it returns `top` unchanged — the `each(...)` wrapper stays.

Definition: [daslib/linq_fold.das:431-445](daslib/linq_fold.das#L431-L445).

```
def private peel_each(var top : Expression?) : Expression? {
    if (!(top is ExprCall)) return top
    var topCall = top as ExprCall
    if (!is_each_call(topCall) || topCall.arguments |> length != 1) return top
    let argExpr = topCall.arguments[0]
    if ((argExpr == null || argExpr._type == null)
            || (!argExpr._type.isGoodArrayType && !argExpr._type.isArray)) return top
    return clone_expression(argExpr)
}
```

**Why gate on array-ness?** Downstream emitters call `length(src)` for buffer-reserve hints and rely on indexable / random-access semantics (e.g., chunked iteration, slice). An iterator behind the `each` wrapper has neither. Peeling would put the bare iterator in the emission and break those assumptions silently.

**Implication for splice planner authors:** when you call `top = peel_each(top)`, `top._type` may still be iterator-typed afterward. That changes which library overload your emission dispatches to (`order_by(iter)` vs `order_by(arr)`, etc. — see [[when-does-daslibs-order-by-return-iterator-vs-array]]). Don't assume `peel_each` always gives you an array.

## Questions
- When does `peel_each` in daslib/linq_fold.das unwrap `each(<x>)` to `<x>`, and what's the design rationale?
- Why doesn't `peel_each` unwrap `each(range(N))` or `each(some_generator())`?
- After `peel_each`, can `top._type.isIterator` still be true?

## Questions
- When does `peel_each` in daslib/linq_fold.das unwrap `each(<x>)` to `<x>`, and what's the design rationale?
