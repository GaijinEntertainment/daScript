options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `selectable_hover` — hover-target variant of
//! `selectable`. Verifies the feature demo renders, all five rows of
//! the indexed table register under SH_ROW[i], and `find_widget` reports
//! the `selectable_hover` kind.

let SELECTABLE_HOVER_FEATURE = "modules/dasImgui/examples/features/selectable_hover.das"

[test]
def test_selectable_hover_smoke(t : T?) {
    with_imgui_app(SELECTABLE_HOVER_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "SH_WIN", 15.0f)
        t |> success(snap0 != null, "SH_WIN registers (demo renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "SH_WIN")?["kind"] ?? "", "window",
                   "SH_WIN.kind == 'window'")
        var snap_row = wait_for_widget(d, "SH_WIN/SH_ROW[0]", 15.0f)
        t |> success(snap_row != null, "SH_ROW[0] hover-row renders")
        if (snap_row != null) {
            t |> equal(find_widget(snap_row, "SH_WIN/SH_ROW[0]")?["kind"] ?? "",
                       "selectable_hover",
                       "SH_ROW[0].kind == 'selectable_hover'")
        }
    }
}
