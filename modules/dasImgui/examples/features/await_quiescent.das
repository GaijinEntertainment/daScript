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

// =============================================================================
// FEATURE: await_quiescent — Phase 2.5 quiescence probe.
// SHOWS:   imgui_await — server-side liveness snapshot returning
//          {quiescent, frame, pending_coroutines, active_id}. Client polls
//          until `value.quiescent == true` (with timeout). Per-command
//          modifier `await={frames:N}` surfaces in the response wrap.
// DRIVE:   curl -X POST -d '{"name":"imgui_await"}' localhost:9090/command \
//              | jq '.value'
//          curl -X POST -d '{"name":"imgui_drag","args":{"target":"DEMO_SLIDER","dx":80,"steps":6,"await":{"await_frames":3}}}' \
//              localhost:9090/command           # response carries await_frames=3
// STANDALONE: daslang.exe modules/dasImgui/examples/features/await_quiescent.das
// LIVE:       daslang-live modules/dasImgui/examples/features/await_quiescent.das
// =============================================================================

[export]
def init() {
    live_create_window("Phase 2.5 — await_quiescent", 800, 600)
    live_imgui_init(live_window)
    var io & = unsafe(GetIO())
    io.FontGlobalScale = 1.5
    DEMO_SLIDER.bounds = (0.0f, 1000.0f)
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplOpenGL3_NewFrame()
    ImGui_ImplGlfw_NewFrame()
    advance_coroutines()
    NewFrame()

    SetNextWindowSize(ImVec2(720.0, 200.0), ImGuiCond.Always)
    if (Begin("await_quiescent — phase 2.5", null, ImGuiWindowFlags.None)) {
        Text("imgui_await reports quiescent + frame + pending_coroutines + active_id.")
        slider_float(DEMO_SLIDER, (text = "Optional: drag to spawn a coroutine"))
    }
    End()

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
