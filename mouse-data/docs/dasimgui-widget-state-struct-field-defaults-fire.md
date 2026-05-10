---
slug: dasimgui-widget-state-struct-field-defaults-fire
title: Do dasImgui [widget] state struct field defaults fire on the auto-emitted module global?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**Yes**, since dasImgui commit `111f7dc` on `bbatkin/dasimgui-widget-auto-init`. The `[widget]` macro now emits `var IDENT : T = T()` (via `ExprMakeStruct(useInitializer)` attached to the auto-emitted Variable's `init`) instead of the bare `var IDENT : T` it emitted previously. The state struct's default constructor runs, so source-declared field defaults take effect on the per-ident global.

Confirmed for: `int / float / bool` numeric defaults; vector struct-ctor defaults like `float3(0.5, 0.6, 0.7)`; string defaults like `caption : string = "default-label"`; enum value defaults like `mode : ImGuiButtonFlags = ImGuiButtonFlags.MouseButtonRight`. See `modules/dasImgui/tests/integration/test_widget_init_defaults.das` (the canonical e2e check) and `modules/dasImgui/examples/features/widget_init_defaults.das` (the feature it drives).

**Practical effect on existing 0b state structs** (commit `e3a7ff7` swept these post-fix):
- `InputTextState.capacity = 256` (the body's `if (state.capacity == 0) { state.capacity = 256 }` workaround is gone).
- `InputTextState.value = "" / pending_value = ""` — the `@safe_when_uninitialized` annotations are dropped; `= ""` does the same job.
- `ComboState.value = -1 / ListBoxState.value = -1` — first frame matches the "selected index, -1 = none" docstring (was zero-init = 0 = first item before the fix).
- `WindowState.open = true / TabItemState.open = true` — closable windows + tabs render from frame 1; X-button + `imgui_close` still flip false. `PopupState.open` intentionally stays false (popups stay closed until triggered).

**Rule of thumb when designing a `[widget]` state struct**, post-fix:
- Persistent `@live` values the user wants preserved across reloads → state, with whatever default makes the first frame correct.
- Zero-init that genuinely is the right starting state (slider/drag values, bounds when ImGui's `min==max=unclamped` semantics apply, all `pending_*`/`changed`/`has_pending` transient flags, `flags` fields where `None=0` is the natural default) → no default needed.
- String fields → either `= ""` default or `@safe_when_uninitialized`; the former is the common case.

**Behaviour changes vs pre-fix** (consequences of `e3a7ff7`):
- A combo or list_box with no per-frame `set_value` shows "no selection" on frame 1 instead of the first item.
- A closable window or closable tab is visible from frame 1 without explicit `STATE.open = true` in init.
- An InputText with no explicit capacity uses 256 bytes from frame 1 (was: same value, but reached via the body workaround).

## Questions
- Do dasImgui [widget] state struct field defaults fire on the auto-emitted module global?
