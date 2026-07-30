options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: with_tab_stop — stateless block-arg wrapper around
//          PushTabStop / PopTabStop. The wrapped block's widgets are
//          skipped by TAB / Shift+Tab focus cycling.
// SHOWS:   four input_text rows; the middle one wrapped in
//          with_tab_stop(false) is unreachable via TAB.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/with_tab_stop.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/with_tab_stop.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/with_tab_stop.das
// =============================================================================

[export]
def init() {
    harness_init("with_tab_stop — scope wrapper", 560, 400)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(500.0, 340.0), ImGuiCond.Always)
    window(TS_WIN, (text = "with_tab_stop", closable = false,
                    flags = ImGuiWindowFlags.None)) {
        text("Press TAB / Shift+TAB to cycle focus.")
        input_text(TS_FIELD_A, (text = "Field A", init = "alpha"))
        input_text(TS_FIELD_B, (text = "Field B", init = "beta"))
        with_tab_stop(false) {
            input_text(TS_FIELD_SKIP,
                (text = "Skipped by TAB", init = "(unreachable via keyboard)"))
        }
        input_text(TS_FIELD_C, (text = "Field C", init = "gamma"))
        input_text(TS_FIELD_D, (text = "Field D", init = "delta"))
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
