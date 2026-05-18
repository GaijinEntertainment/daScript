---
slug: my-macro-substitutes-it-for-a-projection-expression-via-template-replacevariable-it-proj-apply-template-but-the-result-fails-to
title: My macro substitutes `it` for a projection expression via `Template.replaceVariable("it", proj) + apply_template`, but the result fails to compile with "can only dereference a reference". What's going wrong?
created: 2026-05-16
last_verified: 2026-05-18
links: []
---

Post-typer, reads of a `var` local appear wrapped as `ExprRef2Value(ExprVar(name))` — the invisible adapter the typer inserts to dereference a reference for its value. `templates_boost.TemplateVisitor.visitExprVar` (the engine behind `Template.replaceVariable + apply_template`) only matches the inner `ExprVar` and replaces IT with a clone of the substitute. The outer `ExprRef2Value` wrapper stays, but now it wraps a non-reference value — compile error `30921: can only dereference a reference`.

This is the same `ExprRef2Value`-transparency problem `daslib/ast_match.das` documents for `qmatch` — they solve it on both pattern and source sides via `qm_peel_ref2value`. `apply_template` does NOT auto-peel.

Two fixes for substitution:

1. **Pre-peel the destination** before `apply_template`: walk `dst` and replace every `ExprRef2Value(ExprVar(name))` with the inner `ExprVar(name)` first. After substitution, the result is clean. Drawback: removes wrappers globally (around other identifiers too) — if other refs still need the wrapper, the typer will re-insert them, but you've added a pass.

2. **Use a custom visitor instead of `Template.replaceVariable`**: override `visitExprRef2Value` to detect `ExprRef2Value(ExprVar(name))` and return `clone_expression(replacement)` directly (stripping the wrapper as part of the substitution). Override `visitExprVar` as a fallback for bare ExprVars. The pattern mirrors `qm_peel_ref2value`'s "peel both sides" approach.

Concrete repro: daslang `linq_fold`'s Phase 2A planner tried to fuse chained `_select|_select` via `substitute_it_for(proj2, "it", proj1)`. proj1 was `it * 2` (where `it` is the typed-and-wrapped loop var), proj2 was `it + 1`. Substituting via Template replaced the inner ExprVar in proj2 but left `ExprRef2Value(it * 2) + 1` — type error. The fix was deferred (chained-select falls through unfolded in Phase 2A) but Phase 2B needs option 2.

See `skills/das_macros.md` "Peel ExprRef2Value before qmatch" for the matcher-side analog.

**Update 2026-05-18 (PR #2712):** The substitution-side helper has landed as `replaceVariablePeeling` in `daslib/templates_boost.das`. Same signature as `replaceVariable` — populates a new `var2exprPeeling : table<string; Expression?>` field on `Template`. The `TemplateVisitor` gets a `visitExprRef2Value` override that detects `ExprRef2Value(ExprVar(name))` for any peeling-registered name and returns `clone_expression(replacement)` directly (Option 2 above). First user is `fold_linq_cond_peel` in `daslib/linq_fold.das` for the `_select |> _where |> terminator` splice arm; bails to tier-2 cascade when `has_sideeffects(projection)` to avoid double-evaluation.

When to choose `replaceVariablePeeling` over `replaceVariable`: any time you substitute into already-typed AST. The typer's `ExprRef2Value` wrappers are invisible in the IDE outline but real in the AST.

## Questions
- My macro substitutes `it` for a projection expression via `Template.replaceVariable("it", proj) + apply_template`, but the result fails to compile with "can only dereference a reference". What's going wrong?
- ExprRef2Value blocker — `_select|_where` (where-after-select) in `_fold` splice — macro substitutes `it` for projection via `Template::replaceVariable`, compile error "can only dereference a reference"
- How do I peel `ExprRef2Value` during typed-AST substitution? When should I use `replaceVariablePeeling` over `replaceVariable`?
- Why does my `apply_template` substitution leave a `30921` error around the substituted expression?
