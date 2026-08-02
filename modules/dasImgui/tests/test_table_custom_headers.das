options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `table_header` + `table_angled_headers_row`. Two
//! tables: CUSTOM_TABLE submits manual header cells via table_header();
//! ANGLED_TABLE submits the row via table_angled_headers_row() with
//! AngledHeader flag per column. Both are imperative (no [widget] state),
//! so we verify the surrounding data_table container kind + body cells.

let FEATURE = "modules/dasImgui/examples/features/table_custom_headers.das"

[test]
def test_table_custom_headers_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
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
