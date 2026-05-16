---
slug: imgui-macos-configmacosxbehaviors-shortcut-is-super-not-ctrl
title: Why does synth Ctrl+A do nothing on macOS but works on Linux/Windows in an ImGui InputText test?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

**ImGui sets `io.ConfigMacOSXBehaviors = true` automatically on macOS** (`#ifdef __APPLE__` in `ImGuiIO` ctor). When that flag is true, the **"shortcut key" is Super (Cmd), not Ctrl**.

So on macOS:
- `Ctrl+A` → "move to start of line" (text-editing convention)
- `Cmd+A` → "select all"
- `Ctrl+Backspace` → does not delete word
- `Cmd+Backspace` → "delete to start of line"

A synth-IO test that drives `Ctrl+A` to clear an InputText buffer **fails silently on macOS** — the assertion message will look like "NAME_INPUT.value == "" after Ctrl+A then Backspace" with the buffer still holding `"abc"` (because Ctrl+A moved cursor to start, then Backspace deleted nothing).

**Detection from a playwright test**: snapshot `io.config_macos_behaviors` and branch the chord. dasImgui's `widgets/imgui_boost_runtime.das` exposes the flag via the `io_jv()` snapshot key `"config_macos_behaviors"`:

```daslang
let macos = snap0?["io"]?["config_macos_behaviors"] ?? false
if (macos) {
    post_command(d, "imgui_key_chord", JV((mods = ["Super"], key = "A")))
} else {
    post_command(d, "imgui_key_chord", JV((mods = ["Ctrl"], key = "A")))
}
```

**Why we don't unconditionally use Super**: on Linux/Windows there's no Super-key binding for select-all in InputText; the wrong choice silently fails on those platforms instead. Branch on the actual `config_macos_behaviors` flag — works everywhere.

Discovered in dasImgui PR #38 `test_click_then_ctrl_a_clears_input` failing only on macOS CI after the playwright cascade fix made other failures observable. Copilot diagnosed and fixed in commit `42b7292` + the IO-snapshot extension. Caveat: Copilot drafted the fix with `if macos { ... }` (gen1 syntax) — gen2 requires `if (macos) { ... }`, watch for that pattern when ferrying AI suggestions through.

**Related ImGui flags worth knowing**:
- `io.KeyCtrl` reflects either Ctrl OR (Cmd on macOS when `ConfigMacOSXBehaviors`) — the "shortcut" lookups go through `io.KeySuper` instead on macOS.
- `ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_A)` automatically maps to Cmd on macOS — but `Shortcut(ImGuiMod_Super | ImGuiKey_A)` does NOT remap, only `ImGuiMod_Ctrl` is the platform-aware "primary modifier".

## Questions
- Why does synth Ctrl+A do nothing on macOS but works on Linux/Windows in an ImGui InputText test?
