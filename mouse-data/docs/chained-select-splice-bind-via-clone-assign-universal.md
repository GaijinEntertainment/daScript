---
slug: chained-select-splice-bind-via-clone-assign-universal
title: How do I splice chained `_select|_select` stages in a daslib macro without hitting the ExprRef2Value substitution trap, and is the bind-via-`:=` approach correct for every element type?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---


Bind the previous projection to a fresh local with **`:=` (clone assignment)**, then rename the next select's `_` to that bind name via `Template.replaceVariable("_", $i(bindName))`. Single emission shape — no `static_if`, no macro-time workhorse branching.

```das
// projection = previous select's lambda body (already typed, may carry ExprRef2Value
// wrappers from the typer pass)
let bindName = "`v`{at.line}`{at.column}`{length(intermediateBinds)}"
intermediateBinds |> push <| qmacro_expr() {
    var $i(bindName) := $e(projection)
}
// next stage's `_` gets renamed to bindName via Template.replaceVariable
```

**Why bind-and-rename instead of direct substitution:** trying to substitute the full previous projection expression into the next stage's body hits the `ExprRef2Value` trap — see [[my-macro-substitutes-it-for-a-projection-expression-via-template-replacevariable-it-proj-apply-template-but-the-result-fails-to]]. The bind step sidesteps it: the substitute is now a simple `ExprVar(bindName)`, no wrapper, `Template.replaceVariable` works cleanly.

**Why `:=` is universally correct** — and why the workhorse / non-workhorse branch you might be tempted to write isn't needed:

| Source type | What `:=` does | Side effects on source |
|---|---|---|
| Workhorse (`int`, `float`, `bool`, `string`, …) | byte copy | none — same cost as plain `=` |
| Non-workhorse (`array<T>`, lambdas, user structs) | deep clone, new heap alloc | none — source remains intact |
| Lvalue projection like `_._field` | clones the field's value into the local | the parent object is untouched |

Compare:
- **`=`** (plain assign) — fails to compile for non-copyable types (`array<T>`, lambdas, etc.) and triggers the strict "this type is not assignable" check.
- **`<-`** (move) — corrupts the source by zeroing it. For an lvalue projection like `_._field`, this would zero the field on the underlying iterated element — incorrect, since the user expects the projection to be a *read*.

So a single emission `var $i(bind) := $e(projection)` covers every chain shape. Earlier `linq_fold` versions had a `prevWorkhorse=false → return null` guard at the chained-`select` arm; Phase 2C Ring 4 (2026-05-17) lifted it after the audit confirmed `:=` is safe everywhere.

**Concrete location:** `daslib/linq_fold.das` `plan_loop_or_count` select-stage arm — chained selects of any type splice through this one path. The push-vs-emplace decision in the array lane (different problem — that's about `push(value)` vs `var v <- value; emplace(v)`) still branches on workhorse-ness; see [[when-a-call-macro-needs-to-pick-copy-vs-move-init-for-a-projection-should-i-emit-static-if-typeinfo-is-workhorse-e-proj-or-decid]].

**What workhorse branches still legitimately exist in `linq_fold.das`** (audit 2026-05-17):
1. `fold_select_where` — `static_if (typeinfo is_workhorse($e(selectExpr)))` — frozen `_old_fold` baseline path; changing it would invalidate the `m3f_old` benchmark column.
2. `min_max_compare` — workhorse uses `<` / `>` directly (single-instruction compare); non-workhorse falls back to `_::less` for user/tuple comparators. Perf-critical (~2× per-element on int columns), Boris-mandated.

Outside those two, no workhorse branch in the splice path.

## Questions
- How do I splice chained `_select|_select` stages in a daslib macro without hitting the ExprRef2Value substitution trap, and is the bind-via-`:=` approach correct for every element type?
