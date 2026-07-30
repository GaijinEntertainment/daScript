options gen2
options _comment_hygiene = true

require imgui
require imgui_app
require opengl/opengl_boost
require live/glfw_live
require live/live_api
require live/live_commands
require live/live_vars
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_layout_builtin
require imgui/imgui_visual_aids

// =============================================================================
// TUTORIAL: layout — split_h / split_v / dock_left on a single panel.
//
// Three boost layout helpers compose into a classic IDE-style layout:
//   SIDEBAR     : dock_left   — fixed-width left rail with a draggable edge
//   SPLIT_VERT  : split_v     — splits the main area into top + bottom
//   SPLIT_MAIN  : split_h     — splits the top area into left + right
//
// Each helper takes a state struct (registered automatically), an `init`
// value, optional `bounds`, and a block (`${ ... }`) per pane. The state's
// `value` field holds either the split fraction (split_*) or the pane width
// in pixels (dock_*). All three are draggable at runtime.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/layout.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/layout.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN","value":0.7}}' localhost:9090/command
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"LAYOUT_WIN/SPLIT_VERT","value":0.4}}'           localhost:9090/command
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"LAYOUT_WIN/SIDEBAR","value":220.0}}'            localhost:9090/command
//
// Note: the boost ``window(LAYOUT_WIN, ...)`` wrapper pushes the window's
// name as a path prefix on every nested widget, so live targets are
// path-qualified (no bare ``SIDEBAR``).
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui layout tutorial", 1024, 720)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.5
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    // One window holds the whole layout. dock_left carves out the sidebar;
    // split_v then split_h compose the main pane.
    SetNextWindowPos(ImVec2(60.0f, 60.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(900.0f, 600.0f), ImGuiCond.Always)
    window(LAYOUT_WIN, (text = "IDE layout",
                        closable = false,
                        flags = ImGuiWindowFlags.None)) {

        // Left rail: 200 px by default, draggable between 80 and 320 px.
        dock_left(SIDEBAR, (init = 200.0f, bounds = (80.0f, 320.0f))) {
            text("Sidebar")
            spacing(LO_SP_1)
            button(EXPLORE_BTN, (text = "Explore"))
            button(SEARCH_BTN,  (text = "Search"))
            button(SOURCE_BTN,  (text = "Source"))
        }

        // Main pane: top/bottom (split_v) where top is left/right (split_h).
        split_v(SPLIT_VERT, (init = 0.65f, bounds = (0.1f, 0.9f)),
                ${
                    split_h(SPLIT_MAIN, (init = 0.4f, bounds = (0.1f, 0.9f)),
                            ${
                                text("Files")
                                button(FILE_A_BTN, (text = "main.das"))
                                button(FILE_B_BTN, (text = "lib.das"))
                            },
                            ${
                                text("Editor")
                                text("// drag the splitters -")
                                text("// the pane bounds")
                                text("// stay clipped.")
                            })
                },
                ${
                    text("Output")
                    text("> ready.")
                    button(CLEAR_BTN, (text = "Clear output"))
                })
    }

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    live_imgui_render()

    live_end_frame()
}

[export]
def shutdown() {
    live_imgui_shutdown()
    live_destroy_window()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
