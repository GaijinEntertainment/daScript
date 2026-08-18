options gen2
options _comment_hygiene = true
options gc

require imgui/imgui_harness
require user_guide

// =============================================================================
// HARNESS: imgui_demo user guide — single-scene driver for the C++
//          ShowUserGuide port (the bullet list under Help > USER GUIDE).
// SHOWS:   the bullet_text / with_indent guide inside a host MAIN_WIN window
//          (ShowUserGuide does NOT open its own window).
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_user_guide.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_user_guide.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_user_guide.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - User Guide", 640, 480)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    SetNextWindowSize(ImVec2(600.0, 440.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "User Guide", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        user_guide::ShowUserGuide()
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
        harness_maybe_collect_gc()
    }
    shutdown()
}
