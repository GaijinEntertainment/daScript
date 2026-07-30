options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record edit_external_tour.apng against examples/tutorial/editing_external.das.
//! The edit_* family edits caller-owned data through a `T?` pointer (safe_addr), so the proof of
//! the rail is that a REAL gesture on the widget writes straight through to a `var private` global
//! (printed live in the readout). Every stage is driven the human way and self-verified -- the
//! recording doubles as a test; a no-op aborts at teardown. NOTHING is force_set (the now-existing
//! drag/type/combo rails make every shape drivable, so this is no longer a force_set scene):
//!   edit_slider_float  -> drag the handle           (value moved)
//!   edit_input_int     -> click, type "42", Enter   (value == 42)
//!   edit_checkbox      -> click                      (bool flips)
//!   edit_drag_float3   -> drag the first field       (float3 changed; a Drag scrubs, never types)
//!   edit_color_edit3   -> drag the red channel       (float3 changed; swatch shifts)
//!   edit_combo         -> open + click item 2        (index == 2)
//!   edit_slider_angle  -> drag the handle            (radian value moved)

def bbox_of(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(b?["x"] ?? 0.0f, b?["y"] ?? 0.0f, b?["z"] ?? 0.0f, b?["w"] ?? 0.0f)
}

// Point at fractional (fx, fy) inside a bbox -- rides the widget's real rect instead of hard pixels.
def frac_point(bb : float4; fx : float; fy : float) : tuple<float; float> {
    return (bb.x + (bb.z - bb.x) * fx, bb.y + (bb.w - bb.y) * fy)
}

// Click point inside an input's editable text field -- left of the trailing +/- step buttons.
def field_click(bb : float4) : tuple<float; float> {
    return (bb.x + 42.0f, (bb.y + bb.w) * 0.5f)
}

[export]
def main {
    with_recording_app("examples/tutorial/editing_external.das",
                       "edit_external_tour.apng", 90) $(app) {
        let T_VOL     = "EDITOR_WIN/VOL_SLIDER"
        let T_COUNT   = "EDITOR_WIN/COUNT_INPUT"
        let T_ENABLED = "EDITOR_WIN/ENABLED_CHECK"
        let T_POS     = "EDITOR_WIN/POS_DRAG"
        let T_COLOR   = "EDITOR_WIN/ACCENT_COLOR"
        let T_COMBO   = "EDITOR_WIN/QUALITY_COMBO"
        let T_ANGLE   = "EDITOR_WIN/ANGLE_SLIDER"

        var snap = wait_for_render(app, T_VOL, 10.0f)
        if (snap == null) {
            panic("{T_VOL} never rendered - wrong app running?")
        }

        // Type stages pop each synth key as a fading keycap so the viewer sees the keystrokes.
        post_command(app, "imgui_key_hud", JV((enabled = true, show_modifiers = false)))

        // ---- Intro: the rail (static, no action) ----
        say(app, "edit_* - widgets that edit YOUR data", T_VOL,
            [voice = "Most widgets keep their own state. The edit family is different. Each one edits a variable you already have, reaching it through a pointer you pass with safe address. Watch every gesture write straight through to my own variables in the readout below."])

        // ---- Stage 1: edit_slider_float - drag the handle ----
        let bb1 = bbox_of(snap, T_VOL)
        let cy1 = (bb1.y + bb1.w) * 0.5f
        move_to(app, (bb1.x + 0.50f * (bb1.z - bb1.x), cy1), 600)   // onto the handle (value 0.5 -> mid)
        hold_content(app, SETTLE_MS)
        let d1 = say_begin(app, "edit_slider_float - drag, value follows", T_VOL,
            [voice = "Drag the volume slider, and the floating point variable behind it moves with the handle. The widget is writing through the pointer, not into a copy of its own."])
        drag_through_voice(app, d1, T_VOL, (bb1.x + 0.85f * (bb1.z - bb1.x), cy1))

        // ---- Stage 2: edit_input_int - click, type, Enter ----
        let p2 = field_click(bbox_of(snap, T_COUNT))
        move_to(app, p2, 700)
        hold_content(app, SETTLE_MS)
        let d2 = say_begin(app, "edit_input_int - type your int", T_COUNT,
            [voice = "Click the count field and type a new number. It commits straight into my integer variable, the same one printed below."])
        type_into_voice(app, d2, T_COUNT, p2, ["42"], 42)

        // ---- Stage 3: edit_checkbox - click flips the bool ----
        move_to(app, widget_click_point(snap, T_ENABLED), 700)
        hold_content(app, SETTLE_MS)
        let d3 = say_begin(app, "edit_checkbox - flip your bool", T_ENABLED,
            [voice = "The checkbox edits a plain boolean. Click it and the value flips from true to false in place."])
        hold_through_voice(app, d3, [T_ENABLED])

        // ---- Stage 4: edit_drag_float3 - drag the first field (a Drag scrubs; click would NOT type) ----
        let bb4 = bbox_of(snap, T_POS)
        move_to(app, frac_point(bb4, 0.08f, 0.5f), 700)
        hold_content(app, SETTLE_MS)
        let d4 = say_begin(app, "edit_drag_float3 - scrub one axis", T_POS,
            [voice = "A vector editor binds to all three components of my position. Drag the first field and only that axis scrubs, while the other two hold steady."])
        drag_through_voice(app, d4, T_POS, frac_point(bb4, 0.22f, 0.5f))

        // ---- Stage 5: edit_color_edit3 - drag the red channel ----
        let bb5 = bbox_of(snap, T_COLOR)
        move_to(app, frac_point(bb5, 0.08f, 0.5f), 700)
        hold_content(app, SETTLE_MS)
        let d5 = say_begin(app, "edit_color_edit3 - scrub a channel", T_COLOR,
            [voice = "The color editor binds to a color of mine. Drag the red channel up and the swatch shifts toward red as the value climbs."])
        drag_through_voice(app, d5, T_COLOR, frac_point(bb5, 0.22f, 0.5f))

        // ---- Stage 6: edit_combo - open and pick item 2 ----
        move_to(app, frac_point(bbox_of(snap, T_COMBO), 0.5f, 0.5f), 700)
        hold_content(app, SETTLE_MS)
        let d6 = say_begin(app, "edit_combo - pick an index", T_COMBO,
            [voice = "The combo writes an index. Open it and choose a quality preset, and my index variable jumps to the row you picked."])
        combo_pick_voice(app, d6, T_COMBO, 2)              // High

        // ---- Stage 7: edit_slider_angle - drag the handle ----
        let bb7 = bbox_of(snap, T_ANGLE)
        let cy7 = (bb7.y + bb7.w) * 0.5f
        move_to(app, (bb7.x + 0.50f * (bb7.z - bb7.x), cy7), 700)   // onto the handle (0 rad -> mid)
        hold_content(app, SETTLE_MS)
        let d7 = say_begin(app, "edit_slider_angle - degrees over radians", T_ANGLE,
            [voice = "Last, an angle slider. It shows degrees on the face, but the variable underneath is in radians. Drag it and the radian value swings along."])
        drag_through_voice(app, d7, T_ANGLE, (bb7.x + 0.78f * (bb7.z - bb7.x), cy7))

        post_command(app, "imgui_key_hud", JV((enabled = false)))
    }
}
