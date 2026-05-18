---
slug: how-do-i-draw-on-the-imgui-foreground-or-window-drawlist-from-dasimgui-code
title: How do I draw on the imgui foreground or window drawlist from dasImgui code?
created: 2026-05-11
last_verified: 2026-05-18
links: []
---

Use the **drawlist rail** in `imgui/imgui_drawlist_builtin` (auto-required via `imgui/imgui_harness`). Raw `imgui::GetWindowDrawList` / `imgui::AddRect` / `AddRectFilled` / `AddLine` / `GetForegroundDrawList` are no longer on `ALLOWED_IMGUI` as of dasImgui PR #48 — `IMGUI002` flags them. Scaffolding-only escape: `options _allow_imgui_legacy = true`.

```das
require imgui/imgui_drawlist_builtin

let green  = rgba(60u, 220u, 100u, 255u)
let yellow = rgba(240u, 220u, 60u, 255u)

with_window_drawlist() $(var dl) {
    dl |> add_rect_filled(p_min, p_max, color, rounding, ImDrawFlags.None)
    dl |> add_rect(p_min, p_max, color, rounding, ImDrawFlags.None, thickness)
    dl |> add_line(p1, p2, color, thickness)
    dl |> add_circle_filled(center, radius, color, num_segments)
    dl |> add_circle(center, radius, color, num_segments, thickness)
    dl |> add_triangle_filled(a, b, c, color)
    dl |> add_triangle(a, b, c, color, thickness)
    dl |> add_text(pos, color, "label")
}

// Viewport-level variants — paint over EVERY window / under EVERY window
// respectively. Same primitives apply.
with_foreground_drawlist() $(var fdl) {
    fdl |> add_rect_filled(p_min, p_max, color)
}
with_background_drawlist() $(var bdl) {
    bdl |> add_rect_filled(p_min, p_max, tint)
}
```

**Three scope wrappers**: `with_window_drawlist` (current window's drawlist — clipped to the window rect), `with_foreground_drawlist` (viewport-level overlay on top), `with_background_drawlist` (viewport-level underlay beneath all windows).

**Eight primitives** so far: `add_text`, `add_line`, `add_rect`, `add_rect_filled`, `add_circle`, `add_circle_filled`, `add_triangle`, `add_triangle_filled`. Each is tagged `[drawlist_prim(kind="add_X")]` in `widgets/imgui_drawlist_builtin.das`. The macro emits a `<mod>:<line>:<col>` path-key per call site (mouse-cards / playwright targeting) WITHOUT registering a `__widget_<line>_<col>` state-global — drawlist is render-only. Multiple `add_line` per frame don't collide because nothing's stored. See `dasimgui-drawlist-prim-macro-path-key-only-no-state-global-render-only-rail`.

**Container nesting is lexical, not visual.** Calling `with_window_drawlist` INSIDE `window(DRAW_WIN, ...) { … }` nests its primitives under `DRAW_WIN/`. Calling `with_foreground_drawlist` OUTSIDE that block produces top-level paths even though the foreground drawlist is viewport-scoped. The split is by lexical container scope at the call site, not by which drawlist gets painted.

Coordinate space: ImGui drawlist coords are screen-space pixels. For widget-relative drawing inside `with_window_drawlist`, use `GetCursorScreenPos()` at the band start as the origin, or pull rects from dasImgui's per-frame registry (`widget_rect(target_path)` in `imgui/imgui_boost_runtime`).

Reference feature + tests: `examples/features/drawlist.das`, `tests/integration/test_drawlist_primitives.das`, `test_with_window_drawlist.das`, `test_drawlist_path_key.das`. Tutorial: `doc/source/tutorials/drawlist.rst` + recording `examples/tutorial/drawlist.das`.

**Historical note** (pre-PR-#48): the rail-less idiom was `*GetWindowDrawList() |> AddRect(...)` — dereffing the `ImDrawList?` return then piping into the bound method. No `unsafe { ... }` wrap was needed. That form still compiles where the legacy escape is on (`options _allow_imgui_legacy = true`), but the rail is the documented surface and gets the path-key telemetry "for free".

## Questions
- How do I draw on the imgui foreground or window drawlist from dasImgui code?
