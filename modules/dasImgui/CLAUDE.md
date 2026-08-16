# dasImgui module instructions

dasImgui is the daslang binding + boost-v2 wrapper layer for [Dear ImGui](https://github.com/ocornut/imgui), **in-tree at `modules/dasImgui/`** and built by default (root CMake option `DAS_IMGUI_DISABLED`, default `OFF`; needs the in-tree dasGlfw + dasClipboard). It ships the C++ native binding (`bind/`, `src/`; also a `dasModuleImgui.shared_module` for DLL builds), the boost-v2 wrapper layer (`widgets/` — `[widget]` / `[container]` / `with_*` macros, telemetry, default-on lint), the `imguiApp` (windowed GLFW+GL) and `imguiAppHeadless` (no display, real ImGui ctx, CPU font atlas) harness backends, and examples under `examples/`.

The old standalone repo (borisbat/dasImgui) is archived with full history. `daspkg` recognizes `require_package("dasImgui")` as in-tree and reports *part of this daslang tree — nothing to install*; in-repo example `.das_package` manifests do NOT declare it (`daspkg release` — native and wasm — discovers module archives from the compiled require chain via `daslang -exe --list-shared-modules`; the manifest `dependencies()` section drives only the install flow, which has nothing to install for an in-tree module).

## Locations

- Module source: `modules/dasImgui/` (`bind/`, `src/`, `widgets/`, `examples/`, `utils/`)
- Tests: `modules/dasImgui/tests/` — nightly CI lane `.github/workflows/nightly_imgui.yml`; see `modules/dasImgui/tests/README.md`
- Docs: stdlib section `doc/source/stdlib/sec_imgui.rst` (+ generated pages), tutorials `doc/source/reference/tutorials/imgui/*.rst` (see Documentation below)
- Skills: **repo root** `skills/imgui_*.md` (see table below)
- Recordings: intermediates in `doc/source/_static/tutorials/` (gitignored); MP4 deliverables on the rolling `docs-assets` GitHub release (see Recordings below)

## Skill files (REQUIRED)

| Skill file | Read BEFORE... |
|---|---|
| `skills/imgui_ui_debugging.md` | Diagnosing/fixing ANY UI or interaction bug (also mandated by the root CLAUDE.md) |
| `skills/imgui_playwright.md` | Writing/editing any `modules/dasImgui/tests/test_*.das` or `record_*.das` driver — the **async rule** (gate on the effect, not a frame/sleep guess), the `wait_*` family, one-host-per-9090 |
| `skills/imgui_recording.md` | Writing/editing any `record_*.das` driver — pacing constants, workflow, APNG→MP4 conversion |
| `skills/imgui_migration.md` | Migrating v1 daslang+imgui code (`require imgui/imgui_boost`, raw `NewFrame()`/`Begin()`) to v2. Read when you hit IMGUI001 / IMGUI002 |
| `skills/imgui_application.md` | Structuring a long-running dasImgui app (init/update/shutdown lifecycle, heap ownership) |

## Build

Builds with the normal daslang build (`cmake --build build --config Release`); targets `dasModuleImgui`, `imguiApp`, `imguiAppHeadless` exist for DLL/shared-module consumers (artifacts land in `modules/dasImgui/`, not the build dir). Stop any running `daslang-live` / `imguiApp` consumers before rebuilding — on Windows the OS holds locks on loaded DLLs.

## Binding regen (`bind/bind_imgui.das`)

The raw binding under `src/` is GENERATED. After changing the binding surface or bumping the imgui version, regenerate and commit:

```bash
./bin/daslang modules/dasImgui/bind/bind_imgui.das
git diff -- modules/dasImgui/src/     # commit the changes
```

Needs the dasClangBind/libclang stack — read `skills/clang_bind_build.md` first. CI freshness gate: `build.yml`'s mingw nightly worker runs the self-binder and fails on a dirty `modules/dasImgui/src/`.

**imgui version pin:** v1.92.6-docking, fetched via CMake FetchContent at build time. Bumping it means re-running the self-binder and committing the regenerated `src/`.

## Module resolution

- `require imgui/<name>` resolves via the module's `.das_module` `register_native_path` entries; the in-tree project-root scan picks them up. Run from the repo root; recipes pass `-project_root .` where needed (e.g. record drivers).
- `require` only resolves **siblings** of the calling file's directory and the registered native paths. **No `..`/absolute-from-root forms.** Files that need both `imgui/*` and a sibling module must live in the sibling's directory.
- If a sibling name collides with an `imgui::` builtin (`ShowAboutWindow`, `ShowStyleEditor`, …), qualify at the call site: `about::ShowAboutWindow()`.

## Examples layout (`modules/dasImgui/examples/`)

- `features/` — small focused demos, one wrapper per file (~20-80 LOC); drive `[test]` smokes in `modules/dasImgui/tests/test_<name>.das`. `harness_*` lifecycle.
- `imgui_demo/` — daslang port of `imgui_demo.cpp`: per-scene modules consumed by `imgui_demo.das`; `main.das` is the live-reload entry; `harness_<scene>.das` for headless smokes + recordings.
- `tutorial/` — annotated tutorials matching `doc/source/reference/tutorials/imgui/*.rst`; `live_*` lifecycle so the live-reload tutorial flow works.
- `save_demo/` — one-shot save/load demo.

## Lifecycle: `harness_*` vs `live_*`

Two distinct frame-loop APIs. **`harness_*`** (in `imgui_harness`, used by `examples/features/*.das`): `harness_begin_frame()` / `harness_new_frame()` / `harness_end_frame()`. `harness_new_frame()` runs `imgui_synth_tick()` itself, so synthetic mouse/keyboard and `set_user_control` work on every harness app with no opt-in. **`live_*`** (in `live_host` + `imgui_live`, used by `examples/tutorial/`) is lowest-level and has no such fold — it MUST call `apply_synth_io_override()` explicitly between the backend `*_NewFrame()` and ImGui's `NewFrame()`.

## Lint (`widgets/imgui_lint.das`)

Default-on for every `.das` file requiring imgui. Forbids raw `imgui::Fn` where `Fn` isn't in `ALLOWED_IMGUI`:

- **IMGUI001**: `imgui_boost::Fn` (legacy v1 surface) — dead, no per-file escape.
- **IMGUI002**: `imgui::Fn` ∉ `ALLOWED_IMGUI`. Per-file escape: `options _allow_imgui_legacy = true` — **scaffolding-only**, target state is zero opt-outs.

To add a raw call to the carve-out, edit `ALLOWED_IMGUI` in `widgets/imgui_lint.das`. Two intentional (not gap) opt-outs remain: `examples/tutorial/custom_widgets.das` (teaches building widgets from primitives) and `examples/features/widget_no_ident.das` (didactic STYLE001 counter-example).

## Widget telemetry & path-prefix

- Every `[widget]` / `[container]` registers under a slash-joined path `WINDOW_IDENT/CONTAINER_IDENT/WIDGET_IDENT`; `with_id("scope")` adds a literal segment.
- **Indexed widget tables** (one widget per loop iteration) must be declared at module scope explicitly — `[widget]` macros do NOT auto-emit table globals for indexed forms: `var private STYLE_COLOR_NAME : table<int; NarrativeState>`.
- `menu()` / `tab_item()` snapshot the header bbox right after `BeginMenu`/`BeginTabItem`, so the registered bbox is the clickable header rect; `main_menu_bar()` itself reports `bbox = (0,0,0,0)` — recording drivers that click the bar hardcode pixel coords.

## Tests (`modules/dasImgui/tests/`)

Run from the repo root (full recipe + curl smoke + recording workflow: `modules/dasImgui/tests/README.md`):

```bash
./bin/daslang dastest/dastest.das -- --test modules/dasImgui/tests --headless
```

- ~161 test files, one daslang-live subprocess each; add `--isolated-mode --isolated-mode-threads 4` to parallelize. **`--headless` is required** — without it the spawned subprocesses pop real GLFW windows and flake on focus/port-reuse.
- Nightly-only in CI (`nightly_imgui.yml`, ubuntu + macos): the `tests/.das_test` gate skips this dir in `--test tests/` full sweeps unless `--imgui` is passed; targeting the folder directly bypasses the gate. CI excludes `glfw_synth`, `key_hud`, `embedded_terminal`. Windows CI is deliberately absent (runner fastfail 0xC0000409); local Windows runs the full suite with no extra excludes.
- **Process cleanup between runs:** a killed dastest leaves a daslang-live child holding port 9090; sweep `daslang`/`daslang-live`/`dastest`/`imguiApp`/`imguiAppHeadless` processes before re-running — **by PATH, never bare by name** (a bare name-kill also murders the dasHerd watcher and every other tree's daslang; observed 2026-07-29). Sweep command: `skills/imgui_playwright.md`.
- Families: `test_<feature>.das` (`with_imgui_app` playwright tests), `failed_imgui_*.das` (lint negative smokes), `record_<scene>.das` (recording drivers — NOT in CI).

## Documentation

Docs live in the main Sphinx tree, published at daslang.io/doc:

- **Stdlib pages**: `doc/source/stdlib/sec_imgui*.rst` + generated pages from `modules/dasImgui/utils/imgui2rst.das` (writes `doc/source/stdlib/generated/`, gitignored). `doc.yml`/`pages.yml` run it before sphinx; locally run it first or `sphinx-build -W` fails on the missing generated toctree entries.
- **Tutorials**: `doc/source/reference/tutorials/imgui/*.rst`, embedding MP4s from `doc/source/_static/tutorials/` via the `.. video::` directive. MP4s are NOT in git — staged from the `docs-assets` release by `utils/docs_assets/fetch.{sh,ps1}`; sphinx `-W` fails on a missing video, so **upload the MP4 to `docs-assets` before the RST cite lands**.
- **Icon catalog**: `generated/imgui_icons.rst` is built by `generate_icon_catalog()` in `utils/imgui2rst.das`; its images are committed PNGs under `doc/source/_static/icons/`, rendered offline by `daslang -project_root . modules/dasImgui/utils/make_icon_doc.das` (drives a real ImGui frame — must run **windowed**, not in CI). Re-run after adding/changing a glyph in `widgets/imgui_icons.das` and commit the PNGs.

## Recordings

Three hard requirements for any recording (see `skills/imgui_recording.md`): (1) **do what it teaches** — every stage performs the real interaction it narrates, never just points; (2) **self-verify every step** — clicks go through `hold_through_voice`, value changes through `force_set_verified`, and a no-op interaction aborts loudly at teardown; (3) **pace by the voice** — dwell is the voiceover wav length, not hand-tuned sleeps. Captions/voice must be **ASCII**.

Soundtracked pipeline (`modules/dasImgui/utils/prepare_recording.das` / `convert_recording.das`): prepare → record (`daslang -project_root . modules/dasImgui/tests/record_X.das`) → eyeball the `.apng` → convert to `.mp4` → `gh release upload docs-assets X.mp4 --clobber`. Full mechanics: `modules/dasImgui/tests/README.md` + `skills/imgui_recording.md`.

When debugging recordings or live-API behaviour: stop stale `daslang-live`/`imguiApp` processes first; probe ground truth via `mcp__daslang__live_command` (`screenshot`, `imgui_snapshot`, `imgui_mouse_status`, `help`); **always full-restart daslang-live between recording iterations** — interactive probes leave state that contaminates the next recording.

## Known deliberate gaps

- **Section 24 Advanced's cpp Debug-details readout** in the `imgui_demo` tables port (`ImVector<ImDrawCmd>.Size` + scroll cur/max): no `Size` accessor exposed in the binding; checkbox + readout dropped, documented inline in `tables.das show_advanced()`. Re-add when an `ImVector` size binding lands.
