---
slug: qmacro-invoke-source-bind-typedecl-modifier-iter-vs-array
title: In a call_macro that emits an `invoke($($i(src) : typedecl($e(topExpr)) <modifier>) { ... }, $e(topExpr))` wrapper, what `<modifier>` do I use so the param matches both array and iterator sources without const/ref mismatches?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

There is no single modifier that works for both — branch on `top._type.isIterator`:

```das
if (top._type != null && top._type.isIterator) {
    // Iterator source — rvalue from a function call like each(range(10)).
    // typedecl() picks up the function-return type which carries const;
    // -const strips it so the body can `for (it in src)` (otherwise
    // daslang complains "can't iterate over const iterator").
    res = qmacro(invoke($($i(srcName) : typedecl($e(topExpr)) - const) {
        // ... body uses $i(srcName) ...
    }, $e(topExpr)))
} else {
    // Container source with length — array/table/string/range/fixed-array.
    // `let arr <- [...]` is `array<T> const&`. Stripping -const would
    // produce a non-const-ref param; passing the const-ref source then
    // fails with `array<T> const& vs array<T>` ("can't ref types
    // can only add constness"). Keep modifiers — typedecl() preserves
    // them and the const-ref source matches exactly.
    res = qmacro(invoke($($i(srcName) : typedecl($e(topExpr))) {
        // ... body uses $i(srcName) ...
    }, $e(topExpr)))
}
```

The two error messages are diagnostic — they tell you which branch you're on:
- `can't iterate over const iterator` → you forgot `-const` on an iterator path
- `array<T> const& vs array<T> ... can't ref types can only add constness` → you have `-const` on an array path

**Why this is needed in the first place:** the block param is your way to bind the source expression to a stable name so the loop body can reference it once without re-evaluating side effects. The "right" param type is "whatever the source actually is" — but qmacro `typedecl(expr)` produces the raw type-of including const-ref from the call return, which only sometimes matches what the consumer needs.

**Use `top._type != null` guard** — `_type` is null for freshly cloned expressions that haven't gone through the typer yet. Treating null as "not iterator" (default to array branch) is wrong if you're past the typer; pick conservatively and call out the assumption.

**See `daslib/linq_fold.das` `plan_loop_or_count`** for a working example with five emission sites — counter lane, array-lane iter/sourceHasLength/else, and the length-shortcut path that's only reachable when the source has length (so it always uses the no-modifier form).

**Fast path for length-shortcut:** if you can emit `length($e(topExpr))` directly without the invoke wrapper, do that — no source-bind problem. Works when the entire body is one expression and the source's evaluation cost is "you'd evaluate it once anyway."

## Questions
- In a call_macro that emits an `invoke($($i(src) : typedecl($e(topExpr)) <modifier>) { ... }, $e(topExpr))` wrapper, what `<modifier>` do I use so the param matches both array and iterator sources without const/ref mismatches?
