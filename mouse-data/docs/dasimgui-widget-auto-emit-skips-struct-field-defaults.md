---
slug: dasimgui-widget-auto-emit-skips-struct-field-defaults
title: In dasImgui [widget] auto-emit, do struct field defaults like `format : string = "%.3f"` apply to the auto-emitted module global?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**No.** The `[widget]` macro emits the per-ident state global as a bare `var IDENT : StateStruct` declaration with no initializer (see `WidgetCallMacro::visit` in `modules/dasImgui/widgets/imgui_boost.das`). Struct field defaults only fire when the constructor `StateStruct()` is invoked — and the macro never invokes it — so at runtime every field of the global starts at the type's zero value, not the source-declared default.

This has two practical consequences:

1. **Numeric/bool/tuple field defaults are silently lost.** Declaring `@live speed : float = 1.0f` in a state struct does NOT mean the global's speed starts at 1.0f. It starts at 0.0f. If your widget body does `let changed = DragFloat(text, ..., state.speed, ...)` and the user never explicitly sets `IDENT.speed = ...`, ImGui will receive 0.0 and the drag won't respond. Same trap for any numeric default users would expect to "just work".

2. **String field defaults trip the uninit-safety check at compile time** (error 31014, "Uninitialized variable IDENT is unsafe. Use initializer syntax or @safe_when_uninitialized when intended."). String is a heap pointer; daslang refuses to zero-init a struct global if any field is a heap pointer without explicit initializer or per-field `@safe_when_uninitialized`. The default literal in the struct is **not** an initializer — it would only fire through a constructor call.

**Workaround: push these to per-call args instead of state fields.** That's what the slider/drag widgets in `imgui_widgets_builtin.das` actually do — `format` and `speed` are NOT in the state struct at all; they're parameters of the widget def with daslang-call-site defaults that DO fire (because they're regular function default args, not struct field defaults):

```das
[widget]
def drag_float(var state : DragStateFloat; text : string;
               speed : float = 1.0f;
               format : string = "%.3f";
               flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    ...
    let (mn, mx) = state.bounds                  // bounds STAYS in state — persistent config, zero-init = unclamped per ImGui contract
    let changed = DragFloat(text, addr, speed, mn, mx, format, flags)
    ...
}
```

Caller writes `drag_float(OPACITY, (text = "Opacity", speed = 0.01f))` and gets a working drag.

**Rule of thumb when designing a `[widget]` state struct:**
- Persistent value(s) the user wants preserved across reloads → state.
- Per-instance config that's safely zero-init (numeric "no clamp", bool "default off", tuple of zeros) → state if you want it `@live`-preserved across reloads.
- Anything where zero-init would silently break the widget OR is a string → per-call arg with a daslang default.

**Found 2026-05-10**, dasImgui phase 0b.2 (Drag* widgets). Burned ~10 min before realizing field defaults weren't firing — the failure mode for numeric is "drag does nothing at runtime" which is much harder to catch than the string compile-time error. If you fix this in the macro (emit `var IDENT : T = T()` so field defaults fire), update or delete this card. Until then: state structs hold `@live`-able persistent values only, configs go to call-site args.

## Questions
- In dasImgui [widget] auto-emit, do struct field defaults like `format : string = "%.3f"` apply to the auto-emitted module global?
