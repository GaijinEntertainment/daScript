---
slug: dasimgui-setitemtooltip-not-bound-substitute-pattern
title: SetItemTooltip(text) errors out in daslang with `no matching functions or generics` — is it bound?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

No — `SetItemTooltip(const char*)` exists in `imgui.h` (line 720 in the bundled fork) but **the daslang binding doesn't expose it**. Calling it produces `error[30341]: no matching functions or generics: SetItemTooltip(string const&)` with "0 total functions with the same name."

Substitute pattern — ImGui itself describes `SetItemTooltip` as a shortcut for the `if (IsItemHovered(ImGuiHoveredFlags_ForTooltip)) SetTooltip(...)` idiom (imgui.h:717), and both `IsItemHovered` and `SetTooltip` ARE bound. So just inline it:

    // Wherever you wanted to write SetItemTooltip(text):
    if (IsItemHovered(ImGuiHoveredFlags.ForTooltip)) {
        SetTooltip(text)
    }

The boost `[widget] set_item_tooltip` (added 2026-05-15 in the v2 boost mega-detour, `widgets/imgui_widgets_builtin.das`) wraps this substitute idiom directly — callers don't see the inlining. Bound functions used by the substitute: `SetTooltip(const char*)`, `IsItemHovered(ImGuiHoveredFlags)`, `ImGuiHoveredFlags.ForTooltip` enum.

If a fresh `SetItemTooltip` binding is desired in the future, add it via `makeExtern<>` in the dasIMGUI C++ glue alongside `SetTooltip` (see `dasIMGUI.func_*.cpp` for the existing binding pattern). Not required as of 2026-05-15 — the substitute is one line of daslang.

## Questions
- SetItemTooltip(text) errors out in daslang with `no matching functions or generics` — is it bound?
