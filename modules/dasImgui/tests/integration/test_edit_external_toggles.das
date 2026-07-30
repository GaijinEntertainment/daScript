options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 edit_checkbox / edit_radio_button /
//! edit_checkbox_flags.

let FEATURE = "modules/dasImgui/examples/features/edit_external_toggles.das"

[test]
def test_edit_external_toggles_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
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
