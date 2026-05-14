---
slug: dasimgui-list-box-unified-block-arg-form
title: In dasImgui v2 boost, what's the current list_box shape — items-array or Begin/End + Selectable?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

**Begin/End + Selectable, block-arg form.** Shipped via PR #28 (2026-05-14, `bbatkin/dasimgui-boost-scope-wrappers`).

Use:

    list_box(LANG, (text = "Language", size = float2(-1.0f, 180.0f))) {
        for (i in range(length(LANG_NAMES))) {
            let is_sel = (i == LANG.value)
            let row_text = "{LANG_ICONS[i]} {LANG_NAMES[i]}"   // NOT `let label = ...` — `label` is a keyword
            if (Selectable(row_text, is_sel,
                           ImGuiSelectableFlags.None,
                           float2(0.0f, 0.0f))) {
                LANG.value = i
            }
        }
    }

Lives in `modules/dasImgui/widgets/imgui_containers_builtin.das`. State struct is `ListBoxState` (`value`, `has_pending`, `pending_value`, `changed` — selection-tracking machinery preserved). `imgui_set {"target": "PATH/LANG", "value": N}` steers `state.value` from outside.

**Breaking-change history (PR #28):**

- The **legacy items-array `[widget] def list_box(state, text, items, height_in_items) : bool`** (single-call form wrapping `ImGui::ListBox(label, &current, items, count, height)`) is **DELETED**. Migrate items-array callers to the block-arg form by emitting `Selectable` rows in a `for` loop.
- The old **`list_box_select`** `[container]` was the modern Begin/End form during a transition period. It is **RENAMED to `list_box`** — drop the `_select` suffix at every call site.

Asymmetry note: `combo` ([widget], items-array) and `combo_select` ([container], block-arg) still coexist. The list_box unification dropped the items-array form because the modern ImGui idiom is Begin/End + Selectable and there was only one user. `combo` may unify the same way in a follow-up — not committed.

**Snapshot kind**: `"list_box"` (both the rename in the dispatcher and the assertion in `tests/integration/test_containers_overlay.das` were updated).

## Questions
- In dasImgui v2 boost, what's the current list_box shape — items-array or Begin/End + Selectable?
