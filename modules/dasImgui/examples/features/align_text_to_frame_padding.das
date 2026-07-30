options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: align_text_to_frame_padding — vertically align next text to a
//                                        framed item's baseline.
// SHOWS:   The standard pattern for pairing a label with an inline framed
//          widget (button, input, slider) on the same line. Without the
//          prefix, the bare `text()` sits at the top of the line and looks
//          mis-aligned against the framed widget's vertical centre. Compare
//          row A (no align) against row B (with align).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/align_text_to_frame_padding.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/align_text_to_frame_padding.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/align_text_to_frame_padding.das
// =============================================================================

[export]
def init() {
    harness_init("align_text_to_frame_padding", 520, 240)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(480.0, 200.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "align_text_to_frame_padding", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Row A vs Row B — same widgets, only difference is the align prefix."))

        // Row A — bare label, looks top-aligned against the framed button.
        text(LABEL_A, (text = "Row A:"))
        same_line()
        if (button(BTN_A, (text = "framed widget"))) {}

        // Row B — align prefix lifts the label baseline to the frame's centre.
        align_text_to_frame_padding(ALIGN_B)
        text(LABEL_B, (text = "Row B:"))
        same_line()
        if (button(BTN_B, (text = "framed widget"))) {}
    }

    harness_end_frame()
}

[export]
def shutdown() {
    harness_shutdown()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
