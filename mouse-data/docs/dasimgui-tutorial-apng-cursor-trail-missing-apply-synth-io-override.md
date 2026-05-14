---
slug: dasimgui-tutorial-apng-cursor-trail-missing-apply-synth-io-override
title: My standalone dasImgui tutorial's APNG recording has no visible mouse cursor or trail — what's missing?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

## Symptom

You write a simple boost-style tutorial (`window(...) { button(...) ... }`), launch it under `daslang-live`, drive it from `tests/integration/record_<slug>.das` (which posts `imgui_mouse_play` via `move_to` / `click_at` etc.), call `record_start`/`record_stop`. The APNG records the widgets but the **cursor sprite and mouse trail never appear** — they were what you wanted to demonstrate the click sequence.

## Cause

The synth cursor sprite + mouse trail are painted by `paint_all()` in `widgets/imgui_visual_aids.das`, but only after `apply_synth_io_override()` has fired in the current frame. `apply_synth_io_override` is what:

1. Picks up the queued `imgui_mouse_play` events,
2. Advances the timeline,
3. Sets `mouse_cursor_x/y` and `synth_cursor_owned=true`,
4. Schedules the sprite + trail draw.

Without that call, ImGui's normal `io.MousePos` chain wins, `synth_cursor_owned` stays false, and `paint_cursor_sprite()` falls back to drawing at `GetMousePos()` — which is either off-screen or stale-from-real-mouse.

## Fix

In the tutorial's `[export] def update()`, call `apply_synth_io_override()` once per frame BEFORE the widget code:

```das
require imgui/imgui_visual_aids

[export] def update() {
    apply_synth_io_override()    // <-- this
    window(MY_WIN, (text = "...", ...)) {
        // widgets
    }
}
```

(`apply_synth_io_override()` is a no-op when there's no synth IO active, so this is safe to call unconditionally — it doesn't change behavior outside record sessions.)

## Where this bit me

`examples/tutorial/boost_basics.das` was the smallest possible boost example, so it didn't require `imgui_visual_aids` at all. When I re-recorded `boost_basics.apng` after the `window(...)` migration, the cursor + trail were invisible. Added the `require` + the one-liner call; trail came back instantly.

The richer tutorials (`widgets_tour.das`, `visual_aids_tour.das`) already require `imgui_visual_aids` and call the override, so they were fine.

## Questions
- My standalone dasImgui tutorial's APNG recording has no visible mouse cursor or trail — what's missing?
