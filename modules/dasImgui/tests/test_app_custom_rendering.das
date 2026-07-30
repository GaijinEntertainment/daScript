options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Smoke for imgui_demo ShowExampleAppCustomRendering — host window opens,
//! the TabBar registers, the default-active Primitives tab renders, and
//! the context-menu popup is INSTALLED (not open). Drag/click matrix is
//! out of scope here; this is the structural-rendering rail.

let CR_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_custom_rendering.das"

[test]
def test_app_custom_rendering_smoke(t : T?) {
    with_imgui_app(CR_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "CR_WIN", 15.0f)
        t |> success(snap0 != null, "CR_WIN host window renders")
        if (snap0 == null) return

        t |> success(widget_exists(snap0, "CR_WIN/CR_TABS"),
                     "CR_WIN/CR_TABS tab_bar registers")
        let tab_prim = find_widget(snap0, "CR_WIN/CR_TABS/CR_TAB_PRIM")
        t |> equal(tab_prim?["kind"] ?? "", "tab_item",
                   "CR_TAB_PRIM (default-active) renders as tab_item")

        // Context-menu popup is always installed (per the cpp port) but
        // open=false by default — confirms the popup container is wired.
        let ctx = find_widget(snap0, "CR_CV_CTX_MENU")
        t |> equal(ctx?["kind"] ?? "", "popup",
                   "CR_CV_CTX_MENU registered as kind=popup")
        t |> equal(ctx?["payload"]?["open"] ?? true, false,
                   "context menu starts closed")
    }
}
