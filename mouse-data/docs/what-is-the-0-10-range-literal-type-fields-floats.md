---
slug: what-is-the-0-10-range-literal-type-fields-floats
title: What is the `0..10` range literal — what type, what fields, does it work for floats?
created: 2026-05-09
last_verified: 2026-05-09
links: []
---

`0..10` is sugar for `interval(0, 10)`, a daslang builtin. It returns a **`range` value**, not an `iterator<int>`.

## Variants by operand type

The `interval` builtin is overloaded for integer types only:

| Operands | Returns |
|---|---|
| `int, int` | `range` |
| `uint, uint` | `urange` |
| `int64, int64` | `range64` |
| `uint64, uint64` | `urange64` |

**There is no float form.** `0.0f .. 10.0f` does not parse — the parser recognizes `..` for the four integer variants. If a use case wants float ranges, the parser + `interval` overload set is editable; not currently shipped.

## Fields

The `range` struct has fields **`.x` and `.y`** (vec-like layout), not `.from`/`.to`:

```das
let r = 0..10
print("{r.x},{r.y}\n")  // "0,10"
```

`describe_type range` returns "not found" — it's a builtin without RTTI registration.

## Iteration

`range` is iterable directly in `for`:

```das
for (i in 0..10) {
    print("{i}\n")
}
```

But `range` ≠ `iterator<int>`. A function declared `def f(it : iterator<int>)` will reject `0..10`:

```
error[30341]: no matching functions or generics: f(range const)
  invalid argument 'it' (0). expecting 'iterator<int> const',
                              passing 'range const'
```

If you want to write functions that accept range literals, declare them as `def f(r : range)` (pass-by-value).

## Macros

Pattern-matching `0..10` in a macro: it parses as a binary `..` operator AST node, NOT as `ExprCall(name="interval")`. Walk the operator form, not the function-call form.

## See also
- CLAUDE.md gen2 syntax bullet on range literal `..`

## Questions
- What is the `0..10` range literal — what type, what fields, does it work for floats?
