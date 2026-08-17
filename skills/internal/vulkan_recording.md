# Recording dasVulkan tutorial videos

The dasVulkan tutorial pages live in the main Sphinx tree at `doc/source/reference/tutorials/vulkan/`, with an MP4 per page. Recordings are produced by a **per-tutorial driver** — `modules/dasVulkan/tutorials/<NN>_<scene>/recording/record_<scene>.das` — that re-renders the scene parametrically (one frame per integer index, headless Vulkan readback), streams it to an APNG, then ffmpeg-muxes a daStrudel music bed, voiceover and captions onto it.

Read this before writing or revising any `record_*.das` driver. Recording is NOT in CI — the drivers are manually-driven artifact producers, eyeballed and listened to before they ship. The deliverable is one `.mp4` per scene, uploaded to the rolling `docs-assets` GitHub release; MP4s are NOT committed, and docs builds stage them via `utils/docs_assets/fetch.{sh,ps1}` before sphinx runs.

**`docs-assets` is one flat namespace shared with the dasImgui recordings, so every dasVulkan MP4 uploads under a `vulkan_` prefix** — the driver writes `<scene>.mp4` beside its tutorial, and it ships as `vulkan_<scene>.mp4`:

```bash
gh release upload docs-assets vulkan_<scene>.mp4 --clobber
```

The RST pages cite the prefixed name; sphinx runs with `-W`, so an unprefixed upload fails the docs build on a missing video.

## The hard requirements (REQUIRED)

1. **The tutorial source is never modified for recording.** The driver is the only file the pipeline touches. If it needs a `render_<scene>_frame(...)` helper, that helper is a small isolated addition to the scene module, useful on its own.

2. **Every frame is an independent function of its index.** `capture_apng` calls the render block once per integer `frame`; no state crosses frames. A driver that accumulates between frames produces a clip that cannot be re-rendered or resumed.

3. **Render headlessly, read back.** Drivers render each frame to a host-visible buffer and read it back — no window, no swapchain capture. The windowed `window/show_<scene>.das` viewers exist for humans, not for recording.

4. **Panic on failure.** `capture_apng`, `prepare_voiceover` and the convert calls all return `bool`; a driver that ignores a `false` ships a silently botched recording.

Plus one constraint that bites silently: **captions and voice strings must be ASCII.** The fallback fonts ffmpeg `drawtext` resolves to carry no em-dash / arrow / smart-quote glyph.

`record_triangle.das` is the canonical silent exemplar; `record_mandelbrot.das` is the canonical voiced one.

## Where things live

```
modules/dasVulkan/tutorials/
  recording/
    tutorial_record.das         silent path (capture_apng + convert_to_mp4 + find_daslang_exe)
    tutorial_record_voiced.das  voiced extension (say + prepare_voiceover + convert_to_mp4_voiced)
  <NN>_<scene>/
    <scene>_tut.das             the tutorial itself - never edited for recording
    test_<scene>.das            the pixel oracle (the CI gate; unrelated to recording)
    recording/
      record_<scene>.das        the driver - this is what you run
      <scene>.apng              intermediate
      <scene>_bed.wav           music bed
      voiceover/                per-line wavs + durations
```

Everything a driver writes is gitignored, the `.mp4` included. The `recording/` folder name is load-bearing: `modules/dasVulkan/tutorials/.das_test` skips the whole folder unless stbimage + audio + dashv are all loaded, which is why CI never runs it.

## The two utilities

`tutorials/recording/tutorial_record.das` — silent, backend-agnostic (stbimage + ffmpeg only, no Vulkan dependency):

```daslang
def public capture_apng(apng_path : string; w, h, n_frames, frame_ms : int;
                        render : block<(frame : int) : array<uint8>>) : bool
def public convert_to_mp4(apng_path, music_feature, daslang_exe, out_mp4 : string;
                          dur_s : float; fps, bed_db, fade_ms : int) : bool
def public find_daslang_exe(das_root : string) : string
```

`capture_apng` streams the block's `w*h*4` RGBA8 buffers to APNG, flipping rows so the writer's GL-style flip cancels out (Vulkan readback is already top-down). `convert_to_mp4` renders the daStrudel `music_feature` to `dur_s` seconds in a subprocess, then muxes APNG + faded, attenuated audio at `-crf 23`. `find_daslang_exe` probes the usual Windows / Linux / macOS layouts and falls back to bare `daslang` on PATH.

`tutorial_record_voiced.das` is a **separate file** so the silent path stays compilable when dasHV (the dasOPENAI HTTP backend) is absent; it re-exports the silent path, so a voiced driver requires only the voiced file. It adds:

| Function | Role |
|---|---|
| `say(frame, caption)` / `say(frame, caption, voice)` | Register a caption + voice anchor at frame index `frame`; voice defaults to the caption |
| `prepare_voiceover(apng_path [, base_url, voice_id, model])` | Synthesize each registered line via Kokoro TTS (`http://127.0.0.1:8880/v1`, voice `bf_emma`) into `<apng_dir>/voiceover/line_<i>.wav` and measure it. Existing wavs are reused, so re-runs after editing one line are cheap |
| `convert_to_mp4_voiced(..., dur_s, fps, bed_db, fade_ms [, font_file])` | `convert_to_mp4` plus: each voiceover wav `adelay`ed to its frame's seconds under the music bed, and each caption rendered by ffmpeg `drawtext`. `font_file` defaults to the Windows `arial.ttf` in ffmpeg-escaped form — pass another path on Linux |

