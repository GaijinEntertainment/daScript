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
require imgui/imgui_layout_builtin
require imgui/imgui_visual_aids

// =============================================================================
// FEATURE: layout_helpers — §4.6 split_h / split_v / dock_left.
// SHOWS:   - `split_h(SPLIT_MAIN, init=0.3, ${left}, ${right})` — draggable
//            vertical splitter; state.value is the left fraction (0..1).
//          - `split_v(SPLIT_VERT, init=0.4, ${top}, ${bottom})` — draggable
//            horizontal splitter.
//          - `dock_left(SIDEBAR, init=180.0, bounds=(80.0, 320.0)) { ... }` —
//            sidebar with a draggable right edge; state.value is width (px).
//          - `apply_synth_io_override()` between Impl_NewFrame and NewFrame so
//            synth-mouse drag tests land (per feedback_synth_io_override_pattern).
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          curl -X POST -d '{"name":"imgui_set",
//                            "args":{"target":"LAYOUT_WIN/SPLIT_MAIN","value":0.7}}' \
//                       localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/layout_helpers.das
// LIVE:       daslang-live modules/dasImgui/examples/features/layout_helpers.das
// =============================================================================

[export]
def init() {
    live_create_window("§4.6 — layout helpers", 800, 600)
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
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(720.0f, 540.0f), ImGuiCond.FirstUseEver)
    window(LAYOUT_WIN, (text = "Layout demo", closable = false,
                        flags = ImGuiWindowFlags.None)) {
        dock_left(SIDEBAR, (init = 180.0f, bounds = (80.0f, 320.0f))) {
            Text("Sidebar")
            button(SIDEBAR_BTN, (text = "Sidebar btn"))
        }
        // Main pane flows after dock_left's trailing SameLine. split_v stacks
        // split_h on top of a row of buttons inside it.
        split_v(SPLIT_VERT, (init = 0.5f, bounds = (0.1f, 0.9f)),
                ${
                    split_h(SPLIT_MAIN, (init = 0.3f, bounds = (0.1f, 0.9f)),
                            ${
                                Text("Left")
                                button(LEFT_BTN, (text = "Left"))
                            },
                            ${
                                Text("Right")
                                button(RIGHT_BTN, (text = "Right"))
                            })
                },
                ${
                    Text("Bottom pane")
                    button(BOTTOM_BTN, (text = "Bottom"))
                })
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
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
