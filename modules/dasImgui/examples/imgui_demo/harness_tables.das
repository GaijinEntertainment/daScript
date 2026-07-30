options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require tables

// =============================================================================
// HARNESS: imgui_demo tables — single-scene driver for the C++
//          ShowDemoWindowTables port. Lives next to tables.das because
//          daslang require uses sibling-directory resolution.
// SHOWS:   TABLES_SECTION collapsing_header inside a host MAIN_WIN window
//          (ShowDemoWindowTables does NOT open its own window).
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_tables.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_tables.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_tables.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - Tables", 760, 640)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    SetNextWindowSize(ImVec2(720.0, 600.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "Tables", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        tables::ShowDemoWindowTables()
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
    }
    shutdown()
}
