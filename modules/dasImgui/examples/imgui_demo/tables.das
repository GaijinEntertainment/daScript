options gen2
options _comment_hygiene = true
options indenting = 4

module tables

require imgui
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_layout_builtin
require imgui/imgui_id_builtin
require imgui/imgui_scope_builtin
require imgui/imgui_style_builtin
require imgui/imgui_table_builtin
require daslib/safe_addr
require math

// =============================================================================
// Mirrors imgui_demo.cpp:4154-6044 — static void ShowDemoWindowTables().
//
// Port status: 24 / 24 sub-sections ported. ~2911 das lines across 8
// commits (PR-D/2 chunks 1-8, see dasImgui PR #52). Two cpp constructs
// deliberately partial:
//   - Section 21 Context menus sub 2 per-column manual-open popups
//     (cpp:5560-5582 raw `OpenPopup`/`BeginPopup` keyed on a shared
//     "MyPopup" str_id): boost surface has no stateless `popup_window`
//     wrapper for this pattern. Fallback: default per-column context
//     menu (auto-emitted by `table_headers_row`) + hovered-column
//     readout. See `show_context_menus()` body comment + backlog memory
//     `project_dasimgui_popup_window_backlog.md`.
//   - Section 24 Advanced Debug-details readout (cpp:6019-6031): drops
//     the `Debug details` checkbox + `ImDrawList.CmdBuffer.Size`
//     readout because `ImVector\`ImDrawCmd` has no `Size` accessor in
//     the daslang binding. See `show_advanced()` body comment.
//
// Boost-surface conventions:
// - cpp `static` locals → module-scope `var private` (state-hoist).
// - cpp `PushID("Tables")` outer scope dropped — each `data_table(IDENT,
//   ...)` already auto-PushID's its unique IDENT, so cross-section
//   collisions can't happen.
// - cpp Expand all / Collapse all driver → `g_open_action` gate with
//   `apply_open_action()` called before each `tree_node(...)`. Reset to
//   -1 at the end of the dispatcher.
// - cpp `HelpMarker(text)` bubbles → dropped per the app_dockspace
//   precedent (extra widget IDENTs + tooltip plumbing for context that's
//   either obvious from the section title or redundant with our own
//   docstring). May be reintroduced as a local helper if later sections
//   carry load-bearing help text.
// - Per-section helpers: one `def private show_section_<n>()` per
//   CollapsingHeader sub-section so the dispatcher reads as a 24-line
//   sequence.
// =============================================================================

// Shared Expand all / Collapse all driver.
var private g_open_action : int = -1

def private apply_open_action() {
    if (g_open_action != -1) {
        SetNextItemOpen(g_open_action != 0, ImGuiCond.Always)
    }
}

// ---- Section 1 Basic state ----
// All three methods iterate procedurally; per-cell text needs its own NarrativeState
// per call site, so indexed table state keeps each cell distinct in the registry.
var private TABLES_BASIC_T1_CELL : table<int; NarrativeState>
var private TABLES_BASIC_T2_CELL : table<int; NarrativeState>
var private TABLES_BASIC_T3_CELL : table<int; NarrativeState>

// ---- Section 2 Borders, background state ----
var private TABLES_BORDERS_FLAGS = ImGuiTableFlags.Borders | ImGuiTableFlags.RowBg
var private TABLES_BORDERS_CONTENTS : RadioIntState
var private TABLES_BORDERS_T1_CELL_TEXT : table<int; NarrativeState>
var private TABLES_BORDERS_T1_CELL_BTN  : table<int; ClickState>

// ---- Section 3 Resizable, stretch state ----
var private TABLES_RSTRETCH_FLAGS = (ImGuiTableFlags.SizingStretchSame |
                                     ImGuiTableFlags.Resizable |
                                     ImGuiTableFlags.BordersOuter |
                                     ImGuiTableFlags.BordersV |
                                     ImGuiTableFlags.ContextMenuInBody)
var private TABLES_RSTRETCH_CELL : table<int; NarrativeState>

// ---- Section 4 Resizable, fixed state ----
var private TABLES_RFIXED_FLAGS = (ImGuiTableFlags.SizingFixedFit |
                                   ImGuiTableFlags.Resizable |
                                   ImGuiTableFlags.BordersOuter |
                                   ImGuiTableFlags.BordersV |
                                   ImGuiTableFlags.ContextMenuInBody)
var private TABLES_RFIXED_CELL : table<int; NarrativeState>

// ---- Section 5 Resizable, mixed state ----
// Same flag set drives both sub-tables (3-col and 6-col). Per-column
// WidthFixed/WidthStretch + DefaultHide configured at TableSetupColumn time.
var private TABLES_RMIXED_FLAGS = (ImGuiTableFlags.SizingFixedFit |
                                   ImGuiTableFlags.RowBg |
                                   ImGuiTableFlags.Borders |
                                   ImGuiTableFlags.Resizable |
                                   ImGuiTableFlags.Reorderable |
                                   ImGuiTableFlags.Hideable)
var private TABLES_RMIXED_T1_CELL : table<int; NarrativeState>
var private TABLES_RMIXED_T2_CELL : table<int; NarrativeState>

// ---- Section 6 Reorderable, hideable, with headers state ----
var private TABLES_RHH_FLAGS = (ImGuiTableFlags.Resizable |
                                ImGuiTableFlags.Reorderable |
                                ImGuiTableFlags.Hideable |
                                ImGuiTableFlags.BordersOuter |
                                ImGuiTableFlags.BordersV)
var private TABLES_RHH_T1_CELL : table<int; NarrativeState>
var private TABLES_RHH_T2_CELL : table<int; NarrativeState>

// ---- Section 7 Padding state ----
// Two sub-tables: table_padding (flag toggles + Avail/FillButton cells)
// and table_padding_2 (CellPadding style-var + InputText grid).
var private TABLES_PAD1_FLAGS = ImGuiTableFlags.BordersV
var private TABLES_PAD1_AVAIL : table<int; NarrativeState>  // row 0 cells: "Avail %.2f"
var private TABLES_PAD1_BTN   : table<int; ClickState>      // rows 1-4 cells: FillButton "Hello c,r"

var private TABLES_PAD2_FLAGS = ImGuiTableFlags.Borders | ImGuiTableFlags.RowBg
var private TABLES_PAD2_CELL_PADDING : float2 = float2(0.0f, 0.0f)
// 15 InputText cells; the boost (text=..., init=...) named-tuple form
// seeds the buffer to "edit me" on first call (matches cpp's static init).
var private TABLES_PAD2_CELL_IT      : table<int; InputTextState>

// ---- Section 8 Sizing policies state ----
// Quad of mini-tables, each with its own SizingFixed*/SizingStretch* policy.
// cpp EditTableSizingFlags is ported as `edit_table_sizing_flags` below.
var private TABLES_SIZING_FLAGS1 = ImGuiTableFlags.Resizable | ImGuiTableFlags.NoHostExtendX
// Policy-per-row state: 4 outer iterations, each owns one flag word
// (initialized via [init] to the 4 cpp defaults) plus two 3-col x 3-row
// inner tables of NarrativeState cells.
var private TABLES_SIZING_POLICY      : table<int; ImGuiTableFlags>
var private TABLES_SIZING_T1          : table<int; TableState>      // 4 data_tables
var private TABLES_SIZING_T2          : table<int; TableState>      // 4 data_tables
var private TABLES_SIZING_T1_CELL     : table<int; NarrativeState>  // key = table_n*9 + row*3 + col
var private TABLES_SIZING_T2_CELL     : table<int; NarrativeState>

// Advanced sub-block: flag toggles + dynamic-column body. CT_ enum is
// mirrored as ints to match the cpp `static int contents_type`.
let private TABLES_SIZING_CT_SHOW_WIDTH    = 0
let private TABLES_SIZING_CT_SHORT_TEXT    = 1
let private TABLES_SIZING_CT_LONG_TEXT     = 2
let private TABLES_SIZING_CT_BUTTON        = 3
let private TABLES_SIZING_CT_FILL_BUTTON   = 4
let private TABLES_SIZING_CT_INPUT_TEXT    = 5

var private TABLES_SIZING_ADV_FLAGS = (ImGuiTableFlags.ScrollY |
                                       ImGuiTableFlags.Borders |
                                       ImGuiTableFlags.RowBg |
                                       ImGuiTableFlags.Resizable)
var private TABLES_SIZING_ADV_CT       : int = 0
var private TABLES_SIZING_ADV_COLUMNS  : int = 3
// Per-CT-type cell maps so widget registry paths stay disjoint when the
// user flips contents_type at runtime (avoids state-shape mismatch).
var private TABLES_SIZING_ADV_TEXT    : table<int; NarrativeState>
var private TABLES_SIZING_ADV_BTN     : table<int; ClickState>
var private TABLES_SIZING_ADV_IT      : table<int; InputTextState>

// ---- Section 9 Vertical scrolling state ----
var private TABLES_VSCROLL_FLAGS = (ImGuiTableFlags.ScrollY |
                                    ImGuiTableFlags.RowBg |
                                    ImGuiTableFlags.BordersOuter |
                                    ImGuiTableFlags.BordersV |
                                    ImGuiTableFlags.Resizable |
                                    ImGuiTableFlags.Reorderable |
                                    ImGuiTableFlags.Hideable)
var private TABLES_VSCROLL_CELL : table<int; NarrativeState>  // key = row*3 + col, 1000 rows

// ---- Section 10 Horizontal scrolling state ----
var private TABLES_HSCROLL_FLAGS = (ImGuiTableFlags.ScrollX |
                                    ImGuiTableFlags.ScrollY |
                                    ImGuiTableFlags.RowBg |
                                    ImGuiTableFlags.BordersOuter |
                                    ImGuiTableFlags.BordersV |
                                    ImGuiTableFlags.Resizable |
                                    ImGuiTableFlags.Reorderable |
                                    ImGuiTableFlags.Hideable)
var private TABLES_HSCROLL_FREEZE_COLS : int = 1
var private TABLES_HSCROLL_FREEZE_ROWS : int = 1
var private TABLES_HSCROLL_CELL : table<int; NarrativeState>  // 20 rows x 7 cols

// Second sub-table (Stretch + ScrollX).
var private TABLES_HSCROLL2_FLAGS = (ImGuiTableFlags.SizingStretchSame |
                                     ImGuiTableFlags.ScrollX |
                                     ImGuiTableFlags.ScrollY |
                                     ImGuiTableFlags.BordersOuter |
                                     ImGuiTableFlags.RowBg |
                                     ImGuiTableFlags.ContextMenuInBody)
var private TABLES_HSCROLL2_INNER_WIDTH : float = 1000.0f
var private TABLES_HSCROLL2_CELL : table<int; NarrativeState>  // 20*7 cells

// ---- Section 11 Columns flags state ----
// Per-column input flags (DefaultSort / None / DefaultHide).
var private TABLES_COLFLAGS_INPUT     : table<int; ImGuiTableColumnFlags>  // 3 columns
// Output flags read from TableGetColumnFlags per frame; displayed in the
// first sub-table via a disabled set of CheckboxFlags.
var private TABLES_COLFLAGS_OUTPUT    : table<int; ImGuiTableColumnFlags>  // 3 columns
var private TABLES_COLFLAGS_BODY_CELL : table<int; NarrativeState>  // 8*3 cells in body table

// Per-column widget state for the setup-table's label cells: 1 label per
// column → 3 each. Indexed so widget paths stay per-column-distinct.
var private TABLES_COLFLAGS_ATFP          : table<int; EmptyMarkerState>
var private TABLES_COLFLAGS_LABEL         : table<int; NarrativeState>
var private TABLES_COLFLAGS_INFLAG_LABEL  : table<int; NarrativeState>
var private TABLES_COLFLAGS_OUTFLAG_LABEL : table<int; NarrativeState>

// ---- Section 12 Columns widths state ----
// Sub-table 1: default-width via TableSetupColumn (cpp:4938-4967).
var private TABLES_CW1_FLAGS = ImGuiTableFlags.Borders | ImGuiTableFlags.NoBordersInBodyUntilResize
var private TABLES_CW1_CELL : table<int; NarrativeState>  // 4*3 cells
// Sub-table 2: explicit-width via TableSetupColumn (cpp:4969-5000).
var private TABLES_CW2_FLAGS = ImGuiTableFlags.None
var private TABLES_CW2_CELL : table<int; NarrativeState>  // 5*4 cells

// ---- Section 13 Nested tables state ----
// One outer 2-col table holding a nested 2-col table in its first cell.
var private TABLES_NESTED_OUTER_CELL : table<int; NarrativeState>  // 4 cells
var private TABLES_NESTED_INNER_CELL : table<int; NarrativeState>  // 4 cells

// ---- Section 14 Row height state ----
// Sub 1: variable min_row_height per row.
var private TABLES_RH1_CELL : table<int; NarrativeState>  // 8 cells
// Sub 2: SameLine(0,0) line-height share — two rows, ColorButton + 2-line text.
var private TABLES_RH2_COLOR_BTN : table<int; ClickState>     // 2 cells
var private TABLES_RH2_LINE1     : table<int; NarrativeState> // 2 cells
var private TABLES_RH2_LINE2     : table<int; NarrativeState> // 2 cells
// Sub 3: per-row CellPadding.y mutation (every 3rd row).
var private TABLES_RH3_CELL : table<int; NarrativeState>  // 8 cells

// ---- Section 15 Outer size state ----
// Sub 1: NoHostExtendX/Y toggles + sliding "Hello!" SameLine target.
var private TABLES_OS1_FLAGS = (ImGuiTableFlags.Borders |
                                ImGuiTableFlags.Resizable |
                                ImGuiTableFlags.ContextMenuInBody |
                                ImGuiTableFlags.RowBg |
                                ImGuiTableFlags.SizingFixedFit |
                                ImGuiTableFlags.NoHostExtendX)
var private TABLES_OS1_CELL : table<int; NarrativeState>  // 10*3 cells
// Sub 2 + 3: two explicit-size tables side-by-side.
var private TABLES_OS2_CELL : table<int; NarrativeState>  // 5*3 cells
var private TABLES_OS3_CELL : table<int; NarrativeState>  // 3*3 cells

// ---- Section 16 Background color state ----
// 3 mode selectors + 5*6 cell text. Each selector is an int-backed Combo.
var private TABLES_BG_FLAGS = ImGuiTableFlags.RowBg
var private TABLES_BG_ROW_BG_TYPE   : int = 1
var private TABLES_BG_ROW_BG_TARGET : int = 1
var private TABLES_BG_CELL_BG_TYPE  : int = 1
var private TABLES_BG_CELL : table<int; NarrativeState>  // 5*6 cells, "row+col" charpairs

// ---- Section 17 Tree view state ----
var private TABLES_TREE_FLAGS = (ImGuiTableFlags.BordersV |
                                 ImGuiTableFlags.BordersOuterH |
                                 ImGuiTableFlags.Resizable |
                                 ImGuiTableFlags.RowBg |
                                 ImGuiTableFlags.NoBordersInBody)
var private TABLES_TREE_NODE_FLAGS = ImGuiTreeNodeFlags.SpanAllColumns
// Per-node widget state — there are 9 cpp nodes (idx 0..8). Each node has
// 1 TreeNode + 2 trailing-column texts. Keyed by node index.
var private TABLES_TREE_NODE       : table<int; TreeNodeState>
var private TABLES_TREE_NODE_LEAF  : table<int; TreeNodeState>
var private TABLES_TREE_SIZE_CELL  : table<int; NarrativeState>
var private TABLES_TREE_TYPE_CELL  : table<int; NarrativeState>
// File-system mirror — same shape as the cpp `MyTreeNode` array.
struct private TreeViewNode {
    name        : string
    node_type   : string
    size        : int
    child_idx   : int
    child_count : int
}
let private TREE_VIEW_NODES <- [
    TreeViewNode(name = "Root",
                 node_type = "Folder", size = -1, child_idx = 1, child_count = 3),
    TreeViewNode(name = "Music",
                 node_type = "Folder", size = -1, child_idx = 4, child_count = 2),
    TreeViewNode(name = "Textures",
                 node_type = "Folder", size = -1, child_idx = 6, child_count = 3),
    TreeViewNode(name = "desktop.ini",
                 node_type = "System file", size = 1024, child_idx = -1, child_count = -1),
    TreeViewNode(name = "File1_a.wav",
                 node_type = "Audio file", size = 123000, child_idx = -1, child_count = -1),
    TreeViewNode(name = "File1_b.wav",
                 node_type = "Audio file", size = 456000, child_idx = -1, child_count = -1),
    TreeViewNode(name = "Image001.png",
                 node_type = "Image file", size = 203128, child_idx = -1, child_count = -1),
    TreeViewNode(name = "Copy of Image001.png",
                 node_type = "Image file", size = 203256, child_idx = -1, child_count = -1),
    TreeViewNode(name = "Copy of Image001 (Final2).png",
                 node_type = "Image file", size = 203512, child_idx = -1, child_count = -1)]

// ---- Section 18 Item width state ----
// 3-col, 3-row, 9 indexed SliderFloats. cpp shares one `dummy_f` across all 9;
// boost slider state owns per-cell values — benign divergence, same ItemWidth demo.
var private TABLES_ITEM_WIDTH_SLIDER : table<int; SliderStateFloat>  // 3*3 sliders

[init]
def seed_item_width_sliders() {
    // All 9 sliders share the same [0.0, 1.0] bounds.
    for (i in range(9)) {
        TABLES_ITEM_WIDTH_SLIDER[i].bounds = (0.0f, 1.0f)
    }
}

