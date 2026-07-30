options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `popup_context_window` — right-click context menu
//! over the host window. Verifies kind, `right_click` opens the popup, menu
//! items register at container-nested paths, and selecting items via `click`
//! updates the LAST_PICK display state. right_click / click are real synthetic
//! clicks (press+release through ImGui's input path; the cursor warps to the
//! bbox center and settles a frame), so they fire the demo's `if (menu_item(...))`
//! branches exactly like a user click — no raw-synth pathway needed.

let FEATURE = "modules/dasImgui/examples/features/popup_context_window.das"

[test]
def test_popup_context_window_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/CTX", 15.0f)
        t |> success(snap != null, "CTX container registers")
        if (snap == null) return

        let ctx = find_widget(snap, "MAIN_WIN/CTX")
        t |> equal(ctx?["kind"] ?? "", "popup_context_window",
                   "CTX.kind == 'popup_context_window'")
        t |> equal(widget_payload_field(snap, "MAIN_WIN/STATUS", "value") ?? "",
                   "last pick: (none yet)",
                   "STATUS shows '(none yet)' initially")
        t |> success(!widget_exists(snap, "MAIN_WIN/CTX/ITEM_NEW"),
                     "ITEM_NEW absent while popup closed")

        // Right-click LEAD to open the context popup. Gate on wait_for_visible (rendered THIS frame),
        // not wait_for_widget (exists-only) — see the re-open note below.
        right_click(d, "MAIN_WIN/LEAD")
        var snap_open = wait_for_visible(d, "MAIN_WIN/CTX/ITEM_NEW", 5.0f)
        t |> success(snap_open != null, "ITEM_NEW visible after right-click on LEAD")
        if (snap_open == null) return

        // Pick "New" — a real synthetic click fires the demo's `if (menu_item(...))` branch.
        click(d, "MAIN_WIN/CTX/ITEM_NEW")
        t |> success(wait_for_string_value(d, "MAIN_WIN/STATUS", "value",
                                           "last pick: New", 300),
                     "STATUS reflects 'New' after menu pick")

        // Re-open + pick "Quit" — the case that used to race. ITEM_QUIT's registry entry persists
        // from the first open, so wait_for_widget (exists-only) returns stale while the popup is
        // closed and the click drops (no bbox). wait_for_visible gates on rendered-this-frame instead.
        right_click(d, "MAIN_WIN/LEAD")
        var snap_open2 = wait_for_visible(d, "MAIN_WIN/CTX/ITEM_QUIT", 5.0f)
        t |> success(snap_open2 != null, "ITEM_QUIT visible on re-open")
        if (snap_open2 == null) return
        click(d, "MAIN_WIN/CTX/ITEM_QUIT")
        let got_quit = wait_for_string_value(d, "MAIN_WIN/STATUS", "value",
                                             "last pick: Quit", 300)
        if (!got_quit) {
            // DIAGNOSTIC: on failure surface what the click did — STATUS shows the last pick that
            // registered (still 'New' = the click never landed), and whether the items still exist.
            var diag = snapshot(d)
            let stat   = widget_payload_field(diag, "MAIN_WIN/STATUS", "value") ?? "<none>"
            let iq_ex  = widget_exists(diag, "MAIN_WIN/CTX/ITEM_QUIT")
            let in_ex  = widget_exists(diag, "MAIN_WIN/CTX/ITEM_NEW")
            to_log(LOG_WARNING, "[popup-diag] second pick FAILED: STATUS='{stat}' ITEM_QUIT_exists={iq_ex} ITEM_NEW_exists={in_ex}\n")
        }
        t |> success(got_quit, "STATUS reflects 'Quit' after second menu pick")
    }
}
