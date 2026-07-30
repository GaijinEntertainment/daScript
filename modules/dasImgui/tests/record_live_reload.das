options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``live_reload.apng`` against ``examples/tutorial/live_reload.das``.
//! The daslang-live surface - @live state and custom [live_command]s. The recording
//! can't trigger a reload, so it exercises the OBSERVABLE surface and verifies each:
//!   1. VOLUME    - @live slider, set from outside (force_set_verified).
//!   2. PING_BTN  - @live click_count, clicked twice (hold_through_voice).
//!   3. bump_counter - user [live_command] climbs the counter (CTR_TEXT -> "...= 7").
//!   4. reset_counter - user [live_command] zeroes it (CTR_TEXT -> "...= 0").

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let b = snap?["globals"]?[ident]?["bbox"]
    return (((b?["x"] ?? 0.0f) + (b?["z"] ?? 0.0f)) * 0.5f,
            ((b?["y"] ?? 0.0f) + (b?["w"] ?? 0.0f)) * 0.5f)
}

[export]
def main {
    with_recording_app("examples/tutorial/live_reload.das",
                       "live_reload.apng", 60) $(app) {
        let T_VOLUME = "LIVE_WIN/VOLUME"
        let T_PING   = "LIVE_WIN/PING_BTN"
        let T_CTR    = "LIVE_WIN/CTR_TEXT"
        let GUTTER   = (840.0f, 320.0f)   // park off the widgets while [live_command]s drive from outside

        var snap = wait_for_render(app, T_VOLUME, 10.0f)
        if (snap == null) {
            panic("{T_VOLUME} never rendered - wrong app running?")
        }
        let p_volume = widget_center(snap, T_VOLUME)
        let p_ping   = widget_center(snap, T_PING)

        // Park the cursor on the slider and warm up the capture pipeline.
        move_to(app, p_volume, 800)
        hold_content(app, 1800u)

        // ---- Beat 1: @live slider, set from outside ----
        let d1 = say_begin(app, "VOLUME: @live, set from outside", T_VOLUME,
            [voice = "Live reload preserves @live state across an edit. Volume is an @live slider - I set it from outside, and the value would carry across a reload."])
        let anchor1 = record_frame_count(app)
        let lead1 = d1 > 1500u ? 700u : d1 / 3u
        hold_content(app, lead1)
        force_set_verified(app, T_VOLUME, 0.65f)
        hold_remainder_content(app, d1, anchor1)

        // ---- Beat 2: @live click_count ----
        move_to(app, p_ping, 600)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "PING_BTN: @live click_count", T_PING,
            [voice = "Click counts are @live too. I ping twice, and PING_BTN.click_count holds at two - it would survive a reload the same way."]),
            [T_PING, T_PING])

        // ---- Beat 3: custom [live_command] climbs the counter ----
        move_to(app, GUTTER, 600)
        hold_content(app, SETTLE_MS)
        let d3 = say_begin(app, "bump_counter: custom [live_command]", T_CTR,
            [voice = "bump_counter is a user-defined live command - an HTTP endpoint the running app exposes. I call it from outside, and the counter climbs."])
        let anchor3 = record_frame_count(app)
        let lead3 = d3 > 1500u ? 700u : d3 / 3u
        hold_content(app, lead3)
        post_command(app, "bump_counter", JV((by = 1)))
        hold_content(app, 700u)
        post_command(app, "bump_counter", JV((by = 6)))
        record_check_value(app, T_CTR, "value", "g_custom_counter = 7", 180)
        hold_remainder_content(app, d3, anchor3)

        // ---- Beat 4: another [live_command] zeroes it ----
        let d4 = say_begin(app, "reset_counter: same surface", T_CTR,
            [voice = "And reset_counter, another custom command on the same surface, sets it back to zero."])
        let anchor4 = record_frame_count(app)
        let lead4 = d4 > 1400u ? 700u : d4 / 3u
        hold_content(app, lead4)
        post_command(app, "reset_counter", null)
        record_check_value(app, T_CTR, "value", "g_custom_counter = 0", 180)
        hold_remainder_content(app, d4, anchor4)
    }
}
