options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 with_font scope wrapper.

let FEATURE = "modules/dasImgui/examples/features/font_stack.das"

[test]
def test_font_stack_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/INSIDE", 15.0f)
        t |> success(snap != null, "with_font block body renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/INSIDE"), "inside text")
        t |> success(widget_exists(snap, "MAIN_WIN/BTN_INSIDE"), "inside button")
    }
}
