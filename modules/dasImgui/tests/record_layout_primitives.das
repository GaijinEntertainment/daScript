options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``layout_primitives.apng`` against ``examples/tutorial/layout_primitives.das``.
//! The cursor-moving markers (same_line / spacing / new_line / dummy) render nothing,
//! so the self-check is that each NAMED marker fired - it registers an `empty_marker`
//! entry even though its payload is `{}`. Three voiced beats:
//!   1. same_line - three buttons share one row; BTN_A/B/C + SL_AB/SL_BC all on screen.
//!   2. spacing   - the vertical-gap marker fired (SP_TOP rendered).
//!   3. new_line + dummy - new_line fired (NL_INSERT rendered); dummy is anonymous.

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let b = snap?["globals"]?[ident]?["bbox"]
    return (((b?["x"] ?? 0.0f) + (b?["z"] ?? 0.0f)) * 0.5f,
            ((b?["y"] ?? 0.0f) + (b?["w"] ?? 0.0f)) * 0.5f)
}

[export]
def main {
    with_recording_app("examples/tutorial/layout_primitives.das",
                       "layout_primitives.apng", 45) $(app) {
        let T_BTN_A = "LAYOUT_WIN/BTN_A"
        let T_BTN_B = "LAYOUT_WIN/BTN_B"
        let T_BTN_C = "LAYOUT_WIN/BTN_C"
        let T_SL_AB = "LAYOUT_WIN/SL_AB"
        let T_SL_BC = "LAYOUT_WIN/SL_BC"
        let T_SP_TOP = "LAYOUT_WIN/SP_TOP"
        let T_NL = "LAYOUT_WIN/NL_INSERT"

        var snap = wait_for_render(app, T_BTN_A, 10.0f)
        if (snap == null) {
            panic("{T_BTN_A} never rendered - wrong app running?")
        }
        let p_a = widget_center(snap, T_BTN_A)
        let p_b = widget_center(snap, T_BTN_B)
        let p_c = widget_center(snap, T_BTN_C)
        let p_sp = widget_center(snap, T_SP_TOP)
        let p_nl = widget_center(snap, T_NL)

        // Park the cursor on button A and warm up the capture pipeline.
        move_to(app, p_a, 800)
        hold_content(app, 1800u)

        // ---- Beat 1: same_line - one row ----
        record_check_rendered(app, T_BTN_A, true, 180)
        record_check_rendered(app, T_BTN_B, true, 180)
        record_check_rendered(app, T_BTN_C, true, 180)
        record_check_rendered(app, T_SL_AB, true, 180)
        record_check_rendered(app, T_SL_BC, true, 180)
        let d1 = say_begin(app, "same_line: one row", T_BTN_A,
            [voice = "same_line keeps the next widget on the current row. Here three buttons - A, B, and C - sit on one strip, because each is followed by a same_line call."])
        let anchor1 = record_frame_count(app)
        let step = d1 > 1500u ? d1 / 3u : 500u
        hold_content(app, step)
        move_to(app, p_b, 350)
        hold_content(app, step)
        move_to(app, p_c, 350)
        hold_remainder_content(app, d1, anchor1)

        // ---- Beat 2: spacing - vertical gap ----
        move_to(app, p_sp, 600)
        hold_content(app, SETTLE_MS)
        record_check_rendered(app, T_SP_TOP, true, 180)
        say(app, "spacing: vertical gap", T_SP_TOP,
            [voice = "spacing inserts a single line of vertical gap. Stack a few when you want a little more breathing room without a fixed-size dummy."])

        // ---- Beat 3: new_line + dummy - reserve space ----
        move_to(app, p_nl, 600)
        hold_content(app, SETTLE_MS)
        record_check_rendered(app, T_NL, true, 180)
        say(app, "new_line + dummy: reserve space", T_NL,
            [voice = "new_line advances a full text line, and dummy reserves an arbitrary rectangle - both move the cursor without drawing anything."])
    }
}
