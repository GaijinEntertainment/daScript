---
slug: dasimgui-hover-target-widget-variant-pattern-color-button-hover-selectable-hover
title: dasImgui hover-target widget variant pattern (color_button_hover / selectable_hover)
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

**Hover-target [widget] variants** mirror their click/toggle cousins but use `EmptyMarkerState` and return a per-frame `bool hovered` from `IsItemHovered()`. Use when the widget is a *surface* for a hover-driven side-effect (cursor swap, capture override, status preview), not a *trigger*.

Pattern shape (PR-A2 introduced two: `color_button_hover` next to `color_button`, `selectable_hover` next to `selectable`):

```das
[widget]
def color_button_hover(var state : EmptyMarkerState; desc_id : string;
                       col : float4;
                       size : float2 = float2(0.0f, 0.0f);
                       flags : ImGuiColorEditFlags = ImGuiColorEditFlags.None) : bool {
    ColorButton(desc_id, col, flags, size)
    let hovered = IsItemHovered()
    marker_finalize(widget_ident, "color_button_hover", state)
    return hovered
}
```

Key choices:
- **`EmptyMarkerState`** (the same shape backing `bullet` / `same_line`) — there's no per-frame state worth persisting (hover is transient; ImGui re-queries each frame). Caller branches on the returned bool synchronously.
- **`marker_finalize`** — same finalizer the empty-marker widgets use. No L2/L3 dispatcher actions; the path-key is what playwright targets.
- **Distinct name** (`_hover` suffix) rather than overloading the existing widget — kind: in the snapshot is the disambiguator, and tests can `find_widget(snap, path)?["kind"] == "color_button_hover"`. Overloading would conflate them in telemetry.
- **Caller pattern**: `let hovered = widget_hover(IDENT, (...))`; if (hovered) { side-effect }. The regular widget's state holds click/toggle bookkeeping; the hover variant has neither.

When a site needs BOTH click and hover, use the click variant + `IsItemHovered()` after the call — the post-item hover query is ALLOWED in `imgui_lint.das`. No need for a third variant.

See dasImgui PR #47 (PR-A2 of imgui_demo port) for the introduction.

## Questions
- dasImgui hover-target widget variant pattern (color_button_hover / selectable_hover)