// ---- Section 19 Custom headers state ----
// 3 per-column header checkboxes + 3*5 selectable cells. `selectable_label` is the
// caller-owned-bool form; selected-appearance comes from TABLES_CHDR_SELECTED[column].
var private TABLES_CHDR_SELECTED : table<int; bool>          // 3 entries
var private TABLES_CHDR_CELL     : table<int; ClickState>    // 5*3 cells

// ---- Section 20 Angled headers state ----
let private TABLES_AH_COLUMN_NAMES <- ["Track", "cabasa", "ride", "smash",
                                       "tom-hi", "tom-mid", "tom-low",
                                       "hihat-o", "hihat-c", "snare-s",
                                       "snare-c", "clap", "rim", "kick"]
let private TABLES_AH_COLUMNS_COUNT = 14
let private TABLES_AH_ROWS_COUNT    = 12
var private TABLES_AH_TABLE_FLAGS = (ImGuiTableFlags.SizingFixedFit |
                                     ImGuiTableFlags.ScrollX |
                                     ImGuiTableFlags.ScrollY |
                                     ImGuiTableFlags.BordersOuter |
                                     ImGuiTableFlags.BordersInnerH |
                                     ImGuiTableFlags.Hideable |
                                     ImGuiTableFlags.Resizable |
                                     ImGuiTableFlags.Reorderable |
                                     ImGuiTableFlags.HighlightHoveredColumn)
var private TABLES_AH_COLUMN_FLAGS = (ImGuiTableColumnFlags.AngledHeader |
                                      ImGuiTableColumnFlags.WidthFixed)
var private TABLES_AH_FROZEN_COLS  : int = 1
var private TABLES_AH_FROZEN_ROWS  : int = 2
var private TABLES_AH_BOOLS  : table<int; bool>             // 14*12 cells
var private TABLES_AH_CB     : table<int; EmptyMarkerState> // 14*12 checkbox state
var private TABLES_AH_TRACK_LABEL : table<int; NarrativeState> // 12 row labels
var private TABLES_AH_TRACK_ATFP  : table<int; EmptyMarkerState> // 12 align markers

// ---- Section 21 Context menus state ----
// Sub 1: flag toggle + 3*4 cells (default context menus only).
var private TABLES_CM1_FLAGS = (ImGuiTableFlags.Resizable |
                                ImGuiTableFlags.Reorderable |
                                ImGuiTableFlags.Hideable |
                                ImGuiTableFlags.Borders |
                                ImGuiTableFlags.ContextMenuInBody)
var private TABLES_CM1_CELL : table<int; NarrativeState>  // 4*3 cells
// Sub 2: per-cell ".." popup + per-column body popup.
var private TABLES_CM2_CELL_TEXT : table<int; NarrativeState> // 4*3 cells
var private TABLES_CM2_SL        : table<int; EmptyMarkerState> // 4*3 same_line
var private TABLES_CM2_DOT_BTN   : table<int; ClickState>     // 4*3 ".." buttons
var private TABLES_CM2_DOT_POPUP : table<int; PopupContextItemState> // per-cell popup
var private TABLES_CM2_DOT_TEXT  : table<int; NarrativeState>
var private TABLES_CM2_DOT_CLOSE : table<int; ClickState>
var private TABLES_CM2_HOVERED_TEXT : NarrativeState
// "MyPopup" string-id stateless popup variables — 4 columns (3 + 1 trailing-unused).
var private TABLES_CM2_HOVERED_COLUMN : int = -1
// Per-column popup_window state — shared str_id "MyPopup", disambiguated by with_id(column).
var private TABLES_CM2_COL_POPUP : table<int; PopupWindowState>
var private TABLES_CM2_COL_TEXT  : table<int; NarrativeState>
var private TABLES_CM2_COL_CLOSE : table<int; ClickState>

// ---- Section 22 Synced instances state ----
var private TABLES_SYNCED_FLAGS = (ImGuiTableFlags.Resizable |
                                   ImGuiTableFlags.Reorderable |
                                   ImGuiTableFlags.Hideable |
                                   ImGuiTableFlags.Borders |
                                   ImGuiTableFlags.SizingFixedFit |
                                   ImGuiTableFlags.NoSavedSettings)
// Per-iter CollapsingHeader state (3 iters).
var private TABLES_SYNCED_HEADER : table<int; CollapsingHeaderState>
// Per-iter cell text — different cell counts per iter (9 vs 27 vs 9).
var private TABLES_SYNCED_CELL : table<int; NarrativeState>  // key = iter * 32 + cell

// ---- Section 23 Sorting state ----
let private TABLES_SORT_TEMPLATE_NAMES <- ["Banana", "Apple", "Cherry",
                                           "Watermelon", "Grapefruit",
                                           "Strawberry", "Mango", "Kiwi",
                                           "Orange", "Pineapple", "Blueberry",
                                           "Plum", "Coconut", "Pear", "Apricot"]
// Per-column user_id codes (mirror cpp MyItemColumnID). Description is
// new in section 24 Advanced — kept adjacent to the section-23 entries
// because both sections reuse the same SortItem struct + template names.
let private TABLES_SORT_COL_ID          = 0u
let private TABLES_SORT_COL_NAME        = 1u
let private TABLES_SORT_COL_ACTION      = 2u
let private TABLES_SORT_COL_QUANTITY    = 3u
let private TABLES_SORT_COL_DESCRIPTION = 4u

struct private SortItem {
    id       : int
    name     : string
    quantity : int
}
var private TABLES_SORT_ITEMS : array<SortItem>
var private TABLES_SORT_FLAGS = (ImGuiTableFlags.Resizable |
                                 ImGuiTableFlags.Reorderable |
                                 ImGuiTableFlags.Hideable |
                                 ImGuiTableFlags.Sortable |
                                 ImGuiTableFlags.SortMulti |
                                 ImGuiTableFlags.RowBg |
                                 ImGuiTableFlags.BordersOuter |
                                 ImGuiTableFlags.BordersV |
                                 ImGuiTableFlags.NoBordersInBody |
                                 ImGuiTableFlags.ScrollY)
// Per-row state: 4 cells per row (ID/Name/Action/Quantity) — keyed by
// SortItem.id (stable across re-orderings) so widget paths track the row
// even after sort.
var private TABLES_SORT_ID_TEXT     : table<int; NarrativeState>
var private TABLES_SORT_NAME_TEXT   : table<int; NarrativeState>
var private TABLES_SORT_ACTION_BTN  : table<int; ClickState>
var private TABLES_SORT_QTY_TEXT    : table<int; NarrativeState>

// ---- Section 24 Advanced state ----
// Mirrors cpp:5713-6033; reuses section 23's SortItem + TABLES_SORT_COL_* ids.
// ITEMS lifecycle is independent (user-editable count, Chop/Eat mutates qty).

// Contents-type enum (mirror cpp ContentsType). The Combo at the bottom
// of the Options/Other sub-tree drives this; the per-row first column
// switches on it.
let private TABLES_ADV_CT_TEXT                  = 0
let private TABLES_ADV_CT_BUTTON                = 1
let private TABLES_ADV_CT_SMALL_BUTTON          = 2
let private TABLES_ADV_CT_FILL_BUTTON           = 3
let private TABLES_ADV_CT_SELECTABLE            = 4
let private TABLES_ADV_CT_SELECTABLE_SPAN_ROW   = 5

// Table flags (cpp:5724-5729). Defaults: full Resizable/Reorderable/
// Hideable/Sortable/SortMulti + RowBg/Borders/NoBordersInBody +
// ScrollX/ScrollY + SizingFixedFit.
var private TABLES_ADV_FLAGS = (ImGuiTableFlags.Resizable |
                                ImGuiTableFlags.Reorderable |
                                ImGuiTableFlags.Hideable |
                                ImGuiTableFlags.Sortable |
                                ImGuiTableFlags.SortMulti |
                                ImGuiTableFlags.RowBg |
                                ImGuiTableFlags.Borders |
                                ImGuiTableFlags.NoBordersInBody |
                                ImGuiTableFlags.ScrollX |
                                ImGuiTableFlags.ScrollY |
                                ImGuiTableFlags.SizingFixedFit)
// Per-column shared flags (cpp:5730 `columns_base_flags`). Only
// AngledHeader is editable in the Options/Headers sub-tree; OR'd into
// every TableSetupColumn call below.
var private TABLES_ADV_COLUMNS_BASE_FLAGS = ImGuiTableColumnFlags.None

var private TABLES_ADV_CONTENTS_TYPE      : int    = 5 // CT_SelectableSpanRow
var private TABLES_ADV_FREEZE_COLS        : int    = 1
var private TABLES_ADV_FREEZE_ROWS        : int    = 1
// items_count default = template_count * 2 = 30 (cpp:5737).
var private TABLES_ADV_ITEMS_COUNT        : int    = 30
var private TABLES_ADV_OUTER_SIZE_VALUE   : float2 = float2(0.0f, 0.0f) // seeded in [init] from TextLineHeightWithSpacing * 12
var private TABLES_ADV_ROW_MIN_HEIGHT     : float  = 0.0f
var private TABLES_ADV_INNER_WIDTH        : float  = 0.0f
var private TABLES_ADV_OUTER_SIZE_ENABLED : bool   = true
var private TABLES_ADV_SHOW_HEADERS       : bool   = true
var private TABLES_ADV_SHOW_WRAPPED_TEXT  : bool   = false
// SHOW_DEBUG_DETAILS / SCROLL_CUR / SCROLL_MAX dropped — see show_advanced().
// cpp's debug readout reads ImDrawList.CmdBuffer.Size, which the daslang
// ImVector binding doesn't surface as a readable accessor.

// items + selection. Items are owned per-section (separate lifecycle
// from TABLES_SORT_ITEMS); selection is an unsorted int array of
// item.id values (mirrors cpp's ImVector<int> selection).
var private TABLES_ADV_ITEMS     : array<SortItem>
var private TABLES_ADV_SELECTION : array<int>
var private TABLES_ADV_ITEMS_NEED_SORT : bool = false
// Cache of last-seen TableSortSpec list — filled when sort_specs() fires (SpecsDirty),
// reused by a Chop/Eat-triggered re-sort. The boost block helper auto-clears SpecsDirty
// on return, so the raw pointer is gone by then — cache it instead.
var private TABLES_ADV_LAST_SPECS : array<TableSortSpec>

// Per-row widget state, keyed by SortItem.id so paths track items across re-sorts.
// Not pre-seeded (the row helper auto-inserts on first access); an items_count
// change wipes them so the registry doesn't accumulate stale entries.
var private TABLES_ADV_ID_TEXT      : table<int; NarrativeState>  // CT_Text
var private TABLES_ADV_ID_BTN       : table<int; ClickState>      // CT_Button / SmallButton / FillButton
var private TABLES_ADV_ID_SEL       : table<int; ClickState>      // CT_Selectable[_SpanRow]
var private TABLES_ADV_NAME_TEXT    : table<int; NarrativeState>
var private TABLES_ADV_CHOP_BTN     : table<int; ClickState>
var private TABLES_ADV_EAT_BTN      : table<int; ClickState>
var private TABLES_ADV_QTY_TEXT     : table<int; NarrativeState>
var private TABLES_ADV_DESC_TEXT    : table<int; NarrativeState>
var private TABLES_ADV_HIDDEN_TEXT  : table<int; NarrativeState>
// Inter-button same_line marker — one per row.
var private TABLES_ADV_SL_CHOP_EAT  : table<int; EmptyMarkerState>

[init]
def seed_sort_items() {
    // cpp:5643 — 50 items, name picked from template_items_names by
    // n % template_count, quantity = (n*n - n) % 20.
    TABLES_SORT_ITEMS |> reserve(50)
    for (n in range(50)) {
        let template_n = n % length(TABLES_SORT_TEMPLATE_NAMES)
        TABLES_SORT_ITEMS |> push(SortItem(
            id = n,
            name = TABLES_SORT_TEMPLATE_NAMES[template_n],
            quantity = (n * n - n) % 20))
    }
}

[init]
def seed_columns_flags() {
    // cpp:4868 — `column_flags[3] = { DefaultSort, None, DefaultHide }`.
    TABLES_COLFLAGS_INPUT[0] = ImGuiTableColumnFlags.DefaultSort
    TABLES_COLFLAGS_INPUT[1] = ImGuiTableColumnFlags.None
    TABLES_COLFLAGS_INPUT[2] = ImGuiTableColumnFlags.DefaultHide
}

[init]
def seed_padding_cells() {
    // Pre-populate the 15 InputText cells with "edit me" (mirrors cpp's static
    // text_bufs + init flag). The boost init= named-tuple sugar isn't supported on
    // the indexed [widget] form, so a direct field set in [init] is the workaround.
    for (i in range(15)) {
        TABLES_PAD2_CELL_IT[i].value := "edit me"
    }
}

[init]
def seed_sizing_policy() {
    // Mirrors `static ImGuiTableFlags sizing_policy_flags[4]` in cpp:4626.
    // Per-iteration table_n is the index into this map; the body OR's the
    // chosen policy with TABLES_SIZING_FLAGS1 before BeginTable.
    TABLES_SIZING_POLICY[0] = ImGuiTableFlags.SizingFixedFit
    TABLES_SIZING_POLICY[1] = ImGuiTableFlags.SizingFixedSame
    TABLES_SIZING_POLICY[2] = ImGuiTableFlags.SizingStretchProp
    TABLES_SIZING_POLICY[3] = ImGuiTableFlags.SizingStretchSame
}

[init]
def seed_advanced_outer_size() {
    // cpp:5738 — outer_size_value = ImVec2(0, TEXT_BASE_HEIGHT * 12).
    // GetTextLineHeightWithSpacing() needs an active frame, so we can't call it at
    // [init]; use a hardcoded ~17px line height, user-adjustable via the DragFloat2.
    TABLES_ADV_OUTER_SIZE_VALUE = float2(0.0f, 17.0f * 12.0f)
}

// Synchronize TABLES_ADV_ITEMS with TABLES_ADV_ITEMS_COUNT. Called every
// frame at the top of show_advanced(); mirrors the cpp:5870-5881 resize
// + per-item re-seed (always overwrites, matching cpp).
def private rebuild_adv_items() {
    let want = max(TABLES_ADV_ITEMS_COUNT, 0)
    if (length(TABLES_ADV_ITEMS) == want) return
    TABLES_ADV_ITEMS |> resize(want)
    let template_count = length(TABLES_SORT_TEMPLATE_NAMES)
    for (n in range(want)) {
        let template_n = n % template_count
        TABLES_ADV_ITEMS[n].id = n
        TABLES_ADV_ITEMS[n].name := TABLES_SORT_TEMPLATE_NAMES[template_n]
        // cpp:5879 — only templates 3/4 get non-zero defaults.
        var qty = 0
        if (template_n == 3) {
            qty = 10
        } elif (template_n == 4) {
            qty = 20
        }
        TABLES_ADV_ITEMS[n].quantity = qty
    }
}

// Multi-key sort comparator shared with the sort_specs() block-arg below (mirrors
// cpp:4024-4061). Walks specs in order, returns the first non-zero delta with
// asc/desc sign; falls back on an ID tie-break since daslang sort() isn't stable.
def private adv_compare(a : SortItem; b : SortItem;
                        specs : array<TableSortSpec>) : bool {
    for (spec in specs) {
        var delta = 0
        if (spec.column_user_id == TABLES_SORT_COL_ID) {
            delta = a.id - b.id
        } elif (spec.column_user_id == TABLES_SORT_COL_NAME) {
            delta = a.name < b.name ? -1 : (a.name > b.name ? 1 : 0)
        } elif (spec.column_user_id == TABLES_SORT_COL_QUANTITY) {
            delta = a.quantity - b.quantity
        } elif (spec.column_user_id == TABLES_SORT_COL_DESCRIPTION) {
            // cpp:4048 — Description sorts by Name (it's a canned literal, no per-item field).
            delta = a.name < b.name ? -1 : (a.name > b.name ? 1 : 0)
        }
        if (delta != 0) {
            let asc = spec.sort_direction == ImGuiSortDirection.Ascending
            return asc ? delta < 0 : delta > 0
        }
    }
    return a.id < b.id
}

def ShowDemoWindowTables() {
    collapsing_header(TABLES_SECTION,
                      (text = "Tables & Columns", closable = false,
                       flags = ImGuiTreeNodeFlags.None)) {
        // Expand all / Collapse all toolbar.
        if (button(TABLES_EXPAND_ALL_BTN, (text = "Expand all"))) {
            g_open_action = 1
        }
        same_line(TABLES_SL_EXPAND)
        if (button(TABLES_COLLAPSE_ALL_BTN, (text = "Collapse all"))) {
            g_open_action = 0
        }
        separator(TABLES_SEP_TOOLBAR)

        show_basic()
        show_borders_background()
        show_resizable_stretch()
        show_resizable_fixed()
        show_resizable_mixed()
        show_reorderable_hideable_headers()
        show_padding()
        show_sizing_policies()
        show_vertical_scrolling()
        show_horizontal_scrolling()
        show_columns_flags()
        show_columns_widths()
        show_nested_tables()
        show_row_height()
        show_outer_size()
        show_background_color()
        show_tree_view()
        show_item_width()
        show_custom_headers()
        show_angled_headers()
        show_context_menus()
        show_synced_instances()
        show_sorting()
        show_advanced()

        // Reset open_action so the next frame's tree_nodes inherit their
        // sticky open/closed state instead of being forced again.
        g_open_action = -1
    }
}

