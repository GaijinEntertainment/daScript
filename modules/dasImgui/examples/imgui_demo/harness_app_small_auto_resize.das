options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require app_small

// =============================================================================
// HARNESS: imgui_demo app_small.ShowExampleAppAutoResize — single-scene driver.
// SHOWS:   window(AlwaysAutoResize) shrinking/growing per the slider value,
//          with text lines indented in 4-space increments.
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_small_auto_resize.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_small_auto_resize.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_app_small_auto_resize.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - app_small AutoResize", 720, 540)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    ShowExampleAppAutoResize()
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
