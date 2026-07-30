options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: containers_layout — Phase 0b.3 tab_bar / tab_item / tree_node /
//          collapsing_header / menu_bar / menu containers.
// SHOWS:   nested containers, hierarchical registry keys spanning 3+ levels,
//          imgui_open / imgui_close on tree nodes & headers.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          # tree leaves render as e.g. "LAYOUT_WIN/SETTINGS_TAB/PHYSICS_TREE/GRAVITY"
//          curl -X POST -d '{"name":"imgui_open","args":{"target":"LAYOUT_WIN/SETTINGS_TAB/PHYSICS_TREE"}}' \
//                       localhost:9090/command
//          curl -X POST -d '{"name":"imgui_force_set",
//                            "args":{"target":"LAYOUT_WIN/SETTINGS_TAB/PHYSICS_TREE/GRAVITY","value":12.5}}' \
//                       localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/containers_layout.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/containers_layout.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/containers_layout.das
// =============================================================================

[export]
def init() {
    harness_init("Phase 0b.3 — containers (layout family)", 800, 700)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    GRAVITY.bounds = (0.0f, 50.0f)
    DENSITY.bounds = (0.0f, 10.0f)
    LIGHT.bounds = (0.0f, 1.0f)

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(720.0f, 640.0f), ImGuiCond.FirstUseEver)
    window(LAYOUT_WIN, (text = "Layout demo", closable = false,
                        flags = ImGuiWindowFlags.MenuBar)) {
        // Menu bar inside the window — only available when window flags
        // include MenuBar.
        menu_bar(MAIN_BAR) {
            menu(FILE_MENU, (text = "File", enabled = true)) {
                menu_item(FILE_NEW, (text = "New", shortcut = "Ctrl+N"))
                menu_item(FILE_OPEN, (text = "Open", shortcut = "Ctrl+O"))
            }
            menu(EDIT_MENU, (text = "Edit", enabled = true)) {
                menu_item(EDIT_UNDO, (text = "Undo", shortcut = "Ctrl+Z"))
                menu_item(EDIT_REDO, (text = "Redo", shortcut = "Ctrl+Y"))
            }
        }

        text("tab_bar with three tab_items")
        separator()

        tab_bar(MAIN_TABS, (text = "MainTabs",
                            flags = ImGuiTabBarFlags.None)) {
            // ----- Settings tab -----
            tab_item(SETTINGS_TAB, (text = "Settings", closable = false,
                                    flags = ImGuiTabItemFlags.None)) {
                text("Use tree nodes to organise grouped settings")
                spacing()

                tree_node(PHYSICS_TREE, (text = "Physics",
                                         flags = ImGuiTreeNodeFlags.DefaultOpen)) {
                    slider_float(GRAVITY, (text = "gravity"))
                    text("gravity = {GRAVITY.value}")
                    slider_float(DENSITY, (text = "density"))
                    text("density = {DENSITY.value}")
                }
                tree_node(RENDER_TREE, (text = "Render",
                                        flags = ImGuiTreeNodeFlags.DefaultOpen)) {
                    checkbox(WIREFRAME, (text = "Wireframe"))
                    text("wireframe = {WIREFRAME.value}")
                    slider_float(LIGHT, (text = "light"))
                    text("light = {LIGHT.value}")
                }
            }

            // ----- Audio tab -----
            tab_item(AUDIO_TAB, (text = "Audio", closable = false,
                                 flags = ImGuiTabItemFlags.None)) {
                checkbox(MUTE, (text = "Mute"))
                text("mute = {MUTE.value}")
            }

            // ----- About tab -----
            tab_item(ABOUT_TAB, (text = "About", closable = false,
                                 flags = ImGuiTabItemFlags.None)) {
                text("Phase 0b.3 — containers")
                text("hierarchical registry path keys")
            }
        }

        spacing()
        text("collapsing_header — like tree_node but no End-pair")
        separator()
        collapsing_header(LOG_HEADER, (text = "Log", closable = false,
                                       flags = ImGuiTreeNodeFlags.DefaultOpen)) {
            text("frame: {LAYOUT_WIN.size.x} x {LAYOUT_WIN.size.y}")
            text("settings tab is open: yes (tab_item.open = {SETTINGS_TAB.open})")
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
    //! Standalone entrypoint. daslang-live drives init/update/shutdown
    //! directly and ignores main(); plain `daslang.exe <file>` runs this loop.
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
