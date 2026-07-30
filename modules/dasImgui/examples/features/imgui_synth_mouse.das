options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: imgui_synth_mouse — ImGui-bypass synthetic mouse driver.
// SHOWS:   `imgui_mouse_pos` / `imgui_mouse_click` / `imgui_mouse_move_to` /
//          `imgui_mouse_play` live commands inject events straight into
//          ImGui's input queue, skipping the GLFW chain. `MouseClickedPos`
//          latches at the synth coords because the cursor event is queued
//          immediately before the button event — IsItemHovered() sees the
//          synth target widget.
// DRIVE:   curl -X POST -d '{"name":"imgui_mouse_pos","args":{"x":200,"y":150}}' \
//              localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/imgui_synth_mouse.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/imgui_synth_mouse.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/imgui_synth_mouse.das
// =============================================================================

[export]
def init() {
    harness_init("imgui_synth_mouse", 640, 480)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowPos(ImVec2(50.0, 50.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(420.0, 280.0), ImGuiCond.Always)
    window(SYNTH_WIN, (text = "synth mouse demo (imgui bypass)", closable = false,
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
