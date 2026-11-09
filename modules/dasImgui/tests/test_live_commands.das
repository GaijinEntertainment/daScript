options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require imgui/imgui_transport public
require daslib/json public
require daslib/json_boost public

//! The live-command surface a driver talks to: the imgui_* commands,
//! the Transport lambda, user control, await, and the recording-sync
//! primitives.

[test(description = "the imgui_* live commands click a button, toggle a checkbox and steer a radio group, and the snapshot entries carry kind, payload, hex_id and bbox", timeout = 1200)]
def test_triggers_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/triggers.das") $(d) {
        // Gate on a known widget showing up — proves the GUI rendered a
        // frame, no need for sleep().
        var snap0 = wait_for_widget(d, "MAIN_WIN/SAVE_BTN", 15.0f)
        t |> success(snap0 != null, "SAVE_BTN registered after first frame")
        if (snap0 == null) return

        // ----- Snapshot shape contract -----
        var save0 = find_widget(snap0, "MAIN_WIN/SAVE_BTN")
        t |> equal(save0?["kind"] ?? "", "button", "SAVE_BTN.kind == 'button'")
        t |> success(save0?["payload"] != null, "SAVE_BTN.payload present")

        // ----- Click family — count++ on imgui_click -----
        let count_before = save0?["payload"]?["click_count"] ?? 0
        click(d, "MAIN_WIN/SAVE_BTN")
        t |> success(wait_for_int_value(d, "MAIN_WIN/SAVE_BTN", "click_count", count_before + 1, 300),
                     "click bumps SAVE_BTN.click_count")

        // ----- Toggle family — imgui_force_set flips the bool -----
        let wf_before = widget_payload_field(snap0, "MAIN_WIN/WIREFRAME", "value") ?? false
        force_set_value(d, "MAIN_WIN/WIREFRAME", JV(!wf_before))
        t |> success(wait_for_bool_value(d, "MAIN_WIN/WIREFRAME", "value", !wf_before, 300),
                     "imgui_force_set toggles WIREFRAME.value")

        // ----- Grouped int radio — imgui_force_set steers the selection -----
        force_set_value(d, "MAIN_WIN/DIFFICULTY", JV(2))
        t |> success(wait_for_int_value(d, "MAIN_WIN/DIFFICULTY", "value", 2, 300),
                     "imgui_force_set picks DIFFICULTY=2 (Hard)")

        // ----- Snapshot envelope — required common fields per widget -----
        // QUICK_SAVE.bbox is per-frame; gate on the menu_item being painted so the snapshot carries
        // its geometry, not a bare snapshot from a frame before the menu bar painted.
        var snap_final = wait_for_visible(d, "MAIN_WIN/QUICK_SAVE", 5.0f)
        t |> success(snap_final != null, "QUICK_SAVE painted")
        var menu = find_widget(snap_final, "MAIN_WIN/QUICK_SAVE")
        t |> equal(menu?["kind"] ?? "", "menu_item", "QUICK_SAVE.kind == 'menu_item'")
        t |> success(menu?["hex_id"] != null, "QUICK_SAVE.hex_id present")
        t |> success(menu?["bbox"] != null, "QUICK_SAVE.bbox present")
    }
}

[test(description = "a Transport lambda bound to the live endpoint fetches a snapshot with globals, dispatches a click with a Result-shaped reply, and awaits quiescence", timeout = 1200)]
def test_transport_lambda_round_trip(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/transport_demo.das") $(d) {
        // Wait for the widget so we know the host is fully ready before
        // routing the smoke commands through the transport lambda.
        let snap0 = wait_for_widget(d, "MAIN_WIN/DEMO_BTN", 15.0f)
        t |> success(snap0 != null, "DEMO_BTN reachable via imgui_playwright")
        if (snap0 == null) return

        // Gate 1+2: build transport, fetch a snapshot through the lambda.
        let transport <- live_api_transport(d.base_url)
        var snap = receive_imgui_snapshot(transport)
        t |> success(snap != null, "transport receive_imgui_snapshot non-null")
        t |> success(snap?["globals"] != null, "snapshot has globals")

        // Gate 3: send a click via the transport; verify Result wire shape.
        var click_resp = send_imgui_command(transport, "imgui_click",
                                            JV((target = "MAIN_WIN/DEMO_BTN")))
        t |> success(click_resp != null, "click via transport non-null")
        t |> equal(click_resp?["ok"] ?? false, true,
                   "transport click response.ok = true")
        t |> equal(click_resp?["value"] ?? "", "MAIN_WIN/DEMO_BTN",
                   "transport click response.value = DEMO_BTN")

        // Gate 4: await_imgui converges on quiescence within the default
        // 5s timeout (no L1 coroutines in flight, no active item).
        t |> success(await_imgui(transport, "quiescent", 5.0f),
                     "await_imgui returns true when idle")
    }
}

