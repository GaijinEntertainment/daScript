---
slug: which-typedecl-predicates-identify-types-where-length-expr-is-statically-resolvable-in-daslang-macros
title: Which TypeDecl predicates identify types where length(expr) is statically resolvable in daslang macros?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

# Length-supporting types in daslang macros

When a `[macro_function]` / `[call_macro]` needs to emit `length($e(src))` and have it compile, the source's `TypeDecl` must be one of:

- `isGoodArrayType` — `array<T>` (the dynamic array, including `array<T>#`)
- `isGoodTableType` — `table<K; V>`
- `isString` — `string` / `string#`
- `isArray` — fixed array `T[N]` (NOT `array<T>` — that's `isGoodArrayType`; the naming is confusing)
- `isRange` — `range` / `urange` / `range64` / `urange64`

**Excluded** (no `length()` overload — emitting `length(src)` will fail to compile inside macro output):

- `isIterator` — iterators don't carry length, even when wrapping a length-having source. Use the underlying container.
- `isGoodLambdaType` — `def each(lam : lambda<...>)` makes lambdas iterable, but they have no `length()`. This is a common trap when peeling `each(<x>)` based solely on "not an iterator."
- Custom user `def each(MyType)` types — depends on whether the user also defined `length(MyType)`; assume no.

## Canonical predicate

```das
[macro_function]
def private type_has_length(t : TypeDecl?) : bool {
    if (t == null) return false
    return (t.isGoodArrayType || t.isGoodTableType || t.isString
        || t.isArray || t.isRange)
}
```

Note the parenthesization: a bare `||`-chain split across lines hits a `gen2` parse error at the leading `||`. Wrap the chain in `(...)`.

## Why this matters for `each(<x>)` peeling

A common optimization: when a chain starts `each(<x>)._where(...)...`, peel the `each` and iterate `<x>` directly so reserve/length work. The peel must be gated on `type_has_length(<x>._type)` — checking only `!isIterator` would silently accept `each(lambda)` and emit broken `reserve(length(lambda))`.

Example from `daslib/linq_fold.das` (PR #2689, Phase 2A):

```das
[macro_function]
def private peel_each_length_source(var top : Expression?) : Expression? {
    if (!(top is ExprCall)) return top
    var topCall = top as ExprCall
    if (topCall.func == null || topCall.func.name != "each"
            || topCall.arguments |> length != 1
            || !type_has_length(topCall.arguments[0]._type)) return top
    return clone_expression(topCall.arguments[0])
}
```

The `clone_expression` is needed because `topCall.arguments[0]` is `Expression? const` (the args vector entry is const-typed even when the outer call is `var`); the planner stores `top` as `var Expression?` so the clone drops the const.

## Discovery

The set of `length()`-supporting types is not advertised as a single predicate anywhere — assembled from `mcp__daslang__describe_type TypeDecl` (the `isXxx` method list) cross-referenced with the `def length(...)` overloads in `daslib/builtin.das` and the `def each(...)` overloads. Lambda iterables surfaced as a Copilot review finding on PR #2689.

## Questions
- Which TypeDecl predicates identify types where length(expr) is statically resolvable in daslang macros?
