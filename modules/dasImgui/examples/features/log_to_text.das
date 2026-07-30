options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: log_text + log_to_clipboard + log_finish — ImGui log capture rail.
// SHOWS:   `log_to_clipboard(auto_open_depth?)` opens the log session
//          targeting the clipboard. `log_text(s)` appends a line. `log_finish()`
//          closes the session and flushes to the chosen target. Subsequent
//          text() submissions are ALSO captured between begin/finish (ImGui's
//          built-in capture for visible widgets). Press the button — the
//          captured payload lands on the clipboard, paste anywhere.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/log_to_text.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/log_to_text.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/log_to_text.das
// =============================================================================

[export]
def init() {
    harness_init("log_text / log_to_clipboard / log_finish", 520, 280)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(480.0, 240.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "log family", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Click the button to capture the three lines below to the clipboard."))

        if (button(COPY_BTN, (text = "copy to clipboard"))) {
            log_to_clipboard(-1)
            log_text("Line A — direct log_text submission")
            log_text("Line B — second log_text line")
            log_text("Line C — third and final")
            log_finish()
        }

        separator(SEP)
        text(STATIC_1, (text = "Static line above the log session — NOT captured."))
        text(STATIC_2, (text = "(The clipboard payload only contains the three Line A/B/C lines.)"))
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
