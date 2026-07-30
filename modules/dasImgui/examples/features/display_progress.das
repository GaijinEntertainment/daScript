options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require math

// =============================================================================
// FEATURE: progress_bar — ProgressBarState-backed display widget.
// SHOWS:   progress_bar(fraction=…, size=…, overlay=…) emits a payload
//          {fraction, size, overlay}. Animates here by driving fraction off
//          a wall-clock sine so the test can assert non-zero motion.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals."MAIN_WIN/PB_DRIVEN".payload.fraction'
// STANDALONE: daslang.exe modules/dasImgui/examples/features/display_progress.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/display_progress.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/display_progress.das
// =============================================================================

var private g_t : float = 0.0f

[export]
def init() {
    harness_init("progress_bar — boost v2 §3.2", 720, 360)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    g_t += 1.0f / 60.0f
    let frac = 0.5f + 0.5f * sin(g_t * 2.0f)

    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 300.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "progress_bar", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(PB_LEAD, (text = "Three progress bars — static, driven, no-overlay."))
        progress_bar(PB_STATIC, (fraction = 0.33f, size = float2(-1.0f, 0.0f),
                                  overlay = "33%"))
        progress_bar(PB_DRIVEN, (fraction = frac, size = float2(-1.0f, 0.0f),
                                  overlay = "{int(frac * 100.0f)}%"))
        progress_bar(PB_PLAIN, (fraction = 0.75f))
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
