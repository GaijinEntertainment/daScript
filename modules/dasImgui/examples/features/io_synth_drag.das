options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: io_synth_drag — mouse-drag synthesis target.
// SHOWS:   A slider driven by a synthetic drag. The playwright `drag` helper
//          resolves the target's bbox center and composes an `imgui_mouse_play`
//          timeline (warp → press → drag → release) on the owned synth primitives;
//          the slider value moves, observable through imgui_snapshot.
// DRIVE:   # playwright: drag(d, "MAIN_WIN/DEMO_SLIDER", 120.0, 0.0, 8)
//          # or imgui_mouse_play with an explicit (t_ms,kind,x,y|button,action) event list
//          curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '{active: .io.active_widget, value: .globals."MAIN_WIN/DEMO_SLIDER".payload.value}'
// STANDALONE: daslang.exe modules/dasImgui/examples/features/io_synth_drag.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/io_synth_drag.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/io_synth_drag.das
// =============================================================================

[export]
def init() {
    harness_init("Phase 2.2 — io_synth_drag", 800, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
    DEMO_SLIDER.bounds = (0.0f, 1000.0f)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 220.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "io_synth_drag — phase 2.2", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("Synthetic drag: mouse press → moves → release (via imgui_mouse_play).")
        text("During the drag, io.active_widget == 'MAIN_WIN/DEMO_SLIDER'.")
        slider_float(DEMO_SLIDER, (text = "Drag me"))
        text("slider value = {DEMO_SLIDER.value}")
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
