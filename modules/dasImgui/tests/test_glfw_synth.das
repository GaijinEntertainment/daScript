options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public
require math

//! GLFW-side synthetic keyboard and mouse driver - end-to-end gates.
//! The driver in `modules/dasGlfw/dasglfw/glfw_live.das` posts events
//! through dasGLFW's C++ chain dispatcher, which delivers them to the
//! ImGui_ImplGlfw callbacks, so ImGui sees them as real input.

let GLFW_KEYS_FEATURE = "modules/dasImgui/examples/features/glfw_synth_keys.das"
let GLFW_MOUSE_FEATURE = "modules/dasImgui/examples/features/glfw_synth_mouse.das"

[test(description = "a synth Shift press raises key_status.held_count to at least 1 and its release brings it back to 0", timeout = 1200)]
def test_key_press_release_held_count(t : T?) {
    with_imgui_app(GLFW_KEYS_FEATURE) $(d) {
        let snap0 = wait_for_render(d, "MAIN_WIN/NAME_INPUT", 15.0f)
        t |> success(snap0 != null, "NAME_INPUT renders")
        if (snap0 == null) return

        // GLFW_KEY_LEFT_SHIFT == 340.
        post_command(d, "key_press", JV((keycode = 340)))
        let pressed = wait_until(d, 180) $(var _snap) {
            let s = post_command(d, "key_status", null)
            return (s?["held_count"] ?? 0) >= 1
        }
        t |> success(pressed != null, "key_status.held_count >= 1 after Shift press")

        post_command(d, "key_release", JV((keycode = 340)))
        let released = wait_until(d, 180) $(var _snap) {
            let s = post_command(d, "key_status", null)
            return (s?["held_count"] ?? 1) == 0
        }
        t |> success(released != null, "key_status.held_count == 0 after Shift release")
    }
}

[test(description = "key_type through the GLFW key chain types mixed-case Hi into a focused input, auto-shifting the capital", timeout = 1200)]
def test_key_type_lands_in_input(t : T?) {
    with_imgui_app(GLFW_KEYS_FEATURE) $(d) {
        let snap0 = wait_for_render(d, "MAIN_WIN/NAME_INPUT", 15.0f)
        t |> success(snap0 != null, "NAME_INPUT renders")
        if (snap0 == null) return

        // Auto-focus on first render reaches keyboard focus but doesn't reliably settle
        // io.active_widget by the time we poll, so drive explicit focus via playwright (same
        // pattern as test_imgui_type_text_buffer_fills, which focuses internally via type_text).
        focus(d, "MAIN_WIN/NAME_INPUT")
        let focused = wait_until(d, 300) $(var snap) {
            return snap?["globals"]?["MAIN_WIN/NAME_INPUT"]?["focus"] ?? false
        }
        t |> success(focused != null, "NAME_INPUT receives keyboard focus")

        // Mixed-case payload exercises the auto-shift path.
        post_command(d, "key_type", JV((text = "Hi", per_char_ms = 30)))

        // Wait for the timeline to drain.
        let done = wait_until(d, 240) $(var _snap) {
            let s = post_command(d, "key_status", null)
            return !(s?["playing"] ?? true)
        }
        t |> success(done != null, "key_type timeline completes")

        // Then verify the input buffer received "Hi".
        let landed = wait_until(d, 180) $(var snap) {
            let v = snap?["globals"]?["MAIN_WIN/NAME_INPUT"]?["payload"]?["value"] ?? ""
            return v == "Hi"
        }
        t |> success(landed != null, "NAME_INPUT.value == \"Hi\" after key_type")
    }
}

[test(description = "key_chord Ctrl+A queues four events and drains with every key released", timeout = 1200)]
def test_key_chord_completes(t : T?) {
    with_imgui_app(GLFW_KEYS_FEATURE) $(d) {
        let snap0 = wait_for_render(d, "MAIN_WIN/NAME_INPUT", 15.0f)
        if (snap0 == null) {
            t |> success(false, "NAME_INPUT renders")
            return
        }

        // Ctrl+A. Queues press(Ctrl)/press(A)/release(A)/release(Ctrl).
        let resp = post_command(d, "key_chord", JV((mods = ["Ctrl"], key = "A")))
        t |> equal(resp?["queued"] ?? 0, 4, "key_chord queues 4 events")

        let done = wait_until(d, 240) $(var _snap) {
            let s = post_command(d, "key_status", null)
            return !(s?["playing"] ?? true) && (s?["held_count"] ?? 1) == 0
        }
        t |> success(done != null, "chord drains and releases all keys")
    }
}

