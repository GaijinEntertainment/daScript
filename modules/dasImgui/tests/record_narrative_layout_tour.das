options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``narrative_layout_tour.apng`` against
//! ``examples/tutorial/narrative_layout_tour.das``. Voiced, self-verifying
//! one-window tour of the read-only families - narrative text, layout, a
//! display progress bar, and a flat tooltip. The display widgets take no
//! input, so each beat narrates while the cursor points and ASSERTS the
//! widget rendered / echoed its string; the final beat HOVERS a button and
//! asserts the flat tooltip fired. A miss aborts the recording at teardown.

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(b?["x"] ?? 0.0f, b?["y"] ?? 0.0f, b?["z"] ?? 0.0f, b?["w"] ?? 0.0f)
}

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let b = widget_bbox(snap, ident)
    return ((b.x + b.z) * 0.5f, (b.y + b.w) * 0.5f)
}

[export]
def main {
    with_recording_app("examples/tutorial/narrative_layout_tour.das",
                       "narrative_layout_tour.apng", 75) $(app) {
        let T_INTRO    = "TOUR_WIN/INTRO"
        let T_WRAP     = "TOUR_WIN/WRAP"
        let T_COLORED  = "TOUR_WIN/COLORED"
        let T_DISABLED = "TOUR_WIN/DISABLED"
        let T_BULLET   = "TOUR_WIN/BULLET_LINE"
        let T_LABEL    = "TOUR_WIN/LABEL_VERSION"
        let T_SEP      = "TOUR_WIN/SEP_TITLE"
        let T_PROG     = "TOUR_WIN/PROG_BAR"
        let T_TIP      = "TOUR_WIN/HOVER_TIP"
        let T_TIPTOOL  = "TOUR_WIN/HOVER_TIP_TOOL"

        var snap = wait_for_render(app, T_TIP, 10.0f)
        if (snap == null) {
            panic("{T_TIP} never rendered - wrong app running?")
        }
        let p_intro   = widget_center(snap, T_INTRO)
        let p_colored = widget_center(snap, T_COLORED)
        let p_bullet  = widget_center(snap, T_BULLET)
        let p_label   = widget_center(snap, T_LABEL)
        let p_prog    = widget_center(snap, T_PROG)
        let p_tip     = widget_center(snap, T_TIP)

        move_to(app, (450.0f, 360.0f), 600)
        hold_content(app, SETTLE_MS)

        // ---- Beat 1: text + text_wrapped ----
        move_to(app, p_intro, 900)
        hold_content(app, SETTLE_MS)
        record_check_value(app, T_INTRO, "value", "Plain text() - one line, telemetry-visible.")
        record_check_rendered(app, T_WRAP, true)
        say(app, "text + text_wrapped", T_INTRO,
            [voice = "This panel tours the read-only families. Plain text is one line; text wrapped reflows a long string to the window edge, and the snapshot keeps the whole string."])

        // ---- Beat 2: text_colored + text_disabled ----
        move_to(app, p_colored, 900)
        hold_content(app, SETTLE_MS)
        record_check_rendered(app, T_COLORED, true)
        record_check_rendered(app, T_DISABLED, true)
        say(app, "text_colored + text_disabled", T_COLORED,
            [voice = "text colored takes an inline color, and text disabled greys out a hint. Same telemetry underneath, only the look changes."])

        // ---- Beat 3: bullet_text + label_text ----
        move_to(app, p_bullet, 900)
        hold_content(app, SETTLE_MS)
        record_check_value(app, T_BULLET, "value", "bullet_text() - marker + text in one call.")
        record_check_value(app, T_LABEL, "value", "v2.0-detour")
        say(app, "bullet_text + label_text", T_BULLET,
            [voice = "bullet text bundles a marker dot and a line; label text pairs a value with its label - both strings land in the snapshot."])

        // ---- Beat 4: separator_text + progress_bar ----
        move_to(app, p_prog, 900)
        hold_content(app, SETTLE_MS)
        record_check_rendered(app, T_SEP, true)
        record_check_rendered(app, T_PROG, true)
        say(app, "separator_text + progress_bar", T_PROG,
            [voice = "A separator text draws a titled divider, and below it a progress bar whose fraction is driven by the app loop."])

        // ---- Beat 5: flat tooltip on hover ----
        move_to(app, p_tip, 900)
        hold_content(app, SETTLE_MS)
        record_check_rendered(app, T_TIPTOOL, true)
        say(app, "set_item_tooltip on hover", T_TIP,
            [voice = "And resting the cursor on this button fires a flat tooltip - set item tooltip attaches it to the previous item, no scope to open or close."])
    }
}
