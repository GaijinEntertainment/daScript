options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Spec §7.1 happy-path test — first test of the new playwright API
//! surface end-to-end against a user-facing demo app. Exercises every
//! new Phase 3 helper:
//!   - with_imgui_app  (subprocess + transport lifecycle)
//!   - expect_render   (assert SAVE_BTN actually painted this frame)
//!   - click           (imgui_click via transport)
//!   - expect_value    (panic-on-timeout assertion form)
//!   - type_text       (auto-focuses NAME_INPUT before sending chars)
//!
//! If any of those wires is broken, this test panics with a focused
//! failure message and dastest surfaces it. The bare `t |> success`
//! at the end is the dastest hook to record "we reached the end."

let SAVE_DEMO_FEATURE = "modules/dasImgui/examples/save_demo/main.das"

[test]
def test_save_flow(t : T?) {
    with_imgui_app(SAVE_DEMO_FEATURE) $(app) {
        // Subprocess up + first frame painted. expect_render's failure
        // message would list every registered widget — useful sanity.
        expect_render(app, "SAVE_BTN", 15.0f)

        // Click → STATUS_TEXT flips to "saved".
        click(app, "SAVE_BTN")
        expect_value(app, "STATUS_TEXT", "value", "saved", 300)

        // type_text auto-focuses NAME_INPUT, settles, then sends chars.
        // expect_value confirms the chars landed on state.value.
        type_text(app, "NAME_INPUT", "hello")
        expect_value(app, "NAME_INPUT", "value", "hello", 300)

        t |> success(true, "save_demo end-to-end: click + type_text auto-focus + expect_value")
    }
}
