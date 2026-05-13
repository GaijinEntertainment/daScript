# Phase 0a — define_widget smoke

End-to-end verification for the [widget]-annotation pipeline shipped in Phase 0a.
Side-by-side with `example/phase00_button/` (the hand-rolled reference), this
demo trades ~220 lines of state + render + live-command code for ~30 lines of
widget invocations. The macros (in `widgets/imgui_boost.das`) handle:

- `[widget]` function-annotation → registers a per-kind `WidgetCallMacro` that
  intercepts `<kind>(IDENT, ...)` calls.
- Body wrap → prepends `widget_prelude(label)`, appends `widget_postlude(label,
  snap)` to `func.body.finalList`. bbox + hex_id + hover/active/focus capture
  flow into the per-frame registry.
- Built-ins via `[widget]`: `button`, `slider` (float), `slider_int`. No
  internal-only path — user widgets follow the same pattern.

## Manual smoke (Windows)

```powershell
# Reinstall the package after edits to dasImgui itself, OR copy individual
# .das files directly to D:\Work\daScript\modules\dasImgui\widgets\ for fast
# iteration (Boris's preferred dev loop).
& "D:\Work\daScript\bin\Release\daslang.exe" `
    "D:\Work\daScript\utils\daspkg\main.das" -- `
    install --global --force "D:\DASPKG\dasImgui"

# Launch the example
& "D:\Work\daScript\bin\Release\daslang-live.exe" `
    "D:\Work\daScript\modules\dasImgui\example\phase0a_define_widget\main.das"
```

Expected stdout on launch:

```
live_api: server started on port 9090
daslang-live: lifecycle mode (init/update/shutdown)
```

## Live-command verification

In a second terminal:

```powershell
# Snapshot — should list all 3 widgets with bbox + hex_id
Invoke-WebRequest -Uri "http://localhost:9090/command" -Method Post `
    -Body '{"name":"imgui_snapshot"}' -UseBasicParsing | Select-Object -Expand Content
```

Expected (truncated):

```json
{
    "globals" : {
        "Save"            : { "kind": "button",     "hex_id": ..., "bbox_x0": 68, "bbox_y0": 130, ... },
        "Speed (int)"     : { "kind": "slider_int", "hex_id": ..., ... },
        "Volume (float)"  : { "kind": "slider",     "hex_id": ..., ... }
    },
    "frame" : <int>
}
```

## What works in v0a

- `[widget] def kind(var state : StateType; text : string) : bool { ... }`
  registers + wraps the function. Multiple widget kinds coexist.
- `kind(IDENT, [text="..."])` at user call sites compiles + dispatches via the
  call_macro to the underlying function.
- Per-frame registry populates correctly — `imgui_snapshot` returns geometry
  + kind tag + hex_id for every rendered widget.
- All 3 widget kinds (button, slider float, slider_int) work in one program.
- **ImGui ctx survives reload.** `require imgui/imgui_live` plus
  `live_imgui_init(live_window)` / `live_imgui_shutdown()` in `init`/`shutdown`
  preserves the C++ context across daslang-live reloads (mirrors
  `live/glfw_live`'s pattern). Font atlas, ImGui window positions, and
  active-widget state all persist.

## Threading

`imgui_click` and `imgui_set` write `pending_click` / `pending_value` on the
state global from the live-command handler. `daslang-live`'s
`live_dispatch_command` drains queued commands on the GLFW main thread (the
same context that runs `update`), so these writes do **not** race with the
render path that reads them. Confirmed against `utils/daslang-live/main.cpp`
lifecycle + Boris's prior knowledge; no main-thread queue marshaling needed
in the macro layer.

## Known v0a gaps (follow-up chunks)

- **Registry key is the LABEL, not the IDENT.** The body wrap uses the function's
  second parameter (conventionally `text`) as the registry key. Two widgets with
  the same label clash. Phase 0a.1 fix: thread the call-site IDENT through to
  the wrapped function (likely via append-as-last-arg + call_macro emits the
  string literal).
- **L2/L3 dispatchers not wired.** `imgui_click` and `imgui_set` return "unknown
  target" because `g_button_dispatchers` / `g_slider_dispatchers` are empty.
  Phase 0a.1 should emit per-global `[init]`-time registration of each widget's
  pending-click / pending-value closure.
- **Globals must be pre-declared.** The call_macro CAN emit a module-scope
  global (`add_variable` works), but other references to the same identifier
  in the same compile pass fail to resolve. Until that ordering is sorted, the
  example declares globals manually.
- **Kind-specific snap fields stay 0.** `clicked` / `click_count` for buttons,
  `value_*` / `bounds_*` for sliders aren't filled — the wrap is generic and
  doesn't know which fields are kind-relevant. Add per-kind postlude in 0a.1.
- **No `define_widget` user-side syntax.** Per the original API_REWORK §4.9
  sketch (`define_widget(button, ButtonState) $(state, ...) { body }`), users
  would write the same shape. Daslang call_macros only fire inside expressions,
  so that exact shape can't compile. The `[widget]` function-annotation form is
  the equivalent that works — same end-user feel, idiomatic daslang.

## Comparing to phase00_button

```
example/phase00_button/main.das        ~220 lines (state + render + commands hand-rolled)
example/phase0a_define_widget/main.das ~ 90 lines (widgets via [widget] decl)
```

Most of the line reduction is in the macro layer, not the example — but the
example shape is the user-facing payoff: one line per widget call site, no
hand-written render function, no per-widget live_command boilerplate.
