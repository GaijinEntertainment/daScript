options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record dropdown_select.apng against examples/tutorial/dropdown_select.das.
//! Four pick-from-list shapes, each driven the human way (open + click, never force_set) and
//! self-verified — the recording doubles as a test:
//!   combo        -> open the dropdown, click an item (combo_pick_voice)
//!   combo_getter -> same, list supplied by a callback
//!   selectable   -> click rows; each toggles its own state (multi-select)
//!   list_box     -> click a row; caller-owned index single-selects it
//! combo / combo_getter are built as BeginCombo + a Selectable loop, so the open popup exposes
//! per-item sub_bboxes; combo_pick_voice targets item N deterministically and asserts the selection.

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    var b = snap?["globals"]?[ident]?["bbox"]
    let x = b?["x"] ?? 0.0f
    let y = b?["y"] ?? 0.0f
    let z = b?["z"] ?? 0.0f
    let w = b?["w"] ?? 0.0f
    return ((x + z) * 0.5f, (y + w) * 0.5f)
}

[export]
def main {
    with_recording_app("examples/tutorial/dropdown_select.das",
                       "dropdown_select.apng", 60) $(app) {
        let T_FRUIT = "DS_WIN/D_FRUIT"
        let T_FONT  = "DS_WIN/D_FONT"
        let T_TAG0  = "DS_WIN/D_TAG[0]"
        let T_TAG2  = "DS_WIN/D_TAG[2]"
        let T_ROW3  = "DS_WIN/D_SHIRT/D_SHIRT_ROW[3]"
        let T_SHIRT = "DS_WIN/D_SHIRT"

        var snap = wait_for_render(app, T_FRUIT, 10.0f)
        if (snap == null) {
            panic("{T_FRUIT} never rendered - wrong app running?")
        }

        // ---- Stage 1: combo (items array) — open the dropdown, click an item ----
        move_to(app, widget_center(snap, T_FRUIT), 600)
        hold_content(app, SETTLE_MS)
        let d1 = say_begin(app, "combo - open and pick", T_FRUIT,
            [voice = "A combo is a dropdown. Click it to open the list, then click an item to choose it. The index you picked is what your code reads back."])
        combo_pick_voice(app, d1, T_FRUIT, 2)              // Cherry

        // ---- Stage 2: combo_getter — same gesture, list from a callback ----
        move_to(app, widget_center(snap, T_FONT), 700)
        hold_content(app, SETTLE_MS)
        let d2 = say_begin(app, "combo_getter - list from a callback", T_FONT,
            [voice = "This combo builds its list from a callback instead of an array. It opens and picks the same way, which is handy when the items are computed or come from a large dataset."])
        combo_pick_voice(app, d2, T_FONT, 2)               // Mono

        // ---- Stage 3: selectable — N rows, each toggles its own state (multi-select) ----
        move_to(app, widget_center(snap, T_TAG0), 700)
        hold_content(app, SETTLE_MS)
        let d3 = say_begin(app, "selectable - multi-select rows", [T_TAG0, T_TAG2],
            [voice = "Selectable rows each keep their own on or off state, so they multi-select. Click a couple and each one toggles independently of the rest."])
        hold_through_voice(app, d3, [T_TAG0, T_TAG2])

        // ---- Stage 4: selectable_label in list_box — caller-owned index single-selects ----
        move_to(app, widget_center(snap, T_ROW3), 700)
        hold_content(app, SETTLE_MS)
        let d4 = say_begin(app, "list_box - single-select", T_ROW3,
            [voice = "A list box with a caller-owned index is single-select. Click a size and the index jumps to that row, leaving every other row switched off."])
        hold_through_voice(app, d4, [T_ROW3])
        record_check_value(app, T_SHIRT, "value", 3, 120)   // the clicked row drove the list_box index
    }
}
