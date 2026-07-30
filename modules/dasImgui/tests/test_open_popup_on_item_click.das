options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `open_popup_on_item_click` (stateful overload). The
//! demo binds right-click on BTN_TARGET to open the MENU popup via the
//! stateful trigger. We verify wiring by opening the popup directly via
//! `open(d, "MAIN_WIN/MENU")` (the L2 dispatch) — the popup chrome + body
//! widgets register, BTN_OK click increments g_picks, STATUS updates.
//! (The right-click-to-open path itself depends on synth-cursor + IsItemHovered
//! frame-timing in the GLFW backend; that's covered by record_*.das drivers.)

let FEATURE = "modules/dasImgui/examples/features/open_popup_on_item_click.das"

[test]
def test_open_popup_on_item_click_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/BTN_TARGET", 15.0f)
        t |> success(snap != null, "BTN_TARGET registers")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/MENU"),
                     "MENU popup container registers")
        t |> equal(find_widget(snap, "MAIN_WIN/MENU")?["kind"] ?? "", "popup",
                   "MENU.kind == popup")
        t |> equal(widget_payload_field(snap, "MAIN_WIN/STATUS", "value") ?? "",
                   "picks = 0", "initial STATUS = 'picks = 0'")

        // L2 open the popup directly (mirrors the trigger that the
        // open_popup_on_item_click sets when fired). Popup body registers.
        open_widget(d, "MAIN_WIN/MENU")
        var open_snap = wait_for_widget(d, "MAIN_WIN/MENU/BTN_OK", 5.0f)
        t |> success(open_snap != null, "BTN_OK registers once popup is open")
        if (open_snap == null) return

        t |> success(widget_exists(open_snap, "MAIN_WIN/MENU/POPUP_LEAD"),
                     "POPUP_LEAD registers inside popup")

        // Click OK; g_picks++ -> STATUS updates.
        click(d, "MAIN_WIN/MENU/BTN_OK")
        t |> success(wait_for_string_value(d, "MAIN_WIN/STATUS", "value",
                                           "picks = 1", 300),
                     "STATUS reflects 'picks = 1' after BTN_OK click")
    }
}
