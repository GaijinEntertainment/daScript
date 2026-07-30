options gen2
options _comment_hygiene = true

require imgui
require imgui_app
require opengl/opengl_boost
require live/glfw_live
require live/live_api
require live/live_commands
require live/live_vars
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_table_builtin
require imgui/imgui_visual_aids

// =============================================================================
// TUTORIAL: data_table — boost container for ImGui's tables API.
//
// `BeginTable` / `EndTable` and the body-internal cursor calls
// (TableSetupColumn, TableHeadersRow, TableNextRow, TableSetColumnIndex,
// TableNextColumn) live behind one container + six snake_case pass-throughs
// in `imgui/imgui_table_builtin`. The container is named `data_table`
// (not `table`, which is a daslang reserved keyword for `table<K;V>`).
//
//   data_table(MY_TABLE, (text = "##split", columns = 3, flags = ...,
//                          outer_size = float2(0,0), inner_width = 0.0f)) {
//       table_setup_column("Name")
//       table_setup_column("Type")
//       table_setup_column("Value")
//       table_headers_row()
//       for (row in rows) {
//           table_next_row()
//           table_set_column_index(0); text(NAME[row], (text = ...))
//           table_set_column_index(1); text(TYPE[row], (text = ...))
//           table_set_column_index(2); text(VAL[row], (text = ...))
//       }
//   }
//
// `TableState` echoes per-call config (columns / flags / outer_size /
// inner_width). `sort_specs()` (block helper) yields the per-frame sort
// spec list — see the Sortable tables section below. Multi-select
// hand-off and custom row-bg callbacks would extend the state
// additively if added later.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/data_table.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/data_table.das
// =============================================================================

struct private Row {
    name : string
    kind : string
    value : string
}

var private ROWS : array<Row>

// Stable column identifiers passed to table_setup_column(.., user_id=...).
// The sort comparator dispatches on user_id (not column_index) so the
// sort stays correct when the user reorders columns via drag.
let private COL_NAME = 0x0001u
let private COL_KIND = 0x0002u
let private COL_VAL  = 0x0003u

[init]
def seed_rows() {
    ROWS <- [
        Row(name = "alpha", kind = "int", value = "42"),
        Row(name = "beta", kind = "float", value = "3.14"),
        Row(name = "gamma", kind = "string", value = "hello"),
        Row(name = "delta", kind = "bool", value = "true"),
        Row(name = "epsilon", kind = "int", value = "-7"),
        Row(name = "zeta", kind = "float", value = "0.001")
    ]
}

def private compare_rows(spec : TableSortSpec; a : Row; b : Row) : int {
    // Returns -1 / 0 / 1 for a-vs-b along the column the spec names.
    var ord = 0
    if (spec.column_user_id == COL_NAME) {
        if (a.name < b.name) {
            ord = -1
        } elif (a.name > b.name) {
            ord = 1
        }
    } elif (spec.column_user_id == COL_KIND) {
        if (a.kind < b.kind) {
            ord = -1
        } elif (a.kind > b.kind) {
            ord = 1
        }
    } elif (spec.column_user_id == COL_VAL) {
        if (a.value < b.value) {
            ord = -1
        } elif (a.value > b.value) {
            ord = 1
        }
    }
    if (spec.sort_direction == ImGuiSortDirection.Descending) {
        ord = -ord
    }
    return ord
}

def private sort_rows(specs : array<TableSortSpec>) {
    // Multi-key comparator: first spec that disambiguates a vs b wins; name is the total-order tiebreak.
    ROWS |> sort() $(a, b) {
        for (s in specs) {
            let ord = compare_rows(s, a, b)
            if (ord != 0) return ord < 0
        }
        return a.name < b.name
    }
}

var DT_NAME : table<int; NarrativeState>
var DT_KIND : table<int; NarrativeState>
var DT_VAL : table<int; NarrativeState>

[export]
def init() {
    live_create_window("dasImgui data_table tutorial", 860, 540)
    live_imgui_init(live_window)
    // Deterministic FirstUseEver layout for the recording: disable imgui.ini so a
    // prior session's window pos/size can't drift the framing. Tutorial-scoped on
    // purpose - not a blanket change to live_imgui_init.
    DisableIniPersistence()
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.4
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(30.0f, 30.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(640.0f, 380.0f), ImGuiCond.FirstUseEver)
    window(DT_WIN, (text = "data_table", closable = false,
                    flags = ImGuiWindowFlags.None)) {
        text(DT_HEADER, (text = "Three columns, six rows, frozen header. Click headers to sort; Shift+click for multi-column."))

        let tflags = (ImGuiTableFlags.BordersOuter |
                      ImGuiTableFlags.RowBg |
                      ImGuiTableFlags.Resizable |
                      ImGuiTableFlags.Reorderable |
                      ImGuiTableFlags.Hideable |
                      ImGuiTableFlags.Sortable |
                      ImGuiTableFlags.SortMulti |
                      ImGuiTableFlags.ScrollY)
        data_table(DT_TABLE, (text = "##rows", columns = 3,
                              flags = tflags,
                              outer_size = float2(0.0f, 0.0f),
                              inner_width = 0.0f)) {
            table_setup_scroll_freeze(0, 1)
            table_setup_column("Name",  ImGuiTableColumnFlags.DefaultSort, 0.0f, COL_NAME)
            table_setup_column("Type",  ImGuiTableColumnFlags.None,        0.0f, COL_KIND)
            table_setup_column("Value", ImGuiTableColumnFlags.None,        0.0f, COL_VAL)
            table_headers_row()

            sort_specs() $(specs) {
                sort_rows(specs)
            }

            for (i in range(length(ROWS))) {
                table_next_row()
                table_set_column_index(0)
                text(DT_NAME[i], (text = ROWS[i].name))
                table_set_column_index(1)
                text(DT_KIND[i], (text = ROWS[i].kind))
                table_set_column_index(2)
                text(DT_VAL[i], (text = ROWS[i].value))
            }
        }
    }

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    live_imgui_render()

    live_end_frame()
}

[export]
def shutdown() {
    live_imgui_shutdown()
    live_destroy_window()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
