---
slug: when-a-call-macro-needs-to-pick-copy-vs-move-init-for-a-projection-should-i-emit-static-if-typeinfo-is-workhorse-e-proj-or-decid
title: When a call_macro needs to pick copy-vs-move-init for a projection, should I emit `static_if (typeinfo is_workhorse($e(proj)))` or decide at macro time?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

Decide at macro time. By the time a `[call_macro]` `visit()` fires, inner macros have expanded and the typer has run, so every sub-expression carries a resolved `_type`. Read `projection._type.isWorkhorseType` directly and emit exactly one branch — no `static_if`, no `typeinfo is_workhorse` at runtime, less AST for the typer to fold away later.

Pattern:

```das
let workhorseProj = projection._type != null && projection._type.isWorkhorseType
var perElem : Expression?
if (workhorseProj) {
    perElem = qmacro_expr() { $i(accName) |> push($e(projection)) }
} else {
    perElem = qmacro_block() {
        var $i(valName) <- $e(projection)
        $i(accName) |> emplace($i(valName))
    }
}
```

For workhorse types (`int`, `float`, `bool`, `string`, …, anything `isWorkhorseType` returns true for) you can push the expression directly with no intermediate `var v = expr`. For non-workhorse, `<-` is a statement not an expression — you need `var v <- proj; acc |> emplace(v)`. The two-step is only required there.

This trick brought daslang `linq_fold`'s `where|select|to_array` emission from 13 → 11 ns/op (parity with the `_old_fold` comprehension baseline) at 100K rows. See [daslib/linq_fold.das](daslib/linq_fold.das) `plan_loop_or_count` (the array lane). The previous version had a runtime `static_if` inside the qmacro — correct but generated 2× the AST and lost the temp-binding optimization opportunity.

Other `TypeDecl` predicates available at macro time: `isIterator`, `isGoodArrayType`, `isConst`, `isPod`, plus `firstType` / `secondType` / `argTypes` for compound types. Use them; the typer has already done the work.

## Questions
- When a call_macro needs to pick copy-vs-move-init for a projection, should I emit `static_if (typeinfo is_workhorse($e(proj)))` or decide at macro time?
