options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Smoke for app_small ShowExampleAppLongText — verifies the window, combo,
//! and child(Log) container register.

let LONG_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_small_long_text.das"

[test]
def test_app_small_long_text_smoke(t : T?) {
    with_imgui_app(LONG_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "SMALL_LONG_WIN/SMALL_LONG_COMBO", 15.0f)
        t |> success(snap0 != null,
                     "SMALL_LONG_WIN/SMALL_LONG_COMBO registers after first frame")
        if (snap0 == null) return

        var combo_entry = find_widget(snap0, "SMALL_LONG_WIN/SMALL_LONG_COMBO")
        t |> equal(combo_entry?["kind"] ?? "", "combo",
                   "test-type combo registers as kind=combo")

        var child = find_widget(snap0, "SMALL_LONG_WIN/SMALL_LONG_CHILD")
        t |> equal(child?["kind"] ?? "", "child",
                   "scrolling child container under window registers")
    }
}
