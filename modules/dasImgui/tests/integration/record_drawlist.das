options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``drawlist.apng`` against ``examples/tutorial/drawlist.das``.
//! A display-only rail - no gesture to drive - so the recording proves the
//! drawlist actually PAINTS rather than reacting to input. Four voiced beats,
//! one per scope wrapper plus the intro, and the self-check is on the output:
//!   1. intro     - name the rail (DRAW_INTRO on screen).
//!   2. window dl - with_window_drawlist painted the shapes; assert the registry
//!                  holds >= 12 `add_*` primitive entries (record_check_kind_count).
//!                  The prims register under synthesized path keys, not idents,
//!                  so a kind-family count is the honest "did it paint" signal.
//!   3. foreground- with_foreground_drawlist; the FG label is on screen.
//!   4. background- with_background_drawlist; the window is still up over the tint.

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let b = snap?["globals"]?[ident]?["bbox"]
    return (((b?["x"] ?? 0.0f) + (b?["z"] ?? 0.0f)) * 0.5f,
            ((b?["y"] ?? 0.0f) + (b?["w"] ?? 0.0f)) * 0.5f)
}

[export]
def main {
    with_recording_app("examples/tutorial/drawlist.das",
                       "drawlist.apng", 55) $(app) {
        let T_WIN   = "DRAW_WIN"
        let T_INTRO = "DRAW_WIN/DRAW_INTRO"
        let T_BAND  = "DRAW_WIN/DRAW_BAND_SLOT"
        let T_FG    = "DRAW_WIN/DRAW_FG_LABEL"

        var snap = wait_for_render(app, T_INTRO, 10.0f)
        if (snap == null) {
            panic("{T_INTRO} never rendered - wrong app running?")
        }
        let p_intro = widget_center(snap, T_INTRO)
        let p_band  = widget_center(snap, T_BAND)
        let p_fg    = widget_center(snap, T_FG)

        // Park the cursor on the intro line and warm up the capture pipeline.
        move_to(app, p_intro, 800)
        hold_content(app, 1800u)

        // ---- Beat 1: intro ----
        record_check_rendered(app, T_INTRO, true, 180)
        say(app, "the drawlist rail", T_INTRO,
            [voice = "The drawlist rail paints shapes straight onto ImGui's draw lists - below, over, or inside your widgets."])

        // ---- Beat 2: with_window_drawlist - the shapes (the meat) ----
        move_to(app, p_band, 600)
        hold_content(app, SETTLE_MS)
        let d2 = say_begin(app, "with_window_drawlist: shapes into the window", T_BAND,
            [voice = "with_window_drawlist hands you the window's draw list. Rect, line, circle, triangle, text - plus gradients, ellipses, n-gons, and bezier curves, all painted into the window."])
        let anchor2 = record_frame_count(app)
        let lead = d2 > 1400u ? 700u : d2 / 3u
        hold_content(app, lead)
        // Every primitive registers an `add_*` entry; assert the rail really painted.
        record_check_kind_count(app, "add_", 12, 180)
        hold_remainder_content(app, d2, anchor2)

        // ---- Beat 3: with_foreground_drawlist - the FG ribbon ----
        move_to(app, p_fg, 600)
        hold_content(app, SETTLE_MS)
        record_check_rendered(app, T_FG, true, 180)
        say(app, "with_foreground_drawlist: over everything", T_FG,
            [voice = "with_foreground_drawlist paints over every window - here a small FG ribbon, pinned top-left of the viewport."])

        // ---- Beat 4: with_background_drawlist - the viewport tint ----
        move_to(app, p_band, 600)
        hold_content(app, SETTLE_MS)
        record_check_rendered(app, T_WIN, true, 180)
        say(app, "with_background_drawlist: under everything", T_WIN,
            [voice = "And with_background_drawlist paints under everything - a full-viewport tint sitting behind the window."])
    }
}
