---
slug: dasimgui-sort-specs-block-arg-helper-for-sortable-data-table-tablesortspec-struct-sort-specs-specs-pattern
title: dasImgui sort_specs block-arg helper for sortable data_table — TableSortSpec struct + sort_specs() $(specs) { ... } pattern
created: 2026-05-19
last_verified: 2026-05-19
last_updated: 2026-05-19
links: []
---

**`sort_specs() $(specs) { ... }`** is a block-arg helper in `widgets/imgui_table_builtin.das` that captures ImGui's `TableGetSortSpecs()`, converts each `ImGuiTableColumnSortSpecs` entry into a daslang-friendly `TableSortSpec`, invokes the block with the array, and auto-clears `SpecsDirty` on return.

Call it inside a `data_table(...)` body BEFORE emitting rows. Header-row placement is flexible — call `sort_specs()` either BEFORE or AFTER `table_headers_row()`. Sort state is finalized at `BeginTable` time (input events drain at frame start during `NewFrame`), so the header-row call doesn't gate when specs become visible. cpp `ShowDemoWindowTablesAdvanced` calls before headers. Pass `ImGuiTableFlags.Sortable` (and optionally `SortMulti` for multi-column) on the table flags, and tag each column with a stable `user_id` so the comparator dispatches by identity rather than column_index (robust under Reorderable):

```daslang
data_table(MY_TABLE, (text = "...", columns = 3,
                      flags = ImGuiTableFlags.Sortable | ImGuiTableFlags.SortMulti | ...,
                      outer_size = float2(0,0), inner_width = 0.0f)) {
    table_setup_column("ID",   ImGuiTableColumnFlags.DefaultSort, 0.0f, COL_ID)
    table_setup_column("Name", ImGuiTableColumnFlags.None,        0.0f, COL_NAME)
    table_setup_column("Qty",  ImGuiTableColumnFlags.None,        0.0f, COL_QTY)
    table_headers_row()

    sort_specs() $(specs) {
        // Multi-key comparator: walk specs in priority order.
        MY_ITEMS |> sort() $(a, b) {
            for (s in specs) {
                let ord = compare_by_user_id(s, a, b)
                if (ord != 0) return ord < 0
            }
            return a.id < b.id  // tiebreak on a unique field
        }
    }

    for (i in range(length(MY_ITEMS))) { ... }
}
```

`TableSortSpec` fields (daslang-friendly mirror of `ImGuiTableColumnSortSpecs`):
- `column_index : int` — 0-based position
- `column_user_id : uint` — the `user_id` you passed to `table_setup_column` (or 0)
- `sort_order : int` — 0 = primary, 1 = secondary, ... (under SortMulti)
- `sort_direction : ImGuiSortDirection` — Ascending / Descending / None

**Block fires on SpecsDirty, including the empty case.** `sort_specs()` returns `bool` — true iff the body ran (specs non-null AND SpecsDirty). When `SpecsCount == 0` the block STILL fires, called with an empty `specs : array<TableSortSpec>` — this is the `ImGuiTableFlags.SortTristate` "user cleared all sort columns" event (imgui.h:1871, `TableGetSortSpecs() may return specs where SpecsCount == 0`). The comparator should handle the empty case (typically "restore stable insertion order" or "no-op"). When dirty=false, reuse the most recent sort order; ImGui keeps the headers visually in sync without re-firing dirty until the user re-clicks. So the comparator cost is paid once per sort change, not every frame.

**Final tiebreak matters** — `daslang's sort() is qsort, not stable` ([daslang-sort-qsort-not-stable](daslang-sort-qsort-not-stable.md)), so a final tiebreak on a unique field keeps the order total. Without it, equal-key elements can shuffle between frames.

**Wired in dasImgui PR #52** (PR-D/1A, commit ad15368). Wrapper: `widgets/imgui_table_builtin.das sort_specs`. Backing C++ helpers in `src/dasIMGUI.main.cpp` — `GetSortSpec` returns a pointer (not reference) because non-workhorse `const T&` returns trip the WrapType FFI; `GetColumnSortDirection` is renamed from natural `SortDirection` to dodge an MSVC `decltype` member/free-function ambiguity (see [dasimgui-ffi-const-ref-return-and-decltype-name-collision](dasimgui-ffi-const-ref-return-and-decltype-name-collision.md)). Examples: `examples/features/sort_specs.das` (standalone inventory) + `examples/tutorial/data_table.das` (extended sortable tutorial) + `examples/imgui_demo/tables.das show_sorting()` (section 23 of the imgui_demo port).

## Questions
- dasImgui sort_specs block-arg helper for sortable data_table — TableSortSpec struct + sort_specs() $(specs) { ... } pattern
