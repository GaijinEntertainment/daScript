options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `popup_window` — stateless [container] for the
//! manual-trigger BeginPopup/EndPopup pattern (caller drives open_popup,
//! ImGui owns the open state by str_id). Verifies both shared-str_id popup
//! instances register, and that clicking the explicit trigger surfaces the
//! popup body widgets at the expected scoped path.

let POPUP_WINDOW_FEATURE = "modules/dasImgui/examples/features/popup_window.das"

[test]
def test_popup_window(t : T?) {
    with_imgui_app(POPUP_WINDOW_FEATURE) $(d) {
        t |> run("smoke") <| @(t : T?) {
            var snap0 = wait_for_widget(d, "MAIN_WIN/via_button/OPEN_BTN", 15.0f)
            t |> success(snap0 != null, "OPEN_BTN registers under with_id(via_button) scope")
            if (snap0 == null) return

            // Both popup_window instances register every frame via stateless_finalize,
            // regardless of open/closed.
            let btn_entry = find_widget(snap0, "MAIN_WIN/via_button/BTN_POPUP")
            t |> equal(btn_entry?["kind"] ?? "", "popup_window",
                       "BTN_POPUP.kind == 'popup_window'")
            let reg_entry = find_widget(snap0, "MAIN_WIN/via_region/REG_POPUP")
            t |> equal(reg_entry?["kind"] ?? "", "popup_window",
                       "REG_POPUP.kind == 'popup_window'")
        }
        reset(d)

        t |> run("button_opens") <| @(t : T?) {
            var snap0 = wait_for_widget(d, "MAIN_WIN/via_button/OPEN_BTN", 15.0f)
            t |> success(snap0 != null, "feature ready")
            if (snap0 == null) return

            // Click OPEN_BTN. The button handler calls open_popup("fruit_picker");
            // popup_window brackets the body on the next frame, so PICK_APPLE
            // appears in the snapshot at the scoped path.
            click(d, "MAIN_WIN/via_button/OPEN_BTN")
            // Popups are re-showable, so gate on actual paint — wait_for_visible is
            // exists AND rendered-this-frame. (widget_rendered now requires existence,
            // so it no longer false-passes for an absent path.)
            var snap_open = wait_for_visible(d, "MAIN_WIN/via_button/BTN_POPUP/PICK_APPLE", 5.0f)
            t |> success(snap_open != null,
                         "click OPEN_BTN opens BTN_POPUP body (PICK_APPLE visible)")
        }
    }
}
