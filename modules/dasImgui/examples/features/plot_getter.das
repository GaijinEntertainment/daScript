options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require math

// =============================================================================
// FEATURE: plot_getter — Phase 2.7 procedural plot widgets.
// SHOWS:   plot_lines_getter / plot_histogram_getter — values produced by a
//          per-frame lambda instead of a backing array<float>. The lambda
//          fires synchronously inside the C call, one invocation per
//          sample. Useful for synthesized signals (sine, noise) or
//          virtualized buffers.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals."MAIN_WIN/SINE_PLOT"'
// STANDALONE: daslang.exe modules/dasImgui/examples/features/plot_getter.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/plot_getter.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/plot_getter.das
// =============================================================================

let SAMPLES = 64

[export]
def init() {
    harness_init("Phase 2.7 — plot_getter", 800, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 360.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "plot_getter — phase 2.7", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("Sine wave generated per-sample by the getter lambda.")
        plot_lines_getter(SINE_PLOT, "sin(i*0.1)", SAMPLES,
                          @(idx : int) : float => sin(float(idx) * 0.1f),
                          "", 3.40282347e38f, 3.40282347e38f,
                          float2(640.0, 120.0))
        plot_histogram_getter(BARS_PLOT, "abs(sin(i*0.1))", SAMPLES,
                              @(idx : int) : float => abs(sin(float(idx) * 0.1f)),
                              "", 3.40282347e38f, 3.40282347e38f,
                              float2(640.0, 120.0))
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
