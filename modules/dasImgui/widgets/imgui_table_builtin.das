options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options _allow_imgui_legacy = true

module imgui_table_builtin shared

require imgui public
require imgui/imgui_boost_runtime public
require imgui/imgui_boost_v2 public
require imgui/imgui_containers_builtin public

//! Tables family — `[container] data_table` + cursor wrappers + sort-spec capture.
//!
//! ImGui's table API splits into chrome that fires once per `BeginTable`
//! (the column-setup calls) and cursor primitives that step rows/columns
//! inside the body loop. All members live behind proper daslang
//! wrappers in this module (rather than ALLOWED_IMGUI extensions) so the
//! consumer surface stays uniform: `data_table { table_setup_column(...);
//! table_headers_row(); for (...) { table_next_row(); table_set_column_index(0); ... } }`.
//!
//! Sort-spec capture goes through `sort_specs` (block-arg helper that
//! yields a daslang-friendly `array<TableSortSpec>` and auto-clears the
//! dirty flag). Multi-select / frozen-row indicators / custom row-bg
//! callbacks would extend `TableState` additively in future PRs; no boost
//! wrapper for the MultiSelect API exists yet.

[container]
def data_table(var state : TableState; text : string; columns : int;
               flags : ImGuiTableFlags; outer_size : float2; inner_width : float;
               blk : block) {
    //! `BeginTable(id, columns, flags, outer_size, inner_width)` / `EndTable()`
    //! — only-on-true pairing. Body emits `table_setup_column` /
    //! `table_setup_scroll_freeze` / `table_headers_row` / `table_next_row` /
    //! `table_set_column_index` / `table_next_column` cursor calls.
    state.columns = columns
    state.flags = flags
    state.outer_size = outer_size
    state.inner_width = inner_width
    let im_open = BeginTable(text, columns, flags, outer_size, inner_width)
    if (im_open) {
        invoke(blk)
        EndTable()
    }
    stateless_finalize(widget_ident, "data_table", state)
}

def public table_setup_column(text : string;
                              flags : ImGuiTableColumnFlags = ImGuiTableColumnFlags.None;
                              init_width_or_weight : float = 0.0f;
                              user_id : uint = 0u) {
    //! `TableSetupColumn(label, flags, init_width_or_weight, user_id)` — call
    //! once per column directly after BeginTable, before `table_headers_row`.
    TableSetupColumn(text, flags, init_width_or_weight, user_id)
}

def public table_setup_scroll_freeze(cols : int; rows : int) {
    //! `TableSetupScrollFreeze(cols, rows)` — pin the first ``cols`` columns
    //! and ``rows`` rows during ScrollX/ScrollY. Call before `table_headers_row`.
    TableSetupScrollFreeze(cols, rows)
}

def public table_headers_row() {
    //! `TableHeadersRow()` — submit the header row using the labels passed to
    //! the per-column `table_setup_column` calls.
    TableHeadersRow()
}

def public table_header(text : string) {
    //! `TableHeader(label)` — submit a single custom header cell (instead of `table_headers_row`).
    //! Pair with `table_next_row(Headers)` / per-column `table_set_column_index` for full control over
    //! header contents (e.g. checkbox + label, multi-line text, custom alignment).
    TableHeader(text)
}

def public table_angled_headers_row() {
    //! `TableAngledHeadersRow()` — submit a header row with labels rotated by
    //! `ImGuiStyle.TableAngledHeadersAngle` (default 35°); useful when narrow columns can't fit
    //! horizontal labels. Set `ImGuiTableColumnFlags.AngledHeader` on each column before calling.
    TableAngledHeadersRow()
}

def public table_next_row(flags : ImGuiTableRowFlags = ImGuiTableRowFlags.None;
                          min_row_height : float = 0.0f) {
    //! `TableNextRow(flags, min_row_height)` — append a new row; pairs with
    //! per-column `table_set_column_index` or `table_next_column` calls.
    TableNextRow(flags, min_row_height)
}

