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
// FEATURE: io_synth_drag — Phase 2.2 L1 mouse-drag synthesis.
// SHOWS:   imgui_drag live_command — coroutine-driven AddMousePosEvent +
//          AddMouseButtonEvent stream resolves the target via g_registry,
//          presses at bbox center, walks N interpolated steps, releases.
//          During the drag, snapshot.io.active_widget binds to DEMO_SLIDER
//          and the slider value moves; both observable through imgui_snapshot.
// DRIVE:   curl -X POST -d '{"name":"imgui_drag","args":{"target":"DEMO_SLIDER","dx":120.0,"dy":0.0,"steps":8}}' \
//              localhost:9090/command
//          curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '{active: .io.active_widget, value: .globals.DEMO_SLIDER.payload.value}'
// STANDALONE: daslang.exe modules/dasImgui/examples/features/io_synth_drag.das
// LIVE:       daslang-live modules/dasImgui/examples/features/io_synth_drag.das
// =============================================================================

[export]
def init() {
    live_create_window("Phase 2.2 — io_synth_drag", 800, 600)
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

    SetNextWindowSize(ImVec2(720.0, 220.0), ImGuiCond.Always)
    if (Begin("io_synth_drag — phase 2.2", null, ImGuiWindowFlags.None)) {
        Text("L1 drag: imgui_drag synthesizes mouse press → moves → release.")
        Text("During the drag, io.active_widget == 'DEMO_SLIDER'.")
        slider_float(DEMO_SLIDER, (text = "Drag me"))
        Text("slider value = {DEMO_SLIDER.value}")
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
