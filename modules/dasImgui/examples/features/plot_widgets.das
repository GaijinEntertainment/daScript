options gen2
options _comment_hygiene = true

require imgui/imgui_harness
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
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/plot_widgets.das -- --headless --headless-frames=60
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
    harness_init("Phase 1.5 — plot widgets", 800, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    g_phase += 0.05f

    var sine_data : array<float>
    fill_sine(sine_data, g_phase)
    var hist_data : array<float>
    fill_histogram(hist_data)

    SetNextWindowSize(ImVec2(560.0, 360.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "plot widgets demo", closable = false,
                     flags = ImGuiWindowFlags.None)) {
        text("Two read-only plot widgets (telemetry-visible state):")
        plot_lines(SINE_WAVE, "Sine wave", sine_data, -1.2f, 1.2f, float2(0.0f, 80.0f))
        plot_histogram(HISTOGRAM, "Hist", hist_data, 0.0f, 1.5f, float2(0.0f, 80.0f))
    }

    harness_end_frame()
}

[export]
def shutdown() {
    harness_shutdown()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
