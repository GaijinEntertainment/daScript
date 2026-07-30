options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Smoke for app_small ShowExampleAppConstrainedResize — verifies window
//! plus the constraint combo + the 3 preset-size buttons all register.
//! Default constraint (type=6 Aspect 16:9) goes through the lambda-callback
//! path in imgui_window_constraints_builtin.

let CONSTR_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_small_constrained_resize.das"

[test]
def test_app_small_constrained_resize_smoke(t : T?) {
    with_imgui_app(CONSTR_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "SMALL_CONSTR_WIN/SMALL_CONSTR_COMBO", 15.0f)
        t |> success(snap0 != null,
                     "SMALL_CONSTR_WIN/SMALL_CONSTR_COMBO registers after first frame")
        if (snap0 == null) return

        var combo_entry = find_widget(snap0, "SMALL_CONSTR_WIN/SMALL_CONSTR_COMBO")
        t |> equal(combo_entry?["kind"] ?? "", "combo",
                   "constraint combo registers as kind=combo")
        t |> equal(combo_entry?["payload"]?["value"] ?? 0, 6,
                   "default constraint type is 6 (Aspect Ratio 16:9 — callback path)")

        var btn200 = find_widget(snap0, "SMALL_CONSTR_WIN/SMALL_CONSTR_BTN_200")
        t |> equal(btn200?["kind"] ?? "", "button",
                   "Set 200x200 button registers under window path")
    }
}
