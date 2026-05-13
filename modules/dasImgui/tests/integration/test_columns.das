options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public
require math

//! §4.6 `columns(N, ${...}, ${...}, …)` integration smoke.
//! Verifies:
//!   1. All N column block bodies register their children under the window
//!      path (columns is a macro, not a container — no path leaf inserted).
//!   2. Column geometry: button bboxes are ordered left-to-right across columns.
//!   3. Statements emitted after the columns block still register normally.

let COLS_FEATURE = "modules/dasImgui/examples/features/columns_demo.das"

[test]
def test_columns_smoke(t : T?) {
    with_imgui_app(COLS_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "COLS_WIN/C0_BTN", 15.0f)
        t |> success(snap0 != null, "C0_BTN registers under COLS_WIN")
        if (snap0 == null) return

        t |> success(widget_exists(snap0, "COLS_WIN/C1_BTN"),
                     "C1_BTN registers in column 1")
        t |> success(widget_exists(snap0, "COLS_WIN/C2_BTN"),
                     "C2_BTN registers in column 2")
        t |> success(widget_exists(snap0, "COLS_WIN/AFTER_BTN"),
                     "AFTER_BTN registers after the columns_close()")
    }
}

[test]
def test_columns_geometry(t : T?) {
    with_imgui_app(COLS_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "COLS_WIN/C2_BTN", 15.0f)
        t |> success(snap0 != null, "snapshot reachable")
        if (snap0 == null) return

        // bbox is float4 = {x, y, z, w} = (x0, y0, x1, y1).
        let c0 = find_widget(snap0, "COLS_WIN/C0_BTN")?["bbox"]
        let c1 = find_widget(snap0, "COLS_WIN/C1_BTN")?["bbox"]
        let c2 = find_widget(snap0, "COLS_WIN/C2_BTN")?["bbox"]
        let c0_x = c0?["x"] ?? 0.0f
        let c1_x = c1?["x"] ?? 0.0f
        let c2_x = c2?["x"] ?? 0.0f
        t |> success(c0_x < c1_x,
                     "columns ordered L→R: C0.left ({c0_x}) < C1.left ({c1_x})")
        t |> success(c1_x < c2_x,
                     "columns ordered L→R: C1.left ({c1_x}) < C2.left ({c2_x})")

        // AFTER_BTN flows below the columns block (not inside any column).
        let after = find_widget(snap0, "COLS_WIN/AFTER_BTN")?["bbox"]
        let after_y = after?["y"] ?? 0.0f
        let c0_y1 = c0?["w"] ?? 0.0f
        t |> success(after_y > c0_y1,
                     "AFTER_BTN.top ({after_y}) > C0_BTN.bottom ({c0_y1})")
    }
}
