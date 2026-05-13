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
// FEATURE: io_synth_text — Phase 2.2 L1 keyboard input synthesis.
// SHOWS:   imgui_type_text live_command — coroutine pushes one
//          AddInputCharactersUTF8 per frame to whichever widget holds
//          keyboard focus. The example auto-focuses DEMO_INPUT on first
//          render via SetKeyboardFocusHere(0), then chars stream in.
//          Phase 2.4 will add a `target`-driven focus action so callers can
//          aim type_text at any widget without owning the focus dance.
// DRIVE:   curl -X POST -d '{"name":"imgui_type_text","args":{"target":"DEMO_INPUT","text":"hello"}}' \
//              localhost:9090/command
//          curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals.DEMO_INPUT.payload.value'
// STANDALONE: daslang.exe modules/dasImgui/examples/features/io_synth_text.das
// LIVE:       daslang-live modules/dasImgui/examples/features/io_synth_text.das
// =============================================================================

var auto_focused = false

[export]
def init() {
    live_create_window("Phase 2.2 — io_synth_text", 800, 600)
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
    advance_coroutines()
    NewFrame()

    SetNextWindowSize(ImVec2(720.0, 220.0), ImGuiCond.Always)
    if (Begin("io_synth_text — phase 2.2", null, ImGuiWindowFlags.None)) {
        Text("L1 keyboard: imgui_type_text streams chars one frame each.")
        Text("DEMO_INPUT auto-focused on first render via SetKeyboardFocusHere.")
        if (!auto_focused) {
            SetKeyboardFocusHere(0)
            auto_focused = true
        }
        input_text(DEMO_INPUT, (text = "Type here"))
        Text("buffer = {DEMO_INPUT.value}")
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
