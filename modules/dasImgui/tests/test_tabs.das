options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Tab bars: tab buttons, selection from automation, wrapping.

let TAB_BUTTON_FEATURE = "modules/dasImgui/examples/features/tab_item_button.das"

[test(description = "tab_item_button widgets register their kind and count clicks, and the ADD_TAB click grows the indexed tab list while HELP adds no tab.", timeout = 1200)]
def test_tab_item_button_smoke(t : T?) {
    with_imgui_app(TAB_BUTTON_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/BAR/ADD_TAB", 15.0f)
        t |> success(snap0 != null, "ADD_TAB renders")
        if (snap0 == null) return

        // Gate 1: kind sanity.
        let help_entry = find_widget(snap0, "MAIN_WIN/BAR/HELP")
        t |> equal(help_entry?["kind"] ?? "", "tab_item_button",
                   "HELP.kind == 'tab_item_button'")
        let add_entry = find_widget(snap0, "MAIN_WIN/BAR/ADD_TAB")
        t |> equal(add_entry?["kind"] ?? "", "tab_item_button",
                   "ADD_TAB.kind == 'tab_item_button'")

        // Gate 2: initial seed — 3 tabs (IDs 0/1/2). Indexed-form widget_ident
        // is `IDENT[k]` (see imgui_boost.das `emit_indexed_call`), so the path
        // segment is `DYN_TAB[N]`, not the rendered "{tab_id:04}" label.
        t |> success(widget_exists(snap0, "MAIN_WIN/BAR/DYN_TAB[0]"),
                     "seed tab DYN_TAB[0] renders")
        t |> success(widget_exists(snap0, "MAIN_WIN/BAR/DYN_TAB[2]"),
                     "seed tab DYN_TAB[2] renders")

        // Gate 3: click ADD_TAB → click_count grows + new tab 0003 appears.
        let click_resp = click(d, "MAIN_WIN/BAR/ADD_TAB")
        t |> equal(click_resp?["ok"] ?? false, true,
                   "imgui_click on ADD_TAB returns ok")

        let grew = wait_until(d, 300) $(var s) {
            return widget_exists(s, "MAIN_WIN/BAR/DYN_TAB[3]")
        }
        t |> success(grew != null,
                     "clicking ADD_TAB grows the tab list (DYN_TAB[3] renders)")
        var snap_after = snapshot(d)
        let add_after = find_widget(snap_after, "MAIN_WIN/BAR/ADD_TAB")
        t |> success((add_after?["payload"]?["click_count"] ?? 0) >= 1,
                     "ADD_TAB.click_count incremented")

        // Gate 4: click HELP → click_count grows; no new tab.
        let help_resp = click(d, "MAIN_WIN/BAR/HELP")
        t |> equal(help_resp?["ok"] ?? false, true,
                   "imgui_click on HELP returns ok")
        let help_clicked = wait_until(d, 300) $(var s) {
            let h = find_widget(s, "MAIN_WIN/BAR/HELP")
            return (h?["payload"]?["click_count"] ?? 0) >= 1
        }
        t |> success(help_clicked != null,
                     "HELP.click_count incremented after imgui_click")
    }
}

let TAB_SELECT_FEATURE = "modules/dasImgui/examples/features/containers_layout.das"

def private is_selected(var snap : JsonValue?; tab : string) : bool =>
    widget_payload_field(snap, tab, "selected") ?? false

