options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require daslib/safe_addr

// =============================================================================
// FEATURE: edit_external_combo — combo dropdown on external int? pointer.
// SHOWS:   edit_combo(safe_addr(idx), (id=…, items <- ARR)) — passes the
//          items as `array<string>`; the C-side reads via the array-of-
//          C-string-pointers Combo overload (no per-frame zero-separated
//          buffer packing — daslang `string` already carries the trailing
//          null). Payload is {value: N} where N is the selected index.
// DRIVE:   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/COMBO_THEME","value":2}}' \
//              localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/edit_external_combo.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/edit_external_combo.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/edit_external_combo.das
// =============================================================================

var private g_theme_idx : int = 1
var private g_difficulty_idx : int = 2

let private g_themes : array<string> <- ["Light", "Dark", "Solarized", "Forge"]
let private g_difficulties : array<string> <- ["Easy", "Normal", "Hard", "Insane"]

[export]
def init() {
    harness_init("edit_external_combo — boost v2 §3.7", 720, 360)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 300.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "edit_combo", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Two combos against caller-owned int? indices."))
        edit_combo(safe_addr(g_theme_idx), (id = "COMBO_THEME",
                                            text = "theme",
                                            items <- g_themes))
        text(STATUS_THEME, (text = "selected theme idx = {g_theme_idx} ({g_themes[g_theme_idx]})"))
        separator(SEP)
        edit_combo(safe_addr(g_difficulty_idx), (id = "COMBO_DIFFICULTY",
                                                 text = "difficulty",
                                                 items <- g_difficulties,
                                                 popup_max_height_in_items = 3))
        text(STATUS_DIFF, (text = "selected difficulty idx = {g_difficulty_idx}"))
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
