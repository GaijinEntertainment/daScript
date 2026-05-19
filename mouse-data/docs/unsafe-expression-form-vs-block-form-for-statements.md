---
slug: unsafe-expression-form-vs-block-form-for-statements
title: Why does unsafe(delete foo) fail to parse, but unsafe { delete foo } works?
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

`delete` is a **statement**, not an expression. `unsafe(...)` is the expression form of unsafe — only expressions go inside the parentheses. Statements need the block form `unsafe { ... }`.

```das
// PARSE ERROR: "unexpected delete"
unsafe(delete foo)

// OK
unsafe {
    delete foo
}
```

Same trap for any statement-form keyword: `return`, `break`, `continue`, `for`, `while`, variable declarations (`var x = ...`), assignments at statement level.

For pure expressions like `reinterpret<T?>(addr(x))`, `unsafe(...)` is the preferred narrow form (STYLE024 lint pushes this). The mental rule:

- "Does this thing have a value I could assign to a variable?" → expression → `unsafe(...)`.
- "Does it produce a side effect with no return value?" → statement → `unsafe { ... }`.

Edge case: single-statement `unsafe { stmt }` where `stmt` itself has a sub-expression that needs unsafe (e.g. `let x = unsafe(reinterpret<...>(...))` inside the block) — STYLE025 prefers narrowing the inner `unsafe(...)` and dropping the outer block. But when the statement itself is the unsafe operation (`delete`, `addr` at statement level, etc.) the block form stays.

## Why this catches people

Reaching for the narrow expression form is a learned reflex from STYLE024/STYLE025 lint guidance. But the lint only applies when there IS an expression to narrow. `delete x` produces no value — no narrowing possible — block is required.

## Questions
- Why does unsafe(delete foo) fail to parse, but unsafe { delete foo } works?
