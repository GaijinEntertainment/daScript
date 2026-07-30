options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: dockspace_in_window — explicit-host dockspace nested inside a
//                                Begin/End window (compare dock_basic.das
//                                which uses DockSpaceOverViewport for the
//                                full-viewport variant).
// SHOWS:   - window(HOST, (flags = MenuBar | NoDocking | NoCollapse)) — the
//            host window owns its own MenuBar; dockspace_in_window does NOT
//            manage host-window flags.
//          - menu_bar(HOST_MENU) inside the host: regular menu/menu_item.
//          - dockspace_in_window(DS, (size = (0,0), flags = None)) —
//            DockSpace(GetID(widget_ident), size, flags, null). size=(0,0)
//            fills the host's available content region (post-menu-bar).
//            state.dock_id is captured for DockBuilder* layout calls.
//          - DockBuilder seeded layout (1 horizontal split: Files | Output)
//            gated by state.has_initial_layout (one-shot per session, or
//            after imgui_dock_reset).
//          - dock_window(FILES) / dock_window(OUTPUT) — dockable panels
//            INSIDE the host window via the captured DS.dock_id.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/dockspace_in_window.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/dockspace_in_window.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/dockspace_in_window.das
// =============================================================================

def setup_default_layout(dock_id : uint) {
    //! 2-panel horizontal split: Files (left 40%) | Output (right 60%).
    //! SetNodeSize before SplitNode — without it the 40/60 ratio is
    //! computed from a zero-sized node (matches dock_basic.das order).
    DockBuilderRemoveNode(dock_id)
    DockBuilderAddDockSpaceNode(dock_id, ImGuiDockNodeFlags.None)
    let vp = GetMainViewport()
    DockBuilderSetNodeSize(dock_id, ImVec2(vp.Size.x, vp.Size.y))
    var left_id : uint = 0u
    var right_id : uint = 0u
    DockBuilderSplitNode(dock_id, ImGuiDir.Left, 0.40f, left_id, right_id)
    DockBuilderDockWindow("Files", left_id)
    DockBuilderDockWindow("Output", right_id)
    DockBuilderFinish(dock_id)
}

[export]
def init() {
    harness_init("dockspace_in_window — boost v2", 720, 480)
    var io & = unsafe(GetIO())
    io.ConfigFlags |= ImGuiConfigFlags.DockingEnable
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(680.0f, 440.0f), ImGuiCond.FirstUseEver)
    window(HOST, (text = "Host window with embedded dockspace",
                  closable = false,
                  flags = ImGuiWindowFlags.MenuBar |
                          ImGuiWindowFlags.NoDocking |
                          ImGuiWindowFlags.NoCollapse)) {
        menu_bar(HOST_MENU) {
            menu(FILE_MENU, (text = "File", enabled = true)) {
                menu_item(SAVE_ITEM, (text = "Save"))
                menu_item(QUIT_ITEM, (text = "Quit"))
            }
        }

        dockspace_in_window(DS, (size = float2(0.0f, 0.0f),
                                 flags = ImGuiDockNodeFlags.None)) {
            // Seed layout once after the first frame (DS.dock_id is captured
            // by the dockspace_in_window body before this block fires).
            if (!DS.has_initial_layout && DS.dock_id != 0u) {
                setup_default_layout(DS.dock_id)
                DS.has_initial_layout = true
            }
            dock_window(FILES, (text = "Files", closable = false,
                                flags = ImGuiWindowFlags.None)) {
                text(FILES_LINE_1, (text = "file_a.das"))
                text(FILES_LINE_2, (text = "file_b.das"))
                text(FILES_LINE_3, (text = "file_c.das"))
            }
            dock_window(OUTPUT, (text = "Output", closable = false,
                                 flags = ImGuiWindowFlags.None)) {
                text(OUTPUT_HEADER, (text = "Build output:"))
                text(OUTPUT_LINE_1, (text = "[INFO] compiled file_a.das"))
                text(OUTPUT_LINE_2, (text = "[INFO] compiled file_b.das"))
                text(OUTPUT_LINE_3, (text = "[INFO] done in 142ms"))
            }
        }
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
