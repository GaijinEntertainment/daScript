options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require imgui/imgui_table_builtin

// =============================================================================
// FEATURE: table_set_bg_color — per-row and per-cell background tinting.
// SHOWS:   - `table_set_bg_color(ImGuiTableBgTarget.RowBg0, color)` after
//            `table_next_row()` — paints the entire row's first bg layer.
//            Useful for status/severity stripes that read at a glance.
//          - `table_set_bg_color(ImGuiTableBgTarget.CellBg, color)` after
//            `table_set_column_index(...)` / `table_next_column()` —
//            paints just the current cell. Stacks above RowBg0/RowBg1.
//          - Color packing via `rgba(r, g, b, a)` direct — no Style.Alpha
//            multiplier (use `GetColorU32(float4(...))` if you want one).
//          - `RowBg` table flag is independent of `table_set_bg_color` —
//            the flag gives default alternating zebra; the override
//            paints on top.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/table_set_bg_color.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/table_set_bg_color.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/table_set_bg_color.das
// =============================================================================

struct private StatusRow {
    id     : int
    name   : string
    status : string
    value  : int
}

var private BG_ROWS : array<StatusRow>

var private BG_ROW_ID     : table<int; NarrativeState>
var private BG_ROW_NAME   : table<int; NarrativeState>
var private BG_ROW_STATUS : table<int; NarrativeState>
var private BG_ROW_VALUE  : table<int; NarrativeState>

[init]
def seed_bg_rows() {
    BG_ROWS <- [
        StatusRow(id = 0, name = "server-01", status = "ok",   value = 12),
        StatusRow(id = 1, name = "server-02", status = "warn", value = 67),
        StatusRow(id = 2, name = "server-03", status = "crit", value = 94),
        StatusRow(id = 3, name = "server-04", status = "ok",   value = 34),
        StatusRow(id = 4, name = "server-05", status = "warn", value = 58),
        StatusRow(id = 5, name = "server-06", status = "crit", value = 88),
        StatusRow(id = 6, name = "server-07", status = "ok",   value = 21),
        StatusRow(id = 7, name = "server-08", status = "ok",   value = 45)
    ]
}

// Returns 0u for "ok" (no tint), warn = amber, crit = muted red.
// Low alpha keeps text legible over the tint.
def private status_row_color(s : string) : uint {
    if (s == "warn") return rgba(220u, 170u, 50u, 70u)
    if (s == "crit") return rgba(220u, 60u, 60u, 90u)
    return 0u
}

[export]
def init() {
    harness_init("table_set_bg_color — row & cell background tints", 560, 380)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(520.0f, 340.0f), ImGuiCond.Always)
    window(BG_WIN, (text = "table_set_bg_color", closable = false,
                    flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "RowBg0 = full-row tint by status. CellBg = overlay on Value cells over threshold."))

        data_table(BG_TABLE, (text = "status_table", columns = 4,
                              flags = ImGuiTableFlags.RowBg |
                                      ImGuiTableFlags.BordersOuter |
                                      ImGuiTableFlags.BordersV |
                                      ImGuiTableFlags.ScrollY,
                              outer_size = float2(0.0f, 240.0f),
                              inner_width = 0.0f)) {
            table_setup_column("ID",     ImGuiTableColumnFlags.WidthFixed,   40.0f)
            table_setup_column("Name",   ImGuiTableColumnFlags.WidthStretch)
            table_setup_column("Status", ImGuiTableColumnFlags.WidthFixed,   80.0f)
            table_setup_column("Value",  ImGuiTableColumnFlags.WidthFixed,   80.0f)
            table_setup_scroll_freeze(0, 1)
            table_headers_row()

            for (i in range(length(BG_ROWS))) {
                let r = BG_ROWS[i]
                table_next_row()

                // Row-level tint: full-row RowBg0 by status severity.
                let row_color = status_row_color(r.status)
                if (row_color != 0u) {
                    table_set_bg_color(ImGuiTableBgTarget.RowBg0, row_color)
                }

                if (table_set_column_index(0)) {
                    text(BG_ROW_ID[i], (text = "{r.id}"))
                }
                if (table_next_column()) {
                    text(BG_ROW_NAME[i], (text = r.name))
                }
                if (table_next_column()) {
                    text(BG_ROW_STATUS[i], (text = r.status))
                }
                if (table_next_column()) {
                    text(BG_ROW_VALUE[i], (text = "{r.value}"))
                    // Cell-level overlay: deep red for >80, amber for >50.
                    if (r.value > 80) {
                        table_set_bg_color(ImGuiTableBgTarget.CellBg, rgba(220u, 50u, 50u, 110u))
                    } elif (r.value > 50) {
                        table_set_bg_color(ImGuiTableBgTarget.CellBg, rgba(220u, 140u, 50u, 110u))
                    }
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
