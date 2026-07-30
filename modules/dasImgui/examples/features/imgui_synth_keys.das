options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: imgui_synth_keys — ImGui-bypass synthetic keyboard driver.
// SHOWS:   `imgui_key_press` / `imgui_key_release` / `imgui_key_char` /
//          `imgui_key_type` / `imgui_key_chord` / `imgui_key_play` live
//          commands inject events directly into ImGui's input queue,
//          skipping the GLFW backend chain. Ctrl-shortcuts (Ctrl+A etc.)
//          route correctly after a synth click — the bypass purges
//          backend events that would otherwise poison Shortcut() routing.
// DRIVE:   curl -X POST -d '{"name":"imgui_focus","args":{"target":"MAIN_WIN/NAME_INPUT"}}' \
//              localhost:9090/command
//          curl -X POST -d '{"name":"imgui_key_type","args":{"text":"Hello, World!","total_duration_s":3.0}}' \
//              localhost:9090/command
//          curl -X POST -d '{"name":"imgui_key_chord","args":{"mods":["Ctrl"],"key":"A"}}' \
//              localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/imgui_synth_keys.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/imgui_synth_keys.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/imgui_synth_keys.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_synth_keys", 720, 320)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowPos(ImVec2(40.0, 40.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(640.0, 240.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "synth keys demo (imgui bypass)", closable = false,
                      flags = ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove)) {
        text("Drive `imgui_key_type`, `imgui_key_chord`, `imgui_key_play` to feed the input below.")
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
