---
slug: dasimgui-with-id-for-per-iter-id-namespacing-inside-loops-where-widget-idents-are-literal-strings
title: dasImgui with_id() for per-iter ID namespacing inside loops where widget IDENTs are literal strings
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

**`with_id("<scope>_{i}") { ... }`** is the standard pattern for per-iter ID namespacing when a loop body contains widget calls with literal-string IDENTs or `id =` args that can't be interpolated per iteration.

The trap: boost macros like `edit_checkbox_flags(safe_addr(...), (id = "F_RESIZABLE", ...))` require the `id =` argument to be a string LITERAL — it's compile-time identifier scaffolding, not a runtime value. So a loop body that emits N identical-shape widgets can't write `id = "F_RESIZABLE_{i}"` to disambiguate each iter. Without disambiguation, ImGui's ID hash collides across iters and you get one shared widget instead of N.

**Fix:** wrap the loop body in `with_id("scope_{n}") { ... }`. ImGui's `PushID(string)` mixes the prefix into every IDENT hash inside the block, so per-iter instances stay disjoint:

```daslang
for (column in range(3)) {
    with_id("colflags_col_{column}") {
        text(LABEL, (text = "Column {column}"))
        edit_checkbox_flags(safe_addr(g_input[column]),
            (id = "F_DEFAULT_SORT", text = "DefaultSort",
             flags_value = int(ImGuiTableColumnFlags.DefaultSort)))
        edit_checkbox_flags(safe_addr(g_input[column]),
            (id = "F_NO_HIDE", text = "NoHide",
             flags_value = int(ImGuiTableColumnFlags.NoHide)))
        // ... N more fixed-IDENT toggles ...
    }
}
```

All N+ toggles inside share the same hard-coded `id = "..."` literals across all iters, but each iter's `with_id` prefix gives them distinct ImGui IDs. Found at `widgets/imgui_id_builtin.das:with_id`.

**When NOT to use with_id:** if the inner widget IDENT itself is indexed (`text(TABLES_RHH_T1_CELL[row * 3 + col], ...)`), no with_id is needed — the table-indexed state IS the unique key per cell. `with_id` is specifically for the case where the inner widget call has a fixed literal-string IDENT or a literal `id=` arg.

**Cross-iter PushID hint:** `with_id("name_{i}")` interpolates fine (it's a regular string, not a literal-required compile-time arg). So the prefix CAN be runtime — only the inner widget IDs are the literal-string trap.

Used heavily across imgui_demo ports: PR #52 sections 8 (sizing per-table loop), 11 (columns flags per-column), 18 (per-row sliders), 19 (per-column header cells), 20 (per-row + per-col checkboxes), 21 (per-cell ".." buttons), 23 (per-item rows). See `widgets.das` and `tables.das` for concrete examples.

Related:
- [how-do-indexed-table-keyed-dasimgui-widgets-work-slider-float-channel-vol-i](how-do-indexed-table-keyed-dasimgui-widgets-work-slider-float-channel-vol-i.md) — indexed-form widget state pattern (use this when the IDENT itself is indexed, INSTEAD of with_id).
- [dasimgui-widget-no-ident-form](dasimgui-widget-no-ident-form.md) — when IDENT can be dropped entirely (single-call sites).

## Questions
- dasImgui with_id() for per-iter ID namespacing inside loops where widget IDENTs are literal strings
