options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `popup_context_item` — right-click context menu
//! attached to the previously submitted item. The popup itself drives
//! open/close internally (no pending_open/close). This test verifies
//! TARGET_BTN registers and the popup_context_item container is itself
//! reachable in the snapshot path tree.

let POPUP_CTX_FEATURE = "modules/dasImgui/examples/features/popup_context_item.das"

[test]
def test_popup_context_item_smoke(t : T?) {
    with_imgui_app(POPUP_CTX_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/TARGET_BTN", 15.0f)
        t |> success(snap0 != null, "TARGET_BTN registers")
        if (snap0 == null) return

        // popup_context_item itself registers at MAIN_WIN/TARGET_CTX even
        // when closed — the [container] wrapper calls stateless_finalize
        // every frame regardless of Begin's return.
        let ctx_entry = find_widget(snap0, "MAIN_WIN/TARGET_CTX")
        t |> equal(ctx_entry?["kind"] ?? "", "popup_context_item",
                   "TARGET_CTX.kind == 'popup_context_item'")
    }
}
