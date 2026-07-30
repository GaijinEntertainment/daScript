options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: focus_control — set_keyboard_focus_here + set_item_default_focus.
// SHOWS:   `set_item_default_focus()` after the first input claims focus on
//          window first-open (typing starts there). `set_keyboard_focus_here(0)`
//          before a widget schedules focus to LAND on that widget next frame
//          — clicking REFOCUS_NAME moves focus to NAME_INPUT; REFOCUS_EMAIL
//          moves it to EMAIL_INPUT. Compare against the `focus` field in each
//          input's snapshot payload to see who currently has keyboard focus.
// DRIVE:   curl -X POST -d '{"name":"imgui_click","args":{"target":"MAIN_WIN/REFOCUS_EMAIL"}}' \
//              localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/focus_control.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/focus_control.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/focus_control.das
// =============================================================================

[export]
def init() {
    harness_init("focus_control — keyboard focus rail", 560, 360)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(520.0, 300.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "focus control", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "set_item_default_focus claims NAME_INPUT on first frame; the refocus buttons move it."))

        if (button(REFOCUS_NAME, (text = "Focus Name"))) {
            set_keyboard_focus_here(0)
        }
        input_text(NAME_INPUT, (text = "Name"))
        // First frame only: this input is the default focus target.
        set_item_default_focus()

        if (button(REFOCUS_EMAIL, (text = "Focus Email"))) {
            set_keyboard_focus_here(0)
        }
        input_text(EMAIL_INPUT, (text = "Email"))

        text(STATUS, (text = "name='{NAME_INPUT.value}' email='{EMAIL_INPUT.value}'"))
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
