options gen2
options _comment_hygiene = true
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
        var snap0 = wait_for_widget(d, "MAIN_WIN/SAVE_BTN", 15.0f)
        t |> success(snap0 != null, "SAVE_BTN registered after first frame")
        if (snap0 == null) return

        // ----- Snapshot shape contract -----
        var save0 = find_widget(snap0, "MAIN_WIN/SAVE_BTN")
        t |> equal(save0?["kind"] ?? "", "button", "SAVE_BTN.kind == 'button'")
        t |> success(save0?["payload"] != null, "SAVE_BTN.payload present")

        // ----- Click family — count++ on imgui_click -----
        let count_before = save0?["payload"]?["click_count"] ?? 0
        click(d, "MAIN_WIN/SAVE_BTN")
        t |> success(wait_for_int_value(d, "MAIN_WIN/SAVE_BTN", "click_count", count_before + 1, 300),
                     "click bumps SAVE_BTN.click_count")

        // ----- Toggle family — imgui_force_set flips the bool -----
        let wf_before = widget_payload_field(snap0, "MAIN_WIN/WIREFRAME", "value") ?? false
        force_set_value(d, "MAIN_WIN/WIREFRAME", JV(!wf_before))
        t |> success(wait_for_bool_value(d, "MAIN_WIN/WIREFRAME", "value", !wf_before, 300),
                     "imgui_force_set toggles WIREFRAME.value")

        // ----- Grouped int radio — imgui_force_set steers the selection -----
        force_set_value(d, "MAIN_WIN/DIFFICULTY", JV(2))
        t |> success(wait_for_int_value(d, "MAIN_WIN/DIFFICULTY", "value", 2, 300),
                     "imgui_force_set picks DIFFICULTY=2 (Hard)")

        // ----- Snapshot envelope — required common fields per widget -----
        // QUICK_SAVE.bbox is per-frame; gate on the menu_item being painted so the snapshot carries
        // its geometry, not a bare snapshot from a frame before the menu bar painted.
        var snap_final = wait_for_visible(d, "MAIN_WIN/QUICK_SAVE", 5.0f)
        t |> success(snap_final != null, "QUICK_SAVE painted")
        var menu = find_widget(snap_final, "MAIN_WIN/QUICK_SAVE")
        t |> equal(menu?["kind"] ?? "", "menu_item", "QUICK_SAVE.kind == 'menu_item'")
        t |> success(menu?["hex_id"] != null, "QUICK_SAVE.hex_id present")
        t |> success(menu?["bbox"] != null, "QUICK_SAVE.bbox present")
    }
}
