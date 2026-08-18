options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for the imgui_demo `user_guide` scene. Boots the
//! single-scene harness driver (the guide is hosted in MAIN_WIN since it
//! doesn't open its own window) and waits for the host window.

let IMGUI_DEMO_USER_GUIDE_HARNESS = "modules/dasImgui/examples/imgui_demo/harness_user_guide.das"

[test]
def test_imgui_demo_user_guide_smoke(t : T?) {
    with_imgui_app(IMGUI_DEMO_USER_GUIDE_HARNESS) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN", 15.0f)
        t |> success(snap0 != null, "MAIN_WIN registers (host window renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "MAIN_WIN")?["kind"] ?? "", "window",
                   "MAIN_WIN.kind == 'window'")
    }
}
