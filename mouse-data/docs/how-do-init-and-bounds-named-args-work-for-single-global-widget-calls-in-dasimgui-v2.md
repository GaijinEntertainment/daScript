---
slug: how-do-init-and-bounds-named-args-work-for-single-global-widget-calls-in-dasimgui-v2
title: How do `init=` and `bounds=` named args work for single-global [widget] calls in dasImgui v2?
created: 2026-05-12
last_verified: 2026-05-12
links: []
---

**Single-global form only. `init=expr` seeds the auto-emitted state global's `value` field; `bounds=expr` seeds `bounds`. They flow into `ExprMakeStruct(useInitializer)` at module init, not into the per-frame call.**

```das
slider_float(VOLUME, (text="Vol", bounds=(0.0f, 1.0f), init=0.5f))
//                                ^^^^^^                ^^^^
//   pumped into the global's initializer
```

Macro behavior at expansion time (imgui_boost.das `WidgetCallMacro.visit`):
1. Walks the named-tuple args. For each `init=expr` and `bounds=expr`, extracts the expression and **strips it from the call**.
2. Builds `MakeFieldDecl`s and attaches them to the auto-emitted `ExprMakeStruct` for the global. The initializer becomes equivalent to `SliderStateFloat(value=0.5f, bounds=(0,1))`.
3. `useInitializer=true` is preserved so all other state struct field defaults still fire normally.

Sugar names recognised today: just `init` (renames to `value`) and `bounds` (matches the state field name 1:1). `range` is reserved in daslang's grammar and can't be a record-name — that's why `bounds=` is the second sugar instead of `range=`.

**Restrictions / scope:**

- **Single-global form only.** The indexed form (`slider_float(CHANNEL_VOL[i], ...)`) **errors** on `init=`/`bounds=` and tells the user to pre-populate the table imperatively in an `[init]` function: `CHANNEL_VOL[i].bounds = (0,1); CHANNEL_VOL[i].value = 0.5f`. Reason: the wrapper is emitted once per IDENT but called from N call-sites with possibly different inits, and silently picking-first-call's-value is worse UX than rejecting.
- Both args fire **only on first creation** — `useInitializer` runs once per global at module init. Subsequent renders of the same global don't re-apply init/bounds (matches API_REWORK §9.1 "value preserved across reload, bounds re-binds on reload" — V1 doesn't reload-re-bind yet; bounds stay at whatever the initializer set).
- Unmapped named args (`text=`, `flags=`, `format=` …) fall through to the call as today — destructured into positional widget-def args. The macro **only consumes names listed in `init_arg_to_field`** (`init`, `bounds`); everything else passes through untouched, so name collisions with widget-def parameters (e.g. WindowState.flags is also a `window` call parameter named `flags`) keep going to the call.
- State struct must have the field. If you pass `bounds=` to a button (ClickState has no `bounds`), the typer rejects the macro-synthesized `ExprMakeStruct` against `ClickState` and the error points at the user's call site.

Landed 2026-05-12 alongside indexed-form widgets. See `modules/dasImgui/widgets/imgui_boost.das:init_arg_to_field` and the `initFieldNames`/`initFieldExprs` loop in `visit()`.</body>
<parameter name="slug">dasimgui-widget-init-bounds-sugar-single-global-only

## Questions
- How do `init=` and `bounds=` named args work for single-global [widget] calls in dasImgui v2?
