options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: set_window_size — runtime window-size mutation.
// SHOWS:   `set_window_size(float2(w, h), ImGuiCond.Always)` resizes the
//          current/last window mid-frame. Three buttons (Small / Medium /
//          Large) each post a different size; the window snaps to the new
//          dimensions on the next frame and the displayed size readout
//          updates. The `[export] def init` uses SetNextWindowSize with
//          ImGuiCond.FirstUseEver so the runtime mutation isn't overwritten
//          on subsequent frames.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/set_window_size_runtime.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/set_window_size_runtime.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/set_window_size_runtime.das
// =============================================================================

[export]
def init() {
    harness_init("set_window_size runtime mutation", 720, 480)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(480.0, 320.0), ImGuiCond.FirstUseEver)
    window(MAIN_WIN, (text = "set_window_size", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Click a size button to resize this window via set_window_size()."))
        if (button(BTN_SMALL, (text = "Small (320x200)"))) {
            set_window_size(float2(320.0f, 200.0f), ImGuiCond.Always)
        }
        same_line()
        if (button(BTN_MEDIUM, (text = "Medium (480x320)"))) {
            set_window_size(float2(480.0f, 320.0f), ImGuiCond.Always)
        }
        same_line()
        if (button(BTN_LARGE, (text = "Large (640x440)"))) {
            set_window_size(float2(640.0f, 440.0f), ImGuiCond.Always)
        }
        let sz = GetWindowSize()
        text(SIZE_READOUT, (text = "current = ({sz.x}, {sz.y})"))
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
