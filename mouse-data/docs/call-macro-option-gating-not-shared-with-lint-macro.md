---
slug: call-macro-option-gating-not-shared-with-lint-macro
title: When my [call_macro] emits a lint-like macro_error, does it inherit the lint_macro's options gating, or do I need to check the option flag independently?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

# `[call_macro]` option gating is independent from `[lint_macro]` — check the flag yourself

`[lint_macro]` and `[call_macro]` are two separate AST extension points with separate firing schedules. A `[lint_macro]`'s `apply()` can early-return when its option flag is absent (or present, for default-on/opt-out shapes), and the visitor stops. But a `[call_macro]` fires every time the matching call shape appears in the source — it doesn't know or care what the lint_macro decided.

So if you add a "lint-like" check inside a `[call_macro].visit()` — e.g., STYLE001 inside `WidgetCallMacro.visit()` rejecting a verbose call shape — that check **fires unconditionally** unless you also read the flag.

## Concrete: STYLE001 in WidgetCallMacro (dasImgui PR #33)

```das
class WidgetCallMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : ExpressionPtr {
        // ... Form 2 detection ...
        if (length(expr.arguments) == 1 && length(mtCheck.recordNames) == 1
                && (mtCheck.values[0] is ExprConstString || mtCheck.values[0] is ExprStringBuilder)
                && accepts_positional_string()) {
            let firstParamName = string(render_fn.arguments[2].name)
            let tupleFieldName = string(mtCheck.recordNames[0])
            // CRUCIAL: re-check the option flag here, the lint_macro's check doesn't reach us
            let opted_out = prog._options |> find_arg("_allow_imgui_legacy") ?as tBool ?? false
            if (firstParamName == tupleFieldName && !opted_out) {
                macro_error(prog, expr.at, "STYLE001: ...")
                return <- default<ExpressionPtr>
            }
        }
        // ...
    }
}
```

Without the `opted_out` re-check, `options _allow_imgui_legacy = true` would correctly suppress IMGUI001/IMGUI002 (which live in the lint_macro) but STYLE001 (which lives in the call_macro) would still fire — a single opt-out flag that only half-works.

## Why this surprised me

The mental model was "the option turns off the lint" — but the lint is split across two extension points. The single user-facing option needs to be threaded into every place that emits the lint's diagnostics. The fix is mechanical (one `find_arg` call per emit site) but it's an easy thing to forget.

## Rule of thumb

If a lint family is split across multiple macro entry points (e.g., a `[lint_macro]` for the visitor walk + a `[call_macro]` for call-shape checks), the opt-out flag must be checked in **every** entry point that emits diagnostics. Centralize the check in a helper (`is_lint_opted_out(prog)`) if you have more than two emit sites.

## Questions
- When my [call_macro] emits a lint-like macro_error, does it inherit the lint_macro's options gating?
- Does `options _allow_imgui_legacy = true` suppress STYLE001 emitted from WidgetCallMacro?
- How do I share an opt-out flag between a [lint_macro] and a [call_macro]?
- Why does my opt-out flag suppress some lint codes but not others?

## Questions
- When my [call_macro] emits a lint-like macro_error, does it inherit the lint_macro's options gating, or do I need to check the option flag independently?