// =============================================================================
// Section 1: Basic — cpp:4198-4256
// =============================================================================
def private show_basic() {
    apply_open_action()
    tree_node(TABLES_BASIC, (text = "Basic", flags = ImGuiTreeNodeFlags.None)) {
        // Method 1: TableNextRow + TableSetColumnIndex per cell.
        data_table(TABLES_BASIC_T1, (text = "##t1", columns = 3,
                                     flags = ImGuiTableFlags.None,
                                     outer_size = float2(0.0f, 0.0f),
                                     inner_width = 0.0f)) {
            for (row in range(4)) {
                table_next_row()
                for (col in range(3)) {
                    table_set_column_index(col)
                    let key = row * 3 + col
                    text(TABLES_BASIC_T1_CELL[key],
                         (text = "Row {row} Column {col}"))
                }
            }
        }

        // Method 2: TableNextRow + TableNextColumn manually per cell.
        data_table(TABLES_BASIC_T2, (text = "##t2", columns = 3,
                                     flags = ImGuiTableFlags.None,
                                     outer_size = float2(0.0f, 0.0f),
                                     inner_width = 0.0f)) {
            for (row in range(4)) {
                table_next_row()
                table_next_column()
                text(TABLES_BASIC_T2_CELL[row * 3 + 0], (text = "Row {row}"))
                table_next_column()
                text(TABLES_BASIC_T2_CELL[row * 3 + 1], (text = "Some contents"))
                table_next_column()
                text(TABLES_BASIC_T2_CELL[row * 3 + 2], (text = "123.456"))
            }
        }

        // Method 3: TableNextColumn auto-wraps to new rows.
        data_table(TABLES_BASIC_T3, (text = "##t3", columns = 3,
                                     flags = ImGuiTableFlags.None,
                                     outer_size = float2(0.0f, 0.0f),
                                     inner_width = 0.0f)) {
            for (item in range(14)) {
                table_next_column()
                text(TABLES_BASIC_T3_CELL[item], (text = "Item {item}"))
            }
        }
    }
}

// =============================================================================
// Section 2: Borders, background — cpp:4261-4327
// =============================================================================
def private show_borders_background() {
    apply_open_action()
    tree_node(TABLES_BORDERS, (text = "Borders, background",
                               flags = ImGuiTreeNodeFlags.None)) {
        // Borders/background flag toggles; nested with_indent blocks mirror cpp's Indent/Unindent grouping.
        edit_checkbox_flags(safe_addr(TABLES_BORDERS_FLAGS),
            (id = "TBL_B_F_ROWBG", text = "ImGuiTableFlags_RowBg",
             flags_value = ImGuiTableFlags.RowBg))
        edit_checkbox_flags(safe_addr(TABLES_BORDERS_FLAGS),
            (id = "TBL_B_F_BORDERS", text = "ImGuiTableFlags_Borders",
             flags_value = ImGuiTableFlags.Borders))
        with_indent(0.0f) {
            edit_checkbox_flags(safe_addr(TABLES_BORDERS_FLAGS),
                (id = "TBL_B_F_BORDERSH", text = "ImGuiTableFlags_BordersH",
                 flags_value = ImGuiTableFlags.BordersH))
            with_indent(0.0f) {
                edit_checkbox_flags(safe_addr(TABLES_BORDERS_FLAGS),
                    (id = "TBL_B_F_BORDERSOH", text = "ImGuiTableFlags_BordersOuterH",
                     flags_value = ImGuiTableFlags.BordersOuterH))
                edit_checkbox_flags(safe_addr(TABLES_BORDERS_FLAGS),
                    (id = "TBL_B_F_BORDERSIH", text = "ImGuiTableFlags_BordersInnerH",
                     flags_value = ImGuiTableFlags.BordersInnerH))
            }
            edit_checkbox_flags(safe_addr(TABLES_BORDERS_FLAGS),
                (id = "TBL_B_F_BORDERSV", text = "ImGuiTableFlags_BordersV",
                 flags_value = ImGuiTableFlags.BordersV))
            with_indent(0.0f) {
                edit_checkbox_flags(safe_addr(TABLES_BORDERS_FLAGS),
                    (id = "TBL_B_F_BORDERSOV", text = "ImGuiTableFlags_BordersOuterV",
                     flags_value = ImGuiTableFlags.BordersOuterV))
                edit_checkbox_flags(safe_addr(TABLES_BORDERS_FLAGS),
                    (id = "TBL_B_F_BORDERSIV", text = "ImGuiTableFlags_BordersInnerV",
                     flags_value = ImGuiTableFlags.BordersInnerV))
            }
            edit_checkbox_flags(safe_addr(TABLES_BORDERS_FLAGS),
                (id = "TBL_B_F_BORDERSO", text = "ImGuiTableFlags_BordersOuter",
                 flags_value = ImGuiTableFlags.BordersOuter))
            edit_checkbox_flags(safe_addr(TABLES_BORDERS_FLAGS),
                (id = "TBL_B_F_BORDERSI", text = "ImGuiTableFlags_BordersInner",
                 flags_value = ImGuiTableFlags.BordersInner))
        }

        // Contents-type radio (Text vs FillButton) + display-headers checkbox.
        align_text_to_frame_padding(TABLES_BORDERS_ATFP)
        text(TABLES_BORDERS_CT_LABEL, (text = "Cell contents:"))
        same_line(TABLES_BORDERS_SL_RT)
        radio_button_int(TABLES_BORDERS_CONTENTS, (text = "Text", v_button = 0))
        same_line(TABLES_BORDERS_SL_RFB)
        radio_button_int(TABLES_BORDERS_CONTENTS, (text = "FillButton", v_button = 1))

        checkbox(TABLES_BORDERS_DH_CHECK, (text = "Display headers", init = false))

        edit_checkbox_flags(safe_addr(TABLES_BORDERS_FLAGS),
            (id = "TBL_B_F_NBINBODY", text = "ImGuiTableFlags_NoBordersInBody",
             flags_value = ImGuiTableFlags.NoBordersInBody))

        // 3x5 grid driven by the runtime flag set + contents-type selection.
        data_table(TABLES_BORDERS_T1, (text = "##bt1", columns = 3,
                                       flags = TABLES_BORDERS_FLAGS,
                                       outer_size = float2(0.0f, 0.0f),
                                       inner_width = 0.0f)) {
            if (TABLES_BORDERS_DH_CHECK.value) {
                table_setup_column("One")
                table_setup_column("Two")
                table_setup_column("Three")
                table_headers_row()
            }
            for (row in range(5)) {
                table_next_row()
                for (col in range(3)) {
                    table_set_column_index(col)
                    let key = row * 3 + col
                    let cell_text = "Hello {col},{row}"
                    if (TABLES_BORDERS_CONTENTS.value == 0) {
                        text(TABLES_BORDERS_T1_CELL_TEXT[key], (text = cell_text))
                    } else {
                        button(TABLES_BORDERS_T1_CELL_BTN[key],
                               (text = cell_text, size = float2(-1.0f, 0.0f)))
                    }
                }
            }
        }
    }
}

// =============================================================================
// Section 3: Resizable, stretch — cpp:4332-4359
// =============================================================================
def private show_resizable_stretch() {
    apply_open_action()
    tree_node(TABLES_RSTRETCH, (text = "Resizable, stretch",
                                flags = ImGuiTreeNodeFlags.None)) {
        // SizingStretchSame is the default when ScrollX is off; note _Resizable implicitly enables _BordersInnerV.
        edit_checkbox_flags(safe_addr(TABLES_RSTRETCH_FLAGS),
            (id = "TBL_RS_F_RESIZABLE", text = "ImGuiTableFlags_Resizable",
             flags_value = ImGuiTableFlags.Resizable))
        edit_checkbox_flags(safe_addr(TABLES_RSTRETCH_FLAGS),
            (id = "TBL_RS_F_BORDERSV", text = "ImGuiTableFlags_BordersV",
             flags_value = ImGuiTableFlags.BordersV))

        data_table(TABLES_RSTRETCH_T1, (text = "##rs1", columns = 3,
                                        flags = TABLES_RSTRETCH_FLAGS,
                                        outer_size = float2(0.0f, 0.0f),
                                        inner_width = 0.0f)) {
            for (row in range(5)) {
                table_next_row()
                for (col in range(3)) {
                    table_set_column_index(col)
                    text(TABLES_RSTRETCH_CELL[row * 3 + col],
                         (text = "Hello {col},{row}"))
                }
            }
        }
    }
}

// =============================================================================
// Section 4: Resizable, fixed — cpp:4364-4393
// =============================================================================
def private show_resizable_fixed() {
    apply_open_action()
    tree_node(TABLES_RFIXED, (text = "Resizable, fixed",
                              flags = ImGuiTreeNodeFlags.None)) {
        // SizingFixedFit keeps columns at their initial width (subject to NoHostExtendX); double-click a border to auto-fit.
        edit_checkbox_flags(safe_addr(TABLES_RFIXED_FLAGS),
            (id = "TBL_RF_F_NOHOST", text = "ImGuiTableFlags_NoHostExtendX",
             flags_value = ImGuiTableFlags.NoHostExtendX))

        data_table(TABLES_RFIXED_T1, (text = "##rf1", columns = 3,
                                      flags = TABLES_RFIXED_FLAGS,
                                      outer_size = float2(0.0f, 0.0f),
                                      inner_width = 0.0f)) {
            for (row in range(5)) {
                table_next_row()
                for (col in range(3)) {
                    table_set_column_index(col)
                    text(TABLES_RFIXED_CELL[row * 3 + col],
                         (text = "Hello {col},{row}"))
                }
            }
        }
    }
}

// =============================================================================
// Section 5: Resizable, mixed — cpp:4398-4443
// =============================================================================
def private show_resizable_mixed() {
    apply_open_action()
    tree_node(TABLES_RMIXED, (text = "Resizable, mixed",
                              flags = ImGuiTreeNodeFlags.None)) {
        // Per-column WidthFixed/WidthStretch mix; the 6-col table also demonstrates DefaultHide.
        let tflags = TABLES_RMIXED_FLAGS

        // Table 1: 3 columns (2 Fixed + 1 Stretch).
        data_table(TABLES_RMIXED_T1, (text = "##rm1", columns = 3, flags = tflags,
                                      outer_size = float2(0.0f, 0.0f),
                                      inner_width = 0.0f)) {
            table_setup_column("AAA", ImGuiTableColumnFlags.WidthFixed)
            table_setup_column("BBB", ImGuiTableColumnFlags.WidthFixed)
            table_setup_column("CCC", ImGuiTableColumnFlags.WidthStretch)
            table_headers_row()
            for (row in range(5)) {
                table_next_row()
                for (col in range(3)) {
                    table_set_column_index(col)
                    let kind = col == 2 ? "Stretch" : "Fixed"
                    text(TABLES_RMIXED_T1_CELL[row * 3 + col],
                         (text = "{kind} {col},{row}"))
                }
            }
        }

        // Table 2: 6 columns (3 Fixed + 3 Stretch, one DefaultHide in each).
        data_table(TABLES_RMIXED_T2, (text = "##rm2", columns = 6, flags = tflags,
                                      outer_size = float2(0.0f, 0.0f),
                                      inner_width = 0.0f)) {
            table_setup_column("AAA", ImGuiTableColumnFlags.WidthFixed)
            table_setup_column("BBB", ImGuiTableColumnFlags.WidthFixed)
            table_setup_column("CCC", ImGuiTableColumnFlags.WidthFixed | ImGuiTableColumnFlags.DefaultHide)
            table_setup_column("DDD", ImGuiTableColumnFlags.WidthStretch)
            table_setup_column("EEE", ImGuiTableColumnFlags.WidthStretch)
            table_setup_column("FFF", ImGuiTableColumnFlags.WidthStretch | ImGuiTableColumnFlags.DefaultHide)
            table_headers_row()
            for (row in range(5)) {
                table_next_row()
                for (col in range(6)) {
                    table_set_column_index(col)
                    let kind = col >= 3 ? "Stretch" : "Fixed"
                    text(TABLES_RMIXED_T2_CELL[row * 6 + col],
                         (text = "{kind} {col},{row}"))
                }
            }
        }
    }
}

// =============================================================================
// Section 6: Reorderable, hideable, with headers — cpp:4448-4503
// =============================================================================
def private show_reorderable_hideable_headers() {
    apply_open_action()
    tree_node(TABLES_RHH, (text = "Reorderable, hideable, with headers",
                           flags = ImGuiTreeNodeFlags.None)) {
        // Click+drag a header to reorder columns; right-click a header for the visibility menu.
        edit_checkbox_flags(safe_addr(TABLES_RHH_FLAGS),
            (id = "TBL_RHH_F_RESIZABLE", text = "ImGuiTableFlags_Resizable",
             flags_value = ImGuiTableFlags.Resizable))
        edit_checkbox_flags(safe_addr(TABLES_RHH_FLAGS),
            (id = "TBL_RHH_F_REORDERABLE", text = "ImGuiTableFlags_Reorderable",
             flags_value = ImGuiTableFlags.Reorderable))
        edit_checkbox_flags(safe_addr(TABLES_RHH_FLAGS),
            (id = "TBL_RHH_F_HIDEABLE", text = "ImGuiTableFlags_Hideable",
             flags_value = ImGuiTableFlags.Hideable))
        edit_checkbox_flags(safe_addr(TABLES_RHH_FLAGS),
            (id = "TBL_RHH_F_NBINBODY", text = "ImGuiTableFlags_NoBordersInBody",
             flags_value = ImGuiTableFlags.NoBordersInBody))
        edit_checkbox_flags(safe_addr(TABLES_RHH_FLAGS),
            (id = "TBL_RHH_F_NBINBODYUNTILRESIZE",
             text = "ImGuiTableFlags_NoBordersInBodyUntilResize",
             flags_value = ImGuiTableFlags.NoBordersInBodyUntilResize))
        edit_checkbox_flags(safe_addr(TABLES_RHH_FLAGS),
            (id = "TBL_RHH_F_HIGHLIGHTHOVERED",
             text = "ImGuiTableFlags_HighlightHoveredColumn",
             flags_value = ImGuiTableFlags.HighlightHoveredColumn))

        let tflags = TABLES_RHH_FLAGS

        // Table 1: default outer_size (stretches to fill available width).
        data_table(TABLES_RHH_T1, (text = "##rhh1", columns = 3, flags = tflags,
                                   outer_size = float2(0.0f, 0.0f),
                                   inner_width = 0.0f)) {
            table_setup_column("One")
            table_setup_column("Two")
            table_setup_column("Three")
            table_headers_row()
            for (row in range(6)) {
                table_next_row()
                for (col in range(3)) {
                    table_set_column_index(col)
                    text(TABLES_RHH_T1_CELL[row * 3 + col],
                         (text = "Hello {col},{row}"))
                }
            }
        }

        // Table 2: outer_size.x=0 + SizingFixedFit packs columns tight (only valid without ScrollX or stretch columns).
        let tflags2 = TABLES_RHH_FLAGS | ImGuiTableFlags.SizingFixedFit
        data_table(TABLES_RHH_T2, (text = "##rhh2", columns = 3, flags = tflags2,
                                   outer_size = float2(0.0f, 0.0f),
                                   inner_width = 0.0f)) {
            table_setup_column("One")
            table_setup_column("Two")
            table_setup_column("Three")
            table_headers_row()
            for (row in range(6)) {
                table_next_row()
                for (col in range(3)) {
                    table_set_column_index(col)
                    text(TABLES_RHH_T2_CELL[row * 3 + col],
                         (text = "Fixed {col},{row}"))
                }
            }
        }
    }
}

// =============================================================================
// Section 7: Padding — cpp:4508-4613
// =============================================================================
def private show_padding_inputtext_grid() {
    // Body extracted so the conditional FrameBg push wraps one call instead of duplicating the 15-cell loop.
    for (cell in range(15)) {
        table_next_column()
        SetNextItemWidth(-FLT_MIN)
        input_text(TABLES_PAD2_CELL_IT[cell], (text = "##cell"))
    }
}

