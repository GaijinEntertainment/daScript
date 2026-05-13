options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public
require math

//! GLFW-side synthetic keyboard driver — end-to-end gates.
//!
//! Mirrors `test_glfw_synth_mouse.das`. The synth driver in
//! `modules/dasGlfw/dasglfw/glfw_live.das` posts key+char events through
//! dasGLFW's C++ chain dispatcher; the chain delivers them to
//! ImGui_ImplGlfw_KeyCallback / _CharCallback, so ImGui sees them as real
//! input. Unlike mouse, there's no per-frame poll-override race
//! (ImGui_ImplGlfw doesn't repoll keyboard state), so a single chain
//! dispatch suffices — no `apply_synth_io_override` needed for keys.

let FEATURE = "modules/dasImgui/examples/features/glfw_synth_keys.das"

[test]
def test_key_press_release_held_count(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap0 = wait_for_render(d, "NAME_INPUT", 15.0f)
        t |> success(snap0 != null, "NAME_INPUT renders")
        if (snap0 == null) return

        // GLFW_KEY_LEFT_SHIFT == 340.
        post_command(d, "key_press", JV((keycode = 340)))
        let pressed = wait_until(d, 180) $(var snap) {
            let s = post_command(d, "key_status", null)
            return (s?["held_count"] ?? 0) >= 1
        }
        t |> success(pressed != null, "key_status.held_count >= 1 after Shift press")

        post_command(d, "key_release", JV((keycode = 340)))
        let released = wait_until(d, 180) $(var snap) {
            let s = post_command(d, "key_status", null)
            return (s?["held_count"] ?? 1) == 0
        }
        t |> success(released != null, "key_status.held_count == 0 after Shift release")
    }
}

[test]
def test_key_type_lands_in_input(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap0 = wait_for_render(d, "NAME_INPUT", 15.0f)
        t |> success(snap0 != null, "NAME_INPUT renders")
        if (snap0 == null) return

        // Auto-focus on first render reaches keyboard focus but doesn't reliably
        // settle io.active_widget by the time we poll, so drive explicit focus
        // via playwright. Same pattern as `test_imgui_type_text_buffer_fills`,
        // which goes through `type_text` (which calls focus internally).
        focus(d, "NAME_INPUT")
        let focused = wait_until(d, 120) $(var snap) {
            return snap?["globals"]?["NAME_INPUT"]?["focus"] ?? false
        }
        t |> success(focused != null, "NAME_INPUT receives keyboard focus")

        // Mixed-case payload exercises the auto-shift path.
        post_command(d, "key_type", JV((text = "Hi", per_char_ms = 30)))

        // Wait for the timeline to drain.
        let done = wait_until(d, 240) $(var snap) {
            let s = post_command(d, "key_status", null)
            return !(s?["playing"] ?? true)
        }
        t |> success(done != null, "key_type timeline completes")

        // Then verify the input buffer received "Hi".
        let landed = wait_until(d, 180) $(var snap) {
            let v = snap?["globals"]?["NAME_INPUT"]?["payload"]?["value"] ?? ""
            return v == "Hi"
        }
        t |> success(landed != null, "NAME_INPUT.value == \"Hi\" after key_type")
    }
}

[test]
def test_key_chord_completes(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap0 = wait_for_render(d, "NAME_INPUT", 15.0f)
        if (snap0 == null) {
            t |> success(false, "NAME_INPUT renders")
            return
        }

        // Ctrl+A. Queues press(Ctrl)/press(A)/release(A)/release(Ctrl).
        let resp = post_command(d, "key_chord", JV((mods = ["Ctrl"], key = "A")))
        t |> equal(resp?["queued"] ?? 0, 4, "key_chord queues 4 events")

        let done = wait_until(d, 240) $(var snap) {
            let s = post_command(d, "key_status", null)
            return !(s?["playing"] ?? true) && (s?["held_count"] ?? 1) == 0
        }
        t |> success(done != null, "chord drains and releases all keys")
    }
}

[test]
def test_key_play_event_list_completes(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap0 = wait_for_render(d, "NAME_INPUT", 15.0f)
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

        let done = wait_until(d, 240) $(var snap) {
            let s = post_command(d, "key_status", null)
            return !(s?["playing"] ?? true) && (s?["held_count"] ?? 1) == 0
        }
        t |> success(done != null, "key_play timeline runs to completion")
    }
}

[test]
def test_key_stop_aborts(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap0 = wait_for_render(d, "NAME_INPUT", 15.0f)
        if (snap0 == null) {
            t |> success(false, "NAME_INPUT renders")
            return
        }

        // Start a long-duration type, then stop it mid-flight.
        post_command(d, "key_type", JV((text = "the quick brown fox", per_char_ms = 500)))
        let started = wait_until(d, 60) $(var snap) {
            let s = post_command(d, "key_status", null)
            return s?["playing"] ?? false
        }
        t |> success(started != null, "key_type enters playing state")

        let stopped = post_command(d, "key_stop", null)
        t |> equal(stopped?["stopped"] ?? false, true, "key_stop reports stopped=true")

        let halted = post_command(d, "key_status", null)
        t |> equal(halted?["playing"] ?? true, false, "key_status.playing == false after stop")
    }
}

[test]
def test_key_chord_ctrl_visible_to_imgui(t : T?) {
    // Ctrl+A in the recording didn't select text. Root cause: ImGui_ImplGlfw_
    // KeyCallback runs UpdateKeyModifiers which polls glfwGetKey() — finds the
    // physical Ctrl unheld and posts AddKeyEvent(ImGuiMod_Ctrl, false)
    // immediately after our synth Ctrl press. Queue drains: io.KeyCtrl = false,
    // InputText's Ctrl+A shortcut never fires. Fix: apply_synth_io_override
    // re-queues ImGuiMod_Ctrl=true after the impl's poll, so NewFrame's drain
    // ends with KeyCtrl true. Assert via the new io.key_ctrl snapshot field.
    with_imgui_app(FEATURE) $(d) {
        var snap0 = wait_for_render(d, "NAME_INPUT", 15.0f)
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

[test]
def test_key_stop_releases_held_modifiers(t : T?) {
    // PR #2633 review (copilot r3229531303): key_stop must release any
    // synth-pressed keys, or ImGui inherits stuck modifier state. Simulate
    // an interrupted key_chord by manually pressing LEFT_CONTROL (no paired
    // release queued) and then calling key_stop — the held_count must drop
    // to 0 and the stop response must report released=1.
    with_imgui_app(FEATURE) $(d) {
        var snap0 = wait_for_render(d, "NAME_INPUT", 15.0f)
        if (snap0 == null) {
            t |> success(false, "NAME_INPUT renders")
            return
        }

        // GLFW_KEY_LEFT_CONTROL == 341.
        post_command(d, "key_press", JV((keycode = 341)))
        let pressed = wait_until(d, 180) $(var snap) {
            let s = post_command(d, "key_status", null)
            return (s?["held_count"] ?? 0) >= 1
        }
        t |> success(pressed != null, "Ctrl is held (held_count >= 1)")

        let stopped = post_command(d, "key_stop", null)
        t |> equal(stopped?["released"] ?? -1, 1, "key_stop.released == 1")

        let cleared = wait_until(d, 180) $(var snap) {
            let s = post_command(d, "key_status", null)
            return (s?["held_count"] ?? 1) == 0
        }
        t |> success(cleared != null, "held_count == 0 after key_stop")
    }
}
