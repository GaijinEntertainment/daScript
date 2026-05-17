---
slug: how-do-i-verify-a-dasimgui-apng-recording-is-correct-without-playing-it-in-a-viewer-screenshot-live-process-extract-individual-f
title: how do I verify a dasImgui APNG recording is correct without playing it in a viewer — screenshot live process + extract individual frames
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

# Two ground-truth probes for dasImgui recordings

When iterating on a `record_*.das` driver and the resulting APNG looks
wrong (no cursor, wrong menu state, mistimed narrate), don't rely on
Boris's eyeball — instrument it:

## Probe 1: live-process screenshot (state at *this moment*)

While daslang-live is running, post arbitrary synth events via MCP,
then capture the framebuffer to a PNG you can read in Claude Code:

    mcp__daslang__live_command name="imgui_mouse_play" args='{"events":[
        {"t_ms":0,"kind":"move","x":50,"y":15},
        {"t_ms":1000,"kind":"move","x":300,"y":300}]}'
    mcp__daslang__live_command name="imgui_mouse_status"    # cursor_owned?
    mcp__daslang__live_command name="screenshot" args='{"file":"/path/diag.png"}'

Then `Read` the PNG. This shows what daslang-live is rendering RIGHT NOW
— cursor sprite included, foreground draw list included, narrate
callouts included. If the screenshot shows the cursor but the APNG
doesn't, the bug is in the recording path, not the render path. If
neither shows it, the synth IO isn't draining (see related card on
harness_apply_synth_io).

Useful for: confirming menu opens on click, finding empirical pixel
coords for unregistered widgets, verifying narrate visibility.

## Probe 2: extract individual frames from the APNG with ffmpeg

APNG inspection without a viewer:

    ffmpeg -i scene.apng -vf "select=eq(n\,80)" -frames:v 1 \
           -update 1 frame80.png -y

Notes:
- `-update 1` is REQUIRED for single-frame output; otherwise ffmpeg
  expects a `%d`-style pattern and errors out.
- `select=eq(n\,N)` picks frame N (0-based). Comma needs the backslash.
- Picking N values spread across the recording (e.g. 60, 200, 400,
  600, 800, 950 for a 1000-frame recording) gives you a fast story-
  arc check without watching the whole thing.

Then `Read` each PNG to verify each stage looks right.

## Workflow

1. Iterate driver `record_*.das` → run against live host.
2. After each record_stop, extract 5-6 frames spanning the timeline.
3. Read frames to verify: cursor visible? trail visible? menu open
   when narrated? narrate text matches visual?
4. If anything's off, kill daslang-live (NEVER reuse a dirty session
   — earlier interactive probes contaminate menu state), restart
   fresh, re-record.

## Reader pacing rule

Recordings for tutorials need TIME to be read. Per stage:

    let NARRATE_FRAMES = 240   // 4.0s visible at 60 fps app
    let READ_MS        = 5000u // 5s read + 1s gap before next action
    let SETTLE_MS      = 1500u // 1.5s cursor settle before next narrate
    let RESULT_MS      = 2000u // 2s action result dwell

Set `record_start max_seconds` ≥ sum of (settle + read + result) per
stage + slack. Recordings that fit in 30s are usually too dense.

## Related

- `mcp__daslang__live_command help` — list all live commands incl.
  `screenshot`, `imgui_mouse_status`, `imgui_snapshot`.
- `tests/integration/record_with_id.das` — canonical recording template
  (uses `live_*` lifecycle, not `harness_*`).
- `tests/integration/record_imgui_demo_about.das` /
  `record_imgui_demo_app_main_menu.das` — drivers using the pacing
  constants above against `harness_*.das` hosts.

## Questions
- how do I verify a dasImgui APNG recording is correct without playing it in a viewer — screenshot live process + extract individual frames
