options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: await_quiescent — Phase 2.5 quiescence probe.
// SHOWS:   imgui_await — server-side liveness snapshot returning
//          {quiescent, frame, pending_coroutines, active_id}. Client polls
//          until `value.quiescent == true` (with timeout). Per-command
//          modifier `await={frames:N}` surfaces in the response wrap.
// DRIVE:   curl -X POST -d '{"name":"imgui_await"}' localhost:9090/command \
//              | jq '.value'
//          curl -X POST -d '{"name":"imgui_click","args":{"target":"MAIN_WIN/DEMO_SLIDER","await":{"await_frames":3}}}' \
//              localhost:9090/command           # response carries await_frames=3
// STANDALONE: daslang.exe modules/dasImgui/examples/features/await_quiescent.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/await_quiescent.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/await_quiescent.das
// =============================================================================

[export]
def init() {
    harness_init("Phase 2.5 — await_quiescent", 800, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
    DEMO_SLIDER.bounds = (0.0f, 1000.0f)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 200.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "await_quiescent — phase 2.5", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("imgui_await reports quiescent + frame + pending_coroutines + active_id.")
        slider_float(DEMO_SLIDER, (text = "Optional: drag to spawn a coroutine"))
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
