options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: set_scroll_here_y — scroll the parent so a freshly-submitted
//          widget lands at a chosen vertical fraction of the visible area.
// SHOWS:   When JUMP_TARGET changes (via Top / Middle / Bottom buttons), the
//          loop that renders ROW[0..199] inside a 360px child fires
//          `set_scroll_here_y(0.5)` right after drawing ROW[JUMP_TARGET].
//          The child's scroll_y snaps so that row sits mid-viewport on the
//          next frame. Snapshot `MAIN_WIN/SCROLL_AREA.scroll.y` to see it
//          land near the row's static y-offset.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/scroll_here_y.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/scroll_here_y.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/scroll_here_y.das
// =============================================================================

var private ROW : table<int; NarrativeState>
let private ITEM_COUNT = 200
var private g_jump_target = -1

[export]
def init() {
    harness_init("set_scroll_here_y — jump-to-row", 540, 540)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(500.0, 500.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "set_scroll_here_y", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text(LEAD, (text = "Pick a target row; the child scrolls so that row sits mid-viewport."))

        if (button(JUMP_TOP, (text = "Top (row 0)"))) {
            g_jump_target = 0
        }
        same_line()
        if (button(JUMP_MIDDLE, (text = "Middle (row 100)"))) {
            g_jump_target = 100
        }
        same_line()
        if (button(JUMP_BOTTOM, (text = "Bottom (row 199)"))) {
            g_jump_target = 199
        }

        child(SCROLL_AREA, (text = "rows", size = float2(460.0f, 360.0f),
                            child_flags = ImGuiChildFlags.Borders,
                            window_flags = ImGuiWindowFlags.None)) {
            for (i in range(ITEM_COUNT)) {
                text(ROW[i], (text = "row {i}"))
                if (i == g_jump_target) {
                    set_scroll_here_y(0.5f)
                    g_jump_target = -1
                }
            }
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
