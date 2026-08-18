options gen2
options _comment_hygiene = true
options gc

require imgui/imgui_harness
require popups

// =============================================================================
// HARNESS: imgui_demo popups — single-scene driver for the C++
//          ShowDemoWindowPopups port (popups, context menus, modals,
//          menus-in-regular-window).
// SHOWS:   POPUPS_SECTION collapsing_header inside a host MAIN_WIN window
//          (ShowDemoWindowPopups does NOT open its own window).
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_popups.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_popups.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_popups.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - Popups", 760, 640)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    SetNextWindowSize(ImVec2(720.0, 600.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "Popups & Modal windows", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        popups::ShowDemoWindowPopups()
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
