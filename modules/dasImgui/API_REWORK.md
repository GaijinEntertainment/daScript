# dasImgui Boost API Rework — Master Plan

**Status:** design-locked, pre-implementation. Created 2026-05-08.

This is the master plan-doc for the dasImgui boost-API redesign. It captures every pinned decision from the kickoff discussion plus the phased implementation plan. Living document — update as decisions evolve.

Companion docs (when they appear):
- Per-chunk plans live under `~/.claude/plans/dasimgui_*.md`
- Design strawmans / fake-tutorials (if/when needed) live in `API_MISSING.md` (separate doc, not yet created)

---

## 1. Goals

The current `daslib/imgui_boost` is **anemic** — ~500 lines of `Begin`/`Menu`/`MenuItem`/`HelpMarker` block-arg wrappers and a handful of callback-shim structs (`ImGuiInputTextBuffer`, `ImGuiComboGetter`, `ImGuiPlotGetter`, `ImGuiSizeConstraints`) whose layouts are pinned to C++-side structs in `bind/`. There is no widget-state model, no telemetry hook, no identifier-as-id discipline. Examples write idiomatic-C++-style ImGui inside daslang — manual `Begin`/`End` pairs, `static_let` for per-widget state (a historical macro-day artifact), `unsafe(addr(field))` everywhere. We need a daslang-native API. **The migration is full-rewrite, not additive** — the block-arg wrappers re-emerge from `define_widget`, and the ABI-paired shims get rewritten with their C++ partners updated in lockstep.

While we're at it, we ship five adjacent capabilities:

1. **Idiomatic boost API** — block-arg containers, declarative state, ergonomic loops.
2. **Telemetry** — runtime introspection: which widgets are on screen, in what state. Plus interactive control surface: press buttons, move sliders, edit text, open/close windows.
3. **Transport** — explicit `send_imgui_command` / `receive_imgui_command` wired to a default (daslang-live) transport, with a swappable wrapper for custom transports.
4. **Playwright** — scriptable UI testing on top of telemetry + transport.
5. **Visual aids** — mouse trail, element highlight, tutorial narration overlays. Built-in video recording is *deferred* (no test_engine, see §3).

---

## 2. Principles

These shape every decision below.

- **Defaults secure.** No telemetry unless the app explicitly calls `imgui_telemetry_serve()`. No remote attack surface unless the user opted in. "No install — no telemetry — game does not get hacked."
- **Defaults idiomatic.** The 80% case writes one line per widget, no boilerplate. Ceremony is opt-in, not opt-out.
- **No internal-only paths.** Anything the boost layer's built-ins can do, user-defined widgets can do. `define_widget` is the contract; `button` and `slider` are *examples* of using it.
- **Identifier is identity.** Widgets are addressed by their daslang identifier (which is also their telemetry path), never by 32-bit hashes. Magic numbers are unacceptable in scripts and tutorials.
- **Class is so not daslang.** The transport interface and other extension seams are lambdas / structs of function pointers, not virtual classes. (Classes exist in daslang because users want them, not because the language is class-oriented.)
- **State lives in daslang, not imgui.** Widget values are real daslang globals — grep-able, RTTI-walkable, hot-reload-friendly via `@live`, persistable via existing serialization. ImGui never owns user-visible state.
- **Boost wrappers do the registry work.** Since we wrap every widget call, we naturally know its bbox/hover/focus post-call (via public `GetItemRectMin/Max` + `IsItemHovered/Active/Focused`). No hooking ImGui internals.

---

## 3. Constraints

- **`imgui_test_engine` is off-limits.** Gaijin's annual turnover exceeds the $2M free-license threshold. Cannot vendor, link, or include. A reference-only checkout at `E:\imgui_test_engine\` is allowed for clean-room reimplementation guidance.
- **Public `imgui.h` only.** `imgui_internal.h` (with `ItemAdd`, `ButtonBehavior`, etc.) is *not* bound today. The `InvisibleButton + DrawList` pattern covers ~95% of custom widgets without it. Add internal binding only when a real use case bumps into it.
- **No CI on dasImgui repo.** Branches are for backup, not for review. Review happens in fresh Claude sessions playing "better copilot" — see §11.
- **GLFW main-thread requirement.** The imgui app runs the frame loop on the GLFW thread. All multi-threading (HTTP server, command queue) is around it, not on it.

---

## 4. The boost API surface

