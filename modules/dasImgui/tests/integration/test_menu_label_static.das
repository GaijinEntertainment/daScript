options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 menu_label widget.
//! Drives `examples/features/menu_label_static.das`; verifies the anchor
//! button registers (popup contents only emit while open, so the test
//! stays loose like test_menu_main).

let FEATURE = "modules/dasImgui/examples/features/menu_label_static.das"

[test]
def test_menu_label_static_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/ANCHOR", 15.0f)
        t |> success(snap != null, "ANCHOR button renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/STATUS"),
                     "STATUS text renders below popup anchor")
    }
}
