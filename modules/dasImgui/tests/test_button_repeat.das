options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 with_button_repeat scope wrapper.

let FEATURE = "modules/dasImgui/examples/features/button_repeat.das"

[test]
def test_button_repeat_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/BTN_INSIDE", 15.0f)
        t |> success(snap != null, "BTN_INSIDE registers")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/BTN_OUTSIDE"),
                     "fire-once button also registers")
        // The repeat-vs-fire-once behavioral difference requires hold-time
        // input; record_button_repeat.das exercises that. Smoke gate is
        // just existence + initial click_count == 0.
        let outside_clicks = widget_payload_field(snap, "MAIN_WIN/BTN_OUTSIDE", "click_count") ?? 0
        t |> equal(outside_clicks, 0, "outside button has not been clicked yet")
    }
}
