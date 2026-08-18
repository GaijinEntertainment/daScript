options gen2
options _comment_hygiene = true
options gc

require imgui/imgui_harness
require app_log

// =============================================================================
// HARNESS: imgui_demo Example: Log — single-scene driver for the C++
//          ShowExampleAppLog port. The function opens its own window
//          (LOG_WIN: debug add/clear buttons, filter, scrolling log region).
// SHOWS:   LOG_WIN with the "[Debug] Add 5 entries" row inline at the top.
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_log.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_log.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_app_log.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - Log", 760, 540)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    app_log::ShowExampleAppLog()
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