### 4.1 Containers (block-arg, auto-end)

```das
window(SETUP_WIN, label="Setup") {
    tab_bar(MAIN_TABS) {
        tab_item("Render") { ... }
        tab_item("Audio")  { ... }
    }
    tree_node(ADV) { ... }
    collapsing_header(EXTRAS) { ... }
}
```

The block runs once per frame. Begin/End is implicit in the block-arg. Same shape for `child`, `group`, `menu_bar`/`menu`, `popup`, `modal`, `tab_bar`/`tab_item`.

### 4.2 Stateful inputs

State lives in a macro-emitted module global named by the first argument:

```das
slider(RPS_SLIDER, label="RPS", range(0, 10), init=0.1f) $(rps) {
    // rps is the live value this frame; use inline
    if (rps > 5.0f) { tooltip("Going fast!") }
}

// equivalent return form (no block):
let rps = slider(RPS_SLIDER, label="RPS", range(0, 10), init=0.1f)

// read state from anywhere later:
let speed = RPS_SLIDER.value
```

The `slider(IDENT, ...)` macro emits at module scope:

```das
@live variable private RPS_SLIDER : SliderState<float> = SliderState(value=0.1f, min=0.0f, max=10.0f)
```

…then rewrites the call to `slider_render(unsafe(addr(RPS_SLIDER)), label="RPS") $(rps) { ... }`.

Both block and return forms are supported. Block form is idiomatic for tight side-effect coupling; return form for "value flows to N consumers without N levels of nesting."

### 4.3 Triggers (stateless)

```das
button(SAVE_BTN, label="Save")     // returns bool; clicked-this-frame
button(label="Save")               // id optional for triggers; still telemetry-visible
```

If an identifier is given, the button gets a global (with hover/click counters for telemetry). If no identifier, it's still tracked in the per-frame registry but has no persistent state.

### 4.3a Output (text_show)

For app-driven text values (status lines, computed labels) that need to be telemetry-readable:

```das
text_show(STATUS_TEXT, value="saved")
// or store + show:
STATUS_TEXT.value = compute_status()
text_show(STATUS_TEXT)
```

`text_show` mirrors `text_input` for the read-only direction. Tests assert via `expect_value(app, STATUS_TEXT, "saved")`. The state global holds the displayed string; raw `imgui::Text(s)` calls *don't* register and aren't telemetry-visible.

### 4.4 Loops — array and table forms

The macro dispatches on the index expression's type. **Both forms use `table<K; T>` storage** — int and string just differ in key type. Uniform insert-on-read semantics, no auto-grow asymmetry between dense and sparse indexing:

```das
// integer index → table<int; SliderState>
for (i in range(8)) {
    slider(CHANNEL_VOL[i], label="Vol", 0..1, init=0.5f) $(v) { ... }
}

// string index → table<string; SliderState>
for (track in tracks) {
    slider(TRACK_VOL[track.name], label="Vol", 0..1, init=0.5f) $(v) { ... }
}
```

Telemetry path: `Mixer/CHANNEL_VOL[3]` or `Mixer/TRACK_VOL[lead_synth]`. (Dense int indexing in a hot path could be `array<T>`-backed in a future opt; not worth the asymmetry today.)

Auto-grows on first access to a new index.

**Single-global widget renders exactly once per frame — period.** Two failure modes collapse into one rule:

1. Single-global widget inside a `for` loop (would render N times).
2. Single-global widget called from two distinct sites (e.g. shown in two windows simultaneously).

Both → **runtime panic** at end-of-frame when the registry detects N>1 renders of the same global. When the macro can see the misuse lexically (e.g. `slider(IDENT, ...)` directly inside a `for` where `IDENT` is declared as a single global, not subscripted), it `macro_error`s at expansion with a fixit pointing to `IDENT[i]` / `IDENT[key]`. When the misuse is hidden behind helpers, generators, or block-args the macro can't trace, runtime catches it. Loud, not a warning — telemetry needs an unambiguous path → widget mapping or test commands can't locate the right widget. Want the same widget kind in two places? Use the array/table form.

### 4.5 Identifier flags (dotted suffix)

Widget identifiers are conventionally `UPPER_SNAKE_CASE`. The macro takes its first argument **unparsed** and splits on `.` — the leading token is the identifier (and telemetry path component); trailing tokens are flags on the generated global:

