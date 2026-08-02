options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `table_set_bg_color` — per-row RowBg0 + per-cell
//! CellBg tinting. The bg paint hits the table drawlist directly (not the
//! widget snapshot), so the test value is "no panic on the call chain
//! across 8 row-tint + 24 cell-tint invocations, widget tree intact".

let FEATURE = "modules/dasImgui/examples/features/table_set_bg_color.das"

[test]
def test_table_set_bg_color_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_widget(d, "BG_WIN/BG_TABLE", 15.0f)
        t |> success(snap != null, "BG_TABLE registers")
        if (snap == null) return

        let tbl = find_widget(snap, "BG_WIN/BG_TABLE")
        t |> equal(tbl?["kind"] ?? "", "data_table",
                   "BG_TABLE.kind == 'data_table'")

        // All 32 cell widgets register under the table container.
        for (i in range(8)) {
            t |> success(widget_exists(snap, "BG_WIN/BG_TABLE/BG_ROW_ID[{i}]"),
                         "BG_ROW_ID[{i}] registers")
            t |> success(widget_exists(snap, "BG_WIN/BG_TABLE/BG_ROW_NAME[{i}]"),
                         "BG_ROW_NAME[{i}] registers")
            t |> success(widget_exists(snap, "BG_WIN/BG_TABLE/BG_ROW_STATUS[{i}]"),
                         "BG_ROW_STATUS[{i}] registers")
            t |> success(widget_exists(snap, "BG_WIN/BG_TABLE/BG_ROW_VALUE[{i}]"),
                         "BG_ROW_VALUE[{i}] registers")
        }

        // Content sanity — confirms the per-row/per-cell loop didn't bail
        // mid-iteration on the bg-color side calls. server-03 is the crit
        // entry (row 2); server-06 has value 88 (row 5, triggers >80 red).
        t |> equal(widget_payload_field(snap, "BG_WIN/BG_TABLE/BG_ROW_STATUS[2]", "value") ?? "",
                   "crit",
                   "BG_ROW_STATUS[2].value == 'crit' (RowBg0 deep-red branch)")
        t |> equal(widget_payload_field(snap, "BG_WIN/BG_TABLE/BG_ROW_VALUE[5]", "value") ?? "",
                   "88",
                   "BG_ROW_VALUE[5].value == '88' (CellBg deep-red branch)")
    }
}
