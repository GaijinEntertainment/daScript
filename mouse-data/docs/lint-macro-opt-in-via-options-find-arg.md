---
slug: lint-macro-opt-in-via-options-find-arg
title: How do I gate a [lint_macro] on an options flag (opt-in or opt-out)?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

# Gate a [lint_macro] via `prog._options |> find_arg("...")`

Two flavors of the same plumbing — only the `if` direction differs:

- **Opt-in (default off)** — `daslib/style_lint.das` `_comment_hygiene` family. Lint registers via `[lint_macro]`, runs only when `options _flag = true` is set on the consumer file.
- **Opt-out (default on)** — `modules/dasImgui/widgets/imgui_lint.das` after PR #33. Same registration, but runs by default and `options _flag = true` *disables* it. Pairs naturally with `require X public` bundling so consumers can't accidentally skip the lint.

```das
// OPT-IN shape (default off):
[lint_macro]
class MyLintMacro : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        let enabled = prog._options |> find_arg("_my_flag") ?as tBool ?? false
        if (!enabled) return true                      // skip when flag absent
        var v = new MyLintVisitor()
        make_visitor(*v) $(adapter) {
            visit_module(prog, adapter, prog.getThisModule)
        }
        unsafe { delete v; }
        return true
    }
}
```

```das
// OPT-OUT shape (default on) — single line changes:
let opted_out = prog._options |> find_arg("_my_flag") ?as tBool ?? false
if (opted_out) return true                             // skip when flag present
```

Consumer file:

```das
options _my_flag = true       // enable (opt-in) or disable (opt-out)
require my_module/my_lint     // direct require, or transitive via `require X public` from a bundling module
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

- `daslib/style_lint.das` — `_comment_hygiene` family (opt-in, default off).
- `modules/dasImgui/widgets/imgui_lint.das` — `_allow_imgui_legacy` (opt-out, default on; bundled in `imgui/imgui_boost_v2` via `require imgui/imgui_lint public`).

## When to pick which

- **Opt-in** when the lint is experimental, niche, or noisy on existing code that you don't plan to clean up.
- **Opt-out (default on)** when the rule reflects a project-wide invariant the team wants enforced — see the [3 Horsemen](https://daslang.io/blog/the-3-horseman.html) framing. Pair with `require X public` bundling from the headline module so every consumer compile picks up the lint with no opt-in ceremony.

## Questions
- How do I make a [lint_macro] opt-in via an options flag like _comment_hygiene?
- How do I make a [lint_macro] default-on with per-file opt-out?
- When should a lint be opt-in vs default-on with opt-out?
