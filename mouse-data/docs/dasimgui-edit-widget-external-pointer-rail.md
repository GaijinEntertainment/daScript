---
slug: dasimgui-edit-widget-external-pointer-rail
title: How does the dasImgui `edit_*` external-pointer widget rail work?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

**Caller owns the pointee; the widget edits it through a `T?` pointer.** The `edit_*` family parallels the state-managed boost widgets (`slider_float` / `checkbox` / `combo` / …) but doesn't auto-emit a state global — the value already lives somewhere else (a `var private`, a struct field, a C-side buffer).

Call shape:

    edit_kind(<ptr>, (id = "STABLE_IDENT", text = "label", ...other_named_args))

The `id="..."` literal is REQUIRED — it keys the telemetry path. The `EditExternalCallMacro` hard-errors at compile time without it.

## Pointer expressions

- `safe_addr(g_global)` — for plain locals/globals. Type-safe + cheap.
- `unsafe(addr(struct.field))` — for struct field access via reference. `safe_addr` rejects field-via-reference because of `is_local_or_global_expr` (`daslib/safe_addr.das:42`).

In `style_editor.das`, every pointee is an `ImGuiStyle` field reached via `*GetStyle()` — so all calls use `unsafe(addr(style.Alpha))` etc. In `editing_external.das` (tutorial), pointees are plain `var private` globals → `safe_addr` works.

## What lands in the call

The full coverage (commits 10a-10c of the v2 boost mega-detour, modules/dasImgui/widgets/imgui_widgets_builtin.das):

- **Scalars** (10a): `edit_slider_{float,int}` / `edit_drag_{float,int}` / `edit_input_{float,int,double}` — 8 overloads
- **Vectors** (10b): `_2`/`_3`/`_4` variants of slider / drag / input — 12 overloads (caller passes `float2?` / `float3?` etc.; the wrapper does `unsafe(reinterpret<float?>(ptr))` at the ImGui call site since SliderFloat2/3/4 + DragFloat2/3/4 + InputFloat2/3/4 + ColorEdit/Picker3/4 all take a single `T*`)
- **Color / Combo / Special** (10c): `edit_color_edit{3,4}` / `edit_color_picker{3,4}` / `edit_combo` / `edit_slider_angle` / `edit_checkbox` / `edit_radio_button` / `edit_checkbox_flags` — 14 overloads
- 34 total `[edit_widget]` overloads

## State / snapshot semantics

- **No auto-emit.** No state-global is created; the macro just records the path key.
- **Deep-copy mirror.** The boost wrapper copies `*ptr` into a hidden per-call mirror at frame end, so the serializer reads the mirror (not `*ptr` directly). Prevents UB if the external owner reallocates between render and snapshot read.
- **Payload:** `{value: T}` for scalars; vector forms emit `{value: {x, y[, z[, w]]}}`.

## Combo specifics

`edit_combo` takes `array<string>` via `items <- ARR`. The wrapper uses the array-of-string-pointers ImGui overload (`addr(items[0])` + `length(items)`); no per-frame zero-separated buffer packing needed — daslang `string` carries the trailing null already.

## `id` requirement rationale

The whole reason `edit_*` exists is stable telemetry paths for caller-owned data. Auto-gen idents (commit 19 `[widget]` change) shift on edits — they'd defeat the purpose. So `id="..."` stays mandatory.

## Questions
- How does the dasImgui `edit_*` external-pointer widget rail work?
- What's the difference between dasImgui boost `slider_float` and `edit_slider_float`?
- Why does `edit_*` need `id="..."` instead of an ident?
- Should I use `safe_addr` or `unsafe(addr(...))` with edit_*?