[test(description = "a hand-built key_play press-char-release timeline runs to completion with no key left held", timeout = 1200)]
def test_key_play_event_list_completes(t : T?) {
    with_imgui_app(GLFW_KEYS_FEATURE) $(d) {
        let snap0 = wait_for_render(d, "MAIN_WIN/NAME_INPUT", 15.0f)
        if (snap0 == null) {
            t |> success(false, "NAME_INPUT renders")
            return
        }

        // Hand-built: press 'A' (keycode 65), char 'A' (codepoint 65), release.
        var events <- [
            JV((t_ms = 0,   kind = "press",   keycode = 65)),
            JV((t_ms = 0,   kind = "char",    codepoint = 65u)),
            JV((t_ms = 30,  kind = "release", keycode = 65))
        ]
        post_command(d, "key_play", JV((events = events)))

        let done = wait_until(d, 240) $(var _snap) {
            let s = post_command(d, "key_status", null)
            return !(s?["playing"] ?? true) && (s?["held_count"] ?? 1) == 0
        }
        t |> success(done != null, "key_play timeline runs to completion")
    }
}

[test(description = "key_stop ends a long key_type mid-flight, reporting stopped and clearing playing", timeout = 1200)]
def test_key_stop_aborts(t : T?) {
    with_imgui_app(GLFW_KEYS_FEATURE) $(d) {
        let snap0 = wait_for_render(d, "MAIN_WIN/NAME_INPUT", 15.0f)
        if (snap0 == null) {
            t |> success(false, "NAME_INPUT renders")
            return
        }

        // Start a long-duration type, then stop it mid-flight.
        post_command(d, "key_type", JV((text = "the quick brown fox", per_char_ms = 500)))
        let started = wait_until(d, 60) $(var _snap) {
            let s = post_command(d, "key_status", null)
            return s?["playing"] ?? false
        }
        t |> success(started != null, "key_type enters playing state")

        let stopped = post_command(d, "key_stop", null)
        t |> equal(stopped?["stopped"] ?? false, true, "key_stop reports stopped=true")

        // Gate the stop taking effect (it may clear across a frame boundary)
        // rather than reading key_status synchronously.
        let halted = wait_until(d, 60) $(var _snap) {
            let s = post_command(d, "key_status", null)
            return !(s?["playing"] ?? true)
        }
        t |> success(halted != null, "key_status.playing == false after stop")
    }
}

//! Not a [test]: the glfw synth chord path no longer makes Ctrl
//! visible to ImGui (imgui_visual_aids is decoupled from glfw_live);
//! Ctrl-shortcut coverage is test_synth_io.das.
def test_key_chord_ctrl_visible_to_imgui(t : T?) {
    with_imgui_app(GLFW_KEYS_FEATURE) $(d) {
        let snap0 = wait_for_render(d, "MAIN_WIN/NAME_INPUT", 15.0f)
        if (snap0 == null) {
            t |> success(false, "NAME_INPUT renders")
            return
        }
        // Hold Ctrl long enough for snapshot poll to observe — key_press
        // doesn't auto-release, unlike key_chord's tight-stamped sequence.
        post_command(d, "key_press", JV((keycode = 341)))   // GLFW_KEY_LEFT_CONTROL
        let saw_ctrl = wait_until(d, 240) $(var snap) {
            return snap?["io"]?["key_ctrl"] ?? false
        }
        t |> success(saw_ctrl != null, "io.key_ctrl == true while LEFT_CONTROL synth-held")
        post_command(d, "key_release", JV((keycode = 341)))
        let cleared = wait_until(d, 240) $(var snap) {
            return !(snap?["io"]?["key_ctrl"] ?? true)
        }
        t |> success(cleared != null, "io.key_ctrl == false after release")
    }
}

