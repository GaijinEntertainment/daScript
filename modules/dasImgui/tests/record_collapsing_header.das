options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record collapsing_header.apng against examples/tutorial/collapsing_header.das.
//! Every section is driven the human way and self-verified, so the recording doubles as a
//! test. collapsing_header now exposes its header bbox (current_item_bbox -> open_close_finalize,
//! this rail), so the chevron and the X-button are real click targets. The fold is asserted by
//! the body's rendered state (the body runs only while the section is open) -- exactly what the
//! widget teaches:
//!   BASIC    (flags None)   -> click header to expand, flip the checkbox, click to collapse
//!   CLOSABLE (closable)     -> drag the volume slider, then click the X to hide the whole strip
//!   STARTUP  (DefaultOpen)  -> collapse then expand; the flag only sets the first-frame state

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    var b = snap?["globals"]?[ident]?["bbox"]
    let x = b?["x"] ?? 0.0f
    let y = b?["y"] ?? 0.0f
    let z = b?["z"] ?? 0.0f
    let w = b?["w"] ?? 0.0f
    return ((x + z) * 0.5f, (y + w) * 0.5f)
}

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(b?["x"] ?? 0.0f, b?["y"] ?? 0.0f, b?["z"] ?? 0.0f, b?["w"] ?? 0.0f)
}

[export]
def main {
    with_recording_app("examples/tutorial/collapsing_header.das",
                       "collapsing_header.apng", 90) $(app) {
        let H_BASIC   = "CH_WIN/BASIC_CH"
        let C_BASIC   = "CH_WIN/BASIC_CH/BASIC_LIVE"
        let H_CLOSE   = "CH_WIN/CLOSABLE_CH"
        let C_VOLUME  = "CH_WIN/CLOSABLE_CH/VOLUME"
        let H_STARTUP = "CH_WIN/STARTUP_CH"
        let C_HINT    = "CH_WIN/STARTUP_CH/STARTUP_HINT"

        var snap = wait_for_render(app, H_BASIC, 10.0f)
        if (snap == null) {
            panic("{H_BASIC} never rendered - wrong app running?")
        }

        // ---- Stage 1: BASIC starts closed -> click the header to expand ----
        move_to(app, widget_center(snap, H_BASIC), 600)
        hold_content(app, SETTLE_MS)
        let d1 = say_begin(app, "collapsing header - click to expand", H_BASIC,
            [voice = "A collapsing header folds a whole section of controls under one clickable strip. This one starts closed. Click the header and it opens, showing the controls inside."])
        toggle_tree_voice(app, d1, H_BASIC, false, C_BASIC, true)

        // ---- Stage 2: flip the checkbox inside the opened section ----
        snap = snapshot(app)
        move_to(app, widget_center(snap, C_BASIC), 500)
        hold_content(app, SETTLE_MS)
        let d2 = say_begin(app, "controls work normally", C_BASIC,
            [voice = "Everything inside behaves like any other widget. Flip this checkbox and it toggles, right there under the header."])
        hold_through_voice(app, d2, [C_BASIC])

        // ---- Stage 3: click the header again to collapse ----
        snap = snapshot(app)
        move_to(app, widget_center(snap, H_BASIC), 500)
        hold_content(app, SETTLE_MS)
        let d3 = say_begin(app, "click again to collapse", H_BASIC,
            [voice = "Click the header again and the whole body stops drawing. Unlike a tree node there is no tree pop to pair, the header owns the fold by itself."])
        toggle_tree_voice(app, d3, H_BASIC, false, C_BASIC, false)

        // ---- Stage 4: closable section -> drag the volume slider ----
        snap = snapshot(app)
        let vb = widget_bbox(snap, C_VOLUME)
        let vcy = (vb.y + vb.w) * 0.5f
        move_to(app, (vb.x + 14.0f, vcy), 700)
        hold_content(app, SETTLE_MS)
        let d4 = say_begin(app, "closable section, live controls", C_VOLUME,
            [voice = "The middle header is closable. Its controls work just the same, drag the volume slider and the value follows along."])
        drag_through_voice(app, d4, C_VOLUME, (vb.x + 0.72f * (vb.z - vb.x), vcy))

        // ---- Stage 5: click the X-button to hide the whole strip ----
        snap = snapshot(app)
        move_to(app, widget_center(snap, H_CLOSE), 600)
        hold_content(app, SETTLE_MS)
        let d5 = say_begin(app, "the X hides the whole strip", H_CLOSE,
            [voice = "A closable header also draws a little X on the right. Click it and the entire row disappears, header and all, not just the body the way the chevron folds it."])
        close_button_voice(app, d5, H_CLOSE, C_VOLUME)

        // ---- Stage 6: DefaultOpen -> collapse it ----
        snap = snapshot(app)
        move_to(app, widget_center(snap, H_STARTUP), 600)
        hold_content(app, SETTLE_MS)
        let d6 = say_begin(app, "DefaultOpen sets only the first frame", H_STARTUP,
            [voice = "The last header used the default open flag, so it started expanded. That flag only sets the very first frame, after that you are in control. Click to fold it."])
        toggle_tree_voice(app, d6, H_STARTUP, false, C_HINT, false)

        // ---- Stage 7: and expand it back ----
        snap = snapshot(app)
        move_to(app, widget_center(snap, H_STARTUP), 500)
        hold_content(app, SETTLE_MS)
        let d7 = say_begin(app, "and back open", H_STARTUP,
            [voice = "One more click brings it right back. Three headers, three behaviors, every one driven by a real click."])
        toggle_tree_voice(app, d7, H_STARTUP, false, C_HINT, true)
    }
}
