options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 main_menu_bar container.
//! Drives `examples/features/menu_main.das`; verifies the main_menu_bar
//! kind registers + nested menu_item paths surface under MAIN_MENU_BAR/.

let FEATURE = "modules/dasImgui/examples/features/menu_main.das"

[test]
def test_menu_main_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_MENU_BAR", 15.0f)
        t |> success(snap != null, "MAIN_MENU_BAR renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_MENU_BAR"),
                     "main_menu_bar container registered")
        // Menus + items only emit entries while the menu is open (BeginMenu returns true), so
        // container existence alone is the headless-drive smoke gate; opening + clicking is
        // exercised by record_menu_main.das.
    }
}
