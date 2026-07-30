options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `set_scroll_here_y`. The feature renders 200 rows
//! inside a 360px scrollable child. Initial scroll_y is 0 (top); clicking
//! JUMP_MIDDLE sets g_jump_target = 100 -> the next frame's loop fires
//! `set_scroll_here_y(0.5)` after drawing row 100 -> child's scroll.y snaps
//! to ~mid-content (~1700px). Asserting on `>= 1000` keeps the test robust
//! against font/padding variance.

let FEATURE = "modules/dasImgui/examples/features/scroll_here_y.das"

[test]
def test_scroll_here_y_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/SCROLL_AREA", 15.0f)
        t |> success(snap != null, "SCROLL_AREA child registers")
        if (snap == null) return

        for (ident in ["LEAD", "JUMP_TOP", "JUMP_MIDDLE", "JUMP_BOTTOM"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "MAIN_WIN/{ident} registers")
        }
        t |> success(widget_exists(snap, "MAIN_WIN/SCROLL_AREA/ROW[0]"),
                     "ROW[0] in initial visible chunk")

        // Initial scroll: child opens at top.
        let initial_y = find_widget(snap, "MAIN_WIN/SCROLL_AREA")?["payload"]?["scroll"]?["y"] ?? -1.0f
        t |> success(initial_y == 0.0f,
                     "initial SCROLL_AREA.scroll.y = {initial_y}")

        // Click JUMP_MIDDLE -> set_scroll_here_y(0.5) after row 100 -> scroll snaps.
        click(d, "MAIN_WIN/JUMP_MIDDLE")
        let scrolled = wait_until(d, 300) $(var s) {
            let y = s?["globals"]?["MAIN_WIN/SCROLL_AREA"]?["payload"]?["scroll"]?["y"] ?? 0.0f
            return y >= 1000.0f
        }
        t |> success(scrolled != null,
                     "SCROLL_AREA.scroll.y >= 1000 after JUMP_MIDDLE click")

        // JUMP_TOP returns to top.
        click(d, "MAIN_WIN/JUMP_TOP")
        let back_to_top = wait_until(d, 300) $(var s) {
            let y = s?["globals"]?["MAIN_WIN/SCROLL_AREA"]?["payload"]?["scroll"]?["y"] ?? 1.0f
            return y == 0.0f
        }
        t |> success(back_to_top != null,
                     "SCROLL_AREA.scroll.y == 0 after JUMP_TOP click")
    }
}
