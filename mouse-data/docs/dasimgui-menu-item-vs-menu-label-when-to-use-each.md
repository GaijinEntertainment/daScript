---
slug: dasimgui-menu-item-vs-menu-label-when-to-use-each
title: When do I use dasImgui `menu_item` vs the new `menu_label`? Both wrap ImGui's MenuItem(label, shortcut, selected, enabled).
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

Two distinct boost widgets covering the two semantic forms of ImGui's `MenuItem(label, shortcut, selected, enabled)`. Added 2026-05-15 in the v2 boost mega-detour (`widgets/imgui_widgets_builtin.das`).

**`menu_item(IDENT, (text=, shortcut=, enabled=true))`** — **TOGGLEABLE**. ImGui receives `&state.value` as the `bool* p_selected` arg, so clicking toggles the check mark. `state.value` is `@live` and persists across hot-reload. Backed by `ToggleState`. Use for actual user-settable boolean preferences: "Show grid", "Enable foo", "Word wrap".

    menu_item(VIEW_GRID, (text = "Show Grid", shortcut = "G"))
    menu_item(EDIT_UNDO, (text = "Undo", shortcut = "CTRL+Z", enabled = can_undo()))

**`menu_label(IDENT, (text=, shortcut=, selected=false, enabled=true))`** — **STATIC**. `selected` is a plain `bool`, NOT a `bool*` — the widget never mutates it. Backed by `ClickState` (tracks `click_count` for telemetry, no toggle). Use for:
- Disabled section headers: `(text="(demo menu)", enabled=false)` — renders greyed-out non-interactive label
- Permanent check marks: `(text="Checked", selected=true)` — always shows ✓, click doesn't toggle
- Disabled-with-shortcut hints: `(text="Redo", shortcut="CTRL+Y", enabled=false)` — like the C++ `MenuItem("Redo","CTRL+Y",false,false)` idiom

    menu_label(FILE_HEADER, (text="(section)", shortcut="", selected=false, enabled=false))
    menu_label(FILE_CHECKED, (text="Static check", shortcut="", selected=true, enabled=true))

**Gotcha — positional-dispatch on named-tuple args**: daslang dispatches named-tuple fields **positionally**, not by name (`dasimgui-widget-named-tuple-args-positional-not-named.md`). Both widgets order args as `(text, shortcut, ...)` — if you write `menu_label(X, (text=Y, enabled=false))` you'll get a compile error because `enabled=false` lands in `shortcut`'s slot (bool ≠ string). You must spell every middle arg. The compile error catches it; just fill in `shortcut = ""` to skip past it.

**Why the split (instead of one `menu_item` with `selected_override` + `use_selected_override` flags)**: keeps the type meaningful — toggleable has a `value`, static has only a click count. No flag-bool muddying. Confirmed by Boris during plan approval 2026-05-14.

## Questions
- When do I use dasImgui `menu_item` vs the new `menu_label`? Both wrap ImGui's MenuItem(label, shortcut, selected, enabled).
