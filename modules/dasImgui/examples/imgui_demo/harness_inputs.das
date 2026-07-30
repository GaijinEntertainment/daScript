options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require inputs

// =============================================================================
// HARNESS: imgui_demo inputs — single-scene driver for the C++
//          ShowDemoWindowInputs port. ShowDemoWindowInputs does NOT open
//          its own window, so wrap in MAIN_WIN.
// SHOWS:   INPUTS_SECTION collapsing_header inside the host MAIN_WIN.
// STANDALONE: daslang.exe modules/dasImgui/examples/imgui_demo/harness_inputs.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/imgui_demo/harness_inputs.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/imgui_demo/harness_inputs.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_demo - Inputs", 760, 640)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    SetNextWindowSize(ImVec2(720.0, 600.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "Inputs", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        inputs::ShowDemoWindowInputs()
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
