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
// FEATURE: io_mirror — Phase 1.3 io mirror in imgui_snapshot.
// SHOWS:   imgui_snapshot now folds an "io" key alongside "globals":
//            io.mouse_pos     = [x, y] (object {x, y} via JV(float2))
//            io.mouse_buttons = [left, middle, right] (bool array)
//          Read by io_jv() in imgui_boost_runtime; uses public API only
//          (GetMousePos, IsMouseDown). Active-widget resolution deferred —
//          ImGui::GetActiveID() isn't bound on the daslang side yet.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          → top-level "io": {"mouse_pos":{"x":..,"y":..}, "mouse_buttons":[..,..,..]}
// STANDALONE: daslang.exe modules/dasImgui/examples/features/io_mirror.das
// LIVE:       daslang-live modules/dasImgui/examples/features/io_mirror.das
// =============================================================================

[export]
def init() {
    live_create_window("Phase 1.3 — io mirror", 800, 600)
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

    SetNextWindowSize(ImVec2(560.0, 240.0), ImGuiCond.Always)
    window(IO_DEMO_WIN, (text = "io mirror demo", closable = false,
                        flags = ImGuiWindowFlags.None)) {
        Text("Move the mouse / hold a button — imgui_snapshot.io reflects it.")
        let pos = GetMousePos()
        Text("mouse_pos = ({pos.x}, {pos.y})")
        Text("mouse_buttons = [{IsMouseDown(ImGuiMouseButton.Left)}, {IsMouseDown(ImGuiMouseButton.Middle)}, {IsMouseDown(ImGuiMouseButton.Right)}]")

        // A button to anchor the snapshot's globals payload.
        button(NUDGE_BTN, (text = "Nudge"))
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
