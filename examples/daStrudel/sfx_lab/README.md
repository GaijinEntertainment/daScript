# SFX Lab

An interactive procedural sound-effect workbench built on the daStrudel audio engine.
Tweak a full **sfxr**-style synth with sliders, fire the classic presets, randomize/mutate,
and watch the rendered **waveform + spectrum** (ImPlot) update live. Play with the button
(or autoplay on a preset).

## Components

- `sfxr_presets.das` — headless CLI: renders the 7 classic sfxr presets to `./sfxr_renders/*.wav`
  and auditions them in sequence. No GUI dependencies.

  ```
  daslang examples/daStrudel/sfx_lab/sfxr_presets.das
  ```

- `main.das` — the interactive GUI workbench (sliders + presets + live waveform/spectrum plots).

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

A future revision turns the lab into a **multi-layer** tool: a sound = a stack of sfxr/modal
voices with per-voice gain + onset delay, summed.
