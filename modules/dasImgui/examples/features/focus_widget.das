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
// FEATURE: focus_widget — Phase 2.4 keyboard focus dispatch.
// SHOWS:   imgui_focus live_command — server inserts target's path into
//          g_pending_focus; widget_prelude consumes the entry on the
//          target's next render via SetKeyboardFocusHere(0). The example
//          renders a button and an input_text WITHOUT auto-focus, so by
//          default no widget holds keyboard focus.
// DRIVE:   curl -X POST -d '{"name":"imgui_focus","args":{"target":"DEMO_INPUT"}}' \
//              localhost:9090/command
//          curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals.DEMO_INPUT.focus'           # → true within ~1 frame
//          curl -X POST -d '{"name":"imgui_focus","args":{"target":"DEMO_TEXT"}}' \
//              localhost:9090/command                     # → Result.err (display-only)
// STANDALONE: daslang.exe modules/dasImgui/examples/features/focus_widget.das
// LIVE:       daslang-live modules/dasImgui/examples/features/focus_widget.das
// =============================================================================

[export]
def init() {
    live_create_window("Phase 2.4 — focus_widget", 800, 600)
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

    SetNextWindowSize(ImVec2(720.0, 240.0), ImGuiCond.Always)
    if (Begin("focus_widget — phase 2.4", null, ImGuiWindowFlags.None)) {
        Text("imgui_focus inserts into g_pending_focus; widget_prelude consumes.")
        Text("DEMO_TEXT is display-only — focus dispatch returns Result.err.")
        text_show(DEMO_TEXT, "static label")
        if (button(DEMO_BTN, (text = "I can be focused too"))) {
            print("DEMO_BTN clicked\n")
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
