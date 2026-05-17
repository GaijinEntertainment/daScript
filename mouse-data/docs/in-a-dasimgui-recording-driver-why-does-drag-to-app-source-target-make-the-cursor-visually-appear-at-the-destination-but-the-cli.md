---
slug: in-a-dasimgui-recording-driver-why-does-drag-to-app-source-target-make-the-cursor-visually-appear-at-the-destination-but-the-cli
title: In a dasImgui recording driver, why does drag_to(app, source, target) make the cursor visually appear at the destination but the click never seems "held" (no drag tooltip, drag_drop target never accepts payload, slider doesn't move)?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

**`drag_to` dispatches the L1 `imgui_drag` coroutine, which calls `AddMouseButtonEvent` / `AddMousePosEvent` DIRECTLY on `io` — bypassing the synth pipeline. That races against `imgui_synth_tick`'s per-frame re-assertion of cursor pos + held-buttons.**

`widgets/imgui_boost_runtime.das:1393-1406` `drag_coro`:

    [async]
    def private drag_coro(start, endp, steps, button) {
        var io & = unsafe(GetIO())
        io |> AddMousePosEvent(start.x, start.y)
        io |> AddMouseButtonEvent(button, true)
        await_next_frame()
        for (i in range(1, steps + 1)) {
            let p = start + (endp - start) * t
            io |> AddMousePosEvent(p.x, p.y)
            await_next_frame()
        }
        io |> AddMouseButtonEvent(button, false)
    }

Meanwhile every frame `apply_synth_io_override()` (live) or `harness_apply_synth_io()` (harness) calls `imgui_synth_tick`, which:

1. Re-asserts `synth_cursor_x, synth_cursor_y` via `AddMousePosEvent` — overrides whatever `drag_coro` just emitted with the LAST `move_to`'s target
2. Re-asserts `synth_held_buttons` via `AddMouseButtonEvent` — but `drag_coro`'s press never lands in `synth_held_buttons` (different code path), so it isn't re-asserted

End result: ImGui sees the cursor at the static `move_to` target (not the interpolated drag positions), the button-down event arrives once but the held state isn't re-asserted, so ImGui's internal drag-threshold logic never trips. Cursor sprite ends up at the target, but drag never visually "engaged."

## Fix: use `drag_along` + `imgui_mouse_play`

`drag_along` builds an events array; `imgui_mouse_play` queues them via the synth pipeline (`synth_held_buttons` + `synth_cursor`), which IS the drained path:

    var drag_events : array<JsonValue?>
    drag_events |> drag_along(0, p_source, p_target, 1200, 400)
    post_command(app, "imgui_mouse_play", JV((events = drag_events)))
    sleep(uint(2200))   //! approach(400) + 100 + drag(1200) + 200 + 300

Verified 2026-05-17 on `examples/features/drag_drop.das` recording: drag_to → `Drops accepted: 0`. drag_along → drag tooltip "Dragging int: 42" follows cursor, TARGET highlighted on hover, `Drops accepted: 1`.

## Why does `drag_to` exist at all?

It works in non-synth contexts (running daslang-live without `apply_synth_io_override`, no synth IO conflicts) and for tests that don't care about the visual drag. The L1 coroutine ships with `imgui_boost_runtime`; the L2 playwright helper (`drag_along` + `imgui_mouse_play`) is the synth-pipeline-integrated path.

**Rule of thumb:** if your recording driver is making an APNG, use `drag_along + imgui_mouse_play`. If it's a fast integration smoke that just needs the drag to fire telemetry-wise, `drag_to` is fine.

## Related

- `tests/integration/record_drag_drop.das` — uses the `drag_along + imgui_mouse_play` form post-fix.
- `widgets/imgui_playwright.das:92-112` — `drag_along` signature.
- `widgets/imgui_live_core.das:702-720` — `imgui_synth_tick` re-assertion logic.
- mouse card `feedback_synth_io_override_pattern` — broader synth IO discipline.

## Questions
- In a dasImgui recording driver, why does drag_to(app, source, target) make the cursor visually appear at the destination but the click never seems "held" (no drag tooltip, drag_drop target never accepts payload, slider doesn't move)?
