options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: triggers — every Phase 0b.1 trigger widget in one window.
// SHOWS:   Button / SmallButton / ArrowButton / InvisibleButton / ImageButton
//          / Checkbox / RadioButton (bool + grouped int) / Selectable / MenuItem
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          curl -X POST -d '{"name":"imgui_click","args":{"target":"MAIN_WIN/SAVE_BTN"}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/OPT_A","value":true}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/DIFFICULTY","value":2}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/triggers.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/triggers.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/triggers.das
// =============================================================================

[export]
def init() {
    harness_init("Phase 0b.1 — triggers", 800, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 560.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "Triggers — phase 0b.1", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        // ----- Click family -----
        text("Click family")
        separator()
        if (button(SAVE_BTN, (text = "Save"))) {
            print("save clicked, count={SAVE_BTN.click_count}\n")
        }
        text("save clicks: {SAVE_BTN.click_count}")
        small_button(QUIT_BTN, (text = "Quit"))
        text("quit clicks: {QUIT_BTN.click_count}")
        same_line()
        arrow_button(BACK_BTN, (text = "back", dir = ImGuiDir.Left))
        same_line()
        arrow_button(FWD_BTN, (text = "fwd", dir = ImGuiDir.Right))
        text("nav clicks: back={BACK_BTN.click_count}, fwd={FWD_BTN.click_count}")
        invisible_button(HOTSPOT, (text = "hotspot", size = float2(120.0f, 24.0f)))
        text("invisible-hotspot clicks: {HOTSPOT.click_count}")

        let io & = unsafe(GetIO())
        if (io.Fonts.TexData != null) {
            image_button(FONT_BTN, (text = "font_atlas", user_texture_id = io.Fonts.TexRef,
                                    size = float2(48.0f, 48.0f)))
            text("image-button clicks: {FONT_BTN.click_count}")
        }

        spacing()

        // ----- Toggle family -----
        text("Toggle family")
        separator()
        checkbox(WIREFRAME, (text = "Wireframe"))
        text("wireframe = {WIREFRAME.value}")
        radio_button(MUTE, (text = "Mute"))
        text("mute = {MUTE.value}")
        selectable(SEL_ITEM, (text = "Selectable item"))
        text("selectable = {SEL_ITEM.value}")
        menu_item(QUICK_SAVE, (text = "Quick save", shortcut = "Ctrl+S"))
        text("quick-save = {QUICK_SAVE.value}")

        spacing()

        // ----- Grouped radios (shared state) -----
        text("Difficulty (radio_button_int — three calls share DIFFICULTY)")
        separator()
        radio_button_int(DIFFICULTY, (text = "Easy", v_button = 0))
        same_line()
        radio_button_int(DIFFICULTY, (text = "Normal", v_button = 1))
        same_line()
        radio_button_int(DIFFICULTY, (text = "Hard", v_button = 2))
        text("difficulty = {DIFFICULTY.value}")
    }

    harness_end_frame()
}

[export]
def shutdown() {
    harness_shutdown()
}

[export]
def main() {
    //! Standalone entrypoint. daslang-live drives init/update/shutdown
    //! directly and ignores main(); plain `daslang.exe <file>` runs this loop.
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
