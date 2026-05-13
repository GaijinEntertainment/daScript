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
// FEATURE: columns_demo — §4.6 `columns(N, ${a}, ${b}, …)` call_macro.
// SHOWS:   - `columns(3, ${col0}, ${col1}, ${col2})` expands to
//            columns_open(3); invoke(col0); next_col(); invoke(col1);
//            next_col(); invoke(col2); columns_close().
//          - Each column's children are addressed by the surrounding window's
//            path (columns is not a container — it doesn't push a path leaf).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/columns_demo.das
// LIVE:       daslang-live modules/dasImgui/examples/features/columns_demo.das
// =============================================================================

[export]
def init() {
    live_create_window("§4.6 — columns demo", 640, 480)
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
    SetNextWindowSize(ImVec2(560.0f, 380.0f), ImGuiCond.FirstUseEver)
    window(COLS_WIN, (text = "Columns demo", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        Text("Three columns:")
        columns(3,
                ${
                    Text("Col 0")
                    button(C0_BTN, (text = "Col 0 btn"))
                },
                ${
                    Text("Col 1")
                    button(C1_BTN, (text = "Col 1 btn"))
                },
                ${
                    Text("Col 2")
                    button(C2_BTN, (text = "Col 2 btn"))
                })
        Text("After columns block")
        button(AFTER_BTN, (text = "After"))
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