//! Real-input blocking needs a physical mouse and is checked by hand;
//! headless, the source-detach is a no-op that runs its null guard.
[test(description = "set_user_control round-trips off and on repeatedly while frames keep rendering, and the UI still answers a click afterward", timeout = 1200)]
def test_user_control_toggle(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/active_widget.das") $(d) {
        t |> success(wait_for_widget(d, "MAIN_WIN/DEMO_BTN", 15.0f) != null,
                     "app rendered (ImGui context up)")

        // Disable: subsequent ticks detach the GLFW input callbacks at the source.
        let r_off = post_command(d, "set_user_control", JV((enabled = false)))
        t |> equal(r_off?["user_control"] ?? true, false, "control disabled")

        // Frames keep advancing cleanly while disabled (the per-frame detach +
        // ClearEventsQueue path runs without crashing or wedging).
        t |> success(wait_for_render(d, "MAIN_WIN/DEMO_BTN", 5.0f) != null,
                     "still rendering while control disabled")

        // Re-enable: subsequent ticks reattach the callbacks.
        let r_on = post_command(d, "set_user_control", JV((enabled = true)))
        t |> equal(r_on?["user_control"] ?? false, true, "control re-enabled")

        // Repeated toggle must remain stable (no double install/restore wedge)...
        post_command(d, "set_user_control", JV((enabled = false)))
        let r_on2 = post_command(d, "set_user_control", JV((enabled = true)))
        t |> equal(r_on2?["user_control"] ?? false, true, "stable after repeated toggles")

        // ...and the UI is still responsive afterward (L3 click round-trips).
        let click_after = click(d, "MAIN_WIN/DEMO_BTN")
        t |> equal(click_after?["ok"] ?? false, true, "UI responsive after toggles")
    }
}

let TARGET = "CBH_WIN/CBH_RED"

[test(description = "wait_for_hover sees hover arrive and clear without false-passing on a missing widget, and record_check returns its ok argument without panicking on false", timeout = 1200)]
def test_recording_sync(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/color_button_hover.das") $(d) {
        var snap = wait_for_widget(d, TARGET, 15.0f)
        t |> success(snap != null, "{TARGET} renders")
        if (snap == null) return

        // ---- wait_for_hover ----
        // Move onto the swatch — it must report hovered (the move -> wait_for_hover -> click gate).
        move_to(d, widget_click_point(snap, TARGET))
        t |> success(wait_for_hover(d, TARGET) != null, "wait_for_hover registers hover after move_to")

        // Move off — hover must clear (expected=false on a PRESENT widget).
        move_to(d, (4.0f, 4.0f))
        t |> success(wait_for_hover(d, TARGET, false) != null, "wait_for_hover sees hover clear after moving away")

        // Regression for the #186-A false-pass: a missing/misspelled ident must NOT pass as "not
        // hovered". The old predicate coalesced null hover to false and returned on the first poll;
        // the widget_exists guard makes it time out instead (tiny timeout still separates the two).
        t |> success(wait_for_hover(d, "CBH_WIN/__no_such_widget__", false, 0.3f) == null,
                     "missing widget does not false-pass wait_for_hover(expected=false)")

        // ---- record_check ----
        // Returns its `ok` argument and accumulates (does NOT panic) on false; reaching the line
        // after the false call proves it didn't panic mid-body.
        t |> success(record_check(d, "ok branch returns true", true), "record_check returns true when ok")
        let failed = record_check(d, "intentional miss (accumulated, not panicked)", false)
        t |> success(!failed, "record_check returns false and accumulates instead of panicking")
    }
}

[test(description = "imgui_await reports quiescent while idle and busy while a paused click coroutine is pending, recovers quiescence, and echoes a per-command await_frames modifier", timeout = 1200)]
def test_imgui_await(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/await_quiescent.das") $(d) {
        // Gate 1: idle quiescence.
        let snap0 = wait_for_widget(d, "MAIN_WIN/DEMO_SLIDER", 15.0f)
        t |> success(snap0 != null, "DEMO_SLIDER reachable")
        if (snap0 == null) return
        let idle = await_probe(d)
        t |> equal(idle?["ok"] ?? false, true,
                   "await response.ok = true")
        t |> equal(idle?["value"]?["quiescent"] ?? false, true,
                   "idle: quiescent = true")
        t |> equal(idle?["value"]?["pending_coroutines"] ?? -1, 0,
                   "idle: pending_coroutines = 0")

        // Gate 2: click → coroutine in flight; await says not quiescent. Headless runs multi-kHz
        // (no vsync), so the click coroutine finishes in microseconds; with_paused freezes update()
        // (hence advance_coroutines) so the spawned coroutine sits in g_coroutines until observed.
        with_paused(d) {
            let dr = click(d, "MAIN_WIN/DEMO_SLIDER")
            t |> equal(dr?["ok"] ?? false, true, "click dispatched")
            let busy = await_probe(d)
            t |> success((busy?["value"]?["pending_coroutines"] ?? 0) >= 1 ||
                         !(busy?["value"]?["quiescent"] ?? true),
                         "during click: not quiescent OR pending_coroutines>=1")
        }

        // Gate 3: poll until quiescent.
        let settled = wait_until(d, 300) $(_snap) {
            let probe = await_probe(d)
            return probe?["value"]?["quiescent"] ?? false
        }
        t |> success(settled != null, "post-drag: quiescent recovers")

        // Gate 4: per-command await modifier surfaces in response wrap.
        let resp = post_command(d, "imgui_click", JV((
            target = "MAIN_WIN/DEMO_SLIDER",
            await = JV((await_frames = 7))
        )))
        t |> equal(resp?["await_frames"] ?? -1, 7,
                   "with_await merges await_frames=7 from per-command modifier")
    }
}
