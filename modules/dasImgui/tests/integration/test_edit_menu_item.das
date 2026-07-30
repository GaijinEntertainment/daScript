options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 edit_menu_item rail.
//! Drives `examples/features/edit_menu_item.das` — verifies the menu bar +
//! its registered menu containers exist. Menu_items themselves only emit
//! bbox entries while their parent menu is OPEN (matching the pattern
//! documented in skills/recording.md), so existence-only smoke is the
//! headless gate. Click + toggle are exercised by record_main_menu_bar.

let FEATURE = "modules/dasImgui/examples/features/edit_menu_item.das"

[test]
def test_edit_menu_item_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MENU_BAR", 15.0f)
        t |> success(snap != null, "MENU_BAR renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "MENU_BAR"),
                     "main_menu_bar container registered")
        t |> success(widget_exists(snap, "MENU_BAR/FILE_MENU"),
                     "File menu header registered")
        t |> success(widget_exists(snap, "MENU_BAR/VIEW_MENU"),
                     "View menu header registered")
        t |> success(widget_exists(snap, "BODY"),
                     "BODY readout window registered")
    }
}
