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
// FEATURE: active_widget — Phase 2.1 foundation gate.
// SHOWS:   GetActiveID() binding (cherry-picked from imgui_internal.h) +
//          g_id_to_path / g_id_to_bbox reverse maps populated by widget_finalize +
//          io_jv() surfacing active_id/active_widget when something is active +
//          Result<T,string> wire format on imgui_click responses + with_await
//          per-command modifier merging (await/timeout_sec/fire_and_forget).
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.io'
//          curl -X POST -d '{"name":"imgui_click","args":{"target":"DEMO_BTN"}}' \
//              localhost:9090/command          # → {"ok": true, "value": "DEMO_BTN", ...}
//          curl -X POST -d '{"name":"imgui_click","args":{"target":"DEMO_BTN","await":{"fire_and_forget":true}}}' \
//              localhost:9090/command          # → bare {"ok":true,"value":"..."} (no timeout_sec)
// STANDALONE: daslang.exe modules/dasImgui/examples/features/active_widget.das
// LIVE:       daslang-live modules/dasImgui/examples/features/active_widget.das
// =============================================================================

[export]
def init() {
    live_create_window("Phase 2.1 — active_widget foundation", 800, 600)
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

    SetNextWindowSize(ImVec2(720.0, 280.0), ImGuiCond.Always)
    if (Begin("active_widget — phase 2.1", null, ImGuiWindowFlags.None)) {
        Text("Foundation gate: GetActiveID + reverse maps + Result wire.")
        Text("Drag the slider to make snapshot.io.active_widget resolve.")

        // Single button + single slider — minimal surface; the test reads
        // snapshot.io.active_widget, snapshot.io.mouse_buttons, and the
        // imgui_click response wire shape.
        if (button(DEMO_BTN, (text = "Click me"))) {
            print("DEMO_BTN clicked, count={DEMO_BTN.click_count}\n")
        }
        Text("clicks: {DEMO_BTN.click_count}")

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
