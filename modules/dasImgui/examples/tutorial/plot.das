options gen2
options _comment_hygiene = true

require math
require imgui
require imgui_app
require opengl/opengl_boost
require live/glfw_live
require live/live_api
require live/live_commands
require live/live_vars
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_visual_aids

// =============================================================================
// TUTORIAL: plot — four read-only sample-plot widgets.
//
//   plot_lines(IDENT, "title", values, scale_min, scale_max, size)
//   plot_histogram(IDENT, "title", values, scale_min, scale_max, size)
//     Array form — pass a per-frame array<float>. ImGui copies it
//     synchronously. Pass scale_min = scale_max = FLT_MAX to auto-fit
//     bounds to the data; explicit bounds avoid the per-frame jitter.
//
//   plot_lines_getter(IDENT, "title", count, getter, overlay,
//                     scale_min, scale_max, size)
//   plot_histogram_getter(IDENT, "title", count, getter, overlay,
//                         scale_min, scale_max, size)
//     Lambda form — callback (idx : int) : float fires once per sample
//     synchronously inside the C call. Use for synthesized signals or
//     virtualized buffers (avoids building a backing array).
//
// All four share PlotState — title + sample telemetry round-trips into
// the snapshot payload for visual regression testing.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/plot.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/plot.das
// =============================================================================

let SAMPLES = 64
let FLT_MAX = 3.40282347e38f

var private g_phase : float = 0.0f

def fill_sine(var arr : array<float>; phase : float; n : int) {
    arr |> resize(n)
    for (i in range(n)) {
        arr[i] = sin(float(i) * 0.2f + phase)
    }
}

def fill_bars(var arr : array<float>) {
    arr |> resize(8)
    for (i in range(8)) {
        arr[i] = float(i) * 0.15f + 0.1f
    }
}

[export]
def init() {
    live_create_window("dasImgui plot tutorial", 800, 720)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.4
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    g_phase += 0.05f
    var sine_data : array<float>
    fill_sine(sine_data, g_phase, SAMPLES)
    var bar_data : array<float>
    fill_bars(bar_data)

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(760.0f, 680.0f), ImGuiCond.Always)
    window(PL_WIN, (text = "plot tutorial", closable = false,
                    flags = ImGuiWindowFlags.None)) {

        text("Read-only sample plots - PlotState records title + samples each frame.")
        text(PL_HINT, (text = "Array form (lines / histogram) vs lambda form (lines_getter / histogram_getter)."))
        separator()

        // ---- Stage 1: plot_lines (array) — sine wave with explicit bounds ----
        plot_lines(PL_LINES, "Sine wave", sine_data, -1.2f, 1.2f, float2(0.0f, 80.0f))
        spacing()

        // ---- Stage 2: plot_histogram (array) — fixed bars ----
        plot_histogram(PL_HIST, "Histogram", bar_data, 0.0f, 1.5f, float2(0.0f, 80.0f))
        spacing()

        // ---- Stage 3: plot_lines_getter (lambda) — no backing array ----
        plot_lines_getter(PL_LINES_G, "sin(i*0.1)", SAMPLES,
                          @(idx : int) : float => sin(float(idx) * 0.1f + g_phase),
                          "", -1.2f, 1.2f, float2(0.0f, 80.0f))
        spacing()

        // ---- Stage 4: plot_histogram_getter (lambda) ----
        plot_histogram_getter(PL_HIST_G, "abs(sin(i*0.1))", SAMPLES,
                              @(idx : int) : float => abs(sin(float(idx) * 0.1f + g_phase)),
                              "", 0.0f, 1.2f, float2(0.0f, 80.0f))
    }

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    live_imgui_render()

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
