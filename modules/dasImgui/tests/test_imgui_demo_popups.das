options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for the imgui_demo `popups` scene. Boots the
//! single-scene harness driver (the section is hosted in MAIN_WIN since it
//! doesn't open its own window) and waits for the host window + the
//! POPUPS_SECTION collapsing_header inside.

let IMGUI_DEMO_POPUPS_HARNESS = "modules/dasImgui/examples/imgui_demo/harness_popups.das"

[test]
def test_imgui_demo_popups_smoke(t : T?) {
    with_imgui_app(IMGUI_DEMO_POPUPS_HARNESS) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN", 15.0f)
        t |> success(snap0 != null, "MAIN_WIN registers (host window renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "MAIN_WIN")?["kind"] ?? "", "window",
                   "MAIN_WIN.kind == 'window'")
        var snap1 = wait_for_widget(d, "MAIN_WIN/POPUPS_SECTION", 15.0f)
        t |> success(snap1 != null, "POPUPS_SECTION registers inside MAIN_WIN")
    }
}
