options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require app_small

// =============================================================================
// HARNESS: imgui_demo app_small.ShowExampleAppSimpleOverlay — single-scene driver.
// SHOWS:   overlay window pinned to a viewport corner with a popup_context_window
//          context menu offering 7 anchor choices (4 corners + Custom + Center + Close).
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_small_simple_overlay.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_small_simple_overlay.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_app_small_simple_overlay.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - app_small SimpleOverlay", 720, 540)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    ShowExampleAppSimpleOverlay()
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
