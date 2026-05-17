---
slug: my-dasimgui-single-scene-harness-driver-has-no-cursor-sprite-in-the-recorded-apng-and-clicks-don-t-open-menus-what-am-i-missing
title: my dasImgui single-scene harness driver has no cursor sprite in the recorded APNG and clicks don't open menus — what am I missing?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

# harness_apply_synth_io() is the missing link

The dasImgui harness API exposes `harness_apply_synth_io()` as an
**optional** call (per `widgets/imgui_harness.das:163` docstring). When
a harness is **driven by synth IO** — i.e. a `record_*.das` script
posting `imgui_mouse_play` / `imgui_key_play` via daslang-live — that
call is **mandatory**, not optional. Insert it between
`harness_begin_frame()` and `harness_new_frame()`:

    [export]
    def update() {
        if (!harness_begin_frame()) return
        harness_apply_synth_io()       // <-- this line
        harness_new_frame()
        ShowMyDemo()
        harness_end_frame()
    }

## Why

`harness_begin_frame()` calls `ImGui_ImplGlfw_NewFrame()` (windowed mode)
which polls the real GLFW mouse and writes to `io.MousePos`. Without
`harness_apply_synth_io()` after it, the GLFW poll wins the race and
overwrites whatever synth events queued. Result:

- `synth_cursor_owned` stays `false`
- `paint_cursor_sprite` (foreground draw list) paints at `GetMousePos()`
  = real OS cursor (typically off-window) → invisible in recording
- `paint_trail` same story
- synth `mouse_button` press/release events never reach widget hover
  state → menus stay closed, buttons don't activate

The diagnostic warning `synth events queued but
apply_synth_io_override() has never run` (from
`imgui_live_core.das:737`) fires only when `imgui_synth_tick` has run
ZERO times in the process. If you call `apply_synth_io_override()` once
but then SOME frames also skip it (e.g. you forgot to add it inside an
if-branch), the warning DOESN'T fire and you get silent partial drain.

## Verification

1. Run `mcp__daslang__live_command imgui_mouse_status` against the
   harness with synth events queued. Should show `"cursor_owned": true`.
2. `mcp__daslang__live_command screenshot` to a PNG should show the
   red+white cursor sprite at the synth position.
3. If both succeed but the recorded APNG still has no cursor, the
   `imgui_cursor_sprite` / `imgui_mouse_trail` live_commands weren't
   posted before `record_start`. Order matters:

       post_command(app, "imgui_mouse_trail",   JV((enabled = true)))
       post_command(app, "imgui_cursor_sprite", JV((enabled = true)))
       post_command(app, "record_start", JV(...))
       // ... move_to / click_at sequence

## Related

- `widgets/imgui_harness.das:163` — `harness_apply_synth_io` docstring.
- `widgets/imgui_live_core.das:737` — the one-shot warning that fires
  when synth events queue before any tick.
- `examples/imgui_demo/harness_about.das` — canonical example with the
  call in place.
- `examples/features/*.das` — the 8 feature files that already include
  it (active_widget, columns_demo, dock_basic, glfw_synth_*,
  imgui_synth_*, layout_helpers). The rest skip it because their tests
  don't drive synth IO.

## Questions
- my dasImgui single-scene harness driver has no cursor sprite in the recorded APNG and clicks don't open menus — what am I missing?
