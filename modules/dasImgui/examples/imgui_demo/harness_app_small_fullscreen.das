options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require app_small

// =============================================================================
// HARNESS: imgui_demo app_small.ShowExampleAppFullscreen — single-scene driver.
// SHOWS:   viewport-spanning window with edit_checkbox (Use work area) +
//          HelpMarker + 5 edit_checkbox_flags for window-flags + with_indent block.
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_small_fullscreen.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_small_fullscreen.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_app_small_fullscreen.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - app_small Fullscreen", 720, 540)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    ShowExampleAppFullscreen()
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
