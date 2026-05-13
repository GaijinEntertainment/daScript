options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public
require math

//! GLFW-side synthetic mouse driver — end-to-end gates.
//!
//! The synth driver in `modules/dasGlfw/dasglfw/glfw_live.das` posts events
//! through dasGLFW's C++ chain dispatcher (no `glfwSetCursorPos`, so the
//! user's OS cursor isn't yanked around). The chain delivers events to
//! whatever previously-installed callback existed (e.g. ImGui's
//! `ImGui_ImplGlfw_*Callback`), so ImGui sees the synth events.
//!
//! Tests verify state via `mouse_status` (daslang's authoritative cursor
//! state) and via `io.mouse_buttons` (ImGui's event-driven button state,
//! which the per-frame ImGui_ImplGlfw polling doesn't override). Position
//! tests via `io.mouse_pos` are intentionally absent: on focused windows
//! `ImGui_ImplGlfw_UpdateMouseData` re-polls `glfwGetCursorPos` every frame
//! and overrides the synth pos in io.MousePos. Overlay rendering
//! (mouse_trail, cursor_sprite) reads `get_synth_cursor()` directly from
//! glfw_live to dodge that race — see `imgui_visual_aids.effective_cursor_pos`.

let FEATURE = "modules/dasImgui/examples/features/glfw_synth_mouse.das"

[test]
def test_mouse_pos_lands_in_status(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap0 = wait_for_render(d, "SYNTH_WIN", 15.0f)
        t |> success(snap0 != null, "SYNTH_WIN renders")
        if (snap0 == null) return

        let resp = post_command(d, "mouse_pos", JV((x = 230.0f, y = 150.0f)))
        t |> success(resp != null, "mouse_pos returned a response")
        // mouse_status reflects the daslang-side cursor immediately.
        let s = post_command(d, "mouse_status", null)
        let cx = s?["cursor_x"] ?? -1.0f
        let cy = s?["cursor_y"] ?? -1.0f
        t |> equal(cx, 230.0f, "mouse_status.cursor_x = 230 after mouse_pos")
        t |> equal(cy, 150.0f, "mouse_status.cursor_y = 150 after mouse_pos")
    }
}

[test]
def test_mouse_buttons_io_mirror(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap0 = wait_for_render(d, "SYNTH_WIN", 15.0f)
        t |> success(snap0 != null, "SYNTH_WIN renders")
        if (snap0 == null) return

        // Chain delivers AddMouseButtonEvent; ImGui's button state is
        // event-driven (no polling override), so io.mouse_buttons[0] sticks
        // until release.
        post_command(d, "mouse_click", JV((button = 0, action = "press")))
        let pressed = wait_until(d, 180) $(var snap) {
            return snap?["io"]?["mouse_buttons"]?[0] ?? false
        }
        t |> success(pressed != null, "io.mouse_buttons[0] == true after press")

        post_command(d, "mouse_click", JV((button = 0, action = "release")))
        let released = wait_until(d, 180) $(var snap) {
            let b = snap?["io"]?["mouse_buttons"]?[0] ?? true
            return !b
        }
        t |> success(released != null, "io.mouse_buttons[0] == false after release")
    }
}

[test]
def test_mouse_move_to_animates(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap0 = wait_for_render(d, "SYNTH_WIN", 15.0f)
        if (snap0 == null) {
            t |> success(false, "SYNTH_WIN renders")
            return
        }

        // Park at (50, 50), then sweep to (350, 250) over 400ms.
        post_command(d, "mouse_pos", JV((x = 50.0f, y = 50.0f)))
        post_command(d, "mouse_move_to", JV((x = 350.0f, y = 250.0f, duration_ms = 400)))

        // Mid-flight: cursor_x somewhere between start and end (via mouse_status).
        // Wide window — under suite load a single slow frame can advance the
        // timeline a lot, but anywhere strictly between start and end proves
        // the tick is interpolating rather than snapping straight to the end.
        let midflight = wait_until(d, 180) $(var snap) {
            let s = post_command(d, "mouse_status", null)
            let mx = s?["cursor_x"] ?? -1.0f
            return mx > 55.0f && mx < 345.0f
        }
        t |> success(midflight != null, "cursor passes through mid-flight x")

        // Wait until playback completes and cursor lands on the endpoint.
        let landed = wait_until(d, 300) $(var snap) {
            let s = post_command(d, "mouse_status", null)
            let playing = s?["playing"] ?? true
            let mx = s?["cursor_x"] ?? -1.0f
            let my = s?["cursor_y"] ?? -1.0f
            return !playing && abs(mx - 350.0f) < 1.0f && abs(my - 250.0f) < 1.0f
        }
        t |> success(landed != null, "cursor lands on (350,250) per mouse_status")
    }
}

