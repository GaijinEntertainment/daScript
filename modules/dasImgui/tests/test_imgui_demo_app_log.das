options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for the imgui_demo `app_log` scene. Boots the
//! single-scene harness driver and waits for LOG_WIN + its debug button row.

let IMGUI_DEMO_APP_LOG_HARNESS = "modules/dasImgui/examples/imgui_demo/harness_app_log.das"

[test]
def test_imgui_demo_app_log_smoke(t : T?) {
    with_imgui_app(IMGUI_DEMO_APP_LOG_HARNESS) $(d) {
        var snap0 = wait_for_widget(d, "LOG_WIN", 15.0f)
        t |> success(snap0 != null, "LOG_WIN registers (log renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "LOG_WIN")?["kind"] ?? "", "window",
                   "LOG_WIN.kind == 'window'")
        var snap1 = wait_for_widget(d, "LOG_WIN/LOG_BTN_ADD5", 15.0f)
        t |> success(snap1 != null, "LOG_BTN_ADD5 registers inside LOG_WIN")
    }
}
