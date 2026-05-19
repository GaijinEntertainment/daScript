---
slug: imgui-angled-headers-row-plus-normal-headers-row-both-intentional
title: ImGui table headers — both TableAngledHeadersRow and TableHeadersRow called intentional in cpp imgui_demo; angled has no column context menu
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

**cpp `imgui_demo.cpp` intentionally calls BOTH `TableAngledHeadersRow()` AND `TableHeadersRow()`** in the same `BeginTable`/`EndTable` block when angled headers are in use. This is NOT a redundant double-emit — it's a deliberate UX pattern:

- `TableAngledHeadersRow()` draws the rotated column labels (the visual "angled" effect at the top of each column). It does NOT expose the per-column context menu (right-click on header → Sort / Hide / Resize / etc.).
- `TableHeadersRow()` draws an additional flat header row below. This is what provides the column context menu (per `ImGuiTableFlags_ContextMenuInBody` note in imgui.h, the context menu surface is attached to the standard header row, not the angled one).

Authoritative sources (cpp imgui_demo.cpp):
- `imgui_demo.cpp:5455-5456` — `TableAngledHeadersRow(); TableHeadersRow();` // comment: "Draw remaining headers and allow access to context-menu and other functions."
- `imgui_demo.cpp:4911-4913` — same pattern in `ShowDemoWindowTablesAdvanced` AngledHeaders demo
- `imgui_demo.cpp:5919-5922` — same pattern in `ShowDemoWindowTables` Sorting demo

**Implication for the dasImgui port**: when porting any tables.das section that calls `table_angled_headers_row()`, also keep the `table_headers_row()` call immediately after it. Don't "clean up" what looks like a duplicate — you'd silently delete column context-menu access.

**Surfaced during dasImgui PR #52 review** (R5 round-2 Copilot comment): Copilot suggested dropping the `table_headers_row()` call after `table_angled_headers_row()` as "redundant". REJECTED — verified against cpp parity. The redundancy is intentional UX scaffolding; both calls stay.

Reach for this whenever a code reviewer (Copilot or human) flags duplicate-looking header-row calls in a tables port. Verify against `imgui/imgui_demo.cpp` lines above before accepting any removal.

## Questions
- ImGui table headers — both TableAngledHeadersRow and TableHeadersRow called intentional in cpp imgui_demo; angled has no column context menu
