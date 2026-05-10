---
slug: daslang-enum-value-as-default-argument-gen2
title: Can I write `def foo(flags : ImGuiSelectableFlags = ImGuiSelectableFlags.None)` in daslang gen2 — i.e. enum value as a default argument?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

Yes. Daslang gen2 accepts any const expression as a default arg, including enum values, named struct constructors, and `float2(0,0)`-style scalar constructors.

Confirmed working signatures from `modules/dasImgui/widgets/imgui_widgets_builtin.das` (Phase 0b.1):
```das
[widget]
def selectable(var state : ToggleState; text : string;
               flags : ImGuiSelectableFlags = ImGuiSelectableFlags.None;
               size : float2 = float2(0.0f, 0.0f)) : bool { ... }

[widget]
def invisible_button(var state : ClickState; text : string; size : float2;
                     flags : ImGuiButtonFlags = ImGuiButtonFlags.None) : bool { ... }

[widget]
def image_button(var state : ClickState; text : string; user_texture_id : void?; size : float2;
                 uv0 : float2 = float2(0.0f, 0.0f);
                 uv1 : float2 = float2(1.0f, 1.0f);
                 bg_col : float4 = float4(0.0f, 0.0f, 0.0f, 0.0f);
                 tint_col : float4 = float4(1.0f, 1.0f, 1.0f, 1.0f)) : bool { ... }
```

All compile_check + lint clean. Default expressions are evaluated at the call site (per-call), so they're real defaults — not interned.

**Call macro caveat:** if the function is wrapped in a `[widget]`/`[function_macro]` annotation that destructures named-tuple call args (`(text="x", flags=...)` form) into positional, the macro must respect default-arg positions. The dasImgui `WidgetCallMacro` does this correctly: it pushes user-provided args in source order; trailing defaults fill in.

## Questions
- Can I write `def foo(flags : ImGuiSelectableFlags = ImGuiSelectableFlags.None)` in daslang gen2 — i.e. enum value as a default argument?
