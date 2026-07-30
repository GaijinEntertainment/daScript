options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require app_small

// =============================================================================
// HARNESS: imgui_demo app_small.ShowExampleAppLongText — single-scene driver.
// SHOWS:   combo (test_type) + buttons (Clear / Add 1000 lines) + child(Log)
//          rendering one of three strategies (single TextUnformatted, clipped
//          per-line text, unclipped per-line text).
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_small_long_text.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_small_long_text.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_app_small_long_text.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - app_small LongText", 600, 660)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    ShowExampleAppLongText()
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
