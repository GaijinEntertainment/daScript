options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require app_custom_rendering

// =============================================================================
// HARNESS: imgui_demo Example: Custom Rendering — single-scene driver for the
//          C++ ShowExampleAppCustomRendering port. ShowExampleAppCustomRendering
//          opens its own window (4-tab TabBar inside), so this harness just
//          calls it directly from the frame body.
// SHOWS:   CR_WIN window with Primitives / Canvas / BG-FG / Channels tabs.
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_custom_rendering.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_custom_rendering.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_app_custom_rendering.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - Custom Rendering", 760, 720)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    app_custom_rendering::ShowExampleAppCustomRendering()
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
