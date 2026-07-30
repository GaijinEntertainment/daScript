options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``visual_aids_tour.apng`` against
//! ``examples/tutorial/visual_aids_tour.das``. Voiced, self-verifying tour of
//! the visual-aid layer. Each beat speaks an ear-first ``voice`` line while a
//! REAL gesture fires under it and is VERIFIED (a no-op aborts the recording):
//!   1. highlight  - click each highlight button (rect flashes on the subject).
//!   2. trail+drag - scrub VOLUME; the trail marks the path, value must change.
//!   3. narrate    - click each narrate button (callout points at the widget).
//!   4. key HUD    - type into NAME_INPUT; focus rect + keycaps; value verified.
//!   5. chords     - Ctrl+A then Backspace; the buffer must clear.

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(b?["x"] ?? 0.0f, b?["y"] ?? 0.0f, b?["z"] ?? 0.0f, b?["w"] ?? 0.0f)
}

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let b = widget_bbox(snap, ident)
    return ((b.x + b.z) * 0.5f, (b.y + b.w) * 0.5f)
}

[export]
def main {
    with_recording_app("examples/tutorial/visual_aids_tour.das",
                       "visual_aids_tour.apng", 60) $(app) {
        let T_VOLUME = "SUBJECT_WIN/VOLUME"
        let T_NAME   = "SUBJECT_WIN/NAME_INPUT"
        let B_HI_ST  = "CONTROLS_WIN/BTN_HIGHLIGHT_STATUS"
        let B_HI_VOL = "CONTROLS_WIN/BTN_HIGHLIGHT_VOLUME"
        let B_HI_SAV = "CONTROLS_WIN/BTN_HIGHLIGHT_SAVE"
        let B_NR_VOL = "CONTROLS_WIN/BTN_NARRATE_VOLUME"
        let B_NR_SAV = "CONTROLS_WIN/BTN_NARRATE_SAVE"

        var snap = wait_for_render(app, B_HI_VOL, 10.0f)
        if (snap == null) {
            panic("{B_HI_VOL} never rendered - wrong app running?")
        }
        let p_hi_st  = widget_center(snap, B_HI_ST)
        let p_nr_vol = widget_center(snap, B_NR_VOL)
        let p_name   = widget_center(snap, T_NAME)
        let vol_bb = widget_bbox(snap, T_VOLUME)
        let vol_y  = (vol_bb.y + vol_bb.w) * 0.5f
        let vol_x0 = vol_bb.x + (vol_bb.z - vol_bb.x) * 0.18f
        let vol_x1 = vol_bb.x + (vol_bb.z - vol_bb.x) * 0.85f

        move_to(app, p_hi_st, 600)
        hold_content(app, SETTLE_MS)

        // ---- Beat 1: highlight - flash a rect around a widget ----
        hold_through_voice(app, say_begin(app, "highlight - point at a widget",
            [B_HI_ST, B_HI_VOL, B_HI_SAV],
            [voice = "The first aid is highlight. Click a control on the right, and a colored rectangle flashes around the matching widget on the left - status, then volume, then save."]),
            [B_HI_ST, B_HI_VOL, B_HI_SAV])

        // ---- Beat 2: mouse trail + real drag (cursor sprite visible throughout) ----
        move_to(app, (vol_x0, vol_y), 800)
        hold_content(app, SETTLE_MS)
        drag_through_voice(app, say_begin(app, "mouse trail + real drag", T_VOLUME,
            [voice = "As the cursor moves, a fading trail marks its path. Here it scrubs the volume slider for real, and the snapshot confirms the value changed."]),
            T_VOLUME, (vol_x1, vol_y))

        // ---- Beat 3: narrate - sticky-note callout with connector ----
        move_to(app, p_nr_vol, 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "narrate - sticky-note callout",
            [B_NR_VOL, B_NR_SAV],
            [voice = "Narrate pops a sticky note with a connector line to a widget. Clicking these two points a callout at the volume slider, then at the save button."]),
            [B_NR_VOL, B_NR_SAV])

        // ---- Beat 4: keyboard - key HUD + focus rect while typing ----
        post_command(app, "imgui_key_hud",    JV((enabled = true, show_modifiers = true)))
        post_command(app, "imgui_focus_rect", JV((enabled = true)))
        move_to(app, p_name, 800)
        hold_content(app, SETTLE_MS)
        type_into_voice(app, say_begin(app, "key HUD + focus rect", T_NAME,
            [voice = "For keyboard work, the focus rectangle shows where typing lands and the keycap HUD pops each key. Watch as we type a short greeting; the shift pill lights on the capitals."]),
            T_NAME, p_name, ["Hello, World!"], "Hello, World!")

        // ---- Beat 5: chords - Ctrl+A selects, Backspace clears (verified empty) ----
        let dwell5 = say_begin(app, "chords - Ctrl A then Backspace", T_NAME,
            [voice = "Chords work too. Control A selects everything - the modifier pill lights up - and Backspace clears the field."])
        let anchor5 = record_frame_count(app)
        let lead5 = dwell5 > 1200u ? 600u : dwell5 / 3u
        hold_content(app, lead5)
        click(app, T_NAME)                              // refocus the input (real click)
        hold_content(app, 300u)
        post_command(app, "imgui_key_chord", JV((mods = ["Ctrl"], key = "A")))
        hold_content(app, 500u)
        key_tap(app, int(ImGuiKey.Backspace))
        record_check_value(app, T_NAME, "value", "", 120)
        hold_remainder_content(app, dwell5, anchor5)

        post_command(app, "imgui_key_hud",    JV((enabled = false)))
        post_command(app, "imgui_focus_rect", JV((enabled = false)))
    }
}
