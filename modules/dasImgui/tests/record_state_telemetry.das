options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``state_telemetry.apng`` against ``examples/tutorial/state_telemetry.das``.
//! Widget state lives in daslang globals, not ImGui - so every beat drives a real value
//! and asserts the snapshot reflects it. Four voiced, self-verifying beats:
//!   1. auto-emit  - click Save twice; SAVE_BTN.click_count tracks the clicks (hold_through_voice).
//!   2. dotted flags - force_set SPEED + VOLUME from outside; each value verified (force_set_verified).
//!   3. text_show  - force_set STATUS_TEXT from outside; the mirror takes the string (force_set_verified).
//!   4. bump       - click bump; the app rewrites STATUS_TEXT.value from inside (record_check_changed).

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let b = snap?["globals"]?[ident]?["bbox"]
    return (((b?["x"] ?? 0.0f) + (b?["z"] ?? 0.0f)) * 0.5f,
            ((b?["y"] ?? 0.0f) + (b?["w"] ?? 0.0f)) * 0.5f)
}

[export]
def main {
    with_recording_app("examples/tutorial/state_telemetry.das",
                       "state_telemetry.apng", 60) $(app) {
        let T_SAVE   = "STATE_WIN/SAVE_BTN"
        let T_SPEED  = "STATE_WIN/SPEED"
        let T_VOLUME = "STATE_WIN/VOLUME"
        let T_STATUS = "STATE_WIN/STATUS_TEXT"
        let T_BUMP   = "STATE_WIN/BUMP_STATUS"
        let GUTTER   = (840.0f, 320.0f)   // park the cursor off the widgets while values drive from outside

        var snap = wait_for_render(app, T_SAVE, 10.0f)
        if (snap == null) {
            panic("{T_SAVE} never rendered - wrong app running?")
        }
        let p_save = widget_center(snap, T_SAVE)
        let p_bump = widget_center(snap, T_BUMP)

        // Park the cursor on Save and warm up the capture pipeline.
        move_to(app, p_save, 800)
        hold_content(app, 1800u)

        // ---- Beat 1: auto-emit + click_count ----
        hold_through_voice(app, say_begin(app, "auto-emit: SAVE_BTN.click_count", T_SAVE,
            [voice = "Every boost widget emits a daslang global named by its first argument - no declaration to keep in sync. I click Save twice, and SAVE_BTN.click_count reads two."]),
            [T_SAVE, T_SAVE])

        // ---- Beat 2: dotted flags steer the global (driven from outside) ----
        move_to(app, GUTTER, 600)
        hold_content(app, SETTLE_MS)
        let d2 = say_begin(app, "dotted flags: SPEED.PUBLIC / VOLUME.NOTLIVE", T_SPEED,
            [voice = "Dotted flags tune that global - SPEED is public so other modules can read it, Volume opts out of live-reload. I set both from outside, and the values update in place."])
        let anchor2 = record_frame_count(app)
        let lead2 = d2 > 1600u ? 700u : d2 / 3u
        hold_content(app, lead2)
        force_set_verified(app, T_SPEED, 7)
        hold_content(app, 700u)
        force_set_verified(app, T_VOLUME, 0.75f)
        hold_remainder_content(app, d2, anchor2)

        // ---- Beat 3: text_show mirrors an app-side string ----
        let d3 = say_begin(app, "text_show: driven from outside", T_STATUS,
            [voice = "text_show mirrors an app-side string into the snapshot. I drive STATUS_TEXT from outside, and it appears here."])
        let anchor3 = record_frame_count(app)
        let lead3 = d3 > 1400u ? 700u : d3 / 3u
        hold_content(app, lead3)
        force_set_verified(app, T_STATUS, "set from outside")
        hold_remainder_content(app, d3, anchor3)

        // ---- Beat 4: bump rewrites STATUS_TEXT from inside the app ----
        move_to(app, p_bump, 600)
        hold_content(app, SETTLE_MS)
        let before = write_json(snapshot(app)?["globals"]?[T_STATUS]?["payload"]?["value"])
        let d4 = say_begin(app, "bump: rewritten from inside", T_BUMP,
            [voice = "And the app can write it back from inside - clicking bump stamps STATUS_TEXT with the current frame."])
        let anchor4 = record_frame_count(app)
        let lead4 = d4 > 1400u ? 700u : d4 / 3u
        hold_content(app, lead4)
        click(app, T_BUMP)
        record_check_changed(app, T_STATUS, "value", before, 180)
        hold_remainder_content(app, d4, anchor4)
    }
}
