---
slug: why-does-my-lint-macro-fire-on-the-wrapper-module-that-legitimately-uses-the-forbidden-symbols-even-though-i-scope-visit-module
title: Why does my [lint_macro] fire on the wrapper module that legitimately uses the forbidden symbols, even though I scope visit_module to prog.getThisModule?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

# `[lint_macro]` runs per-module — wrapper modules need a defensive opt-out

`[lint_macro]` doesn't fire once per *program* compile. It fires once per *module* compile in the require chain. When `foo.das` requires `imgui_harness` which requires `imgui_harness_lint`, the lint runs:

1. Once with `prog.getThisModule()` = `imgui_harness` (during the harness's own compile pass).
2. Once with `prog.getThisModule()` = the consumer (during `foo.das`'s compile pass).

The `visit_module(prog, adapter, prog.getThisModule)` line scopes the walk correctly each time — but the visited module for pass 1 is the *harness*, whose body legitimately calls into the very surface the lint is supposed to forbid (because that's what the wrapper does).

## Symptom

A clean consumer file fails to compile with diagnostics pointing INTO the wrapper module:

```
error[50503]: HARNESS001: glfw_live::live_create_window is forbidden ...
modules/dasImgui/widgets/imgui_harness.das:102:4
    live_create_window(title, width, height)
    ^^^^^^^^^^^^^^^^^^
```

You stare at the consumer file and there's no `live_create_window` in it. The diagnostic is fired from the *wrapper's* compile pass, then propagates up as the consumer's compile failure.

## Fix — defensive opt-out at the top of the wrapper

Every wrapper / sibling module that the lint touches transitively needs to carry the per-file escape:

```das
options gen2
options _allow_glfw_calls = true        // <-- defensive opt-out

module imgui_harness shared public

require imgui_app           // legitimately uses GLFW/GL — that's the point
require glfw/glfw_boost
require live/glfw_live
// ...
```

This is the same pattern `widgets/imgui_lint.das` documents in its header:

> Wrapper modules under `widgets/` also carry `options _allow_imgui_legacy = true` defensively so they compile cleanly when targeted directly (MCP compile_check, lint, format_file).

It's not just for `compile_check` / `lint` direct-targeting. It also kicks in during the consumer's require chain — every time the wrapper's module is compiled as a dependency.

## Why this is non-obvious

`visit_module(prog, adapter, prog.getThisModule)` reads like "walk only the consumer's functions" — and it does, for the consumer's pass. But the same macro registration also fires for the wrapper's pass, where `getThisModule` *is* the wrapper. The scoping is correct; the surprise is that the lint executes N times across the require chain, not once at the top.

## Don't try to fix in the visitor

Adding a "skip if `current_function._module != this_module`" filter (where `this_module = prog.getThisModule` captured in `apply()`) does *nothing* — the per-pass `getThisModule` already equals `current_function._module` during the wrapper's pass. The wrapper IS thisModule from its own pass's perspective.

The opt-out at the wrapper's source IS the fix. Match the existing convention.

## Reference

- `modules/dasImgui/widgets/imgui_lint.das` header — has the canonical paragraph documenting the convention for `_allow_imgui_legacy`.
- `modules/dasImgui/widgets/imgui_harness.das` line ~5 — carries `_allow_glfw_calls = true` defensively for the same reason.

## Questions
- Why does a [lint_macro] error appear at a line in the wrapper module instead of my consumer file?
- Do I need to filter calls inlined from a different module in my [lint_macro] AST visitor?
- Why does my [lint_macro] run multiple times for one consumer compile?

## Questions
- Why does my [lint_macro] fire on the wrapper module that legitimately uses the forbidden symbols, even though I scope visit_module to prog.getThisModule?
