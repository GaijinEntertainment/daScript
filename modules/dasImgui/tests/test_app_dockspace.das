options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Smoke for imgui_demo ShowExampleAppDockSpace — host window opens with
//! its MenuBar + Options menu, the embedded dockspace_in_window registers
//! with a non-zero dock_id, and the two sample dock_window panels both
//! render under it.

let DS_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_dockspace.das"

[test]
def test_app_dockspace_smoke(t : T?) {
    with_imgui_app(DS_FEATURE) $(d) {
        var snap = wait_for_widget(d, "DS_WIN", 15.0f)
        t |> success(snap != null, "DS_WIN host window renders")
        if (snap == null) return

        let ds = find_widget(snap, "DS_WIN/DS")
        t |> equal(ds?["kind"] ?? "", "dockspace_in_window",
                   "DS_WIN/DS registers as kind=dockspace_in_window")
        let dock_id = ds?["payload"]?["dock_id"] ?? 0u
        t |> success(dock_id != 0u,
                     "dock_id captured (got 0x{dock_id})")

        t |> success(widget_exists(snap, "DS_WIN/DS/DS_PANEL_A"),
                     "DS_PANEL_A dock_window renders")
        t |> success(widget_exists(snap, "DS_WIN/DS/DS_PANEL_B"),
                     "DS_PANEL_B dock_window renders")
        t |> success(widget_exists(snap, "DS_WIN/DS_MENU/DS_OPTIONS_MENU"),
                     "DS_MENU menu_bar + Options menu register")
    }
}
