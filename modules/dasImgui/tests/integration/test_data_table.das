options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `data_table` — Phase 0b.3 table container + cell
//! cursor primitives (table_setup_column / scroll_freeze / headers_row /
//! next_row / next_column / set_column_index). Drives a 3×8 inventory grid.
//! Verifies: table itself registers as data_table kind; all 24 cell widgets
//! register at container-nested paths (INVENTORY/CELL[i]); cell text reads
//! back as authored; columns ordered left-to-right by bbox.x.

let FEATURE = "modules/dasImgui/examples/features/data_table.das"

[test]
def test_data_table_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/INVENTORY", 15.0f)
        t |> success(snap != null, "INVENTORY table registers")
        if (snap == null) return

        let tbl = find_widget(snap, "MAIN_WIN/INVENTORY")
        t |> equal(tbl?["kind"] ?? "", "data_table",
                   "INVENTORY.kind == 'data_table'")

        // All 24 cell widgets register at container-nested paths.
        for (i in range(8)) {
            t |> success(widget_exists(snap, "MAIN_WIN/INVENTORY/GRID_ROW_NAME[{i}]"),
                         "GRID_ROW_NAME[{i}] registers under INVENTORY")
            t |> success(widget_exists(snap, "MAIN_WIN/INVENTORY/GRID_ROW_QTY[{i}]"),
                         "GRID_ROW_QTY[{i}] registers under INVENTORY")
            t |> success(widget_exists(snap, "MAIN_WIN/INVENTORY/GRID_ROW_PRICE[{i}]"),
                         "GRID_ROW_PRICE[{i}] registers under INVENTORY")
        }

        // Content sanity — row 0 reads back as authored. text widget echoes
        // its call-site string into payload.value (NarrativeState).
        t |> equal(widget_payload_field(snap, "MAIN_WIN/INVENTORY/GRID_ROW_NAME[0]", "value") ?? "",
                   "Widget",
                   "GRID_ROW_NAME[0].value == 'Widget'")
        t |> equal(widget_payload_field(snap, "MAIN_WIN/INVENTORY/GRID_ROW_QTY[0]", "value") ?? "",
                   "12",
                   "GRID_ROW_QTY[0].value == '12'")

        // Column geometry — left-to-right ordering via bbox.x at row 0.
        let name0_x = find_widget(snap, "MAIN_WIN/INVENTORY/GRID_ROW_NAME[0]")?["bbox"]?["x"] ?? 0.0f
        let qty0_x  = find_widget(snap, "MAIN_WIN/INVENTORY/GRID_ROW_QTY[0]")?["bbox"]?["x"] ?? 0.0f
        let pri0_x  = find_widget(snap, "MAIN_WIN/INVENTORY/GRID_ROW_PRICE[0]")?["bbox"]?["x"] ?? 0.0f
        t |> success(name0_x < qty0_x && qty0_x < pri0_x,
                     "columns L→R: name({name0_x}) < qty({qty0_x}) < price({pri0_x})")
    }
}
