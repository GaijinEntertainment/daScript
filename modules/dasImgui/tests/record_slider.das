options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record slider.apng against examples/tutorial/slider.das.
//! Four slider forms, each driven the human way (a REAL drag of the handle, never force_set)
//! and self-verified — the recording doubles as a test:
//!   slider_float  -> grab the handle, scrub right
//!   slider_int    -> same, integer track
//!   slider_float3 -> sweep the first handle across its sub-track
//!   vslider_float -> drag the handle upward
//! Each drag starts at the cursor (move_to put it on the handle) and ends inside the track, so
//! the scrub stays in the box and reads at a natural speed; drag_through_voice asserts the value
//! actually MOVED (a no-op drag aborts the recording at teardown).

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(b?["x"] ?? 0.0f, b?["y"] ?? 0.0f, b?["z"] ?? 0.0f, b?["w"] ?? 0.0f)
}

def sub_bbox(var snap : JsonValue?; ident : string; component : int) : float4 {
    var b = snap?["globals"]?[ident]?["sub_bboxes"]?[component]
    return float4(b?["x"] ?? 0.0f, b?["y"] ?? 0.0f, b?["z"] ?? 0.0f, b?["w"] ?? 0.0f)
}

[export]
def main {
    with_recording_app("examples/tutorial/slider.das",
                       "slider.apng", 60) $(app) {
        let T_FLOAT = "SLIDER_WIN/S_FLOAT"
        let T_INT   = "SLIDER_WIN/S_INT"
        let T_VEC3  = "SLIDER_WIN/S_VEC3"
        let T_VMID  = "SLIDER_WIN/V_MID"

        var snap = wait_for_render(app, T_FLOAT, 10.0f)
        if (snap == null) {
            panic("{T_FLOAT} never rendered - wrong app running?")
        }

        // ---- Stage 1: scalar float — grab the handle and scrub right ----
        let bbF = widget_bbox(snap, T_FLOAT)
        let cyF = (bbF.y + bbF.w) * 0.5f
        move_to(app, (bbF.x + 14.0f, cyF), 700)            // onto the handle (value 0 -> left end)
        hold_content(app, SETTLE_MS)
        let d1 = say_begin(app, "slider_float - drag the handle", T_FLOAT,
            [voice = "A slider edits a bounded number along a fixed track. Grab the handle and scrub it, and the value moves with you between the low and high bounds."])
        drag_through_voice(app, d1, T_FLOAT, (bbF.x + 0.72f * (bbF.z - bbF.x), cyF))

        // ---- Stage 2: scalar int — same shape, whole steps ----
        let bbI = widget_bbox(snap, T_INT)
        let cyI = (bbI.y + bbI.w) * 0.5f
        move_to(app, (bbI.x + 14.0f, cyI), 700)
        hold_content(app, SETTLE_MS)
        let d2 = say_begin(app, "slider_int - integer steps", T_INT,
            [voice = "The integer slider has the very same shape, but it snaps to whole steps. Its format string adds a percent sign, so the label reads as a percentage while you drag."])
        drag_through_voice(app, d2, T_INT, (bbI.x + 0.72f * (bbI.z - bbI.x), cyI))

        // ---- Stage 3: vector float3 — sweep the FIRST handle across its sub-track ----
        // sub_bboxes[0] is component 0's own rect (telemetry), so we target it deterministically
        // instead of guessing an x inside the combined widget.
        let s0 = sub_bbox(snap, T_VEC3, 0)
        let cyV = (s0.y + s0.w) * 0.5f
        move_to(app, widget_component_point(snap, T_VEC3, 0), 700)   // onto the first handle (value 0 -> sub-track center)
        hold_content(app, SETTLE_MS)
        let d3 = say_begin(app, "slider_float3 - one handle per axis", T_VEC3,
            [voice = "A vector slider lays three handles side by side, one per component. Sweep the first handle along its track and that component changes, while the other two stay where they were."])
        drag_through_voice(app, d3, T_VEC3, (s0.x + 0.85f * (s0.z - s0.x), cyV))

        // ---- Stage 4: vertical orientation — drag the handle upward ----
        let bbM = widget_bbox(snap, T_VMID)
        let cxM = (bbM.x + bbM.z) * 0.5f
        move_to(app, (cxM, bbM.w - 12.0f), 900)            // onto the handle (value 0 -> bottom)
        hold_content(app, SETTLE_MS)
        let d4 = say_begin(app, "vslider_float - vertical track", T_VMID,
            [voice = "The vertical slider shares the exact same state, just drawn from bottom to top. Drag it upward and the value climbs."])
        drag_through_voice(app, d4, T_VMID, (cxM, bbM.y + 0.20f * (bbM.w - bbM.y)))
    }
}
