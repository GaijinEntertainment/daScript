---
slug: in-dasimgui-phase-1-what-s-the-difference-between-widget-exists-snap-ident-and-widget-rendered-snap-ident-when-do-i-use-which
title: In dasImgui Phase 1+, what's the difference between `widget_exists(snap, ident)` and `widget_rendered(snap, ident)` — when do I use which?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**Phase 1 split** (semantic shift from Phase 0b):

- `widget_exists(snap, ident)` — registered in `g_widgets` (long-lived, populated at module `[init]` via `register_widget`). True from module load onward, regardless of paint state. Use for "does the source declare this widget?" / "wait until subprocess startup has finished compiling".

- `widget_rendered(snap, ident)` — actually painted this frame (per-frame `g_registry` populated by `widget_finalize`). Use for "is this widget currently visible to the user?" / "did open/close just take effect?".

**Snapshot output telegraphs the distinction:**
- Rendered widget: `{kind, hex_id, bbox, hover?, active?, focus?, payload}` (no `rendered` key — bbox is the positive signal).
- Unrendered widget: `{kind, rendered: false, payload}` (no bbox).

`widget_rendered` checks for `rendered:false` field absence (or `bbox` presence). DON'T use `?["bbox"] != null` — the daslib/json `?[]` quirk returns JV(null) for missing keys, always non-null.

**Phase 0b regressions** (test_containers_window/overlay/layout) used `widget_exists` to mean "currently painting" — Phase 1 broke them. Fix is mechanical: replace with `widget_rendered` where the assertion is about paint state. Same goes for `wait_for_widget` (registered) → `wait_for_render` (painting). Both helpers exist in `live_driver.das`.

**Path-key migration tradeoff** (1.1.2): `[init]` registers under bare ident; first render's `widgets_mark_seen` migrates to container-aware path_key. So `widget_exists("MAIN_WIN/RPS")` is false until first render of MAIN_WIN/RPS — until then the entry lives under `widget_exists("RPS")` (bare). For unconditional pre-render visibility, you'd need to register under full path at macro time (deferred — needs lexical container chain resolution).

## Questions
- In dasImgui Phase 1+, what's the difference between `widget_exists(snap, ident)` and `widget_rendered(snap, ident)` — when do I use which?
