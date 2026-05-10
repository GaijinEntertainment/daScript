---
slug: daslang-generic-widget-finalize-helper-typedecl-from_jv
title: How do I write one generic widget-finalize helper in daslang that captures `var state : auto(StateT)` and dispatches `imgui_set` for any of N different state struct types without per-type duplication?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**Pattern:** generic on `auto(StateT)`, use `typedecl(state.field)` to drive `from_JV`'s type witness. Single helper, 10+ instantiations, no per-type duplication.

```das
def private drag_set_finalize(widget_ident : string; kind : string; var state : auto(StateT)) {
    var entry : WidgetEntry
    unsafe {
        let ser <- @ capture(& state) () : JsonValue ? {
            return JV(state)
        }
        let disp <- @ capture(& state) (action : string; payload : JsonValue ?) {
            if (action == "set") {
                state.pending_value = from_JV(payload?["value"],
                                              type<typedecl(state.pending_value)>,
                                              default<typedecl(state.pending_value)>)
                state.has_pending = true
            }
        }
        widget_finalize(widget_ident, kind, entry, ser, disp)
    }
}
```

This one helper covers DragStateFloat (pending_value:float), DragStateFloat2/3/4 (pending_value:float2/3/4), DragStateInt/2/3/4 (pending_value:int/int2/3/4), DragStateRangeFloat (pending_value:float2), DragStateRangeInt (pending_value:int2). 10 instantiations, all generated from one body. See `modules/dasImgui/widgets/imgui_widgets_builtin.das` (phase0b.2 commit `3a2bde3`).

**Three load-bearing pieces:**

1. **`var state : auto(StateT)`** — generic state param. The `auto(StateT)` form binds an alias `StateT` you can name, but here it's not even needed — the helper never references StateT directly. Plain `auto` works the same. Keeping the alias is a hint to readers that the struct varies per call.

2. **`typedecl(state.pending_value)`** — compile-time type-of the state field. From CLAUDE.md: "`typedecl(expr)` — compile-time type-of, usable inside `default<>`: `default<typedecl(field)>`." Same composes inside `type<>`: `type<typedecl(state.pending_value)>` is the type witness for `from_JV`'s auto(VecT) overload-resolution.

3. **Two args to from_JV: `type<...>` and `default<...>`.** Don't use `default<X>` for both — error 31200 "template argument must be type<...> and not 0. ExprConstInt" because `default<int>` collapses to the literal `0` and the template expects an actual type tag.

**Capture `& state` works across helper return because state is a function param REF to the module global.** Lambda holds the reference; `widget_finalize`'s g_dispatchers / g_serializers tables retain the lambda (and via that the captured ref) past the helper's return. See `daslang-extract-lambda-register-postlude-into-private-helper-via-var-state-param` for the validation that this is fine.

**When NOT to extract a generic helper:** when the per-state-type bodies differ in more than just the field type — e.g. `click_finalize` (action="click", state.pending_click=true), `toggle_finalize` (action="set", state.pending_toggle=true + pending_value), `radio_int_finalize` (action="set", state.pending_set=true + pending_value). Those branches differ on field NAMES, not types. Generic-on-StateT can't see that field names differ. Three concrete-state helpers there beat one generic that branches on `is`-checks of an auto-typed field. Generic is the right hammer when the state shape is uniform and only the field type varies.

**Found 2026-05-10**, dasImgui phase 0b.2.

## Questions
- How do I write one generic widget-finalize helper in daslang that captures `var state : auto(StateT)` and dispatches `imgui_set` for any of N different state struct types without per-type duplication?
