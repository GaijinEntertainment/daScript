options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Smoke for app_small ShowExampleAppSimpleOverlay — verifies the overlay
//! window registers and the popup_context_window container is wired
//! (popup body renders only on right-click, so we don't assert its leaves).

let OVERLAY_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_small_simple_overlay.das"

[test]
def test_app_small_simple_overlay_smoke(t : T?) {
    with_imgui_app(OVERLAY_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "SMALL_OVERLAY_WIN/SMALL_OVERLAY_SEP", 15.0f)
        t |> success(snap0 != null,
                     "SMALL_OVERLAY_WIN/SMALL_OVERLAY_SEP registers after first frame")
        if (snap0 == null) return

        var win_entry = find_widget(snap0, "SMALL_OVERLAY_WIN")
        t |> equal(win_entry?["kind"] ?? "", "window",
                   "SMALL_OVERLAY_WIN registers as kind=window")

        // popup_context_window registers but its body only fires when right-clicked.
        var ctx = find_widget(snap0, "SMALL_OVERLAY_WIN/SMALL_OVERLAY_CTX")
        t |> equal(ctx?["kind"] ?? "", "popup_context_window",
                   "context popup container registers under window path")
    }
}
