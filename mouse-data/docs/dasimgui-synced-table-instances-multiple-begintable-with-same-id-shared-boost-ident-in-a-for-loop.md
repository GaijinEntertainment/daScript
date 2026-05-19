---
slug: dasimgui-synced-table-instances-multiple-begintable-with-same-id-shared-boost-ident-in-a-for-loop
title: dasImgui synced table instances — multiple BeginTable with same id, shared boost IDENT in a for-loop
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

**Cpp synced-instances pattern** (`for n = 0; n < 3; n++ ImGui::BeginTable("Table", ...)` — 3x `BeginTable` with the same string id) triggers ImGui's "synced" behavior: column width/visibility/order changes on one instance propagate to all visible instances. Useful when a table needs to render in multiple places (e.g. a list view + a detail panel) with identical column layout.

**Das equivalent:** use ONE shared boost `data_table(IDENT, ...)` IDENT inside a `for n in range(N)` loop. The boost `[container]` macro auto-PushIDs the IDENT in `widget_prelude` (see `widgets/imgui_boost_runtime.das`), so each iter sees the same prefix, and the inner `BeginTable("Table", ...)` string ID is also shared. Net effect: 3 BeginTable calls hash to the same ImGui ID → synced behavior.

```daslang
var private TABLES_SYNCED_FLAGS : int = int(ImGuiTableFlags.Resizable |
                                            ImGuiTableFlags.Reorderable |
                                            ImGuiTableFlags.Hideable |
                                            ImGuiTableFlags.Borders)
// Per-iter CollapsingHeader needs INDEPENDENT state (open/close per header),
// so that one uses an indexed `table<int; CollapsingHeaderState>`.
var private TABLES_SYNCED_HEADER : table<int; CollapsingHeaderState>

for (n in range(3)) {
    collapsing_header(TABLES_SYNCED_HEADER[n],
                      (text = "Synced Table {n}",
                       flags = ImGuiTreeNodeFlags.DefaultOpen)) {
        // ONE shared IDENT — auto-PushID is identical per iter, BeginTable
        // string id is also shared, so all 3 hash to the same ImGui ID.
        data_table(TABLES_SYNCED_T, (text = "Table", columns = 3,
                                     flags = unsafe(reinterpret<ImGuiTableFlags>(TABLES_SYNCED_FLAGS)),
                                     outer_size = float2(0,0),
                                     inner_width = 0.0f)) {
            // body — column width/visibility/order changes here propagate
            // to all 3 visible instances.
        }
    }
}
```

**Key:** `TABLES_SYNCED_T` is intentionally a single shared IDENT, NOT indexed per-iter. Do NOT write `TABLES_SYNCED_T[n]` — that would give each iter its own ImGui ID via the indexed-state pattern, and the synced behavior breaks (changes wouldn't propagate). The per-iter independent state (here the CollapsingHeader's open/close + the per-iter visible row count) lives in a SEPARATE indexed state table.

**Per-iter cell state still needs disambiguation** — if you write per-cell text inside the body, key the cell-state table by `n * MAX_CELLS_PER_ITER + cell` (or similar) so each iter's cells stay in their own state slot. The synced-instances trick is purely about sharing the TABLE-level ID; per-cell registry paths can and should still be unique per iter.

Landed dasImgui PR #52 (PR-D/2 commit c791118, section 22 Synced instances). Source: `examples/imgui_demo/tables.das show_synced_instances()`. Mirrors `imgui_demo.cpp:5594-5624`.

## Questions
- dasImgui synced table instances — multiple BeginTable with same id, shared boost IDENT in a for-loop
