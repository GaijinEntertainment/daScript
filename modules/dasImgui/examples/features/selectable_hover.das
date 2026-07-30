options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: selectable_hover — hover-target variant of selectable.
//          Pins selected=false, returns per-frame hover bool. No persistent
//          selection state; the row's hover surface drives a side-effect at
//          the call site.
// SHOWS:   five rows; STATE_MESSAGE follows the hovered row.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/selectable_hover.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/selectable_hover.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/selectable_hover.das
// =============================================================================

var private STATE_MESSAGE = "Hover any row."
var private SH_ROW : table<int; EmptyMarkerState>

let private ROWS = fixed_array("Arrow", "TextInput", "ResizeAll", "Hand", "NotAllowed")

[export]
def init() {
    harness_init("selectable_hover — hover target", 520, 320)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(460.0, 260.0), ImGuiCond.Always)
    window(SH_WIN, (text = "selectable_hover", closable = false,
                    flags = ImGuiWindowFlags.None)) {
        for (i in range(length(ROWS))) {
            let hovered = selectable_hover(SH_ROW[i], (text = "Row {i}: {ROWS[i]}"))
            if (hovered) {
                STATE_MESSAGE = "Hovering: {ROWS[i]}"
            }
        }
        separator()
        text(SH_STATUS, (text = STATE_MESSAGE))
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
