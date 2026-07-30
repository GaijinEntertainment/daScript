# jobque-timeline

Per-lane jobque trace viewer — the in-repo successor to the hand-spliced lane-timeline
browser artifact. Opens `jobque_trace_save` / `daslib/jobque_profile` JSON files (perfetto-
compatible) in dockable ImGui windows: one row per lane, events colored by category with
per-stage shading, unit markers (`token` / `layer` / `step`), computed stats (idle %,
parallelism, busy by category), multi-file compare with shared zoom/timer, and a launch
rail that reruns your trace-producing app and auto-refreshes on save.

## Run

The viewer needs dasImgui, which ships in-tree (`modules/dasImgui`) — nothing to install.

```
bin/Release/daslang.exe -project_root utils/jobque-timeline utils/jobque-timeline/main.das -- a.trace.json b.trace.json
```

Or live (hot reload + remote commands):

```
daslang-live -project_root utils/jobque-timeline utils/jobque-timeline/main.das
```

## Interaction

- wheel = zoom at cursor, drag = pan, double-click = full span, drag the overview strip = scrub
- shift+drag = time-range selection (stats line appears above the canvas; Esc clears)
- hover = event tooltip (kind, category, stage, chunk, chain, t, dur)
- Controls panel: share zoom / share timer (on by default), auto refresh, per-file reload,
  and the launch rail (browse for an app, args, launch, exit code + output tail)

## Live commands (`daslang-live`, port 9090)

`timeline_files`, `timeline_open {path}`, `timeline_get_view {file}`,
`timeline_set_view {file,t0,t1}`, `timeline_focus_unit {file,kind,index}`,
`timeline_get_selection {file}` / `timeline_set_selection {file,t0,t1}`,
`timeline_range_stats {file,t0,t1}` (pure query), `timeline_describe`,
`timeline_launch {app,args}`, `timeline_launch_status`, `timeline_refresh {file}`,
`timeline_set_autorefresh {on}`.

All times are µs, file-relative. `timeline_range_stats` and `timeline_describe` are the
screenshot-free rail: selections and views come back as structured JSON (events, busy µs
by category, idle %, parallelism, publish count).

## Files

`main.das` (app + dock layout + live commands), `tl_model.das` (trace model), `tl_loader.das`
(fast rigid-format parser), `tl_lod.das` (per-lane bucket mipmaps), `tl_view.das` (canvas +
cached draw runs), `tl_stats.das` (range statistics), `tl_launch.das` (subprocess launch +
file watch). Tests are co-located (`test_tl_*.das`):

```
bin/Release/daslang.exe dastest/dastest.das -- --test utils/jobque-timeline
```
