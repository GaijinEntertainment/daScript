options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: with_disabled — stateless BeginDisabled / EndDisabled scope wrapper.
// SHOWS:   with_disabled(disabled) { ... } greys out and disables every widget
//          rendered inside without state. Same shape as with_id / with_style —
//          no [container] annotation, no telemetry global. Toggle the gate
//          checkbox to flip every child between active and disabled.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/disabled_block.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/disabled_block.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/disabled_block.das
// =============================================================================

[export]
def init() {
    harness_init("with_disabled — boost v2 §3.6", 720, 360)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 300.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "with_disabled", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Toggle the gate to disable every widget inside the block."))
        checkbox(GATE_DISABLED, (text = "disable block below"))
        separator(SEP)

        with_disabled(GATE_DISABLED.value) {
            if (button(BTN_INSIDE, (text = "inside-block button"))) {}
            checkbox(CB_INSIDE, (text = "inside-block checkbox"))
            slider_float(SLIDER_INSIDE, (text = "inside-block slider"))
        }

        separator(SEP_AFTER)
        if (button(BTN_OUTSIDE, (text = "always-enabled button"))) {}
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