```das
slider(RPS, ...)                  // default: variable private, @live
slider(RPS.PUBLIC, ...)           // exported across modules
slider(RPS.NOTLIVE, ...)          // skip @live; rebinds on reload
slider(RPS.PUBLIC.NOTLIVE, ...)   // both
```

The flag set starts small (`PUBLIC`, `PRIVATE`, `NOTLIVE`) and grows on demand. Telemetry path uses only the bare identifier (`Setup/RPS`, never `Setup/RPS.PUBLIC`). The dotted form scales to any future per-global modifier without claiming new syntactic positions.

### 4.6 Layout helpers (block-scoped)

Each region is a positional block literal (`${ ... }`):

```das
split_h(left_pct=0.3,
    ${
        // left pane content
    },
    ${
        // right pane content
    })

columns(3,
    ${ /* col 0 */ },
    ${ /* col 1 */ },
    ${ /* col 2 */ })

dock_left(width=200) {
    // sidebar content (single trailing block, idiomatic)
}
```

Two-region helpers (`split_h`, `split_v`) take exactly two block args. Variable-arity helpers (`columns(N, ...)`) take N positional blocks; the macro errors at expansion if the count doesn't match. No declarative tree DSL. Immediate-mode is incompatible with retained layout.

### 4.7 Style helpers (block-scoped)

`with_style` is a `[call_macro]` taking a variadic list of `(key, value)` tuples. Each tuple dispatches to `PushStyleColor` or `PushStyleVar` based on the key's enum type (`ImGuiCol_` → color, `ImGuiStyleVar` → var). One block-scoped push/pop pair total, regardless of N styles:

```das
with_style(
    (ImGuiCol_.Button,         red),
    (ImGuiCol_.Text,           white),
    (ImGuiStyleVar.Rounding,   4.0f),
    (ImGuiStyleVar.FramePadding, float2(8, 4))
) {
    button(SAVE_BTN, label="Save")
}
```

Existing imgui themes (`StyleColorsDark`/`Light`/`Classic`) stay reachable. No declarative theme system.

### 4.8 ID overrides

Default: id = identifier. Override only when needed:

```das
slider(RPS_SLIDER, label="Speed", id="rps_v1", 0..10, init=0.1)
//                                ^^^^^^^^^^ override (e.g. for stable scripts when label changes)

with_id("section_a") {
    button(SAVE_BTN, label="Save")  // path: "Setup/section_a/SAVE_BTN"
}
```

`with_id(s)` is the explicit ID-stack push when the macro form isn't enough (rare; loops use the array/table form instead).

### 4.9 User-extensible widgets

The boost provides `define_widget` as the canonical extension mechanism. **Built-in widgets are defined using it** — no internal-only path:

```das
// shipped with the boost
define_widget(button, ButtonState) $(state, label : string) {
    let clicked = ImGui::Button(label)
    if (state.pending_click) {
        state.pending_click = false
        return true   // L2 short-circuit (see §6)
    }
    return clicked
}

// user-defined custom widget
define_widget(my_color_picker, MyColorPickerState) $(state, label : string) {
    invisible_button(label, ImVec2(60, 20))
    if (is_item_clicked()) { state.open = !state.open }
    let dl = get_window_draw_list()
    add_rect_filled(dl, get_item_rect_min(), get_item_rect_max(), state.color)
    if (state.open) { /* popup with sliders */ }
}
```

Power users can hand-write the call_macro if `define_widget` doesn't fit — the escape hatch stays open.

The exact contract — what the macro injects around the user body (id push, hex_id/bbox capture, hover/active/focus, registry insert), what the user body sees as `state`, how state-pending fields like `pending_click` flow back — is **deferred to Phase 0a**. The button case above is enough to anchor design; the stateful contract lands as code, then doc backfills.

---

## 5. Telemetry

### 5.1 Snapshot model

Always-full on demand, served as JSON:

```json
{
  "frame": 1284,
  "globals": {
    "Setup/RPS_SLIDER": {
      "kind": "slider<float>",
      "value": 0.42,
      "range": [0, 10],
      "bbox": [200, 50, 400, 70],
      "hex_id": "0x4f3a2b1c",
      "hover": false,
      "active": false,
      "focus": false
    },
    "Setup/SAVE_BTN": { "kind": "button", "bbox": [...], "hex_id": "0x..." },
    "Setup": { "kind": "window", "open": true, "pos": [...], "size": [...] }
  },
  "io": {
    "mouse_pos": [320, 180],
    "mouse_buttons": [false, false, false],
    "active_widget": "Setup/RPS_SLIDER"
  },
  "extras": {
    "Setup/NAME_INPUT": { "cursor": 7, "selection": [3, 7] },
    "Setup/LIST": { "scroll_y": 120.0 }
  }
}
```