def private show_padding() {
    apply_open_action()
    tree_node(TABLES_PADDING, (text = "Padding",
                               flags = ImGuiTreeNodeFlags.None)) {
        // ----- Sub-table 1: padding-flag effect on X spacing -----

        // First row reads ContentRegionAvail.x so each toggle's available-width effect is visible.
        edit_checkbox_flags(safe_addr(TABLES_PAD1_FLAGS),
            (id = "TBL_P1_F_PAD_OUT_X", text = "ImGuiTableFlags_PadOuterX",
             flags_value = ImGuiTableFlags.PadOuterX))
        edit_checkbox_flags(safe_addr(TABLES_PAD1_FLAGS),
            (id = "TBL_P1_F_NOPAD_OUT_X", text = "ImGuiTableFlags_NoPadOuterX",
             flags_value = ImGuiTableFlags.NoPadOuterX))
        edit_checkbox_flags(safe_addr(TABLES_PAD1_FLAGS),
            (id = "TBL_P1_F_NOPAD_IN_X", text = "ImGuiTableFlags_NoPadInnerX",
             flags_value = ImGuiTableFlags.NoPadInnerX))
        edit_checkbox_flags(safe_addr(TABLES_PAD1_FLAGS),
            (id = "TBL_P1_F_BORDERS_OV", text = "ImGuiTableFlags_BordersOuterV",
             flags_value = ImGuiTableFlags.BordersOuterV))
        edit_checkbox_flags(safe_addr(TABLES_PAD1_FLAGS),
            (id = "TBL_P1_F_BORDERS_IV", text = "ImGuiTableFlags_BordersInnerV",
             flags_value = ImGuiTableFlags.BordersInnerV))
        checkbox(TABLES_PAD1_SHOW_HEADERS, (text = "show_headers", init = false))

        data_table(TABLES_PAD1_T, (text = "##pad1", columns = 3,
                                   flags = TABLES_PAD1_FLAGS,
                                   outer_size = float2(0.0f, 0.0f),
                                   inner_width = 0.0f)) {
            if (TABLES_PAD1_SHOW_HEADERS.value) {
                table_setup_column("One")
                table_setup_column("Two")
                table_setup_column("Three")
                table_headers_row()
            }
            for (row in range(5)) {
                table_next_row()
                for (col in range(3)) {
                    table_set_column_index(col)
                    if (row == 0) {
                        let avail = GetContentRegionAvail().x
                        text(TABLES_PAD1_AVAIL[col],
                             (text = "Avail {avail}"))
                    } else {
                        let key = (row - 1) * 3 + col
                        button(TABLES_PAD1_BTN[key],
                               (text = "Hello {col},{row}",
                                size = float2(-FLT_MIN, 0.0f)))
                    }
                }
            }
        }

        // ----- Sub-table 2: explicit CellPadding style-var -----

        // Sliderable CellPadding overrides ImGuiStyle; optional FrameBg=0 hides the InputText frame so padding is clear.
        edit_checkbox_flags(safe_addr(TABLES_PAD2_FLAGS),
            (id = "TBL_P2_F_BORDERS", text = "ImGuiTableFlags_Borders",
             flags_value = ImGuiTableFlags.Borders))
        edit_checkbox_flags(safe_addr(TABLES_PAD2_FLAGS),
            (id = "TBL_P2_F_BORDERSH", text = "ImGuiTableFlags_BordersH",
             flags_value = ImGuiTableFlags.BordersH))
        edit_checkbox_flags(safe_addr(TABLES_PAD2_FLAGS),
            (id = "TBL_P2_F_BORDERSV", text = "ImGuiTableFlags_BordersV",
             flags_value = ImGuiTableFlags.BordersV))
        edit_checkbox_flags(safe_addr(TABLES_PAD2_FLAGS),
            (id = "TBL_P2_F_BORDERS_IN", text = "ImGuiTableFlags_BordersInner",
             flags_value = ImGuiTableFlags.BordersInner))
        edit_checkbox_flags(safe_addr(TABLES_PAD2_FLAGS),
            (id = "TBL_P2_F_BORDERS_OUT", text = "ImGuiTableFlags_BordersOuter",
             flags_value = ImGuiTableFlags.BordersOuter))
        edit_checkbox_flags(safe_addr(TABLES_PAD2_FLAGS),
            (id = "TBL_P2_F_ROWBG", text = "ImGuiTableFlags_RowBg",
             flags_value = ImGuiTableFlags.RowBg))
        edit_checkbox_flags(safe_addr(TABLES_PAD2_FLAGS),
            (id = "TBL_P2_F_RESIZABLE", text = "ImGuiTableFlags_Resizable",
             flags_value = ImGuiTableFlags.Resizable))
        checkbox(TABLES_PAD2_SHOW_FRAMEBG, (text = "show_widget_frame_bg", init = true))
        edit_slider_float2(safe_addr(TABLES_PAD2_CELL_PADDING),
            (id = "TBL_P2_CELL_PAD", text = "CellPadding",
             v_min = 0.0f, v_max = 10.0f, format = "%.0f"))

        with_style((ImGuiStyleVar.CellPadding, TABLES_PAD2_CELL_PADDING)) {
            data_table(TABLES_PAD2_T, (text = "##pad2", columns = 3,
                                       flags = TABLES_PAD2_FLAGS,
                                       outer_size = float2(0.0f, 0.0f),
                                       inner_width = 0.0f)) {
                if (!TABLES_PAD2_SHOW_FRAMEBG.value) {
                    with_style((ImGuiCol.FrameBg, 0u)) {
                        show_padding_inputtext_grid()
                    }
                } else {
                    show_padding_inputtext_grid()
                }
            }
        }
    }
}

// =============================================================================
// Section 8: Sizing policies — cpp:4618-4722
// =============================================================================

// cpp helper EditTableSizingFlags ported here — strips the four Sizing* policy bits
// out, lets the user pick one via a Combo, and ORs it back in. Pure bit manipulation.
def private edit_table_sizing_flags(var p_flags : ImGuiTableFlags? implicit; id : string) {
    let policy_mask = ImGuiTableFlags.SizingMask_
    let values = fixed_array(ImGuiTableFlags.None,
                             ImGuiTableFlags.SizingFixedFit,
                             ImGuiTableFlags.SizingFixedSame,
                             ImGuiTableFlags.SizingStretchProp,
                             ImGuiTableFlags.SizingStretchSame)
    var current : ImGuiTableFlags
    unsafe { current = *p_flags & policy_mask; }
    var idx = 0
    for (n in range(5)) {
        if (values[n] == current) {
            idx = n
            break
        }
    }
    // Per-call PushID scope so the same Combo IDENT can fire under each caller.
    with_id(id) {
        var items <- ["Default",
                      "ImGuiTableFlags_SizingFixedFit",
                      "ImGuiTableFlags_SizingFixedSame",
                      "ImGuiTableFlags_SizingStretchProp",
                      "ImGuiTableFlags_SizingStretchSame"]
        var local_idx = idx
        if (edit_combo(unsafe(addr(local_idx)),
                       (id = "SIZING_POLICY_COMBO", text = "Sizing Policy",
                        items = items))) {
            unsafe { *p_flags = (*p_flags & ~policy_mask) | values[local_idx]; }
        }
        delete items
    }
}

def private show_sizing_policies() {
    apply_open_action()
    tree_node(TABLES_SIZING, (text = "Sizing policies",
                              flags = ImGuiTreeNodeFlags.None)) {
        // Shared flag toggles applied on top of each per-row policy.
        edit_checkbox_flags(safe_addr(TABLES_SIZING_FLAGS1),
            (id = "TBL_SZ_F_RESIZABLE", text = "ImGuiTableFlags_Resizable",
             flags_value = ImGuiTableFlags.Resizable))
        edit_checkbox_flags(safe_addr(TABLES_SIZING_FLAGS1),
            (id = "TBL_SZ_F_NOHOST", text = "ImGuiTableFlags_NoHostExtendX",
             flags_value = ImGuiTableFlags.NoHostExtendX))

        // Four mini-tables, one per sizing policy; per-table with_id keeps the four Combo IDs disjoint.
        for (table_n in range(4)) {
            with_id("sizing_{table_n}") {
                SetNextItemWidth(GetFontSize() * 30.0f)
                edit_table_sizing_flags(unsafe(addr(TABLES_SIZING_POLICY[table_n])),
                                        "sizing_{table_n}")

                let combined = TABLES_SIZING_POLICY[table_n] | TABLES_SIZING_FLAGS1

                // Table 1: equal-width content ("Oh dear" x 3 across 3 cols).
                data_table(TABLES_SIZING_T1[table_n],
                           (text = "##sz1_{table_n}", columns = 3,
                            flags = combined,
                            outer_size = float2(0.0f, 0.0f),
                            inner_width = 0.0f)) {
                    for (row in range(3)) {
                        table_next_row()
                        let base = table_n * 9 + row * 3
                        for (col in range(3)) {
                            table_next_column()
                            text(TABLES_SIZING_T1_CELL[base + col],
                                 (text = "Oh dear"))
                        }
                    }
                }

                // Table 2: ragged-width content (AAAA / BBBBBBBB / CCCCCCCCCCCC).
                data_table(TABLES_SIZING_T2[table_n],
                           (text = "##sz2_{table_n}", columns = 3,
                            flags = combined,
                            outer_size = float2(0.0f, 0.0f),
                            inner_width = 0.0f)) {
                    let labels = fixed_array("AAAA", "BBBBBBBB", "CCCCCCCCCCCC")
                    for (row in range(3)) {
                        table_next_row()
                        let base = table_n * 9 + row * 3
                        for (col in range(3)) {
                            table_next_column()
                            text(TABLES_SIZING_T2_CELL[base + col],
                                 (text = labels[col]))
                        }
                    }
                }
            }
        }

        // ----- Advanced sub-block -----
        text(TABLES_SIZING_ADV_LABEL, (text = "Advanced"))
        with_id("sizing_advanced") {
            SetNextItemWidth(GetFontSize() * 30.0f)
            edit_table_sizing_flags(safe_addr(TABLES_SIZING_ADV_FLAGS),
                                    "sizing_adv_combo")

            // Contents-type Combo (CT enum mirror).
            var ct_items <- ["Show width", "Short Text", "Long Text",
                             "Button", "Fill Button", "InputText"]
            edit_combo(safe_addr(TABLES_SIZING_ADV_CT),
                       (id = "TBL_SZ_ADV_CT", text = "Contents",
                        items = ct_items))
            delete ct_items

            edit_drag_int(safe_addr(TABLES_SIZING_ADV_COLUMNS),
                (id = "TBL_SZ_ADV_COLS", text = "Columns",
                 speed = 0.1f, v_min = 1, v_max = 64, format = "%d",
                 flags = ImGuiSliderFlags.AlwaysClamp))
            edit_checkbox_flags(safe_addr(TABLES_SIZING_ADV_FLAGS),
                (id = "TBL_SZ_ADV_F_RESIZABLE",
                 text = "ImGuiTableFlags_Resizable",
                 flags_value = ImGuiTableFlags.Resizable))
            edit_checkbox_flags(safe_addr(TABLES_SIZING_ADV_FLAGS),
                (id = "TBL_SZ_ADV_F_PRECISE",
                 text = "ImGuiTableFlags_PreciseWidths",
                 flags_value = ImGuiTableFlags.PreciseWidths))
            edit_checkbox_flags(safe_addr(TABLES_SIZING_ADV_FLAGS),
                (id = "TBL_SZ_ADV_F_SCROLLX",
                 text = "ImGuiTableFlags_ScrollX",
                 flags_value = ImGuiTableFlags.ScrollX))
            edit_checkbox_flags(safe_addr(TABLES_SIZING_ADV_FLAGS),
                (id = "TBL_SZ_ADV_F_SCROLLY",
                 text = "ImGuiTableFlags_ScrollY",
                 flags_value = ImGuiTableFlags.ScrollY))
            edit_checkbox_flags(safe_addr(TABLES_SIZING_ADV_FLAGS),
                (id = "TBL_SZ_ADV_F_NOCLIP",
                 text = "ImGuiTableFlags_NoClip",
                 flags_value = ImGuiTableFlags.NoClip))

            let col_count = max(TABLES_SIZING_ADV_COLUMNS, 1)
            let line_height = GetTextLineHeightWithSpacing()
            data_table(TABLES_SIZING_ADV_T,
                       (text = "##sz_adv", columns = col_count,
                        flags = TABLES_SIZING_ADV_FLAGS,
                        outer_size = float2(0.0f, line_height * 7.0f),
                        inner_width = 0.0f)) {
                for (cell in range(10 * col_count)) {
                    table_next_column()
                    let column = table_get_column_index()
                    let row = table_get_row_index()
                    let cell_label = "Hello {column},{row}"
                    if (TABLES_SIZING_ADV_CT == TABLES_SIZING_CT_SHORT_TEXT) {
                        text(TABLES_SIZING_ADV_TEXT[cell], (text = cell_label))
                    } elif (TABLES_SIZING_ADV_CT == TABLES_SIZING_CT_LONG_TEXT) {
                        let adj = column == 0 ? "long" : "longeeer"
                        text(TABLES_SIZING_ADV_TEXT[cell],
                             (text = "Some {adj} text {column},{row}\nOver two lines.."))
                    } elif (TABLES_SIZING_ADV_CT == TABLES_SIZING_CT_SHOW_WIDTH) {
                        let avail = GetContentRegionAvail().x
                        text(TABLES_SIZING_ADV_TEXT[cell],
                             (text = "W: {avail}"))
                    } elif (TABLES_SIZING_ADV_CT == TABLES_SIZING_CT_BUTTON) {
                        button(TABLES_SIZING_ADV_BTN[cell],
                               (text = cell_label, size = float2(0.0f, 0.0f)))
                    } elif (TABLES_SIZING_ADV_CT == TABLES_SIZING_CT_FILL_BUTTON) {
                        button(TABLES_SIZING_ADV_BTN[cell],
                               (text = cell_label, size = float2(-FLT_MIN, 0.0f)))
                    } elif (TABLES_SIZING_ADV_CT == TABLES_SIZING_CT_INPUT_TEXT) {
                        SetNextItemWidth(-FLT_MIN)
                        input_text(TABLES_SIZING_ADV_IT[cell], (text = "##sz_adv_it"))
                    }
                }
            }
        }
    }
}

// =============================================================================
// Section 9: Vertical scrolling, with clipping — cpp:4727-4771
// =============================================================================
def private show_vertical_scrolling() {
    apply_open_action()
    tree_node(TABLES_VSCROLL, (text = "Vertical scrolling, with clipping",
                               flags = ImGuiTreeNodeFlags.None)) {
        // ScrollY toggle.
        edit_checkbox_flags(safe_addr(TABLES_VSCROLL_FLAGS),
            (id = "TBL_VS_F_SCROLLY", text = "ImGuiTableFlags_ScrollY",
             flags_value = ImGuiTableFlags.ScrollY))

        // Outer size MUST be specified when ScrollX/ScrollY is on, else the table fills the host like a child window.
        let line_height = GetTextLineHeightWithSpacing()
        let outer_size = float2(0.0f, line_height * 8.0f)
        data_table(TABLES_VSCROLL_T,
                   (text = "##table_scrolly", columns = 3,
                    flags = TABLES_VSCROLL_FLAGS,
                    outer_size = outer_size,
                    inner_width = 0.0f)) {
            table_setup_scroll_freeze(0, 1) // pin the header row
            table_setup_column("One")
            table_setup_column("Two")
            table_setup_column("Three")
            table_headers_row()

            // 1000 rows via list_clipper — only the visible window is submitted per frame.
            list_clipper(1000) $(start, end_) {
                for (row in range(start, end_)) {
                    table_next_row()
                    for (col in range(3)) {
                        table_set_column_index(col)
                        text(TABLES_VSCROLL_CELL[row * 3 + col],
                             (text = "Hello {col},{row}"))
                    }
                }
            }
        }
    }
}

// =============================================================================
// Section 10: Horizontal scrolling — cpp:4772-4861
// =============================================================================
def private show_horizontal_scrolling() {
    apply_open_action()
    tree_node(TABLES_HSCROLL, (text = "Horizontal scrolling",
                               flags = ImGuiTreeNodeFlags.None)) {
        edit_checkbox_flags(safe_addr(TABLES_HSCROLL_FLAGS),
            (id = "TBL_HS_F_RESIZABLE", text = "ImGuiTableFlags_Resizable",
             flags_value = ImGuiTableFlags.Resizable))
        edit_checkbox_flags(safe_addr(TABLES_HSCROLL_FLAGS),
            (id = "TBL_HS_F_SCROLLX", text = "ImGuiTableFlags_ScrollX",
             flags_value = ImGuiTableFlags.ScrollX))
        edit_checkbox_flags(safe_addr(TABLES_HSCROLL_FLAGS),
            (id = "TBL_HS_F_SCROLLY", text = "ImGuiTableFlags_ScrollY",
             flags_value = ImGuiTableFlags.ScrollY))

        SetNextItemWidth(GetFrameHeight())
        edit_drag_int(safe_addr(TABLES_HSCROLL_FREEZE_COLS),
            (id = "TBL_HS_FREEZE_COLS", text = "freeze_cols",
             speed = 0.2f, v_min = 0, v_max = 9, format = "%d",
             flags = ImGuiSliderFlags.NoInput))
        SetNextItemWidth(GetFrameHeight())
        edit_drag_int(safe_addr(TABLES_HSCROLL_FREEZE_ROWS),
            (id = "TBL_HS_FREEZE_ROWS", text = "freeze_rows",
             speed = 0.2f, v_min = 0, v_max = 9, format = "%d",
             flags = ImGuiSliderFlags.NoInput))

        let line_height = GetTextLineHeightWithSpacing()
        let outer_size = float2(0.0f, line_height * 8.0f)
        data_table(TABLES_HSCROLL_T1,
                   (text = "##table_scrollx", columns = 7,
                    flags = TABLES_HSCROLL_FLAGS,
                    outer_size = outer_size,
                    inner_width = 0.0f)) {
            table_setup_scroll_freeze(TABLES_HSCROLL_FREEZE_COLS,
                                      TABLES_HSCROLL_FREEZE_ROWS)
            table_setup_column("Line #", ImGuiTableColumnFlags.NoHide)
            table_setup_column("One")
            table_setup_column("Two")
            table_setup_column("Three")
            table_setup_column("Four")
            table_setup_column("Five")
            table_setup_column("Six")
            table_headers_row()
            for (row in range(20)) {
                table_next_row()
                for (col in range(7)) {
                    // Skip non-visible columns past col 0 (cpp:4819-4820 opt — col 0 is always visible via freeze + NoHide).
                    continue if (!table_set_column_index(col) && col > 0)
                    if (col == 0) {
                        text(TABLES_HSCROLL_CELL[row * 7 + col],
                             (text = "Line {row}"))
                    } else {
                        text(TABLES_HSCROLL_CELL[row * 7 + col],
                             (text = "Hello world {col},{row}"))
                    }
                }
            }
        }

        spacing()
        text(TABLES_HSCROLL2_LABEL, (text = "Stretch + ScrollX"))

        with_id("hscroll2") {
            with_item_width(GetFontSize() * 30.0f) {
                edit_checkbox_flags(safe_addr(TABLES_HSCROLL2_FLAGS),
                    (id = "TBL_HS2_F_SCROLLX", text = "ImGuiTableFlags_ScrollX",
                     flags_value = ImGuiTableFlags.ScrollX))
                edit_drag_float(safe_addr(TABLES_HSCROLL2_INNER_WIDTH),
                    (id = "TBL_HS2_INNER_WIDTH", text = "inner_width",
                     v_speed = 1.0f, v_min = 0.0f, v_max = FLT_MAX,
                     format = "%.1f"))
            }
            data_table(TABLES_HSCROLL2_T,
                       (text = "##table_hscroll2", columns = 7,
                        flags = TABLES_HSCROLL2_FLAGS,
                        outer_size = outer_size,
                        inner_width = TABLES_HSCROLL2_INNER_WIDTH)) {
                for (cell in range(20 * 7)) {
                    table_next_column()
                    let col = table_get_column_index()
                    let row = table_get_row_index()
                    text(TABLES_HSCROLL2_CELL[cell],
                         (text = "Hello world {col},{row}"))
                }
            }
        }
    }
}

