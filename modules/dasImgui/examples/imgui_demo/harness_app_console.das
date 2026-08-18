options gen2
options _comment_hygiene = true
options gc

require imgui/imgui_harness
require app_console

// =============================================================================
// HARNESS: imgui_demo Example: Console — single-scene driver for the C++
//          ShowExampleAppConsole port. The function opens its own window
//          (CONSOLE_WIN).
// SHOWS:   CONSOLE_WIN + the "Close Console" title-bar context menu; the
//          close request is consumed here the way imgui_demo.das does it.
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_console.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_console.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_app_console.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - Console", 760, 540)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    app_console::ShowExampleAppConsole()
    if (app_console::take_close_request()) {
        request_exit()
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
