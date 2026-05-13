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
require imgui/imgui_visual_aids

// =============================================================================
// FEATURE: glfw_synth_mouse — GLFW-side synthetic mouse driver (chain shim).
// SHOWS:   `mouse_pos` / `mouse_click` / `mouse_move_to` / `mouse_play` live
//          commands inject events into the GLFW callback chain. The whole app
//          sees them — ImGui-GLFW backend ingests the events normally, so
//          io.mouse_pos / io.mouse_buttons reflect synthetic input and
//          buttons increment click_count.
// DRIVE:   curl -X POST -d '{"name":"mouse_pos","args":{"x":200,"y":150}}' \
//              localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/glfw_synth_mouse.das
// LIVE:       daslang-live modules/dasImgui/examples/features/glfw_synth_mouse.das
// =============================================================================

[export]
def init() {
    live_create_window("glfw_synth_mouse", 640, 480)
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
    apply_synth_io_override()    // re-assert synth cursor pos + held modifier
                                 // keys after ImGui_ImplGlfw's per-frame poll.
    NewFrame()

    // Anchored window so SYNTH_BTN's bbox is stable across runs.
    SetNextWindowPos(ImVec2(50.0, 50.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(420.0, 280.0), ImGuiCond.Always)
    window(SYNTH_WIN, (text = "synth mouse demo", closable = false,
                       flags = ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove)) {
        let pos = GetMousePos()
        Text("mouse_pos = ({pos.x}, {pos.y})")
        Text("left = {IsMouseDown(ImGuiMouseButton.Left)}")
        button(SYNTH_BTN, (text = "      Click me      "))
        Text("SYNTH_BTN clicks: {SYNTH_BTN.click_count}")
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
