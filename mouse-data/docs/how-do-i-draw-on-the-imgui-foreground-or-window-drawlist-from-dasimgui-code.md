---
slug: how-do-i-draw-on-the-imgui-foreground-or-window-drawlist-from-dasimgui-code
title: How do I draw on the imgui foreground or window drawlist from dasImgui code?
created: 2026-05-11
last_verified: 2026-05-11
links: []
---

Use the pipe pattern, **without** wrapping the call in `unsafe { ... }`:

```das
*GetForegroundDrawList() |> AddRect(p_min, p_max, color, rounding, ImDrawFlags.None, thickness)
*GetWindowDrawList()      |> AddRectFilled(p_min, p_max, color)
*GetWindowDrawList()      |> AddText(pos, color, text_str)
*GetWindowDrawList()      |> AddCircleFilled(center, radius, color, num_segments)
*GetWindowDrawList()      |> AddLine(p1, p2, color, thickness)
```

`GetForegroundDrawList()` and `GetWindowDrawList()` return `ImDrawList?`; dereffing with `*` and piping into the bound `Add*` methods works directly. Many examples in `modules/dasImgui/example/imgui_demo.das` (e.g. lines 753, 953, 1674, 2689-2695).

**Do NOT wrap in `unsafe { ... }`.** Even though dereffing a pointer normally needs unsafe, the bound `Add*` shims handle it themselves — wrapping the pipe call in `unsafe` causes it to **silently no-op in interpreted/live mode** (call returns, no exception, draw command never reaches the foreground drawlist's command buffer). Root cause is still under investigation (see project memory `project_dasimgui_unsafe_drawlist_noop`); workaround is just to skip the wrapper.

Coordinate space: ImGui drawlist coords are screen-space pixels. For widget-relative drawing you typically use `GetItemRectMin/Max` or values out of dasImgui's per-frame registry (`widget_rect(target_path)` in `imgui/imgui_boost_runtime`).

Verified: `widgets/imgui_visual_aids.das` paints all three primitives (highlight rect, mouse trail dots, narrate box + connector line) via this pattern without any `unsafe` wrapping.

## Questions
- How do I draw on the imgui foreground or window drawlist from dasImgui code?
