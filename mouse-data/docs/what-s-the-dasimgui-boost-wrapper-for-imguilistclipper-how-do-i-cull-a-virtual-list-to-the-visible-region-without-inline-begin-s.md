---
slug: what-s-the-dasimgui-boost-wrapper-for-imguilistclipper-how-do-i-cull-a-virtual-list-to-the-visible-region-without-inline-begin-s
title: What's the dasImgui boost wrapper for ImGuiListClipper — how do I cull a virtual list to the visible region without inline Begin/Step/End?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

`list_clipper(items_count, body)` in `widgets/imgui_layout_builtin.das` (added 2026-05-16, PR #40). Takes a `block<(start : int; end_ : int) : void>` callback that fires once per visible chunk with the `[start, end_)` index range.

```daslang
require imgui/imgui_layout_builtin   // brings list_clipper

child(LOG_SCROLL_CHILD, (text = "scrolling", size = float2(0.0f, 0.0f), …)) {
    list_clipper(length(LOG_ITEMS)) <| $(start, end_) {
        for (i in range(start, end_)) {
            text(LOG_ITEM_TEXT[i], (text = LOG_ITEMS[i]))
        }
    }
}
```

The wrapper internally constructs `ImGuiListClipper`, calls Begin/Step/End inside an `unsafe { ... }` block. Replaces the four-line cargo at every consumer:

```daslang
// OLD — inline:
unsafe {
    var clipper = ImGuiListClipper()
    clipper |> Begin(2000, -1.0f)
    while (clipper |> Step()) {
        for (i in range(clipper.DisplayStart, clipper.DisplayEnd)) { … }
    }
    clipper |> End()
}
```

**When NOT to use:** if the visible range isn't random-access by physical row (e.g., filtered log lines where filter rejection breaks the indexed mapping), drop to a plain `for (i in range(length(items)))` since the clipper assumes O(1) random-access to row N. The `app_log.das` port uses both paths: clipper on the unfiltered render, plain `for` on the filtered render.

**Reference:** `widgets/imgui_layout_builtin.das:list_clipper`, used in `examples/imgui_demo/columns.das` (Horizontal Scrolling section) and `examples/imgui_demo/app_log.das`.

## Questions
- What's the dasImgui boost wrapper for ImGuiListClipper — how do I cull a virtual list to the visible region without inline Begin/Step/End?
