options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require about

// =============================================================================
// HARNESS: imgui_demo about — single-scene driver for the C++ ShowAboutWindow
//          port. Lives next to about.das because daslang require uses
//          sibling-directory resolution; cross-folder requires aren't supported.
// SHOWS:   ABOUT_WINDOW + the "Config/Build Information" child + the v2
//          log_to_clipboard / log_text / log_finish trio (when the
//          "Copy to clipboard" button fires).
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_about.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_about.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_about.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - About", 720, 540)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    about::ShowAboutWindow()
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
