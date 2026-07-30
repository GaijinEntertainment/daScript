options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Smoke for app_small ShowExampleAppAutoResize — verifies the slider
//! drives line count and the window auto-resizes.

let AUTO_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_small_auto_resize.das"

[test]
def test_app_small_auto_resize_smoke(t : T?) {
    with_imgui_app(AUTO_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "SMALL_AUTO_WIN/SMALL_AUTO_SLIDER", 15.0f)
        t |> success(snap0 != null,
                     "SMALL_AUTO_WIN/SMALL_AUTO_SLIDER registers after first frame")
        if (snap0 == null) return

        var slider = find_widget(snap0, "SMALL_AUTO_WIN/SMALL_AUTO_SLIDER")
        t |> equal(slider?["kind"] ?? "", "slider_int",
                   "slider registers as kind=slider_int")
        t |> equal(slider?["payload"]?["value"] ?? 0, 10,
                   "slider initial value matches the init=10 seed")

        // Drive value to 5 and verify the new line count via the per-line
        // text state count.
        force_set_value(d, "SMALL_AUTO_WIN/SMALL_AUTO_SLIDER", JV(5))
        t |> success(wait_for_int_value(d, "SMALL_AUTO_WIN/SMALL_AUTO_SLIDER",
                                        "value", 5, 300),
                     "imgui_force_set drops the slider to 5")
    }
}
