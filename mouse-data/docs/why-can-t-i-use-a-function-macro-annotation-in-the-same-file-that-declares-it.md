---
slug: why-can-t-i-use-a-function-macro-annotation-in-the-same-file-that-declares-it
title: Why can't I use a [function_macro] annotation in the same file that declares it?
created: 2026-05-09
last_verified: 2026-05-09
links: []
---

Annotations declared via `[function_macro(name="foo")] class FooMacro : AstFunctionAnnotation { ... }` are not visible as the `[foo]` annotation form **within the same source file**. Trying to use `[foo] def something(...)` next to the class definition errors with:

```
error[20600]: annotation foo is not found
```

**Why:** annotation registration happens during the parse-pass for the file. Within the same file, the typer hasn't finalized the registration yet when it tries to apply the annotation to a sibling decl. Same-file forward reference doesn't work for annotation classes (unlike, say, function names).

**Workaround:** put the macro CLASS in one module file, and the `[foo]`-decorated functions in a separate file that `require`s the first.

```
widgets/imgui_boost.das          // [function_macro(name="widget")] class WidgetFunctionMacro
widgets/imgui_widgets_builtin.das // require imgui/imgui_boost_v2 public; [widget] def button(...)
```

The second file's `[widget]` resolves fine because the imported module already finished parsing (and registering the annotation) before this file's typer pass runs.

**Same applies to `[call_macro]`, `[structure_macro]`, etc.** Any user-defined annotation class needs to live in a different file from its first usage.

Canonical example: `daslib/decs_boost.das` declares `[function_macro(name="decs")] class DecsEcsMacro`, and `[decs(stage=update_ai)] def update_ai(...)` decorations live in user files (e.g. `decs_passes.das`) that `require daslib/decs_boost`.

**Surfaced 2026-05-09** during dasImgui Phase 0a; tried to declare button/slider widgets right next to `[function_macro(name="widget")] class WidgetFunctionMacro`. Errored. Split into a sibling file, worked.

last_verified: 2026-05-09

## Questions
- Why can't I use a [function_macro] annotation in the same file that declares it?
