options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `set_window_size` — runtime window-size mutation.
//! The feature renders three buttons; clicking BTN_LARGE calls
//! `set_window_size(float2(640, 440), Always)`. set_window_size is an
//! imperative pass-through (no [widget] state), so we verify the button
//! click_count rises (proves the call site fires) AND poll the
//! SIZE_READOUT text payload for the post-resize string.

let FEATURE = "modules/dasImgui/examples/features/set_window_size_runtime.das"

[test]
def test_set_window_size_runtime_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/BTN_LARGE", 15.0f)
        t |> success(snap != null, "BTN_LARGE registers")
        if (snap == null) return

        for (ident in ["BTN_SMALL", "BTN_MEDIUM", "BTN_LARGE", "SIZE_READOUT", "LEAD"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "MAIN_WIN/{ident} registers")
        }

        // Click LARGE -> set_window_size(640, 440) -> SIZE_READOUT updates.
        click(d, "MAIN_WIN/BTN_LARGE")
        t |> success(wait_for_int_value(d, "MAIN_WIN/BTN_LARGE", "click_count", 1, 300),
                     "BTN_LARGE.click_count == 1 after click")
        t |> success(wait_for_string_value(d, "MAIN_WIN/SIZE_READOUT", "value",
                                           "current = (640, 440)", 300),
                     "SIZE_READOUT reflects 640x440 after BTN_LARGE click")

        // Click MEDIUM -> 480x320 (round-trip back from LARGE).
        click(d, "MAIN_WIN/BTN_MEDIUM")
        t |> success(wait_for_string_value(d, "MAIN_WIN/SIZE_READOUT", "value",
                                           "current = (480, 320)", 300),
                     "SIZE_READOUT reflects 480x320 after BTN_MEDIUM click")
    }
}
