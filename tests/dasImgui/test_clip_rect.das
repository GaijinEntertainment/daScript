options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 with_clip_rect scope wrapper.

let FEATURE = "modules/dasImgui/examples/features/clip_rect.das"

[test]
def test_clip_rect_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/LEAD", 15.0f)
        t |> success(snap != null, "LEAD renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/GUARD"),
                     "dummy GUARD inside clip block registers")
        t |> success(widget_exists(snap, "MAIN_WIN/NOTE"),
                     "NOTE after clip block registers")
    }
}
