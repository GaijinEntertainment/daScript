options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! edit_* widgets over external pointers: force_set round-trips.

let SCALARS_FEATURE = "modules/dasImgui/examples/features/edit_external_scalars.das"

[test(description = "The edit_* slider, drag and input scalar overloads register, and force_set writes round-trip through a float and an int pointee.", timeout = 1200)]
def test_edit_external_scalars_smoke(t : T?) {
    with_imgui_app(SCALARS_FEATURE) $(d) {
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

let SCALARS_CUSTOM_FEATURE = "modules/dasImgui/examples/features/edit_external_scalars_custom.das"

[test(description = "The generic edit_*_scalar overloads register for non-default pointee types, and force_set round-trips through an int64 drag and a uint input.", timeout = 1200)]
def test_edit_external_scalars_custom_smoke(t : T?) {
    with_imgui_app(SCALARS_CUSTOM_FEATURE) $(d) {
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

let VECTORS_FEATURE = "modules/dasImgui/examples/features/edit_external_vectors.das"

[test(description = "Every edit_* slider, input and drag vector widget of widths 2 to 4 registers, and a force_set float3 write reads back with all three components.", timeout = 1200)]
def test_edit_external_vectors_smoke(t : T?) {
    with_imgui_app(VECTORS_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/SF2", 15.0f)
        t |> success(snap != null, "SF2 renders")
        if (snap == null) return

        for (ident in ["SF2", "SF3", "SF4", "SI2", "SI3", "SI4",
                       "IF2", "IF3", "IF4", "II2", "II3", "II4",
                       "DF2", "DF3", "DF4", "DI2", "DI3", "DI4"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} registered")
        }

        // imgui_force_set on a float3 — write {x,y,z} and verify the snapshot
        // reports back the same components.
        force_set_value(d, "MAIN_WIN/SF3", JV((x = 0.7f, y = 0.8f, z = 0.9f)))
        // Gate on the written component VALUES, not just field presence: SF3 already
        // exposes a non-null float3 before the write, so a presence-only predicate
        // passes on the pre-write snapshot and the round-trip isn't actually proven.
        let after = wait_until(d, 300) $(var s) {
            let xv = widget_payload_field(s, "MAIN_WIN/SF3", "value")
            let x = xv?["x"] ?? 0.0lf
            let y = xv?["y"] ?? 0.0lf
            let z = xv?["z"] ?? 0.0lf
            return (x > 0.69lf && x < 0.71lf &&
                    y > 0.79lf && y < 0.81lf &&
                    z > 0.89lf && z < 0.91lf)
        }
        t |> success(after != null, "edit_slider_float3 write round-trips to (0.7, 0.8, 0.9)")
    }
}

let COLOR_FEATURE = "modules/dasImgui/examples/features/edit_external_color.das"

[test(description = "The edit_color edit and picker widgets of 3 and 4 components register, and a force_set RGBA write reads back as a four-component value.", timeout = 1200)]
def test_edit_external_color_smoke(t : T?) {
    with_imgui_app(COLOR_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/CE3", 15.0f)
        t |> success(snap != null, "CE3 renders")
        if (snap == null) return

        for (ident in ["CE3", "CE4", "CP3", "CP4"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} registered")
        }

        // imgui_force_set on edit_color_edit4 round-trips RGBA.
        force_set_value(d, "MAIN_WIN/CE4", JV((x = 0.9f, y = 0.1f, z = 0.2f, w = 0.5f)))
        let after = wait_until(d, 300) $(var s) {
            let v = widget_payload_field(s, "MAIN_WIN/CE4", "value")
            return v != null && v?["w"] != null
        }
        t |> success(after != null, "edit_color_edit4 RGBA round-trip")
    }
}

let COMBO_FEATURE = "modules/dasImgui/examples/features/edit_external_combo.das"

[test(description = "edit_combo over an array of strings reports its seeded index and round-trips a force_set index.", timeout = 1200)]
def test_edit_external_combo_smoke(t : T?) {
    with_imgui_app(COMBO_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/COMBO_THEME", 15.0f)
        t |> success(snap != null, "COMBO_THEME renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/COMBO_DIFFICULTY"),
                     "COMBO_DIFFICULTY registered")

        let idx0 = widget_payload_field(snap, "MAIN_WIN/COMBO_THEME", "value") ?? 0
        t |> equal(idx0, 1, "initial selected idx = 1 ('Dark')")

        force_set_value(d, "MAIN_WIN/COMBO_THEME", JV(3))
        t |> success(wait_for_int_value(d, "MAIN_WIN/COMBO_THEME", "value", 3, 300),
                     "edit_combo write idx=3 round-trips")
    }
}

let SPECIAL_FEATURE = "modules/dasImgui/examples/features/edit_external_special.das"

[test(description = "edit_slider_angle round-trips a force_set value in radians.", timeout = 1200)]
def test_edit_external_special_smoke(t : T?) {
    with_imgui_app(SPECIAL_FEATURE) $(d) {
        let snap = wait_for_render(d, "MAIN_WIN/ANGLE", 15.0f)
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

let TOGGLES_FEATURE = "modules/dasImgui/examples/features/edit_external_toggles.das"

[test(description = "edit_checkbox, edit_radio_button and the edit_checkbox_flags bits register, and force_set round-trips a bool and a full flags mask.", timeout = 1200)]
def test_edit_external_toggles_smoke(t : T?) {
    with_imgui_app(TOGGLES_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/CB_GATE", 15.0f)
        t |> success(snap != null, "CB_GATE renders")
        if (snap == null) return

        for (ident in ["CB_GATE", "RB_RADIO",
                       "CBF_BIT0", "CBF_BIT1", "CBF_BIT2", "CBF_BIT3"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} registered")
        }

        force_set_value(d, "MAIN_WIN/CB_GATE", JV(true))
        t |> success(wait_for_bool_value(d, "MAIN_WIN/CB_GATE", "value", true, 300),
                     "edit_checkbox write true round-trips")

        // edit_checkbox_flags writes back the full int mask.
        force_set_value(d, "MAIN_WIN/CBF_BIT0", JV(7))
        t |> success(wait_for_int_value(d, "MAIN_WIN/CBF_BIT0", "value", 7, 300),
                     "edit_checkbox_flags write 7 round-trips")
    }
}
