---
slug: dasimgui-widget-named-tuple-args-positional-not-named
title: In dasImgui's `[widget]` call_macro, do named-tuple args `(name = val, …)` dispatch by NAME like Python kwargs, or by SOURCE-ORDER position?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**By source-order position. The names are documentation only.**

The `[widget]` call_macro destructures `(text = "X", flags = …, …)` by iterating the `ExprMakeTuple.values` array in stored order and pushing them as positional args (see `imgui_boost.das:209-221`). The corresponding `recordNames` array is checked only as a guard to enable destructuring (non-empty + same length as values), but the names themselves never reach the typer's overload resolution.

Practical consequences for callers:

1. **Source order MUST match function signature order.** If a widget def is `def color_picker4(state, text, flags, ref_col, has_ref_col)`, then the caller must write `(text = …, flags = …, ref_col = …, has_ref_col = …)` in that exact order. Writing `(text = …, has_ref_col = true, ref_col = …)` puts `has_ref_col=true` (a `bool`) into the `flags` positional slot — type mismatch error 30341.

2. **You can't skip middle args.** Stopping at the last arg you care about is fine (trailing defaults take over) — but to set arg N, you must pass args 1..N-1. Meaning you can't surface a "rare last default" without writing every preceding default explicitly.

3. **Param ordering should put rarely-overridden args last.** When designing a widget def, list params from "always-customized" → "rare default-override". Then 90% of callers write a short `(text = …)` form; the 10% that need the rare arg write the full chain.

4. **For widgets with two optional middle args + a common-skip last arg**, sometimes the right move is to reorder the wrapper's signature against ImGui's. E.g. `color_button` puts `size` before `flags` (more callers override size than flags, so the skip-tail is `flags`). The wrapper body still calls `ColorButton(desc_id, col, flags, size)` — the wrapper's param order is independent of the underlying ImGui call's order.

5. **Avoid square-bracket `[name = val]`** — that builds an `ExprNamedCall` which bypasses call_macro dispatch entirely (= no auto-emit, no L2/L3 dispatcher install). The parens form is mandatory.

**Possible upgrade to the macro:** use `recordNames` to do real name → param-position mapping (Python-style kwargs). Would let callers skip middle args (`(text = "X", has_ref_col = true)` works regardless of where `has_ref_col` sits in the signature). The current macro doesn't do this — it would need to read `func.arguments` after the macro fires, match record names to param names, and reorder before calling. Real improvement; out of 0b.2 scope. Mention if a real user trips on this.

**Found 2026-05-10**, dasImgui phase 0b.2-color (ColorPicker4's `ref_col` + `has_ref_col` couldn't be set without explicit `flags = ImGuiColorEditFlags.None` middle arg).

## Questions
- In dasImgui's `[widget]` call_macro, do named-tuple args `(name = val, …)` dispatch by NAME like Python kwargs, or by SOURCE-ORDER position?
