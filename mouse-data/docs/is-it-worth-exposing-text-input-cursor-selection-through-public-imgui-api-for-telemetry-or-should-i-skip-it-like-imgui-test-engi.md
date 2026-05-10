---
slug: is-it-worth-exposing-text-input-cursor-selection-through-public-imgui-api-for-telemetry-or-should-i-skip-it-like-imgui-test-engi
title: Is it worth exposing text-input cursor / selection through public ImGui API for telemetry, or should I skip it like imgui_test_engine does?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**Skip it.** ImGui's public API doesn't expose `InputTextState` (cursor / selection lives in `imgui_internal.h`'s `ImGuiInputTextState`). Even imgui_test_engine — the most thorough imgui test framework — never bothers with it.

Clean-room read of `e:\imgui_test_engine\imgui_test_engine\imgui_te_context.cpp:3258-3270`: their `ItemReadAsString(ref)` works by:
1. `ItemInput(ref)` — focus the input.
2. `KeyPress(ImGuiKey_A | ImGuiMod_Ctrl)` — select all.
3. `KeyPress(ImGuiKey_C | ImGuiMod_Ctrl)` — copy to clipboard.
4. `KeyPress(ImGuiKey_Enter)`.
5. `ImGui::GetClipboardText()` — read it back.

No `GetInputTextState` reference anywhere in the repo. Cursor/selection isn't part of their reportable surface either.

**Implication for dasImgui:** the **value** of an input field is what playwright tests need, and it's already in our `InputTextState.value` field, surfaced by the standard serializer. Cursor / selection would be visual-aids material (Phase 4 — show where the user is typing during tutorial overlays), not playwright-test material.

This came up in dasImgui Phase 1.2 planning when I was scoping an "extras channel" for transient runtime data. The clean-room read killed cursor/selection as a justification, leaving only scroll position — which already fits inline in container state structs (`ChildState.scroll`, `WindowState.scroll`). So the extras channel itself got dropped from Phase 1.2 entirely. Re-introduce only when a real consumer needs data that genuinely doesn't fit a state-struct field.

## Questions
- Is it worth exposing text-input cursor / selection through public ImGui API for telemetry, or should I skip it like imgui_test_engine does?
