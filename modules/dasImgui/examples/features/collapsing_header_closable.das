options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: collapsing_header(closable=true) — close-button route (boost v2 §3.5).
// SHOWS:   With closable=true, the header surfaces an X button. Click hides
//          the section entirely (the body skips, the chrome disappears) per
//          ImGui's `bool* p_visible` overload contract. Reopen via
//          `imgui_open` against the path key — the state.pending_open flag
//          flips state.open back to true and the next frame re-renders.
// DRIVE:   curl -X POST -d '{"name":"imgui_open","args":{"target":"MAIN_WIN/HEADER_CLOSE"}}' \
//              localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/collapsing_header_closable.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/collapsing_header_closable.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/collapsing_header_closable.das
// =============================================================================

[export]
def init() {
    harness_init("collapsing_header_closable — boost v2 §3.5", 720, 420)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 360.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "collapsing_header(closable=…)",
                      closable = false, flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Two collapsing headers — first closable (X button), second not."))

        collapsing_header(HEADER_CLOSE, (text = "Closable section",
                                          closable = true,
                                          flags = ImGuiTreeNodeFlags.None)) {
            text(BODY_CLOSE_1, (text = "Body of closable header. Click the X to hide."))
            text(BODY_CLOSE_2, (text = "Re-open via imgui_open MAIN_WIN/HEADER_CLOSE."))
            if (button(REVEAL_BTN, (text = "I'm only visible while open"))) {}
        }

        collapsing_header(HEADER_STD, (text = "Standard (non-closable) section",
                                        closable = false,
                                        flags = ImGuiTreeNodeFlags.None)) {
            text(BODY_STD, (text = "No X button — header collapses via the caret arrow only."))
        }
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