- **`globals`** — RTTI walk over module globals of UI-state types (auto-discovered via `widget_state_types` registry populated by `define_widget`).
- **Per-frame registry fields** (bbox, hover, active, focus, hex_id) — captured by every boost wrapper after the underlying ImGui call, into a frame-local table.
- **`io`** — `ImGuiIO` mirror: mouse pos / buttons, active widget id, focused window.
- **`extras`** — widget-kind-specific read-only metadata (text-input cursor, scroll offset, tree expansion, tooltip content). Populated per widget kind.
- **`hex_id`** — `ImGui::GetID()` value for every widget. Lets clients (Claude, scripts) target widgets the user never named.

### 5.2 Sync semantics for commands

**Client-polling model.** Server reports `{quiescent, frame, pending_coroutines, active_id}` per `imgui_await` probe; the client (`await_imgui` in `widgets/imgui_transport`) drives the wait loop until quiescent (no active drag, popups settled, animations done) with a 5s default timeout. The dispatch handler runs on the GLFW main thread between frames (§3 invariant), so a server-side `while (!quiescent) sleep(...)` would deadlock the loop that advances frames — server-side blocking is structurally unavailable.

Per-command override modifiers extracted at the dispatch boundary by `with_await(input, payload)` in `imgui_boost_runtime.das`:
- `await=N` — surfaces `await_frames: N` in the response so the client polls for `g_frame ≥ baseline + N`
- `await_until=<predicate>` — surfaces in the response; today only `"quiescent"` is implemented
- `timeout_sec=N` (default 5.0) — surfaces in the response wrap; client honors as the poll deadline
- `fire_and_forget=true` — server skips the merge entirely; response is the raw payload, no `await_*` keys, no client-side wait

On timeout: the client returns `false` from `await_imgui` (no panic). Test harnesses that want hard-fail on hang wrap the await in their own assertion.

---

## 6. Transport and commands

### 6.1 Server side — reuse `live_api`

No new HTTP server. The dasLiveHost `live_api` debug agent (port 9090 by default) handles transport. **No new command queue either** — `live_dispatch_command` (in `dasLiveHost/live/live_commands.das`) already drains posted commands on the main context at frame boundaries. Imgui commands ride that same queue; the GLFW main-thread requirement (§3) is satisfied by the existing infrastructure. The boost ships imgui-flavored `[live_command]` functions reachable via `POST /command` with `{"name":"imgui_<verb>","args":{...}}`:

| Command | Action |
|---|---|
| `imgui_snapshot` | Returns full JSON snapshot |
| `imgui_set` | Mutates widget state (`{"target":"Setup/RPS_SLIDER","value":0.7}`) |
| `imgui_click` | Sets pending-click flag for a button (L2) |
| `imgui_drag` | Synthesizes drag events (L1) |
| `imgui_type_text` | Synthesizes character input (L1) |
| `imgui_focus` | Force-focus a widget |
| `imgui_open` / `imgui_close` | Toggle window state |
| `imgui_await` | Returns `{quiescent, frame, pending_coroutines, active_id}` snapshot for client-side polling |

Activation is explicit:

```das
require imgui/imgui_boost
require live/live_api

[export]
def init() {
    live_create_window("My App", 800, 600)
    imgui_telemetry_serve()    // installs imgui_* commands; ensures live_api is up
}
```

**No install → no telemetry → no remote control.**

### 6.2 Client side — lambda transport

```das
// transport is a lambda
let transport = live_api_transport("http://localhost:9090")

// send a command
let result = send_imgui_command(transport, "imgui_click", {"target" => "Setup/SAVE_BTN"})

// receive snapshot
let snap = receive_imgui_command(transport)

// wait for a condition
await_imgui(transport, until="quiescent")
```

`transport` is `lambda<(action : string; payload : JsonValue?) : JsonValue?>`. Default impl `live_api_transport(url)` POSTs to `/command`. Users override for in-process buses, mocked tests, custom IPC, etc. (In-process drive = "open transport to yourself.")

### 6.3 How clicks actually work

