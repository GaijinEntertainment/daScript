options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require widgets

// =============================================================================
// HARNESS: imgui_demo widgets — single-scene driver for the C++
//          ShowDemoWindowWidgets port. Lives next to widgets.das because
//          daslang require uses sibling-directory resolution.
// SHOWS:   WIDGETS_SECTION collapsing_header inside a host MAIN_WIN window
//          (ShowDemoWindowWidgets does NOT open its own window).
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_widgets.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_widgets.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_widgets.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - Widgets", 760, 640)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    SetNextWindowSize(ImVec2(720.0, 600.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "Widgets", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        widgets::ShowDemoWindowWidgets()
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
