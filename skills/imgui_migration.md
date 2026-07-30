# Migrating v1 daslang+imgui code to dasImgui v2

The v1 surface (`daslib/imgui_boost`, raw `imgui::*` calls inside an
`imgui_app(name) <| $() { ... }` host) is dead. dasImgui v2 wraps every
widget in a `[widget]` / `[container]` macro that auto-generates state and
registers telemetry. `widgets/imgui_lint.das` rejects v1 patterns at compile
time so old code can't silently coexist.

This page is the recipe. Read it before migrating any v1 dasImgui code, or
when you hit `IMGUI001` / `IMGUI002` in compile output.

## Lint codes

| Code | Trigger | Escape |
|---|---|---|
| **IMGUI001** | Call into the dead `imgui_boost::Fn` v1 module | None — must migrate |
| **IMGUI002** | Raw `imgui::Fn` call where `Fn` is not in `ALLOWED_IMGUI` (`widgets/imgui_lint.das:36`) | Per-file `options _allow_imgui_legacy = true` — scaffolding only |

`ALLOWED_IMGUI` is the curated set of raw imgui calls that have no v2 wrapper
and are intentionally host-agnostic — io queries, mouse/keyboard state,
cursor/layout queries, font/color helpers, draw-list primitives, demo/debug
entry points, drag-drop payload primitives, input-text callback primitives. If
your call belongs there, extend `ALLOWED_IMGUI` rather than opting out the
file.

## The migration table

| v1 pattern | v2 pattern | Notes |
|---|---|---|
| `imgui_app(name) <\| $() { ... }` | `require imgui/imgui_harness` + `init/update/shutdown` with `harness_init/begin_frame/new_frame/end_frame/shutdown` | See README "Usage" for the canonical shape |
| `if (Begin("Hello", null, flags)) { ... } End()` | `window(WIN_IDENT, (text="Hello", flags=...)) { ... }` | No `End()` — container is block-arg |
| `Text("hello")` | `text("hello")` (immediate) or `text(LABEL, (text="hello"))` (with telemetry) | The latter registers under `WIN/LABEL` path |
| `Button("Click")` | `if (button(BTN, (text="Click"))) { ... }` | `BTN.click_count` auto-tracks |
| `Checkbox("X", addr(g_flag))` | `checkbox(CHK, (text="X"))` | State lives in `CHK.value` |
| `SliderFloat("X", addr(v), 0, 1)` | `VOL.bounds = (0.0f, 1.0f); slider_float(VOL, (text="X"))` | State in `VOL.value` |
| `InputText("X", addr(buf), len)` | `input_text(NAME, (text="X"))` | State in `NAME.value` |
| `if (BeginCombo(...)) { ... EndCombo() }` | `dropdown_select(CMB, items, (text=...))` | One call; no End pair |
| `if (BeginTabBar(...)) { ... EndTabBar() }` | `tab_bar(BAR) { tab_item(TAB_A, ...) { ... } }` | Block-arg container |
| `BeginPopupModal(...)` | `popup_modal(POP, ...) { ... }` | Block-arg |
| `BeginChild(...) / EndChild()` | `child(KID, size=(w,h)) { ... }` | Block-arg |
| `Indent() / Unindent()` pair | `with_indent(amount) { ... }` | Stateless block-arg; `amount=0` defers to style |
| `PushID / PopID` pair | `with_id("scope") { ... }` | Path-prefix segment added |
| `SameLine()` | `same_line()` (no args) or `same_line((spacing=24.0f))` (named-tuple form) | Named-tuple args required for non-positional |
| `Separator()` | `separator(SEP)` | Optional telemetry ident |
| `Spacing()` | `spacing(SP_1)` | Optional telemetry ident |
| Manual TreeNode pair | `tree_node(NODE, (text="...")) { ... }` (block-arg) or `tree_node_open(...)/tree_pop()` (imperative pair) | Prefer block-arg form |
| `TableNextRow / TableSetColumnIndex / cell calls` | `data_table(GRID, cols) { for (row in rows) { text(NAME[i], ...); text(QTY[i], ...) } }` | `[container]` does the row/column bookkeeping; cells path under `WIN/GRID/...` |

