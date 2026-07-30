options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Smoke for app_small ShowExampleAppPropertyEditor — verifies the table
//! container + first object's tree_node row register.

let PROPED_FEATURE = "modules/dasImgui/examples/imgui_demo/harness_app_small_property_editor.das"

[test]
def test_app_small_property_editor_smoke(t : T?) {
    with_imgui_app(PROPED_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "SMALL_PROPED_WIN/SMALL_PROPED_TABLE", 15.0f)
        t |> success(snap0 != null,
                     "SMALL_PROPED_WIN/SMALL_PROPED_TABLE registers after first frame")
        if (snap0 == null) return

        var tab = find_widget(snap0, "SMALL_PROPED_WIN/SMALL_PROPED_TABLE")
        t |> equal(tab?["kind"] ?? "", "data_table",
                   "SMALL_PROPED_TABLE registers as kind=data_table")
    }
}
