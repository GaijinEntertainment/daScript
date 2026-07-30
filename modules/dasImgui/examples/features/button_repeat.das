options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: with_button_repeat — stateless Push/PopButtonRepeat wrapper.
// SHOWS:   `with_button_repeat(true) { ... }` makes any button inside fire
//          repeatedly while held (typical use: increment/decrement chevron
//          rows in numeric editors). Outside the block, buttons go back to
//          fire-once semantics. Compare BTN_INSIDE.click_count vs
//          BTN_OUTSIDE.click_count while holding each.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/button_repeat.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/button_repeat.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/button_repeat.das
// =============================================================================

[export]
def init() {
    harness_init("with_button_repeat — boost v2 §3.6", 720, 360)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 300.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "with_button_repeat", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Hold the inside button — click_count climbs every frame. The outside one fires once per press."))

        with_button_repeat(true) {
            if (button(BTN_INSIDE, (text = "repeat (hold)"))) {}
            same_line(LAY_GAP)
            text(STATUS_IN, (text = "inside count: {BTN_INSIDE.click_count}"))
        }

        if (button(BTN_OUTSIDE, (text = "fire-once (hold)"))) {}
        same_line(LAY_GAP2)
        text(STATUS_OUT, (text = "outside count: {BTN_OUTSIDE.click_count}"))
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