[test(description = "imgui_select on a tab flips its snapshot-visible selected flag on, the previous tab's off, and stops the previous tab's body painting.", timeout = 1200)]
def test_tab_select_smoke(t : T?) {
    with_imgui_app(TAB_SELECT_FEATURE) $(d) {
        var snap = wait_for_render(d, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE/GRAVITY", 15.0f)
        t |> success(snap != null, "settings-tab child renders initially (SETTINGS active)")
        if (snap == null) {
            return
        }

        // `selected` is exposed in the snapshot payload (TabItemState reflected).
        t |> success(is_selected(snap, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB"),
                     "SETTINGS_TAB.selected == true initially")
        t |> success(!is_selected(snap, "LAYOUT_WIN/MAIN_TABS/AUDIO_TAB"),
                     "AUDIO_TAB.selected == false initially")

        // Activate the Audio tab from automation.
        select_widget(d, "LAYOUT_WIN/MAIN_TABS/AUDIO_TAB")
        let became = wait_until_sec(d, 5.0f) $(var s) {
            return is_selected(s, "LAYOUT_WIN/MAIN_TABS/AUDIO_TAB")
        }
        t |> success(became != null, "AUDIO_TAB.selected == true after imgui_select")

        // The previously-active tab deselects and its child stops rendering.
        var s2 = snapshot(d)
        t |> success(!is_selected(s2, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB"),
                     "SETTINGS_TAB.selected == false after select")
        t |> success(!widget_rendered(s2, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE/GRAVITY"),
                     "settings-tab child stops rendering once Audio is active")
    }
}

let WRAP_FEATURE = "modules/dasImgui/examples/tutorial/wrap_tab_bar.das"

[test(description = "The wrapping tab bar starts on tab 0 with only its body painted, and a force_set index switches both the value and the painted body.", timeout = 1200)]
def test_wrap_tab_bar(t : T?) {
    with_imgui_app(WRAP_FEATURE) $(d) {
        // 30s (not the usual 15) — the macOS CI lane runs serially on a starved
        // runner; the host can take >15s to spawn + render its first widget frame.
        var snap = wait_for_widget(d, "WTB_WIN/WTB_BAR", 30.0f)
        t |> success(snap != null, "wrap_tab_bar renders")
        if (snap == null) {
            return
        }

        // default: first tab active, its body shown, an inactive body hidden
        t |> success((widget_payload_field(snap, "WTB_WIN/WTB_BAR", "value") ?? -1) == 0,
                     "tab 0 active by default")
        t |> success(widget_rendered(snap, "WTB_WIN/WTB_BAR/WTB_B0"), "tab 0 body renders")
        t |> success(!widget_rendered(snap, "WTB_WIN/WTB_BAR/WTB_B2"), "inactive tab 2 body hidden")

        // drive: select tab 2 (lava) by index — exercises has_pending/pending_value
        force_set_value(d, "WTB_WIN/WTB_BAR", JV(2))
        var snap2 = wait_until_sec(d, 15.0f) $(var s) {
            return (widget_payload_field(s, "WTB_WIN/WTB_BAR", "value") ?? -1) == 2
        }
        t |> success(snap2 != null, "force_set selects tab 2")
        if (snap2 == null) {
            return
        }
        t |> success(widget_rendered(snap2, "WTB_WIN/WTB_BAR/WTB_B2"), "tab 2 body renders after switch")
        t |> success(!widget_rendered(snap2, "WTB_WIN/WTB_BAR/WTB_B0"), "tab 0 body hidden after switch")
    }
}

[test(description = "Each wrapping tab header registers a clickable alias by label, and a synth click on it, on the first or the second wrapped row, selects that tab.", timeout = 1200)]
def test_wrap_tab_bar_click_by_name(t : T?) {
    // The header strip registers each tab as a clickable alias WTB_BAR/<label>
    // (register_clickable_bbox), so a REAL synth click selects a tab by name -
    // not just force_set by index. This is what the tutorial recording drives.
    with_imgui_app(WRAP_FEATURE) $(d) {
        var snap = wait_for_widget(d, "WTB_WIN/WTB_BAR/lava", 30.0f)
        t |> success(snap != null, "lava header registers as a clickable alias by name")
        if (snap == null) {
            return
        }
        t |> equal(find_widget(snap, "WTB_WIN/WTB_BAR/lava")?["kind"] ?? "", "wrap_tab_item",
                   "header alias kind is wrap_tab_item")

        // Click "lava" (index 2) by name -> active index follows.
        click(d, "WTB_WIN/WTB_BAR/lava")
        var snap_lava = wait_until_sec(d, 15.0f) $(var s) {
            return (widget_payload_field(s, "WTB_WIN/WTB_BAR", "value") ?? -1) == 2
        }
        t |> success(snap_lava != null, "clicking lava by name selects tab 2")
        if (snap_lava != null) {
            t |> success(widget_rendered(snap_lava, "WTB_WIN/WTB_BAR/WTB_B2"), "lava body renders after click")
        }

        // Click a header on the second wrapped row ("grid", index 6) -> follows too.
        click(d, "WTB_WIN/WTB_BAR/grid")
        let snap_grid = wait_until_sec(d, 15.0f) $(var s) {
            return (widget_payload_field(s, "WTB_WIN/WTB_BAR", "value") ?? -1) == 6
        }
        t |> success(snap_grid != null, "clicking grid (second row) by name selects tab 6")
    }
}
