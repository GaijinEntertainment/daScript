options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Smoke for `child_scroll_reenter` against the
//! ``examples/features/child_scroll_reenter.das`` feature. Clicks the down
//! button (which calls `child_scroll_reenter("SCROLLY", "lines") {
//! SetScrollY(...) }`) and asserts the original child's observed scroll.y
//! actually changes. The original child publishes its scroll into
//! ``SCROLLY.scroll`` from inside its body — if the wrapper's PushID was
//! wrong, BeginChild would hash to a phantom sibling and SCROLLY.scroll.y
//! would never move (the contrapositive of "this test passes" is "the
//! PushID frame in child_scroll_reenter matches the original child").

let FEATURE = "modules/dasImgui/examples/features/child_scroll_reenter.das"

[test]
def test_child_scroll_reenter_moves_scroll(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/SCROLLY", 15.0f)
        t |> success(snap0 != null, "SCROLLY child renders")
        if (snap0 == null) return
        // SCROLLY.scroll.y starts at 0.0 — the child opens scrolled to top.
        let y0 : double = find_widget(snap0, "MAIN_WIN/SCROLLY")?["payload"]?["scroll"]?["y"] ?? 0.0lf
        t |> equal(y0, 0.0lf, "initial SCROLLY.scroll.y == 0")
        // One click fires child_scroll_reenter once, moving scroll by `delta = 8.0f` (per the
        // feature file). Click several times for a clearly-non-zero observation robust to
        // single-frame jitter / button-repeat timing.
        for (_i in range(5)) {
            click(d, "MAIN_WIN/BTN_DOWN")
        }
        await_quiescent(d, 5.0f)
        var snap1 = snapshot(d)
        t |> success(snap1 != null, "post-click snapshot available")
        if (snap1 == null) return
        let y1 : double = find_widget(snap1, "MAIN_WIN/SCROLLY")?["payload"]?["scroll"]?["y"] ?? 0.0lf
        t |> success(y1 > 0.0lf,
                     "SCROLLY.scroll.y moved (y={y1}); wrapper's PushID(IDENT) reentered the same window")
    }
}
