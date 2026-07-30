options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require app_small

// =============================================================================
// HARNESS: imgui_demo app_small.ShowExampleAppConstrainedResize — single-scene driver.
// SHOWS:   9-option resize-constraint combo (6 no-callback + 3 lambda callbacks
//          via ImGuiSizeConstraints from imgui_window_constraints_builtin),
//          three preset-size buttons, drag_int for display-line count, and
//          two checkboxes (Auto-resize / Window padding).
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_small_constrained_resize.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_small_constrained_resize.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_app_small_constrained_resize.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - app_small ConstrainedResize", 720, 540)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    ShowExampleAppConstrainedResize()
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
