# dasImgui project instructions

dasImgui is the daslang binding + boost-v2 wrapper layer for [Dear ImGui](https://github.com/ocornut/imgui). It ships:

- a **C++ native binding** (`bind/`, `src/`) compiled into `dasModuleImgui.shared_module` (the raw imgui surface);
- a **boost-v2 wrapper layer** (`widgets/`) — `[widget]` / `[container]` / `with_*` macros, the imgui-side telemetry path, and a default-on lint that rejects unwrapped raw `imgui::` calls;
- two **app harnesses** — `imguiApp` (windowed via GLFW+OpenGL) and `imguiAppHeadless` (no display, real ImGui ctx with CPU font atlas) — also shipped as shared modules;
- **examples** organised by purpose (see below) and **integration tests** under `tests/integration/` driven by `dastest`;
- a **sphinx tutorial site** under `doc/source/` with annotated walkthroughs + recorded APNGs.

Throughout this doc:

- `<daslang>` = path to a built daslang tree (e.g. the GaijinEntertainment/daScript checkout with a `Release` build).
- `<dasimgui>` = path to this repo (your dasImgui checkout).

Develop in **your local dasImgui checkout**, NOT inside `<daslang>/modules/dasImgui/`. The daslang-side install path (under daslang's tree) is a *separate clone* that daspkg manages; do not edit it.

## Module resolution

- `modules/dasImgui/` inside this repo is a **self-junction/symlink** back to `.`. It exists so paths like `modules/dasImgui/examples/imgui_demo/main.das` resolve when daslang-live or dastest run from this repo root. Recreate with `mklink /J modules\dasImgui .` on Windows or `ln -s .. modules/dasImgui` on Linux/macOS if missing.
- **`require imgui/<name>`** resolves via `.das_module`'s `register_native_path` calls (e.g. `register_native_path("imgui", "imgui_harness", "{project_path}/widgets/imgui_harness.das")`). For daslang or daslang-live to load this, **pass `-project_root .`** when running from this repo root.
- `require` only resolves **siblings** of the calling file's directory and the registered native-paths above. **No `..`/absolute-from-root forms.** Files that need to require both `imgui/*` and a sibling module like `about` must live in the sibling's directory.
- If a sibling name collides with an `imgui::` builtin (`ShowAboutWindow`, `ShowStyleEditor`, …), **qualify with the module name** at the call site: `about::ShowAboutWindow()`, `style_editor::ShowStyleEditor()`.

## Skill files (REQUIRED)

Task-specific instructions live under `skills/`. Read the relevant file(s) before performing the corresponding task.

| Skill file | Read BEFORE... |
|---|---|
| `skills/playwright.md` | Writing/editing any `tests/integration/test_*.das` or `record_*.das` driver, or the node-editor `ne_*` layer — the **async rule** (gate on the effect, not a frame/sleep guess), the `wait_*` family + what each guarantees, wait-gate holes, one-host-per-9090 (incl. Win/WSL localhost bridging) |
| `skills/recording.md` | Writing/editing any `tests/integration/record_*.das` driver — pacing constants, two-shell vs one-shell workflow, menu-bbox quirks, APNG→MP4 conversion |
| `skills/migration.md` | Migrating any v1 daslang+imgui code (`require imgui/imgui_boost`, `imgui_app(...) <\| $() {...}`, raw `NewFrame()`/`Begin()`/etc.) to dasImgui v2. Read when you hit IMGUI001 / IMGUI002 in compile output |

## Build

CMake-based. Three shared-module targets (`dasModuleImgui`, `imguiApp`, `imguiAppHeadless`):

```bash
cd <dasimgui>
cmake -B _build -S . -DDASLANG_DIR=<daslang>
cmake --build _build --config Release
```

Artifacts land at the repo root (NOT in `_build/`): `dasModuleImgui.shared_module`, `imguiApp.shared_module`, `imguiAppHeadless.shared_module`. The `.das_module`'s `initialize()` registers them via `register_dynamic_module()` when daslang loads any `.das` file under this tree.

Stop any running `daslang-live` / `imguiApp` consumers before rebuilding — on Windows in particular, the OS holds locks on loaded DLLs.

**Stale-build-dir gotcha (cmake upgrade).** `_build` (and any legacy `build_msvc`) is pinned to the cmake that generated it. After a system cmake upgrade (e.g. 3.24 → 4.3), `cmake --build _build` fails its reconfigure with `.../share/cmake-<old>/Modules/... does not exist`. Fix: delete the stale dir(s) and reconfigure fresh — `rm -rf _build build_msvc && cmake -B _build -S . -DDASLANG_DIR=<daslang>`. (`build/` is the sphinx docs output, NOT a cmake dir — leave it.) The `.shared_module` artifacts at the repo root are untouched by a failed reconfigure, so a rebuild is only needed if the daslang module ABI changed (a `-compile-only` load of any example tells you: clean = still compatible).

## Running a .das file

Single-shot windowed:

```bash
<daslang>/bin/Release/daslang -project_root . examples/features/with_indent.das
```

Single-shot headless (terminates after N frames):

```bash
<daslang>/bin/Release/daslang -project_root . examples/features/with_indent.das -- --headless --headless-frames=60
```

Compile-only (lint pass + type check, no simulate):

```bash
<daslang>/bin/Release/daslang -project_root . -compile-only examples/imgui_demo/main.das
```

Live-reload server (keeps running until killed; HTTP API on port 9090):

```bash
<daslang>/bin/Release/daslang-live -project_root . examples/imgui_demo/main.das
```

(Append `.exe` on Windows where appropriate. `Release/` is the MSVC multi-config layout — on single-config Ninja/Make `<daslang>/build/daslang` is the binary.)

## Examples layout (`examples/`)

| Dir | Purpose | Lifecycle |
|---|---|---|
| `features/` | Small focused demos of one wrapper at a time. Each file is `~20-80 LOC`, opens its own window via `imgui_harness`, drives `[test]` smokes in `tests/integration/test_<name>.das`. | `harness_*` |
| `imgui_demo/` | Daslang port of `imgui_demo.cpp` — per-scene modules (`about.das`, `widgets.das`, etc.) consumed by `imgui_demo.das` (the dispatcher). `main.das` is the live-reload entry. Single-scene `harness_<scene>.das` files exist for headless smokes + recordings. | mixed: `imgui_demo.das` is library, `main.das` is `live_*`, `harness_*.das` are `harness_*` |
| `tutorial/` | Annotated step-by-step tutorials matching `doc/source/tutorials/*.rst`. Use `live_*` lifecycle so the live-reload tutorial flow works. | `live_*` |
| `save_demo/` | One-shot save/load demo. | self-contained |
| `graphics/` | Custom-GL + ImGui interop demo (Fourier viz). | self-contained, mixes raw GL and harness/live. |

## Lifecycle: `harness_*` vs `live_*`

Two distinct frame-loop APIs:

**`live_*` (in `live_host` + `imgui_live`):** lowest-level. Tutorial files use this:

```daslang
[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()
    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()         //! REQUIRED for synth IO to drain
    NewFrame()
    // ... widgets ...
    end_of_frame()
    Render()
    // ... GL clear + live_imgui_render() ...
    live_end_frame()
}
```

**`harness_*` (in `imgui_harness`):** higher-level wrapper used by `examples/features/*.das` and `examples/imgui_demo/harness_*.das`. Same flow folded into 3 calls:

```daslang
[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()               // runs imgui_synth_tick() built-in — synth IO drains here
    // ... widgets ...
    harness_end_frame()
}
```

**`harness_new_frame()` runs `imgui_synth_tick()` itself** (between the backend `*_NewFrame()` and ImGui's `NewFrame()`), so synthetic mouse/keyboard and `set_user_control` work on every harness app with **no opt-in** — a harness recording/test that posts `imgui_mouse_play` / `imgui_click` needs no extra call. (Earlier versions exposed a separate `harness_apply_synth_io()`; that was folded into `harness_new_frame()` and no longer exists.) The `live_*` lifecycle has no such fold — it must call `apply_synth_io_override()` explicitly (see above).

## Lint (`widgets/imgui_lint.das`)

Default-on for every `.das` file under this tree. Forbids any raw `imgui::Fn` call where `Fn` isn't in `ALLOWED_IMGUI`. Two error codes:

- **IMGUI001**: `imgui_boost::Fn` (legacy v1 surface) — dead, no per-file escape.
- **IMGUI002**: `imgui::Fn` where `Fn` ∉ `ALLOWED_IMGUI`. Per-file escape: `options _allow_imgui_legacy = true`.

`_allow_imgui_legacy = true` is **scaffolding-only** — target state is zero opt-outs. Track the remaining files via `git grep _allow_imgui_legacy` before final delivery.

To add a new raw call to the carve-out, edit `ALLOWED_IMGUI` in `widgets/imgui_lint.das`. Categories already in place: io/state queries, mouse/keyboard state, cursor/layout queries, font/color helpers, draw-list primitives, demo/debug entry points, pre-window state, drag-drop payload primitives, input-text callback primitives.

## Widget telemetry & path-prefix

Every `[widget]` / `[container]` registers under a slash-joined path: `WINDOW_IDENT/CONTAINER_IDENT/WIDGET_IDENT`. `with_id("scope") { ... }` adds a literal scope segment.

**Indexed widget tables** (e.g. one widget per loop iteration) must be declared at module scope explicitly — `[widget]` macros do NOT auto-emit table globals for indexed forms:

```daslang
var private STYLE_COLOR_NAME : table<int; NarrativeState>
// ...
for (col in type<ImGuiCol>) {
    text(STYLE_COLOR_NAME[int(col)], (text = name))
}
```

**Header bbox capture for `menu` and `tab_item`.** `menu()` and `tab_item()` snapshot `GetItemRectMin/Max` right after `BeginMenu`/`BeginTabItem` returns — the parent-strip header is the "last item" at that point, so the registered bbox is the clickable header rect. `main_menu_bar()` itself still reports `bbox = (0,0,0,0)` (the bar chrome has no meaningful "header"); recording drivers that need to click the bar's screen position hardcode pixel coords. See `skills/recording.md`.

## Tests (`tests/integration/`)

Tests run via `dastest`. Two equivalent local recipes — one mirrors CI exactly (`daspkg install --global`), one skips the install and uses this checkout directly via `-load_module`. **Both require `cwd = <dasImgui-root>`** because (a) the `modules/dasImgui` self-junction has to be visible to satisfy `--test modules/dasImgui/tests/integration` AND (b) `test_module_isolation.das` does `fopen("modules/dasImgui/widgets/imgui_visual_aids.das")` relative to cwd. **Both require `--headless`** — without it, the spawned daslang-live subprocesses pop real GLFW windows and flake on focus/port-reuse.

**Recipe A — CI mirror (daspkg --global):**

```bash
cd <daslang>
<daslang>/bin/Release/daslang utils/daspkg/main.das -- install <path-to-this-dasImgui> --global
<daslang>/bin/Release/daslang dastest/dastest.das -- \
    --test modules/dasImgui/tests/integration \
    --headless \
    --exclude glfw_synth --exclude key_hud
```

**Recipe B — local-only (`-load_module`, no install):**

```bash
cd <dasImgui-root>                             # so modules/dasImgui self-junction resolves
<daslang>/bin/Release/daslang \
    -load_module <dasImgui-root> \             # daslang flag, BEFORE the script path
    <daslang>/dastest/dastest.das -- \
    --test modules/dasImgui/tests/integration \
    --headless \
    --exclude glfw_synth --exclude key_hud
```

`-load_module` is the daslang-side flag (see `daslang -h`) and **must come before the dastest script path**; placing it after `--` makes dastest swallow it and daslang's default project_root scan never picks up the module's `.das_module` (`register_native_path("imgui", "imgui_harness", ...)`) — so the very first test fails with `error[20605]: missing prerequisite 'imgui/imgui_harness'`. The self-junction alone (`modules/dasImgui -> .`) is NOT sufficient; daslang's project_root scan skips self-references.

**Windows local runs also need the same `--exclude` set as Windows CI** (libhv's IOCP path stalls at 16 POSTs/subprocess):

```bash
    --exclude inputs_drag --exclude inputs_numeric --exclude inputs_slider \
    --exclude indexed_dynamic --exclude inputs_color --exclude inputs_choice \
    --exclude inputs_text
```

The `--headless` flag propagates to spawned daslang-live subprocesses via `widgets/imgui_playwright.das:515` (`playwright_wants_headless`).

**Process cleanup between runs.** A killed/timed-out dastest leaves the live `daslang-live` child holding port 9090; the next run then fails immediately with `ERROR: another instance of daslang-live is already running on port 9090` on every test. Sweep before re-running:

```powershell
Get-Process -Name 'daslang','daslang-live','dastest','imguiApp','imguiAppHeadless' -ErrorAction SilentlyContinue | Stop-Process -Force
```

**Three test families:**

- `test_<feature>.das` — `[test]` functions calling `with_imgui_app(FEATURE) $(d) { ... }`. Boots a daslang-live subprocess running the feature, waits for widgets, asserts state. ~2 POSTs per test; well under the 16-POST Windows-CI threshold.
- `test_imgui_lint_*.das` — lint allow/forbid smokes. `failed_imgui_lint_raw.das` uses `expect 50503` for negative test.
- `record_<scene>.das` — recording drivers, two-shell against an already-running daslang-live. **NOT in CI** — manual artifact-producing scripts. See `skills/recording.md`.

CI workflow: `.github/workflows/tests.yml`. Windows CI excludes 7 high-POST tests (libhv's IOCP path stalls at 16 POSTs/subprocess on `windows-latest`); Linux + macOS run the full suite.

## Documentation (`doc/source/`)

Sphinx-based tutorial site. CI gates `-W` (warnings-as-errors) BUT pre-runs `utils/imgui2rst.das --detail_output doc/source/stdlib/generated` to generate per-module stdlib refs. **Locally**, `sphinx-build -W` fails on `stdlib/sec_boost.rst:11: toctree contains reference to nonexisting document 'stdlib/generated/imgui_boost_v2'` unless you run imgui2rst first. Plain `sphinx-build --keep-going` (no `-W`) builds clean.

Tutorial pages live in `doc/source/tutorials/*.rst` and embed MP4 recordings from `doc/source/_static/tutorials/*.mp4` via the local `.. video:: name.mp4` directive (registered in `doc/source/tutorial_video.py`). Each tutorial appears in `doc/source/tutorials/index.rst`'s toctree.

### Icon catalog (`imgui_icons`)

The icon set ships a generated catalog page (`generated/imgui_icons.rst`, built by `generate_icon_catalog()` in `utils/imgui2rst.das` straight from the module's `icon_count`/`icon_name`/`icon_category` accessors — it can't drift from the set). Its images are **committed PNGs** under `doc/source/_static/icons/<name>.png`, one per glyph. The `:icon:`name`` inline role (registered in `doc/source/icon_role.py`) renders a glyph inline in any `//!` docstring or handmade page.

`imgui_icons` draws **runtime vector geometry** (no asset files), so the PNGs are rendered offline by **`utils/make_icon_doc.das`** — the single regeneration entry point — which drives a real ImGui frame and reads back the framebuffer, so it must run **windowed** (not in CI):

```bash
<daslang>/bin/Release/daslang -project_root . utils/make_icon_doc.das
```

Run it after adding/changing a glyph in `widgets/imgui_icons.das`, then commit the new PNGs. A missing PNG fails the `-W` sphinx build (the catalog references it), so CI catches a forgotten regen.

## Tutorial recordings — MP4 in source, APNG ignored

**`doc/source/_static/tutorials/*.mp4` ships in-tree** (~5 MB total for the full set). The `.apng` plus all voiceover/music intermediates (`voiceover/*.wav`, `*.manifest.json`, `*.sidecar.json`, `*_music.wav`, `*.mp4.ffmpeg.txt`) are gitignored — only the `.mp4` deliverable is tracked.

**Three hard requirements for any recording (REQUIRED — see `skills/recording.md`):** (1) **do what it teaches** — every stage performs the real interaction it narrates (click the button so the counter ticks), never just points at a widget; (2) **self-verify every step** — a recording is also an integration test: clicks go through `hold_through_voice` (verified by widget kind) and value changes through `force_set_verified`, and a no-op interaction aborts the recording loudly at teardown; (3) **pace by the voice** — dwell is the voiceover wav length (`say_begin` returns it, `hold_through_voice` splits it), not hand-tuned sleeps. Captions/voice must be **ASCII** (the font has no em-dash/arrow glyph). These rules apply to ALL recordings; legacy pre-voice drivers are a retrofit backlog, not an exemption.

**Fresh clone before docs build:** nothing to fetch. The `.mp4` files are in the checkout. The `.. video::` directive references them.

**Soundtracked workflow:** `prepare_recording --driver record_X.das` (scan say() lines → Kokoro TTS → manifest) → `daslang.exe -project_root . tests/integration/record_X.das` (auto-arms voiceover; writes sidecar) → eyeball-review the `.apng` → `convert_recording --apng X.apng` (music bed + voiceover mux → `X.mp4`) → `git add X.mp4`. Full mechanics: `skills/recording.md`.

## Recording

See `skills/recording.md` for the full recipe — the three hard requirements (do-what-it-teaches, self-verify every step, voice-driven pacing), the prepare → record → convert soundtrack pipeline, the `say_begin`/`hold_through_voice`/`force_set_verified` driver model, menu-header coord workaround, and screenshot/ffmpeg verification methodology.

## Workflow

Per-PR: eyeball-review only, no Copilot, no required CI gate, branches as backup. After landing a change, push the branch and open a draft PR to trigger CI (workflows don't fire on bare branch push; they fire on `pull_request`).

When debugging recordings or live-API behaviour:
1. Stop stale `daslang-live` / `imguiApp` processes first.
2. Launch fresh with `-project_root .`.
3. Use `mcp__daslang__live_command` for ground-truth probes: `screenshot`, `imgui_snapshot`, `imgui_mouse_status`, `help`.
4. **Always full-restart daslang-live between recording iterations** — any interactive probe (menu click, key press) leaves state that contaminates the next recording.

## Asking the mouse

For "how do I X?" / "why does Y behave this way?" questions about dasImgui patterns, use `mcp__mouse__mouse__ask` BEFORE grep/research. The blind-mouse cache (`mouse-data/docs/`) holds curated Q&A on dasImgui-specific recurring traps — synth IO setup, harness vs live lifecycle, lint carve-out additions, recording driver patterns, menu-bbox quirks, etc.

## Followup-tracked gaps

**Final-delivery gate: ACHIEVED as of PR-D (2026-05-19).**

All `examples/imgui_demo/*.das` files are opt-out-free: `git grep _allow_imgui_legacy examples/imgui_demo/` returns nothing. Every `app_*.das` carries a real port. The 24-section `tables.das` port landed in PR-D (cpp:4154-6044, 2911 das lines across 8 commits).

Two example files keep ALIVE opt-outs that are intentional, not gaps:

- `examples/tutorial/custom_widgets.das` — teaches building widgets from primitives; uses raw imgui by design.
- `examples/features/widget_no_ident.das` — exercises the STYLE001-rejected `text((text=...))` form for didactic value.

One cpp construct remains deliberately partial in the port:

- **Section 24 Advanced's cpp Debug-details readout** (cpp:6019-6031, `ImDrawList.CmdBuffer.Size + scroll cur/max`) — `ImVector\`ImDrawCmd` has no `Size` accessor exposed in the daslang binding. The checkbox + readout are dropped; re-add when an `ImVector` size binding lands. Documented inline in `tables.das show_advanced()`.

Section 21 sub 2's per-column manual-open popups (formerly partial) shipped in the same PR as the new `popup_window` `[container]` — the cpp shape now runs 1:1 against the boost surface.
