options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: narrative_separator — separator/separator_text/label_text family.
// SHOWS:   separator() — EmptyMarkerState marker, no payload (zero divider).
//          separator_text(text=...) — NarrativeState with rendered title.
//          label_text(key=..., value=...) — two-string display, LabelTextState
//          carries both halves.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals."MAIN_WIN/LABEL_FPS"'
//          # → {"payload":{"key":"frames per second","value":"60"}, ...}
// STANDALONE: daslang.exe modules/dasImgui/examples/features/narrative_separator.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/narrative_separator.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/narrative_separator.das
// =============================================================================

[export]
def init() {
    harness_init("narrative_separator — boost v2 §3.2", 720, 460)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 400.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "separator / separator_text / label_text",
                      closable = false, flags = ImGuiWindowFlags.None)) {
        text(LEAD_TEXT, (text = "Three siblings — bare divider, titled divider, two-string row."))
        separator(SEP_AFTER_LEAD)

        separator_text(SECTION_RUNTIME, (text = "Runtime"))
        label_text(LABEL_FPS, (key = "frames per second", value = "60"))
        label_text(LABEL_FRAME, (key = "frame", value = "1234"))
        separator(SEP_BETWEEN)

        separator_text(SECTION_BUILD, (text = "Build"))
        label_text(LABEL_GIT, (key = "commit", value = "abc123 (HEAD)"))
        label_text(LABEL_CFG, (key = "config", value = "Release"))
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
