---
slug: lint-macro-opt-in-via-options-find-arg
title: How do I make a [lint_macro] opt-in via an options flag like _comment_hygiene?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

# Opt-in [lint_macro] via `prog._options |> find_arg("...")`

The `daslib/style_lint.das` pattern for `_comment_hygiene` / `_no_imgui_legacy` style options: the lint registers via `[lint_macro]` (so the runner always loads it), but checks an options flag in `apply()` and returns early when unset. Off-by-default, opt-in per file via `options _flag = true`.

```das
[lint_macro]
class MyLintMacro : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        let enabled = prog._options |> find_arg("_my_flag") ?as tBool ?? false
        if (!enabled) return true
        var v = new MyLintVisitor()
        make_visitor(*v) $(adapter) {
            visit_module(prog, adapter, prog.getThisModule)
        }
        unsafe { delete v; }
        return true
    }
}
```

User opts in at the top of their consumer file:

```das
options _my_flag = true
require my_module/my_lint
```

## Pieces

- **`prog._options`** is the program-level options bag (annotation-arg list).
- **`find_arg("name")`** returns `AnnotationArgument | null`.
- **`?as tBool`** extracts the bool case from the variant; `?? false` gives the absent-or-not-bool default.
- **Always return `true` from `apply`** unless you want to abort the compile. The lint emits errors via `compiling_program() |> macro_error(at, "...")` and that's what fails the build — `return false` would be a hard abort with no diagnostic.
- **`visit_module(prog, adapter, prog.getThisModule)`** scopes the walk to the consumer file only. Transitively-required modules don't get walked (so library code with deliberate "forbidden" patterns inside it doesn't self-flag).

## Why the always-load + check-flag-in-apply shape

`[lint_macro]` is registered at module-require time, before the consumer's `options` are parsed. You can't skip registration based on an option. So the macro always loads, but exits cheaply when the flag is absent. Cost when disabled: one `find_arg` lookup per compile.

## Reference implementations

- `daslib/style_lint.das` — `_comment_hygiene` family.
- `modules/dasImgui/widgets/imgui_lint.das` — `_no_imgui_legacy` (PR #29).

## Questions
- How do I make a [lint_macro] opt-in via an options flag like _comment_hygiene?
