---
slug: dasimgui-container-annotation-and-hierarchical-path-keys
title: How does the dasImgui [container] annotation work — what's the difference from [widget] and how does the body get path-stack push/pop?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

`[container]` is a sibling function annotation to `[widget]` (Phase 0b.3, `modules/dasImgui/widgets/imgui_boost.das`). Same widget_ident injection, same dotted-flag parsing, same auto-emit gate. Two differences:

1. **Last param must be `blk : block`** — validated in `ContainerFunctionMacro.apply` (else compile-time error 'requires the last parameter to be `blk : block`').
2. **Body gets path-stack wraps**: `apply_widget_or_container(..., is_container=true)` prepends `container_path_push($i("widget_ident"))` to `body.list` and appends `container_path_pop()` to `body.finalList`. The pop is in finalList so it runs on normal scope exit (panic still skips it — same risk surface as widget_finalize).

The CallMacro is shared (`WidgetCallMacro`) — same `kind(IDENT, ...)` → `kind(IDENT_VAR, "IDENT", ...)` rewrite. The body wrapping is the only macro-side difference.

Runtime side (`imgui_boost_runtime.das`):
- `g_container_path : array<string>` — outermost-first chain of currently-open containers.
- `g_container_path_str : string` — cached "/"-joined form, kept in sync by push/pop so leaf path-key construction is one string interpolation, not an O(depth) per-leaf join.
- `widget_path_key(widget_ident)` — for leaves: `g_container_path_str + "/" + widget_ident` (or bare ident if path is empty).
- `container_path_key()` — for containers: just `g_container_path_str` (self already at the chain's tail, since the macro pushed in `body.list` before user body runs).
- Both leaf `widget_finalize` and the new `container_finalize` use these keys for `g_registry` AND `g_dispatchers`. Sibling `RPS` idents under different windows resolve as `MAIN/RPS` vs `OTHER/RPS` — collision-safe.

User-side calls follow the same named-tuple convention as `[widget]`:
```das
window(MAIN_WIN, (text="Main", closable=false, flags=ImGuiWindowFlags.None)) {
    button(SAVE, (text="Save"))    // registers under "MAIN_WIN/SAVE"
}
```

Container kind names that ship in 0b.3: window / child / group / menu_bar / menu / popup / popup_modal / tab_bar / tab_item / tooltip / item_tooltip / tree_node / collapsing_header / combo_select / list_box_select.

`begin_frame()` defensively clears `g_container_path` + `g_container_path_str` — a panic mid-frame the prior tick would otherwise leak the chain into the next frame.

## Questions
- How does the dasImgui [container] annotation work — what's the difference from [widget] and how does the body get path-stack push/pop?
