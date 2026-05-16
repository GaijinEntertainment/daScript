---
slug: dasimgui-popup-context-flags-none-equals-mousebuttonleft-not-default
title: My popup_context_item / popup_context_window in dasImgui never opens on right-click — I passed `flags = ImGuiPopupFlags.None`. What's the bug?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

**`ImGuiPopupFlags.None == 0 == MouseButtonLeft`** in upstream Dear ImGui. So passing `None` to a context-menu wrapper opens it on **left**-click, not right-click — which looks like "the popup never opens" if you're testing with the natural right-click gesture.

The upstream C++ defaults for `BeginPopupContextItem` / `BeginPopupContextWindow` are `ImGuiPopupFlags_MouseButtonRight` (= 1), but the daslang wrappers in `widgets/imgui_containers_builtin.das` take `flags : ImGuiPopupFlags` with **no default value** — every call site must pass an explicit flag.

**Correct pattern:**

```das
popup_context_item(MY_TITLE_POPUP,
                   (str_id = "##title", flags = ImGuiPopupFlags.MouseButtonRight)) {
    if (menu_item(MY_CLOSE, (text = "Close"))) { ... }
}

popup_context_window(MY_BODY_MENU,
                     (str_id = "##body", flags = ImGuiPopupFlags.MouseButtonRight)) {
    if (selectable_label(MY_CLEAR, "Clear", false)) { ... }
}
```

**Useful alternatives:**

- `ImGuiPopupFlags.MouseButtonDefault_` — semantically clearer "use upstream default" (also `MouseButtonRight`).
- `ImGuiPopupFlags.MouseButtonLeft` — for left-click context menus (rare; e.g. a small drop-down attached to a button).

**Caught by:** PR #32 round 2 review. The `app_console.das` title-bar `popup_context_item` was passing `ImGuiPopupFlags.None` and the right-click "Close Console" menu never fired. Diff: `flags = ImGuiPopupFlags.None` → `flags = ImGuiPopupFlags.MouseButtonRight`.

**Bonus** (same review round): the wrapper docstring requires `str_id` to be a unique non-empty stack id (the C-level `"##ContextWindow"` default only kicks in on a NULL `const char*` which daslang's string binding doesn't expose). Pass a stable `"##..."` id per consumer to avoid id collisions if a second context popup lands in the same parent later.

## Questions
- My popup_context_item / popup_context_window in dasImgui never opens on right-click — I passed `flags = ImGuiPopupFlags.None`. What's the bug?