[test(description = "key_stop releases a synth-held Ctrl that had no paired release, reporting released=1 and dropping held_count to 0", timeout = 1200)]
def test_key_stop_releases_held_modifiers(t : T?) {
    // key_stop must release any synth-pressed keys, or ImGui inherits stuck modifier state.
    // Simulate an interrupted key_chord: manually press LEFT_CONTROL (no paired release) then
    // key_stop — held_count must drop to 0 and the stop response must report released=1.
    with_imgui_app(GLFW_KEYS_FEATURE) $(d) {
        let snap0 = wait_for_render(d, "MAIN_WIN/NAME_INPUT", 15.0f)
        if (snap0 == null) {
            t |> success(false, "NAME_INPUT renders")
            return
        }

        // GLFW_KEY_LEFT_CONTROL == 341.
        post_command(d, "key_press", JV((keycode = 341)))
        let pressed = wait_until(d, 180) $(var _snap) {
            let s = post_command(d, "key_status", null)
            return (s?["held_count"] ?? 0) >= 1
        }
        t |> success(pressed != null, "Ctrl is held (held_count >= 1)")

        let stopped = post_command(d, "key_stop", null)
        t |> equal(stopped?["released"] ?? -1, 1, "key_stop.released == 1")

        let cleared = wait_until(d, 180) $(var _snap) {
            let s = post_command(d, "key_status", null)
            return (s?["held_count"] ?? 1) == 0
        }
        t |> success(cleared != null, "held_count == 0 after key_stop")
    }
}

[test(description = "the imgui_key_hud command turns the keycap HUD on with modifiers shown by default, and off with show_modifiers=false honored", timeout = 1200)]
def test_key_hud_toggle(t : T?) {
    with_imgui_app(GLFW_KEYS_FEATURE) $(d) {
        let snap0 = wait_for_render(d, "MAIN_WIN/NAME_INPUT", 15.0f)
        t |> success(snap0 != null, "NAME_INPUT renders")
        if (snap0 == null) return

        let on = post_command(d, "imgui_key_hud", JV((enabled = true)))
        t |> equal(on?["ok"] ?? false, true, "imgui_key_hud ok=true on toggle on")
        t |> equal(on?["enabled"] ?? false, true, "imgui_key_hud reports enabled=true")
        t |> equal(on?["show_modifiers"] ?? false, true,
                   "imgui_key_hud reports show_modifiers=true by default")

        let off = post_command(d, "imgui_key_hud", JV((enabled = false, show_modifiers = false)))
        t |> equal(off?["enabled"] ?? true, false, "imgui_key_hud reports enabled=false on toggle off")
        t |> equal(off?["show_modifiers"] ?? true, false,
                   "imgui_key_hud honors show_modifiers=false")
    }
}

[test(description = "the held-keys count and last-codepoint signals the keycap HUD paints from respond to a synth Shift press and a key_type", timeout = 1200)]
def test_key_hud_signals_reachable(t : T?) {
    // paint_key_hud reads two glfw_live signals: synth_held_keys (size = key_status.held_count) for
    // modifier pills, and synth_last_codepoint_t (advances on a char event) for keycap traces. This
    // confirms both respond to the synth driver — if held_count and last_codepoint move, the HUD works.
    with_imgui_app(GLFW_KEYS_FEATURE) $(d) {
        let snap0 = wait_for_render(d, "MAIN_WIN/NAME_INPUT", 15.0f)
        if (snap0 == null) {
            t |> success(false, "NAME_INPUT renders")
            return
        }

        // Held-keys signal: press Shift, verify held_count rises.
        post_command(d, "key_press", JV((keycode = 340)))   // GLFW_KEY_LEFT_SHIFT
        let held = wait_until(d, 180) $(var _snap) {
            let s = post_command(d, "key_status", null)
            return (s?["held_count"] ?? 0) >= 1
        }
        t |> success(held != null, "key_status.held_count >= 1 after Shift press")
        post_command(d, "key_release", JV((keycode = 340)))

        // Codepoint signal: type one char, verify last_codepoint advances.
        post_command(d, "key_type", JV((text = "x", per_char_ms = 30)))
        let typed = wait_until(d, 180) $(var _snap) {
            let s = post_command(d, "key_status", null)
            return (s?["last_codepoint"] ?? 0u) == uint('x')
        }
        t |> success(typed != null, "key_status.last_codepoint == 'x' after key_type")
    }
}

