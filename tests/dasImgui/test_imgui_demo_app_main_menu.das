options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for the imgui_demo `app_main_menu` scene. Boots the
//! single-scene harness driver and waits for MAIN_BAR_DEMO to register.

let IMGUI_DEMO_APP_MAIN_MENU_HARNESS = "modules/dasImgui/examples/imgui_demo/harness_app_main_menu.das"

[test]
def test_imgui_demo_app_main_menu_smoke(t : T?) {
    with_imgui_app(IMGUI_DEMO_APP_MAIN_MENU_HARNESS) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_BAR_DEMO", 15.0f)
        t |> success(snap0 != null, "MAIN_BAR_DEMO registers (demo renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "MAIN_BAR_DEMO")?["kind"] ?? "", "main_menu_bar",
                   "MAIN_BAR_DEMO.kind == 'main_menu_bar'")
    }
}
