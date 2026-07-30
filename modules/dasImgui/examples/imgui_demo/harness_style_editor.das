options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require style_editor

// =============================================================================
// HARNESS: imgui_demo style_editor — single-scene driver for the Style Editor
//          port. Lives next to style_editor.das (sibling require).
// SHOWS:   STYLE_TABS (Sizes / Colors / Fonts / Rendering) inside a host
//          MAIN_WIN window. ShowStyleEditor does NOT open its own window —
//          this harness wraps it.
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_style_editor.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_style_editor.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_style_editor.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - Style Editor", 720, 560)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    SetNextWindowSize(ImVec2(680.0, 520.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "Style Editor", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        style_editor::ShowStyleEditor()
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
