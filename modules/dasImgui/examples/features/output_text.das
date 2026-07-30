options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: output_text — Phase 1.4 text_show display widget (master plan §4.3a).
// SHOWS:   text_show(IDENT, value="...") and text_show(IDENT) forms. State
//          is a single string accessible via STATUS.value, surfaced through
//          the standard serializer so `expect_value` assertions work
//          end-to-end. The dispatcher accepts imgui_force_set so tests can drive
//          the value remotely.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          → globals."MAIN_WIN/STATUS_LINE".payload.value = "..." (echoes
//             whatever value is set, by source-init or remote imgui_force_set)
//          curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/STATUS_LINE","value":"hello"}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/output_text.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/output_text.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/output_text.das
// =============================================================================

[export]
def init() {
    harness_init("Phase 1.4 — text_show", 800, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(560.0, 220.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "text_show demo", closable = false,
                     flags = ImGuiWindowFlags.None)) {
        text("Two forms of text_show:")

        // (1) source-init form — every frame state.value resets to the
        // literal "ready"; imgui_force_set still works between frames but the
        // next frame overwrites.
        text_show(BANNER, "ready")

        // (2) display-only form — initial state.value is "" until something
        // (the bump button below or imgui_force_set) writes it.
        text_show(STATUS_LINE)

        // Lets the demo loop touch STATUS_LINE programmatically.
        if (button(BUMP_BTN, (text = "bump status"))) {
            STATUS_LINE.value := "bumped at frame {g_frame}"
        }
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
