options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``display_widgets.apng`` against ``examples/tutorial/display_widgets.das``.
//! These are display-only widgets - no gesture to drive - so the recording proves the
//! widgets WORK rather than reacting to input: three voiced beats that narrate the
//! progress bars and images and self-verify their behaviour.
//!   1. progress_bar - the static bar is on screen (record_check_rendered).
//!   2. animation    - the sine-driven bar's fraction CHANGES over time
//!                     (record_check_changed) - the one dynamic behaviour to verify.
//!   3. image        - both textures are on screen (record_check_rendered).
//! (The widgets carry explicit idents now, so each is addressable in the snapshot;
//! the example was anonymous before.)

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let b = snap?["globals"]?[ident]?["bbox"]
    return (((b?["x"] ?? 0.0f) + (b?["z"] ?? 0.0f)) * 0.5f,
            ((b?["y"] ?? 0.0f) + (b?["w"] ?? 0.0f)) * 0.5f)
}

[export]
def main {
    with_recording_app("examples/tutorial/display_widgets.das",
                       "display_widgets.apng", 45) $(app) {
        let PB_STATIC = "DISPLAY_WIN/PB_STATIC"
        let PB_DRIVEN = "DISPLAY_WIN/PB_DRIVEN"
        let IMG_PLAIN = "DISPLAY_WIN/IMG_PLAIN"
        let IMG_TINT  = "DISPLAY_WIN/IMG_TINT"

        var snap = wait_for_render(app, PB_STATIC, 10.0f)
        if (snap == null) {
            panic("{PB_STATIC} never rendered - wrong app running?")
        }
        let p_static = widget_center(snap, PB_STATIC)
        let p_driven = widget_center(snap, PB_DRIVEN)
        let p_img    = widget_center(snap, IMG_TINT)

        // Park the cursor on the bars and warm up the capture pipeline.
        move_to(app, p_static, 800)
        hold_content(app, 1800u)

        // ---- Beat 1: progress_bar overview ----
        record_check_rendered(app, PB_STATIC, true, 180)
        say(app, "progress_bar: fraction + overlay", PB_STATIC,
            [voice = "progress_bar wraps ImGui's progress bar - a fraction, a size, and overlay text. Here are three: a static thirty-three percent, a live one, and a fixed-width bar."])

        // ---- Beat 2: the sine-driven bar animates (the one dynamic behaviour) ----
        move_to(app, p_driven, 500)
        hold_content(app, SETTLE_MS)
        let d2 = say_begin(app, "live fraction, sine-driven", PB_DRIVEN,
            [voice = "The middle bar's fraction is driven by a sine wave, so it sweeps back and forth - and the snapshot's fraction tracks the visible bar exactly."])
        let anchor2 = record_frame_count(app)
        let lead = d2 > 1400u ? 700u : d2 / 3u
        let before = write_json(snapshot(app)?["globals"]?[PB_DRIVEN]?["payload"]?["fraction"])
        hold_content(app, lead)
        record_check_changed(app, PB_DRIVEN, "fraction", before, 180)
        hold_remainder_content(app, d2, anchor2)

        // ---- Beat 3: image - display only ----
        move_to(app, p_img, 700)
        hold_content(app, SETTLE_MS)
        record_check_rendered(app, IMG_PLAIN, true, 180)
        record_check_rendered(app, IMG_TINT, true, 180)
        say(app, "image: display-only texture", IMG_TINT,
            [voice = "image paints a texture - no click, just display. The same font atlas, drawn twice: plain, then tinted green with a soft border."])
    }
}
