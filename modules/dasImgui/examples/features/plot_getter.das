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
require math

// =============================================================================
// FEATURE: plot_getter — Phase 2.7 procedural plot widgets.
// SHOWS:   plot_lines_getter / plot_histogram_getter — values produced by a
//          per-frame lambda instead of a backing array<float>. The lambda
//          fires synchronously inside the C call, one invocation per
//          sample. Useful for synthesized signals (sine, noise) or
//          virtualized buffers.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals.SINE_PLOT'
// STANDALONE: daslang.exe modules/dasImgui/examples/features/plot_getter.das
// LIVE:       daslang-live modules/dasImgui/examples/features/plot_getter.das
// =============================================================================

let SAMPLES = 64

[export]
def init() {
    live_create_window("Phase 2.7 — plot_getter", 800, 600)
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

    SetNextWindowSize(ImVec2(720.0, 360.0), ImGuiCond.Always)
    if (Begin("plot_getter — phase 2.7", null, ImGuiWindowFlags.None)) {
        Text("Sine wave generated per-sample by the getter lambda.")
        plot_lines_getter(SINE_PLOT, "sin(i*0.1)", SAMPLES,
                          @(idx : int) : float => sin(float(idx) * 0.1f),
                          "", 3.40282347e38f, 3.40282347e38f,
                          float2(640.0, 120.0))
        plot_histogram_getter(BARS_PLOT, "abs(sin(i*0.1))", SAMPLES,
                              @(idx : int) : float => abs(sin(float(idx) * 0.1f)),
                              "", 3.40282347e38f, 3.40282347e38f,
                              float2(640.0, 120.0))
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
