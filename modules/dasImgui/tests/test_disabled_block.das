options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 with_disabled scope wrapper.
//! Drives `examples/features/disabled_block.das`; with_disabled is stateless
//! so the gate carries no telemetry — the test verifies inner+outer widgets
//! both render regardless of disabled state.

let FEATURE = "modules/dasImgui/examples/features/disabled_block.das"

[test]
def test_disabled_block_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/BTN_INSIDE", 15.0f)
        t |> success(snap != null, "BTN_INSIDE renders")
        if (snap == null) return

        for (ident in ["GATE_DISABLED", "BTN_INSIDE", "CB_INSIDE",
                       "SLIDER_INSIDE", "BTN_OUTSIDE"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} registers regardless of disabled gate")
        }
    }
}