Three implementation levels, picked per command:

- **L1 — synthesize ImGuiIO events.** `AddMousePosEvent` + `AddMouseButtonEvent` (both public in `imgui.h`). Move cursor to widget bbox center, push down, next frame push up. Works for *anything*. Warps cursor briefly; needs ≥2 frames; can race user input.
- **L2 — boost wrapper short-circuit.** Widget's `ButtonState` carries a `pending_click` flag. The boost `button()` wrapper checks it and returns true regardless of imgui's hit-test. Single frame, no cursor warp, deterministic. Works only for boost-wrapped widgets.
- **L3 — direct state mutation.** Write the daslang global. Sliders/checkboxes/inputs paint the new value next frame. ImGui never knows it was driven externally.

Mapping:
- `imgui_set(target, value)` → **L3**
- `imgui_click(IDENT)` → **L2** (boost-wrapped button)
- `imgui_click(hex_id)` → **L1** (arbitrary widget without a daslang global)
- `imgui_drag` / `imgui_type_text` / multi-frame interactions → **L1**

Default rule: try L2/L3 first, fall back to L1 when widget isn't boost-wrapped.

---

## 7. Playwright

### 7.1 Where tests, examples, tutorials live

Everything ships in the **dasImgui repo** (not daslang):

- `example/` — user-facing demo apps. Already exists; gets new entries in Phase 0b.
- `tutorials/` — RST + `.das` step-by-step lessons (analog to daslang's `tutorials/<area>/`). New in a later phase.
- `tests/` — playwright tests. Use **dastest** as the runner; reference example apps by relative path (e.g. `../example/save_demo/main.das`). Examples double as test fixtures.

Tests import the boost: `require imgui/imgui_boost`. Imperative API:

```das
require imgui/imgui_playwright

[test]
def test_save_flow(t : T?) {
    with_imgui_app("modules/dasImgui/examples/save_demo/main.das") $(app) {
        click(app, "SAVE_BTN")
        expect_value(app, "STATUS_TEXT", "value", "saved")
        type_text(app, "NAME_INPUT", "hello")
        expect_value(app, "NAME_INPUT", "value", "hello")
    }
}
```

The Phase 3 surface settled on a block-form lifecycle (`with_imgui_app <| $(app) { ... }`) rather than the originally-sketched noun form (`launch_imgui_app(...)` + `defer() { app |> shutdown() }`): daslang-live IS the launcher, and `popen_argv` inside the block already runs it. `expect_value` takes an explicit `field` arg (4-arg form) — no per-kind default-field table. `type_text` auto-focuses its target before sending characters; the target app must call `advance_coroutines()` each frame for chars to land (Phase 2 coroutine).

### 7.2 Process model

**Out-of-process only.** `launch_imgui_app(...)` spawns a daslang-live subprocess, drives it via the transport. In-process testing is "open transport to yourself" — same API surface, lambda transport routes to local function calls. No special architectural mode.

### 7.3 Headless / CI

Hidden-window via `glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE)` plus offscreen FBO. Real imgui frame loop, no display. Screenshots/captures still work because the FBO is render-target-attached.

---

## 8. Visual aids (tutorial mode)

Separate opt-in: `imgui_visual_aids_serve()`. Distinct from telemetry — telemetry is for tests + dashboards; visual aids are for tutorial videos.

**Auto-highlight on every command** (toggleable). When a command targets a widget, draw a colored rect around its bbox for N frames. Plus explicit:

```das
imgui_highlight("Setup/SAVE_BTN", duration=2s, color=red)
imgui_mouse_trail(enabled=true, fade=0.8f, color=ImVec4(1, 1, 0, 1))
narrate("Click here to save your work", target="Setup/SAVE_BTN", duration=3s)
```

`narrate` draws a callout box pointing to the widget's bbox with the given text.

All overlays use `ImGui::GetForegroundDrawList()` — public API, no test_engine.

**Recording — done (Phase 5).** Streaming APNG video recording lives in `live/opengl_live` as three live commands: `record_start` / `record_stop` / `record_status`. Pure live-host concern (works for every daslang-live'd app, not just dasImgui), backed by a new APNG writer in `dasStbImage` (`stbi_apng_begin/frame/end/dropped`). Encode + file I/O run on a worker thread; render-loop overhead per captured frame is just glReadPixels + memcpy to a bounded queue. Output is single-file, lossless, plays natively in Chrome / Firefox / Safari.

---

## 9. Cross-cutting

### 9.1 Hot-reload preservation

Macro emits `@live` on every widget global by default. Window positions, slider values, tree state survive `live_api` reloads. Opt out per-widget with the `.NOTLIVE` flag (see §4.5): `slider(RUN_TIMER.NOTLIVE, ...)` rebinds on every reload.

**Value vs bounds.** State globals split user-editable value from source-defined config:

```das
struct SliderState<T> {
    @live value  : T          // preserved across reload
    @live bounds : tuple<T;T> // re-initializes from source on reload
    @live init   : T          // re-initializes from source on reload
}
```

When source edits a slider's `range(0,10)` to `range(0,20)`, the new bounds flow in on reload while the user's current `value` sticks. If `value` is now out-of-range, **clamp on first read** to the new bounds (no silent drift; no preserved-but-invisible state). Same pattern applies to every kind: input bounds, max-length, color-picker mode flags etc. all rebind; user-touched values preserve.

### 9.2 Persistent UI state across runs

Use ImGui's built-in `imgui.ini` for window/dock/tree/tab state. App-specific widget values are plain daslang globals — users serialize them themselves if they want config persistence. Don't bake.

### 9.3 Visibility of generated globals

Default `variable private`. Cross-module reads opt in via the `.PUBLIC` flag on the identifier (see §4.5): `slider(RPS.PUBLIC, ...)` makes the generated global `[export]`-equivalent.

### 9.4 Backwards compatibility — hard cutover at v2.0

The rewrite ships as **dasImgui v2.0**. There is no compat shim:

- `require dasImgui/daslib/imgui_boost` (v1.x path) just breaks.
- New surface lives at `require dasImgui/imgui/imgui_boost`.
- daspkg index pins existing users to v1.x until they migrate; new projects get v2.0+.
- No deprecation phase, no warn-on-import bridge. v1 stays installable via explicit version pin for as long as it's useful.

Migration burden is on downstream callers. Justified because v1.x is anemic enough that most callers reach past it into raw `imgui::*` anyway — there's not much "v1 surface" to migrate, just paths.

---

## 10. Phase plan

Each phase lands as a branch in `D:\DASPKG\dasImgui`, pushed for backup. Reviewed in a fresh Claude session before being declared done.

| Phase | Scope | Out |
|---|---|---|
| **00 — proof of concept** | Tiny imgui_app, one button, hand-written everything (no macros): `ButtonState` global, `render_demo_btn()` capturing bbox/hex_id/hover/clicked into per-frame registry, **hand-rolled snapshot for that one widget** (no RTTI walk), pending-click flag (L2 short-circuit), `[live_command] imgui_snapshot` + `imgui_click` via existing `live_api`. End-to-end demo: curl click → next-frame button reports clicked. | macros, multi-widget, L1, L3, await, visual aids, playwright, RTTI snapshot |
| **0a — define_widget core** | `define_widget` macro shipping; migrate `button` and `slider` as proof-of-concept built-ins. State globals, identifier-as-id, block-arg auto-end. Migrate one example. | other widgets |
| **0b — built-ins migrated** | All of imgui's widget surface migrated to `define_widget`: checkbox, radio, combo, list_box, input_text/int/float, color_edit/picker, drag, slider variants, plot_lines, tree_node, collapsing_header, menu, tab_bar/item, popup/modal, tooltip, etc. **ABI-paired shims rewritten in lockstep with their C++ partners** in `bind/` — `ImGuiInputTextBuffer`, `ImGuiComboGetter`, `ImGuiPlotGetter`, `ImGuiSizeConstraints` all get redesigned around the new state-global model (no more `addr` of a stack `var buf`). Update all examples. Breaking change for downstream `require dasImgui/daslib/imgui_boost` users — coordinate with `daspkg` index. | new capabilities |
| **1 — registry + snapshot** | Per-frame registry table populated by every boost wrapper; **RTTI walk replaces the hand-rolled snapshot from Phase 00** — auto-discovers globals of any registered widget-state type; per-widget `extras` for kinds that have them (text-input cursor, scroll offset, tree expansion). `imgui_snapshot` returns the full picture. | commands beyond snapshot |
| **2 — full command set** | `imgui_set` (L3), `imgui_click` for arbitrary hex_ids (L1), `imgui_drag`, `imgui_type_text`, `imgui_focus`, `imgui_open`/`close`, `imgui_await` with predicate support. Lambda transport interface, `live_api_transport` default impl. | playwright |
| **3 — playwright boost** | **DONE 2026-05-10.** Public `imgui/imgui_playwright` (sibling module, not folded into `imgui_boost`). Block-form `with_imgui_app <| $(app) { ... }`, `click`, `type_text` (auto-focus target, 2-frame settle), `set_value`, `open_widget`/`close_widget`, `drag`, `focus`, `await_probe`, `post_command`, `reload`, full poll-until family (`wait_until` + typed sugars + `wait_for_payload_value`), snapshot navigation (`find_widget`/`widget_exists`/`widget_payload_field`/`widget_rendered`). New assertion helpers panic on timeout with focused failure UX: `expect_value` (4-arg, explicit field), `expect_render`, `await_quiescent`. 27→28 dastest cases green (added `test_save_demo`); 27 prior tests cut over from private `tests/integration/live_driver.das` (deleted, no compat shim per §9.4). Raw-curl smoke ships as cross-tool diagnostic (`smoke_curl.ps1` / `smoke_curl.sh`). **Deferred:** noun-form `launch_imgui_app` (daslang-live IS the launcher; block form satisfies every test); hidden-window / offscreen FBO (rolls into Phase 4 where FBO is needed anyway); programmatic negative-path tests for assertion panic UX (`try/recover` ruled out — panic semantics, lint rule incoming). | visual aids |
| **4 — visual aids** | `imgui_visual_aids_serve()`, auto-highlight on commands, mouse trail, explicit `imgui_highlight`, `narrate` callouts. | recording |
| **5 — recording / extras** | **APNG recording — DONE 2026-05-11.** Streaming APNG writer in `dasStbImage` (`stbi_apng_begin/frame/end/dropped`, ~300 lines reusing stb's CRC32+zlib internals, worker thread + bounded queue). Live commands `record_start` / `record_stop` / `record_status` in `live/opengl_live` (sibling to `screenshot`, fps throttle + max_seconds auto-stop). dastest covers C-API correctness (chunk parse, default-image fallback, RGB+RGBA, error paths, overflow drops, parallel writers); end-to-end smoke recorded 225 frames at 15fps over visual_aids_tour with 0 drops. Theme/layout helpers not yet shipped — final polish. | doc generation |
| **6 — docs + website** | `imgui2rst` tool (parallel to daslang's `doc/reflections/das2rst.das`) generating RST from `//!` comments in dasImgui modules. Stitches in tutorials. Output linked from the main daslang website (module catalog with per-module docs). | n/a |

---

## 11. Workflow

Per `~/.claude/projects/d--Work-daScript/memory/project_dasimgui_workflow.md`:

- **Branch per chunk** in `D:\DASPKG\dasImgui` (canonical work copy). Push to remote for backup, not for review.
- **Fresh-session review** between chunks. New Claude session reads the diff and gives independent feedback (no working memory of writing the code). Plays "better copilot."
- **Per-chunk plans** in `~/.claude/plans/dasimgui_<phase>.md`.
- **No PRs, no CI, no Copilot threads** — dasImgui has none of those. Don't try to use that machinery.
- **Re-install workflow:** edit in `D:\DASPKG\dasImgui`, then `daspkg install --global --force <path>` to update the daslang-tree's `modules/dasImgui/` copy. Or push to github + `daspkg upgrade --global dasImgui`.

---

## 12. Open seams (deferred decisions)

These are known unknowns. We pick when they bite.

- **In-process transport contract** — when the lambda is "loop back to local handler", what's the threading model? Probably "queue at frame boundary, drain like remote".
- **`imgui_internal.h` binding** — only when a real custom widget needs `ItemAdd` etc.
- **Recording approach** — external tools first; PNG-sequence fallback if they fail.
- **MCP integration shape** — direct daslang MCP tools (`mcp__daslang__imgui_*`) that wrap the transport. Land alongside Phase 2.

---

## 13. Out of scope (won't ship)

- `imgui_test_engine` integration — license blocks Gaijin (>$2M turnover).
- `imgui_capture_tool` — same license.
- Declarative layout DSL.
- Declarative theme system.
- Built-in cross-platform video recording (deferred indefinitely; PNG-sequence is the likely fallback).
- Auto-magic `imgui_telemetry_serve()` activation when `imgui_app` runs — must be explicit.