// =============================================================================
// Section 11: Columns flags — cpp:4862-4933
// =============================================================================
def private show_columns_flags() {
    apply_open_action()
    tree_node(TABLES_COLFLAGS, (text = "Columns flags",
                                flags = ImGuiTreeNodeFlags.None)) {
        let column_count = 3
        let column_names = fixed_array("One", "Two", "Three")

        // ----- Setup-table: per-column flag editors + output status display -----
        data_table(TABLES_COLFLAGS_SETUP_T,
                   (text = "##colflags_checkboxes", columns = column_count,
                    flags = ImGuiTableFlags.None,
                    outer_size = float2(0.0f, 0.0f),
                    inner_width = 0.0f)) {
            for (column in range(column_count)) {
                table_next_column()
                with_id("colflags_col_{column}") {
                    align_text_to_frame_padding(TABLES_COLFLAGS_ATFP[column])
                    text(TABLES_COLFLAGS_LABEL[column],
                         (text = "'{column_names[column]}'"))
                    spacing()
                    text(TABLES_COLFLAGS_INFLAG_LABEL[column],
                         (text = "Input flags:"))
                    edit_column_flags(unsafe(addr(TABLES_COLFLAGS_INPUT[column])))
                    spacing()
                    text(TABLES_COLFLAGS_OUTFLAG_LABEL[column],
                         (text = "Output flags:"))
                    with_disabled(true) {
                        show_column_status_flags(TABLES_COLFLAGS_OUTPUT[column])
                    }
                }
            }
        }

        // ----- Real table — body driven by the per-column flag input -----
        let flags = (ImGuiTableFlags.SizingFixedFit | ImGuiTableFlags.ScrollX |
                     ImGuiTableFlags.ScrollY | ImGuiTableFlags.RowBg |
                     ImGuiTableFlags.BordersOuter | ImGuiTableFlags.BordersV |
                     ImGuiTableFlags.Resizable | ImGuiTableFlags.Reorderable |
                     ImGuiTableFlags.Hideable | ImGuiTableFlags.Sortable)
        let line_height = GetTextLineHeightWithSpacing()
        let outer_size = float2(0.0f, line_height * 9.0f)
        let text_base_width = CalcTextSize("A").x

        data_table(TABLES_COLFLAGS_BODY_T,
                   (text = "##colflags_body", columns = column_count,
                    flags = flags,
                    outer_size = outer_size,
                    inner_width = 0.0f)) {
            var has_angled_header = false
            for (column in range(column_count)) {
                let col_flags = TABLES_COLFLAGS_INPUT[column]
                if ((TABLES_COLFLAGS_INPUT[column] &
                     ImGuiTableColumnFlags.AngledHeader) != ImGuiTableColumnFlags.None) {
                    has_angled_header = true
                }
                table_setup_column(column_names[column], col_flags)
            }
            if (has_angled_header) {
                table_angled_headers_row()
            }
            table_headers_row()
            for (column in range(column_count)) {
                TABLES_COLFLAGS_OUTPUT[column] = table_get_column_flags(column)
            }
            let indent_step = float(int(text_base_width / 2.0f))
            // with_indent pops at body exit (unlike cpp's accumulating Indent/Unindent), so each row needs an explicit (row+1)*step.
            for (row in range(8)) {
                with_indent(indent_step * float(row + 1)) {
                    table_next_row()
                    for (column in range(column_count)) {
                        table_set_column_index(column)
                        let prefix = column == 0 ? "Indented" : "Hello"
                        let cname = table_get_column_name(column)
                        text(TABLES_COLFLAGS_BODY_CELL[row * column_count + column],
                             (text = "{prefix} {cname}"))
                    }
                }
            }
        }
    }
}

// edit_column_flags — port of cpp EditTableColumnsFlags (cpp:4121-4144). 16 checkbox
// toggles; the caller wraps each in with_id("colflags_col_{column}") so ImGui's ID stack
// keeps per-column instances disjoint. Width/sort bits reproduce cpp's mutual-exclusion clearing.
def private edit_column_flags(var p_flags : ImGuiTableColumnFlags? implicit) {
    let stretch_mask = ImGuiTableColumnFlags.WidthMask_ ^ ImGuiTableColumnFlags.WidthStretch
    let fixed_mask   = ImGuiTableColumnFlags.WidthMask_ ^ ImGuiTableColumnFlags.WidthFixed

    edit_checkbox_flags(p_flags, (id = "CF_DIS", text = "_Disabled",
        flags_value = ImGuiTableColumnFlags.Disabled))
    edit_checkbox_flags(p_flags, (id = "CF_DH", text = "_DefaultHide",
        flags_value = ImGuiTableColumnFlags.DefaultHide))
    edit_checkbox_flags(p_flags, (id = "CF_DS", text = "_DefaultSort",
        flags_value = ImGuiTableColumnFlags.DefaultSort))
    if (edit_checkbox_flags(p_flags, (id = "CF_WS", text = "_WidthStretch",
        flags_value = ImGuiTableColumnFlags.WidthStretch))) {
        unsafe { *p_flags = *p_flags & ~stretch_mask; }
    }
    if (edit_checkbox_flags(p_flags, (id = "CF_WF", text = "_WidthFixed",
        flags_value = ImGuiTableColumnFlags.WidthFixed))) {
        unsafe { *p_flags = *p_flags & ~fixed_mask; }
    }
    edit_checkbox_flags(p_flags, (id = "CF_NR", text = "_NoResize",
        flags_value = ImGuiTableColumnFlags.NoResize))
    edit_checkbox_flags(p_flags, (id = "CF_NRO", text = "_NoReorder",
        flags_value = ImGuiTableColumnFlags.NoReorder))
    edit_checkbox_flags(p_flags, (id = "CF_NH", text = "_NoHide",
        flags_value = ImGuiTableColumnFlags.NoHide))
    edit_checkbox_flags(p_flags, (id = "CF_NC", text = "_NoClip",
        flags_value = ImGuiTableColumnFlags.NoClip))
    edit_checkbox_flags(p_flags, (id = "CF_NS", text = "_NoSort",
        flags_value = ImGuiTableColumnFlags.NoSort))
    edit_checkbox_flags(p_flags, (id = "CF_NSA", text = "_NoSortAscending",
        flags_value = ImGuiTableColumnFlags.NoSortAscending))
    edit_checkbox_flags(p_flags, (id = "CF_NSD", text = "_NoSortDescending",
        flags_value = ImGuiTableColumnFlags.NoSortDescending))
    edit_checkbox_flags(p_flags, (id = "CF_NHL", text = "_NoHeaderLabel",
        flags_value = ImGuiTableColumnFlags.NoHeaderLabel))
    edit_checkbox_flags(p_flags, (id = "CF_NHW", text = "_NoHeaderWidth",
        flags_value = ImGuiTableColumnFlags.NoHeaderWidth))
    edit_checkbox_flags(p_flags, (id = "CF_PSA", text = "_PreferSortAscending",
        flags_value = ImGuiTableColumnFlags.PreferSortAscending))
    edit_checkbox_flags(p_flags, (id = "CF_PSD", text = "_PreferSortDescending",
        flags_value = ImGuiTableColumnFlags.PreferSortDescending))
    edit_checkbox_flags(p_flags, (id = "CF_IE", text = "_IndentEnable",
        flags_value = ImGuiTableColumnFlags.IndentEnable))
    edit_checkbox_flags(p_flags, (id = "CF_ID", text = "_IndentDisable",
        flags_value = ImGuiTableColumnFlags.IndentDisable))
    edit_checkbox_flags(p_flags, (id = "CF_AH", text = "_AngledHeader",
        flags_value = ImGuiTableColumnFlags.AngledHeader))
}

// show_column_status_flags — read-only display of the per-frame status bits
// (cpp ShowTableColumnsStatusFlags, cpp:4146-4152). Always called inside
// with_disabled(true) so the checkboxes paint but don't accept input.
def private show_column_status_flags(flags : ImGuiTableColumnFlags) {
    var local = flags
    edit_checkbox_flags(unsafe(addr(local)),
        (id = "CSF_IE", text = "_IsEnabled",
         flags_value = ImGuiTableColumnFlags.IsEnabled))
    edit_checkbox_flags(unsafe(addr(local)),
        (id = "CSF_IV", text = "_IsVisible",
         flags_value = ImGuiTableColumnFlags.IsVisible))
    edit_checkbox_flags(unsafe(addr(local)),
        (id = "CSF_IS", text = "_IsSorted",
         flags_value = ImGuiTableColumnFlags.IsSorted))
    edit_checkbox_flags(unsafe(addr(local)),
        (id = "CSF_IH", text = "_IsHovered",
         flags_value = ImGuiTableColumnFlags.IsHovered))
}

// =============================================================================
// Section 12: Columns widths — cpp:4938-5002
// =============================================================================
def private show_columns_widths() {
    apply_open_action()
    tree_node(TABLES_CW, (text = "Columns widths",
                          flags = ImGuiTreeNodeFlags.None)) {
        // ----- Sub-table 1: TableSetupColumn default-width form -----
        edit_checkbox_flags(safe_addr(TABLES_CW1_FLAGS),
            (id = "TBL_CW1_F_RESIZABLE", text = "ImGuiTableFlags_Resizable",
             flags_value = ImGuiTableFlags.Resizable))
        edit_checkbox_flags(safe_addr(TABLES_CW1_FLAGS),
            (id = "TBL_CW1_F_NBINBODYUNTILRESIZE",
             text = "ImGuiTableFlags_NoBordersInBodyUntilResize",
             flags_value = ImGuiTableFlags.NoBordersInBodyUntilResize))

        data_table(TABLES_CW1_T,
                   (text = "##cw_table1", columns = 3,
                    flags = TABLES_CW1_FLAGS,
                    outer_size = float2(0.0f, 0.0f),
                    inner_width = 0.0f)) {
            // Could alternatively set _SizingFixedFit on the table.
            table_setup_column("one", ImGuiTableColumnFlags.WidthFixed, 100.0f)
            table_setup_column("two", ImGuiTableColumnFlags.WidthFixed, 200.0f)
            table_setup_column("three", ImGuiTableColumnFlags.WidthFixed)
            table_headers_row()
            for (row in range(4)) {
                table_next_row()
                for (col in range(3)) {
                    table_set_column_index(col)
                    let key = row * 3 + col
                    if (row == 0) {
                        let w = GetContentRegionAvail().x
                        text(TABLES_CW1_CELL[key], (text = "(w: {w})"))
                    } else {
                        text(TABLES_CW1_CELL[key],
                             (text = "Hello {col},{row}"))
                    }
                }
            }
        }

        // ----- Sub-table 2: explicit width per column (TEXT_BASE_WIDTH * N) -----
        edit_checkbox_flags(safe_addr(TABLES_CW2_FLAGS),
            (id = "TBL_CW2_F_NOKEEP", text = "ImGuiTableFlags_NoKeepColumnsVisible",
             flags_value = ImGuiTableFlags.NoKeepColumnsVisible))
        edit_checkbox_flags(safe_addr(TABLES_CW2_FLAGS),
            (id = "TBL_CW2_F_BORDERS_IV", text = "ImGuiTableFlags_BordersInnerV",
             flags_value = ImGuiTableFlags.BordersInnerV))
        edit_checkbox_flags(safe_addr(TABLES_CW2_FLAGS),
            (id = "TBL_CW2_F_BORDERS_OV", text = "ImGuiTableFlags_BordersOuterV",
             flags_value = ImGuiTableFlags.BordersOuterV))

        let text_base_width = CalcTextSize("A").x
        data_table(TABLES_CW2_T,
                   (text = "##cw_table2", columns = 4,
                    flags = TABLES_CW2_FLAGS,
                    outer_size = float2(0.0f, 0.0f),
                    inner_width = 0.0f)) {
            table_setup_column("", ImGuiTableColumnFlags.WidthFixed, 100.0f)
            table_setup_column("", ImGuiTableColumnFlags.WidthFixed,
                               text_base_width * 15.0f)
            table_setup_column("", ImGuiTableColumnFlags.WidthFixed,
                               text_base_width * 30.0f)
            table_setup_column("", ImGuiTableColumnFlags.WidthFixed,
                               text_base_width * 15.0f)
            for (row in range(5)) {
                table_next_row()
                for (col in range(4)) {
                    table_set_column_index(col)
                    let key = row * 4 + col
                    if (row == 0) {
                        let w = GetContentRegionAvail().x
                        text(TABLES_CW2_CELL[key], (text = "(w: {w})"))
                    } else {
                        text(TABLES_CW2_CELL[key],
                             (text = "Hello {col},{row}"))
                    }
                }
            }
        }
    }
}

// =============================================================================
// Section 13: Nested tables — cpp:5006-5047
// =============================================================================
def private show_nested_tables() {
    apply_open_action()
    tree_node(TABLES_NESTED, (text = "Nested tables",
                              flags = ImGuiTreeNodeFlags.None)) {
        let outer_flags = (ImGuiTableFlags.Borders | ImGuiTableFlags.Resizable |
                           ImGuiTableFlags.Reorderable | ImGuiTableFlags.Hideable)
        data_table(TABLES_NESTED_OUTER_T,
                   (text = "##nested_outer", columns = 2,
                    flags = outer_flags,
                    outer_size = float2(0.0f, 0.0f),
                    inner_width = 0.0f)) {
            table_setup_column("A0")
            table_setup_column("A1")
            table_headers_row()

            // Cell (0, 0): outer label + nested 2-col data_table.
            table_next_column()
            text(TABLES_NESTED_OUTER_CELL[0], (text = "A0 Row 0"))

            let rows_height = GetTextLineHeightWithSpacing() * 2.0f
            data_table(TABLES_NESTED_INNER_T,
                       (text = "##nested_inner", columns = 2,
                        flags = outer_flags,
                        outer_size = float2(0.0f, 0.0f),
                        inner_width = 0.0f)) {
                table_setup_column("B0")
                table_setup_column("B1")
                table_headers_row()

                table_next_row(ImGuiTableRowFlags.None, rows_height)
                table_next_column()
                text(TABLES_NESTED_INNER_CELL[0], (text = "B0 Row 0"))
                table_next_column()
                text(TABLES_NESTED_INNER_CELL[1], (text = "B1 Row 0"))
                table_next_row(ImGuiTableRowFlags.None, rows_height)
                table_next_column()
                text(TABLES_NESTED_INNER_CELL[2], (text = "B0 Row 1"))
                table_next_column()
                text(TABLES_NESTED_INNER_CELL[3], (text = "B1 Row 1"))
            }

            // Continue outer table after the nested one.
            table_next_column()
            text(TABLES_NESTED_OUTER_CELL[1], (text = "A1 Row 0"))
            table_next_column()
            text(TABLES_NESTED_OUTER_CELL[2], (text = "A0 Row 1"))
            table_next_column()
            text(TABLES_NESTED_OUTER_CELL[3], (text = "A1 Row 1"))
        }
    }
}

