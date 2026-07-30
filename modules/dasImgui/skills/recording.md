# Recording tutorial videos

dasImgui ships its tutorial site at `doc/source/` with an MP4 per tutorial page (`doc/source/_static/tutorials/*.mp4`). Recordings are produced by **one-shell driver scripts** under `tests/integration/record_*.das` that spawn their own `daslang-live` host and drive it over HTTP. A recording carries a **voiceover + music soundtrack** and is **also an integration test** — every interaction it narrates is performed for real and verified.

This page is the recipe. Read it before writing or revising any `record_*.das` driver. It builds on
`skills/playwright.md` — read that first for the foundation: a recording is a playwright driver, so the
**async rule applies** (every interaction must be gated on its observable effect, never a frame/sleep
guess; `hold_through_voice`/`verify_click_effect` are the recording-side enforcement of exactly that).

Recording is NOT in CI — drivers are manually-driven artifact producers. The pipeline is three tools (below); the deliverable is one `.mp4` per scene, committed alongside the driver.

## The three hard requirements (REQUIRED)

These are not style preferences. A recording that violates any of them is wrong and must not ship.

1. **Do what it teaches — drive it like a human.** Every stage must *perform the real interaction it narrates, using real synthetic input only* — click the button so its counter ticks, **drag** the slider handle to scrub, **type** into the input field, walk the radio dot across the group. The viewer must SEE the gesture and its effect. **Never `force_set` a value to shortcut the gesture** — `force_set` writes the widget state directly, bypassing the very interaction the tutorial teaches (it is the synthetic-≠-real workaround). The *only* exception is a tutorial whose **subject is programmatic / external driving** (e.g. `driving_outside`, which teaches the `imgui_force_set` verb itself) — there `force_set` is the thing being demonstrated. Pointing the caption at a widget without driving it is not a tutorial.

