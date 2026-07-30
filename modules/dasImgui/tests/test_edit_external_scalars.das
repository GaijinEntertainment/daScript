options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 edit_* scalar rail.
//! Drives `examples/features/edit_external_scalars.das` and round-trips
//! imgui_force_set on a sample of overloads.

let FEATURE = "modules/dasImgui/examples/features/edit_external_scalars.das"

[test]
def test_edit_external_scalars_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/SLIDER_F", 15.0f)
        t |> success(snap != null, "SLIDER_F renders")
        if (snap == null) return

        for (ident in ["SLIDER_F", "SLIDER_I", "DRAG_F", "DRAG_I",
                       "INPUT_F", "INPUT_I", "INPUT_D"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} registered")
        }

        // imgui_force_set on edit_slider_float — float pointee round-trips.
        force_set_value(d, "MAIN_WIN/SLIDER_F", JV(0.875f))
        let after_f = wait_until(d, 300) $(var s) {
            let v = double(widget_payload_field(s, "MAIN_WIN/SLIDER_F", "value") ?? 0.0)
            return v > 0.87lf && v < 0.88lf
        }
        t |> success(after_f != null, "edit_slider_float write 0.875 round-trips")

        // imgui_force_set on edit_input_int — int pointee round-trips.
        force_set_value(d, "MAIN_WIN/INPUT_I", JV(99))
        t |> success(wait_for_int_value(d, "MAIN_WIN/INPUT_I", "value", 99, 300),
                     "edit_input_int write 99 round-trips")
    }
}
