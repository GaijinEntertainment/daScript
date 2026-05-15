---
slug: dasimgui-new-interactive-widget-finalize-must-call-register-focusable
title: When I add a new interactive [widget] to the dasImgui boost rail, what does its finalize helper need to call for imgui_focus / playwright keyboard-focus automation to target it?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

**Call `register_focusable(widget_ident)` at the end of the finalize helper.** Without it, `imgui_focus` (and any playwright-driven focus targeting that reads the focusable set) treats the widget as display-only and refuses to focus it — the widget still renders and responds to mouse, but keyboard-focus automation can't find it.

The contract is parity-by-example: every interactive finalize helper in `widgets/imgui_widgets_builtin.das` ends with `register_focusable(widget_ident)`:

- `pending_value_finalize` (click family) — line ~51
- `toggle_finalize` (Checkbox/Selectable/MenuItem) — line ~73
- `radio_int_finalize` — line ~95
- `text_value_finalize` (InputText family) — line ~119
- `edit_finalize` (scalar edit) — line ~142
- (newly added in PR #32) `text_filter_finalize` for `text_filter` — line ~1192

Display-only finalize helpers (e.g. the `text` / `text_unformatted` finalize that wraps non-interactive labels) deliberately skip the call — there's a comment at `widgets/imgui_widgets_builtin.das:1313` documenting "`register_focusable` is NOT called (display widgets are not focusable)."

**Rule of thumb:** if the widget accepts keyboard input or has any input-text component (e.g. `ImGuiTextFilter::Draw` wraps an InputText under the hood, even though the wrapper looks "passive"), it's interactive and must register. PR #32 round 3 review caught the missing call for `text_filter` exactly because the inline InputText made it focusable in spirit but the finalize didn't register it.

**Why it's easy to miss:** the call is in the finalize helper, not the `[widget]` body itself — so when you copy a similar widget for a new one, you'd see the body but might skip looking at the helper. Always check the finalize helper template against an existing interactive sibling, not just the body.

## Questions
- When I add a new interactive [widget] to the dasImgui boost rail, what does its finalize helper need to call for imgui_focus / playwright keyboard-focus automation to target it?
