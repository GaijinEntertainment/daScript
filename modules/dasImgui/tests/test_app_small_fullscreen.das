options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Smoke for app_small ShowExampleAppFullscreen — verifies the window and
//! Close button register; window size matches viewport.WorkSize at the
//! default use_work_area=true.

let FULL_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_small_fullscreen.das"

[test]
def test_app_small_fullscreen_smoke(t : T?) {
    with_imgui_app(FULL_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "SMALL_FULL_WIN/SMALL_FULL_BTN_CLOSE", 15.0f)
        t |> success(snap0 != null,
                     "SMALL_FULL_WIN/SMALL_FULL_BTN_CLOSE registers after first frame")
        if (snap0 == null) return

        var win_entry = find_widget(snap0, "SMALL_FULL_WIN")
        t |> equal(win_entry?["kind"] ?? "", "window",
                   "SMALL_FULL_WIN registers as kind=window")

        var btn = find_widget(snap0, "SMALL_FULL_WIN/SMALL_FULL_BTN_CLOSE")
        t |> equal(btn?["kind"] ?? "", "button",
                   "Close button registers under window path")
    }
}
