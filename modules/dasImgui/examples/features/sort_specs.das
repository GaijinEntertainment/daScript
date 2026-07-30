options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require imgui/imgui_table_builtin
require strings

// =============================================================================
// FEATURE: sort_specs — sortable inventory table.
// SHOWS:   - `data_table(SORT_TABLE, (flags = ... | Sortable | SortMulti, ...))`
//            — table created with sort-capable flags.
//          - `table_setup_column(label, flags, _, user_id)` — `user_id` argument
//            tags each column with a stable identifier the sort callback uses to
//            recognize columns independent of the runtime column order
//            (Reorderable + Hideable).
//          - `sort_specs() <| $(specs) { sort_items(specs) }` — fires only when
//            ImGui reports SpecsDirty; the wrapper auto-clears the dirty flag.
//            Supports multi-column sort: hold Shift while clicking a second
//            header.
//          - Each `TableSortSpec` carries column_index / column_user_id /
//            sort_order / sort_direction so the comparator can drive both
//            primary and tiebreak ordering.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/sort_specs.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/sort_specs.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/sort_specs.das
// =============================================================================

struct private InvItem {
    id   : int
    name : string
    qty  : int
}

// Stable column identifiers passed to table_setup_column(.., user_id=...). Using
// these instead of column_index in the comparator keeps the sort robust when the
// user reorders columns via drag.
let private COL_ID   = 0x0001u
let private COL_NAME = 0x0002u
let private COL_QTY  = 0x0003u

var private SORT_ITEMS : array<InvItem>
var private SORT_ROW_ID   : table<int; NarrativeState>
var private SORT_ROW_NAME : table<int; NarrativeState>
var private SORT_ROW_QTY  : table<int; NarrativeState>

let private SEED_NAMES <- ["Widget", "Sprocket", "Gizmo", "Doohickey",
                           "Thingamajig", "Whatsit", "Doodad", "Gadget",
                           "Contraption", "Bauble", "Trinket", "Whatchamacallit"]

[init]
def seed_items() {
    SORT_ITEMS |> reserve(length(SEED_NAMES))
    for (i in range(length(SEED_NAMES))) {
        SORT_ITEMS |> push(InvItem(
            id = i + 100,
            name = SEED_NAMES[i],
            qty = (i * i - i) % 20))
    }
}

def compare_field(spec : TableSortSpec; a : InvItem; b : InvItem) : int {
    // Returns -1 / 0 / 1 for a-vs-b along the column the spec names.
    var ord = 0
    if (spec.column_user_id == COL_ID) {
        ord = a.id - b.id
    } elif (spec.column_user_id == COL_NAME) {
        if (a.name < b.name) {
            ord = -1
        } elif (a.name > b.name) {
            ord = 1
        }
    } elif (spec.column_user_id == COL_QTY) {
        ord = a.qty - b.qty
    }
    if (spec.sort_direction == ImGuiSortDirection.Descending) {
        ord = -ord
    }
    return ord
}

def sort_items_with_specs(specs : array<TableSortSpec>) {
    // Multi-key comparator: walk specs in order, return on the first spec that
    // disambiguates a vs b. Final tiebreak by id so the order is total.
    SORT_ITEMS |> sort() $(a, b) {
        for (s in specs) {
            let ord = compare_field(s, a, b)
            if (ord != 0) return ord < 0
        }
        return a.id < b.id
    }
}

[export]
def init() {
    harness_init("sort_specs — sortable inventory table", 640, 460)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(600.0f, 420.0f), ImGuiCond.Always)
    window(SORT_WIN, (text = "sort_specs", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Click headers to sort. Hold Shift + click for multi-column sort."))

        data_table(SORT_TABLE, (text = "inventory_sort", columns = 3,
                                flags = ImGuiTableFlags.Resizable |
                                        ImGuiTableFlags.Reorderable |
                                        ImGuiTableFlags.Hideable |
                                        ImGuiTableFlags.Sortable |
                                        ImGuiTableFlags.SortMulti |
                                        ImGuiTableFlags.RowBg |
                                        ImGuiTableFlags.BordersOuter |
                                        ImGuiTableFlags.BordersV |
                                        ImGuiTableFlags.ScrollY,
                                outer_size = float2(0.0f, 280.0f),
                                inner_width = 0.0f)) {
            table_setup_column("ID",   ImGuiTableColumnFlags.DefaultSort |
                                       ImGuiTableColumnFlags.WidthFixed,
                               0.0f, COL_ID)
            table_setup_column("Name", ImGuiTableColumnFlags.WidthStretch,
                               0.0f, COL_NAME)
            table_setup_column("Qty",  ImGuiTableColumnFlags.PreferSortDescending |
                                       ImGuiTableColumnFlags.WidthFixed,
                               80.0f, COL_QTY)
            table_setup_scroll_freeze(0, 1)
            table_headers_row()

            sort_specs() $(specs) {
                sort_items_with_specs(specs)
            }

            for (i in range(length(SORT_ITEMS))) {
                table_next_row()
                if (table_set_column_index(0)) {
                    text(SORT_ROW_ID[i],   (text = "{SORT_ITEMS[i].id}"))
                }
                if (table_next_column()) {
                    text(SORT_ROW_NAME[i], (text = "{SORT_ITEMS[i].name}"))
                }
                if (table_next_column()) {
                    text(SORT_ROW_QTY[i],  (text = "{SORT_ITEMS[i].qty}"))
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
