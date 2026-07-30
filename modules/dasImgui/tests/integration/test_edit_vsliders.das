options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 edit_vslider_{float,int} rails.
//! Drives `examples/features/edit_vsliders.das` — verifies all 5 vertical
//! sliders register and a representative write round-trips.

let FEATURE = "modules/dasImgui/examples/features/edit_vsliders.das"

[test]
def test_edit_vsliders_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/VSLIDE_F1", 15.0f)
        t |> success(snap != null, "VSLIDE_F1 renders")
        if (snap == null) return

        for (ident in ["VSLIDE_F1", "VSLIDE_F2", "VSLIDE_F3",
                       "VSLIDE_I1", "VSLIDE_I2"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} registered")
        }

        // imgui_force_set on edit_vslider_float — float pointee round-trips.
        force_set_value(d, "MAIN_WIN/VSLIDE_F1", JV(0.875f))
        let after_f = wait_until(d, 300) $(var s) {
            let v = double(widget_payload_field(s, "MAIN_WIN/VSLIDE_F1", "value") ?? 0.0)
            return v > 0.87lf && v < 0.88lf
        }
        t |> success(after_f != null, "edit_vslider_float write 0.875 round-trips")

        // imgui_force_set on edit_vslider_int — int pointee round-trips.
        force_set_value(d, "MAIN_WIN/VSLIDE_I2", JV(88))
        t |> success(wait_for_int_value(d, "MAIN_WIN/VSLIDE_I2", "value", 88, 300),
                     "edit_vslider_int write 88 round-trips")
    }
}
