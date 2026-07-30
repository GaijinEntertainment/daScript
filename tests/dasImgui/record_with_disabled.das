options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``with_disabled.apng`` against
//! ``examples/tutorial/with_disabled.das``. Real-input, self-verifying tour of
//! the stateless scope-wrappers, anchored on the with_disabled inert -> live
//! contrast:
//!   1. checkbox off  -> CHILD_SAVE is greyed; a click is IGNORED (proven inert by 3)
//!   2. click the checkbox -> value flips true (with_disabled's argument becomes false)
//!   3. click CHILD_SAVE -> click_count is now EXACTLY 1, so the stage-1 click never counted
//!   4. press-and-hold STEP_UP under with_button_repeat -> the count climbs TWICE
//!      (one press ticks once; two distinct climbs prove genuine repeat)
//!   5. with_font / with_clip_rect closer (same push-on-enter / pop-on-exit shape)

[export]
def main {
    with_recording_app("examples/tutorial/with_disabled.das",
                       "with_disabled.apng", 60) $(app) {
        let T_ENABLED = "SCOPES_WIN/ENABLED_MASTER"
        let T_SAVE    = "SCOPES_WIN/CHILD_SAVE"
        let T_STEP    = "SCOPES_WIN/STEP_UP"
        let T_FONT    = "SCOPES_WIN/FONT_DEMO"

        var snap = wait_for_render(app, T_ENABLED, 10.0f)
        if (snap == null) {
            panic("{T_ENABLED} never rendered - wrong app running?")
        }
        let p_enabled = widget_click_point(snap, T_ENABLED)   // checkbox: the glyph, where the click lands
        let p_save    = widget_click_point(snap, T_SAVE)
        let p_step    = widget_click_point(snap, T_STEP)

        //! Initial cursor placement - flips synth_cursor_owned.
        move_to(app, (60.0f, 40.0f), 600)
        hold_content(app, SETTLE_MS)

        // ---- Stage 1: disabled -> a click does nothing (proven inert by stage 3) ----
        move_to(app, p_save, 900)
        hold_content(app, SETTLE_MS)
        let d1 = say_begin(app, "with_disabled - greyed, click ignored", T_SAVE,
            [voice = "These buttons live inside a with disabled block, tied to the checkbox above. The checkbox is off, so they are greyed out. Watch me click Save. Nothing happens."])
        let anchor1 = record_frame_count(app)
        let lead1 = d1 > 1200u ? 600u : d1 / 3u
        hold_content(app, lead1)
        click(app, T_SAVE)                                    // lands on a disabled button -> no ripple, no count
        hold_remainder_content(app, d1, anchor1)

        // ---- Stage 2: flip the gate -> with_disabled's argument becomes false ----
        move_to(app, p_enabled, 900)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "enable the group", T_ENABLED,
            [voice = "Now I turn the checkbox on. The block's argument flips to false, and the whole group wakes up."]),
            [T_ENABLED])

        // ---- Stage 3: same button, now live -> click_count == 1 (so stage 1 was inert) ----
        move_to(app, p_save, 900)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "now live - counter ticks to 1", T_SAVE,
            [voice = "Same Save button, no code change. This time the click lands, and the counter ticks up to one."]),
            [T_SAVE])
        record_check_value(app, T_SAVE, "click_count", 1, 120)   // EXACTLY one: the greyed click in stage 1 never registered

        // ---- Stage 4: with_button_repeat -> a held press fires again and again ----
        move_to(app, p_step, 900)
        hold_content(app, SETTLE_MS)
        let d4 = say_begin(app, "with_button_repeat - hold to repeat", T_STEP,
            [voice = "This plus button sits inside with button repeat. I press and hold, and instead of one click it fires over and over. The counter races up."])
        let anchor4 = record_frame_count(app)
        let lead4 = d4 > 1500u ? 700u : d4 / 3u
        hold_content(app, lead4)
        //! A single 3s press-hold (from == to): with_button_repeat turns the held
        //! press into a stream of clicks at ImGui's repeat rate.
        var hold_ev : array<JsonValue?>
        hold_ev |> drag_along(0, p_step, p_step, 3000, 300)
        post_command(app, "imgui_mouse_play", JV((events = hold_ev)))
        unsafe { delete hold_ev; }
        let hold_play = 300u + 100u + 3000u + 200u + 300u        // drag_along timeline span (ms)
        // Prove genuine REPEAT, not one click: the count climbs off zero, then climbs AGAIN.
        record_check_changed(app, T_STEP, "click_count", "", 400)
        var msnap = snapshot(app)
        let mid = widget_payload_field(msnap, T_STEP, "click_count")
        record_check_changed(app, T_STEP, "click_count", mid != null ? write_json(mid) : "", 400)
        // The checks returned early (mid-hold); let the full hold play out, then pad to the voice.
        hold_content(app, hold_play)
        hold_remainder_content(app, d4, anchor4)

        // ---- Stage 5: family closer - font + clip_rect share the scope shape ----
        say(app, "with_font / with_clip_rect - push on enter, pop on exit", T_FONT,
            [voice = "The same shape wraps the font and the clip rectangle. Each pushes its setting when the block opens, and pops it when the block ends."])
    }
}