// =============================================================================
// Section 14: Row height — cpp:5052-5112
// =============================================================================
def private show_row_height() {
    apply_open_action()
    tree_node(TABLES_RH, (text = "Row height",
                          flags = ImGuiTreeNodeFlags.None)) {
        let line_height = GetTextLineHeightWithSpacing()

        // ----- Sub 1: min_row_height grows linearly per row -----
        data_table(TABLES_RH1_T,
                   (text = "##rh_table", columns = 1,
                    flags = ImGuiTableFlags.Borders,
                    outer_size = float2(0.0f, 0.0f),
                    inner_width = 0.0f)) {
            for (row in range(8)) {
                let min_row_height = float(int(line_height * 0.30f * float(row)))
                table_next_row(ImGuiTableRowFlags.None, min_row_height)
                table_next_column()
                text(TABLES_RH1_CELL[row],
                     (text = "min_row_height = {min_row_height}"))
            }
        }

        // ----- Sub 2: SameLine(0, 0) shares line height across cells -----

        // Two rows (ColorButton + 2-line text); row 1's SameLine(0,0) shares the previous column's line height.
        data_table(TABLES_RH2_T,
                   (text = "##rh_share", columns = 2,
                    flags = ImGuiTableFlags.Borders,
                    outer_size = float2(0.0f, 0.0f),
                    inner_width = 0.0f)) {
            let cb_color = float4(0.13f, 0.26f, 0.40f, 1.0f)
            let cb_size = float2(40.0f, 40.0f)

            table_next_row()
            table_next_column()
            color_button(TABLES_RH2_COLOR_BTN[0],
                         (desc_id = "##rh2_cb0", col = cb_color, size = cb_size,
                          flags = ImGuiColorEditFlags.None))
            table_next_column()
            text(TABLES_RH2_LINE1[0], (text = "Line 1"))
            text(TABLES_RH2_LINE2[0], (text = "Line 2"))

            table_next_row()
            table_next_column()
            color_button(TABLES_RH2_COLOR_BTN[1],
                         (desc_id = "##rh2_cb1", col = cb_color, size = cb_size,
                          flags = ImGuiColorEditFlags.None))
            table_next_column()
            // SameLine(0, 0) reuses line height from previous column.
            same_line(TABLES_RH2_SL_REUSE,
                      (offset_from_start_x = 0.0f, spacing = 0.0f))
            text(TABLES_RH2_LINE1[1], (text = "Line 1, with SameLine(0,0)"))
            text(TABLES_RH2_LINE2[1], (text = "Line 2"))
        }

        // ----- Sub 3: per-row CellPadding.y mutation (every 3rd row) -----

        // cpp Push/PopStyleVar(CellPadding) on rows where row%3==2; boost with_style is block-scoped, so wrap just that row.
        data_table(TABLES_RH3_T,
                   (text = "##rh_cellpad_y", columns = 1,
                    flags = ImGuiTableFlags.Borders,
                    outer_size = float2(0.0f, 0.0f),
                    inner_width = 0.0f)) {
            let cell_padding_x = GetStyle().CellPadding.x
            let cell_padding_y = GetStyle().CellPadding.y
            for (row in range(8)) {
                if (row % 3 == 2) {
                    let row_pad_y = 20.0f
                    with_style((ImGuiStyleVar.CellPadding,
                                float2(cell_padding_x, row_pad_y))) {
                        table_next_row(ImGuiTableRowFlags.None)
                        table_next_column()
                        text(TABLES_RH3_CELL[row],
                             (text = "CellPadding.y = {row_pad_y}"))
                    }
                } else {
                    table_next_row(ImGuiTableRowFlags.None)
                    table_next_column()
                    text(TABLES_RH3_CELL[row],
                         (text = "CellPadding.y = {cell_padding_y}"))
                }
            }
        }
    }
}

// =============================================================================
// Section 15: Outer size — cpp:5117-5179
// =============================================================================
def private show_outer_size() {
    apply_open_action()
    tree_node(TABLES_OS, (text = "Outer size",
                          flags = ImGuiTreeNodeFlags.None)) {
        text(TABLES_OS_HEADER_TEXT, (text = "Using NoHostExtendX and NoHostExtendY:"))

        edit_checkbox_flags(safe_addr(TABLES_OS1_FLAGS),
            (id = "TBL_OS1_F_NOHOSTX", text = "ImGuiTableFlags_NoHostExtendX",
             flags_value = ImGuiTableFlags.NoHostExtendX))
        edit_checkbox_flags(safe_addr(TABLES_OS1_FLAGS),
            (id = "TBL_OS1_F_NOHOSTY", text = "ImGuiTableFlags_NoHostExtendY",
             flags_value = ImGuiTableFlags.NoHostExtendY))

        let line_height = GetTextLineHeightWithSpacing()
        let outer_size_1 = float2(0.0f, line_height * 5.5f)
        data_table(TABLES_OS1_T,
                   (text = "##os_table1", columns = 3,
                    flags = TABLES_OS1_FLAGS,
                    outer_size = outer_size_1,
                    inner_width = 0.0f)) {
            for (row in range(10)) {
                table_next_row()
                for (col in range(3)) {
                    table_next_column()
                    text(TABLES_OS1_CELL[row * 3 + col],
                         (text = "Cell {col},{row}"))
                }
            }
        }
        same_line(TABLES_OS1_SL)
        text(TABLES_OS1_HELLO, (text = "Hello!"))

        spacing()
        text(TABLES_OS_EXPLICIT_HEADER, (text = "Using explicit size:"))

        let text_base_width = CalcTextSize("A").x
        let explicit_size = float2(text_base_width * 30.0f, 0.0f)
        let table_2_3_flags = (ImGuiTableFlags.Borders | ImGuiTableFlags.RowBg)

        data_table(TABLES_OS2_T,
                   (text = "##os_table2", columns = 3,
                    flags = table_2_3_flags,
                    outer_size = explicit_size,
                    inner_width = 0.0f)) {
            for (row in range(5)) {
                table_next_row()
                for (col in range(3)) {
                    table_next_column()
                    text(TABLES_OS2_CELL[row * 3 + col],
                         (text = "Cell {col},{row}"))
                }
            }
        }
        same_line(TABLES_OS_SL_2_3)
        data_table(TABLES_OS3_T,
                   (text = "##os_table3", columns = 3,
                    flags = table_2_3_flags,
                    outer_size = explicit_size,
                    inner_width = 0.0f)) {
            for (row in range(3)) {
                table_next_row(ImGuiTableRowFlags.None, line_height * 1.5f)
                for (col in range(3)) {
                    table_next_column()
                    text(TABLES_OS3_CELL[row * 3 + col],
                         (text = "Cell {col},{row}"))
                }
            }
        }
    }
}

// =============================================================================
// Section 16: Background color — cpp:5184-5237
// =============================================================================
def private show_background_color() {
    apply_open_action()
    tree_node(TABLES_BG, (text = "Background color",
                          flags = ImGuiTreeNodeFlags.None)) {
        edit_checkbox_flags(safe_addr(TABLES_BG_FLAGS),
            (id = "TBL_BG_F_BORDERS", text = "ImGuiTableFlags_Borders",
             flags_value = ImGuiTableFlags.Borders))
        edit_checkbox_flags(safe_addr(TABLES_BG_FLAGS),
            (id = "TBL_BG_F_ROWBG", text = "ImGuiTableFlags_RowBg",
             flags_value = ImGuiTableFlags.RowBg))

        var row_bg_items <- ["None", "Red", "Gradient"]
        edit_combo(safe_addr(TABLES_BG_ROW_BG_TYPE),
                   (id = "TBL_BG_ROW_BG_TYPE", text = "row bg type",
                    items = row_bg_items))
        delete row_bg_items
        var row_bg_target_items <- ["RowBg0", "RowBg1"]
        edit_combo(safe_addr(TABLES_BG_ROW_BG_TARGET),
                   (id = "TBL_BG_ROW_BG_TARGET", text = "row bg target",
                    items = row_bg_target_items))
        delete row_bg_target_items
        var cell_bg_items <- ["None", "Blue"]
        edit_combo(safe_addr(TABLES_BG_CELL_BG_TYPE),
                   (id = "TBL_BG_CELL_BG_TYPE", text = "cell bg type",
                    items = cell_bg_items))
        delete cell_bg_items

        // Pre-tabled charpair labels — daslang has no int→char, so we can't do cpp's Text("%c%c", 'A'+row, '0'+col).
        let row_letters = fixed_array("A", "B", "C", "D", "E", "F")
        let col_digits = fixed_array("0", "1", "2", "3", "4")

        data_table(TABLES_BG_T, (text = "##bg_table", columns = 5,
                                 flags = TABLES_BG_FLAGS,
                                 outer_size = float2(0.0f, 0.0f),
                                 inner_width = 0.0f)) {
            for (row in range(6)) {
                table_next_row()
                if (TABLES_BG_ROW_BG_TYPE != 0) {
                    // Flat red vs gradient grey (cpp:5213), translucent so RowBg0/RowBg1 ordering is visible.
                    let row_bg_color = (TABLES_BG_ROW_BG_TYPE == 1
                        ? float4(0.7f, 0.3f, 0.3f, 0.65f)
                        : float4(0.2f + float(row) * 0.1f, 0.2f, 0.2f, 0.65f))
                    // ImGuiTableBgTarget.RowBg0 + N — the enum is int-castable, so the offset arithmetic is direct.
                    let target = unsafe(reinterpret<ImGuiTableBgTarget>(
                        int(ImGuiTableBgTarget.RowBg0) + TABLES_BG_ROW_BG_TARGET))
                    table_set_bg_color(target, GetColorU32(row_bg_color))
                }
                for (col in range(5)) {
                    table_set_column_index(col)
                    let key = row * 5 + col
                    text(TABLES_BG_CELL[key],
                         (text = "{row_letters[row]}{col_digits[col]}"))
                    // Paint cells B1->C2 in blue cell-bg when cell_bg_type == 1.
                    if (row >= 1 && row <= 2 && col >= 1 && col <= 2 &&
                            TABLES_BG_CELL_BG_TYPE == 1) {
                        let cell_bg_color = float4(0.3f, 0.3f, 0.7f, 0.65f)
                        table_set_bg_color(ImGuiTableBgTarget.CellBg,
                                           GetColorU32(cell_bg_color))
                    }
                }
            }
        }
    }
}

// =============================================================================
// Section 17: Tree view — cpp:5242-5315
// =============================================================================

// Recursive descent through TREE_VIEW_NODES, mirroring cpp's MyTreeNode::DisplayNode.
// Folder (child_count>0) opens a tree_node_ex and recurses; file is a leaf tree_node_ex
// with Leaf|Bullet|NoTreePushOnOpen, so there's no paired tree_pop().
def private display_tree_view_node(node_idx : int) {
    let node = TREE_VIEW_NODES[node_idx]
    table_next_row()
    table_next_column()
    let is_folder = node.child_count > 0
    let tree_flags = TABLES_TREE_NODE_FLAGS
    if (is_folder) {
        let opened = tree_node_ex(TABLES_TREE_NODE[node_idx],
                                  (text = node.name, flags = tree_flags))
        table_next_column()
        text_disabled(TABLES_TREE_SIZE_CELL[node_idx], "--")
        table_next_column()
        text(TABLES_TREE_TYPE_CELL[node_idx], (text = node.node_type))
        if (opened) {
            for (child_n in range(node.child_count)) {
                display_tree_view_node(node.child_idx + child_n)
            }
            tree_pop()
        }
    } else {
        let leaf_flags = (TABLES_TREE_NODE_FLAGS | ImGuiTreeNodeFlags.Leaf |
                          ImGuiTreeNodeFlags.Bullet |
                          ImGuiTreeNodeFlags.NoTreePushOnOpen)
        tree_node_ex(TABLES_TREE_NODE_LEAF[node_idx],
                     (text = node.name, flags = leaf_flags))
        table_next_column()
        let sz = node.size
        text(TABLES_TREE_SIZE_CELL[node_idx], (text = "{sz}"))
        table_next_column()
        text(TABLES_TREE_TYPE_CELL[node_idx], (text = node.node_type))
    }
}

def private show_tree_view() {
    apply_open_action()
    tree_node(TABLES_TREE, (text = "Tree view",
                            flags = ImGuiTreeNodeFlags.None)) {
        edit_checkbox_flags(safe_addr(TABLES_TREE_NODE_FLAGS),
            (id = "TBL_TREE_F_SPANFW", text = "ImGuiTreeNodeFlags_SpanFullWidth",
             flags_value = ImGuiTreeNodeFlags.SpanFullWidth))
        edit_checkbox_flags(safe_addr(TABLES_TREE_NODE_FLAGS),
            (id = "TBL_TREE_F_SPANTW", text = "ImGuiTreeNodeFlags_SpanLabelWidth",
             flags_value = ImGuiTreeNodeFlags.SpanLabelWidth))
        edit_checkbox_flags(safe_addr(TABLES_TREE_NODE_FLAGS),
            (id = "TBL_TREE_F_SPANAC", text = "ImGuiTreeNodeFlags_SpanAllColumns",
             flags_value = ImGuiTreeNodeFlags.SpanAllColumns))

        let text_base_width = CalcTextSize("A").x
        data_table(TABLES_TREE_T,
                   (text = "##tree_3ways", columns = 3,
                    flags = TABLES_TREE_FLAGS,
                    outer_size = float2(0.0f, 0.0f),
                    inner_width = 0.0f)) {
            table_setup_column("Name", ImGuiTableColumnFlags.NoHide)
            table_setup_column("Size", ImGuiTableColumnFlags.WidthFixed,
                               text_base_width * 12.0f)
            table_setup_column("Type", ImGuiTableColumnFlags.WidthFixed,
                               text_base_width * 18.0f)
            table_headers_row()
            display_tree_view_node(0)
        }
    }
}

// =============================================================================
// Section 18: Item width — cpp:5320-5361
// =============================================================================
def private show_item_width() {
    apply_open_action()
    tree_node(TABLES_IW, (text = "Item width",
                          flags = ImGuiTreeNodeFlags.None)) {
        let text_base_width = CalcTextSize("A").x
        data_table(TABLES_IW_T,
                   (text = "##table_item_width", columns = 3,
                    flags = ImGuiTableFlags.Borders,
                    outer_size = float2(0.0f, 0.0f),
                    inner_width = 0.0f)) {
            table_setup_column("small")
            table_setup_column("half")
            table_setup_column("right-align")
            table_headers_row()

            for (row in range(3)) {
                table_next_row()
                // Boost forbids raw PushItemWidth (IMGUI002) and with_item_width is block-scoped, so call SetNextItemWidth per-slider instead.
                with_id("iw_row_{row}") {
                    table_set_column_index(0)
                    SetNextItemWidth(text_base_width * 3.0f)
                    slider_float(TABLES_ITEM_WIDTH_SLIDER[row * 3 + 0],
                                 (text = "float0"))
                    table_set_column_index(1)
                    SetNextItemWidth(-GetContentRegionAvail().x * 0.5f)
                    slider_float(TABLES_ITEM_WIDTH_SLIDER[row * 3 + 1],
                                 (text = "float1"))
                    table_set_column_index(2)
                    SetNextItemWidth(-FLT_MIN)
                    slider_float(TABLES_ITEM_WIDTH_SLIDER[row * 3 + 2],
                                 (text = "##float2"))
                }
            }
        }
    }
}

// =============================================================================
// Section 19: Custom headers — cpp:5367-5409
// =============================================================================
def private show_custom_headers() {
    apply_open_action()
    tree_node(TABLES_CHDR, (text = "Custom headers",
                            flags = ImGuiTreeNodeFlags.None)) {
        let column_count = 3
        let column_names = fixed_array("Apricot", "Banana", "Cherry")
        let flags = (ImGuiTableFlags.Borders | ImGuiTableFlags.Reorderable |
                     ImGuiTableFlags.Hideable)
        data_table(TABLES_CHDR_T,
                   (text = "##table_custom_headers", columns = column_count,
                    flags = flags,
                    outer_size = float2(0.0f, 0.0f),
                    inner_width = 0.0f)) {
            for (n in range(column_count)) {
                table_setup_column(column_names[n])
            }
            // Custom-header row instead of table_headers_row() — per cell: tight-padded Checkbox + SameLine + TableHeader(name).
            table_next_row(ImGuiTableRowFlags.Headers)
            for (column in range(column_count)) {
                table_set_column_index(column)
                let column_name = table_get_column_name(column)
                with_id("chdr_col_{column}") {
                    with_style((ImGuiStyleVar.FramePadding,
                                float2(0.0f, 0.0f))) {
                        edit_checkbox(unsafe(addr(TABLES_CHDR_SELECTED[column])),
                                      (id = "CHDR_CHECKALL", text = "##checkall"))
                    }
                    same_line(TABLES_CHDR_SL_CHK,
                              (offset_from_start_x = 0.0f,
                               spacing = GetStyle().ItemInnerSpacing.x))
                    table_header(column_name)
                }
            }
            // Body — 3*5 selectable cells; boost `selectable_label(state, text, selected)` is cpp's caller-owned-bool Selectable.
            for (row in range(5)) {
                table_next_row()
                for (column in range(column_count)) {
                    table_set_column_index(column)
                    let key = row * column_count + column
                    selectable_label(TABLES_CHDR_CELL[key],
                                     "Cell {column},{row}",
                                     TABLES_CHDR_SELECTED[column])
                }
            }
        }
    }
}

