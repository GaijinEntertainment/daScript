options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: layout_helpers — §4.6 split_h / split_v / dock_left.
// SHOWS:   - `split_h(SPLIT_MAIN, init=0.3, ${left}, ${right})` — draggable
//            vertical splitter; state.value is the left fraction (0..1).
//          - `split_v(SPLIT_VERT, init=0.4, ${top}, ${bottom})` — draggable
//            horizontal splitter.
//          - `dock_left(SIDEBAR, init=180.0, bounds=(80.0, 320.0)) { ... }` —
//            sidebar with a draggable right edge; state.value is width (px).
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          curl -X POST -d '{"name":"imgui_force_set",
//                            "args":{"target":"LAYOUT_WIN/SPLIT_MAIN","value":0.7}}' \
//                       localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/layout_helpers.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/layout_helpers.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/layout_helpers.das
// =============================================================================

[export]
def init() {
    harness_init("§4.6 — layout helpers", 800, 600)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(720.0f, 540.0f), ImGuiCond.FirstUseEver)
    window(LAYOUT_WIN, (text = "Layout demo", closable = false,
                        flags = ImGuiWindowFlags.None)) {
        dock_left(SIDEBAR, (init = 180.0f, bounds = (80.0f, 320.0f))) {
            text("Sidebar")
            button(SIDEBAR_BTN, (text = "Sidebar btn"))
        }
        // Main pane flows after dock_left's trailing SameLine. split_v stacks
        // split_h on top of a row of buttons inside it.
        split_v(SPLIT_VERT, (init = 0.5f, bounds = (0.1f, 0.9f)),
                ${
                    split_h(SPLIT_MAIN, (init = 0.3f, bounds = (0.1f, 0.9f)),
                            ${
                                text("Left")
                                button(LEFT_BTN, (text = "Left"))
                            },
                            ${
                                text("Right")
                                button(RIGHT_BTN, (text = "Right"))
                            })
                },
                ${
                    text("Bottom pane")
                    button(BOTTOM_BTN, (text = "Bottom"))
                })
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