2. **Self-verify every step.** A recording is an integration test. Every interaction is verified against the widget's actual state change:
   - clicks go through `hold_through_voice`, which snapshots the pre-state, clicks, and asserts the kind-appropriate effect (`verify_click_effect`);
   - drags / key-types (slider, drag, input, color…) drive the real gesture (`drag` / `drag_to` / `type_text`) and assert the resulting value (`expect_value`); a stage that didn't move the value is a failure.
   A no-op interaction (click landed off-target, state didn't move) is recorded as a failure and the recording **aborts loudly** — so a dud can never silently ship. (Mechanism: failures accumulate into `g_record_failures` and surface as a panic at `with_recording_app` teardown, *after* a clean `record_stop` + `/shutdown`, because daslang panic is fatal and a mid-body panic would orphan the host.)

3. **Pace by the voice, not by fixed sleeps.** Dwell is driven by the voiceover wav length. `say_begin` returns the dwell (the spoken line's duration + gap); `hold_through_voice` splits it — a short lead lets the caption + voice start, the verified click(s) land under the voice (ripple + state change visible while Emma talks), then it holds the caption for the remainder. Do not hand-tune `sleep(READ_MS)` for voiced recordings.

Plus one constraint that bites silently: **captions and voice strings must be ASCII.** The bundled ImGui font has no em-dash / arrow / smart-quote glyph — non-ASCII renders as `?` in the caption box. Write `-` not `—`, `->` not `→`.

`tests/integration/record_buttons.das` and `record_toggles.das` are the canonical exemplars of all three rules — read them before writing a new driver.

> **Retrofit mandate:** these rules apply to **every** recording, including the legacy set. Drivers still using the pre-voice `imgui_narrate` + fixed-`sleep` + `click_at` pattern (most of `record_*.das`) must be re-recorded under the current model. That is an active backlog, not an exemption — do not author a new driver in the legacy style, and prefer to upgrade a legacy driver whenever you touch its scene.

## Tutorial quality bar — the ship / no-ship checklist

A tutorial's `.mp4` ships only when ALL of these hold. The first three are the hard requirements above; the rest are the full-tutorial gate (the recording is also the integration test, so the example file and the RST page get fixed in the **same** pass, not deferred):

- [ ] **Does what it teaches, driven like a human** — every narrated action performed for real via synth input (click / drag / type), never just pointed at and **never `force_set`** (the sole exception is a force_set-subject tutorial like `driving_outside`). *(req 1)*
- [ ] **Self-verifies every interaction** — clicks via `hold_through_voice`; drags/types assert the resulting value (`expect_value`); a no-op aborts the recording at teardown (`g_record_failures` panic). No silent dud. *(req 2)*
- [ ] **Paced by the voice** — dwell = the voiceover wav length (`say_begin` returns it, `hold_through_voice` splits it); no hand-tuned `sleep`. *(req 3)*
- [ ] **Caption/voice decoupled, ear-first** — terse on-screen caption (`text`) vs a natural spoken sentence (`voice`, which keys the TTS line and drives dwell). **ASCII only** — the font has no em-dash / arrow / smart-quote glyph.
- [ ] **Example is correct** — the feature `.das` compiles, runs, and demonstrates the widget without bugs (fix what's broken in this pass).
- [ ] **Page is accurate** — the RST describes the current behavior and `.. video::`-cites an existing, fresh `.mp4`.
- [ ] **Recording is clean** — `dropped = 0`, sidecar frame count == APNG `nb_read_frames`, voice + music bed muxed, ffprobe `video.dur ≈ audio.dur`, mp4 ≈ 50–300 KB, **eyeballed and approved**.

A dasImgui/node-editor library bug surfaced while fixing a tutorial ships as its **own immediate PR**, not buried in the recording commit.

## Author by live-probing, not guessing

Driving a widget "like a human" only works if the synthetic gesture actually lands — a guessed click point or drag path that misses is a dud (and now a teardown-panic failure). **Don't guess the sequence; determine it live against a running host, then bake the proven sequence into the driver.**

Build `daslang-live` once, launch the tutorial's feature file, and probe it through the MCP live API:

```bash
<daslang>/bin/Release/daslang-live -project_root <dasimgui> examples/tutorial/<scene>.das
```

- **FIRST, `mcp__daslang__live_command name="set_user_control" args='{"enabled":false}'`** — a manually-launched probe host has user-control ON, so the real OS cursor races GLFW's poll and clobbers `io.MousePos`; your synth gesture then lands at the wrong place (or not at all) and the snapshot's `mouse_pos` reads the OS cursor, not your synth pos. `with_recording_app` posts this for you; a manual probe host does NOT — set it by hand before any `imgui_mouse_play` / `imgui_click`.
- `mcp__daslang__live_command name="imgui_snapshot"` — read the widget bboxes / payload (real coords + current values).
- `imgui_mouse_play` (events array) / `imgui_click` / `imgui_focus` / `imgui_key_type` — try the gesture.
- `mcp__daslang__live_command name="screenshot"` → `Read` the PNG — confirm the gesture produced the effect (handle moved, counter ticked, menu opened).
- Iterate until the gesture reliably drives the widget; **full-restart the host between tries** (interactive state contaminates the next probe). Then transcribe the proven coords/sequence into the `record_*.das` driver.

Essential for **drags** (the value follows track geometry, so a guessed end-point lands on the wrong value) and **menus/popups** (open-then-click must be one bundled `imgui_mouse_play` stream before the auto-close timer fires). The probes obey the async rule in `skills/playwright.md` — gate each step on the snapshot, not a sleep.

## The pipeline: prepare → record → convert

Three tools under `utils/`, run in order. `<daslang>` = a daslang build tree, `<dasimgui>` = this repo (append `.exe` on Windows).

**1. `prepare_recording` — pre-render the voiceover.** Compiles the driver (same module resolution as running it — if it won't compile, there is no recording), AST-scans it for every `say()` / `say_begin()` literal (caption + voice) plus the APNG basename, synthesizes each unique spoken line to a wav via an OpenAI-compatible TTS server (Kokoro, voice `bf_emma` by default), measures durations, and writes `voiceover/<stem>.manifest.json` beside where the APNG will land.

```bash
<daslang>/bin/Release/daslang -project_root <dasimgui> \
    <dasimgui>/utils/prepare_recording.das -- \
    --driver <dasimgui>/tests/integration/record_buttons.das
```

Needs a running TTS server at `--base-url` (default `http://127.0.0.1:8880/v1`). Re-running skips lines whose wav already exists (hash-named), so it's cheap to re-prepare after editing one line. Captions/voice must be ASCII (see above) — they're scanned verbatim.

**2. `record_X.das` — capture.** Run the driver. `with_recording_app` auto-detects the manifest (`arm_voiceovers`), so `say_begin` returns each line's real wav duration as the dwell. On `record_stop` it writes `voiceover/<stem>.sidecar.json` (clip length + the frame each caption appeared on). When voiceovers are armed, the recorder's `max_seconds` cap is **auto-extended** by the summed wav dwell (+ a margin), so a voiced body can't be cut off mid-stage by the silent-mode budget — leave `max_seconds` sized for the silent run.

```bash
<daslang>/bin/Release/daslang -project_root <dasimgui> \
    <dasimgui>/tests/integration/record_buttons.das
```

**3. `convert_recording` — soundtrack + mux to MP4.** Reads the sidecar, renders the daStrudel music bed to exactly the clip length, then runs one ffmpeg pass that muxes: APNG → H.264 video, a faded low-volume music bed (default `-13 dB`), and each voiceover wav delayed to the frame its caption appeared on. The video is forced onto a constant frames/duration grid (`-r fps_eff`, where `fps_eff = frames/duration`) and each voiceover is anchored to its frame on that *same* grid, so caption and voice share one clock. (Why `fps_eff` and not the nominal fps: the APNG writer is **synchronous** — every frame is captured, none dropped — but it blocks the GL thread during zlib, so a windowed capture's *effective* fps runs below nominal while frames stay evenly spaced. Reclocking onto the real rate keeps the per-frame voiceover anchors true; without it the captions race ahead of their voice.) The exact ffmpeg command is saved to `<out>.mp4.ffmpeg.txt`.

```bash
<daslang>/bin/Release/daslang -project_root <dasimgui> \
    <dasimgui>/utils/convert_recording.das -- \
    --apng <dasimgui>/doc/source/_static/tutorials/buttons.apng
```

Mostly-static recordings (a plot with a brief gesture) land ~0.5–0.8 MB at the defaults. A
**full-motion** clip (e.g. a realtime scroll, where every frame is a full-frame change) defeats
H.264 inter-frame compression and runs several MB. Shrink it the lossless way — add
`--preset veryslow` (better compression at the **same** quality), **not** by raising `--crf`
(which trades away the quality the clip needs). `--crf` stays at the clean default `23`; both are
knobs only for this case.

Only the resulting `.mp4` is tracked. The `.apng`, `voiceover/*.wav`, `*.manifest.json`, `*.sidecar.json`, `*_music.wav`, and `*.mp4.ffmpeg.txt` are all intermediates and gitignored.

## Mental model

A recording has three independent layers:

1. **Host**: `daslang-live.exe` running a feature file (`examples/tutorial/X.das` or `examples/features/X.das`). It opens a window, runs `update()` at 60 fps, listens for live-commands on port 9090. `with_recording_app` **spawns it for you** with `--imgui-content-scale=1.0` + `--no-hdpi-framebuffer` so the framebuffer + ImGui style stay at logical resolution (encoder-friendly APNGs on retina). It also posts `set_user_control(false)` so the real OS cursor can't clobber synth input.
2. **Driver**: `daslang.exe tests/integration/record_X.das` runs once. `with_recording_app(feature, output, max_seconds [, fps]) $(app) { ... }` spawns the host, waits ready, arms voiceovers from the manifest, enables `imgui_cursor_sprite` + `imgui_mouse_trail`, posts `record_start`, invokes the body, posts `record_stop`, writes the sidecar, disables the aids, and `/shutdown`s the host. If the body left any verification failures, it panics with them after teardown.
3. **Visual overlays**: `imgui_cursor_sprite` (red+white circle at synth pos) + `imgui_mouse_trail` (fading dotted line) paint to the foreground draw list during `end_of_frame()`. The helper toggles them around the body; tour-specific overlays (`imgui_key_hud`, `imgui_focus_rect`) are body-managed (see `record_visual_aids.das`).

The recorder captures the host's framebuffer at `fps` Hz from `record_start` to `record_stop`, saving an APNG.

## Host pre-requisites

If the host is a **tutorial** (`examples/tutorial/*.das`, `live_*` lifecycle): nothing extra — these already call `apply_synth_io_override()` between `ImGui_ImplGlfw_NewFrame()` and `NewFrame()`. That override drains BOTH the synth-IO timelines AND `advance_coroutines` — the latter is what advances `imgui_click`'s animated `click_at_coro`; without it, a hand-rolled tutorial loop's synth clicks silently no-op.

If the host is a **harness** (`examples/features/*.das`, `harness_*` lifecycle): nothing extra — `harness_new_frame()` runs `imgui_synth_tick()` built-in (between the backend `*_NewFrame()` and ImGui's `NewFrame()`), so synth IO drains on every harness app with no opt-in. (Earlier versions required a separate `harness_apply_synth_io()` call; it was folded into `harness_new_frame()` and no longer exists.) Without that tick, `synth_cursor_owned` would stay false (GLFW's poll wins the race), the cursor sprite + trail would paint at the off-window OS cursor, and synth clicks would never reach ImGui's hover/active state — but `harness_new_frame()` already prevents all of that.

## Driver model (current)

Mirror `record_buttons.das` / `record_toggles.das`. The spine of a voiced, self-verifying, do-what-it-teaches stage:

```daslang
options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

[export]
def main {
    with_recording_app("examples/tutorial/scene.das", "scene.apng", 45) $(app) {
        let T_BTN = "WIN/BTN"

        var snap = wait_for_widget(app, T_BTN, 10.0f)
        if (snap == null) { panic("{T_BTN} never registered - wrong app running?") }

        // Move the cursor ONTO the first click target before say_begin, so the click
        // can fire straight under the voice with no mid-stage hop.
        move_to(app, widget_click_point(snap, T_BTN), 500)
        sleep(SETTLE_MS)

        // ---- Stage: button — the workhorse ----
        // say_begin posts the caption + records the voiceover anchor and RETURNS the dwell.
        // hold_through_voice splits the dwell: lead -> verified click(s) under the voice -> hold.
        hold_through_voice(app, say_begin(app, "button - the workhorse", T_BTN,
            [voice = "Here is the everyday button. Each time you click it, its counter goes up by one. Watch the number climb."]),
            [T_BTN, T_BTN])   // click it twice -> counter visibly ticks 0,1,2; each click verified
    }
}
```

Key points:

- **Caption vs voice.** The `text` arg is the **on-screen caption** — terse, for the eye. The `voice` arg is the **spoken line** — natural sentences, for the ear. Pass both as *string literals at the call site* so `prepare_recording` can scan them. Empty `voice` falls back to `text` (but a bare caption read aloud sounds robotic — write a real voice line).
- **`say_begin` returns the dwell; `hold_through_voice` consumes it.** Never `sleep()` a fixed read window for a voiced stage. For plain narration with no action, use `say()` (posts + sleeps in one call).
- **The clicks list is the action AND the verification.** `hold_through_voice(app, dwell, [targets...])` clicks each target in order and verifies the kind-appropriate effect of each. Clicking `T_BTN` twice ticks the counter to 2 and asserts both ticks. The caller must have moved the cursor onto the *first* target already.
- **Per-site radio aliases.** `radio_button_int` shares one ident across sites; address each option by `"IDENT#<v_button>"` (e.g. `"WIN/MODE#1"`, `"WIN/MODE#2"`). `verify_click_effect` reads the `#v` to assert `value == v` after the click; passing the list walks the dot.
- **Multi-target stages (sibling controls).** When a stage spans more than one "main" widget (an `arrow_button` pair, an R/G/B triple), use the `targets` overload: `say_begin(app, caption, [T_FIRST, T_SECOND], [voice=...])`. The caption points at `targets[0]` and is placed to **hard-avoid the union** of all targets' bboxes, so the box never covers a sibling. Pass the **same list** to `hold_through_voice` that you passed to `say_begin`.

### What `verify_click_effect` checks, by kind

| widget kind | verified effect after click |
|---|---|
| button / small_button / arrow_button / invisible_button / image_button / tab_item_button | `click_count == pre + 1` |
| checkbox / radio_button / selectable / menu_item | `value` flips (`!pre`) |
| radio_button_int (target `"X#v"`) | `value == v` |
| anything else (slider, drag, input, color…) | not click-tracked — recordings drive these by **real drag / type** and assert the value moved (NOT `force_set`) |

### Verified value changes (non-click stages) — drive the real gesture

Widgets you don't click — slider / drag / input / color — are driven the way a human would: **drag the handle** (`drag` / `drag_to` / `drag_along`) or **focus + type** (`type_text`), then assert the resulting value with `expect_value` (or `wait_for_*_value`). A drag asserts the value *moved* (direction / approximate target, since the value follows track geometry); a type asserts the field equals what was typed. A stage that left the value unchanged is a failure (accumulate-on-miss → teardown panic, same as clicks).

**Do NOT `force_set` to shortcut these** — `force_set` writes the state directly and skips the gesture the tutorial exists to show (synthetic ≠ real). `force_set_verified` is reserved for the one case where the *subject* of the tutorial is the `imgui_force_set` verb itself (`driving_outside`). The `edit_*` external-pointer rail (`edit_external_tour`) is NOT that case — its subject is binding a widget to caller-owned data, and the proof of the rail is that a *real* drag / type / pick writes through the pointer, so it is driven for real (`drag_through_voice` / `type_into_voice` / `combo_pick_voice`) like any other widget. If a stage wants a verified drag/type wrapper with `hold_through_voice`'s ergonomics, add it to `imgui_playwright.das` (a framework rail → its own PR).

## Resolving widget coords

Use `widget_click_point(snap, ident)` (aims toggle-family clicks at the glyph, not the row center) or `widget_center(snap, ident)`. Most widgets — including `menu()` children and `tab_item()` headers — resolve directly: `imgui_containers_builtin.das` snapshots `GetItemRectMin/Max` right after `BeginMenu`/`BeginTabItem`, so the registered bbox is the clickable header.

Two containers are bbox-degenerate by design:
- **`main_menu_bar()`** chrome has no header — `bbox = (0,0,0,0)`. Its `menu()` children DO register.
- **`window()` / `tab_bar()`** are bodies, not headers — `bbox = (0,0,0,0)`. Click the contents, not the shell.

If you need a `main_menu_bar` pixel position (e.g. to dismiss by clicking empty bar), capture it empirically: launch the host, `mcp__daslang__live_command name="screenshot"`, read the PNG, measure, bake as named constants. Submenu items register a bbox **once the parent is open** — open the parent first, then `wait_for_render` + `widget_center` the child.

### Menu interactions: bundle open + child click into ONE mouse_play

A split-stream menu interaction (two `imgui_mouse_play` calls — open, then click child) does NOT work: between calls no fresh mouse events fire and ImGui's auto-close timer shuts the menu before the second call. Bundle header-click + lerp + child-click into a single event stream:

```daslang
let p_darkmode = (p_file._0, p_file._1 + 32.0f)   // child opens DOWN from header; est. +32px
var events : array<JsonValue?>
events |> click_at(0, p_file, 200)         // open File menu
events |> click_at(1200, p_darkmode, 600)  // lerp + click child
post_command(app, "imgui_mouse_play", JV((events = events)))
```

The child's bbox isn't known until the menu opens, so estimate its position from the parent header (item height ≈ 22px; first item ≈ header_bottom + 14px). Frame the narration as one action ("Click File, then Dark mode") since the click lands after the bundled stream. Same pattern for any hover-auto-close element: `combo_select`, `popup_context_window`, tooltip cascades. See `record_main_menu_bar.das`.

## Verifying a recording without playing it

Beyond the built-in self-verification, two probes for diagnosing the visual path:

**Probe 1 — live screenshot (state right now).** Launch a host manually (so it stays alive), drive it via MCP, screenshot:

```
mcp__daslang__live_command name="imgui_mouse_play" args='{"events":[{"t_ms":0,"kind":"move","x":50,"y":15},{"t_ms":1000,"kind":"move","x":300,"y":300}]}'
mcp__daslang__live_command name="screenshot" args='{"file":"<dasimgui>/diag.png"}'
```

`Read` the PNG. If the screenshot shows what you expect but the APNG doesn't, the bug is in the recording path; if neither shows it, the synth IO isn't draining (a harness host drains it in `harness_new_frame()`; a `live_*` host must call `apply_synth_io_override()`).

**Probe 2 — extract APNG frames.**

```bash
ffmpeg -i scene.apng -vf "select=eq(n\,80)" -frames:v 1 -update 1 frame80.png -y
```

`-update 1` is required for single-frame output; `select=eq(n\,N)` picks 0-based frame N (escape the comma). Pick 5-6 N values spanning the clip and `Read` each for a fast story-arc check.

## Where recordings land

`doc/source/_static/tutorials/`. `with_recording_app` writes the absolute path via `dir_name(get_this_module_dir()) / RECORD_ASSET_REL`, so caller cwd is irrelevant. When dasImgui is daspkg-installed under `daScript/modules/dasImgui/`, the APNG lands in that install copy — for source-repo workflows (preferred), launch with `-project_root <dasimgui-source>` so the loaded copy IS the source repo (the helper forwards `-project_root` to the spawned host).

Each tutorial RST cites its video via the local `video` directive (`doc/source/tutorial_video.py`, listed in `conf.py` extensions):

```rst
.. video:: scene_name.mp4
```

It emits `<video controls preload="metadata" playsinline>` — starts paused, native play/pause + scrubber + volume + fullscreen, audio on, no autoplay, no loop. Change player chrome once in `tutorial_video.py`, not per page.

CI's sphinx step uses `-W`, so an RST citing a missing `.mp4` fails the build — commit the `.mp4` ahead of (or with) the RST cite. A voiced `.mp4` (video + voice + music) is typically ~0.5–1.5 MB for a 30–75 s clip (buttons ≈ 1.4 MB, toggles ≈ 0.7 MB, selectable_hover ≈ 0.6 MB); a silent quick-convert is much smaller. Far outside that is suspicious — too small = failed encode. Size is NOT a frame-drop signal: the synchronous writer never drops, so trust `dropped == 0` from `record_stop`, not the byte count.

## Manual ffmpeg conversion (no soundtrack)

`convert_recording` is the normal path. For a quick silent conversion (no voiceover/music), one ffmpeg pass:

```bash
ffmpeg -y -loglevel error -i scene.apng -c:v libx264 -crf 23 \
       -pix_fmt yuv420p -movflags +faststart scene.mp4
```

- `libx264` — universal browser support, hardware-decoded.
- `-crf 23` — quality knob (18 = visually lossless, 28 = noticeably compressed); 23 is ample for UI.
- `-pix_fmt yuv420p` — required cross-browser (Safari refuses yuv444p).
- `-movflags +faststart` — MOOV atom up front so `<video>` plays before the full download.

## Legacy drivers (pre-voice) — being retrofitted

Older drivers (e.g. the original `record_with_id.das`) use the **pre-voice pattern**: `imgui_narrate` + fixed `sleep(READ_MS)` + `click_at` events, with no voiceover, no music, and **no per-step verification**. That pattern predates the soundtrack + self-verifying model and is **not canonical** — it is the retrofit backlog (see the mandate above). Do not copy it for new work. When you touch a legacy scene, upgrade it: scan for `say`/`say_begin`, drive interactions through `hold_through_voice` / `force_set_verified`, and re-run the prepare → record → convert pipeline.

(The fixed-pacing constants `NARRATE_FRAMES=240` / `READ_MS=5000` / `SETTLE_MS=1500` / `RESULT_MS=2000` belong to that legacy model; only `SETTLE_MS` — the cursor-settle beat after a `move_to` — still appears in current drivers.)

## Commit structure for a recording

For scene `foo`:

1. Author/upgrade `tests/integration/record_foo.das` (voiced, self-verifying, do-what-it-teaches).
2. Run prepare → record → convert; **eyeball-review** the resulting `.mp4`.
3. Commit: `recording: foo` — `record_foo.das` + `doc/source/_static/tutorials/foo.mp4`. (The `.apng`/`voiceover/*`/`*.ffmpeg.txt` intermediates are gitignored — don't commit them.)
4. Commit: `tutorial: foo RST page` — `doc/source/tutorials/foo.rst` (`.. video:: foo.mp4`) + the `index.rst` toctree entry.

If recording multiple scenes in one PR, repeat per scene with a PAUSE for eyeball review between scenes. Project workflow: eyeball-only review, no Copilot, branches as backup. Recording is interactive and one-at-a-time — concurrent driver runs collide on port 9090.
