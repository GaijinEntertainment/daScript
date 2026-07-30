options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `edit_tab_item`. Demo binds two closable tabs
//! (TAB_A, TAB_B) to module-scope bools via safe_addr. Verify both render
//! initially, both bodies register, and `imgui_force_set TAB_A false` flips A's
//! bool — body and header disappear, the linked CHECK_A also unchecks
//! (bidir bind), and STATUS reflects "a=false b=true".

let FEATURE = "modules/dasImgui/examples/features/edit_tab_item.das"

[test]
def test_edit_tab_item_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/BAR/TAB_A", 15.0f)
        t |> success(snap != null, "TAB_A registers")
        if (snap == null) return

        t |> equal(find_widget(snap, "MAIN_WIN/BAR/TAB_A")?["kind"] ?? "", "edit_tab_item",
                   "TAB_A.kind == edit_tab_item")
        for (ident in ["LEAD", "CHECK_A", "CHECK_B", "STATUS"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "MAIN_WIN/{ident} registers")
        }
        t |> success(widget_exists(snap, "MAIN_WIN/BAR/TAB_B"), "TAB_B registers")

        // Initially both open: A is selected by default; BODY_A visible.
        t |> success(widget_exists(snap, "MAIN_WIN/BAR/TAB_A/BODY_A"),
                     "BODY_A visible when alpha tab is active")
        t |> equal(widget_payload_field(snap, "MAIN_WIN/STATUS", "value") ?? "",
                   "a=true b=true",
                   "STATUS == 'a=true b=true' initially")

        // Flip A closed via the external bool dispatcher; body+chrome vanish.
        force_set_value(d, "MAIN_WIN/BAR/TAB_A", JV(false))
        t |> success(wait_for_string_value(d, "MAIN_WIN/STATUS", "value",
                                           "a=false b=true", 300),
                     "STATUS reflects A closed after force_set_value(TAB_A, false)")

        // Restore A via the linked checkbox; bidir bind makes the tab reappear.
        force_set_value(d, "MAIN_WIN/CHECK_A", JV(true))
        t |> success(wait_for_string_value(d, "MAIN_WIN/STATUS", "value",
                                           "a=true b=true", 300),
                     "STATUS reflects A restored after force_set_value(CHECK_A, true)")
    }
}