// =============================================================================
// Section 20: Angled headers — cpp:5414-5476
// =============================================================================
def private show_angled_headers() {
    apply_open_action()
    tree_node(TABLES_AH, (text = "Angled headers",
                          flags = ImGuiTreeNodeFlags.None)) {
        edit_checkbox_flags(safe_addr(TABLES_AH_TABLE_FLAGS),
            (id = "TBL_AH_F_SCROLLX", text = "_ScrollX",
             flags_value = ImGuiTableFlags.ScrollX))
        edit_checkbox_flags(safe_addr(TABLES_AH_TABLE_FLAGS),
            (id = "TBL_AH_F_SCROLLY", text = "_ScrollY",
             flags_value = ImGuiTableFlags.ScrollY))
        edit_checkbox_flags(safe_addr(TABLES_AH_TABLE_FLAGS),
            (id = "TBL_AH_F_RESIZABLE", text = "_Resizable",
             flags_value = ImGuiTableFlags.Resizable))
        edit_checkbox_flags(safe_addr(TABLES_AH_TABLE_FLAGS),
            (id = "TBL_AH_F_NBINBODY", text = "_NoBordersInBody",
             flags_value = ImGuiTableFlags.NoBordersInBody))
        edit_checkbox_flags(safe_addr(TABLES_AH_TABLE_FLAGS),
            (id = "TBL_AH_F_HIGHLIGHTHC", text = "_HighlightHoveredColumn",
             flags_value = ImGuiTableFlags.HighlightHoveredColumn))

        SetNextItemWidth(GetFontSize() * 8.0f)
        edit_slider_int(safe_addr(TABLES_AH_FROZEN_COLS),
            (id = "TBL_AH_FROZEN_COLS", text = "Frozen columns",
             v_min = 0, v_max = 2))
        SetNextItemWidth(GetFontSize() * 8.0f)
        edit_slider_int(safe_addr(TABLES_AH_FROZEN_ROWS),
            (id = "TBL_AH_FROZEN_ROWS", text = "Frozen rows",
             v_min = 0, v_max = 2))

        edit_checkbox_flags(safe_addr(TABLES_AH_COLUMN_FLAGS),
            (id = "TBL_AH_CF_NOHEADERWIDTH",
             text = "Disable header contributing to column width",
             flags_value = ImGuiTableColumnFlags.NoHeaderWidth))

        // Style settings sub-node — angle + text-align sliders reach into shared ImGuiStyle, so wrap pointer access in unsafe(addr(...)).
        tree_node(TABLES_AH_STYLE_NODE,
                  (text = "Style settings", flags = ImGuiTreeNodeFlags.None)) {
            SetNextItemWidth(GetFontSize() * 8.0f)
            // SliderAngle shows degrees in the UI but the style field is radians.
            edit_slider_angle(
                unsafe(addr(GetStyle().TableAngledHeadersAngle)),
                (id = "TBL_AH_STYLE_ANGLE", text = "style.TableAngledHeadersAngle",
                 v_degrees_min = -50.0f, v_degrees_max = 50.0f))
            SetNextItemWidth(GetFontSize() * 8.0f)
            edit_slider_float2(
                unsafe(addr(GetStyle().TableAngledHeadersTextAlign)),
                (id = "TBL_AH_STYLE_ALIGN",
                 text = "style.TableAngledHeadersTextAlign",
                 v_min = 0.0f, v_max = 1.0f, format = "%.2f"))
        }

        let line_height = GetTextLineHeightWithSpacing()
        data_table(TABLES_AH_T,
                   (text = "##table_angled_headers", columns = TABLES_AH_COLUMNS_COUNT,
                    flags = TABLES_AH_TABLE_FLAGS,
                    outer_size = float2(0.0f, line_height * 12.0f),
                    inner_width = 0.0f)) {
            // Track column (col 0) — no-hide/no-reorder; the rest share the angled-header flag set.
            table_setup_column(TABLES_AH_COLUMN_NAMES[0],
                               ImGuiTableColumnFlags.NoHide |
                               ImGuiTableColumnFlags.NoReorder)
            let col_flags = TABLES_AH_COLUMN_FLAGS
            for (n in range(1, TABLES_AH_COLUMNS_COUNT)) {
                table_setup_column(TABLES_AH_COLUMN_NAMES[n], col_flags)
            }
            table_setup_scroll_freeze(TABLES_AH_FROZEN_COLS, TABLES_AH_FROZEN_ROWS)

            table_angled_headers_row()
            table_headers_row()

            for (row in range(TABLES_AH_ROWS_COUNT)) {
                with_id("ah_row_{row}") {
                    table_next_row()
                    table_set_column_index(0)
                    align_text_to_frame_padding(TABLES_AH_TRACK_ATFP[row])
                    text(TABLES_AH_TRACK_LABEL[row], (text = "Track {row}"))
                    for (column in range(1, TABLES_AH_COLUMNS_COUNT)) {
                        continue if (!table_set_column_index(column))
                        with_id("ah_col_{column}") {
                            let key = row * TABLES_AH_COLUMNS_COUNT + column
                            edit_checkbox(unsafe(addr(TABLES_AH_BOOLS[key])),
                                          (id = "AH_CB", text = ""))
                        }
                    }
                }
            }
        }
    }
}

// =============================================================================
// Section 21: Context menus — cpp:5483-5587
// =============================================================================
def private show_context_menus() {
    apply_open_action()
    tree_node(TABLES_CM, (text = "Context menus",
                          flags = ImGuiTreeNodeFlags.None)) {
        // ----- Sub-table 1: ContextMenuInBody flag toggle -----

        // Right-click the headers row (or the body, with the flag) opens the default table context menu; no custom popup needed.
        edit_checkbox_flags(safe_addr(TABLES_CM1_FLAGS),
            (id = "TBL_CM1_F_CTXINBODY", text = "ImGuiTableFlags_ContextMenuInBody",
             flags_value = ImGuiTableFlags.ContextMenuInBody))

        let column_count = 3
        data_table(TABLES_CM1_T,
                   (text = "##table_context_menu", columns = column_count,
                    flags = TABLES_CM1_FLAGS,
                    outer_size = float2(0.0f, 0.0f),
                    inner_width = 0.0f)) {
            table_setup_column("One")
            table_setup_column("Two")
            table_setup_column("Three")
            table_headers_row()
            for (row in range(4)) {
                table_next_row()
                for (column in range(column_count)) {
                    table_set_column_index(column)
                    text(TABLES_CM1_CELL[row * column_count + column],
                         (text = "Cell {column},{row}"))
                }
            }
        }

        // ----- Sub-table 2: custom per-cell + per-column popups -----
        let flags2 = (ImGuiTableFlags.Resizable | ImGuiTableFlags.SizingFixedFit |
                      ImGuiTableFlags.Reorderable | ImGuiTableFlags.Hideable |
                      ImGuiTableFlags.Borders)
        data_table(TABLES_CM2_T,
                   (text = "##table_context_menu_2", columns = column_count,
                    flags = flags2,
                    outer_size = float2(0.0f, 0.0f),
                    inner_width = 0.0f)) {
            table_setup_column("One")
            table_setup_column("Two")
            table_setup_column("Three")
            table_headers_row()
            for (row in range(4)) {
                table_next_row()
                for (column in range(column_count)) {
                    table_set_column_index(column)
                    let key = row * column_count + column
                    text(TABLES_CM2_CELL_TEXT[key],
                         (text = "Cell {column},{row}"))
                    same_line(TABLES_CM2_SL[key])
                    with_id("cm2_cell_{row}_{column}") {
                        small_button(TABLES_CM2_DOT_BTN[key], (text = ".."))
                        popup_context_item(TABLES_CM2_DOT_POPUP[key],
                            (str_id = "", flags = ImGuiPopupFlags.MouseButtonRight)) {
                            text(TABLES_CM2_DOT_TEXT[key],
                                 (text = "This is the popup for Button(\"..\") in Cell {column},{row}"))
                            if (button(TABLES_CM2_DOT_CLOSE[key], (text = "Close"))) {
                                close_current_popup()
                            }
                        }
                    }
                }
            }

            // Per-column right-click popup: all iters share str_id "MyPopup"; with_id(column) scopes ImGui's popup-id stack so each column owns its open state.
            var hovered = -1
            for (column in range(column_count + 1)) {
                with_id("cm2_col_{column}") {
                    if ((table_get_column_flags(column) &
                         ImGuiTableColumnFlags.IsHovered) != ImGuiTableColumnFlags.None) {
                        hovered = column
                    }
                    if (hovered == column && !IsAnyItemHovered() &&
                        IsMouseReleased(ImGuiMouseButton.Right)) {
                        open_popup("MyPopup")
                    }
                    popup_window(TABLES_CM2_COL_POPUP[column],
                                 (str_id = "MyPopup",
                                  flags = ImGuiWindowFlags.None)) {
                        if (column == column_count) {
                            text(TABLES_CM2_COL_TEXT[column],
                                 (text = "This is a custom popup for unused space after the last column."))
                        } else {
                            text(TABLES_CM2_COL_TEXT[column],
                                 (text = "This is a custom popup for Column {column}"))
                        }
                        if (button(TABLES_CM2_COL_CLOSE[column], (text = "Close"))) {
                            close_current_popup()
                        }
                    }
                }
            }
            TABLES_CM2_HOVERED_COLUMN = hovered
        }
        text(TABLES_CM2_HOVERED_TEXT,
             (text = "Hovered column: {TABLES_CM2_HOVERED_COLUMN}"))
    }
}

// =============================================================================
// Section 22: Synced instances — cpp:5594-5624
// =============================================================================
def private show_synced_instances() {
    apply_open_action()
    tree_node(TABLES_SYNCED, (text = "Synced instances",
                              flags = ImGuiTreeNodeFlags.None)) {
        edit_checkbox_flags(safe_addr(TABLES_SYNCED_FLAGS),
            (id = "TBL_SYNCED_F_RESIZABLE", text = "ImGuiTableFlags_Resizable",
             flags_value = ImGuiTableFlags.Resizable))
        edit_checkbox_flags(safe_addr(TABLES_SYNCED_FLAGS),
            (id = "TBL_SYNCED_F_SCROLLY", text = "ImGuiTableFlags_ScrollY",
             flags_value = ImGuiTableFlags.ScrollY))
        edit_checkbox_flags(safe_addr(TABLES_SYNCED_FLAGS),
            (id = "TBL_SYNCED_F_FIXEDFIT", text = "ImGuiTableFlags_SizingFixedFit",
             flags_value = ImGuiTableFlags.SizingFixedFit))
        edit_checkbox_flags(safe_addr(TABLES_SYNCED_FLAGS),
            (id = "TBL_SYNCED_F_HIGHLIGHTHC",
             text = "ImGuiTableFlags_HighlightHoveredColumn",
             flags_value = ImGuiTableFlags.HighlightHoveredColumn))

        let line_height = GetTextLineHeightWithSpacing()
        let tflags = TABLES_SYNCED_FLAGS
        // Synced instances need the SAME ImGui ID, so all 3 iters share one IDENT (TABLES_SYNCED_T); only CollapsingHeader state is per-iter (keyed by n).
        for (n in range(3)) {
            let buf = "Synced Table {n}"
            collapsing_header(TABLES_SYNCED_HEADER[n],
                              (text = buf, closable = false,
                               flags = ImGuiTreeNodeFlags.DefaultOpen)) {
                data_table(TABLES_SYNCED_T,
                           (text = "Table", columns = 3, flags = tflags,
                            outer_size = float2(0.0f, line_height * 5.0f),
                            inner_width = 0.0f)) {
                    table_setup_column("One")
                    table_setup_column("Two")
                    table_setup_column("Three")
                    table_headers_row()
                    let cell_count = (n == 1 ? 27 : 9)
                    for (cell in range(cell_count)) {
                        table_next_column()
                        text(TABLES_SYNCED_CELL[n * 32 + cell],
                             (text = "this cell {cell}"))
                    }
                }
            }
        }
    }
}

// =============================================================================
// Section 23: Sorting — cpp:5637-5713
// =============================================================================
def private show_sorting() {
    apply_open_action()
    tree_node(TABLES_SORT, (text = "Sorting", flags = ImGuiTreeNodeFlags.None)) {
        edit_checkbox_flags(safe_addr(TABLES_SORT_FLAGS),
            (id = "TBL_SORT_F_SORTMULTI", text = "ImGuiTableFlags_SortMulti",
             flags_value = ImGuiTableFlags.SortMulti))
        edit_checkbox_flags(safe_addr(TABLES_SORT_FLAGS),
            (id = "TBL_SORT_F_SORTTRISTATE", text = "ImGuiTableFlags_SortTristate",
             flags_value = ImGuiTableFlags.SortTristate))

        let line_height = GetTextLineHeightWithSpacing()
        data_table(TABLES_SORT_T,
                   (text = "##table_sorting", columns = 4,
                    flags = TABLES_SORT_FLAGS,
                    outer_size = float2(0.0f, line_height * 15.0f),
                    inner_width = 0.0f)) {
            // user_id (cpp:5675-5678) lets the sort comparator distinguish columns by id instead of index.
            table_setup_column("ID",
                ImGuiTableColumnFlags.DefaultSort | ImGuiTableColumnFlags.WidthFixed,
                0.0f, TABLES_SORT_COL_ID)
            table_setup_column("Name",
                ImGuiTableColumnFlags.WidthFixed,
                0.0f, TABLES_SORT_COL_NAME)
            table_setup_column("Action",
                ImGuiTableColumnFlags.NoSort | ImGuiTableColumnFlags.WidthFixed,
                0.0f, TABLES_SORT_COL_ACTION)
            table_setup_column("Quantity",
                ImGuiTableColumnFlags.PreferSortDescending |
                ImGuiTableColumnFlags.WidthStretch,
                0.0f, TABLES_SORT_COL_QUANTITY)
            table_setup_scroll_freeze(0, 1)
            table_headers_row()

            // Sort when ImGui reports specs dirty; the multi-key comparator walks specs in order, ColumnUserID picks the column, Asc/Desc flips the sign.
            sort_specs() $(specs) {
                TABLES_SORT_ITEMS |> sort() $(a, b) {
                    for (spec in specs) {
                        var delta = 0
                        if (spec.column_user_id == TABLES_SORT_COL_ID) {
                            delta = a.id - b.id
                        } elif (spec.column_user_id == TABLES_SORT_COL_NAME) {
                            delta = a.name < b.name ? -1 : (a.name > b.name ? 1 : 0)
                        } elif (spec.column_user_id == TABLES_SORT_COL_QUANTITY) {
                            delta = a.quantity - b.quantity
                        }
                        if (delta != 0) {
                            let asc = spec.sort_direction == ImGuiSortDirection.Ascending
                            return asc ? delta < 0 : delta > 0
                        }
                    }
                    // qsort isn't stable; fall back on ID so the order is deterministic.
                    return a.id < b.id
                }
            }

            list_clipper(length(TABLES_SORT_ITEMS)) $(start, end_) {
                for (row_n in range(start, end_)) {
                    let item = TABLES_SORT_ITEMS[row_n]
                    let item_id = item.id
                    with_id("sort_item_{item_id}") {
                        table_next_row()
                        table_next_column()
                        text(TABLES_SORT_ID_TEXT[item_id],
                             (text = "{item_id}"))
                        table_next_column()
                        text(TABLES_SORT_NAME_TEXT[item_id],
                             (text = item.name))
                        table_next_column()
                        small_button(TABLES_SORT_ACTION_BTN[item_id],
                                     (text = "None"))
                        table_next_column()
                        let qty = item.quantity
                        text(TABLES_SORT_QTY_TEXT[item_id],
                             (text = "{qty}"))
                    }
                }
            }
        }
    }
}

// =============================================================================
// Section 24: Advanced — cpp:5713-6033
// =============================================================================

