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
require math

// =============================================================================
// FEATURE: plot_widgets — Phase 1.5 plot_lines + plot_histogram (array form).
// SHOWS:   Two display widgets backed by PlotState — title, bounds, samples
//          captured each frame for telemetry. Values come per-frame as an
//          array<float> parameter (not stored on state). Lambda-getter form
//          deferred to 1.6 alongside the ImGuiPlotGetter shim audit.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          → globals."MAIN_WIN/SINE_WAVE".payload = {title, bounds, samples}
//                  ."MAIN_WIN/HISTOGRAM".payload  = {...}
// STANDALONE: daslang.exe modules/dasImgui/examples/features/plot_widgets.das
// LIVE:       daslang-live modules/dasImgui/examples/features/plot_widgets.das
// =============================================================================

var private g_phase : float = 0.0f

def fill_sine(var arr : array<float>; phase : float) {
    arr |> resize(64)
    for (i in range(0, length(arr))) {
        arr[i] = sin(float(i) * 0.2f + phase)
    }
}

def fill_histogram(var arr : array<float>) {
    arr |> resize(8)
    for (i in range(0, length(arr))) {
        arr[i] = float(i) * 0.15f + 0.1f
    }
}

[export]
def init() {
    live_create_window("Phase 1.5 — plot widgets", 800, 600)
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

    g_phase += 0.05f

    var sine_data : array<float>
    fill_sine(sine_data, g_phase)
    var hist_data : array<float>
    fill_histogram(hist_data)

    SetNextWindowSize(ImVec2(560.0, 360.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "plot widgets demo", closable = false,
                     flags = ImGuiWindowFlags.None)) {
        Text("Two read-only plot widgets (telemetry-visible state):")
        plot_lines(SINE_WAVE, "Sine wave", sine_data, -1.2f, 1.2f, float2(0.0f, 80.0f))
        plot_histogram(HISTOGRAM, "Hist", hist_data, 0.0f, 1.5f, float2(0.0f, 80.0f))
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
