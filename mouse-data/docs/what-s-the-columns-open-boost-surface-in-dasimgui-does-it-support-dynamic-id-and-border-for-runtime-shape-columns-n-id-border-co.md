---
slug: what-s-the-columns-open-boost-surface-in-dasimgui-does-it-support-dynamic-id-and-border-for-runtime-shape-columns-n-id-border-co
title: What's the columns_open boost surface in dasImgui — does it support dynamic id and border for runtime-shape Columns(n, id, border) consumers?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

As of 2026-05-16 (PR #40), `columns_open(n; id : string = ""; border : bool = true)` accepts both id (id-stack tag for column-width persistence) and border (vertical-divider toggle) as defaulted trailing params. Use the new signature when a `Columns(n, id, border)` consumer can't go through the static `columns(N, ${col0}, …, ${colN-1})` call_macro — dynamic column count, loop-driven shape, etc.

```daslang
require imgui/imgui_layout_builtin

// Static shape — use the columns(N, …) call_macro:
columns(3, ${col0}, ${col1}, ${col2})

// Dynamic shape — call the primitive trio directly:
columns_open(COL_COUNT, "table-id", BORDER_FLAG)
for (i in range(COL_COUNT * rows)) {
    text("cell {i}")
    next_col()
}
columns_close()
```

**Backwards-compat:** the older `columns_open(n)` call (used by the `columns(N, …)` macro's expansion) still works — `id=""` and `border=true` are the defaults. The call_macro doesn't break.

**Three sub-section patterns from `examples/imgui_demo/columns.das`:**
1. Loop-over-N items with no border: `columns_open(3, "mycolumns3", false)` — Basic 14-item loop.
2. Dynamic count + dynamic border: `columns_open(COLUMNS_BORDERS_COUNT, "", COLUMNS_BORDERS_V)` — Borders demo with user-driven count.
3. Custom id, default border: `columns_open(2, "tree")` — Tree demo (relies on `border=true` default).

**Layout primitives sibling rail:** the same module provides `next_col()`, `columns_close()`, and `list_clipper(items_count, body)`. All three are public exports of `imgui_layout_builtin`.

**Reference:** `widgets/imgui_layout_builtin.das:columns_open` (and the `columns(N, …)` ColumnsCallMacro just below).

## Questions
- What's the columns_open boost surface in dasImgui — does it support dynamic id and border for runtime-shape Columns(n, id, border) consumers?
