options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: with_indent — stateless block-arg wrapper around Indent/Unindent.
// SHOWS:   nested with_indent blocks; default amount (0.0f) defers to ImGui's
//          style IndentSpacing, explicit amounts produce custom indents.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/with_indent.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/with_indent.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/with_indent.das
// =============================================================================

[export]
def init() {
    harness_init("with_indent — scope wrapper", 640, 480)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(580.0, 420.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "with_indent", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("Default indent (0.0f uses style IndentSpacing):")
        with_indent(0.0f) {
            text("Level 1")
            with_indent(0.0f) {
                text("Level 2")
                with_indent(0.0f) {
                    text("Level 3")
                }
            }
        }

        separator()
        text("Custom amounts:")
        with_indent(40.0f) {
            text("indent 40px")
        }
        with_indent(80.0f) {
            text("indent 80px")
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
