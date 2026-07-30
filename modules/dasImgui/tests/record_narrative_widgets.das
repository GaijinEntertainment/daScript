options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``narrative_widgets.apng`` against
//! ``examples/tutorial/narrative_widgets.das``. Voiced, self-verifying tour of
//! the read-only DISPLAY widgets (text / text_wrapped / text_colored /
//! text_disabled / bullet / bullet_text / separator_text / label_text). These
//! take no input, so each beat narrates while the cursor points and ASSERTS the
//! widget rendered and echoed its call-site string into the snapshot - the
//! "the label says what I expect" claim the tutorial is built on. A missing or
//! wrong value aborts the recording at teardown.

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
    with_recording_app("examples/tutorial/narrative_widgets.das",
                       "narrative_widgets.apng", 60) $(app) {
        let T_PLAIN    = "NARRATIVE_WIN/PLAIN_LINE"
        let T_WRAPPED  = "NARRATIVE_WIN/WRAPPED_LINE"
        let T_COLORED  = "NARRATIVE_WIN/COLORED_LINE"
        let T_DISABLED = "NARRATIVE_WIN/DISABLED_LINE"
        let T_BULLET   = "NARRATIVE_WIN/BULLET_MARK"
        let T_BFIRST   = "NARRATIVE_WIN/BULLET_FIRST"
        let T_LABEL    = "NARRATIVE_WIN/VERSION_LABEL"

        var snap = wait_for_render(app, T_LABEL, 10.0f)
        if (snap == null) {
            panic("{T_LABEL} never rendered - wrong app running?")
        }
        let p_plain   = widget_center(snap, T_PLAIN)
        let p_colored = widget_center(snap, T_COLORED)
        let p_bullet  = widget_center(snap, T_BFIRST)
        let p_label   = widget_center(snap, T_LABEL)

        move_to(app, (60.0f, 40.0f), 600)
        hold_content(app, SETTLE_MS)

        // ---- Beat 1: text + text_wrapped ----
        move_to(app, p_plain, 900)
        hold_content(app, SETTLE_MS)
        record_check_value(app, T_PLAIN, "value", "text() - one line, telemetry-visible.")
        record_check_rendered(app, T_WRAPPED, true)
        say(app, "text + text_wrapped", T_PLAIN,
            [voice = "These are display widgets - they show text and never take input. Plain text is one line; text wrapped reflows a long string to the window edge, and the snapshot keeps the whole string either way."])

        // ---- Beat 2: text_colored + text_disabled ----
        move_to(app, p_colored, 900)
        hold_content(app, SETTLE_MS)
        record_check_rendered(app, T_COLORED, true)
        record_check_rendered(app, T_DISABLED, true)
        say(app, "text_colored + text_disabled", T_COLORED,
            [voice = "text colored takes an inline color, and text disabled greys out a hint you cannot act on. Same telemetry underneath - only the look changes."])

        // ---- Beat 3: bullet + bullet_text ----
        move_to(app, p_bullet, 900)
        hold_content(app, SETTLE_MS)
        record_check_rendered(app, T_BULLET, true)
        record_check_value(app, T_BFIRST, "value", "First point of interest.")
        say(app, "bullet + bullet_text", T_BFIRST,
            [voice = "A bullet draws just the marker dot, while bullet text bundles the dot and a line together. Handy for a list of points."])

        // ---- Beat 4: label_text ----
        move_to(app, p_label, 900)
        hold_content(app, SETTLE_MS)
        record_check_value(app, T_LABEL, "value", "v2.0-detour")
        say(app, "label_text - value and key", T_LABEL,
            [voice = "And label text pairs a value with its label and echoes both into the snapshot - perfect for a property sheet."])
    }
}
