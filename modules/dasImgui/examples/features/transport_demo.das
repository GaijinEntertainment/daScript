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
// FEATURE: transport_demo — Phase 2.6 lambda transport.
// SHOWS:   widgets/imgui_transport.das ships a `Transport` lambda type +
//          `live_api_transport(url)` constructor + `await_imgui` /
//          `send_with_await` polling helpers. Tests build one transport per
//          fixture and route every command through it; users can substitute
//          mocks/in-process buses without touching call sites.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals.DEMO_BTN'
// STANDALONE: daslang.exe modules/dasImgui/examples/features/transport_demo.das
// LIVE:       daslang-live modules/dasImgui/examples/features/transport_demo.das
// =============================================================================

[export]
def init() {
    live_create_window("Phase 2.6 — transport_demo", 800, 600)
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

    SetNextWindowSize(ImVec2(720.0, 200.0), ImGuiCond.Always)
    if (Begin("transport_demo — phase 2.6", null, ImGuiWindowFlags.None)) {
        Text("Transport lambda routes commands; await_imgui polls quiescence.")
        if (button(DEMO_BTN, (text = "Click me"))) {
            print("DEMO_BTN clicked, count={DEMO_BTN.click_count}\n")
        }
        Text("clicks: {DEMO_BTN.click_count}")
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
