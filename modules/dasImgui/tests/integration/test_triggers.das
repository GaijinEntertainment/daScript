options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 0b.1 triggers.
//!
//! Spawns daslang-live against `examples/features/triggers.das`, waits for
//! the GLFW window to render its first frame, then drives every trigger
//! widget through the `imgui_*` live_command surface. Uses playwright-style
//! `wait_for_*_value` polls instead of raw sleeps — assertions block on
//! observable state convergence with a per-call timeout, never on a
//! magic-number "give the host a frame" delay.

let TRIGGERS_FEATURE = "modules/dasImgui/examples/features/triggers.das"

[test]
def test_triggers_smoke(t : T?) {
    with_imgui_app(TRIGGERS_FEATURE) $(d) {
        // Gate on a known widget showing up — proves the GUI rendered a
        // frame, no need for sleep().
        var snap0 = wait_for_widget(d, "SAVE_BTN", 15.0f)
        t |> success(snap0 != null, "SAVE_BTN registered after first frame")
        if (snap0 == null) return

        // ----- Snapshot shape contract -----
        var save0 = find_widget(snap0, "SAVE_BTN")
        t |> equal(save0?["kind"] ?? "", "button", "SAVE_BTN.kind == 'button'")
        t |> success(save0?["payload"] != null, "SAVE_BTN.payload present")

        // ----- Click family — count++ on imgui_click -----
        let count_before = save0?["payload"]?["click_count"] ?? 0
        click(d, "SAVE_BTN")
        t |> success(wait_for_int_value(d, "SAVE_BTN", "click_count", count_before + 1, 300),
                     "click bumps SAVE_BTN.click_count")

        // ----- Toggle family — imgui_set flips the bool -----
        let wf_before = widget_payload_field(snap0, "WIREFRAME", "value") ?? false
        set_value(d, "WIREFRAME", JV(!wf_before))
        t |> success(wait_for_bool_value(d, "WIREFRAME", "value", !wf_before, 300),
                     "imgui_set toggles WIREFRAME.value")

        // ----- Grouped int radio — imgui_set steers the selection -----
        set_value(d, "DIFFICULTY", JV(2))
        t |> success(wait_for_int_value(d, "DIFFICULTY", "value", 2, 300),
                     "imgui_set picks DIFFICULTY=2 (Hard)")

        // ----- Snapshot envelope — required common fields per widget -----
        var snap_final = snapshot(d)
        var menu = find_widget(snap_final, "QUICK_SAVE")
        t |> equal(menu?["kind"] ?? "", "menu_item", "QUICK_SAVE.kind == 'menu_item'")
        t |> success(menu?["hex_id"] != null, "QUICK_SAVE.hex_id present")
        t |> success(menu?["bbox"] != null, "QUICK_SAVE.bbox present")
    }
}
