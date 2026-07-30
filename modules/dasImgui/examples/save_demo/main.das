options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: save_demo — the spec §7.1 example. Three widgets exercised by
//          test_save_demo (the playwright-API end-to-end happy path):
//            NAME_INPUT   (input_text)  — user types here
//            SAVE_BTN     (button)      — click sets STATUS_TEXT
//            STATUS_TEXT  (text_show)   — reflects "saved" after click
// SHOWS:   First externally-facing test of the public imgui_playwright
//          surface — click + type_text (auto-focus) + expect_value flow.
// DRIVE:   curl -X POST -d '{"name":"imgui_click","args":{"target":"MAIN_WIN/SAVE_BTN"}}' \
//               localhost:9090/command
//          → globals."MAIN_WIN/STATUS_TEXT".payload.value flips to "saved"
//          curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/NAME_INPUT","value":"hello"}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/save_demo/main.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/save_demo/main.das -- --headless
// LIVE:       daslang-live modules/dasImgui/examples/save_demo/main.das
// =============================================================================

[export]
def init() {
    harness_init("save_demo — playwright spec §7.1", 600, 240)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(560.0, 200.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "Save demo", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        input_text(NAME_INPUT, (text = "Name"))
        if (button(SAVE_BTN, (text = "Save"))) {
            STATUS_TEXT.value := "saved"
        }
        text_show(STATUS_TEXT)
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
