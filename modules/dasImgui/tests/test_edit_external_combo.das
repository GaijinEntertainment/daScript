options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for boost v2 edit_combo (array-of-string-pointers form).

let FEATURE = "modules/dasImgui/examples/features/edit_external_combo.das"

[test]
def test_edit_external_combo_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/COMBO_THEME", 15.0f)
        t |> success(snap != null, "COMBO_THEME renders")
        if (snap == null) return

        t |> success(widget_exists(snap, "MAIN_WIN/COMBO_DIFFICULTY"),
                     "COMBO_DIFFICULTY registered")

        let idx0 = widget_payload_field(snap, "MAIN_WIN/COMBO_THEME", "value") ?? 0
        t |> equal(idx0, 1, "initial selected idx = 1 ('Dark')")

        force_set_value(d, "MAIN_WIN/COMBO_THEME", JV(3))
        t |> success(wait_for_int_value(d, "MAIN_WIN/COMBO_THEME", "value", 3, 300),
                     "edit_combo write idx=3 round-trips")
    }
}
