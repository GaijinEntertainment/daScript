---
slug: where-does-nolint-rule-go-when-a-lint-warning-is-emitted-from-inside-a-qmacro-expr-and-fires-at-the-user-s-call-site-rather-than
title: Where does `// nolint:RULE` go when a lint warning is emitted from inside a `qmacro_expr` and fires at the user's call site rather than at the macro source?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

The nolint comment must be **inline at the end of the offending line**, inside the `qmacro_expr {...}` block — NOT on a separate comment line above and NOT at the user call site.

When a macro emits code via `qmacro_expr { var $i(name) = $e(expr) }`, lint analyzes the expansion at the user's call site but **reports the source position** as the line inside the qmacro_expr body. To suppress, the comment must travel with the emitted line:

```daslang
} else {
    blk.list |> emplace_new <| qmacro_expr() {
        var $i(newArgName) = $e(newCall)  // nolint:PERF009
    }
    ...
}
```

What DOESN'T work:
- `// nolint:PERF009` on a comment line above the qmacro_expr block — suppresses nothing.
- `// nolint:PERF009` on the user-side `let x = _fold(...)` line — the lint engine reports against the macro source position, not the user site.

The placement rule generalizes: `nolint:RULE` must be **on the literal line** that the lint output points at. For macro-quoted code, that's inside the `qmacro_expr { ... }` body.

Concrete example: PERF009 ("redundant move into variable immediately returned") fired at `daslib/linq_fold.das:490:24` (a line inside `fold_linq_default`'s qmacro_expr) when called via `benchmarks/sql/take_count.das`'s single-pass chain. Inline `// nolint:PERF009` on the emitted `var = expr` line suppresses it cleanly.

## Questions
- Where does `// nolint:RULE` go when a lint warning is emitted from inside a `qmacro_expr` and fires at the user's call site rather than at the macro source?
- nolint for macro-generated lint warnings
- How to suppress a lint rule that fires only at certain user call sites?

## Questions
- Where does `// nolint:RULE` go when a lint warning is emitted from inside a `qmacro_expr` and fires at the user's call site rather than at the macro source?
