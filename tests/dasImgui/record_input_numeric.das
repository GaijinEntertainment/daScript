options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record input_numeric.apng against examples/tutorial/input_numeric.das.
//! Numeric-input forms, each driven the human way (CLICK to focus, TYPE on the keyboard, Enter to
//! commit — never force_set) and self-verified — the recording doubles as a test:
//!   input_float  -> click, type "12.5", Enter
//!   input_int    -> click, type "42", Enter
//!   input_float3 -> click field 0, type + Tab across all three components, Enter commits the row
//!   input_double -> click, type a high-precision value, Enter
//! type_into_voice asserts the committed value equals what was typed (a no-op / mis-parse aborts the
//! recording at teardown). The text field is the LEFT chunk of the item (the +/- step buttons + label
//! sit to its right), so each click lands at x = bbox.x + a small inset.

def field_click(var snap : JsonValue?; ident : string) : tuple<float; float> {
    var b = snap?["globals"]?[ident]?["bbox"]
    let x = b?["x"] ?? 0.0f
    let y = b?["y"] ?? 0.0f
    let w = b?["w"] ?? 0.0f
    return (x + 42.0f, (y + w) * 0.5f)               // inside the editable text field, left of the buttons
}

[export]
def main {
    with_recording_app("examples/tutorial/input_numeric.das",
                       "input_numeric.apng", 60) $(app) {
        let T_FLOAT  = "IN_WIN/I_FLOAT"
        let T_INT    = "IN_WIN/I_INT"
        let T_VEC3   = "IN_WIN/I_VEC3"
        let T_DOUBLE = "IN_WIN/I_DOUBLE"

        var snap = wait_for_render(app, T_FLOAT, 10.0f)
        if (snap == null) {
            panic("{T_FLOAT} never rendered - wrong app running?")
        }

        // This tutorial is all keyboard input — pop each synth key as a fading
        // keycap at bottom-center so the viewer sees exactly what is typed.
        post_command(app, "imgui_key_hud", JV((enabled = true, show_modifiers = false)))

        // ---- Stage 1: scalar float — click, type, Enter ----
        let pF = field_click(snap, T_FLOAT)
        move_to(app, pF, 700)
        hold_content(app, SETTLE_MS)
        let d1 = say_begin(app, "input_float - type a number", T_FLOAT,
            [voice = "A numeric input takes the value straight from your keyboard. Click to focus the field, type a number, and press Enter to commit it."])
        type_into_voice(app, d1, T_FLOAT, pF, ["12.5"], 12.5f)

        // ---- Stage 2: scalar int — same gesture, whole numbers ----
        let pI = field_click(snap, T_INT)
        move_to(app, pI, 700)
        hold_content(app, SETTLE_MS)
        let d2 = say_begin(app, "input_int - whole numbers", T_INT,
            [voice = "The integer input works exactly the same way, only it keeps whole numbers. The plus and minus buttons step it up or down by one."])
        type_into_voice(app, d2, T_INT, pI, ["42"], 42)

        // ---- Stage 3: vector float3 — type field 0, Tab across, Enter commits the row ----
        let pV = field_click(snap, T_VEC3)
        move_to(app, pV, 700)
        hold_content(app, SETTLE_MS)
        let d3 = say_begin(app, "input_float3 - Tab between fields", T_VEC3,
            [voice = "A vector input gives each component its own field. Type the first, press Tab to jump to the next, and Enter commits the whole row at once."])
        type_into_voice(app, d3, T_VEC3, pV, ["1.5", "2.5", "3.5"], float3(1.5f, 2.5f, 3.5f))

        // ---- Stage 4: double precision — same shape, more digits ----
        let pD = field_click(snap, T_DOUBLE)
        move_to(app, pD, 700)
        hold_content(app, SETTLE_MS)
        let d4 = say_begin(app, "input_double - double precision", T_DOUBLE,
            [voice = "The double input has the very same shape at double precision, with room for many more digits, handy for timestamps or coordinates."])
        type_into_voice(app, d4, T_DOUBLE, pD, ["1234567.5"], 1234567.5lf)
    }
}