//! No io.mouse_pos gates: on a focused window ImGui_ImplGlfw re-polls
//! glfwGetCursorPos every frame and overrides the synth position.
[test(description = "mouse_pos sets the daslang-side cursor that mouse_status reports to the exact coordinates", timeout = 1200)]
def test_mouse_pos_lands_in_status(t : T?) {
    with_imgui_app(GLFW_MOUSE_FEATURE) $(d) {
        let snap0 = wait_for_render(d, "SYNTH_WIN", 15.0f)
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

[test(description = "a synth button press and release reach ImGui's event-driven io.mouse_buttons[0]", timeout = 1200)]
def test_mouse_buttons_io_mirror(t : T?) {
    with_imgui_app(GLFW_MOUSE_FEATURE) $(d) {
        let snap0 = wait_for_render(d, "SYNTH_WIN", 15.0f)
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

[test(description = "mouse_move_to passes through mid-flight positions and lands on its endpoint when playback ends", timeout = 1200)]
def test_mouse_move_to_animates(t : T?) {
    with_imgui_app(GLFW_MOUSE_FEATURE) $(d) {
        let snap0 = wait_for_render(d, "SYNTH_WIN", 15.0f)
        if (snap0 == null) {
            t |> success(false, "SYNTH_WIN renders")
            return
        }

        // Park at (50, 50), then sweep to (350, 250) over 400ms.
        post_command(d, "mouse_pos", JV((x = 50.0f, y = 50.0f)))
        post_command(d, "mouse_move_to", JV((x = 350.0f, y = 250.0f, duration_ms = 400)))

        // Mid-flight: cursor_x somewhere between start and end (via mouse_status). Wide window —
        // under suite load one slow frame can advance the timeline a lot, but anywhere strictly
        // between start and end proves the tick interpolates rather than snapping to the end.
        let midflight = wait_until(d, 180) $(var _snap) {
            let s = post_command(d, "mouse_status", null)
            let mx = s?["cursor_x"] ?? -1.0f
            return mx > 55.0f && mx < 345.0f
        }
        t |> success(midflight != null, "cursor passes through mid-flight x")

        // Wait until playback completes and cursor lands on the endpoint.
        let landed = wait_until(d, 300) $(var _snap) {
            let s = post_command(d, "mouse_status", null)
            let playing = s?["playing"] ?? true
            let mx = s?["cursor_x"] ?? -1.0f
            let my = s?["cursor_y"] ?? -1.0f
            return !playing && abs(mx - 350.0f) < 1.0f && abs(my - 250.0f) < 1.0f
        }
        t |> success(landed != null, "cursor lands on (350,250) per mouse_status")
    }
}

[test(description = "a hand-built mouse_play timeline of moves, a scroll and a click runs to completion and leaves the cursor on its last move", timeout = 1200)]
def test_mouse_play_event_list_completes(t : T?) {
    with_imgui_app(GLFW_MOUSE_FEATURE) $(d) {
        let snap0 = wait_for_render(d, "SYNTH_WIN", 15.0f)
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
        let done = wait_until(d, 300) $(var _snap) {
            let s = post_command(d, "mouse_status", null)
            let playing = s?["playing"] ?? true
            let mx = s?["cursor_x"] ?? -1.0f
            let my = s?["cursor_y"] ?? -1.0f
            return !playing && abs(mx - 300.0f) < 1.0f && abs(my - 200.0f) < 1.0f
        }
        t |> success(done != null, "mouse_play timeline runs to completion + lands on last move")
    }
}

[test(description = "mouse_stop ends a long mouse_move_to mid-flight, reporting stopped and clearing playing", timeout = 1200)]
def test_mouse_stop_aborts(t : T?) {
    with_imgui_app(GLFW_MOUSE_FEATURE) $(d) {
        let snap0 = wait_for_render(d, "SYNTH_WIN", 15.0f)
        if (snap0 == null) {
            t |> success(false, "SYNTH_WIN renders")
            return
        }

        // Start a long-duration move, then stop it mid-flight.
        post_command(d, "mouse_pos", JV((x = 50.0f, y = 50.0f)))
        post_command(d, "mouse_move_to", JV((x = 400.0f, y = 300.0f, duration_ms = 5000)))
        // After kickoff, mouse_status reports playing=true.
        let started = wait_until(d, 60) $(var _snap) {
            let s = post_command(d, "mouse_status", null)
            return s?["playing"] ?? false
        }
        t |> success(started != null, "mouse_move_to enters playing state")

        let stopped = post_command(d, "mouse_stop", null)
        t |> equal(stopped?["stopped"] ?? false, true, "mouse_stop reports stopped=true")

        // Gate the stop taking effect (it may clear across a frame boundary)
        // rather than reading mouse_status synchronously.
        let halted = wait_until(d, 60) $(var _snap) {
            let s = post_command(d, "mouse_status", null)
            return !(s?["playing"] ?? true)
        }
        t |> success(halted != null, "mouse_status.playing == false after stop")
    }
}

[test(description = "any synth mouse event makes cursor_owned true, and it stays true after a mouse_play timeline drains", timeout = 1200)]
def test_synth_cursor_ownership_sticky(t : T?) {
    // Cursor sprite vanished mid-recording: after a mouse_play timeline drained, playing flipped
    // false and effective_cursor_pos fell back to GetMousePos(), jumping the sprite to the real OS
    // cursor. Fix is sticky ownership: any synth event flips cursor_owned true (mouse_status exposes it).
    with_imgui_app(GLFW_MOUSE_FEATURE) $(d) {
        let snap0 = wait_for_render(d, "SYNTH_WIN", 15.0f)
        if (snap0 == null) {
            t |> success(false, "SYNTH_WIN renders")
            return
        }
        let s0 = post_command(d, "mouse_status", null)
        t |> equal(s0?["cursor_owned"] ?? true, false, "cursor_owned starts false")
        // Single synth event flips ownership. Gate on cursor_owned converging
        // rather than reading mouse_status synchronously after the one-shot.
        post_command(d, "mouse_pos", JV((x = 100.0f, y = 100.0f)))
        let owned = wait_until(d, 60) $(var _snap) {
            let s = post_command(d, "mouse_status", null)
            return s?["cursor_owned"] ?? false
        }
        t |> success(owned != null, "cursor_owned flips true after mouse_pos")
        let s1 = post_command(d, "mouse_status", null)
        t |> equal(s1?["playing"] ?? true, false, "mouse_status.playing is false (mouse_pos is one-shot)")
        // Run a short timeline, wait for drain, ownership stays.
        var events <- [
            JV((t_ms = 0,  kind = "move", x = 50.0f,  y = 50.0f)),
            JV((t_ms = 80, kind = "move", x = 200.0f, y = 200.0f))
        ]
        post_command(d, "mouse_play", JV((events = events)))
        let drained = wait_until(d, 240) $(var _snap) {
            let s = post_command(d, "mouse_status", null)
            return !(s?["playing"] ?? true)
        }
        t |> success(drained != null, "mouse_play timeline drains")
        let s2 = post_command(d, "mouse_status", null)
        t |> equal(s2?["cursor_owned"] ?? false, true, "cursor_owned still true after timeline drain")
    }
}

[test(description = "mouse_stop releases a synth-pressed button, reporting released=1 and clearing both held_count and io.mouse_buttons[0]", timeout = 1200)]
def test_mouse_stop_releases_held_buttons(t : T?) {
    // Parity with key_stop: an interrupted mouse_play that pressed a button without queuing its
    // release leaves ImGui believing the button is held. Press button 0 manually, verify ImGui
    // sees it held, mouse_stop, then verify held_count and io.mouse_buttons[0] both clear.
    with_imgui_app(GLFW_MOUSE_FEATURE) $(d) {
        let snap0 = wait_for_render(d, "SYNTH_WIN", 15.0f)
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
