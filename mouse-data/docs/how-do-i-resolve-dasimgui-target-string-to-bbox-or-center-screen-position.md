---
slug: how-do-i-resolve-dasimgui-target-string-to-bbox-or-center-screen-position
title: How do I resolve a dasImgui target string (registry path or hex_id) to its bounding box rect or center position in screen coordinates?
created: 2026-05-12
last_verified: 2026-05-12
links: []
---

**Use `widget_rect(target : string) : tuple<bool; float4>`** from `imgui_boost_runtime` ([modules/dasImgui/widgets/imgui_boost_runtime.das:1251](https://github.com/GaijinEntertainment/daScript/blob/master/modules/dasImgui/widgets/imgui_boost_runtime.das#L1251)).

```das
let r = widget_rect("VOLUME")          // registry path
// or
let r = widget_rect("0x4f3a2b1c")      // hex_id

if (r._0) {
    let bb = r._1                       // float4(x0, y0, x1, y1) screen coords
    let center = float2((bb.x + bb.z) * 0.5f, (bb.y + bb.w) * 0.5f)
}
```

**Returns:** `(found : bool, bbox : float4)`. `bbox` is `(x0, y0, x1, y1)` in screen-space pixels — the same coords imgui uses for `GetForegroundDrawList()` draws. When not found, returns `(false, float4(0,0,0,0))`.

**Resolution order:**
1. Registry path lookup in `g_registry[target]` (the Phase 1 per-frame registry of named widgets keyed by their identifier path, e.g. `"Setup/SAVE_BTN"`).
2. If that misses, parses `target` as a hex_id (8-hex-digit string, with or without `0x` prefix), looks up `g_id_to_bbox[hid]`. Hex_id is `ImGui::GetID()` of the widget — survives across-frame caching for widgets that aren't currently rendered.

Both tables are populated by every boost wrapper's `widget_finalize` call during `update()`. A widget that didn't render this frame is absent from `g_registry` but may still be in `g_id_to_bbox` from the last frame that rendered it.

**Center helper (private):** `resolve_target_bbox(target) : tuple<bool; float2>` at [imgui_boost_runtime.das:1263](https://github.com/GaijinEntertainment/daScript/blob/master/modules/dasImgui/widgets/imgui_boost_runtime.das#L1263) — same lookup chain, returns `(found, center_xy)` directly. Currently `private` (intended for internal use by `drag_coro` and friends). If you need the center from outside the module, compute it from `widget_rect`'s float4 inline.

**Used by:** `paint_highlights` ([imgui_visual_aids.das:163](https://github.com/GaijinEntertainment/daScript/blob/master/modules/dasImgui/widgets/imgui_visual_aids.das#L163)), `paint_narrates` ([imgui_visual_aids.das:217](https://github.com/GaijinEntertainment/daScript/blob/master/modules/dasImgui/widgets/imgui_visual_aids.das#L217)). Any new overlay drawing on widget bboxes follows this pattern — don't re-walk `g_registry` yourself.

## Questions
- How do I resolve a dasImgui target string (registry path or hex_id) to its bounding box rect or center position in screen coordinates?
- Where do I find a widget's screen-space bbox from a path like "Setup/SAVE_BTN"?
- What's the difference between g_registry and g_id_to_bbox for widget lookup?
- How do dasImgui overlays (highlight, narrate) resolve targets to draw positions?

## Questions
- How do I resolve a dasImgui target string (registry path or hex_id) to its bounding box rect or center position in screen coordinates?
