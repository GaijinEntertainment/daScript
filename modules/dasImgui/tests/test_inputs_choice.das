options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 0b.2 choice widgets — `combo` (zero-separated
//! buffer + reinterpret<string#>) and `list_box` (direct addr-of-array
//! pass-through), driven through `imgui_force_set` via the playwright-style harness.

let CHOICE_FEATURE = "modules/dasImgui/examples/features/inputs_choice.das"

[test]
def test_inputs_choice_smoke(t : T?) {
    with_imgui_app(CHOICE_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/FRUIT", 15.0f)
        t |> success(snap0 != null, "FRUIT registered after first frame")
        if (snap0 == null) return

        // ----- Snapshot shape -----
        var fruit0 = find_widget(snap0, "MAIN_WIN/FRUIT")
        t |> equal(fruit0?["kind"] ?? "", "combo", "FRUIT.kind == 'combo'")
        t |> success(fruit0?["payload"] != null, "FRUIT.payload present")

        var size0 = find_widget(snap0, "MAIN_WIN/SHIRT_SIZE")
        t |> equal(size0?["kind"] ?? "", "list_box", "SHIRT_SIZE.kind == 'list_box'")

        // ----- Combo — imgui_force_set jumps to N-th item -----
        force_set_value(d, "MAIN_WIN/FRUIT", JV(2))
        t |> success(wait_for_int_value(d, "MAIN_WIN/FRUIT", "value", 2, 300),
                     "imgui_force_set steers FRUIT to index 2 (Cherry)")

        force_set_value(d, "MAIN_WIN/FRUIT", JV(4))
        t |> success(wait_for_int_value(d, "MAIN_WIN/FRUIT", "value", 4, 300),
                     "imgui_force_set steers FRUIT to index 4 (Elderberry)")

        force_set_value(d, "MAIN_WIN/WEEKDAY", JV(6))
        t |> success(wait_for_int_value(d, "MAIN_WIN/WEEKDAY", "value", 6, 300),
                     "imgui_force_set steers WEEKDAY to index 6 (Sun)")

        // ----- ListBox — same dispatch shape -----
        force_set_value(d, "MAIN_WIN/SHIRT_SIZE", JV(3))
        t |> success(wait_for_int_value(d, "MAIN_WIN/SHIRT_SIZE", "value", 3, 300),
                     "imgui_force_set steers SHIRT_SIZE to index 3 (L)")

        force_set_value(d, "MAIN_WIN/SHIRT_SIZE", JV(0))
        t |> success(wait_for_int_value(d, "MAIN_WIN/SHIRT_SIZE", "value", 0, 300),
                     "imgui_force_set steers SHIRT_SIZE to index 0 (XS)")

        // ----- Final snapshot envelope -----
        var snap_final = snapshot(d)
        var fruit_final = find_widget(snap_final, "MAIN_WIN/FRUIT")
        t |> equal(fruit_final?["payload"]?["value"] ?? -1, 4,
                   "FRUIT.payload.value persists final imgui_force_set")
        var size_final = find_widget(snap_final, "MAIN_WIN/SHIRT_SIZE")
        t |> equal(size_final?["payload"]?["value"] ?? -1, 0,
                   "SHIRT_SIZE.payload.value persists final imgui_force_set")
    }
}
