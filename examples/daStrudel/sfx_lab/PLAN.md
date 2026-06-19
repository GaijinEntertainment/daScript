# SFX Lab — multi-layer tool plan

Working design doc for turning the lab from a single-sfxr-voice toy into a multi-layer
sound-authoring tool. Engine (slice A) is already built + tested.

## Goal

Design game-ready sound effects with zero audio art. A **sound** = a stack of **layers**,
each an **sfxr** (subtractive) or **modal** (struck/resonant) voice with its own gain +
onset delay, summed. Author it, hear it, see it, save it; a game loads it back.

## What exists (slice A — `strudel_sfx.das`, committed)

- `VoiceParams` variant { `sfxr : SfxrSound`; `modal : ModalVoice` } + `render_voice`.
- `Layer` { name, voice, gain, delay, enabled, locked, link_source }.
- `render_sound(layers) -> array<float>` — mix (offset by delay × gain, peak-normalized).
- `propagate_links(var layers)` — a linked layer mirrors its source's **voice** params,
  keeping its own gain/delay. `sfxr_layer` / `modal_layer` constructors.

## Position 0 — make the sound observable (build the detector first)

Authoring multi-component sound by ear alone is a trap; bake in inspection before features:

- **Per-layer solo / mute.** Solo isolates one voice (mutes the rest); mute drops one.
- **Per-layer audition + preview.** Every layer (locked or not) has a ▶ play button (hear that one
  voice) and the selected layer shows its own waveform + spectrum in a collapsible Editor section.
- **Spectrum overlay.** The mix curve + each layer's curve on one ImPlot chart, colour-keyed to
  the layer list, so you *see* which layer owns which band and where they fight.
- **Reference comparison (mimic workflow).** Load an external audio file as the *target*; overlay
  its waveform/spectrum on the mix plots. Matching a real sound = drive the layers until the mix
  spectrum sits on the reference spectrum. (Later this panel front-ends modal analysis: drop a wav
  → suggest modes.)
- **Waveform of the mix** (already have it), with per-layer onset markers.

## UI layout — DOCKED (decided)

dasImgui's docking build means we don't freeze a layout — each panel is a **dockable window**,
the user re-docks freely (imgui.ini persists it, gitignored). Panels: **Layers**, **Editor**,
**Mix** (waveform + spectrum, with reference overlay), **Reference** (loaded target file), plus a
top **main menu bar** (File: new/open/save Sound; Reference: load/clear; Play).

- **Layers** — list rows (select via `selectable_label`; ▶ play-layer, mute, lock as buttons;
  per-row gain + delay) + add sfxr / add modal / duplicate / dup-linked / delete.
