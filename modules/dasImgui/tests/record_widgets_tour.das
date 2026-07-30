options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``widgets_tour.apng`` against
//! ``examples/tutorial/widgets_tour.das``. Voiced, self-verifying tour of one
//! of every common boost widget on an audio-settings panel. Each stage speaks
//! an ear-first ``voice`` line while a REAL gesture fires under it and is
//! VERIFIED (a no-op aborts the recording at teardown):
//!   1. input_text  - type a name; committed value verified.
//!   2. slider_float - drag the volume; value must change.
//!   3. checkbox    - click mute; the toggle must register.
//!   4. combo       - open the dropdown and pick High; selection verified.
//!   5. color_edit3 - set the accent from the API (force_set_verified).
//!   6. button      - click Save; the click must register.

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
    with_recording_app("examples/tutorial/widgets_tour.das",
                       "widgets_tour.apng", 90) $(app) {
        let T_NAME    = "AUDIO_WIN/USER_NAME"
        let T_VOL     = "AUDIO_WIN/VOLUME"
        let T_MUTED   = "AUDIO_WIN/MUTED"
        let T_QUALITY = "AUDIO_WIN/QUALITY"
        let T_TINT    = "AUDIO_WIN/TINT"
        let T_SAVE    = "AUDIO_WIN/SAVE_BTN"

        var snap = wait_for_render(app, T_SAVE, 10.0f)
        if (snap == null) {
            panic("{T_SAVE} never rendered - wrong app running?")
        }
        let p_name    = widget_center(snap, T_NAME)
        let p_muted   = widget_center(snap, T_MUTED)
        let p_quality = widget_center(snap, T_QUALITY)
        let p_save    = widget_center(snap, T_SAVE)
        let vol_bb = widget_bbox(snap, T_VOL)
        let vol_y  = (vol_bb.y + vol_bb.w) * 0.5f
        let vol_x0 = vol_bb.x + (vol_bb.z - vol_bb.x) * 0.15f
        let vol_x1 = vol_bb.x + (vol_bb.z - vol_bb.x) * 0.80f

        move_to(app, p_name, 600)
        hold_content(app, SETTLE_MS)

        // ---- Stage 1: input_text - type a name ----
        type_into_voice(app, say_begin(app, "input_text - type a name", T_NAME,
            [voice = "First, a text field. Click in and type a name; the buffer mirror below updates as each character lands."]),
            T_NAME, p_name, ["Alice"], "Alice")

        // ---- Stage 2: slider_float - drag to set volume ----
        move_to(app, (vol_x0, vol_y), 800)
        hold_content(app, SETTLE_MS)
        drag_through_voice(app, say_begin(app, "slider_float - drag to set", T_VOL,
            [voice = "Next, a slider. Drag the handle to set the master volume; the value follows the cursor."]),
            T_VOL, (vol_x1, vol_y))

        // ---- Stage 3: checkbox - toggle mute ----
        move_to(app, p_muted, 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "checkbox - toggle mute", T_MUTED,
            [voice = "A checkbox flips a boolean. One click mutes, and the tick mark appears."]),
            [T_MUTED])

        // ---- Stage 4: combo - open dropdown and pick a preset ----
        move_to(app, p_quality, 800)
        hold_content(app, SETTLE_MS)
        combo_pick_voice(app, say_begin(app, "combo - pick a preset", T_QUALITY,
            [voice = "A combo box opens a dropdown. We open it and pick the high quality preset; the box shows the new value."]),
            T_QUALITY, 2)

        // ---- Stage 5: color_edit3 - set accent from the API ----
        let dwell5 = say_begin(app, "color_edit3 - set accent", T_TINT,
            [voice = "The color editor holds three channels. Here we set a green accent from the API; clicking the swatch would open a full picker."])
        let anchor5 = record_frame_count(app)
        let lead5 = dwell5 > 1200u ? 600u : dwell5 / 3u
        hold_content(app, lead5)
        force_set_verified(app, T_TINT, float3(0.25f, 0.75f, 0.45f))
        hold_remainder_content(app, dwell5, anchor5)

        // ---- Stage 6: button - save ----
        move_to(app, p_save, 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "button - save settings", T_SAVE,
            [voice = "Finally, a button. Click Save, and the counter at the bottom ticks up by one."]),
            [T_SAVE])
    }
}
