options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: glfw_synth_keys — GLFW-side synthetic keyboard driver (chain shim).
// SHOWS:   `key_press` / `key_release` / `key_char` / `key_type` / `key_chord`
//          / `key_play` live commands inject events into the GLFW key+char
//          callback chain. ImGui_ImplGlfw_KeyCallback / _CharCallback ingest
//          them normally, so io.MousePos / focus / text-input buffers update
//          exactly as they would with real hardware input.
// DRIVE:   curl -X POST -d '{"name":"imgui_focus","args":{"target":"MAIN_WIN/NAME_INPUT"}}' \
//              localhost:9090/command
//          curl -X POST -d '{"name":"key_type","args":{"text":"Hello, World!","total_duration_s":3.0}}' \
//              localhost:9090/command
//          curl -X POST -d '{"name":"key_chord","args":{"mods":["Ctrl"],"key":"A"}}' \
//              localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/glfw_synth_keys.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/glfw_synth_keys.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/glfw_synth_keys.das
// =============================================================================

[export]
def init() {
    harness_init("glfw_synth_keys", 720, 320)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowPos(ImVec2(40.0, 40.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(640.0, 240.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "synth keys demo", closable = false,
                      flags = ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove)) {
        text("Drive `key_type`, `key_chord`, `key_play` to feed the input below.")
        text("Use `imgui_focus` to put keyboard focus on NAME_INPUT first.")
        input_text(NAME_INPUT, (text = "Name"))
        text("buffer = {NAME_INPUT.value}")
        text("Ctrl held: {IsKeyDown(ImGuiKey.LeftCtrl) || IsKeyDown(ImGuiKey.RightCtrl)}")
        text("Shift held: {IsKeyDown(ImGuiKey.LeftShift) || IsKeyDown(ImGuiKey.RightShift)}")
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
