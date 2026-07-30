options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require app_dockspace

// =============================================================================
// HARNESS: imgui_demo Example: DockSpace — single-scene driver for the
//          C++ ShowExampleAppDockSpace port. The function opens its own host
//          window (MenuBar + NoDocking) with an embedded dockspace_in_window
//          + 2 sample dock_window panels.
// SHOWS:   DS_WIN host + DS dockspace + DS_PANEL_A / DS_PANEL_B.
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_dockspace.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_app_dockspace.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_app_dockspace.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - DockSpace", 760, 540)
    var io & = unsafe(GetIO())
    io.ConfigFlags |= ImGuiConfigFlags.DockingEnable
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    app_dockspace::ShowExampleAppDockSpace()
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
