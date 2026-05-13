options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 0b.0 foundation.
//!
//! Drives `examples/features/foundation.das` to verify the four 0b.0 contracts:
//!   1. Auto-emit — SAVE_BTN/SPEED/VOLUME globals appear without manual `var`
//!      decls (visible as registry keys after the first frame).
//!   2. Dotted-suffix flags — SPEED.PUBLIC and VOLUME.NOTLIVE bypass parser
//!      cleanly (no compile error) AND emit working widgets keyed under bare
//!      identifiers ("SPEED", "VOLUME").
//!   3. Lazy serializer — snapshot returns per-kind payloads (click_count for
//!      ClickState, value+bounds for SliderStateInt/Float).
//!   4. Unified L2/L3 dispatch — imgui_click steers the button, imgui_set
//!      steers the sliders, all through one g_dispatchers table.

let FOUNDATION_FEATURE = "modules/dasImgui/examples/features/foundation.das"

[test]
def test_foundation_smoke(t : T?) {
    with_imgui_app(FOUNDATION_FEATURE) $(d) {
        // Gate 1: auto-emit — SAVE_BTN exists in registry without any manual decl.
        var snap0 = wait_for_widget(d, "SAVE_BTN", 15.0f)
        t |> success(snap0 != null, "SAVE_BTN auto-emitted (registry has it)")
        if (snap0 == null) return

        // Gate 2: dotted-suffix flags — bare identifiers as registry keys.
        // SPEED came from `slider_int(SPEED.PUBLIC, ...)`; key must be "SPEED",
        // not "SPEED.PUBLIC". Same for VOLUME.NOTLIVE → "VOLUME".
        t |> success(widget_exists(snap0, "SPEED"), "SPEED.PUBLIC keys as 'SPEED'")
        t |> success(widget_exists(snap0, "VOLUME"), "VOLUME.NOTLIVE keys as 'VOLUME'")
        t |> success(!widget_exists(snap0, "SPEED.PUBLIC"), "no 'SPEED.PUBLIC' literal key")
        t |> success(!widget_exists(snap0, "VOLUME.NOTLIVE"), "no 'VOLUME.NOTLIVE' literal key")

        // Gate 3: lazy serializer — payload shapes match per kind.
        var save = find_widget(snap0, "SAVE_BTN")
        t |> equal(save?["kind"] ?? "", "button", "SAVE_BTN.kind")
        t |> success(save?["payload"]?["click_count"] != null, "ClickState payload.click_count")

        var speed = find_widget(snap0, "SPEED")
        t |> equal(speed?["kind"] ?? "", "slider_int", "SPEED.kind")
        t |> success(speed?["payload"]?["value"] != null, "SliderStateInt payload.value")
        t |> success(speed?["payload"]?["bounds"] != null, "SliderStateInt payload.bounds")

        var volume = find_widget(snap0, "VOLUME")
        t |> equal(volume?["kind"] ?? "", "slider_float", "VOLUME.kind")
        t |> success(volume?["payload"]?["value"] != null, "SliderStateFloat payload.value")

        // Gate 4: unified dispatch — imgui_click and imgui_set both work.
        let count_before = save?["payload"]?["click_count"] ?? 0
        click(d, "SAVE_BTN")
        t |> success(wait_for_int_value(d, "SAVE_BTN", "click_count", count_before + 1, 300),
                     "imgui_click bumps SAVE_BTN.click_count")

        set_value(d, "SPEED", JV(7))
        t |> success(wait_for_int_value(d, "SPEED", "value", 7, 300),
                     "imgui_set steers SPEED.value to 7")
    }
}
