options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `color_button_hover` — hover-target variant of
//! `color_button`. Verifies the feature demo renders, the three swatches
//! register under their idents, and `find_widget` reports the
//! `color_button_hover` kind.

let COLOR_BUTTON_HOVER_FEATURE = "modules/dasImgui/examples/features/color_button_hover.das"

[test]
def test_color_button_hover_smoke(t : T?) {
    with_imgui_app(COLOR_BUTTON_HOVER_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "CBH_WIN", 15.0f)
        t |> success(snap0 != null, "CBH_WIN registers (demo renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "CBH_WIN")?["kind"] ?? "", "window",
                   "CBH_WIN.kind == 'window'")
        var snap_red = wait_for_widget(d, "CBH_WIN/CBH_RED", 15.0f)
        t |> success(snap_red != null, "CBH_RED hover-swatch renders")
        if (snap_red != null) {
            t |> equal(find_widget(snap_red, "CBH_WIN/CBH_RED")?["kind"] ?? "",
                       "color_button_hover",
                       "CBH_RED.kind == 'color_button_hover'")
        }
        var snap_status = wait_for_widget(d, "CBH_WIN/CBH_STATUS", 15.0f)
        t |> success(snap_status != null, "CBH_STATUS narrative renders")
    }
}
