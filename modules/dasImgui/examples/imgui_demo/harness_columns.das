options gen2
options _comment_hygiene = true
options gc

require imgui/imgui_harness
require columns

// =============================================================================
// HARNESS: imgui_demo columns — single-scene driver for the C++
//          ShowDemoWindowColumns port (the legacy Columns() API section).
// SHOWS:   COLUMNS_OUTER tree_node inside a host MAIN_WIN window
//          (ShowDemoWindowColumns does NOT open its own window).
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_columns.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_columns.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_columns.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - Columns", 760, 640)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    SetNextWindowSize(ImVec2(720.0, 600.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "Legacy Columns", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        columns::ShowDemoWindowColumns()
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
