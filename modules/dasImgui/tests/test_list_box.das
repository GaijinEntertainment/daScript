options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration test for unified `list_box` [container] — BeginListBox/EndListBox
//! with Selectable rows in the body. `imgui_force_set {"value": N}` steers next-frame
//! selection via the dispatcher; user-side Selectable click writes state.value.

let LIST_BOX_FEATURE = "modules/dasImgui/examples/features/list_box.das"

[test]
def test_list_box_set_steers_selection(t : T?) {
    with_imgui_app(LIST_BOX_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/LANG", 15.0f)
        t |> success(snap0 != null, "LANG list_box registers")
        if (snap0 == null) return

        let lang_entry = find_widget(snap0, "MAIN_WIN/LANG")
        t |> equal(lang_entry?["kind"] ?? "", "list_box",
                   "LANG.kind == 'list_box'")

        // imgui_force_set steers selection to "Rust" (index 2).
        force_set_value(d, "MAIN_WIN/LANG", JV(2))
        t |> success(wait_for_int_value(d, "MAIN_WIN/LANG", "value", 2, 300),
                     "imgui_force_set steers LANG to index 2 (Rust)")

        // Another step — Python (4).
        force_set_value(d, "MAIN_WIN/LANG", JV(4))
        t |> success(wait_for_int_value(d, "MAIN_WIN/LANG", "value", 4, 300),
                     "imgui_force_set steers LANG to index 4 (Python)")

        // Final snapshot envelope.
        var snap_final = snapshot(d)
        let lang_final = find_widget(snap_final, "MAIN_WIN/LANG")
        t |> equal(lang_final?["payload"]?["value"] ?? -1, 4,
                   "LANG.payload.value persists final imgui_force_set")
    }
}
