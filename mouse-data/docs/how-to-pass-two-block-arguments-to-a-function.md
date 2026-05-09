---
slug: how-to-pass-two-block-arguments-to-a-function
title: How do I pass two (or more) block arguments to a function in one call?
created: 2026-05-09
last_verified: 2026-05-09
links: []
---

Use the **positional `${ ... }` block-literal** syntax. This is distinct from the trailing block-arg pipe form documented in CLAUDE.md ("block/lambda after `func()` pipes as last arg") — that one only handles ONE block, the trailing one.

## The pattern

```das
def split_h(left : block<():void>; right : block<():void>) {
    invoke(left)
    invoke(right)
}

split_h(
    ${
        // left pane
    },
    ${
        // right pane
    })
```

Each `${ ... }` is an anonymous parameterless block, usable in any positional argument slot — not just trailing.

## Single-line form needs `;`

Multi-line works as written. Single-line needs a trailing `;` after the body statement, because braces with content otherwise hit the "one statement per line, separator required" rule:

```das
// Works:
split_h(${ print("L\n"); }, ${ print("R\n"); })

// Fails — error[30151]: syntax error, unexpected '}':
split_h(${ print("L\n") }, ${ print("R\n") })
```

## When to use which form

| Situation | Form |
|---|---|
| One block, naturally trailing | Trailing block-arg: `func(arg) { ... }` |
| One block, must be in a non-trailing slot | `${ ... }` |
| Two or more blocks | `${ ... }, ${ ... }` |
| Block needs parameters | `@(x) { ... }` (lambda) or `$(x) { ... }` (block) |

## Notes

- Block literal type: `block<():void>` (parameterless, void-returning). For other shapes use `@(args) { ... }` (capture lambda) or `@@(args) { ... }` (no-capture function pointer).
- Variadic-arity helpers (e.g. `columns(N, ${ }, ${ }, ...)`) work natively at macro level — the macro walks `call.arguments` and dispatches per element.

## See also
- CLAUDE.md gen2 syntax bullet on positional block literal `${ ... }`

## Questions
- How do I pass two (or more) block arguments to a function in one call?
