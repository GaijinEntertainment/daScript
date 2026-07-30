options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require imgui/imgui_table_builtin

// =============================================================================
// FEATURE: table_header + table_angled_headers_row — custom header rendering.
// SHOWS:   Two tables side-by-side. CUSTOM_TABLE submits a manual header row
//          via `table_next_row(Headers)` + `table_set_column_index(i)` +
//          `table_header("X")` per column — full control over cell label and
//          alignment. ANGLED_TABLE sets `ImGuiTableColumnFlags.AngledHeader`
//          on each column and submits the row with one
//          `table_angled_headers_row()` call — labels render rotated to the
//          ImGuiStyle.TableAngledHeadersAngle (default 35°).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/table_custom_headers.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/table_custom_headers.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/table_custom_headers.das
// =============================================================================

var private CUSTOM_CELL_X : table<int; NarrativeState>
var private CUSTOM_CELL_Y : table<int; NarrativeState>
var private CUSTOM_CELL_Z : table<int; NarrativeState>
var private ANGLED_CELL_A : table<int; NarrativeState>
var private ANGLED_CELL_B : table<int; NarrativeState>
var private ANGLED_CELL_C : table<int; NarrativeState>
var private ANGLED_CELL_D : table<int; NarrativeState>

let private ROW_COUNT = 4

[export]
def init() {
    harness_init("table_header + table_angled_headers_row", 720, 480)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 440.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "custom + angled headers", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Top: manual table_header() per column. Bottom: table_angled_headers_row()."))

        // ---- Custom headers via table_header() ----
        data_table(CUSTOM_TABLE, (text = "custom", columns = 3,
                                  flags = ImGuiTableFlags.Borders |
                                          ImGuiTableFlags.RowBg,
                                  outer_size = float2(0.0f, 140.0f),
                                  inner_width = 0.0f)) {
            table_setup_column("XX")
            table_setup_column("YY")
            table_setup_column("ZZ")
            table_next_row(ImGuiTableRowFlags.Headers)
            if (table_set_column_index(0)) {
                table_header("X (custom)")
            }
            if (table_set_column_index(1)) {
                table_header("Y (custom)")
            }
            if (table_set_column_index(2)) {
                table_header("Z (custom)")
            }
            for (i in range(ROW_COUNT)) {
                table_next_row()
                if (table_set_column_index(0)) {
                    text(CUSTOM_CELL_X[i], (text = "x{i}"))
                }
                if (table_next_column()) {
                    text(CUSTOM_CELL_Y[i], (text = "y{i}"))
                }
                if (table_next_column()) {
                    text(CUSTOM_CELL_Z[i], (text = "z{i}"))
                }
            }
        }

        // ---- Angled headers via table_angled_headers_row() ----
        data_table(ANGLED_TABLE, (text = "angled", columns = 4,
                                  flags = ImGuiTableFlags.Borders |
                                          ImGuiTableFlags.RowBg,
                                  outer_size = float2(0.0f, 200.0f),
                                  inner_width = 0.0f)) {
            table_setup_column("Alpha",   ImGuiTableColumnFlags.AngledHeader |
                                          ImGuiTableColumnFlags.WidthFixed, 60.0f)
            table_setup_column("Bravo",   ImGuiTableColumnFlags.AngledHeader |
                                          ImGuiTableColumnFlags.WidthFixed, 60.0f)
            table_setup_column("Charlie", ImGuiTableColumnFlags.AngledHeader |
                                          ImGuiTableColumnFlags.WidthFixed, 60.0f)
            table_setup_column("Delta",   ImGuiTableColumnFlags.AngledHeader |
                                          ImGuiTableColumnFlags.WidthFixed, 60.0f)
            table_angled_headers_row()
            for (i in range(ROW_COUNT)) {
                table_next_row()
                if (table_set_column_index(0)) {
                    text(ANGLED_CELL_A[i], (text = "a{i}"))
                }
                if (table_next_column()) {
                    text(ANGLED_CELL_B[i], (text = "b{i}"))
                }
                if (table_next_column()) {
                    text(ANGLED_CELL_C[i], (text = "c{i}"))
                }
                if (table_next_column()) {
                    text(ANGLED_CELL_D[i], (text = "d{i}"))
                }
            }
        }
    }

    harness_end_frame()
}

[export]
def shutdown() {
    harness_shutdown()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