def public table_next_column() : bool {
    //! `TableNextColumn()` — advance one column (or wrap to next row); returns
    //! true when the column is visible (caller may skip non-visible content).
    return TableNextColumn()
}

def public table_set_column_index(col : int) : bool {
    //! `TableSetColumnIndex(col)` — jump to ``col`` in the current row; returns
    //! true when the column is visible.
    return TableSetColumnIndex(col)
}

def public table_set_bg_color(target : ImGuiTableBgTarget; color : uint;
                              column_n : int = -1) {
    //! `TableSetBgColor(target, color, column_n)` — paint a row/cell background. ``target`` picks the
    //! layer (``RowBg0``/``RowBg1``/``CellBg``); ``color`` is `pack_color`-packed; ``column_n = -1`` is the
    //! current column under ``CellBg``. Call after the row's `table_next_row` (and `table_set_column_index` for ``CellBg``).
    TableSetBgColor(target, color, column_n)
}

def public table_get_column_count() : int {
    //! `TableGetColumnCount()` — column count of the current table.
    return TableGetColumnCount()
}

def public table_get_column_index() : int {
    //! `TableGetColumnIndex()` — 0-based index of the column the cursor is
    //! currently in.
    return TableGetColumnIndex()
}

def public table_get_row_index() : int {
    //! `TableGetRowIndex()` — 0-based index of the row the cursor is
    //! currently in.
    return TableGetRowIndex()
}

def public table_get_column_name(col : int = -1) : string {
    //! `TableGetColumnName(column_n)` — label of column ``col`` (the value
    //! passed to `table_setup_column`); ``-1`` means the current column.
    return TableGetColumnName(col)
}

def public table_get_column_flags(col : int = -1) : ImGuiTableColumnFlags {
    //! `TableGetColumnFlags(column_n)` — current per-column flags, including runtime status bits
    //! (``IsHovered`` / ``IsSorted`` / ``IsEnabled`` / ``IsVisible``); ``-1`` = current column. Mask with
    //! ``ImGuiTableColumnFlags.StatusMask_`` to read just the status bits.
    return TableGetColumnFlags(col)
}

struct public TableSortSpec {
    //! Daslang-friendly mirror of one `ImGuiTableColumnSortSpecs` entry,
    //! yielded by `sort_specs` inside a `data_table` body.
    column_index   : int                //!< 0-based index of the column being sorted on (matches the order of `table_setup_column` calls).
    column_user_id : uint               //!< The ``user_id`` you passed to `table_setup_column` for this column (or 0 if you didn't).
    sort_order     : int                //!< Sort precedence: 0 = primary, 1 = secondary, ... Under `ImGuiTableFlags.SortMulti` multiple entries appear with increasing ``sort_order``.
    sort_direction : ImGuiSortDirection //!< `Ascending` / `Descending` / `None`.
}

def public sort_specs(blk : block<(specs : array<TableSortSpec>) : void>) : bool {
    //! Capture the table's sort specs when ImGui marks them dirty, convert each to a `TableSortSpec`, and
    //! yield the array to ``blk`` (auto-clears the dirty flag). Call in a `data_table` body with `ImGuiTableFlags.Sortable`.
    //! Returns true iff invoked; the array may be EMPTY under `SortTristate` (last column cleared = "reset to default order").
    var p = TableGetSortSpecs()
    if (p == null || !p.SpecsDirty) return false
    let n = p.SpecsCount
    var arr : array<TableSortSpec>
    if (n > 0) {
        arr |> reserve(n)
        for (i in range(n)) {
            let s = GetSortSpec(p, i)
            arr |> push(TableSortSpec(
                column_index = int(s.ColumnIndex),
                column_user_id = s.ColumnUserID,
                sort_order = int(s.SortOrder),
                sort_direction = GetColumnSortDirection(s)))
        }
    }
    invoke(blk, arr)
    delete arr
    unsafe { p.SpecsDirty = false; }
    return true
}
