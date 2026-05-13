options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! §4.7 `with_style((key, val), ...) { ... }` smoke. ImGui's style stack
//! asserts unbalanced Push/Pop counts at EndFrame, so a leak from the
//! macro's pop count would crash the live subprocess and the test would
//! never reach widget assertions. Reaching the assertions at all is the
//! primary signal; widget rendering + click dispatch is the secondary
//! signal that styled buttons stay interactive.

let STYLE_FEATURE = "modules/dasImgui/examples/features/style_override.das"

[test]
def test_with_style_smoke(t : T?) {
    with_imgui_app(STYLE_FEATURE) $(d) {
        // All three buttons must reach the snapshot. STYLED_BTN sits under a
        // 2-style with_style; NESTED_STYLED_BTN sits under nested with_style
        // blocks; UNSTYLED_BTN sits AFTER all with_style blocks (proves
        // pops balanced — if not, EndFrame would assert and the subprocess
        // would die before this snapshot resolves).
        var snap0 = wait_for_widget(d, "STYLE_WIN/UNSTYLED_BTN", 15.0f)
        t |> success(snap0 != null, "post-with_style button registers (pop count balanced)")
        if (snap0 == null) return

        t |> success(widget_exists(snap0, "STYLE_WIN/STYLED_BTN"),
                     "STYLED_BTN inside 2-style with_style registers")
        t |> success(widget_exists(snap0, "STYLE_WIN/NESTED_STYLED_BTN"),
                     "NESTED_STYLED_BTN inside nested with_style registers")

        // Styled buttons stay interactive — L2 click dispatch works through
        // the synthesized push/invoke/pop body.
        click(d, "STYLE_WIN/STYLED_BTN")
        t |> success(wait_for_int_value(d, "STYLE_WIN/STYLED_BTN", "click_count", 1, 300),
                     "STYLED_BTN.click_count increments after click")

        click(d, "STYLE_WIN/NESTED_STYLED_BTN")
        t |> success(wait_for_int_value(d, "STYLE_WIN/NESTED_STYLED_BTN", "click_count", 1, 300),
                     "NESTED_STYLED_BTN.click_count increments after click")
    }
}
