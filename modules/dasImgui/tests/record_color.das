options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record color.apng against examples/tutorial/color.das.
//! Drives the color family the human way and self-verifies — the recording doubles as a test.
//! Every value change is a REAL synthetic drag (never force_set): the inline editors are scrubbed
//! by dragging a channel field, the picker by dragging inside its square, and the swatch is a real
//! click. Each gesture asserts the value moved / the counter ticked, so a dud aborts at teardown:
//!   color_edit3   -> drag the red channel; the swatch turns red
//!   color_edit4   -> drag the alpha channel; the 4-form's extra channel
//!   color_picker4 -> drag inside the square to set the shade
//!   color_button  -> two real clicks; click_count ticks 0,1,2

def bbox_of(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(b?["x"] ?? 0.0f, b?["y"] ?? 0.0f, b?["z"] ?? 0.0f, b?["w"] ?? 0.0f)
}

// Point at fractional (fx, fy) inside a bbox — lets the channel / square drag targets ride the
// widget's real rect instead of hard-coded pixels (the layout is fixed but this stays honest).
def frac_point(bb : float4; fx : float; fy : float) : tuple<float; float> {
    return (bb.x + (bb.z - bb.x) * fx, bb.y + (bb.w - bb.y) * fy)
}

[export]
def main {
    with_recording_app("examples/tutorial/color.das",
                       "color.apng", 45) $(app) {
        let T_EDIT3  = "CO_WIN/CO_EDIT3"
        let T_EDIT4  = "CO_WIN/CO_EDIT4"
        let T_PICKER = "CO_WIN/CO_PICKER"
        let T_SWATCH = "CO_WIN/CO_SWATCH"

        var snap = wait_for_render(app, T_EDIT3, 10.0f)
        if (snap == null) {
            panic("{T_EDIT3} never rendered - wrong app running?")
        }

        // ---- Stage 1: color_edit3 — drag the red channel ----
        var bb = bbox_of(snap, T_EDIT3)
        move_to(app, frac_point(bb, 0.08f, 0.5f), 500)
        hold_content(app, SETTLE_MS)
        let d1 = say_begin(app, "color_edit3 - inline RGB", T_EDIT3,
            [voice = "This is a color editor. It is a compact row: a small swatch on the left, then the red, green and blue channels. Drag a channel to scrub its value, or click the swatch for the full popup picker."])
        drag_through_voice(app, d1, T_EDIT3, frac_point(bb, 0.22f, 0.5f))

        // ---- Stage 2: color_edit4 — drag the alpha channel ----
        snap = snapshot(app)
        bb = bbox_of(snap, T_EDIT4)
        move_to(app, frac_point(bb, 0.62f, 0.5f), 700)
        hold_content(app, SETTLE_MS)
        let d2 = say_begin(app, "color_edit4 - adds alpha", T_EDIT4,
            [voice = "The four version adds one more channel on the end: alpha, which is transparency. Drag the alpha channel and the color shifts from see-through toward solid."])
        drag_through_voice(app, d2, T_EDIT4, frac_point(bb, 0.74f, 0.5f))

        // ---- Stage 3: color_picker4 — drag inside the square ----
        snap = snapshot(app)
        bb = bbox_of(snap, T_PICKER)
        move_to(app, frac_point(bb, 0.28f, 0.62f), 700)
        hold_content(app, SETTLE_MS)
        let d3 = say_begin(app, "color_picker4 - square and hue bar", T_PICKER,
            [voice = "When picking a color is the whole point of the panel, reach for the picker. The big square and the hue bar stay open the whole time. Drag inside the square to set the shade. The swatch on the right remembers where you started."])
        drag_through_voice(app, d3, T_PICKER, frac_point(bb, 0.60f, 0.20f))

        // ---- Stage 4: color_button — real clicks, counter asserted ----
        snap = snapshot(app)
        move_to(app, frac_point(bbox_of(snap, T_SWATCH), 0.5f, 0.5f), 700)
        hold_content(app, SETTLE_MS)
        let d4 = say_begin(app, "color_button - click trigger", T_SWATCH,
            [voice = "A color button is just a swatch you can click. It does not store a color of its own; you hand it one, and it simply counts your clicks. Use it for a palette cell, or to open your own picker."])
        hold_through_voice(app, d4, [T_SWATCH, T_SWATCH])
    }
}
