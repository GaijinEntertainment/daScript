---
slug: menuitem-outside-menu-and-p-selected-pointer-form
title: Does ImGui's MenuItem work outside a Menu — i.e. without BeginMenu/EndMenu? Does the p_selected pointer form mutate?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

Yes — `MenuItem(label, shortcut, p_selected : bool?, enabled : bool)` renders fine at any frame position. ImGui paints it as a Selectable-style row; the menu chrome is just where it's typically used.

**Pointer form mutates** `*p_selected` on click — same shape as Checkbox's pointer form. Returns true on the frame the user clicks.

**Bool form** (`MenuItem(label, shortcut, selected : bool, enabled : bool)`) — does NOT mutate, returns true on click. Same as RadioButton bool-vs-pointer split.

dasImgui boost uses the pointer form for the `menu_item` toggle widget (Phase 0b.1):
```das
[widget]
def menu_item(var state : ToggleState; text : string; shortcut : string = "") : bool {
    if (state.pending_toggle) { state.value = state.pending_value; state.pending_toggle = false }
    let changed = MenuItem(text, shortcut, unsafe(addr(state.value)), true)
    state.changed = changed
    toggle_finalize(widget_ident, "menu_item", state)
    return changed
}
```

**Block-arg "menu container" form** (`BeginMenu("File") { menu_item(...); menu_item(...) }`) is separate — that's a `[container]` widget, lands in Phase 0b.3, not 0b.1.

## Questions
- Does ImGui's MenuItem work outside a Menu — i.e. without BeginMenu/EndMenu? Does the p_selected pointer form mutate?
