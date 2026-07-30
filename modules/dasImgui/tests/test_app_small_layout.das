options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Smoke for app_small ShowExampleAppLayout — verifies the window opens and
//! the left-pane / right-pane container structure registers.

let LAYOUT_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_small_layout.das"

[test]
def test_app_small_layout_smoke(t : T?) {
    with_imgui_app(LAYOUT_FEATURE) $(d) {
        // First selectable in the left pane — nested under the LEFT_CHILD container.
        var snap0 = wait_for_widget(d, "SMALL_LAYOUT_WIN/SMALL_LAYOUT_LEFT_CHILD/SMALL_LAYOUT_LEFT_SEL[0]", 15.0f)
        t |> success(snap0 != null,
                     "SMALL_LAYOUT_WIN/SMALL_LAYOUT_LEFT_CHILD/SMALL_LAYOUT_LEFT_SEL[0] registers after first frame")
        if (snap0 == null) return

        // Window container itself.
        var win_entry = find_widget(snap0, "SMALL_LAYOUT_WIN")
        t |> equal(win_entry?["kind"] ?? "", "window",
                   "SMALL_LAYOUT_WIN registers as kind=window")

        // Menu-bar + File menu container chain.
        var mb = find_widget(snap0, "SMALL_LAYOUT_WIN/SMALL_LAYOUT_MENU_BAR")
        t |> equal(mb?["kind"] ?? "", "menu_bar",
                   "menu_bar nested under window registers under hierarchical path")

        // First selectable is the kind=selectable confirmation.
        var sel0 = find_widget(snap0, "SMALL_LAYOUT_WIN/SMALL_LAYOUT_LEFT_CHILD/SMALL_LAYOUT_LEFT_SEL[0]")
        t |> equal(sel0?["kind"] ?? "", "selectable",
                   "first selectable in the 100-row loop registers under child path")

        // Right-pane chain: button is nested under the group container.
        var btn = find_widget(snap0, "SMALL_LAYOUT_WIN/SMALL_LAYOUT_RIGHT_GROUP/SMALL_LAYOUT_BTN_REVERT")
        t |> equal(btn?["kind"] ?? "", "button",
                   "Revert button registers under window/group hierarchical path")
    }
}
