---
slug: daslang-extract-lambda-register-postlude-into-private-helper-via-var-state-param
title: In daslang, can I extract the "lambda-build + register-with-table" pattern into a private helper that takes `var state : T` and builds the lambda from the helper's own param scope? Does the captured `& state` survive after the helper returns?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

Yes — works because struct parameters pass by reference and `addr(state)` / `& state` resolves to the underlying global's address, not the helper's stack slot.

**Pattern from dasImgui Phase 0b.1** (`modules/dasImgui/widgets/imgui_widgets_builtin.das`):

Five click-style widgets (button, small_button, arrow_button, invisible_button, image_button) all share an identical lambda+register postlude. Without the helper, that's ~10 lines of `unsafe { let ser <- ...; let disp <- ...; widget_finalize(...) }` repeated five times. Extracted helper:

```das
def private click_finalize(widget_ident : string; kind : string; var state : ClickState) {
    var entry : WidgetEntry
    unsafe {
        let ser <- @ capture(& state) () : JsonValue ? {
            return JV(state)
        }
        let disp <- @ capture(& state) (action : string; payload : JsonValue ?) {
            if (action == "click") { state.pending_click = true }
        }
        widget_finalize(widget_ident, kind, entry, ser, disp)
    }
}
```

Each widget collapses to:
```das
[widget]
def small_button(var state : ClickState; text : string) : bool {
    let im_clicked = SmallButton(text)
    /* ... per-widget state mutation ... */
    click_finalize(widget_ident, "small_button", state)
    return clicked
}
```

**Why the captured pointer survives:** in daslang, struct/array/table types pass by reference (no `&` needed in the param). The function parameter is a name binding to the caller's storage — for dasImgui widgets, that storage is a module-scope global (`SAVE_BTN`, `QUIT_BTN`, ...) auto-emitted by the `[widget]` call_macro. `addr(state)` returns the global's address, which is stable for the program's lifetime. The lambda's captured `& state` outlives both the helper and the per-frame widget call.

**Three helpers fit Phase 0b.1's three state shapes:**
- `click_finalize(... var state : ClickState)` — dispatcher action `"click"`
- `toggle_finalize(... var state : ToggleState)` — dispatcher action `"set"` (bool payload)
- `radio_int_finalize(... var state : RadioIntState)` — dispatcher action `"set"` (int payload)

**Why three helpers, not one generic:** state struct types differ; dispatcher closure body differs (which pending field to set, which payload type to extract). A generic would need static dispatch on the struct type — more machinery than just three named helpers. Three named functions is also greppable.

**Don't extract too eagerly:** sliders' bodies share less with each other (per-numeric-type pending fields, format strings), so `slider`/`slider_int` left their finalize inline pending the 0b.2 stateful-input expansion that'll create more concrete shape candidates.

## Questions
- In daslang, can I extract the "lambda-build + register-with-table" pattern into a private helper that takes `var state : T` and builds the lambda from the helper's own param scope? Does the captured `& state` survive after the helper returns?
