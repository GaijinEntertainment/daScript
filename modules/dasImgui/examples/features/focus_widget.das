options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: focus_widget — Phase 2.4 keyboard focus dispatch.
// SHOWS:   imgui_focus live_command — server inserts target's path into
//          g_pending_focus; widget_prelude consumes the entry on the
//          target's next render via SetKeyboardFocusHere(0). The example
//          renders a button and an input_text WITHOUT auto-focus, so by
//          default no widget holds keyboard focus.
// DRIVE:   curl -X POST -d '{"name":"imgui_focus","args":{"target":"MAIN_WIN/DEMO_INPUT"}}' \
//              localhost:9090/command
//          curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
//              | jq '.globals."MAIN_WIN/DEMO_INPUT".focus'    # → true within ~1 frame
//          curl -X POST -d '{"name":"imgui_focus","args":{"target":"MAIN_WIN/DEMO_TEXT"}}' \
//              localhost:9090/command                     # → Result.err (display-only)
// STANDALONE: daslang.exe modules/dasImgui/examples/features/focus_widget.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/focus_widget.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/focus_widget.das
// =============================================================================

[export]
def init() {
    harness_init("Phase 2.4 — focus_widget", 800, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 240.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "focus_widget — phase 2.4", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("imgui_focus inserts into g_pending_focus; widget_prelude consumes.")
        text("DEMO_TEXT is display-only — focus dispatch returns Result.err.")
        text_show(DEMO_TEXT, "static label")
        if (button(DEMO_BTN, (text = "I can be focused too"))) {
            print("DEMO_BTN clicked\n")
        }
        input_text(DEMO_INPUT, (text = "Type here"))
        text("buffer = {DEMO_INPUT.value}")
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
