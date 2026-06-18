# SFX Lab

An interactive procedural sound-effect workbench built on the daStrudel audio engine.
A **sound** is a stack of **layers** — each an **sfxr** (subtractive) or **modal** (struck/resonant)
voice with its own gain + onset delay, mixed. Build it from the docked panels, audition each layer
or the whole mix, and watch the rendered **waveform + spectrum** (ImPlot) update live.

## Components

- `sfxr_presets.das` — headless CLI: renders the 7 classic sfxr presets to `./sfxr_renders/*.wav`
  and auditions them in sequence. No GUI dependencies.

  ```
  daslang examples/daStrudel/sfx_lab/sfxr_presets.das
  ```

- `main.das` — the interactive GUI workbench. Docked panels (re-dock freely; layout persists in
  `imgui.ini`): **Layers** (add sfxr/modal, select, solo/mute, duplicate, dup-linked, delete, ▶ play),
  **Editor** (the selected layer's voice params + gain/delay/lock + a collapsible layer preview),
  **Reference** (load an external wav to mimic — view + overlay its waveform/spectrum on the Mix),
  and **Mix** (the summed waveform + spectrum). See `PLAN.md` for the roadmap (save/load `.sfx`,
  drum re-authoring).

## Running the GUI

The GUI depends on two external modules — [dasImgui](https://github.com/borisbat/dasImgui)
and [dasImguiImplot](https://github.com/borisbat/dasImguiImplot) — loaded with `-load_module`
(same pattern as the other dasImgui examples; not built by CI). Build both first (see their
READMEs), then:

```
daslang \
  -load_module <path>/dasImgui \
  -load_module <path>/dasImguiImplot \
  examples/daStrudel/sfx_lab/main.das
```

It runs standalone (windowed), headless (`-- --headless --headless-frames N`), and live
(`daslang-live …` for `imgui_snapshot` / playwright inspection) identically — audio is wired
in `init()` so every mode has sound.

## Engine

The sound generators live in `modules/dasAudio/strudel/` and are reusable on their own:

- `strudel_sfxr` — faithful sfxr-style subtractive voice (`render_sfxr`, presets, randomize/mutate).
- `strudel_modal` — modal impact voice for struck/resonant sounds (`render_modal_voice`).
- `strudel_sfx` — the multi-layer model: `Layer` (sfxr or modal voice + gain + delay),
  `render_sound(layers)`, and `.sfx` persistence (`save_sound`/`load_sound`/`render_sound_file`,
  a `Sound` doc = layers + an optional `mimic_target` reference path). A game loads a saved sound
  with `render_sound_file("foo.sfx")`. Example sounds live in `sounds/*.sfx` (File ▸ Open).
