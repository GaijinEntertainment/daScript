options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``custom_widgets.apng`` against
//! ``examples/tutorial/custom_widgets.das``. Real-input, self-verifying tour of
//! a custom [widget] (a rotary knob) on a mixer panel:
//!   1. DRAG the Master knob along its arc -> value rises 0 -> 0.5
//!   2. DRAG the Treble knob up a little   -> value rises 0 -> ~0.33
//!   3. imgui_force_set Bass               -> force_set IS the subject here, so
//!      it is the real input; verify the value took
//!   4. click the ordinary Reset button    -> every knob snaps to its default
//!      (assert Bass returned from -0.5 to 0)
//! The knob disc sits in the TOP third of its 72x112 hitbox, so its centre is
//! (bbox.x + 36, bbox.y + 36) - NOT the bbox centre. Drag endpoints ride a
//! radius-25 arc around that, kept clear of the 90-deg bottom dead zone.

let KNOB_R = 25.0f

//! Centre of the knob disc within its hitbox (top third), not the bbox centre.
def knob_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    var b = snap?["globals"]?[ident]?["bbox"]
    return ((b?["x"] ?? 0.0f) + 36.0f, (b?["y"] ?? 0.0f) + 36.0f)
}

[export]
def main {
    with_recording_app("examples/tutorial/custom_widgets.das",
                       "custom_widgets.apng", 55) $(app) {
        let T_MASTER = "MIXER_WIN/MASTER"
        let T_TREBLE = "MIXER_WIN/TREBLE"
        let T_BASS   = "MIXER_WIN/BASS"
        let T_RESET  = "MIXER_WIN/RESET_BTN"

        var snap = wait_for_render(app, T_RESET, 10.0f)
        if (snap == null) {
            panic("{T_RESET} never rendered - wrong app running?")
        }
        let c_master = knob_center(snap, T_MASTER)
        let c_treble = knob_center(snap, T_TREBLE)
        let c_bass   = knob_center(snap, T_BASS)
        let p_reset  = widget_click_point(snap, T_RESET)

        // Arc endpoints. atan2(mouse - centre) drives the value, so the cursor
        // must ride the active 270-deg arc (top + sides), never the bottom gap.
        let diag = KNOB_R * 0.707f
        // MASTER (0..1, starts 0): 7 o'clock (frac 0) -> 12 o'clock (frac 0.5).
        let master_start = (c_master._0 - diag, c_master._1 + diag)
        let master_end   = (c_master._0,        c_master._1 - KNOB_R)
        // TREBLE (-1..1, starts 0 = 12 o'clock): 12 o'clock -> 2 o'clock (~+0.33).
        let treble_start = (c_treble._0,        c_treble._1 - KNOB_R)
        let treble_end   = (c_treble._0 + diag, c_treble._1 - diag)

        //! Initial cursor placement - flips synth_cursor_owned.
        move_to(app, (40.0f, 30.0f), 600)
        hold_content(app, SETTLE_MS)

        // ---- Beat 1: drag MASTER along its arc -> value 0 -> 0.5 ----
        move_to(app, master_start, 800)
        hold_content(app, SETTLE_MS)
        drag_through_voice(app, say_begin(app, "[widget] - a custom rotary knob", T_MASTER,
            [voice = "There is no built-in rotary knob, so we made our own. It is a single widget definition, a hitbox plus a few draw calls. I drag the Master knob, and its value rises with the angle."]),
            T_MASTER, master_end)

        // ---- Beat 2: drag TREBLE up a little -> value 0 -> ~0.33 ----
        move_to(app, treble_start, 800)
        hold_content(app, SETTLE_MS)
        drag_through_voice(app, say_begin(app, "same state layout as slider_float", T_TREBLE,
            [voice = "The knob reuses the exact state layout a slider uses, so the same value plumbing just works. Here is Treble, dragged up a little."]),
            T_TREBLE, treble_end)

        // ---- Beat 3: imgui_force_set Bass (force_set IS the subject) ----
        move_to(app, c_bass, 800)
        hold_content(app, SETTLE_MS)
        let anchor3 = record_frame_count(app)
        let d3 = say_begin(app, "imgui_force_set drives it too", T_BASS,
            [voice = "A live command can set the value from outside as well. That takes one small dispatch hook, just a few lines, and then force set drives Bass directly."])
        let lead3 = d3 > 1200u ? 700u : d3 / 3u
        hold_content(app, lead3)
        force_set_verified(app, T_BASS, -0.5f)              // the value took; -0.5 is non-zero so it is always present
        hold_remainder_content(app, d3, anchor3)

        // ---- Beat 4: click Reset -> every knob snaps to its default ----
        move_to(app, p_reset, 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "Reset - an ordinary built-in button", T_RESET,
            [voice = "And an ordinary button sits on the same panel. I click Reset, and every knob snaps back to its default."]),
            [T_RESET])
        record_check_value(app, T_BASS, "value", 0.0f, 120) // Reset returned Bass from -0.5 to its default 0
    }
}
