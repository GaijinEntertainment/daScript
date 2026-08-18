options gen2
options _comment_hygiene = true
options gc

require imgui/imgui_harness
require imgui_demo

// =============================================================================
// EXAMPLE: imgui_demo — daslang port of Dear ImGui's imgui_demo.cpp.
// SHOWS:   complete tour of the dasImgui boost surface as a single host file
//          dispatching into per-section sub-modules under imgui_demo/.
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/main.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/main.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/main.das
// =============================================================================

[export]
def init() {
    harness_init("Dear ImGui Demo (daslang port)", 1280, 720)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    RunImGuiDemo()
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
