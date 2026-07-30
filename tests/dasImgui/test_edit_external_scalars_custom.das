options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 edit_*_scalar generic-T rails.
//! Drives `examples/features/edit_external_scalars_custom.das` — verifies
//! each non-default-typed pointee registers and round-trips imgui_force_set.

let FEATURE = "modules/dasImgui/examples/features/edit_external_scalars_custom.das"

[test]
def test_edit_external_scalars_custom_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/DRAG_I8", 15.0f)
        t |> success(snap != null, "DRAG_I8 renders")
        if (snap == null) return

        for (ident in ["DRAG_I8", "DRAG_U16", "DRAG_I64",
                       "SLIDER_D", "SLIDER_U8",
                       "INPUT_U", "INPUT_D2"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} registered")
        }

        // imgui_force_set on edit_drag_scalar with int64 — verify round-trip.
        force_set_value(d, "MAIN_WIN/DRAG_I64", JV(500000))
        t |> success(wait_for_int_value(d, "MAIN_WIN/DRAG_I64", "value", 500000, 300),
                     "edit_drag_scalar<int64> write 500000 round-trips")

        // imgui_force_set on edit_input_scalar with uint — verify round-trip.
        force_set_value(d, "MAIN_WIN/INPUT_U", JV(42))
        t |> success(wait_for_int_value(d, "MAIN_WIN/INPUT_U", "value", 42, 300),
                     "edit_input_scalar<uint> write 42 round-trips")
    }
}
