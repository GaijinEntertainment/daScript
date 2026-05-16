---
slug: my-fold-macro-emits-a-loop-with-for-it-in-source-acc-reserve-length-source-but-the-reserve-doesn-t-fire-when-the-chain-starts-wi
title: My fold macro emits a loop with `for (it in source); acc |> reserve(length(source))` but the reserve doesn't fire when the chain starts with `each(arr)`. How do I make it work?
created: 2026-05-16
last_verified: 2026-05-16
links: [[daslang-generic-instance-detect-via-fromgeneric]]
---

Peel `each(<expr>)` at macro time. `each(arr)` reports as `iterator<T>`, so any "is the source an iterator?" check (e.g. `top._type.isIterator`) sees `true` and the array-only reserve path is skipped. But the iteration semantics of `for (it in each(arr))` and `for (it in arr)` are identical — the wrapper iterator is incidental in fold context.

Pattern (corrected version, from `daslib/linq_fold.das` after Phase 2A bind-elision PR):

```das
[macro_function]
def private is_each_call(call : ExprCall?) : bool {
    // `each` in daslib/builtin.das is generic — the resolved `func.name`
    // on a typed instance is mangled (e.g. `builtin`each`30908...`).
    // The original generic's name lives in `func.fromGeneric.name`.
    if (call == null || call.func == null) return false
    return (call.func.name == "each"
        || (call.func.fromGeneric != null && call.func.fromGeneric.name == "each"))
}

[macro_function]
def private peel_each(var top : Expression?) : Expression? {
    if (!(top is ExprCall)) return top
    var topCall = top as ExprCall
    if (!is_each_call(topCall) || topCall.arguments |> length != 1) return top
    let argExpr = topCall.arguments[0]
    // Only peel when the argument is a true array (or fixed-size array).
    // Don't peel iterator-typed args like `each(range(10))` — replacing the
    // each call with the raw range would break length-shortcut + reserve
    // hints that assume an indexable source.
    if ((argExpr == null || argExpr._type == null)
            || (!argExpr._type.isGoodArrayType && !argExpr._type.isArray)) return top
    return clone_expression(argExpr)
}
```

**Two gotchas the original version missed:**

1. `func.name == "each"` never matched typed instances — generic-instance detection requires `fromGeneric.name`. See [[daslang-generic-instance-detect-via-fromgeneric]].
2. Peel gate must be **positive** (`is good array`) not negative (`isn't iterator`). `each(range(N))` returns an iterator but its argument `range(N)` is also iterator-shaped (`isRange`) and would otherwise pass `!isIterator`. The positive `isGoodArrayType || isArray` gate cleanly excludes range/string/lambda sources.

**Block-parameter typedecl needs branching on source shape after peel.** When peel fires, the source goes from iterator (rvalue, no modifiers) to array (`array<T> const&` for `let arr <-` chains). The block parameter type:
- iterator source: `typedecl($e(topExpr)) - const` — strip rvalue const so body can iterate
- array source: `typedecl($e(topExpr))` (no modifier) — keep `const&` so const-ref source matches

Both wrong → either `array<int> const& vs array<int>` mismatch or `can't iterate over const iterator`.

**What this is worth:** brought `linq_fold`'s `each(arr)._where(...)._select(_.price).to_array()` benchmark from 13 → 10 ns/op (parity with comprehension baseline). The count→length shortcut built on top brings pure `each(arr)._select(_.x).count()` from 2 → 0 ns/op (loop entirely elided).

**Generalizes:** any fused-loop emitter that needs the source's length (reserve, two-pass, length-aware operators like `take_last`), peel inner-array-yielding wrappers — but use `fromGeneric` for generic helpers and a positive array gate, not a negative iterator gate.

## Questions
- My fold macro emits a loop with `for (it in source); acc |> reserve(length(source))` but the reserve doesn't fire when the chain starts with `each(arr)`. How do I make it work?
