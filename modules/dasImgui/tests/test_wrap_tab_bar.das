options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for the wrapping tab bar. Opens the wrap_tab_bar tutorial,
//! asserts tab 0 is active by default (its body renders, an inactive body does
//! not), then force-selects tab 2 by index and asserts the active body follows.
//! Proves the widget is both snapshot-visible (the `value` payload) and
//! programmatically drivable — the upgrade over ImGui's native TabBar.

let WRAP_FEATURE = "modules/dasImgui/examples/tutorial/wrap_tab_bar.das"

[test]
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

[test]
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
        var snap_grid = wait_until_sec(d, 15.0f) $(var s) {
            return (widget_payload_field(s, "WTB_WIN/WTB_BAR", "value") ?? -1) == 6
        }
        t |> success(snap_grid != null, "clicking grid (second row) by name selects tab 6")
    }
}
