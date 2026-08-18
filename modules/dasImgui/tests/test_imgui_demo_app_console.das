options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for the imgui_demo `app_console` scene. Boots the
//! single-scene harness driver and waits for CONSOLE_WIN + its command input.

let IMGUI_DEMO_APP_CONSOLE_HARNESS = "modules/dasImgui/examples/imgui_demo/harness_app_console.das"

[test]
def test_imgui_demo_app_console_smoke(t : T?) {
    with_imgui_app(IMGUI_DEMO_APP_CONSOLE_HARNESS) $(d) {
        var snap0 = wait_for_widget(d, "CONSOLE_WIN", 15.0f)
        t |> success(snap0 != null, "CONSOLE_WIN registers (console renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "CONSOLE_WIN")?["kind"] ?? "", "window",
                   "CONSOLE_WIN.kind == 'window'")
        var snap1 = wait_for_widget(d, "CONSOLE_WIN/CONSOLE_INPUT", 15.0f)
        t |> success(snap1 != null, "CONSOLE_INPUT registers inside CONSOLE_WIN")
    }
}
