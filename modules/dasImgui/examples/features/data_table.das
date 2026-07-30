options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require imgui/imgui_table_builtin

// =============================================================================
// FEATURE: data_table — Phase 0b.3 table container + cursor primitives.
// SHOWS:   - `data_table(TBL, (columns=3, text="grid", flags=Resizable|Borders,
//            outer_size=(0,0), inner_width=0.0))` — BeginTable/EndTable
//            only-on-true paired container.
//          - `table_setup_column(label, flags?, init_width?, user_id?)` — one
//            call per column directly after BeginTable, before headers.
//          - `table_setup_scroll_freeze(cols, rows)` — pin the first column +
//            header row during scroll.
//          - `table_headers_row()` — submit header row using setup labels.
//          - `table_next_row(flags?, min_row_height?)` — append row.
//          - `table_next_column() : bool` — advance one column; returns true
//            when the cell is visible.
//          - `table_set_column_index(col) : bool` — jump to a specific column
//            index in the current row.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/data_table.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/data_table.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/data_table.das
// =============================================================================

var private GRID_ROW_NAME : table<int; NarrativeState>
var private GRID_ROW_QTY : table<int; NarrativeState>
var private GRID_ROW_PRICE : table<int; NarrativeState>

// Module-scope inventory rows — hoisted out of update() so these arrays
// allocate once at init instead of leaking a fresh allocation every frame
// (a local `var array<T>` in a hot path never finalizes on scope exit).
let private GRID_NAMES <- ["Widget", "Sprocket", "Gizmo", "Doohickey", "Thingamajig",
                           "Whatsit", "Doodad", "Gadget"]
let private GRID_QTY <- [12, 4, 7, 33, 1, 9, 22, 5]
let private GRID_PRICE <- [1.50, 9.99, 4.25, 0.75, 19.00, 3.10, 2.80, 6.45]

[export]
def init() {
    harness_init("data_table — boost v2 table rail", 640, 420)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(600.0, 380.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "data_table", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Three-column inventory grid; first column + headers are scroll-frozen."))

        data_table(INVENTORY, (text = "inventory", columns = 3,
                               flags = ImGuiTableFlags.Resizable |
                                       ImGuiTableFlags.Borders |
                                       ImGuiTableFlags.ScrollY |
                                       ImGuiTableFlags.RowBg,
                               outer_size = float2(0.0f, 240.0f),
                               inner_width = 0.0f)) {
            table_setup_column("Name", ImGuiTableColumnFlags.WidthStretch)
            table_setup_column("Qty",  ImGuiTableColumnFlags.WidthFixed, 60.0f)
            table_setup_column("$",    ImGuiTableColumnFlags.WidthFixed, 80.0f)
            table_setup_scroll_freeze(1, 1)
            table_headers_row()

            for (i in range(length(GRID_NAMES))) {
                table_next_row()
                if (table_set_column_index(0)) {
                    text(GRID_ROW_NAME[i], (text = "{GRID_NAMES[i]}"))
                }
                if (table_next_column()) {
                    text(GRID_ROW_QTY[i], (text = "{GRID_QTY[i]}"))
                }
                if (table_next_column()) {
                    text(GRID_ROW_PRICE[i], (text = "${GRID_PRICE[i]}"))
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
