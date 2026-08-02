options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``driving_outside.apng`` against ``examples/tutorial/driving_outside.das``.
//! This is the one tutorial whose SUBJECT is the JSON command surface itself, so every
//! action is performed via the L2/L3 live commands (imgui_force_set / imgui_click /
//! imgui_open / imgui_close) - the "no input device required" model the page teaches -
//! and verified against the resulting widget state (force_set_verified is the correct
//! rail here, per the recording-bar exception for force_set-subject tutorials). Five
//! voiced, self-verifying beats:
//!   1. force_set string -> USER.value == "Alice".
//!   2. force_set number -> SPEED.value == 7.
//!   3. force_set index  -> PRESET.value == 2 (combo selection).
//!   4. imgui_click x2    -> RUN_BTN.click_count == 2.
//!   5. imgui_open/close  -> the popup body renders, then stops (verified via a stable
//!      named widget inside it, since a popup carries no bbox).

//! force_set ``value`` into ``target`` UNDER an already-posted voice line and verify it
//! took (force_set_verified accumulates a miss -> teardown panic on failure).
def fset_under_voice(app : ImguiApp; dwell : uint; target : string; value : auto(T)) {
    let anchor = record_frame_count(app)
    let lead = dwell > 1400u ? 700u : dwell / 3u
    hold_content(app, lead)
    force_set_verified(app, target, value)
    hold_remainder_content(app, dwell, anchor)
}

//! Fire ``imgui_click`` on ``target`` ``clicks`` times UNDER a voice line, then assert
//! the click counter reached ``clicks``.
def click_under_voice(app : ImguiApp; dwell : uint; target : string; clicks : int) {
    let anchor = record_frame_count(app)
    let lead = dwell > 1400u ? 700u : dwell / 3u
    hold_content(app, lead)
    for (_i in range(clicks)) {
        post_command(app, "imgui_click", JV((target = target)))
        hold_content(app, 550u)
    }
    record_check_value(app, target, "click_count", clicks, 180)
    hold_remainder_content(app, dwell, anchor)
}

//! Drive a popup open then shut via ``imgui_open`` / ``imgui_close`` UNDER a voice line,
//! asserting ``probe`` (a stable widget inside the popup body) renders then stops.
def popup_under_voice(app : ImguiApp; dwell : uint; popup_target : string; probe : string) {
    let anchor = record_frame_count(app)
    let lead = dwell > 1400u ? 700u : dwell / 3u
    hold_content(app, lead)
    post_command(app, "imgui_open", JV((target = popup_target)))
    record_check_rendered(app, probe, true, 180)
    hold_content(app, 2000u)
    post_command(app, "imgui_close", JV((target = popup_target)))
    record_check_rendered(app, probe, false, 180)
    hold_remainder_content(app, dwell, anchor)
}

[export]
def main {
    with_recording_app("examples/tutorial/driving_outside.das",
                       "driving_outside.apng", 60) $(app) {
        let T_USER   = "DRIVE_WIN/USER"
        let T_SPEED  = "DRIVE_WIN/SPEED"
        let T_PRESET = "DRIVE_WIN/PRESET"
        let T_RUN    = "DRIVE_WIN/RUN_BTN"
        let T_POPUP  = "DRIVE_WIN/STATUS_POPUP"
        let POPUP_PROBE = "DRIVE_WIN/STATUS_POPUP/DR_SEP_5"

        var snap = wait_for_render(app, T_USER, 10.0f)
        if (snap == null) {
            panic("{T_USER} never rendered - wrong app running?")
        }

        // No cursor gesture in beats 1-3/5 (force_set / open / close inject state with no pointer),
        // so park the sprite in the right gutter and warm up the capture pipeline. imgui_click in
        // beat 4 then animates the cursor to the button — the one place a pointer appears.
        move_to(app, (760.0f, 300.0f), 800)
        hold_content(app, 1800u)

        // ---- Beat 1: force_set a string into the text input ----
        fset_under_voice(app, say_begin(app, "force_set: string -> input", T_USER,
            [voice = "Every boost widget is also a JSON endpoint. force_set drops a string straight into the text input - no typing."]),
            T_USER, "Alice")

        // ---- Beat 2: force_set a number into the slider ----
        fset_under_voice(app, say_begin(app, "force_set: number -> slider", T_SPEED,
            [voice = "The same verb drives the slider by number. Speed jumps to seven, with no dragging."]),
            T_SPEED, 7)

        // ---- Beat 3: force_set the selected index into the combo ----
        fset_under_voice(app, say_begin(app, "force_set: index -> combo", T_PRESET,
            [voice = "A combo takes the selected index. Two picks the third preset, Active."]),
            T_PRESET, 2)

        // ---- Beat 4: imgui_click fires the button ----
        click_under_voice(app, say_begin(app, "imgui_click: fire the button", T_RUN,
            [voice = "imgui_click fires the button from outside - the very path the chrome would take. Watch the counter climb."]),
            T_RUN, 2)

        // ---- Beat 5: imgui_open / imgui_close drive a popup ----
        popup_under_voice(app, say_begin(app, "imgui_open / imgui_close", T_RUN,
            [voice = "And imgui_open and imgui_close drive the popup open, then shut it again - all from outside, with no mouse at all."]),
            T_POPUP, POPUP_PROBE)
    }
}
