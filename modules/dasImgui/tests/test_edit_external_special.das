options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 edit_slider_angle.

let FEATURE = "modules/dasImgui/examples/features/edit_external_special.das"

[test]
def test_edit_external_special_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/ANGLE", 15.0f)
        t |> success(snap != null, "ANGLE renders")
        if (snap == null) return

        // imgui_force_set radians; verify round-trip.
        force_set_value(d, "MAIN_WIN/ANGLE", JV(1.5707f))
        let after = wait_until(d, 300) $(var s) {
            let v = widget_payload_field(s, "MAIN_WIN/ANGLE", "value") ?? 0.0
            return v > 1.5 && v < 1.6
        }
        t |> success(after != null, "edit_slider_angle radians round-trip")
    }
}
