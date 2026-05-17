---
slug: why-are-imgui-main-menu-bar-child-menu-bboxes-0-0-0-0-in-imgui-snapshot-how-do-i-click-a-file-edit-menu-header-from-a-record-das
title: why are imgui main_menu_bar child menu() bboxes (0,0,0,0) in imgui_snapshot — how do I click a File/Edit menu header from a record_*.das driver?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

# `menu()` headers register bbox correctly; only `main_menu_bar()` root is degenerate

**Updated 2026-05-17:** `widgets/imgui_containers_builtin.das` now snapshots
`GetItemRectMin/Max` right after `BeginMenu` / `BeginTabItem` returns, while
the parent-strip header is still the "last item." So:

- `menu()` children of `main_menu_bar` / `menu_bar` — bbox = the clickable
  header rect in the parent strip. `widget_center(snap, "MAIN_BAR/APP_MENU_FILE")`
  returns the visual center of the "File" label.
- `tab_item()` headers — same fix; bbox = the clickable tab strip rect.
- `main_menu_bar()` ROOT — STILL `(0,0,0,0)`. The bar chrome has no
  meaningful "header" — there's no parent strip to click into. If you need
  to click outside any open child menu to dismiss it, hardcode pixel coords.

Confirmed via snapshot diff against the fix commit (style_editor's
`STYLE_TAB_COLORS` registered `bbox = (119,168,171,188)` post-fix vs
`(0,0,0,0)` pre-fix).

## How to click a menu header now

    var snap = wait_for_render(app, "MAIN_BAR_DEMO/APP_MENU_FILE", 10.0f)
    let p_file = widget_center(snap, "MAIN_BAR_DEMO/APP_MENU_FILE")
    var events : array<JsonValue?>
    events |> click_at(0, p_file, 250)
    post_command(app, "imgui_mouse_play", JV((events = events)))

For submenu items: items DO register bboxes once the parent menu is open.
Click the parent first (via its header bbox), `wait_for_render` for the
child path, then `widget_center` the child.

## Historical artifact

`tests/integration/record_imgui_demo_app_main_menu.das` predates this fix
and bakes empirical pixel coords:

    let P_FILE_HEADER = (24.0f,  10.0f)
    let P_EDIT_HEADER = (56.0f,  10.0f)
    let P_FILE_COLORS = (50.0f, 173.0f)

Already-committed APNG works; not worth re-recording. **New drivers
should use `widget_center`** for menu children and tab items.

## Root cause that got fixed

`stateless_finalize` and `open_close_finalize` previously ignored the
item-rect entirely (default-constructed entry → zero bbox). The fix
takes an optional `bbox : float4` parameter and stamps it before
`container_finalize` writes the registry entry. `menu()` and
`tab_item()` capture via a new `current_item_bbox()` helper between
their `BeginXxx()` and `invoke(blk)` calls.

`widgets/imgui_containers_builtin.das:43-89` for the helper signatures;
`menu()` at line ~205 and `tab_item()` at line ~377 for the call sites.

## Related

- `examples/imgui_demo/app_main_menu.das` — main menu bar port.
- `examples/imgui_demo/style_editor.das` — exercises `tab_item` bbox.
- `tests/integration/record_imgui_demo_style_editor.das` — first
  recording driver to rely on the bbox fix (clicks 4 tabs via
  `widget_center`).

## Questions
- why are imgui main_menu_bar child menu() bboxes (0,0,0,0) in imgui_snapshot — how do I click a File/Edit menu header from a record_*.das driver?
- can I use widget_center on a menu() child or tab_item() in dasImgui?
- after the 2026-05-17 fix, which containers still have bbox=(0,0,0,0)?
