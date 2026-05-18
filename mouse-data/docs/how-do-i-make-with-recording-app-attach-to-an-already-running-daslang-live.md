---
slug: how-do-i-make-with-recording-app-attach-to-an-already-running-daslang-live
title: How do I make a recording driver attach to an already-running daslang-live host instead of spawning a new one?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

Verified 2026-05-17 (dasImgui PR #44, master post-merge).

`with_recording_app` (in `widgets/imgui_playwright.das`) probes
`GET /status` at the live-API port before spawning. If a host is
already serving, it skips both the `popen_argv` spawn AND the
`/shutdown` signal at the end — runs `record_start` / body /
`record_stop` directly against the running host.

**Use case:** dev keeps `daslang-live` open while iterating on a
tutorial (live-reload edits visible immediately), then triggers the
recorder without restarting the host. Before PR #44, spawning would
collide on port 9090, recorder panicked, and you had to kill the
host first.

**The probe pattern (lines ~656-672 of imgui_playwright.das after PR #44):**

```daslang
let ATTACH_PROBE_SEC : float = 0.3f   // module-level tunable

// ... feature_path / output_apng_path / app setup ...

if (wait_until_ready(app, ATTACH_PROBE_SEC)) {
    print("[record] attaching to existing daslang-live at {base_url}\n")
    post_command(app, "imgui_mouse_trail",   JV((enabled = true)))
    post_command(app, "imgui_cursor_sprite", JV((enabled = true)))
    post_command(app, "record_start", JV((file = output_apng_path,
                                          fps = fps,
                                          max_seconds = max_seconds)))
    invoke(body, app)
    let stopped = post_command(app, "record_stop", null)
    // ... cleanup mouse_trail + cursor_sprite ...
    return    // DO NOT post /shutdown
}

// ... fall through to existing spawn flow ...
```

`wait_until_ready(app, 0.3f)` = three `/status` polls at the existing
100 ms cadence. Instant when something's listening; fast TCP-refused
when not. 0.3 s feels imperceptible to the user but is enough to
detect any healthy host.

**Contract differences spawn-vs-attach:**

| Aspect | Spawn | Attach |
|---|---|---|
| Process lifecycle | helper owns | caller owns |
| `/shutdown` at end | yes | NO — host kept alive |
| Test-timeout watchdog | `popen_argv(test_timeout)` | none (body must self-terminate) |
| HDPI/style | logical 1x (`--imgui-content-scale=1.0` + `--no-hdpi-framebuffer`) | host's current setting |
| Feature loaded | helper passes `feature_path` to spawned host | caller owns whatever's loaded |

**Caller responsibility in attach mode:** the helper does NOT post a
switch-feature command. If the running host has a different feature
loaded than what the driver expects, the body's
`wait_for_render(app, T_WIDGET, 10.0f)` will time out and panic with
the standard `"{T_WIDGET} never rendered — wrong app running?"`
message. That's the existing fail-loud path.

**Reproducing for the dev workflow:**

```
# Shell 1 — start the host
daslang-live -project_root <dasimgui> <dasimgui>/examples/tutorial/boost_basics.das

# Shell 2 — record (the driver scans 127.0.0.1:9090, sees the host, attaches)
daslang -project_root <dasimgui> <dasimgui>/tests/integration/record_boost_basics.das
# [record] attaching to existing daslang-live at http://127.0.0.1:9090
# ... APNG written, host stays alive ...
```

**Gotcha:** attached recordings stay at native HDPI on retina/scaled
Windows. The logical-1x mode is spawn-only because that's where
`--imgui-content-scale=1.0` + `--no-hdpi-framebuffer` are passed. For
canonical APNG regen (consistent dimensions across machines), use the
no-host-running invocation so the helper spawns at logical 1x. See
`how-do-i-disable-hdpi-framebuffer-scaling-in-daslang-live-for-recording`.

## Questions
- How do I make a recording driver attach to an already-running daslang-live host instead of spawning a new one?