[test]
def test_mouse_play_event_list_completes(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap0 = wait_for_render(d, "SYNTH_WIN", 15.0f)
        if (snap0 == null) {
            t |> success(false, "SYNTH_WIN renders")
            return
        }

        // Hand-built timeline: park, sweep, scroll, click.
        var events <- [
            JV((t_ms = 0,   kind = "move",   x = 30.0f,  y = 30.0f)),
            JV((t_ms = 200, kind = "move",   x = 300.0f, y = 200.0f)),
            JV((t_ms = 220, kind = "scroll", x = 0.0f,   y = 1.0f)),
            JV((t_ms = 250, kind = "button", button = 0, action = "press")),
            JV((t_ms = 300, kind = "button", button = 0, action = "release"))
        ]
        post_command(d, "mouse_play", JV((events = events)))

        // Convergence: playing flips to false and cursor lands on the last move waypoint.
        let done = wait_until(d, 300) $(var snap) {
            let s = post_command(d, "mouse_status", null)
            let playing = s?["playing"] ?? true
            let mx = s?["cursor_x"] ?? -1.0f
            let my = s?["cursor_y"] ?? -1.0f
            return !playing && abs(mx - 300.0f) < 1.0f && abs(my - 200.0f) < 1.0f
        }
        t |> success(done != null, "mouse_play timeline runs to completion + lands on last move")
    }
}

[test]
def test_mouse_stop_aborts(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap0 = wait_for_render(d, "SYNTH_WIN", 15.0f)
        if (snap0 == null) {
            t |> success(false, "SYNTH_WIN renders")
            return
        }

        // Start a long-duration move, then stop it mid-flight.
        post_command(d, "mouse_pos", JV((x = 50.0f, y = 50.0f)))
        post_command(d, "mouse_move_to", JV((x = 400.0f, y = 300.0f, duration_ms = 5000)))
        // After kickoff, mouse_status reports playing=true.
        let started = wait_until(d, 60) $(var snap) {
            let s = post_command(d, "mouse_status", null)
            return s?["playing"] ?? false
        }
        t |> success(started != null, "mouse_move_to enters playing state")

        let stopped = post_command(d, "mouse_stop", null)
        t |> equal(stopped?["stopped"] ?? false, true, "mouse_stop reports stopped=true")

        let halted = post_command(d, "mouse_status", null)
        t |> equal(halted?["playing"] ?? true, false, "mouse_status.playing == false after stop")
    }
}

[test]
def test_synth_cursor_ownership_sticky(t : T?) {
    // Cursor sprite disappearing mid-recording: get_synth_cursor previously
    // returned (playing, x, y) — after a mouse_play timeline drained, playing
    // flipped false, effective_cursor_pos fell back to GetMousePos(), and the
    // sprite jumped to the real OS cursor (off-screen if the user is in
    // another window). Fix is sticky ownership: any synth event flips
    // cursor_owned true; mouse_status now exposes it.
    with_imgui_app(FEATURE) $(d) {
        var snap0 = wait_for_render(d, "SYNTH_WIN", 15.0f)
        if (snap0 == null) {
            t |> success(false, "SYNTH_WIN renders")
            return
        }
        let s0 = post_command(d, "mouse_status", null)
        t |> equal(s0?["cursor_owned"] ?? true, false, "cursor_owned starts false")
        // Single synth event flips ownership.
        post_command(d, "mouse_pos", JV((x = 100.0f, y = 100.0f)))
        let s1 = post_command(d, "mouse_status", null)
        t |> equal(s1?["cursor_owned"] ?? false, true,  "cursor_owned flips true after mouse_pos")
        t |> equal(s1?["playing"]      ?? true,  false, "mouse_status.playing is false (mouse_pos is one-shot)")
        // Run a short timeline, wait for drain, ownership stays.
        var events <- [
            JV((t_ms = 0,  kind = "move", x = 50.0f,  y = 50.0f)),
            JV((t_ms = 80, kind = "move", x = 200.0f, y = 200.0f))
        ]
        post_command(d, "mouse_play", JV((events = events)))
        let drained = wait_until(d, 240) $(var snap) {
            let s = post_command(d, "mouse_status", null)
            return !(s?["playing"] ?? true)
        }
        t |> success(drained != null, "mouse_play timeline drains")
        let s2 = post_command(d, "mouse_status", null)
        t |> equal(s2?["cursor_owned"] ?? false, true, "cursor_owned still true after timeline drain")
    }
}

[test]
def test_mouse_stop_releases_held_buttons(t : T?) {
    // Parity with key_stop (PR #2633 review, copilot r3229531303): an
    // interrupted mouse_play sequence that pressed a button without queuing
    // its release would leave ImGui believing the button is still held.
    // Press button 0 manually, verify ImGui sees it held, mouse_stop, then
    // verify both held_count and io.mouse_buttons[0] clear.
    with_imgui_app(FEATURE) $(d) {
        var snap0 = wait_for_render(d, "SYNTH_WIN", 15.0f)
        if (snap0 == null) {
            t |> success(false, "SYNTH_WIN renders")
            return
        }

        post_command(d, "mouse_click", JV((button = 0, action = "press")))
        let pressed = wait_until(d, 180) $(var snap) {
            let s = post_command(d, "mouse_status", null)
            return (s?["held_count"] ?? 0) >= 1 && (snap?["io"]?["mouse_buttons"]?[0] ?? false)
        }
        t |> success(pressed != null, "button 0 is held (held_count + io.mouse_buttons[0])")

        let stopped = post_command(d, "mouse_stop", null)
        t |> equal(stopped?["released"] ?? -1, 1, "mouse_stop.released == 1")

        let cleared = wait_until(d, 180) $(var snap) {
            let s = post_command(d, "mouse_status", null)
            let still_held = snap?["io"]?["mouse_buttons"]?[0] ?? true
            return (s?["held_count"] ?? 1) == 0 && !still_held
        }
        t |> success(cleared != null, "held_count == 0 + io.mouse_buttons[0] == false after stop")
    }
}
