# Changelog

All notable changes to dasImgui are documented in this file. Format based on
[Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [Unreleased]

### Added
- Embedded PowerShell/ConPTY terminal UI via `imgui/imgui_terminal`, with an
  accessible docked example, semantic live snapshots, selection and clipboard
  actions, cursor state, Unicode font fallbacks, and five-percent zoom controls.
- Shared Unicode text infrastructure: role-based prose/monospace font stacks
  with color emoji, grapheme-safe flow and source views, source-mapped
  CommonMark/GFM documents and Markdown rendering, selectable rich/plain text,
  semantic URI/image controls, clipboard exports, host-resolved images, and
  deterministic live interaction coverage. `MarkdownTypography` lets hosts
  override heading and code proportions while keeping measurement, wrapping,
  hit-testing, selection, and painting synchronized.
- Editable document projections and semantic command bindings. Canonical UTF-8
  source preserves BOM and line endings across save/load; GFM checkboxes apply
  exact source edits; menu, keyboard, toolbar, and live inputs share command IDs
  with scoped, conflict-detectable, explicitly persisted bindings.
- **F11 toggles fullscreen** in `imgui_harness` (`widgets/imgui_harness.das`) — in `harness_new_frame` (after `NewFrame`, windowed mode only): web routes through the dasGlfw `glfw_toggle_fullscreen()` extern (real browser fullscreen — the canvas grows to screen at native scale + DPR, not a CSS-magnified iframe), desktop uses the existing `glfwToggleNativeFullscreen(live_window)` (native macOS fullscreen, no-op elsewhere). Headless is skipped. Requires daslang with `glfw_toggle_fullscreen` (`GaijinEntertainment/daScript#3326`).
- `with_font(font, size)` (`widgets/imgui_scope_builtin.das`) — the scope wrapper now forwards a per-push pixel size to `PushFont` (imgui 1.92 dynamic fonts), so one font renders at several sizes in a single frame. `size` defaults to `0.0f` (keep current size), so existing `with_font(font) { ... }` call sites are unchanged.
- 1.92-API tutorials + walkthroughs (`examples/tutorial/` + `doc/source/tutorials/`): `dynamic_fonts` (`FontScaleMain` + `with_font(font, size)`), `texture_ref` (`ImTextureRef` / `TexData` null-gate), `item_flags` (`with_item_flag` + `ImGuiItemFlagsPrivate` cross-enum `|`), `log_capture` (`with_log` + renamed `ImGuiLogFlags`). Plus `examples/features/dynamic_font_scale.das` + the `tests/integration/test_dynamic_font_scale.das` size-gate smoke.
- `imgui_mouse_click_at` live command (`widgets/imgui_live_core.das`) — atomic click at `(x, y)`: travel → press → release as one self-contained synth timeline, with a `button` arg (`0`=left, `1`=right, `2`=middle) and optional `travel_ms`. Callers no longer hand-build a `click_at` event array for `imgui_mouse_play`; one round-trip can't interleave with real input mid-gesture.
- `set_user_control` live command (`widgets/imgui_live_core.das`) — `{enabled:false}` makes `imgui_synth_tick` clear the GLFW backend's queued mouse/keyboard each frame (via `GetIO().ClearEventsQueue()`) so ONLY synth IO drives the UI, with no real-input bleed-through racing the synth cursor; `{enabled:true}` releases synth ownership (stops playback, releases held buttons/keys, clears `synth_cursor_owned`) and hands control back. Defaults on — zero behavior change when never called. Public getter `user_control_enabled()`.
- Live docs site at https://borisbat.github.io/dasImgui
- GitHub repo link + daspkg install snippet on the docs landing page (`doc/source/index.rst`)
- "Edit on GitHub" header link on every docs page (via `sphinx_rtd_theme.html_context` in `doc/source/conf.py`)
- `skills/migration.md` skill file covering the v1->v2 API migration recipe
- CI / docs / license badges on README
- `CHANGELOG.md` (this file) — Keep-a-Changelog format, retroactively documents the v2.0 stabilization

### Changed
- **Bumped Dear ImGui 1.90.6-docking → 1.92.6-docking** (`CMakeLists.txt` FetchContent pin + hash). Regenerated the native binding (`bind/bind_imgui.das` → `src/`) and migrated the boost-v2 layer + examples to the 1.92 API:
  - **Dynamic-fonts/texture rework:** `Image`/`ImageButton`/`ImageWithBg` now take `ImTextureRef` (manual `const ImTextureRef&` wrappers so a const `GetIO().Fonts.TexRef` binds); `ImFontAtlas.TexID`→`TexRef`; headless harness claims `ImGuiBackendFlags_RendererHasTextures` and drains the per-frame texture queue (`imgui_headless_process_textures`).
  - **Enum/flag renames:** `ImGuiDir`/`ImGuiSortDirection` are now C++11 typed enums; `ImGuiCol_Tab{Active,Unfocused,UnfocusedActive}`→`Tab{Selected,Dimmed,DimmedSelected}`, `NavHighlight`→`NavCursor`; `ImGuiChildFlags_Border`→`Borders`; `FittingPolicyResizeDown`→`FittingPolicyShrink`; `SpanTextWidth`→`SpanLabelWidth`; `ImGuiColorEditFlags_AlphaPreview`→`AlphaOpaque`; `ImGuiLogType`→`ImGuiLogFlags`; `ImGuiNavHighlightFlags`→`ImGuiNavRenderCursorFlags`; private item flags (`MixedValue`/`ReadOnly`/`AllowOverlap`) split into `ImGuiItemFlagsPrivate` (cross-enum `|` rail added).
  - **Moved/removed API:** `io.FontGlobalScale`→`style.FontScaleMain`; `PushFont` gained a size arg; `ClearInputKeys` split out `ClearInputMouse`; `PushButtonRepeat`/`PushTabStop`→`PushItemFlag(ImGuiItemFlags_{ButtonRepeat,NoTabStop})`; `DockSpaceOverViewport` gained a leading `dockspace_id`; `RenderTextEllipsis` dropped `clip_max_x`; `RenderDragDropTargetRect`→`RenderDragDropTargetRectForItem`; removed `SetWindowFontScale`, `GetWindowContentRegionMax`, `GetContentRegionMaxAbs`, `RenderRectFilledRangeH`, `ImGuiButtonFlags_Repeat` (demos adapted/dropped); nav config flags moved to `io.ConfigNav*` bools.
- `IMGUI001` / `IMGUI002` lint messages in `widgets/imgui_lint.das` now point at `skills/migration.md` for the v1->v2 mapping table
- README "Usage" example now shows only the canonical `imgui_harness` path; the dead gen1 `imgui_app(name) <| $() { ... }` example is removed (it would IMGUI001/IMGUI002-violate against the current lint)
- README "Examples" section paths fixed (`examples/` plural, dropped nonexistent `imgui_opengl2.das`, added `features/with_indent.das` + `tutorial/`)
- README "Documentation" section no longer says gh-pages is "forthcoming"
- README "Modules" table reordered — `imgui/imgui_harness` is now the canonical first entry; the dead `imgui/imgui_boost` row is gone
- `CLAUDE.md` adds a `Skill files (REQUIRED)` table mirroring the daslang pattern; indexes `skills/recording.md` and the new `skills/migration.md`

## [2.0] - 2026-05-15

The 2.0 line stabilizes the v2 boost-macro surface. `widgets/imgui_lint.das`
is default-on, rejecting raw `imgui::*` calls (IMGUI002) and any reference
to the dead `imgui_boost` v1 module (IMGUI001). All in-tree examples and
tests migrated to the v2 surface; the per-file `_allow_imgui_legacy` escape
is scaffolding only.

### Changed

- **`[widget_dispatch]` annotation re-keyed by state-struct type, not by
  function name.** One `[widget_dispatch] def _(var state : T; action;
  payload)` body fires for every widget kind whose state is `T` — the
  fn name is irrelevant. Dispatch lookup at HTTP-receive time uses
  `meta.ti.hash` (one global `g_dispatchers : table<uint64;
  KindDispatcher>`); non-Structure `ti` (scalar pointee from
  `[edit_widget]`) falls through to a generic `sscan_json_at(state_addr,
  *ti)` in `dispatch_or_err`. Built-in widgets builtin file no longer
  carries the `_register_*_dispatch_kinds()` `[init]` blocks (gone:
  ~150 explicit `register_widget_kind` calls); ~50 per-kind
  `dispatch_*` helpers collapse to ~35 per-state-struct
  `[widget_dispatch]` bodies. Display-only state structs
  (Narrative/Empty/Plot/Image/etc.) get no dispatcher and silently
  no-op.
- **Per-widget serializer/dispatcher lambdas killed.** Pre-PR4 every
  `[widget]` body emitted 2 heap-allocated closures per render
  (`ser <- @ capture(...)` + `disp <- @ capture(...)`); `widget_finalize`
  routed them through path-keyed `g_serializers` / `g_dispatchers`
  tables. Post-PR4: `widget_finalize` takes `(state_addr : void?, ti :
  TypeInfo const?)` directly, no closures emitted. Snapshot rail
  routes through `sprint_json_at(state_addr, *ti)` (which now matches
  daslib `JV<vec>` wire shape after upstream daslang #2871). Compile
  delta on `examples/imgui_demo/imgui_demo.das`: **−19.8% wall**
  (7.01s → 5.62s mean, every patched run beats every baseline run).
- **`[edit_widget]` family no longer needs per-pointee-type dispatcher
  registrations.** Pre-PR4: 10 hardcoded `dispatch_edit_bool` /
  `dispatch_edit_int` / `dispatch_edit_float3` / ... wrappers + one
  `register_widget_kind` per edit_* kind. Post-PR4: 0. The scalar-
  pointee `ti` is recorded in `g_widgets[path].ti`; `dispatch_or_err`
  detects non-Structure ti and uses it to drive
  `sscan_json_at(state_addr, *ti)` directly. Generic auto(T) edit_*
  rails (`edit_drag_scalar` / `edit_input_scalar` / `edit_checkbox_flags`
  / ...) work out of the box — meta.ti varies per instantiation, the
  generic path covers everything.

### Added

- `popup_window(IDENT, str_id, flags) $blk` — stateless `[container]`
  for the manual-trigger BeginPopup/EndPopup pattern. Caller drives
  `open_popup(str_id, flags)` from any custom predicate (hover-region +
  right-click, key press, …); the wrapper brackets the body while ImGui
  reports the popup open. Closes the cpp ``imgui_demo.cpp`` Tables
  "Context menus" section 2 partial-port deferred in PR-D (per-column
  ``"MyPopup"`` shared-str_id pattern keyed by ``PushID(column)``). The
  ``[container]`` macro injects ``widget_prelude → PushID(widget_ident)``
  before the body — but the caller's ``OpenPopup`` ran one ID-stack
  level higher, so ``BeginPopup`` hashed under a deeper stack and the
  popup never visibly opened. Fix: ``popup_window`` peels its own
  ``widget_ident`` PushID off just around ``BeginPopup`` (PopID before,
  PushID after), so BeginPopup's hash matches the caller's OpenPopup.
- `tests/integration/test_popup_window.das` — smoke + click-opens-popup
  pair. Verifies both ``BTN_POPUP`` / ``REG_POPUP`` register under their
  ``with_id`` scopes and that ``imgui_click`` on the trigger button
  surfaces ``MAIN_WIN/via_button/BTN_POPUP/PICK_APPLE`` in the next
  snapshot.
- `examples/features/popup_window.das` — feature demo of the
  manual-trigger popup pattern with two scopes sharing the same
  ``"fruit_picker"`` str_id (button-click trigger + hover-region
  right-click trigger).
- `examples/tutorial/popup_window.das` + `doc/source/tutorials/popup_window.rst`
  — tutorial pairing with one-shell recorded
  ``doc/source/_static/tutorials/popup_window.apng`` driven by
  ``tests/integration/record_popup_window.das``.

### Changed

- `examples/imgui_demo/tables.das` — Section 21 "Context menus" sub 2
  per-column popup loop restored to full cpp shape (3 real columns +
  trailing-unused-space column), driven by the new ``popup_window``
  surface plus the existing
  ``IsMouseReleased(ImGuiMouseButton.Right) && !IsAnyItemHovered()``
  hover predicate. Replaces the simplified "track hovered column,
  no popup" fallback documented inline in the prior PR-D port.

### Fixed

- Two pre-existing lint hits surfaced while linting the modified files:
  ``examples/imgui_demo/tables.das:1114`` LINT010 dead-store of
  ``current`` (declare-then-unsafe-assign), and
  ``widgets/imgui_boost_runtime.das:1137`` PERF020 redundant
  ``string(...)`` cast on an already-string variant arm.



- `imgui/imgui_harness` — canonical wrapper module for examples/tests. Hides
  GLFW/OpenGL backend boilerplate behind five helpers (`harness_init`,
  `harness_begin_frame`, `harness_new_frame`, `harness_end_frame`,
  `harness_shutdown`) plus optional `harness_apply_synth_io`. Re-exports the
  backend-agnostic dasImgui v2 stack so example files need only a single
  `require imgui/imgui_harness` line.
- `imgui_app_headless.shared_module` — display-less C++ backend (3-fn
  surface: `imgui_headless_init_fonts`, `imgui_headless_set_display_size`,
  `imgui_headless_advance_dt`). No GLFW, no OpenGL, no gl3w. Used by
  `imgui_harness` when its `--headless` arm is active; the GLFW/GL symbol
  set is structurally absent from headless scope.
- `--headless` CLI flag in `imgui_harness` — pass after the daslang `--`
  separator (`daslang FILE.das -- --headless`) to run with no window and no
  GL context. CPU-only font atlas, real wall-clock `DeltaTime`, `Render()`
  output discarded. Synth IO (`imgui_mouse_play`, `imgui_key_chord`, …)
  works in both modes via the existing `imgui_*` `[live_command]` family.
- `--headless-frames=N` CLI flag — auto-exit after `N` frames. Useful for
  unattended CI smokes that have no out-of-band exit signal.
- New tutorial: `doc/source/tutorials/harness_headless_mode.rst` — walks
  through what `--headless` dispatches to, how to launch in either mode,
  and the limits (`screenshot` / `record_*` and live-API HTTP stay
  windowed-only).
- `imgui/imgui_harness_lint` — default-on `[lint_macro]` that fires on any
  file requiring `imgui/imgui_harness` and forbids direct calls into the
  windowed-backend modules (`glfw_boost`, `opengl_boost`, `glfw_live`,
  `opengl_live`, `imgui_live`). Diagnostic code `HARNESS001`
  (macro_error 50503). Bundled transitively via
  `require imgui/imgui_harness_lint public` from `imgui_harness.das`.
  Per-file opt-out: `options _allow_glfw_calls = true` (scaffolding only,
  target end-state = no opt-out — same shape as PR #33's
  `_allow_imgui_legacy`).
- `tests/integration/failed_imgui_harness_lint_raw.das` — negative fixture
  proving the default-on lint fires. Uses `expect 50503` so dastest treats
  the compile failure as a PASS.
- CI: `tests.yml` resurrected. Now runs on every push/PR (was parked
  `workflow_dispatch:` since the xvfb/Mesa hang). The integration suite
  drives daslang-live in headless mode — `imgui_playwright` reads
  `--headless` from its own `get_user_args()` and appends `-- --headless`
  to every spawned subprocess. No `xvfb-run`, no virtual display.
  Depends on daslang PR #2681 (daslang-live forwards post-`--` argv to
  scripts via `setCommandLineArguments`). Local smoke: 98/111 PASS
  headless; the 13 failures (and CI excludes) are
  `test_glfw_synth_keys`, `test_glfw_synth_mouse`,
  `test_visual_aids_key_hud` — all post `key_press` / `mouse_*` HTTP
  commands targeting `glfw_live`'s synth driver, which has no event
  queue without a GLFW context. Inherently windowed-only; revisit
  alongside a windowed CI job or a backend-neutral key-event API.
- CI: `docs.yml` no longer filters by `paths:` — any change to
  `master` rebuilds and republishes. The narrow `doc/**` + `widgets/**`
  filter silently skipped PRs that only touched `examples/**` or
  `CHANGELOG.md`, leaving the site stale vs master.

### Changed

- `examples/features/foundation.das` and `active_widget.das` ported to
  `imgui/imgui_harness` (PR 1+2; restated here for completeness).
- First migration batch (this PR) — 12 additional `examples/features/`
  files ported to the harness API. Each file dropped 14 requires + ~25
  lines of GLFW/GL boilerplate down to a single
  `require imgui/imgui_harness` + 5 helper calls. All verified working in
  both windowed (live mode) and headless (`--headless --headless-frames=60`)
  modes; integration suite: 111/111 PASS (was 110/110 before lint canary):
    - `narrative_text.das`, `narrative_bullet.das`, `narrative_separator.das`
    - `output_text.das`
    - `with_indent.das`, `with_item_width.das`, `with_text_wrap_pos.das`
    - `menu_main.das`, `menu_label_static.das`
    - `tooltip_flat.das`
    - `button_repeat.das`, `collapsing_header_closable.das`
    - `disabled_block.das`
- `imgui_harness.das` re-exports broadened to include all sibling
  `imgui_*_builtin` modules (`imgui_scope_builtin`, `imgui_id_builtin`,
  `imgui_style_builtin`, `imgui_layout_builtin`, `imgui_docking_builtin`,
  `imgui_colors`). Matches the design intent — one require should bring
  the full backend-agnostic dasImgui v2 surface into scope. Migration
  batch surfaced the gap (5 of 13 files needed a second require for
  `scope_builtin` before the harness was extended).
- **Second migration batch (this PR) — all 48 remaining `examples/features/`
  files ported to the harness API.** With the 15 already-migrated files
  (foundation, active_widget, and the 13 from PR 3), every file under
  `examples/features/` now uses `require imgui/imgui_harness`. Net
  diff: +345 / -1680 across 48 files (mechanical 14-require chain →
  single require, 25-line update/shutdown boilerplate → 5 helper calls).
  Three sub-patterns covered:
    - **Canonical (35 files)** — `await_quiescent`, `clip_rect`,
      `containers_layout/_overlay/_window`, `display_image/_progress`,
      `drag_drop`, `focus_widget`, `font_stack`, `hex_id_click`,
      `id_override`, `indexed_dynamic`, `inputs_choice/_color/_drag/`
      `_indexed/_numeric/_slider/_text`, `io_mirror`, `io_synth_drag/_text`,
      `layout_primitives`, `list_box`, `plot_getter/_widgets`,
      `popup_context_item`, `scroll_state`, `snapshot_unrendered`,
      `style_override`, `transport_demo`, `triggers`, `widget_init_defaults`,
      `widget_no_ident`.
    - **Synth-IO (7 files)** — `columns_demo`, `dock_basic`, `layout_helpers`,
      `glfw_synth_keys/_mouse`, `imgui_synth_keys/_mouse`. These call
      `harness_apply_synth_io()` between `harness_begin_frame()` and
      `harness_new_frame()` (the slot where `apply_synth_io_override()`
      used to live).
    - **edit_external (6 files)** — `edit_external_color/_combo/_scalars/`
      `_special/_toggles/_vectors`. Preserve the `require daslib/safe_addr`
      that the family uses for caller-owned pointer args.
  Verified per-file: `compile_check` + `lint` + `format_file` clean,
  headless smoke (60 frames) exit 0, windowed smoke (5 s timeout-killed)
  ran. Full dastest integration: 111/111 PASS (unchanged).

### Additional PRs shipped during v2.0 (one-line summaries)

The entries above are the in-depth narratives Boris kept while building toward
v2.0. The following merged PRs also shipped in the same window and aren't
otherwise captured in this file:

- **#38 — CI: resurrect headless tests workflow.** `tests.yml` runs on every push/PR via headless dastest; xvfb dependency dropped.
- **#39 — `daslang-theme` style preset.** dasImgui colors match daslang.io docs.
- **#40 — `imgui_demo`: columns + applog.** Columns demo + ExampleAppConsole-style log scene ported.
- **#41 — `imgui_demo`: port-and-recording pass.** Initial recording rail for every imgui_demo scene.
- **#42 — HDPI theme scaling.** `--imgui-content-scale` flag + automatic per-monitor DPI handling.
- **#43 — One-shell recording driver model.** `with_recording_app` spawns daslang-live in-process; replaces the two-shell pattern.
- **#44 — Recording cmdline + config fixes.** Driver scripts consume `-project_root` from their own argv and forward to the spawned host.
- **#45 — `imgui_demo` app-small scenes.** auto-resize, constrained-resize, simple-overlay, long-text, window-titles, layout, property-editor, fullscreen.
- **#46 — `imgui_demo` widgets sweep.** Widgets section fully ported.
- **#47 — `imgui_demo` inputs sweep.** Inputs/Sliders section fully ported.
- **#48 — Drawlist rail.** `with_window_drawlist` / `with_foreground_drawlist` / `with_background_drawlist` block-arg containers + 8 drawlist primitive helpers.
- **#49 — `imgui_demo` banner with daslang.io link.** Banner widget links to docs site.
- **#50 — `layout.das` raw-imgui sweep.** Plus `tree_node_open` primitive + 11 `ALLOWED_IMGUI` additions.
- **#51 — `imgui_demo` PR-C three apps.** Custom-rendering, Documents, Dockspace.
- **#52 — `imgui_demo` PR-D tables.** 24 sub-sections, ~2900 das lines.
- **#54 — `imgui_narrate` scoring cascade.** Overlay picks placement via scored cascade (above/below/left/right of `target.bbox`).
- **#55 — `imgui_demo` tier-A.** Selectables polish.
- **#56 — `imgui_demo` tier-B tabs.**
- **#57 — `imgui_demo` tier-B rest.** Tree-table demos.
- **#58 — `imgui_demo` tier-C status flags.**
- **#59 — `imgui_demo` color picker.** Full Color-edit/picker surface via `edit_color3` / `edit_color4` `[edit_widget]` macros.
- **#60 — `imgui_demo` data types.** Full numeric type matrix on `slider_*` / `input_*`.
- **#61 — `imgui_demo` phase-3 bundle.** Plots, Tooltips, Trees in one pass.
- **#62 — `imgui_demo` menubar.** `main_menu_bar` / `menu` / `menu_item` containers; header-bbox capture for `menu` and `tab_item`.
- **#63 — `imgui_demo` help/config/winopts.** About / style-editor / metrics / debug-log demos wrapped.
- **#64 — Int/enum unification sweep.** `enum E` accepts at `int` API surfaces without explicit cast where the macro emits `int(value)`.
- **#65 — Recording tutorials batch 1.** First tutorial RST pages with embedded recordings.
- **#66 — Features backfill phase 2.** 13 additional `examples/features/*.das` drivers + smokes.
- **#67 — Container tutorials phase 3.** Block-arg `popup_*` / `tab_bar` / `tab_item` containers polished; tutorial recording pass.
- **#68 — Macro emit qualify + fnptr getters.** `_::clone` / `_::finalize` qualification for cross-module generic dispatch; function-pointer accessor generators.
- **#69 — Widget meta `addr` / `typeinfo` pivot.** `[widget]` macros generate state via `typeinfo` instead of legacy `safe_addr` boilerplate.
- **#70 — Real-glyph logo + favicon.** Replaces SVG placeholder.
- **#72 — Widget tutorial: drag/slider.**
- **#73 — APNG → MP4 migration.** Tutorial videos shipped as H.264 MP4 (`-c:v libx264 -crf 23 -pix_fmt yuv420p`); APNG is gitignored.
- **#74 — Widget tutorial: input numeric/text.**
- **#75 — Widget tutorial: toggles/dropdown.**
- **#76 — Widget tutorial: color/buttons.**
- **#77 — Widget tutorial: final batch.**
- **#78 — Playwright Windows libhv throttle.** Client paces under the 16-POST-per-subprocess IOCP limit on `windows-latest`.
- **#79 — Universal recording-quality sweep.** Consistent fps + pacing constants across all `record_*.das` drivers; long captures bumped to `fps=20`.
- **#80 — Parallel integration-test worker port indexing.** Workers share `DEFAULT_LIVE_PORT + worker_index` so `--isolated-mode-threads N` runs don't collide.
- **#81 — Integration tests PR-1 (high priority).** `list_clipper`, `input_text_callback`, `text_filter`, `data_table`, `table_set_bg_color`.
- **#82 — Integration tests PR-2 (med+low).** `edit_collapsing_header`, `popup_context_window`, `tree_node_open_manual`, `log_to_text`, `align_text_to_frame_padding`, `narrate_placement`, `imgui_synth_keys`, `imgui_synth_mouse`.
- **#83 — Coroutine pump + demos + favicon.** `harness` auto-pumps `advance_coroutines()` per frame so playwright `click`/`right_click`/`type_text`/`drag` drain reliably; per-tutorial demo fixes.
- **#84 — 15-API backfill.** 7 new feature demos + 7 integration tests covering `set_window_size`, `set_keyboard_focus_here`/`set_item_default_focus`, `set_scroll_here_y`, `table_header`/`table_angled_headers_row`, `edit_tab_item`, `open_popup_on_item_click`, and 7 visual-aid live commands.
- **#85 — Full re-record sweep after #83.** All 47 tutorial MP4s regenerated to reflect the post-PR-#83 runtime (macro fix layout corrections, coroutine pump fix).

[Unreleased]: https://github.com/borisbat/dasImgui/compare/v2.0...HEAD
[2.0]: https://github.com/borisbat/dasImgui/releases/tag/v2.0
