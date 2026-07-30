options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record dynamic_fonts.apng against examples/tutorial/dynamic_fonts.das.
//! imgui 1.92 dynamic-font scaling, two levers:
//!   with_font(f, 16/24/32) -> three static rows, one font at three pixel sizes (narrated).
//!   GLOBAL_SCALE edit_slider -> driven the human way (a REAL drag, never force_set): scrubbing
//!     it writes g_global_scale, which the frame feeds to GetStyle().FontScaleMain, so the WHOLE
//!     window visibly rescales as the handle moves. Self-verified by the slider value MOVING
//!     (drag_through_voice -> record_check_changed); a no-op drag aborts the recording at teardown.

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(b?["x"] ?? 0.0f, b?["y"] ?? 0.0f, b?["z"] ?? 0.0f, b?["w"] ?? 0.0f)
}

[export]
def main {
    with_recording_app("examples/tutorial/dynamic_fonts.das",
                       "dynamic_fonts.apng", 45) $(app) {
        let T_SCALE = "DF_WIN/GLOBAL_SCALE"
        let T_16    = "DF_WIN/SIZE_16"
        let T_24    = "DF_WIN/SIZE_24"
        let T_32    = "DF_WIN/SIZE_32"

        var snap = wait_for_render(app, T_SCALE, 10.0f)
        if (snap == null) {
            panic("{T_SCALE} never rendered - wrong app running?")
        }

        // ---- Stage 1: per-push size -- one font, three sizes (static rows) ----
        say(app, "with_font(f, size) - one font, three sizes", [T_16, T_24, T_32],
            [voice = "In imgui one nine two you can ask for the same font at any pixel size. These three rows are one font drawn at sixteen, twenty four, and thirty two pixels, each rasterized crisply on demand."])

        // ---- Stage 2: FontScaleMain -- drag the slider, the whole frame rescales ----
        // GLOBAL_SCALE runs 0.5..2.5, starts at 1.0 (frac 0.25). Grab the handle and scrub right;
        // the value drives GetStyle().FontScaleMain, so every glyph this frame grows with the drag.
        let bb = widget_bbox(snap, T_SCALE)
        let cy = (bb.y + bb.w) * 0.5f
        move_to(app, (bb.x + 0.25f * (bb.z - bb.x), cy), 700)
        hold_content(app, SETTLE_MS)
        let d = say_begin(app, "FontScaleMain - global scale", T_SCALE,
            [voice = "This slider feeds the global font scale, Font Scale Main. As I drag it, every glyph in the whole window is re-rasterized larger - sharp at any scale, never a stretched bitmap."])
        drag_through_voice(app, d, T_SCALE, (bb.x + 0.78f * (bb.z - bb.x), cy))
    }
}
