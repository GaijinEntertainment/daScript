options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Visual-aids keycap HUD — wire integration gates.
//!
//! The HUD itself is a drawn overlay (asserted visually in the APNG
//! recording). This test verifies the data path: the live_command toggle
//! works, and the underlying modifier-state and synth-codepoint signals
//! that `paint_key_hud()` reads are reachable from the GLFW key chain.

let FEATURE = "modules/dasImgui/examples/features/glfw_synth_keys.das"

[test]
def test_key_hud_toggle(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap0 = wait_for_render(d, "NAME_INPUT", 15.0f)
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

[test]
def test_key_hud_signals_reachable(t : T?) {
    //! `paint_key_hud` reads two signals from glfw_live: `synth_held_keys`
    //! (size = key_status.held_count) for modifier-pill state, and
    //! `synth_last_codepoint_t` (advances when a char event fires) for new
    //! keycap traces. This test confirms both signals respond to the synth
    //! driver — if `held_count` and `last_codepoint` both move, the HUD has
    //! everything it needs to render correctly.
    with_imgui_app(FEATURE) $(d) {
        var snap0 = wait_for_render(d, "NAME_INPUT", 15.0f)
        if (snap0 == null) {
            t |> success(false, "NAME_INPUT renders")
            return
        }

        // Held-keys signal: press Shift, verify held_count rises.
        post_command(d, "key_press", JV((keycode = 340)))   // GLFW_KEY_LEFT_SHIFT
        let held = wait_until(d, 180) $(var snap) {
            let s = post_command(d, "key_status", null)
            return (s?["held_count"] ?? 0) >= 1
        }
        t |> success(held != null, "key_status.held_count >= 1 after Shift press")
        post_command(d, "key_release", JV((keycode = 340)))

        // Codepoint signal: type one char, verify last_codepoint advances.
        post_command(d, "key_type", JV((text = "x", per_char_ms = 30)))
        let typed = wait_until(d, 180) $(var snap) {
            let s = post_command(d, "key_status", null)
            return (s?["last_codepoint"] ?? 0u) == uint('x')
        }
        t |> success(typed != null, "key_status.last_codepoint == 'x' after key_type")
    }
}