// The heaviest demo: nearly every table flag, all contents-types, virtualized data,
// ID-keyed state, Ctrl+click multi-select, quantity-edit-triggers-resort.
// Skipped from cpp: PushStyleCompact/PopStyleCompact + all HelpMarker bubbles (file-wide policy).
def private show_advanced() {
    apply_open_action()
    tree_node(TABLES_ADV, (text = "Advanced",
                           flags = ImGuiTreeNodeFlags.None)) {
        // ---- Options sub-tree (cpp:5746-5864) ----

        // Per-cell SetNextItemWidth(28*width) before each edit_drag_* — boost edit_* widgets don't honor PushItemWidth uniformly.
        let text_base_width = CalcTextSize("A").x
        let item_width = text_base_width * 28.0f

        if (tree_node_ex(TABLES_ADV_OPTIONS,
                         (text = "Options",
                          flags = ImGuiTreeNodeFlags.DefaultOpen))) {
            // ---- Features sub-node (cpp:5752-5761) ----
            if (tree_node_ex(TABLES_ADV_F_NODE,
                             (text = "Features:",
                              flags = ImGuiTreeNodeFlags.DefaultOpen))) {
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_RESIZABLE", text = "ImGuiTableFlags_Resizable",
                     flags_value = ImGuiTableFlags.Resizable))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_REORDERABLE", text = "ImGuiTableFlags_Reorderable",
                     flags_value = ImGuiTableFlags.Reorderable))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_HIDEABLE", text = "ImGuiTableFlags_Hideable",
                     flags_value = ImGuiTableFlags.Hideable))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_SORTABLE", text = "ImGuiTableFlags_Sortable",
                     flags_value = ImGuiTableFlags.Sortable))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_NOSAVED", text = "ImGuiTableFlags_NoSavedSettings",
                     flags_value = ImGuiTableFlags.NoSavedSettings))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_CTXMENU", text = "ImGuiTableFlags_ContextMenuInBody",
                     flags_value = ImGuiTableFlags.ContextMenuInBody))
                tree_pop()
            }

            // ---- Decorations sub-node (cpp:5763-5775) ----
            if (tree_node_ex(TABLES_ADV_D_NODE,
                             (text = "Decorations:",
                              flags = ImGuiTreeNodeFlags.DefaultOpen))) {
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_ROWBG", text = "ImGuiTableFlags_RowBg",
                     flags_value = ImGuiTableFlags.RowBg))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_BORDERSV", text = "ImGuiTableFlags_BordersV",
                     flags_value = ImGuiTableFlags.BordersV))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_BORDERSOV", text = "ImGuiTableFlags_BordersOuterV",
                     flags_value = ImGuiTableFlags.BordersOuterV))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_BORDERSIV", text = "ImGuiTableFlags_BordersInnerV",
                     flags_value = ImGuiTableFlags.BordersInnerV))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_BORDERSH", text = "ImGuiTableFlags_BordersH",
                     flags_value = ImGuiTableFlags.BordersH))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_BORDERSOH", text = "ImGuiTableFlags_BordersOuterH",
                     flags_value = ImGuiTableFlags.BordersOuterH))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_BORDERSIH", text = "ImGuiTableFlags_BordersInnerH",
                     flags_value = ImGuiTableFlags.BordersInnerH))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_NOBORDERSBODY", text = "ImGuiTableFlags_NoBordersInBody",
                     flags_value = ImGuiTableFlags.NoBordersInBody))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_NOBORDERSBODYUR", text = "ImGuiTableFlags_NoBordersInBodyUntilResize",
                     flags_value = ImGuiTableFlags.NoBordersInBodyUntilResize))
                tree_pop()
            }

            // ---- Sizing sub-node (cpp:5777-5792) ----
            if (tree_node_ex(TABLES_ADV_S_NODE,
                             (text = "Sizing:",
                              flags = ImGuiTreeNodeFlags.DefaultOpen))) {
                SetNextItemWidth(item_width)
                edit_table_sizing_flags(safe_addr(TABLES_ADV_FLAGS),
                                        "tbl_adv_sizing_combo")
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_NOHOSTX", text = "ImGuiTableFlags_NoHostExtendX",
                     flags_value = ImGuiTableFlags.NoHostExtendX))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_NOHOSTY", text = "ImGuiTableFlags_NoHostExtendY",
                     flags_value = ImGuiTableFlags.NoHostExtendY))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_NOKEEPCV", text = "ImGuiTableFlags_NoKeepColumnsVisible",
                     flags_value = ImGuiTableFlags.NoKeepColumnsVisible))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_PRECISE", text = "ImGuiTableFlags_PreciseWidths",
                     flags_value = ImGuiTableFlags.PreciseWidths))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_NOCLIP", text = "ImGuiTableFlags_NoClip",
                     flags_value = ImGuiTableFlags.NoClip))
                tree_pop()
            }

            // ---- Padding sub-node (cpp:5794-5800) ----
            if (tree_node_ex(TABLES_ADV_P_NODE,
                             (text = "Padding:",
                              flags = ImGuiTreeNodeFlags.DefaultOpen))) {
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_PADOUTERX", text = "ImGuiTableFlags_PadOuterX",
                     flags_value = ImGuiTableFlags.PadOuterX))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_NOPADOUTERX", text = "ImGuiTableFlags_NoPadOuterX",
                     flags_value = ImGuiTableFlags.NoPadOuterX))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_NOPADINNERX", text = "ImGuiTableFlags_NoPadInnerX",
                     flags_value = ImGuiTableFlags.NoPadInnerX))
                tree_pop()
            }

            // ---- Scrolling sub-node (cpp:5802-5813) ----
            if (tree_node_ex(TABLES_ADV_SC_NODE,
                             (text = "Scrolling:",
                              flags = ImGuiTreeNodeFlags.DefaultOpen))) {
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_SCROLLX", text = "ImGuiTableFlags_ScrollX",
                     flags_value = ImGuiTableFlags.ScrollX))
                same_line(TABLES_ADV_SL_FCOLS)
                SetNextItemWidth(GetFrameHeight())
                edit_drag_int(safe_addr(TABLES_ADV_FREEZE_COLS),
                    (id = "TBL_ADV_FREEZE_COLS", text = "freeze_cols",
                     speed = 0.2f, v_min = 0, v_max = 9, format = "%d",
                     flags = ImGuiSliderFlags.NoInput))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_SCROLLY", text = "ImGuiTableFlags_ScrollY",
                     flags_value = ImGuiTableFlags.ScrollY))
                same_line(TABLES_ADV_SL_FROWS)
                SetNextItemWidth(GetFrameHeight())
                edit_drag_int(safe_addr(TABLES_ADV_FREEZE_ROWS),
                    (id = "TBL_ADV_FREEZE_ROWS", text = "freeze_rows",
                     speed = 0.2f, v_min = 0, v_max = 9, format = "%d",
                     flags = ImGuiSliderFlags.NoInput))
                tree_pop()
            }

            // ---- Sorting sub-node (cpp:5815-5822) ----
            if (tree_node_ex(TABLES_ADV_SO_NODE,
                             (text = "Sorting:",
                              flags = ImGuiTreeNodeFlags.DefaultOpen))) {
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_SORTMULTI", text = "ImGuiTableFlags_SortMulti",
                     flags_value = ImGuiTableFlags.SortMulti))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_SORTTRISTATE", text = "ImGuiTableFlags_SortTristate",
                     flags_value = ImGuiTableFlags.SortTristate))
                tree_pop()
            }

            // ---- Headers sub-node (cpp:5824-5831) ----
            if (tree_node_ex(TABLES_ADV_H_NODE,
                             (text = "Headers:",
                              flags = ImGuiTreeNodeFlags.DefaultOpen))) {
                edit_checkbox(safe_addr(TABLES_ADV_SHOW_HEADERS),
                              (id = "TBL_ADV_SHOW_HEADERS", text = "show_headers"))
                edit_checkbox_flags(safe_addr(TABLES_ADV_FLAGS),
                    (id = "TBL_ADV_F_HHC", text = "ImGuiTableFlags_HighlightHoveredColumn",
                     flags_value = ImGuiTableFlags.HighlightHoveredColumn))
                edit_checkbox_flags(safe_addr(TABLES_ADV_COLUMNS_BASE_FLAGS),
                    (id = "TBL_ADV_CBF_AH", text = "ImGuiTableColumnFlags_AngledHeader",
                     flags_value = ImGuiTableColumnFlags.AngledHeader))
                tree_pop()
            }

            // ---- Other sub-node (cpp:5833-5858) ----
            if (tree_node_ex(TABLES_ADV_O_NODE,
                             (text = "Other:",
                              flags = ImGuiTreeNodeFlags.DefaultOpen))) {
                edit_checkbox(safe_addr(TABLES_ADV_SHOW_WRAPPED_TEXT),
                              (id = "TBL_ADV_SHOW_WRAPPED",
                               text = "show_wrapped_text"))
                // OuterSize: DragFloat2 paired with the enable checkbox via SameLine + GetStyle().ItemInnerSpacing.x (cpp:5837-5839).
                SetNextItemWidth(item_width)
                edit_drag_float2(safe_addr(TABLES_ADV_OUTER_SIZE_VALUE),
                                 (id = "TBL_ADV_OUTERSIZE",
                                  text = "##OuterSize"))
                same_line(TABLES_ADV_SL_OUTERSIZE,
                          (offset_from_start_x = 0.0f,
                           spacing = GetStyle().ItemInnerSpacing.x))
                edit_checkbox(safe_addr(TABLES_ADV_OUTER_SIZE_ENABLED),
                              (id = "TBL_ADV_OUTERSIZE_EN", text = "outer_size"))

                // inner_width only takes effect when ScrollX is on; otherwise we pipe 0.0 to BeginTable (cpp:5848-5849).
                SetNextItemWidth(item_width)
                edit_drag_float(safe_addr(TABLES_ADV_INNER_WIDTH),
                    (id = "TBL_ADV_INNER_WIDTH",
                     text = "inner_width (when ScrollX active)",
                     speed = 1.0f, v_min = 0.0f, v_max = FLT_MAX,
                     format = "%.3f"))

                SetNextItemWidth(item_width)
                edit_drag_float(safe_addr(TABLES_ADV_ROW_MIN_HEIGHT),
                    (id = "TBL_ADV_ROWMINH",
                     text = "row_min_height",
                     speed = 1.0f, v_min = 0.0f, v_max = FLT_MAX,
                     format = "%.3f"))

                SetNextItemWidth(item_width)
                edit_drag_int(safe_addr(TABLES_ADV_ITEMS_COUNT),
                    (id = "TBL_ADV_ITEMS_COUNT",
                     text = "items_count",
                     speed = 0.1f, v_min = 0, v_max = 9999,
                     format = "%d"))

                SetNextItemWidth(item_width)
                var ct_items <- ["Text", "Button", "SmallButton",
                                 "FillButton", "Selectable",
                                 "Selectable (span row)"]
                edit_combo(safe_addr(TABLES_ADV_CONTENTS_TYPE),
                           (id = "TBL_ADV_CT",
                            text = "items_type (first column)",
                            items = ct_items))
                delete ct_items

                tree_pop()
            }

            spacing(TABLES_ADV_OPTIONS_SPACING)
            tree_pop()
        }

        // ---- Rebuild items if count changed (cpp:5870-5881) ----
        rebuild_adv_items()

        // ---- BeginTable (cpp:5889-5890) ----
        let inner_width_to_use = ((TABLES_ADV_FLAGS & ImGuiTableFlags.ScrollX) != ImGuiTableFlags.None
                                  ? TABLES_ADV_INNER_WIDTH
                                  : 0.0f)
        let effective_outer_size = (TABLES_ADV_OUTER_SIZE_ENABLED
                                    ? TABLES_ADV_OUTER_SIZE_VALUE
                                    : float2(0.0f, 0.0f))
        data_table(TABLES_ADV_T,
                   (text = "##table_advanced", columns = 6,
                    flags = TABLES_ADV_FLAGS,
                    outer_size = effective_outer_size,
                    inner_width = inner_width_to_use)) {
            // ---- Columns (cpp:5895-5901) ----

            // Description picks WidthStretch only when NoHostExtendX is off (cpp:5899) — else the table can't grow into the stretch column.
            let base = TABLES_ADV_COLUMNS_BASE_FLAGS
            table_setup_column("ID",
                base | ImGuiTableColumnFlags.DefaultSort |
                    ImGuiTableColumnFlags.WidthFixed |
                    ImGuiTableColumnFlags.NoHide,
                0.0f, TABLES_SORT_COL_ID)
            table_setup_column("Name",
                base | ImGuiTableColumnFlags.WidthFixed,
                0.0f, TABLES_SORT_COL_NAME)
            table_setup_column("Action",
                base | ImGuiTableColumnFlags.NoSort |
                    ImGuiTableColumnFlags.WidthFixed,
                0.0f, TABLES_SORT_COL_ACTION)
            table_setup_column("Quantity",
                base | ImGuiTableColumnFlags.PreferSortDescending,
                0.0f, TABLES_SORT_COL_QUANTITY)
            let desc_extra = ((TABLES_ADV_FLAGS & ImGuiTableFlags.NoHostExtendX) != ImGuiTableFlags.None
                              ? ImGuiTableColumnFlags.None
                              : ImGuiTableColumnFlags.WidthStretch)
            table_setup_column("Description", base | desc_extra,
                0.0f, TABLES_SORT_COL_DESCRIPTION)
            table_setup_column("Hidden",
                base | ImGuiTableColumnFlags.DefaultHide |
                    ImGuiTableColumnFlags.NoSort)
            table_setup_scroll_freeze(TABLES_ADV_FREEZE_COLS,
                                      TABLES_ADV_FREEZE_ROWS)

            // ---- Chop/Eat-triggered re-sort (cpp:5905-5910 items_need_sort path) ----

            // Last frame's Chop/Eat may set NEED_SORT without dirtying SpecsDirty; re-run the comparator on cached specs so a quantity edit re-orders next frame.
            if (TABLES_ADV_ITEMS_NEED_SORT && !empty(TABLES_ADV_LAST_SPECS)) {
                TABLES_ADV_ITEMS |> sort() $(a, b) {
                    return adv_compare(a, b, TABLES_ADV_LAST_SPECS)
                }
            }
            TABLES_ADV_ITEMS_NEED_SORT = false

            // ---- SpecsDirty-driven sort (cpp:5904-5912 sort_specs path) ----
            sort_specs() $(specs) {
                // Cache specs for the next-frame Chop/Eat re-sort; clear+push avoids cross-context pointer reuse (TableSortSpec is plain fields).
                TABLES_ADV_LAST_SPECS |> clear
                for (s in specs) {
                    TABLES_ADV_LAST_SPECS |> push(s)
                }
                TABLES_ADV_ITEMS |> sort() $(a, b) {
                    return adv_compare(a, b, specs)
                }
            }

            // cpp:5916 — if the Quantity column is sorted, Chop/Eat triggers a re-sort on button-release.
            let sorts_specs_using_quantity = ((table_get_column_flags(3) &
                ImGuiTableColumnFlags.IsSorted) != ImGuiTableColumnFlags.None)

            // ---- Headers (cpp:5919-5922) ----
            if (TABLES_ADV_SHOW_HEADERS &&
                (TABLES_ADV_COLUMNS_BASE_FLAGS & ImGuiTableColumnFlags.AngledHeader) != ImGuiTableColumnFlags.None) {
                table_angled_headers_row()
            }
            if (TABLES_ADV_SHOW_HEADERS) {
                table_headers_row()
            }

            // ---- Body — PushButtonRepeat + ListClipper (cpp:5926-6011) ----
            with_button_repeat(true) {
                list_clipper(length(TABLES_ADV_ITEMS)) $(start, end_) {
                    for (row_n in range(start, end_)) {
                        var item & = unsafe(TABLES_ADV_ITEMS[row_n])
                        let item_id = item.id
                        let item_is_selected = (find_index(TABLES_ADV_SELECTION,
                                                           item_id) != -1)
                        with_id("adv_row_{item_id}") {
                            table_next_row(ImGuiTableRowFlags.None,
                                           TABLES_ADV_ROW_MIN_HEIGHT)
                            // Col 0 (ID): contents-type switch (cpp:5949-5978).
                            table_set_column_index(0)
                            // cpp:5951 zero-pads "%04d"; gen2 "{val}" has no width-pad and the demo's 4-digit pad is cosmetic, so use a plain "{item_id}".
                            let text_label = "{item_id}"
                            if (TABLES_ADV_CONTENTS_TYPE == TABLES_ADV_CT_TEXT) {
                                text(TABLES_ADV_ID_TEXT[item_id],
                                     (text = text_label))
                            } elif (TABLES_ADV_CONTENTS_TYPE == TABLES_ADV_CT_BUTTON) {
                                button(TABLES_ADV_ID_BTN[item_id],
                                       (text = text_label))
                            } elif (TABLES_ADV_CONTENTS_TYPE == TABLES_ADV_CT_SMALL_BUTTON) {
                                small_button(TABLES_ADV_ID_BTN[item_id],
                                             (text = text_label))
                            } elif (TABLES_ADV_CONTENTS_TYPE == TABLES_ADV_CT_FILL_BUTTON) {
                                button(TABLES_ADV_ID_BTN[item_id],
                                       (text = text_label,
                                        size = float2(-FLT_MIN, 0.0f)))
                            } elif (TABLES_ADV_CONTENTS_TYPE == TABLES_ADV_CT_SELECTABLE ||
                                    TABLES_ADV_CONTENTS_TYPE == TABLES_ADV_CT_SELECTABLE_SPAN_ROW) {
                                let sel_flags = (TABLES_ADV_CONTENTS_TYPE == TABLES_ADV_CT_SELECTABLE_SPAN_ROW
                                    ? ImGuiSelectableFlags.SpanAllColumns | ImGuiSelectableFlags.AllowOverlap
                                    : ImGuiSelectableFlags.None)
                                if (selectable_label(TABLES_ADV_ID_SEL[item_id],
                                                     (text = text_label,
                                                      selected = item_is_selected,
                                                      flags = sel_flags,
                                                      size = float2(0.0f, TABLES_ADV_ROW_MIN_HEIGHT)))) {
                                    // cpp:5965-5976 — Ctrl+click toggles the item in selection; plain click resets to single-selection.
                                    if (GetIO().KeyCtrl) {
                                        if (item_is_selected) {
                                            let idx = find_index(TABLES_ADV_SELECTION,
                                                                 item_id)
                                            if (idx != -1) {
                                                TABLES_ADV_SELECTION |> erase(idx)
                                            }
                                        } else {
                                            TABLES_ADV_SELECTION |> push(item_id)
                                        }
                                    } else {
                                        TABLES_ADV_SELECTION |> clear()
                                        TABLES_ADV_SELECTION |> push(item_id)
                                    }
                                }
                            }

                            // Col 1 (Name) — cpp:5980-5981.
                            if (table_set_column_index(1)) {
                                text(TABLES_ADV_NAME_TEXT[item_id],
                                     (text = item.name))
                            }

                            // Col 2 (Action) — Chop/Eat (cpp:5987-5994); re-sort fires on button-release (IsItemDeactivated) so holding doesn't churn the table.
                            if (table_set_column_index(2)) {
                                if (small_button(TABLES_ADV_CHOP_BTN[item_id],
                                                 (text = "Chop"))) {
                                    item.quantity++
                                }
                                if (sorts_specs_using_quantity && IsItemDeactivated()) {
                                    TABLES_ADV_ITEMS_NEED_SORT = true
                                }
                                same_line(TABLES_ADV_SL_CHOP_EAT[item_id])
                                if (small_button(TABLES_ADV_EAT_BTN[item_id],
                                                 (text = "Eat"))) {
                                    item.quantity--
                                }
                                if (sorts_specs_using_quantity && IsItemDeactivated()) {
                                    TABLES_ADV_ITEMS_NEED_SORT = true
                                }
                            }

                            // Col 3 (Quantity) — cpp:5996-5997.
                            if (table_set_column_index(3)) {
                                let qty = item.quantity
                                text(TABLES_ADV_QTY_TEXT[item_id],
                                     (text = "{qty}"))
                            }

                            // Col 4 (Description) — wrapped or plain per the show_wrapped_text checkbox (cpp:5999-6003); TableSetColumnIndex(4) is unconditional, mirroring cpp.
                            table_set_column_index(4)
                            if (TABLES_ADV_SHOW_WRAPPED_TEXT) {
                                text_wrapped(TABLES_ADV_DESC_TEXT[item_id],
                                             (text = "Lorem ipsum dolor sit amet"))
                            } else {
                                text(TABLES_ADV_DESC_TEXT[item_id],
                                     (text = "Lorem ipsum dolor sit amet"))
                            }

                            // Col 5 (Hidden) — cpp:6005-6006.
                            if (table_set_column_index(5)) {
                                text(TABLES_ADV_HIDDEN_TEXT[item_id],
                                     (text = "1234"))
                            }
                        }
                    }
                }
            }

        }

        // ---- Debug details (cpp:6019-6031) — NOT PORTED ----

        // cpp's "Debug details" checkbox reads ImDrawList::CmdBuffer.Size, which the daslang ImVector binding doesn't expose; re-add when a size accessor lands.
    }
}