- **Editor** — selected layer's voice params (sfxr group or modal group by kind) + a collapsible
  **layer preview** (that layer's own waveform + spectrum) + ▶ play-layer. gain/delay always live;
  voice read-only when locked.
- **Mix** — mix waveform + spectrum, with a **show-reference** overlay toggle.
- **Reference** — Load (file dialog → `read_wav`), ▶ play, waveform + spectrum of the target.

Default layout ≈ option 2 (seeded once via `setup_default_layout` / DockBuilder; re-seedable via
`imgui_dock_reset`):

```
+-----------------------------------------------+
| File   Reference   Play  [x]autoplay          |  <- main_menu_bar
+-----------------------------------------------+
| Layers (strip)                                |  <- top ~0.16
+----------------------+------------------------+
| Editor (sel layer    | Reference (target wav  |  <- middle, split L/R
|  + collapsible        |  waveform + spectrum)  |
|  layer preview)       |                        |
+----------------------+------------------------+
| Mix: waveform + spectrum (+ reference overlay)|  <- bottom ~0.34
+-----------------------------------------------+
```

Pattern (from dasImgui `examples/features/dock_basic.das`): `io.ConfigFlags |= DockingEnable` in
init(); `dockspace(DOCK_ROOT, (flags=PassthruCentralNode)) { …dock_window(PANEL, …){} }`;
`setup_default_layout(dock_id)` = DockBuilderRemoveNode → AddDockSpaceNode → SetNodeSize →
SplitNode(Up 0.14)→layers / SplitNode(Down 0.40)→plots / SplitNode(Left 0.5)→wave|spec → DockWindow
by title → Finish, gated on `DOCK_ROOT.has_initial_layout`.

## Layer operations (the list panel)

`+ sfxr` / `+ modal` (append a layer) · select · **mute** · **solo** · **duplicate**
(independent copy) · **duplicate linked** (mirrors source voice; for bounces) · **lock**
(collapse + voice params read-only; gain + delay stay editable) · delete · reorder (drag or
up/down) · per-row **gain** + **delay**.

Bounce recipe (no sequencer): duplicate a click layer N times, each with rising delay + falling
gain; link them so tuning the source updates all bounces.

## Per-voice editors (contextual, for the selected layer)

- **sfxr** — the existing grouped slider set (Envelope / Frequency / Tone / Filter / volume),
  wave combo, and the 7 preset buttons (now "set this layer to preset X").
- **modal** — macro sliders: pitch, spread, n_modes, ring, brightness, damp_skew, transient,
  transient_rate, echo_amount, echo_delay, duration.
- A locked layer shows its editor disabled (read-only); gain + delay always live.

## Presets / examples

- **Per-voice presets** when editing a layer (sfxr's 7; a few modal starters: knock/bell/click).
- **Whole-sound examples** loadable from a menu: bone_on_bone, bone_on_marble, ball_on_table
  (the 2-layer thud+click). These ship as `.sfx` files.

## Persistence (`.sfx` = JSON)

- Save/Load the layer stack as JSON via `sprint_json` / `sscan_json`. Schema:
  `{ version, layers: [ { name, kind:"sfxr"|"modal", gain, delay, enabled, locked, link_source,
  params:{…all voice fields…} } ] }`. Files live in `examples/daStrudel/sfx_lab/sounds/`.
- Game-facing: `load_sound(path) -> array<Layer>` then `render_sound`, plus a one-call
  `render_sound_file(path) -> array<float>`. (Lives in `strudel_sfx.das` so games reuse it.)
- Optional `mimic_target` field = the reference file path, so reopening a `.sfx` reloads its
  target. The reference audio itself is never embedded — just the path.

## Slices + verification

- **B — docked shell + layer list + contextual editor + mix play.** Rewrite `main.das` to the
  layer model (docked panels, menu bar, default layout). Per-row ▶ play-layer + mute/lock; the
  selected-layer editor (sfxr/modal) with gain/delay always live. Verify each step via
  `imgui_snapshot` (panels, list rows, selection, editor, mix plot).
- **C — inspection + comparison. DONE.** Per-layer solo (additive); selected layer's collapsible
  preview; the **Reference** panel (load wav via file dialog → `read_wav` → mono/44k; play; waveform/
  spectrum) + **reference overlay** on the Mix plots. (Per-layer-on-mix spectrum overlay deferred to
  polish.)
- **D — persistence + examples. DONE.** `.sfx` = JSON (`sprint_json`/`sscan_json` on a `Sound`
  doc = layers + `mimic_target` ref path; the voice variant encodes per-layer kind). File menu
  Open/Save via the dialog; `save_sound`/`load_sound`/`render_sound_file` in the lib for games;
  3 example sounds shipped in `sounds/` (bone/marble/ball-on-table).
- **(final) drum re-authoring** — rebuild the strudel drum voices as `.sfx` sounds: the proof
  the tool is real. Separate pass.

Engine stays headless-tested; UI proven via snapshot per the imgui discipline (never "looks right").

## Open decisions

1. Per-layer preview depth: selected-layer preview in the Editor (collapsible) + per-row ▶ play
   (default) — or also make every row independently expandable to its own mini plots (heavier)?
2. Reference path in the `.sfx` (`mimic_target`) — save it (reopen reloads target), or session-only?
3. Reorder UI: drag-to-reorder vs up/down arrows (default: arrows first, drag later).
4. Solo behaviour: exclusive (one at a time) vs additive solo-set (default: additive set).