## Lifecycle: pick `harness_*` or `live_*`

Two equivalent ways to drive a frame loop:

- **`require imgui/imgui_harness`** — folds the frame into 3 calls
  (`harness_begin_frame`, `harness_new_frame`, `harness_end_frame`). Supports
  `--headless`. **Use this for new code, feature demos, tests, and most apps.**
- **`require live/glfw_live` + `require imgui/imgui_live`** — manual frame
  with separate `ImGui_ImplGlfw_NewFrame` / `NewFrame` / `Render` / `glClear` /
  `live_imgui_render` calls. Used by
  `examples/tutorial/*.das` so the tutorial can demonstrate the full stack.
  **Use this only when you need explicit control** over each backend call
  (custom GL clear color per frame, custom font atlas building, etc.).

## Migration recipe

When asked to migrate a v1 file:

1. **Compile first** — note every IMGUI001 + IMGUI002 hit. Each is a concrete
   call to fix.
2. **Pick the lifecycle** — harness for new code, `live_*` only when explicit
   frame control is needed.
3. **For each rejected call, find the v2 wrapper** — `grep -l "name_of_widget"
   widgets/imgui_*.das` finds the `[widget]` / `[container]` macro that wraps
   it. The migration table above covers the common cases.
4. **Hoist state to globals** — v2 macros auto-generate the widget state
   struct (`BTN_IDENT.click_count`, `SLIDER_IDENT.value`, `CHK_IDENT.value`).
   Remove the v1 `var g_*` globals; the macro emits them.
5. **Indexed widgets need an explicit table global** — `[widget]` macros do
   NOT auto-emit table forms. Add `var private NAME : table<int;
   NarrativeState>` at module scope before using `widget_fn(NAME[i], ...)`.
6. **Recompile** — expect zero lint hits. If a raw call legitimately has no v2
   wrapper (drawlist primitives, io state queries), check if it's in
   `ALLOWED_IMGUI`; add it there if it's truly host-agnostic.
7. **Last resort: `options _allow_imgui_legacy = true`** — scaffolding for
   in-progress migrations. Target state is zero opt-outs; track via `git grep
   _allow_imgui_legacy`.

## Intentional opt-outs

Two example files keep `_allow_imgui_legacy = true` permanently — these are
educational, not gaps:

- `examples/tutorial/custom_widgets.das` — teaches building widgets from raw
  imgui primitives.
- `examples/features/widget_no_ident.das` — exercises the STYLE001-rejected
  `text((text=...))` form for didactic value.

If you add a new permanent opt-out, document the WHY inline at the `options`
line and in `CLAUDE.md` "Followup-tracked gaps".

## Finding remaining unmigrated code

```bash
git grep -l _allow_imgui_legacy                # files still using v1 escape
grep -rn "imgui::" examples/                   # raw imgui calls inside opt-outs
```

## Where the v2 surface lives

- `widgets/imgui_widgets_builtin.das` — `[widget]` macros for primitives
  (button, text, checkbox, sliders, inputs, etc.)
- `widgets/imgui_containers_builtin.das` — `[container]` macros (window,
  child, tab_bar/tab_item, popup_*, menu, etc.)
- `widgets/imgui_layout_builtin.das` — layout helpers (with_indent, group,
  list_clipper, tree_node, etc.)
- `widgets/imgui_table_builtin.das` — `data_table` + `table_*` helpers
- `widgets/imgui_boost_v2.das` — the macro layer entry point: the
  `WidgetCallMacro` that rewrites `widget(IDENT, (named=tuple))` calls
  (required transitively via `imgui_harness` / `imgui_live`)
- `widgets/imgui_harness.das` — the 5-call harness API (`harness_init` /
  `harness_begin_frame` / `harness_apply_synth_io` / `harness_new_frame` /
  `harness_end_frame` / `harness_shutdown`)
- `widgets/imgui_live.das` — the live-reload lifecycle (`live_begin_frame`,
  `begin_frame`, `apply_synth_io_override`, `end_of_frame`, `live_end_frame`)
- `widgets/imgui_lint.das` — `IMGUI001` / `IMGUI002` enforcement +
  `ALLOWED_IMGUI` carve-out
