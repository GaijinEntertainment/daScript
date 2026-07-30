options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 §3.2 layout primitives.
//! Drives `examples/features/layout_primitives.das`.

let FEATURE = "modules/dasImgui/examples/features/layout_primitives.das"

[test]
def test_layout_primitives_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/LAY_LEAD", 15.0f)
        t |> success(snap != null, "LAY_LEAD renders")
        if (snap == null) return

        let markers = ["LAY_SAME_LINE", "LAY_SAME_LINE_2", "LAY_SPACING_ROW",
                       "LAY_DUMMY", "LAY_NEW_LINE"]
        for (m in markers) {
            let path = "MAIN_WIN/{m}"
            t |> success(widget_exists(snap, path), "marker {path} registered")
        }
    }
}
