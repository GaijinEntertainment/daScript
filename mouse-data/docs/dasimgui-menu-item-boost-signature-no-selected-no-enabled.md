---
slug: dasimgui-menu-item-boost-signature-no-selected-no-enabled
title: Does the dasImgui `menu_item` boost widget take `selected` and `enabled` named-tuple args?
created: 2026-05-14
last_verified: 2026-05-15
links: []
---

**As of 2026-05-15: partially yes, partially split.** `menu_item` now takes `enabled`; the `selected` use case moved to a sibling widget `menu_label`. See `dasimgui-menu-item-vs-menu-label-when-to-use-each.md` for the picker rule.

Current signatures (`widgets/imgui_widgets_builtin.das`, v2 boost mega-detour):

    [widget]
    def menu_item(var state : ToggleState; text : string;
                  shortcut : string = ""; enabled : bool = true) : bool   // toggleable

    [widget]
    def menu_label(var state : ClickState; text : string;
                   shortcut : string = ""; selected : bool = false;
                   enabled : bool = true) : bool                          // static

`menu_item`'s `selected` arg was deliberately NOT added — `state.value` carries the toggle, exposing a separate `selected` would muddy the type. Static-selected (permanent check) goes through `menu_label(selected=true)`. Static-disabled (section header) goes through `menu_label(enabled=false)`.

---

**Historical note — what this card said before 2026-05-15:**

> No — the boost `menu_item` widget only takes `text` and `shortcut`. Despite ImGui's `MenuItem(label, shortcut, selected, enabled)` C++ signature exposing four args, the boost wrapper omits `selected` and `enabled`.

That was true through 2026-05-14 (port_use_boost_not_raw / v2-gap detour planning surfaced the gap). The mega-detour PR commit 7 (`0348287`) closed it.

Bit Boris originally 2026-05-14 writing the `popup_context_item.das` feature demo.

## Questions
- Does the dasImgui `menu_item` boost widget take `selected` and `enabled` named-tuple args?
