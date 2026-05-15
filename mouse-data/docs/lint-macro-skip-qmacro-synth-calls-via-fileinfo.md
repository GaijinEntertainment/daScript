---
slug: lint-macro-skip-qmacro-synth-calls-via-fileinfo
title: How do I filter macro-synthesized ExprCalls from a [lint_macro] AST visitor?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

# Lint-macro AST walkers must skip macro-synthesized calls

Symptom: your `[lint_macro]` walks `prog.getThisModule` and fires on calls the user never wrote. Example: `imgui_lint` flagged raw `PushID`/`PopID` calls inside the user's file — but those calls came from inside a `[container] with_id(...)` macro expansion, not from user source.

Root cause: qmacro emits `ExprCall` nodes whose `at` field points at the **macro template's source file** (e.g. `widgets/imgui_boost.das`), not the consumer's. The visitor sees them while walking the function body in the user's module, because the qmacro splice baked them into that body during infer.

## Fix — combined skip in preVisitExprCall

Track the containing function (`preVisitFunction` / `visitFunction`), then in `preVisitExprCall` skip when EITHER:

1. The containing function is generated (`current_function.flags.generated`) — these are macro-generated function bodies, not user code.
2. The call's `at.fileInfo` doesn't match the containing function's `at.fileInfo` — these are spliced-in macro bodies.

```das
class MyLintVisitor : AstVisitor {
    @do_not_delete current_function : Function?

    def override preVisitFunction(var fn : FunctionPtr) : void {
        current_function = fn
    }
    def override visitFunction(var fn : FunctionPtr) : FunctionPtr {
        current_function = null
        return <- fn
    }
    def override preVisitExprCall(var expr : ExprCall?) : void {
        if (expr.func == null || expr.func._module == null
                || (current_function != null
                    && (current_function.flags.generated
                        || (current_function.at.fileInfo != null
                            && expr.at.fileInfo != current_function.at.fileInfo)))) return
        // ... real lint logic here
    }
}
```

## Why both checks

- `flags.generated` catches helper functions the macro inserts whole (e.g. AOT-emitted thunks). The function itself never existed in user source.
- `fileInfo` mismatch catches the more common case: a user-authored function whose body got spliced bits during infer. The function `at` is user source, but individual `ExprCall` nodes inside it carry the macro-template source's fileInfo.

Without the `fileInfo` check, lint passes fire on every `[widget]` or `[container]` expansion inside the user's function.

## Cross-references

- `modules/dasImgui/widgets/imgui_lint.das` is the canonical reference implementation (PR #29, merged 2026-05-14).
- The pattern is the same one used by `daslib/style_lint.das` for `_comment_hygiene` and friends.
- Scope the walk via `make_visitor(*v) $(adapter) { visit_module(prog, adapter, prog.getThisModule) }` so transitively-required modules aren't walked (the boost itself legitimately calls raw `imgui::*`).

## Questions
- How do I filter macro-synthesized ExprCalls from a [lint_macro] AST visitor?
