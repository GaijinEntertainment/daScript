options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: list_box — unified [container] for BeginListBox/EndListBox. The
//          legacy items-array `[widget] def list_box` was dropped; the modern
//          form is block-arg + user-emitted Selectable rows. Click writes
//          state.value; imgui_force_set steers it from the test harness.
// SHOWS:   list_box scope with Selectable-driven selection state, custom
//          row rendering (icons in front of names), and dynamic content
//          (filterable item list).
// DRIVE:   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/LANG","value":3}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/list_box.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/list_box.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/list_box.das
// =============================================================================

let LANG_NAMES : array<string> <- ["daslang", "C++", "Rust", "Go", "Python", "Lua"]
let LANG_ICONS : array<string> <- ["[D]", "[C]", "[R]", "[G]", "[P]", "[L]"]
var LANG_ROW : table<int; ClickState>

[export]
def init() {
    harness_init("list_box — unified scope form", 720, 540)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(660.0, 480.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "list_box", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("Body emits Selectable rows; click writes LANG.value.")
        separator()

        list_box(LANG, (text = "Language", size = float2(-1.0f, 180.0f))) {
            for (i in range(length(LANG_NAMES))) {
                let is_sel = (i == LANG.value)
                let row_text = "{LANG_ICONS[i]} {LANG_NAMES[i]}"
                if (selectable_label(LANG_ROW[i], row_text, is_sel)) {
                    LANG.value = i
                }
            }
        }

        separator()
        var picked = "(none)"
        if (LANG.value >= 0 && LANG.value < length(LANG_NAMES)) {
            picked = LANG_NAMES[LANG.value]
        }
        text("Picked: {picked} (idx {LANG.value})")
        text("imgui_force_set value=N steers next-frame selection.")
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
