options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: glfw_synth_mouse — GLFW-side synthetic mouse driver (chain shim).
// SHOWS:   `mouse_pos` / `mouse_click` / `mouse_move_to` / `mouse_play` live
//          commands inject events into the GLFW callback chain. The whole app
//          sees them — ImGui-GLFW backend ingests the events normally, so
//          io.mouse_pos / io.mouse_buttons reflect synthetic input and
//          buttons increment click_count.
// DRIVE:   curl -X POST -d '{"name":"mouse_pos","args":{"x":200,"y":150}}' \
//              localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/glfw_synth_mouse.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/glfw_synth_mouse.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/glfw_synth_mouse.das
// =============================================================================

[export]
def init() {
    harness_init("glfw_synth_mouse", 640, 480)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    // Anchored window so SYNTH_BTN's bbox is stable across runs.
    SetNextWindowPos(ImVec2(50.0, 50.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(420.0, 280.0), ImGuiCond.Always)
    window(SYNTH_WIN, (text = "synth mouse demo", closable = false,
                       flags = ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove)) {
        let pos = GetMousePos()
        text("mouse_pos = ({pos.x}, {pos.y})")
        text("left = {IsMouseDown(ImGuiMouseButton.Left)}")
        button(SYNTH_BTN, (text = "      Click me      "))
        text("SYNTH_BTN clicks: {SYNTH_BTN.click_count}")
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
