options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record drag.apng against examples/tutorial/drag.das.
//! Drag forms, each driven the human way (a REAL press-and-scrub, never force_set) and
//! self-verified — the recording doubles as a test:
//!   drag_float        -> press inside, scrub right
//!   drag_int          -> same feel, whole steps
//!   drag_float3       -> scrub the first handle of the row
//!   drag_float_range2 -> open the band (high handle), then drag the low into the lo<=hi clamp
//! Drag widgets scrub by relative motion (no fixed track), so each press lands inside the
//! frame and the scrub stays in the box; drag_through_voice asserts the value actually MOVED
//! (a no-op drag aborts the recording at teardown).

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(b?["x"] ?? 0.0f, b?["y"] ?? 0.0f, b?["z"] ?? 0.0f, b?["w"] ?? 0.0f)
}

[export]
def main {
    with_recording_app("examples/tutorial/drag.das",
                       "drag.apng", 60) $(app) {
        let T_FLOAT = "DRAG_WIN/D_FLOAT"
        let T_INT   = "DRAG_WIN/D_INT"
        let T_VEC3  = "DRAG_WIN/D_VEC3"
        let T_RANGE = "DRAG_WIN/D_RANGE"

        var snap = wait_for_render(app, T_FLOAT, 10.0f)
        if (snap == null) {
            panic("{T_FLOAT} never rendered - wrong app running?")
        }

        // ---- Stage 1: scalar float — press inside the frame and scrub right ----
        let bbF = widget_bbox(snap, T_FLOAT)
        let cyF = (bbF.y + bbF.w) * 0.5f
        move_to(app, (bbF.x + 70.0f, cyF), 700)             // inside the frame, left of center
        hold_content(app, SETTLE_MS)
        let d1 = say_begin(app, "drag_float - scrub a number", T_FLOAT,
            [voice = "A drag widget edits a number right in place. Press inside and scrub sideways, and the value follows your motion, faster or slower with the speed you set."])
        drag_through_voice(app, d1, T_FLOAT, (bbF.x + 210.0f, cyF))

        // ---- Stage 2: scalar int — same feel, whole steps ----
        let bbI = widget_bbox(snap, T_INT)
        let cyI = (bbI.y + bbI.w) * 0.5f
        move_to(app, (bbI.x + 70.0f, cyI), 700)
        hold_content(app, SETTLE_MS)
        let d2 = say_begin(app, "drag_int - whole steps", T_INT,
            [voice = "The integer drag has the very same feel. Scrub it, and the value lands on whole steps, one point per pixel, kept inside its bounds."])
        drag_through_voice(app, d2, T_INT, (bbI.x + 155.0f, cyI))

        // ---- Stage 3: vector float3 — scrub the FIRST handle of the row ----
        // widget_component_point reads sub_bboxes[0] (component 0's own rect), so we target it
        // deterministically instead of guessing an x inside the combined widget.
        let c0 = widget_component_point(snap, T_VEC3, 0)
        move_to(app, c0, 700)
        hold_content(app, SETTLE_MS)
        let d3 = say_begin(app, "drag_float3 - one handle per axis", T_VEC3,
            [voice = "A vector drag is three numbers in a row, one handle each. Scrub the first, and only that component moves while the others hold still."])
        drag_through_voice(app, d3, T_VEC3, (c0._0 + 55.0f, c0._1))

        // ---- Stage 4a: range — open the band by dragging the HIGH handle outward ----
        // sub_bboxes[1] is the high handle's rect, [0] the low's.
        let hi = widget_component_point(snap, T_RANGE, 1)
        move_to(app, hi, 900)
        hold_content(app, SETTLE_MS)
        let d4 = say_begin(app, "drag_float_range2 - open the band", T_RANGE,
            [voice = "A range drag has a low and a high handle on one bar. Drag the right one outward, and the high end rises while the low stays put."])
        drag_through_voice(app, d4, T_RANGE, (hi._0 + 100.0f, hi._1))

        // ---- Stage 4b: range — drag the LOW handle until it bumps the high (lo <= hi clamp) ----
        let lo = widget_component_point(snap, T_RANGE, 0)
        move_to(app, lo, 700)
        hold_content(app, SETTLE_MS)
        let d5 = say_begin(app, "low cannot pass high", T_RANGE,
            [voice = "Now drag the left handle to the right. It climbs until it meets the high end and stops, because the low can never cross the high."])
        drag_through_voice(app, d5, T_RANGE, (lo._0 + 130.0f, lo._1))
    }
}
