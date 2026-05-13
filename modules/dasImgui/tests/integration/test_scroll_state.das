options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 1.2 — scroll capture on window + child.
//!
//! Drives `examples/features/scroll_state.das` to verify:
//!   1. WindowState.scroll_max.y is non-zero on a window whose body
//!      overflows (40 rows of text → vertical scrollbar appears).
//!   2. ChildState.scroll_max.y is non-zero on a child region whose body
//!      overflows (25 rows in a 140-px-tall child).
//!   3. Both `scroll` and `scroll_max` are float2 pairs surfaced through
//!      the standard state-struct serializer (no separate extras channel).

let SCROLL_FEATURE = "modules/dasImgui/examples/features/scroll_state.das"

[test]
def test_scroll_state_smoke(t : T?) {
    with_imgui_app(SCROLL_FEATURE) $(d) {
        var snap = wait_for_render(d, "SCROLLY_WIN", 15.0f)
        t |> success(snap != null, "SCROLLY_WIN renders")
        if (snap == null) return
        // Gate 1: window's overflow content gives scroll_max.y > 0.
        // JV(float2) serializes as an object {"x":..., "y":...} (not as
        // an array), so navigate by named field.
        var win_scroll_max = widget_payload_field(snap, "SCROLLY_WIN", "scroll_max")
        t |> success(win_scroll_max != null,
                     "SCROLLY_WIN.payload.scroll_max present")
        let win_max_y = win_scroll_max?["y"] ?? 0.0f
        t |> success(win_max_y > 0.0f,
                     "SCROLLY_WIN.scroll_max.y > 0 (got {win_max_y})")

        // Gate 2: same shape for child region.
        var child_snap = wait_for_render(d, "NESTED_WIN/LOG_REGION", 5.0f)
        t |> success(child_snap != null, "NESTED_WIN/LOG_REGION renders")
        if (child_snap == null) return
        var child_scroll_max = widget_payload_field(child_snap, "NESTED_WIN/LOG_REGION", "scroll_max")
        t |> success(child_scroll_max != null,
                     "NESTED_WIN/LOG_REGION.payload.scroll_max present")
        let child_max_y = child_scroll_max?["y"] ?? 0.0f
        t |> success(child_max_y > 0.0f,
                     "NESTED_WIN/LOG_REGION.scroll_max.y > 0 (got {child_max_y})")

        // Gate 3: scroll itself is a float2 object (initial value {x:0, y:0}).
        var win_scroll = widget_payload_field(child_snap, "SCROLLY_WIN", "scroll")
        t |> success(win_scroll != null, "SCROLLY_WIN.payload.scroll present")
    }
}
