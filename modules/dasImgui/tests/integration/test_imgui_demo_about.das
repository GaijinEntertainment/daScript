options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for the imgui_demo `about` scene. Boots the
//! single-scene harness driver and waits for ABOUT_WINDOW to register.

let IMGUI_DEMO_ABOUT_HARNESS = "modules/dasImgui/examples/imgui_demo/harness_about.das"

[test]
def test_imgui_demo_about_smoke(t : T?) {
    with_imgui_app(IMGUI_DEMO_ABOUT_HARNESS) $(d) {
        var snap0 = wait_for_widget(d, "ABOUT_WINDOW", 15.0f)
        t |> success(snap0 != null, "ABOUT_WINDOW registers (demo renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "ABOUT_WINDOW")?["kind"] ?? "", "window",
                   "ABOUT_WINDOW.kind == 'window'")
    }
}
