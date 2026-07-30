options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require layout

// =============================================================================
// HARNESS: imgui_demo layout — single-scene driver for the C++
//          ShowDemoWindowLayout port. Lives next to layout.das because
//          daslang require uses sibling-directory resolution.
// SHOWS:   LAYOUT_SECTION collapsing_header inside a host MAIN_WIN window
//          (ShowDemoWindowLayout does NOT open its own window).
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_layout.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_layout.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_layout.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - Layout & Scrolling", 760, 640)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    SetNextWindowSize(ImVec2(720.0, 600.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "Layout & Scrolling", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        layout::ShowDemoWindowLayout()
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
