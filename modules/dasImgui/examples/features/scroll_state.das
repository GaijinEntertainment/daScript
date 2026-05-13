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
// FEATURE: scroll_state — Phase 1.2 scroll capture for window + child.
// SHOWS:   WindowState and ChildState now carry `scroll` and `scroll_max`
//          (float2). Each frame the container body reads GetScrollX/Y/MaxX/MaxY
//          inside Begin → invoke(blk) → End and stores them on state. The
//          standard serializer surfaces them through imgui_snapshot.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          → globals."SCROLLY_WIN".payload.scroll      = [0, 0..max_y]
//                  .scroll_max  = [0, max_y]
//                  .size        = [400, 200]
// STANDALONE: daslang.exe modules/dasImgui/examples/features/scroll_state.das
// LIVE:       daslang-live modules/dasImgui/examples/features/scroll_state.das
// =============================================================================

[export]
def init() {
    live_create_window("Phase 1.2 — scroll state", 800, 600)
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

    SetNextWindowSize(ImVec2(420.0, 240.0), ImGuiCond.Always)
    window(SCROLLY_WIN, (text = "Scrolly window", closable = false,
                        flags = ImGuiWindowFlags.None)) {
        Text("Window scroll values appear in SCROLLY_WIN.payload.scroll")
        Text("along with scroll_max. Forty rows below to make it scroll.")
        for (i in range(0, 40)) {
            Text("row {i}: lorem ipsum dolor sit amet consectetur adipiscing elit")
        }
    }

    SetNextWindowPos(ImVec2(440.0, 0.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(360.0, 240.0), ImGuiCond.Always)
    window(NESTED_WIN, (text = "Child host", closable = false,
                       flags = ImGuiWindowFlags.None)) {
        Text("Below: a fixed-size child region with its own scroll state.")
        child(LOG_REGION, (text = "##log", size = float2(320.0, 140.0),
                           child_flags = ImGuiChildFlags.Border,
                           window_flags = ImGuiWindowFlags.None)) {
            for (i in range(0, 25)) {
                Text("log line {i}")
            }
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
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
