options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! data_table: cells, headers, background tints, sort specs.

let DATA_TABLE_FEATURE = "modules/dasImgui/examples/features/data_table.das"

[test(description = "data_table registers its kind with all 24 cells at container-nested paths, cell text reads back as authored, and the columns order left to right.", timeout = 1200)]
def test_data_table_smoke(t : T?) {
    with_imgui_app(DATA_TABLE_FEATURE) $(d) {
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
                     "columns L->R: name({name0_x}) < qty({qty0_x}) < price({pri0_x})")
    }
}

let CUSTOM_HEADERS_FEATURE = "modules/dasImgui/examples/features/table_custom_headers.das"

[test(description = "Tables with manual table_header cells and with an angled headers row both register as data_table with every body cell and its authored text.", timeout = 1200)]
def test_table_custom_headers_smoke(t : T?) {
    with_imgui_app(CUSTOM_HEADERS_FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/CUSTOM_TABLE", 15.0f)
        t |> success(snap != null, "CUSTOM_TABLE registers")
        if (snap == null) return

        t |> equal(find_widget(snap, "MAIN_WIN/CUSTOM_TABLE")?["kind"] ?? "", "data_table",
                   "CUSTOM_TABLE.kind == data_table")
        t |> equal(find_widget(snap, "MAIN_WIN/ANGLED_TABLE")?["kind"] ?? "", "data_table",
                   "ANGLED_TABLE.kind == data_table")
        t |> success(widget_exists(snap, "MAIN_WIN/LEAD"), "LEAD registers")

        // 4 rows * 3 cells in CUSTOM_TABLE.
        for (i in range(4)) {
            for (col in ["CUSTOM_CELL_X", "CUSTOM_CELL_Y", "CUSTOM_CELL_Z"]) {
                t |> success(widget_exists(snap, "MAIN_WIN/CUSTOM_TABLE/{col}[{i}]"),
                             "MAIN_WIN/CUSTOM_TABLE/{col}[{i}] registers")
            }
        }

        // 4 rows * 4 cells in ANGLED_TABLE.
        for (i in range(4)) {
            for (col in ["ANGLED_CELL_A", "ANGLED_CELL_B", "ANGLED_CELL_C", "ANGLED_CELL_D"]) {
                t |> success(widget_exists(snap, "MAIN_WIN/ANGLED_TABLE/{col}[{i}]"),
                             "MAIN_WIN/ANGLED_TABLE/{col}[{i}] registers")
            }
        }

        // Cell content sanity (per-row label echoes).
        t |> equal(widget_payload_field(snap, "MAIN_WIN/CUSTOM_TABLE/CUSTOM_CELL_X[0]", "value") ?? "",
                   "x0", "CUSTOM[0,X].value == 'x0'")
        t |> equal(widget_payload_field(snap, "MAIN_WIN/ANGLED_TABLE/ANGLED_CELL_D[3]", "value") ?? "",
                   "d3", "ANGLED[3,D].value == 'd3'")
    }
}

let BG_COLOR_FEATURE = "modules/dasImgui/examples/features/table_set_bg_color.das"

[test(description = "Per-row and per-cell table_set_bg_color calls leave the table's 32 cells registered with their values, the tinted branches included.", timeout = 1200)]
def test_table_set_bg_color_smoke(t : T?) {
    with_imgui_app(BG_COLOR_FEATURE) $(d) {
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

let SORT_FEATURE = "modules/dasImgui/examples/features/sort_specs.das"

//! Gates on a cell: the body drains only on the second table pass
[test(description = "A sortable data_table with a sort_specs block renders its rows and the text after the table.", timeout = 1200)]
def test_sort_specs_table_renders(t : T?) {
    with_imgui_app(SORT_FEATURE) $(d) {
        // Wait for the DEEPEST widget so the snapshot lands AFTER the table's layout-then-render
        // two-pass. Waiting on SORT_WIN/LEAD races on CI's headless framebuffer: LEAD appears in
        // pass 1 but the table body only drains in pass 2 (widths computed then cells rendered).
        var snap = wait_for_widget(d, "SORT_WIN/SORT_TABLE/SORT_ROW_ID[0]", 15.0f)
        t |> success(snap != null,
                     "SORT_WIN renders with first inventory row's ID cell (sort_specs feature drained through table_next_row + table_set_column_index)")
        if (snap == null) return
        t |> success(widget_exists(snap, "SORT_WIN/SORT_TABLE"),
                     "SORT_WIN/SORT_TABLE registers (data_table container with Sortable + SortMulti)")
        t |> success(widget_exists(snap, "SORT_WIN/LEAD"),
                     "trailing text LEAD registers (sort_specs feature reached the post-table text)")
    }
}
