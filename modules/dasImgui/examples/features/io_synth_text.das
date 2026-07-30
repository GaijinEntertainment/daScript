options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: io_synth_text — keyboard input synthesis target.
// SHOWS:   A text input filled by synthetic keystrokes. The playwright `type_text`
//          helper focuses the target, then streams the text via `imgui_key_type`
//          (key+char events on the owned synth primitives) into the focused widget.
//          The example also auto-focuses DEMO_INPUT on first render via
//          SetKeyboardFocusHere(0), so a bare imgui_key_type lands without a focus step.
// DRIVE:   curl -X POST -d '{"name":"imgui_key_type","args":{"text":"hello"}}' localhost:9090/command
//          curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals."MAIN_WIN/DEMO_INPUT".payload.value'
// STANDALONE: daslang.exe modules/dasImgui/examples/features/io_synth_text.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/io_synth_text.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/io_synth_text.das
// =============================================================================

var auto_focused = false

[export]
def init() {
    harness_init("Phase 2.2 — io_synth_text", 800, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 220.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "io_synth_text — phase 2.2", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("Synthetic keyboard: imgui_key_type streams key+char events.")
        text("DEMO_INPUT auto-focused on first render via SetKeyboardFocusHere.")
        if (!auto_focused) {
            SetKeyboardFocusHere(0)
            auto_focused = true
        }
        input_text(DEMO_INPUT, (text = "Type here"))
        text("buffer = {DEMO_INPUT.value}")
        input_text_multiline(DEMO_MULTI, (text = "Multi", size = float2(0.0f, 70.0f)))
        text("multi = {DEMO_MULTI.value}")
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
