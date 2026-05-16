---
slug: why-does-each-arr-fail-with-unsafe-when-not-source-of-for-loop-outside-a-for-and-what-s-the-alternative-in-a-linq-chain
title: Why does `each(arr)` fail with "unsafe when not source of for-loop" outside a for, and what's the alternative in a linq chain?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

`each(arr)` returns an iterator that walks the array. Daslang's safety rules say that iterator is unsafe **unless it's directly consumed by a for-loop in the same scope** — passing it through `|>` chains, capturing it in a `let`, or handing it to a function argument all trip:

```
error[31013]: '__::builtin`each`...' is unsafe, when not source of the for-loop;
              must be inside the 'unsafe' block
```

**Inside `_fold(...)`** the error doesn't fire because `_fold` is a macro that expands to a for-loop body where `each(arr)` IS the source. So `_fold(each(arr)._where(...).count())` compiles cleanly.

**Outside a fold macro**, in a plain pipe chain, use the array directly — most `_<op>` call-macros (`AstCallMacro_LinqPred2`) accept both `iterator<T>` and `array<T>` for arg 0:

| Doesn't work | Use instead |
|---|---|
| `let prices <- (each(arr) \|> _where(...) \|> _select_to_array(_.price))` (iterator outside `_fold`) | `let prices <- (arr \|> _where(...) \|> _select(_.price))` — array+macros chains as array; result is `array<T>`, no `_to_array` suffix needed |
| `let c = each(cars)._join(each(dealers), ...)` inside `_fold` (two `each()`s, one not the chain source) | `let c = _fold(cars \|> _join(dealers, ..., ...) \|> count())` — pass arrays directly |
| `let r = each(arr) \|> ...` outside any fold | wrap in `unsafe(each(arr))`, OR start the chain with `arr` directly and let the macro handle iterator promotion |

**Heuristic:** if the chain ends in a `_fold(...)` / `_old_fold(...)` wrapper or a for-loop, `each(arr)` works as the source. If the chain produces a value (or array) that escapes the expression — a `let`, a function return, the second arg to a macro — drop the `each()` and pass the array directly.

The compiler error points at the **specific** `each(arr)` call that escapes, so for multi-each chains (`_join`, `_zip`), check which side is the issue.

## Questions
- Why does `each(arr)` fail with "unsafe when not source of for-loop" outside a for, and what's the alternative in a linq chain?
- error[31013] '__::builtin`each`' is unsafe — how to fix?
- When can I use `each(arr)` in a linq pipe chain?

## Questions
- Why does `each(arr)` fail with "unsafe when not source of for-loop" outside a for, and what's the alternative in a linq chain?
