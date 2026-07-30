options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: visual_aids_dashboard — stage for the 7 [live_command] overlays.
// SHOWS:   The named widgets (TARGET_BTN, TARGET_INPUT, TARGET_CHECK) act as
//          targets for the visual aid overlays in `widgets/imgui_visual_aids.das`:
//            * imgui_highlight     — flash colored rect on target.bbox
//            * imgui_mouse_trail   — fading dotted line behind cursor
//            * imgui_narrate       — callout box w/ optional connector to target
//            * imgui_auto_highlight — auto-flash on every accepted command
//            * imgui_cursor_sprite — synth-cursor pointer sprite
//            * imgui_key_hud       — bottom-center keycap + modifier strip
//            * imgui_focus_rect    — rect around io.active_widget
//          The demo is a passive host; drive overlays via post_command from
//          tests or curl. The bottom STATUS line surfaces interaction state.
// DRIVE:   curl -X POST -d '{"name":"imgui_highlight","args":{"target":"MAIN_WIN/TARGET_BTN","frames":120}}' \
//              localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/visual_aids_dashboard.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/visual_aids_dashboard.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/visual_aids_dashboard.das
// =============================================================================

[export]
def init() {
    harness_init("visual_aids dashboard", 640, 360)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(600.0, 320.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "visual_aids stage", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Targets below — drive imgui_highlight / narrate / focus_rect against them."))

        if (button(TARGET_BTN, (text = "Target button"))) {}
        same_line()
        checkbox(TARGET_CHECK, (text = "Target check"))

        input_text(TARGET_INPUT, (text = "Target input"))

        separator()
        text(STATUS, (text = "btn_clicks={TARGET_BTN.click_count} check={TARGET_CHECK.value}"))
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
