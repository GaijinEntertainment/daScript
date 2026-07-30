options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require app_small

// =============================================================================
// HARNESS: imgui_demo app_small.ShowExampleAppWindowTitles — single-scene driver.
// SHOWS:   3 simultaneous windows demonstrating `##` ID disambiguation (windows
//          1 + 2 share visible title) and `###` identifier-stable retitling
//          (window 3 animates title text every frame, ID fixed to "AnimatedTitle").
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_small_window_titles.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_small_window_titles.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_app_small_window_titles.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - app_small WindowTitles", 720, 540)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    ShowExampleAppWindowTitles()
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
