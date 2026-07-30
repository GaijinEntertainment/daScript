options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: io_mirror — Phase 1.3 io mirror in imgui_snapshot.
// SHOWS:   imgui_snapshot now folds an "io" key alongside "globals":
//            io.mouse_pos     = [x, y] (object {x, y} via JV(float2))
//            io.mouse_buttons = [left, middle, right] (bool array)
//          Read by io_jv() in imgui_boost_runtime; uses public API only
//          (GetMousePos, IsMouseDown). Active-widget resolution deferred —
//          ImGui::GetActiveID() isn't bound on the daslang side yet.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          → top-level "io": {"mouse_pos":{"x":..,"y":..}, "mouse_buttons":[..,..,..]}
// STANDALONE: daslang.exe modules/dasImgui/examples/features/io_mirror.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/io_mirror.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/io_mirror.das
// =============================================================================

[export]
def init() {
    harness_init("Phase 1.3 — io mirror", 800, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(560.0, 240.0), ImGuiCond.Always)
    window(IO_DEMO_WIN, (text = "io mirror demo", closable = false,
                        flags = ImGuiWindowFlags.None)) {
        text("Move the mouse / hold a button — imgui_snapshot.io reflects it.")
        let pos = GetMousePos()
        text("mouse_pos = ({pos.x}, {pos.y})")
        text("mouse_buttons = [{IsMouseDown(ImGuiMouseButton.Left)}, {IsMouseDown(ImGuiMouseButton.Middle)}, {IsMouseDown(ImGuiMouseButton.Right)}]")

        // A button to anchor the snapshot's globals payload.
        button(NUDGE_BTN, (text = "Nudge"))
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
