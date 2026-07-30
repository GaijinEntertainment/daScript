options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver script: record ``icons.apng`` against
//! ``examples/tutorial/icons.das``. Tours the asset-free vector icon set —
//! clickable ``icon_button`` controls (a transport bar, then an edit toolbar with
//! hover tips), then ``icon()`` draw-only glyphs at several sizes, then the
//! catalog enumeration. Each icon_button stage clicks UNDER its voice line and
//! verifies the click_count tick; the draw-only + catalog stages narrate.

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(
        b?["x"] ?? 0.0f,
        b?["y"] ?? 0.0f,
        b?["z"] ?? 0.0f,
        b?["w"] ?? 0.0f
    )
}

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let b = widget_bbox(snap, ident)
    return ((b.x + b.z) * 0.5f, (b.y + b.w) * 0.5f)
}

[export]
def main {
    with_recording_app("examples/tutorial/icons.das",
                       "icons.apng", 60) $(app) {
        let T_PLAY  = "IC_WIN/IC_PLAY"
        let T_PAUSE = "IC_WIN/IC_PAUSE"
        let T_STOP  = "IC_WIN/IC_STOP"
        let T_REC   = "IC_WIN/IC_REC"
        let T_UNDO  = "IC_WIN/IC_UNDO"
        let T_REDO  = "IC_WIN/IC_REDO"
        let T_DISP  = "IC_WIN/IC_DISP"   // the icon() draw-only row header

        var snap = wait_for_widget(app, T_PLAY, 10.0f)
        if (snap == null) {
            panic("{T_PLAY} never registered - wrong app running?")
        }
        let p_play = widget_center(snap, T_PLAY)
        let p_undo = widget_center(snap, T_UNDO)

        //! Initial cursor placement on the first target.
        move_to(app, p_play, 500)
        hold_content(app, SETTLE_MS)

        // ---- Stage 1: icon_button — a clickable transport bar ----
        hold_through_voice(app, say_begin(app, "icon_button - vector glyphs", [T_PLAY, T_REC],
            [voice = "These are icon buttons. Every glyph is vector geometry drawn straight onto the draw list - no image files, no font atlas. You address one by name, like play or record, and it clicks like any other button."]),
            [T_PLAY, T_PAUSE, T_STOP, T_REC])

        // ---- Stage 2: edit toolbar — same widget, hover tips ----
        move_to(app, p_undo, 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "icon_button - hover for a tip", [T_UNDO, T_REDO],
            [voice = "The same button builds an edit toolbar - undo, redo, cut, save. Pass a tip and it shows a tooltip on hover, so a compact icon strip stays discoverable."]),
            [T_UNDO, T_REDO])

        // ---- Stage 3: icon() — draw-only, any size ----
        say(app, "icon() - draw only, any size", T_DISP,
            [voice = "When you just want to show a glyph, call icon with a name and a size. It draws at the cursor with no interaction, and it scales freely - the same waveform at twenty, thirty-two, and forty-eight pixels."])

        // ---- Stage 4: the breadth of the set ----
        say(app, "one growing set", T_DISP,
            [voice = "The set spans transport, edit, mix, synth, scene and chrome glyphs, and it keeps growing. Enumerate the whole catalog with icon count, icon name, and icon category - handy for building an icon picker. The full catalog with images lives in the docs."])
    }
}
