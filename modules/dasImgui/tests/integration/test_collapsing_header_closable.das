options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for collapsing_header(closable=true) — verifies
//! the close-button route: imgui_close hides; imgui_open re-renders.

let FEATURE = "modules/dasImgui/examples/features/collapsing_header_closable.das"

[test]
def test_collapsing_header_closable_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/HEADER_CLOSE", 15.0f)
        t |> success(snap != null, "HEADER_CLOSE renders open")
        if (snap == null) return

        let open0 = widget_payload_field(snap, "MAIN_WIN/HEADER_CLOSE", "open") ?? false
        t |> equal(open0, true, "header starts open")

        close_widget(d, "MAIN_WIN/HEADER_CLOSE")
        let closed = wait_until(d, 300) $(var s) {
            return !widget_exists(s, "MAIN_WIN/HEADER_CLOSE/REVEAL_BTN")
        }
        t |> success(closed != null, "closing the header hides REVEAL_BTN inside")

        // Re-open must gate on actual paint, not stale registry existence:
        // widget_exists returns true on the expunged-but-cached entry. wait_for_visible
        // polls widget_rendered (registered AND painted this frame).
        open_widget(d, "MAIN_WIN/HEADER_CLOSE")
        t |> success(wait_for_visible(d, "MAIN_WIN/HEADER_CLOSE/REVEAL_BTN", 5.0f) != null,
                     "imgui_open re-renders body")
    }
}
