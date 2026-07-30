options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Smoke for app_small ShowExampleAppWindowTitles — verifies all 3 windows
//! register simultaneously (proves the `##` ID-separator semantics survive
//! the boost `window(...)` container and that 3 distinct daslang state
//! globals back the three ImGui-side windows).

let TITLES_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_small_window_titles.das"

[test]
def test_app_small_window_titles_smoke(t : T?) {
    with_imgui_app(TITLES_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "SMALL_TITLES_W1/SMALL_TITLES_W1_TEXT", 15.0f)
        t |> success(snap0 != null,
                     "SMALL_TITLES_W1/SMALL_TITLES_W1_TEXT registers after first frame")
        if (snap0 == null) return

        var w1 = find_widget(snap0, "SMALL_TITLES_W1")
        var w2 = find_widget(snap0, "SMALL_TITLES_W2")
        var w3 = find_widget(snap0, "SMALL_TITLES_W3")
        t |> equal(w1?["kind"] ?? "", "window", "window 1 (##1) registers")
        t |> equal(w2?["kind"] ?? "", "window", "window 2 (##2) registers")
        t |> equal(w3?["kind"] ?? "", "window", "window 3 (###AnimatedTitle) registers")
    }
}
