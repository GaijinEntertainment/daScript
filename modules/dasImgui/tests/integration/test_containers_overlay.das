options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 0b.3 overlay containers — popup, popup_modal,
//! tooltip, item_tooltip, combo_select, list_box_select. Verifies the
//! pending-open / pending-close flow on popups and the block-arg combo /
//! list_box paths.

let OVERLAY_FEATURE = "modules/dasImgui/examples/features/containers_overlay.das"

[test]
def test_containers_overlay_smoke(t : T?) {
    with_imgui_app(OVERLAY_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "OVERLAY_WIN/OPEN_POPUP_BTN", 15.0f)
        t |> success(snap0 != null, "OVERLAY_WIN/OPEN_POPUP_BTN registered")
        if (snap0 == null) return

        // ----- combo_select is registered under the window's path -----
        var combo_entry = find_widget(snap0, "OVERLAY_WIN/PALETTE_COMBO")
        t |> equal(combo_entry?["kind"] ?? "", "combo_select",
                   "combo_select registers under nested path")
        var listbox_entry = find_widget(snap0, "OVERLAY_WIN/TASK_LIST")
        t |> equal(listbox_entry?["kind"] ?? "", "list_box_select",
                   "list_box_select registers under nested path")

        // ----- imgui_set on combo_select index -----
        set_value(d, "OVERLAY_WIN/PALETTE_COMBO", JV(2))
        t |> success(wait_for_int_value(d, "OVERLAY_WIN/PALETTE_COMBO", "value", 2, 300),
                     "imgui_set steers PALETTE_COMBO to 2 (Forest)")

        // ----- imgui_set on list_box_select index -----
        set_value(d, "OVERLAY_WIN/TASK_LIST", JV(3))
        t |> success(wait_for_int_value(d, "OVERLAY_WIN/TASK_LIST", "value", 3, 300),
                     "imgui_set steers TASK_LIST to 3 (Package)")

        // ----- imgui_open on a popup flips state.open via the dispatcher -----
        // OPTIONS_POPUP starts closed; opening it via imgui_open routes
        // through pending_open, which the next frame's body consumes by
        // calling OpenPopup() — so the popup body runs and its leaf
        // OPT_VSYNC appears in the snapshot.
        open_widget(d, "OVERLAY_WIN/OPTIONS_POPUP")
        var snap_popup_open = wait_until(d, 300) $(var snap) {
            return widget_rendered(snap, "OVERLAY_WIN/OPTIONS_POPUP/OPT_VSYNC")
        }
        t |> success(snap_popup_open != null,
                     "imgui_open OPTIONS_POPUP renders popup body (OPT_VSYNC visible)")

        // ----- imgui_close on the open popup closes it -----
        close_widget(d, "OVERLAY_WIN/OPTIONS_POPUP")
        var snap_popup_closed = wait_until(d, 300) $(var snap) {
            return !widget_rendered(snap, "OVERLAY_WIN/OPTIONS_POPUP/OPT_VSYNC")
        }
        t |> success(snap_popup_closed != null,
                     "imgui_close OPTIONS_POPUP hides popup body next frame")

        // ----- imgui_open on popup_modal -----
        open_widget(d, "OVERLAY_WIN/CONFIRM_MODAL")
        var snap_modal = wait_until(d, 300) $(var snap) {
            return widget_rendered(snap, "OVERLAY_WIN/CONFIRM_MODAL/CONFIRM_YES")
        }
        t |> success(snap_modal != null,
                     "imgui_open CONFIRM_MODAL renders modal body")

        // Modal stays up until pending_close fires; close it via imgui_close.
        close_widget(d, "OVERLAY_WIN/CONFIRM_MODAL")
        var snap_modal_closed = wait_until(d, 300) $(var snap) {
            return !widget_rendered(snap, "OVERLAY_WIN/CONFIRM_MODAL/CONFIRM_YES")
        }
        t |> success(snap_modal_closed != null,
                     "imgui_close CONFIRM_MODAL closes the modal")
    }
}
