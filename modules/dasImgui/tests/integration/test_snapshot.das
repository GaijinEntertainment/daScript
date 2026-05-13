options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Isolate the first POST /command roundtrip.
//!
//! Lifecycle test (spawn → /status → /shutdown) passes; widget test hangs.
//! This test does ONE imgui_snapshot call between spawn and shutdown to
//! pin down whether the hang is in the HTTP layer, the live_command
//! dispatch, or the widget body. A pass here means /command works.

let TRIGGERS_FEATURE = "modules/dasImgui/examples/features/triggers.das"

[test]
def test_one_snapshot(t : T?) {
    with_imgui_app_opt(TRIGGERS_FEATURE, 9090, 30.0f, 30.0f) $(d) {
        var snap = wait_for_widget(d, "SAVE_BTN", 15.0f)
        t |> success(snap != null, "snapshot returned non-null payload with SAVE_BTN registered")
    }
}
