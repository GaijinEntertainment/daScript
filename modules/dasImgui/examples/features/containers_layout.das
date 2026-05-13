options gen2

require imgui
require imgui_app
require glfw/glfw_boost
require opengl/opengl_boost
require live/glfw_live
require live/live_api
require live/live_commands
require live/live_vars
require live/opengl_live
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin

// =============================================================================
// FEATURE: containers_layout — Phase 0b.3 tab_bar / tab_item / tree_node /
//          collapsing_header / menu_bar / menu containers.
// SHOWS:   nested containers, hierarchical registry keys spanning 3+ levels,
//          imgui_open / imgui_close on tree nodes & headers.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          # tree leaves render as e.g. "LAYOUT_WIN/SETTINGS_TAB/PHYSICS_TREE/GRAVITY"
//          curl -X POST -d '{"name":"imgui_open","args":{"target":"LAYOUT_WIN/SETTINGS_TAB/PHYSICS_TREE"}}' \
//                       localhost:9090/command
//          curl -X POST -d '{"name":"imgui_set",
//                            "args":{"target":"LAYOUT_WIN/SETTINGS_TAB/PHYSICS_TREE/GRAVITY","value":12.5}}' \
//                       localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/containers_layout.das
// LIVE:       daslang-live modules/dasImgui/examples/features/containers_layout.das
// =============================================================================

[export]
def init() {
    live_create_window("Phase 0b.3 — containers (layout family)", 800, 700)
    live_imgui_init(live_window)
    var io & = unsafe(GetIO())
    io.FontGlobalScale = 1.5
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplOpenGL3_NewFrame()
    ImGui_ImplGlfw_NewFrame()
    NewFrame()

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

        Text("tab_bar with three tab_items")
        Separator()

        tab_bar(MAIN_TABS, (text = "MainTabs",
                            flags = ImGuiTabBarFlags.None)) {
            // ----- Settings tab -----
            tab_item(SETTINGS_TAB, (text = "Settings", closable = false,
                                    flags = ImGuiTabItemFlags.None)) {
                Text("Use tree nodes to organise grouped settings")
                Spacing()

                tree_node(PHYSICS_TREE, (text = "Physics",
                                         flags = ImGuiTreeNodeFlags.DefaultOpen)) {
                    slider_float(GRAVITY, (text = "gravity"))
                    Text("gravity = {GRAVITY.value}")
                    slider_float(DENSITY, (text = "density"))
                    Text("density = {DENSITY.value}")
                }
                tree_node(RENDER_TREE, (text = "Render",
                                        flags = ImGuiTreeNodeFlags.DefaultOpen)) {
                    checkbox(WIREFRAME, (text = "Wireframe"))
                    Text("wireframe = {WIREFRAME.value}")
                    slider_float(LIGHT, (text = "light"))
                    Text("light = {LIGHT.value}")
                }
            }

            // ----- Audio tab -----
            tab_item(AUDIO_TAB, (text = "Audio", closable = false,
                                 flags = ImGuiTabItemFlags.None)) {
                checkbox(MUTE, (text = "Mute"))
                Text("mute = {MUTE.value}")
            }

            // ----- About tab -----
            tab_item(ABOUT_TAB, (text = "About", closable = false,
                                 flags = ImGuiTabItemFlags.None)) {
                Text("Phase 0b.3 — containers")
                Text("hierarchical registry path keys")
            }
        }

        Spacing()
        Text("collapsing_header — like tree_node but no End-pair")
        Separator()
        collapsing_header(LOG_HEADER, (text = "Log",
                                       flags = ImGuiTreeNodeFlags.DefaultOpen)) {
            Text("frame: {LAYOUT_WIN.size.x} x {LAYOUT_WIN.size.y}")
            Text("settings tab is open: yes (tab_item.open = {SETTINGS_TAB.open})")
        }
    }

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    ImGui_ImplOpenGL3_RenderDrawData(GetDrawData())

    live_end_frame()
}

[export]
def shutdown() {
    live_imgui_shutdown()
    live_destroy_window()
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
