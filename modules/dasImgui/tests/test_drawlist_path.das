options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for the drawlist path family (path_clear /
//! path_line_to / path_arc_to / path_bezier_quadratic_curve_to /
//! path_fill_convex / path_fill_concave / path_stroke). The path
//! helpers are control-flow primitives — they shape what later
//! ``[drawlist_prim]`` calls render but do not themselves register
//! snapshot rows. The smoke value here is end-to-end: the body fires
//! every path call without panicking, the host window + LEAD + slot
//! all reach the registry, and the three shapes draw without ImGui
//! asserting on path stack misuse.

let PATH_FEATURE = "modules/dasImgui/examples/features/drawlist_path.das"

[test]
def test_drawlist_path_renders(t : T?) {
    with_imgui_app(PATH_FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/LEAD", 15.0f)
        t |> success(snap != null,
                     "MAIN_WIN/LEAD renders (path body reached the trailing text)")
        if (snap == null) return
        t |> success(widget_exists(snap, "MAIN_WIN/PATH_SLOT"),
                     "MAIN_WIN/PATH_SLOT registers (dummy after the path body completed)")
    }
}