## Driver shape

```daslang
options gen2
require ../triangle_tut.das
require ../../recording/tutorial_record.das
require daslib/fio
require daslib/module_path
require math

[export]
def main {
    let here = get_this_module_dir()
    let apng = path_join(here, "triangle.apng")
    let out_mp4 = path_join(dir_name(here), "triangle.mp4")
    let das_root = get_das_root()
    let daslang_exe = find_daslang_exe(das_root)
    let music = path_join(das_root, "examples/daStrudel/features/sf2_integration_full_ambient.das")

    let n_frames = 120
    let fps = 30
    let dur_s = float(n_frames) / float(fps)

    let ok = capture_apng(apng, TRI_W, TRI_H, n_frames, 1000 / fps) $(frame : int) {
        return <- render_spin_triangle(float(frame) / float(n_frames) * 2.0f * PI)
    }
    if (!ok) { panic("capture failed") }
    if (!convert_to_mp4(apng, music, daslang_exe, out_mp4, dur_s, fps, -13, 100)) {
        panic("convert failed")
    }
}
```

Paths come off `get_this_module_dir()` so the caller's cwd is irrelevant. A voiced driver adds `say(...)` calls before the capture and swaps the last two steps:

```daslang
say( 30, "dasVulkan tutorial 2 -- Mandelbrot zoom", "das Vulkan tutorial two. The Mandelbrot zoom.")
say(360, "Time drives the animation", "One time push constant drives the whole animation. Zoom. Rotation. Color.")

if (!prepare_voiceover(apng)) { panic("voiceover failed -- is Kokoro running at :8880?") }
convert_to_mp4_voiced(apng, music, daslang_exe, out_mp4, dur_s, fps, -13, 100)
```

## Run it

From the repo root, one command per tutorial:

```bash
bin/Release/daslang -project_root . \
    modules/dasVulkan/tutorials/01_triangle/recording/record_triangle.das
```

Recording is one-at-a-time — a voiced run holds the TTS server, and concurrent runs collide on the shared `voiceover/` cache.

## Caption vs voice — pronunciation conventions

`say(...)` takes a terse on-screen `caption` and a natural spoken `voice` because TTS does not read brand-name camelCase or hyphenated acronyms the way a human does. Captions stay canonical; voice text is respelled for Kokoro / `bf_emma`:

| Canonical (caption) | Voice text |
|---|---|
| `dasVulkan` | `das Vulkan` |
| `dasSpirv` | `das Spear V` |
| `SPIR-V` | `Spear V` (Khronos's intended pronunciation) |
| `2` / `3` / … | `two` / `three` / … (digits are fine in captions, not aloud) |

Strip `-`, `--` and other punctuation from voice text — the eye reads it, the TTS spells it out. **Use periods, not commas, to force pauses:** *"Zoom. Rotation. Color."* lands as three deliberate beats where the comma form runs together.

## Quality tuning

The voiced default is `-crf 28` with `-tune animation -preset slower`; the silent path uses libx264's default `-crf 23`. The tune biases x264 toward larger blocks and the slower preset gives motion search headroom — both matter for the smooth gradients and pans that shader-pure tutorial content produces, and together they let the higher CRF pass unnoticed while roughly halving the file. CRF is logarithmic (+6 ≈ half the bitrate).

**Adding a voiced recording:** start at `-crf 28`, eyeball at 100%, bump (`30`, `32`, …) until artifacts appear, then back off one step. Content with real photographic detail (textures from disk, camera blits) does not tolerate this — default back to `-crf 23` there.

## Prerequisites

- **ffmpeg** on PATH, with `libx264` and (for captions) the `drawtext` filter.
- **A full local daslang build** — the drivers need stbimage + audio, and voiced ones need dashv via dasOPENAI. The headless CI build has none of them.
- **Kokoro TTS** at `http://127.0.0.1:8880/v1` (voiced drivers only), default voice `bf_emma`.
- **No display and no Vulkan SDK** — headless readback through the vendored headers + volk is enough.

## Commit structure for a recording

For scene `foo`:

1. Write `modules/dasVulkan/tutorials/<NN>_foo/recording/record_foo.das` modeled on the exemplars, plus whatever small `render_foo_frame(...)` helper it needs in the scene module.
2. Run it; **eyeball and listen to** the resulting `.mp4`.
3. Upload it under the prefixed name: `gh release upload docs-assets vulkan_foo.mp4 --clobber`.
4. Commit `recording: foo` — the driver + the scene-module helper. The RST page cites it with `.. video:: vulkan_foo.mp4`; sphinx runs with `-W`, so upload before the cite lands.

## What this is NOT

- **Not a daslang-live host.** No live commands, no HTTP, no env-var arming — unlike the dasImgui recordings, which drive a live host over port 9090.
- **Not self-verifying.** Correctness is the tutorial's own pixel oracle in `test_<scene>.das`; the driver only produces the doc figure.
- **Not CI.** The only automated check anywhere near it is the docs build failing on a cited MP4 that is missing from the `docs-assets` release.
