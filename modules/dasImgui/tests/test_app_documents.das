options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Smoke for imgui_demo ShowExampleAppDocuments — host window + tab bar
//! + per-doc state. Also verifies the round-1/round-2 fixes:
//!   - DOCS_OPEN_CHECKBOX[i].value mirrors doc.open (seeded-open docs
//!     render checked, not unchecked).
//!   - DOCS_CLOSE_MODAL container is installed but starts closed (no
//!     dirty docs in fresh fixtures).

let DOCS_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_documents.das"

[test]
def test_app_documents_smoke(t : T?) {
    with_imgui_app(DOCS_FEATURE) $(d) {
        var snap = wait_for_widget(d, "DOCS_WIN", 15.0f)
        t |> success(snap != null, "DOCS_WIN host window renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "DOCS_WIN/DOCS_MENUBAR/DOCS_FILE_MENU"),
                     "File menu in the menu bar")
        t |> success(widget_exists(snap, "DOCS_WIN/DOCS_TABS"),
                     "DOCS_TABS tab_bar registers")

        // Per round-1/#11: the debug checkbox mirrors doc.open every frame.
        // seed_docs() opens docs 0, 1, 2 and leaves 3, 4, 5 closed.
        let cb0_val = find_widget(snap, "DOCS_WIN/DOCS_OPEN_CHECKBOX[0]")?["payload"]?["value"] ?? false
        t |> equal(cb0_val, true, "DOCS_OPEN_CHECKBOX[0] mirrors seeded doc.open=true (Lettuce)")
        let cb3_val = find_widget(snap, "DOCS_WIN/DOCS_OPEN_CHECKBOX[3]")?["payload"]?["value"] ?? true
        t |> equal(cb3_val, false, "DOCS_OPEN_CHECKBOX[3] mirrors seeded doc.open=false (Tomato)")

        // Close-confirmation modal is installed; opens only when a dirty
        // doc is being closed. Fresh fixture: starts closed.
        let modal = find_widget(snap, "DOCS_CLOSE_MODAL")
        t |> equal(modal?["kind"] ?? "", "popup_modal",
                   "DOCS_CLOSE_MODAL registered as popup_modal")
        t |> equal(modal?["payload"]?["open"] ?? true, false,
                   "close-confirmation modal starts closed (no dirty docs)")
    }
}
