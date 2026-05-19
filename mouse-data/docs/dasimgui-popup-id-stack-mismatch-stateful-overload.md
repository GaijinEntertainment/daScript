---
slug: dasimgui-popup-id-stack-mismatch-stateful-overload
title: Why doesn't my dasImgui popup open when I call open_popup or open_popup_on_item_click with a string id outside the popup/popup_modal container body? The trigger fires but the container's BeginPopup returns false.
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

**Root cause:** ID-stack mismatch between the trigger and the container.

`open_popup(str_id)` and `open_popup_on_item_click(str_id, flags)` compute `GetID(str_id)` at the **call site's** ID stack — usually outside any `[container]`. But the `popup`/`popup_modal` containers run their `BeginPopup(text)` / `BeginPopupModal(text)` **INSIDE** the internal `PushID(IDENT)` frame emitted by the `[container]` macro's `widget_prelude`. The two IDs hash differently — the trigger opens one popup slot, the container reads a different slot, the popup never displays.

Two real bugs of this shape shipped (round-1/round-2 of dasImgui PR #51):
1. `app_custom_rendering` canvas: `open_popup_on_item_click("context", RMB)` outside `popup(CR_CV_CTX_MENU)` — right-click never opened the ctx menu.
2. `app_documents` close-confirm: `open_popup("Save?")` outside `popup_modal(DOCS_CLOSE_MODAL)` — dirty-doc close requests queued forever.

**Fix:** Use the **stateful overloads** that defer the OpenPopup into the container's PushID scope. They live in `widgets/imgui_containers_builtin.das`:

```daslang
// Imperative open (set pending_open; container's next visit calls OpenPopup INSIDE its PushID frame).
open_popup(state : PopupState)

// Right-click-on-previously-submitted-item open (detects IsMouseReleased + IsItemHovered).
open_popup_on_item_click(var state : PopupState; flags : ImGuiPopupFlags = MouseButtonRight)
```

Call sites:

```daslang
popup(MY_POPUP, (text = "ctx")) {
    text("...")
}
if (button(OPEN_BTN, (text="Open"))) {
    open_popup(MY_POPUP)                      // stateful
}
// or, for right-click on the previous item:
some_item_widget(...)
open_popup_on_item_click(MY_POPUP, ImGuiPopupFlags.MouseButtonRight)  // stateful
```

The string-id overloads (`open_popup(str_id, flags)` / `open_popup_on_item_click(str_id, flags)`) are still useful for popups that are opened from inside the same ID-stack frame as the BeginPopup (e.g. when the trigger and the container both live inside `with_id` / a deeper container) — but for the typical "trigger outside the container body" pattern, ALWAYS use the stateful state-arg form. Their docstrings cross-reference and call this out.

Mnemonic: if your popup is registered as `<parent>/MY_POPUP` in snapshots (so the container's PushID landed) but the trigger doesn't open it, the trigger's GetID hashed at a stack frame ABOVE that PushID. Switch to the state-arg overload.

## Questions
- Why doesn't my dasImgui popup open when I call open_popup or open_popup_on_item_click with a string id outside the popup/popup_modal container body? The trigger fires but the container's BeginPopup returns false.
