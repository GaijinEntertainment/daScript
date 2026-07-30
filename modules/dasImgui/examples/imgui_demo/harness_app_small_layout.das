options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require app_small

// =============================================================================
// HARNESS: imgui_demo app_small.ShowExampleAppLayout — single-scene driver.
// SHOWS:   menu_bar + child(ResizeX) + selectable loop + group + tab_bar +
//          tab_item pair + button pair.
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_small_layout.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_small_layout.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_app_small_layout.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - app_small Layout", 720, 540)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    ShowExampleAppLayout()
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
