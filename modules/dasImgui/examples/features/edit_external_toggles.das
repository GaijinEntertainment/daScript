options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require daslib/safe_addr

// =============================================================================
// FEATURE: edit_external_toggles — bool / radio / flags on external pointers.
// SHOWS:   edit_checkbox(bool?), edit_radio_button(bool?) (flips on click —
//          ImGui's bool form doesn't mutate, the wrapper does), and
//          edit_checkbox_flags(int?) which toggles a specific bit in a
//          shared int mask.
// DRIVE:   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/CB_GATE","value":true}}' \
//              localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/edit_external_toggles.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/edit_external_toggles.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/edit_external_toggles.das
// =============================================================================

var private g_gate : bool = false
var private g_radio : bool = true
var private g_flags : int = 5

[export]
def init() {
    harness_init("edit_external_toggles — boost v2 §3.7", 720, 380)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 320.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "edit_* toggles", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        edit_checkbox(safe_addr(g_gate), (id = "CB_GATE", text = "checkbox (gate)"))
        edit_radio_button(safe_addr(g_radio), (id = "RB_RADIO", text = "radio (flips on click)"))
        separator(SEP)
        text(MASK, (text = "flags mask = {g_flags}"))
        edit_checkbox_flags(safe_addr(g_flags), (id = "CBF_BIT0",
                                                 text = "bit 0 (value 1)", flags_value = 1))
        edit_checkbox_flags(safe_addr(g_flags), (id = "CBF_BIT1",
                                                 text = "bit 1 (value 2)", flags_value = 2))
        edit_checkbox_flags(safe_addr(g_flags), (id = "CBF_BIT2",
                                                 text = "bit 2 (value 4)", flags_value = 4))
        edit_checkbox_flags(safe_addr(g_flags), (id = "CBF_BIT3",
                                                 text = "bit 3 (value 8)", flags_value = 8))
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
