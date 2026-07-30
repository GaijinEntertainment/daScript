options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for the narrate-placement harness — 4×5 = 20 indexed
//! buttons in a 1280×720 viewport. narrate()/highlight() don't register in
//! the snapshot (they push into module-scope arrays painted by the visual
//! aids overlay), so the test verifies the checkbox + all 20 indexed
//! buttons register, and that click-to-select registers as expected.

let FEATURE = "modules/dasImgui/examples/features/narrate_placement.das"

[test]
def test_narrate_placement_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/SHOW_NARRATION", 15.0f)
        t |> success(snap != null, "SHOW_NARRATION registers")
        if (snap == null) return

        let cb = find_widget(snap, "MAIN_WIN/SHOW_NARRATION")
        t |> equal(cb?["kind"] ?? "", "checkbox",
                   "SHOW_NARRATION.kind == 'checkbox'")

        // All 20 grid buttons register with literal index keys.
        for (idx in range(20)) {
            t |> success(widget_exists(snap, "MAIN_WIN/GRID_BTN[{idx}]"),
                         "GRID_BTN[{idx}] registers")
        }

        // Click GRID_BTN[0] — click_count increments; the click-through wire
        // is independent of narrate() banner painting (which isn't snapshot
        // visible).
        click(d, "MAIN_WIN/GRID_BTN[0]")
        t |> success(wait_for_int_value(d, "MAIN_WIN/GRID_BTN[0]", "click_count", 1, 300),
                     "GRID_BTN[0].click_count == 1 after click")

        // Click another button — its count increments, the first stays at 1
        // (retarget routes, doesn't stack on the previous).
        click(d, "MAIN_WIN/GRID_BTN[7]")
        t |> success(wait_for_int_value(d, "MAIN_WIN/GRID_BTN[7]", "click_count", 1, 300),
                     "GRID_BTN[7].click_count == 1 after retarget")
        var snap_after = snapshot(d)
        t |> equal(widget_payload_field(snap_after, "MAIN_WIN/GRID_BTN[0]", "click_count") ?? -1,
                   1,
                   "GRID_BTN[0].click_count still 1 (no extra fire on retarget)")
    }
}
